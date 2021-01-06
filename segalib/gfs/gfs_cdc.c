/*****************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:ファイルシステム
 * Module :CDC関数とのインタフェース
 * File	  :gfs_cdc.c
 * Date   :1994-11-11
 * Version:1.16
 * Auther :M.S.
 *
 * 機能概要:
 *   CDブロック資源の管理とCDC関数を使ったCDブロックへのコマンド発行を行なう。
 *   このモジュールは次のCDブロック資源を管理する。
 *	・絞り
 *	・バッファ区画
 *	・ピックアップ
 *	・データレジスタ（セクタデータ移動、削除も含む）
 *
 *****************************************************************************/

/*****************************************************************************
 *	インクルードファイル
 *****************************************************************************/
#include "sega_gfs.h"
#include "gfs_cdc.h"
#include "gfs_def.h"

/*****************************************************************************
 *	定数マクロ
 *****************************************************************************/
#define SCDQ_COUNT_SEC	60		/* SCDQフラグが1sに立つ回数 */
#define TMOUT_DRV_COUNT	(10 * SCDQ_COUNT_SEC) /* ドライブ関係のタイムアウト */

#define NOBODY	-1			/* 所有者はいない */

#define CDBLK_SWRST	0x00		/* CDブロックソフトリセットしない   */
#define SUBCODE_DECODE	0x00		/* サブコードデコードしない         */
#define USE_MODE2	0x00		/* モード2サブヘッダ認識する        */
#define FORM2_RDRETRY	0x00		/* フォーム2リードリトライしない    */
#define HIGH_SPEED	0x00		/* 倍速                             */
#define NCHG_INIT_FLAG	0x00		/* 初期化フラグ変更あり             */

#define INIT_FLAG       (CDBLK_SWRST|SUBCODE_DECODE|USE_MODE2|FORM2_RDRETRY|\
			 HIGH_SPEED|NCHG_INIT_FLAG)

#define DEF_STBYTIM     0x0000		/* デフォルトタイム(180秒) 	    */
#if	!defined(CHK_RDERR)
#define DEF_ECC         0x04		/* デフォルトECC回数(5回)           */
#define DEF_RETRY       0x0f		/* デフォルトリトライ回数(15回)     */
#else
#define DEF_ECC         0x00		/* デフォルトECC回数(5回)           */
#define DEF_RETRY       0x00		/* デフォルトリトライ回数(15回)     */
#endif

/* セレクタのリセットフラグ */
#define RESET_FLG	0xfc

#define MAX_CMD_PER_EXEC	30	/* ExecServer毎の最大発行コマンド数 */

/* CDC_SetFiltConのcflag */
#define FLTCON_BUF_SET	1		/* 真出力を設定 */
#define FLTCON_FLN_SET	2		/* 偽出力を設定 */

#define CDSTAT_MASK	0x0f		/* 状態部分とりだしマスク */

#define	TRREG_UNUSED	(-1)		/* データ転送レジスタ使用中	*/

/* 40msのタイムアウトループ回数 */
#define SCDQ_TMOUT_COUNT	(12186*2)

/*****************************************************************************
 *	列挙定数
 *****************************************************************************/
/* CDブロックの資源の使用状況、未使用・使用中 */
enum CdbResStat {UNUSED, INUSE};

/* 各サーバータスクのステータス */
enum TaskStatus {
    GFCD_TSK_READY,			/* リクエスト待ち状態 */
    GFCD_TSK_STAT1, GFCD_TSK_STAT2, GFCD_TSK_STAT3, GFCD_TSK_STAT4,
    GFCD_TSK_STAT5
};

/* タスクID */
enum TskId {
    TSK_ID_SETFLT = 0,
    TSK_ID_SETCON,
    TSK_ID_GETLEN,
    TSK_ID_REQDAT,
    TSK_ID_DELSCT,
    TSK_ID_MOVSCT,
    TSK_ID_FLTCON,
    TSK_ID_CHGDIR,
    TSK_ID_END
    };


/*****************************************************************************
 *	処理マクロ
 *****************************************************************************/
#define GFS_WORD_BYTE(word)         ((word) * 2)

#define IS_ROM(toc)	((toc) & 0x40000000)

/* CDブロック管理構造体(GfsCdbMng)アクセスマクロ */
#define CDBMNG_USEBUF(mng, i)	((mng)->use_buf[i])
#define CDBMNG_USEFILT(mng, i)	((mng)->use_filt[i])
#define CDBMNG_USEPU(mng)	((mng)->use_pu)
#define CDBMNG_TRBUFNO(mng)	((mng)->tr_bufno)
#define CDBMNG_PUID(mng)	((mng)->puid)
#define CDBMNG_STAT(mng)	((mng)->stat)
#define CDBMNG_TIMER(mng)	((mng)->timer)
#define CDBMNG_FUNC(mng)	((mng)->func)
#define CDBMNG_OBJ(mng)		((mng)->obj)

/* タスク情報アクセスマクロ */
/* (0) GFCD_SetFilt */
#define TSK_SETFLT_LEN(mng)	((mng)->tsk_setflt.len)
#define TSK_SETFLT_STAT(mng)	((mng)->tsk_setflt.stat)
#define TSK_SETFLT_QELT(mng, i)	((mng)->tsk_setflt.selq[i])
#define TSK_SETFLT_FLT(mng, i)	(TSK_SETFLT_QELT(mng, i).flt)
#define TSK_SETFLT_FMODE(mng, i)	(TSK_SETFLT_QELT(mng, i).fmode)
#define TSK_SETFLT_SUBH(mng, i)	(TSK_SETFLT_QELT(mng, i).subh)
#define TSK_SETFLT_FAD(mng, i)	(TSK_SETFLT_QELT(mng, i).fad)
#define TSK_SETFLT_SNUM(mng, i)	(TSK_SETFLT_QELT(mng, i).snum)
/* (1) GFCD_SetCon */
#define TSK_SETCON_STAT(mng)	((mng)->tsk_setcon.stat)
#define TSK_SETCON_FLT(mng)	((mng)->tsk_setcon.flt)
/* (2) GFCD_GetLenData */
#define TSK_GETLEN_STAT(mng)	((mng)->tsk_getlen.stat)
#define TSK_GETLEN_BUFNO(mng)	((mng)->tsk_getlen.bufno)
#define TSK_GETLEN_SPOS(mng)	((mng)->tsk_getlen.spos)
#define TSK_GETLEN_USCT(mng)	((mng)->tsk_getlen.usct)
#define TSK_GETLEN_CNT(mng)	((mng)->tsk_getlen.cnt)
#define TSK_GETLEN_NSCT(mng)	((mng)->tsk_getlen.nsct)
#define TSK_GETLEN_NBYTE(mng)	((mng)->tsk_getlen.nbyte)
/* (3) GFCD_ReqData */
#define TSK_REQDAT_STAT(mng)	((mng)->tsk_reqdat.stat)
#define TSK_REQDAT_BUFNO(mng)	((mng)->tsk_reqdat.bufno)
#define TSK_REQDAT_SCTPOS(mng)	((mng)->tsk_reqdat.sctpos)
#define TSK_REQDAT_NSCT(mng)	((mng)->tsk_reqdat.nsct)
/* (4) GFCD_DelSctData */
#define TSK_DELSCT_STAT(mng)	((mng)->tsk_delsct.stat)
#define TSK_DELSCT_BUFNO(mng)	((mng)->tsk_delsct.bufno)
#define TSK_DELSCT_SCTPOS(mng)	((mng)->tsk_delsct.sctpos)
#define TSK_DELSCT_NSCT(mng)	((mng)->tsk_delsct.nsct)
/* (5) GFCD_MoveSctData */
#define TSK_MOVSCT_STAT(mng)	((mng)->tsk_movsct.stat)
#define TSK_MOVSCT_DST(mng)	((mng)->tsk_movsct.dst)
#define TSK_MOVSCT_SRC(mng)	((mng)->tsk_movsct.src)
#define TSK_MOVSCT_SPOS(mng)	((mng)->tsk_movsct.spos)
#define TSK_MOVSCT_SNUM(mng)	((mng)->tsk_movsct.snum)
#define TSK_MOVSCT_FMODE(mng)	((mng)->tsk_movsct.fmode)
/* (6) GFCD_ChgDir */
#define TSK_CHGDIR_STAT(mng)	((mng)->tsk_chgdir.stat)
#define TSK_CHGDIR_FID(mng)	((mng)->tsk_chgdir.fid)
#define TSK_CHGDIR_WORK(mng)	((mng)->tsk_chgdir.work)
#define TSK_CHGDIR_NDIR(mng)	((mng)->tsk_chgdir.ndir)
/* (7) GFCD_SetFiltCon */
#define TSK_FLTCON_LEN(mng)	((mng)->tsk_fltcon.len)
#define TSK_FLTCON_STAT(mng)	((mng)->tsk_fltcon.stat)
#define TSK_FLTCON_QELT(mng, i)	((mng)->tsk_fltcon.fconq[i])
#define TSK_FLTCON_FLT(mng, i)		(TSK_FLTCON_QELT(mng, i).flt)
#define TSK_FLTCON_BUF(mng, i)		(TSK_FLTCON_QELT(mng, i).buf)
#define TSK_FLTCON_FLNOUT(mng, i)	(TSK_FLTCON_QELT(mng, i).flnout)

/*****************************************************************************
 *	データ型の定義
 *****************************************************************************/
/* セッション情報 */
typedef union {
    Uint8 nses[4];
    Uint32 xfad;
} GfcdSes;

/* セッション情報(GfcdSes)アクセスマクロ */
#define GFCD_SES_NSES(ses)	((ses)->nses[0])
#define GFCD_SES_SFAD(ses)	((ses)->xfad & 0x00ffffff)


/*****************************************************************************
 *	関数の宣言
 *****************************************************************************/
/* タスク状態判定 */
GFS_LOCAL Bool	gfcd_isIdleTask(Sint32 tsk);

/* タスク実行 */
GFS_LOCAL Sint32	gfcd_doTask(Sint32 tsk, Sint32 *ncmd);

/* SetFiltタスク */
GFS_LOCAL Sint32	gfcd_doSetFiltTsk(Sint32 *ncmd);

/* GFCD_SetConタスク */
GFS_LOCAL Sint32	gfcd_doSetConTsk(Sint32 *ncmd);

/* GFCD_GetLenDataタスク */
GFS_LOCAL Sint32	gfcd_doGetLenDataTsk(Sint32 *ncmd);

/* GFCD_ReqDataタスク */
GFS_LOCAL Sint32	gfcd_doReqDataTsk(Sint32 *ncmd);

/* GFCD_DelSctDataタスク */
GFS_LOCAL Sint32	gfcd_doDelSctDataTsk(Sint32 *ncmd);

/* GFCD_MoveSctDataタスク */
GFS_LOCAL Sint32	gfcd_doMoveSctTsk(Sint32 *ncmd);

/* ChgDirタスク */
GFS_LOCAL Sint32	gfcd_doChgDirTsk(Sint32 *ncmd);

/* SetFiltConタスク */
GFS_LOCAL Sint32	gfcd_doSetFiltConTsk(Sint32 *ncmd);

/* 終了フラグを調べる */
GFS_LOCAL Bool		gfcd_checkEflag(Uint16 mask);

/* CDブロック管理構造体初期化 */
GFS_LOCAL void		gfcd_initCdbMng(void);

/* ステータスを更新する */
GFS_LOCAL void		gfcd_setStat(void);

/*****************************************************************************
 *	変数の定義
 *****************************************************************************/
/* CDブロック管理モジュールの作業領域アドレス */
GfsCdbMng *gfcd_work;

/*****************************************************************************
 *	関数の定義
 *****************************************************************************/
/*****************************************************************************
 *  GFCDモジュールの初期化
 *［入力］
 *  work	GFCDモジュールの作業領域
 *  use_cdbfs	CDファイルシステム使用指定
 *［出力］
 *  なし
 *［関数値］
 *  エラーコード
 *［機能］
 *  
 *［備考］
 *  完了復帰
 *****************************************************************************/
Sint32 GFCD_Init(GfsCdbMng *work, Bool use_cdbfs)
{
    Sint32 tm_count;
    Bool ok_flag;
    Uint32 *toc_tbl;
    Sint32 ret;
    CdcStat stat;
    GfcdSes ses;

    gfcd_work = work;
    gfcd_initCdbMng();
    /* CDブロックファイルシステム用のセレクタとバッファを確保 */
    if (use_cdbfs) {
	CDBMNG_USEBUF(gfcd_work, GFCD_SYS_SEL) = INUSE;
	CDBMNG_USEFILT(gfcd_work, GFCD_SYS_SEL) = INUSE;
    } else {
	CDBMNG_USEBUF(gfcd_work, GFCD_SYS_SEL) = UNUSED;
	CDBMNG_USEFILT(gfcd_work, GFCD_SYS_SEL) = UNUSED;
    }
    GFCD_GetStat(&stat);
    switch (CDC_STAT_STATUS(&stat) & CDSTAT_MASK) {
    case CDC_ST_OPEN:
	return GFCD_ERR_OPEN;
    case CDC_ST_NODISC:
	return GFCD_ERR_NODISC;
    case CDC_ST_ERROR:
	return GFCD_ERR_CDC;
    case CDC_ST_FATAL:
	return GFCD_ERR_FATAL;
    }
    if (CDC_GetHirqReq() & CDC_HIRQ_DCHG) {
	return GFCD_ERR_OPEN;
    }

    /* CDブロックファイルシステムの使用中を考慮した対策	*/
    CDC_AbortFile();

    /* ポーズ状態にさせる */
    /* ２倍速、スタンバイタイム：180s, ECC:5, retry:15回 */
    ret = CDC_CdInit(INIT_FLAG, DEF_STBYTIM, DEF_ECC, DEF_RETRY);
    if (ret != CDC_ERR_OK) {
	return GFCD_ERR_NOCDBLK;
    }

    /* セクタ長の設定	*/
    for (tm_count = 0; ;tm_count++) {
	ret = CDC_SetSctLen(CDC_SLEN_2048, CDC_SLEN_NOCHG);
	if (ret == CDC_ERR_OK) {
	    break;
	}
	if ((ret != CDC_ERR_CMDBUSY)||(tm_count > GFCD_TMOUT_COUNT)) {
	    return GFCD_ERR_NOCDBLK;
	}
    }
    CDBMNG_TIMER(gfcd_work) = GFCD_TMOUT_COUNT;
    while (gfcd_checkEflag(CDC_HIRQ_ESEL) == FALSE) {
	if (--CDBMNG_TIMER(gfcd_work) == 0) {
	    return GFCD_ERR_NOCDBLK;
	}
    }
    CDBMNG_TIMER(gfcd_work) = GFCD_NO_TMOUT;

    /* PEND, BFULのクリア			*/
    CDC_ClrHirqReq(~(CDC_HIRQ_PEND|CDC_HIRQ_BFUL));
    
    /* CDブロック接続チェック＆停止状態待ち	*/
    tm_count = 0;
    ok_flag = FALSE;
    while (ok_flag == FALSE) {
	if (gfcd_checkEflag(CDC_HIRQ_SCDQ) == FALSE) {
	    continue;
	}
	CDC_ClrHirqReq(~CDC_HIRQ_SCDQ);
	++tm_count;
	if (tm_count > TMOUT_DRV_COUNT) {
	    return GFCD_ERR_NOCDBLK;
	}
	GFCD_GetStat(&stat);
	switch (CDC_STAT_STATUS(&stat) & CDSTAT_MASK) {
	case CDC_ST_ERROR:
	    return GFCD_ERR_CDC;
	case CDC_ST_FATAL:
	    return GFCD_ERR_FATAL;
	case CDC_ST_PAUSE:
	case CDC_ST_STANDBY:
	    ok_flag = TRUE;
	    break;
	}
    }
    /* データ転送を終了（DRDY=1待ちで終了を発行すると危険） 		*/
    /* GFS開始後1msec程度待つ必要があるので前方に移動することは不可	*/
    CDC_DataEnd(&ret);

    /* CD-ROMであることのチェック */
    toc_tbl = (Uint32 *)MNG_SECTBUF(gfs_mng_ptr);
    ret = CDC_TgetToc(toc_tbl);
    if (ret != CDC_ERR_OK) {
	/* OPENかNODISCのはずだが、前にチェックしているのでこんなはずはない */
	return GFCD_ERR_NOCDBLK;
    }
    /* 最終セッション第１トラックがデータトラックであることを確認 */
    CDC_GetSes(0, (Uint32 *)&ses);	/* セッション数取得 */
    CDC_GetSes(GFCD_SES_NSES(&ses), (Uint32 *)&ses); /* 最終セッションの情報 */
    if (IS_ROM(toc_tbl[GFCD_SES_NSES(&ses) - 1]) == FALSE) {
	return GFCD_ERR_CDROM;
    }
    /* 全セレクタをクリア */
    ret = CDC_ResetSelector(RESET_FLG, CDC_NUL_SEL);
    CDBMNG_TIMER(gfcd_work) = GFCD_TMOUT_COUNT;
    while (gfcd_checkEflag(CDC_HIRQ_ESEL) == FALSE) {
	if (--CDBMNG_TIMER(gfcd_work) == 0) {
	    return GFCD_ERR_NOCDBLK;
	}
    }
    CDBMNG_TIMER(gfcd_work) = GFCD_NO_TMOUT;
    GFCD_GetStat(&CDBMNG_STAT(gfcd_work));
    return GFCD_ERR_OK;
}


/*****************************************************************************
 *  絞り割り当て
 *［入力］
 *  flt_no	割り当てられた絞り番号の格納先
 *［出力］
 *  
 *［関数値］
 *  
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFCD_AllocFilt(Sint32 *flt_no)
{
    Sint32 i;

    for (i = 0; i < GFS_CDBBUF_NR; ++i) {
	if (CDBMNG_USEFILT(gfcd_work, i) == UNUSED) {
	    CDBMNG_USEFILT(gfcd_work, i) = INUSE;
	    *flt_no = i;
	    return GFCD_ERR_OK;
	}
    }
    *flt_no = -1;
    return GFCD_ERR_NOFILT;
}


/*****************************************************************************
 *  絞り解放
 *［入力］
 *  flt_no	解放する絞りの番号
 *［出力］
 *  
 *［関数値］
 *  
 *［機能］
 *  
 *［備考］
 *  完了復帰
 *****************************************************************************/
Sint32 GFCD_FreeFilt(Sint32 flt_no)
{
    if (flt_no < 0 || GFS_CDBBUF_NR <= flt_no) {
	return GFCD_ERR_RANGE;
    }
    if (CDBMNG_USEFILT(gfcd_work, flt_no) != INUSE) {
	return GFCD_ERR_UNUSE;
    }
    CDBMNG_USEFILT(gfcd_work, flt_no) = UNUSED;
    return GFCD_ERR_OK;
}


/*****************************************************************************
 *  バッファ区画割り当て
 *［入力］
 *  buf_no	割り当てられたバッファ番号の格納先
 *［出力］
 *  
 *［関数値］
 *  
 *［機能］
 *  
 *［備考］
 *  完了復帰
 *****************************************************************************/
Sint32 GFCD_AllocBuf(Sint32 *buf_no)
{
    Sint32 i;

    for (i = 0; i < GFS_CDBBUF_NR; ++i) {
	if (CDBMNG_USEBUF(gfcd_work, i) == UNUSED) {
	    CDBMNG_USEBUF(gfcd_work, i) = INUSE;
	    *buf_no = i;
	    return GFCD_ERR_OK;
	}
    }
    *buf_no = -1;
    return GFCD_ERR_NOBUF;
}


/*****************************************************************************
 *  バッファ区画解放
 *［入力］
 *  buf_no	解放するバッファ区画
 *［出力］
 *  
 *［関数値］
 *  
 *［機能］
 *  割り当てられていたバッファを解放し、含まれていたセクタデータを全削除する。
 *［備考］
 *  完了復帰
 *****************************************************************************/
Sint32 GFCD_FreeBuf(Sint32 buf_no)
{
    Sint32 err;

    if (buf_no < 0 || GFS_CDBBUF_NR <= buf_no) {
	return GFCD_ERR_RANGE;
    }
    if (CDBMNG_USEBUF(gfcd_work, buf_no) != INUSE) {
	return GFCD_ERR_UNUSE;
    }
    /* bufnoの全セクタデータ削除 */
    GFCD_DelSctData(buf_no, 0, CDC_SNUM_END);
    GFCD_SetTimer(GFCD_TMOUT_COUNT);
    for (;;) {
	err = GFCD_ExecServer();
	if (err == GFCD_SVR_TMOUT) {
	    return GFCD_ERR_TMOUT;
	} else if (err == GFCD_SVR_ERROR) {
	    return GFCD_ERR_FATAL;
	}
	if (GFCD_CheckDelSctData(buf_no) == TRUE) {
	    break;
	}
    }
    GFCD_SetTimer(GFCD_NO_TMOUT);
    CDBMNG_USEBUF(gfcd_work, buf_no) = UNUSED;
    return GFCD_ERR_OK;
}


/* 最終セッションの開始FADを取得する */
Sint32 GFCD_GetBaseFad(void)
{
    GfcdSes ses;

    CDC_GetSes(0, (Uint32 *)&ses);	/* セッション数取得 */
    CDC_GetSes(GFCD_SES_NSES(&ses), (Uint32 *)&ses); /* 最終セッションの情報 */
    return GFCD_SES_SFAD(&ses);		/* 開始FAD取得 */
}


/*****************************************************************************
 *  ピックアップ制御権取得
 *［入力］
 *  なし
 *［出力］
 *  なし
 *［関数値］
 *  ユーザID
 *［機能］
 *  ピックアップの制御権が取得できたら、ユーザIDを返す
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFCD_GetPickup(void)
{
    /* ピックアップが未使用であることを確認 */
    if (CDBMNG_USEPU(gfcd_work) == INUSE) {
	return GFCD_ERR_INUSE;
    }
    CDBMNG_USEPU(gfcd_work) = INUSE;
    ++CDBMNG_PUID(gfcd_work);
    if (CDBMNG_PUID(gfcd_work) < 0) {
	CDBMNG_PUID(gfcd_work) = 0;
    }
    return CDBMNG_PUID(gfcd_work);
}


/*****************************************************************************
 *  ピックアップ制御権解放
 *［入力］
 *  uid	ピックアップの制御権を持っているuid
 *［出力］
 *  
 *［関数値］
 *  
 *［機能］
 *  
 *［備考］
 *  絞りの割当とピックアップの制御権を取得していること
 *****************************************************************************/
Sint32 GFCD_UngetPickup(Sint32 puid)
{
    /* ピックアップが使用中であることを確認 */
    if (CDBMNG_USEPU(gfcd_work) == UNUSED) {
	return GFCD_ERR_UNUSE;
    }
    if (CDBMNG_PUID(gfcd_work) != puid) {
	return GFCD_ERR_NOTOWNER;
    }
    /* 未使用状態にする */
    CDBMNG_USEPU(gfcd_work) = UNUSED;
    return GFCD_ERR_OK;
}


/*****************************************************************************
 *  ピックアップの制御権を取得しているか調べる
 *［入力］
 *  puid	ユーザID
 *［出力］
 *  
 *［関数値］
 *  
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFCD_IsPuOwner(Sint32 puid)
{
    if (CDBMNG_USEPU(gfcd_work) == UNUSED) {
	return FALSE;
    }
    return (CDBMNG_PUID(gfcd_work) == puid)? TRUE: FALSE;
}


/*****************************************************************************
 *  絞り設定
 *［入力］
 *  flt		設定先絞りの番号
 *  fmode	絞り条件
 *  subh	サブヘッダ条件
 *［出力］
 *  
 *［関数値］
 *  
 *［機能］
 *  
 *［備考］
 *  絞りの割当を受けていること
 *****************************************************************************/
Sint32 GFCD_SetFilt(Sint32 flt, Sint32 fmode, CdcSubh *subh,
		    Sint32 fad, Sint32 snum)
{
    Sint32 ncmd;
    Sint32 len = TSK_SETFLT_LEN(gfcd_work);

    /* 割り当てられた絞りに対しての操作であることを確認 */
    if (CDBMNG_USEFILT(gfcd_work, flt) != INUSE) {
	return GFCD_ERR_UNUSE;
    }
    /* キューに空きがあることを確認 */
    if (len >= GFS_SELQ_MAX) {
	return GFCD_ERR_QFULL;
    }
    /* キューに登録 */
    if (TSK_SETFLT_STAT(gfcd_work) == GFCD_TSK_READY) {
	TSK_SETFLT_STAT(gfcd_work) = GFCD_TSK_STAT1;
    }
    TSK_SETFLT_FLT(gfcd_work, len) = flt;
    TSK_SETFLT_FMODE(gfcd_work, len) = fmode;
    TSK_SETFLT_SUBH(gfcd_work, len) = *subh;
    TSK_SETFLT_FAD(gfcd_work, len) = fad;
    TSK_SETFLT_SNUM(gfcd_work, len) = snum;
    ++TSK_SETFLT_LEN(gfcd_work);
    gfcd_doSetFiltTsk(&ncmd);
    return GFCD_ERR_OK;
}


/* 絞り取得 */
Sint32 GFCD_GetFilt(Sint32 flt, Sint32 *fmode, CdcSubh *subh)
{
    Sint32 ret;

    ret = CDC_GetFiltMode(flt, fmode);
    if (ret != CDC_ERR_OK) {
	return GFCD_ERR_CDC;
    }
    ret = CDC_GetFiltSubh(flt, subh);
    if (ret != CDC_ERR_OK) {
	return GFCD_ERR_CDC;
    }
    gfcd_setStat();
    return GFCD_ERR_OK;
}


/*****************************************************************************
 *  CDデバイスの接続
 *［入力］
 *  flt	CDデバイスを接続する絞りの番号
 *［出力］
 *  
 *［関数値］
 *  
 *［機能］
 *  
 *［備考］
 *  絞りの割当とピックアップの制御権を取得していること
 *****************************************************************************/
Sint32 GFCD_SetCon(Sint32 flt, Sint32 puid)
{
    Sint32 ncmd;
    
    /* 割り当てられた絞りに対しての操作であることを確認 */
    if (flt != CDC_NUL_SEL && CDBMNG_USEFILT(gfcd_work, flt) != INUSE) {
	return GFCD_ERR_UNUSE;
    }
    /* ピックアップの制御権を取得している絞りでなければエラー */
    if (CDBMNG_PUID(gfcd_work) != puid) {
	return GFCD_ERR_NOTOWNER;
    }
    TSK_SETCON_STAT(gfcd_work) = GFCD_TSK_STAT1;
    TSK_SETCON_FLT(gfcd_work) = flt;
    gfcd_doSetConTsk(&ncmd);
    return GFCD_ERR_OK;
}


/*****************************************************************************
 *  CD再生
 *［入力］
 *  pinfo	再生パラメータ
 *  puid	ピックアップ制御権を取得しているpuid
 *［出力］
 *  
 *［関数値］
 *  
 *［機能］
 *  
 *［備考］
 *  ピックアップの制御権を取得していること
 *****************************************************************************/
Sint32 GFCD_Play(CdcPly *pinfo, Sint32 puid)
{
    Uint16 hirq;
    CdcStat stat;

    /* ピックアップの制御権を取得していなければエラー */
    if (CDBMNG_PUID(gfcd_work) != puid) {
	return GFCD_ERR_NOTOWNER;
    }
    hirq = GFCD_GetStat(&stat);
    if (hirq & CDC_HIRQ_DCHG) {
	return GFCD_ERR_OPEN;
    }
    if ((CDC_STAT_STATUS(&stat) & 0x0f) == CDC_ST_NODISC) {
	return GFCD_ERR_NODISC;
    }
    CDC_ClrHirqReq(~CDC_HIRQ_PEND);
    CDC_CdPlay(pinfo);
    return GFCD_ERR_OK;
}


/*****************************************************************************
 *  区画内データ数取得
 *［入力］
 *  
 *［出力］
 *  
 *［関数値］
 *  
 *［機能］
 *  
 *［備考］
 *  nbyte==NULLでバイト数の出力を抑制できる。
 *****************************************************************************/
Sint32 GFCD_GetLenData(Sint32 bufno, Sint32 spos, Sint32 usct, Sint32 *nsct,
		       Sint32 *nbyte)
{
    Sint32 ncmd;
    
    /* GetLenDataタスクが実行中でないことを確認する */
    if (TSK_GETLEN_STAT(gfcd_work) != GFCD_TSK_READY) {
	return GFCD_ERR_WAIT;
    }
    TSK_GETLEN_STAT(gfcd_work) = GFCD_TSK_STAT1;
    TSK_GETLEN_BUFNO(gfcd_work) = bufno;
    TSK_GETLEN_SPOS(gfcd_work) = spos;
    TSK_GETLEN_USCT(gfcd_work) = usct;
    TSK_GETLEN_CNT(gfcd_work) = 0;
    TSK_GETLEN_NSCT(gfcd_work) = nsct;
    TSK_GETLEN_NBYTE(gfcd_work) = nbyte;
    gfcd_doGetLenDataTsk(&ncmd);
    return GFCD_ERR_OK;
}


/*****************************************************************************
 *  データ転送要求
 *［入力］
 *  
 *［出力］
 *  
 *［関数値］
 *  
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFCD_ReqData(Sint32 bufno, Sint32 sctpos, Sint32 nsct)
{
    Sint32 ncmd;
    
    if (TSK_REQDAT_STAT(gfcd_work) != GFCD_TSK_READY) {
	return GFCD_ERR_WAIT;
    }
    /* データレジスタが使われていないことを確認する */
    if (CDBMNG_TRBUFNO(gfcd_work) != TRREG_UNUSED) {
	return GFCD_ERR_INUSE;
    }
    CDBMNG_TRBUFNO(gfcd_work) = bufno;
    TSK_REQDAT_STAT(gfcd_work) = GFCD_TSK_STAT1;
    TSK_REQDAT_BUFNO(gfcd_work) = bufno;
    TSK_REQDAT_SCTPOS(gfcd_work) = sctpos;
    TSK_REQDAT_NSCT(gfcd_work) = nsct;
    gfcd_doReqDataTsk(&ncmd);
    return GFCD_ERR_OK;
}


/*****************************************************************************
 *  データ転送終了の通知
 *［入力］
 *  
 *［出力］
 *  
 *［関数値］
 *  
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFCD_EndData(Sint32 *nsct)
{
    Sint32 ret;

    /* データレジスタが使用状況を見て、ReqDataが実行されたことを確認する */
    if (CDBMNG_TRBUFNO(gfcd_work) == TRREG_UNUSED) {
	return GFCD_ERR_UNUSE;
    }
    ret = CDC_DataEnd(nsct);
    CDBMNG_TRBUFNO(gfcd_work) = TRREG_UNUSED;
    if (ret != CDC_ERR_OK) {
	return GFCD_ERR_CDC;
    }
    gfcd_setStat();
    return GFCD_ERR_OK;
}


/*****************************************************************************
 *  バッファデータ消去
 *［入力］
 *	bufno	バッファ番号
 *	sctpos	消去先頭セクタ位置
 *	nsct	消去セクタ数
 *［出力］
 *  
 *［関数値］
 *  
 *［機能］
 *  nsctがCDC_SNUM_ENDの時は最終セクタまでのセクタ数を表す。
 *［備考］
 *  消去の完了チェックはGFCD_CheckDelSctDataを使うこと。
 *****************************************************************************/
Sint32 GFCD_DelSctData(Sint32 bufno, Sint32 sctpos, Sint32 nsct)
{
    Sint32 ncmd;
    
    if (TSK_DELSCT_STAT(gfcd_work) != GFCD_TSK_READY) {
	return GFCD_ERR_WAIT;
    }
    /* データ転送中でないことを確認する */
    if (CDBMNG_TRBUFNO(gfcd_work) == bufno) {
	return GFCD_ERR_INUSE;		/* 使ってる方のバグ */
    }
    TSK_DELSCT_STAT(gfcd_work) = GFCD_TSK_STAT1;
    TSK_DELSCT_BUFNO(gfcd_work) = bufno;
    TSK_DELSCT_SCTPOS(gfcd_work) = sctpos;
    TSK_DELSCT_NSCT(gfcd_work) = nsct;
    gfcd_doDelSctDataTsk(&ncmd);
    return GFCD_ERR_OK;
}


/*****************************************************************************
 *  バッファデータ消去チェック
 *［入力］
 *  
 *［出力］
 *  
 *［関数値］
 *  
 *［機能］
 *   消去処理が終っていればTRUE、そうでなければFALSEを返す。
 *   消去処理が行なわれていない場合は終ったとみなす。
 *  
 *［備考］
 *  
 *****************************************************************************/
Bool GFCD_CheckDelSctData(Sint32 bufno)
{
    /* bufnoに対してのDELSCTタスクの存在チェック */
    if (TSK_DELSCT_STAT(gfcd_work) != GFCD_TSK_READY &&
	TSK_DELSCT_BUFNO(gfcd_work) == bufno) {
	return FALSE;
    }
    return TRUE;
}



/* CDステータス取得 */
Uint16 GFCD_GetStat(CdcStat *stat)
{
    Sint32 ret;
    Uint16 hirq;
    CdcPos	pos;

    if (stat != NULL) {
	ret = CDC_GetPeriStat(stat);
	if (ret != CDC_ERR_OK) {
	    CDC_GetCurStat(stat);
	}
    }
    hirq = CDC_GetHirqReq();
    if ((CDC_STAT_STATUS(stat) & 0x0f) == CDC_ST_FATAL) {
	if ((hirq & CDC_HIRQ_DCHG) == 0) {
	    CDC_POS_PTYPE(&pos) = CDC_PTYPE_DFL;
	    CDC_CdSeek(&pos);
	    /* SCDQフラグを2回待つ	*/
	    GFCD_WaitScdqFlag();
	    GFCD_WaitScdqFlag();
	}
    }
    return hirq;
}


/* ピックアップの移動 */
Sint32 GFCD_MovePickup(CdcPos *pos, Sint32 puid)
{
    Uint16 hirq;
    CdcStat stat;

    /* ピックアップの制御権を取得していなければエラー */
    if (CDBMNG_PUID(gfcd_work) != puid) {
	return GFCD_ERR_NOTOWNER;
    }
    hirq = GFCD_GetStat(&stat);
    if (hirq & CDC_HIRQ_DCHG) {
	return GFCD_ERR_OPEN;
    }
    if ((CDC_STAT_STATUS(&stat) & 0x0f) == CDC_ST_NODISC) {
	return GFCD_ERR_NODISC;
    }
    CDC_CdSeek(pos);
    return GFCD_ERR_OK;
}



/* バッファデータの移動 */
Sint32 GFCD_MoveSctData(Sint32 dst, Sint32 src, Sint32 spos, Sint32 snum)
{
    Sint32 ncmd;
    
    if (CDBMNG_USEFILT(gfcd_work, dst) == UNUSED ||
	CDBMNG_USEFILT(gfcd_work, src) == UNUSED) {
	return GFCD_ERR_UNUSE;
    }
    if (TSK_MOVSCT_STAT(gfcd_work) != GFCD_TSK_READY) {
	return GFCD_ERR_WAIT;
    }
    TSK_MOVSCT_STAT(gfcd_work) = GFCD_TSK_STAT1;
    TSK_MOVSCT_DST(gfcd_work) = dst;
    TSK_MOVSCT_SRC(gfcd_work) = src;
    TSK_MOVSCT_SPOS(gfcd_work) = spos;
    TSK_MOVSCT_SNUM(gfcd_work) = snum;
    gfcd_doMoveSctTsk(&ncmd);
    return GFCD_ERR_OK;
}


/*****************************************************************************
 *  移動完了のチェック
 *［入力］
 *  
 *［出力］
 *  
 *［関数値］
 *  
 *［機能］
 *   セクタデータ移動タスクが終了しているかチェックする。
 *   セクタデータ移動タスクがなければ終了しているとみなす。
 *［備考］
 *  
 *****************************************************************************/
Bool GFCD_CheckMove(void)
{
    if (TSK_MOVSCT_STAT(gfcd_work) != GFCD_TSK_READY) {
	return FALSE;
    }
    return TRUE;
}



/* セクタ情報の取得 */
Sint32 GFCD_GetSctInfo(Sint32 bufno, Sint32 sn, CdcSct *info)
{
    Sint32 ret;

    if (bufno < 0 || GFS_CDBBUF_NR <= bufno) {
	return GFCD_ERR_RANGE;
    }
    if (CDBMNG_USEBUF(gfcd_work, bufno) == UNUSED) {
	return GFCD_ERR_UNUSE;
    }
    ret = CDC_GetSctInfo(bufno, sn, info);
    if (ret != CDC_ERR_OK) {
	return GFCD_ERR_CDC;
    }
    gfcd_setStat();
    return GFCD_ERR_OK;
}



/* カレントディレクトリの設定 */
Sint32 GFCD_ChgDir(Sint32 fid, Sint32 work, Sint32 *ndir)
{
    Sint32 ncmd;
    
    /* セレクタが使用可能かチェック */
    if (CDBMNG_USEFILT(gfcd_work, work) != INUSE ||
	CDBMNG_USEBUF(gfcd_work, work) != INUSE) {
	return GFCD_ERR_INUSE;
    }
    if (TSK_CHGDIR_STAT(gfcd_work) != GFCD_TSK_READY) {
	return GFCD_ERR_WAIT;
    }
    /* タスクを登録 */
    if (fid == CDC_NUL_FID) {
	TSK_CHGDIR_STAT(gfcd_work) = GFCD_TSK_STAT3;
    } else {
	TSK_CHGDIR_STAT(gfcd_work) = GFCD_TSK_STAT1;
    }
    TSK_CHGDIR_FID(gfcd_work) = fid;
    TSK_CHGDIR_WORK(gfcd_work) = work;
    TSK_CHGDIR_NDIR(gfcd_work) = ndir;
    gfcd_doChgDirTsk(&ncmd);
    return GFCD_ERR_OK;
}



/*****************************************************************************
 *  絞りとバッファの接続
 *［入力］
 *  
 *［出力］
 *  
 *［関数値］
 *  
 *［機能］
 *   flnoutがGFCD_ANY_FLTなら偽出力コネクタの設定を変更しない
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFCD_SetFiltCon(Sint32 flt, Sint32 buf, Sint32 flnout)
{
    Sint32 ncmd;
    Sint32 len = TSK_FLTCON_LEN(gfcd_work);

    /* 範囲チェック */
    if ((flt != CDC_NUL_SEL && flt != GFCD_ANY_FLT &&
	 (flt < 0 || GFS_CDBBUF_NR <= flt)) ||
	(buf < 0 || GFS_CDBBUF_NR <= buf) ||
	(flnout != CDC_NUL_SEL && flnout != GFCD_ANY_FLT &&
	 (flnout < 0 || GFS_CDBBUF_NR <= flnout))) {
	return GFCD_ERR_RANGE;
    }
    /* 割り当てられた資源に対しての処理であることを確認 */
    if ((flt != CDC_NUL_SEL && flt != GFCD_ANY_FLT &&
	 CDBMNG_USEFILT(gfcd_work, flt) != INUSE) ||
	(flnout != CDC_NUL_SEL && flnout != GFCD_ANY_FLT &&
	 CDBMNG_USEFILT(gfcd_work, flnout) != INUSE) ||
	CDBMNG_USEBUF(gfcd_work, buf) != INUSE) {
	return GFCD_ERR_UNUSE;
    }
    /* キューに空きがあることを確認 */
    if (len >= GFS_FCONQ_MAX) {
	return GFCD_ERR_QFULL;
    }
    /* タスクの登録 */
    if (TSK_FLTCON_STAT(gfcd_work) == GFCD_TSK_READY) {
	TSK_FLTCON_STAT(gfcd_work) = GFCD_TSK_STAT1;
    }
    TSK_FLTCON_FLT(gfcd_work, len) = flt;
    TSK_FLTCON_BUF(gfcd_work, len) = buf;
    TSK_FLTCON_FLNOUT(gfcd_work, len) = flnout;
    ++TSK_FLTCON_LEN(gfcd_work);
    gfcd_doSetFiltConTsk(&ncmd);
    return GFCD_ERR_OK;
}


/* ファイル情報取得 */
Sint32 GFCD_GetFileInfo(Sint32 fid, CdcFile *finfo)
{
    Sint32 ret;
    Sint32 base_fid, infnum;
    Bool drend;

    ret = CDC_GetFileScope(&base_fid, &infnum, &drend);
    if (ret != CDC_ERR_OK) {
	return GFCD_ERR_CDBFS;
    }
    /* fidが有効範囲にはいっているか */
    if (fid < base_fid || base_fid + infnum <= fid) {
	ret = CDC_ReadDir(GFCD_SYS_SEL, fid);
	if (ret != CDC_ERR_OK) {
	    return GFCD_ERR_CDBFS;	/* エラー */
	}
	/* ReadDir終了待ち */
	while (gfcd_checkEflag(CDC_HIRQ_EFLS) == FALSE) {
	}
	ret = CDC_GetFileScope(&base_fid, &infnum, &drend);
	if (ret != CDC_ERR_OK ||
	    (fid < base_fid && base_fid + infnum <= fid)) {
	    return GFCD_ERR_CDBFS;
	}
    }
    /* ファイル情報取得 */
    ret = CDC_TgetFileInfo(fid, finfo);
    if (ret != CDC_ERR_OK) {
	return GFCD_ERR_CDBFS;
    }
    /* ディレクトリの最後か？ */
    if (drend && (base_fid + infnum - 1 == fid)) {
	CDC_FILE_ATR(finfo) |= GFS_ATR_END_TBL;
    }
    gfcd_setStat();
    return GFCD_ERR_OK;
}


/* バッファの空きセクタ数取得 */
Sint32 GFCD_GetBufSiz(void)
{
    Sint32	ret;
    Sint32      bufsiz, partsiz, freesiz;   /* セクタ単位   */

    ret = CDC_GetBufSiz(&bufsiz, &partsiz, &freesiz);
    if (ret != CDC_ERR_OK) {
	return GFCD_ERR_CDC;
    }
    gfcd_setStat();
    return freesiz;
}


/*****************************************************************************
 *  CDブロックとの通信
 *［入力］
 *  なし
 *［出力］
 *  なし
 *［関数値］
 * 残りタスクの状態
 *	GFCD_SVR_COMPLETED	実行中のタスクはない
 *	GFCD_SVR_BUSY		実行中のタスクが存在する
 *	GFCD_SVR_ERROR		続行できないCDのエラーが発生した
 *	GFCD_SVR_TMOUT		タイムアウト
 *［機能］
 *   CDブロックの状態を監視しながら、必要なCDコマンドを発行する。
 *   いくつかの設定をCDブロックに対して行なった時、それらがすべて完了したことを
 * 関数値によって調べることができる。(逆に、個別には調べられない。)
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFCD_ExecServer(void)
{
    Sint32 ncmd = 0;			/* 発行したコマンドの数 */
    Sint32 ntsk = 0;			/* 残りタスク数 */
    Sint32 tsk;
    Sint32 ret;
    CdcStat stat;

    if (CDBMNG_TIMER(gfcd_work) != GFCD_NO_TMOUT) {
	if (--CDBMNG_TIMER(gfcd_work) <= 0) {
	    /* タイムアウトの時はCDブロック管理構造体初期化 */
	    gfcd_initCdbMng();
	    return GFCD_SVR_TMOUT;
	}
    }
    if (gfcd_checkEflag(CDC_HIRQ_SCDQ) == TRUE) {
	CDC_ClrHirqReq(~CDC_HIRQ_SCDQ);
	GFCD_GetStat(&CDBMNG_STAT(gfcd_work));
	if (CDBMNG_FUNC(gfcd_work) != NULL) {
	    CDBMNG_FUNC(gfcd_work)(CDBMNG_OBJ(gfcd_work));
	}
    } else {
	GFCD_GetStat(&stat);
	if ((CDC_STAT_STATUS(&stat) & 0x0f) == CDC_ST_FATAL) {
	    return GFCD_SVR_ERROR;
	}
	CDBMNG_STAT(gfcd_work) = stat;
    }
    /* 各タスク実行 */
    for (tsk = 0; tsk < TSK_ID_END; ++tsk) {
	if (gfcd_isIdleTask(tsk)) {
	    continue;
	}
	ret = gfcd_doTask(tsk, &ncmd);
	if (ret < 0) {
	    return ret;
	}
	ntsk += ret;
	if (ncmd > MAX_CMD_PER_EXEC) {
	    return GFCD_SVR_BUSY;
	}
    }
    return (ntsk > 0)? GFCD_SVR_BUSY: GFCD_SVR_COMPLETED;
}



/* タスク状態判定 */
GFS_LOCAL Bool 	gfcd_isIdleTask(Sint32 tsk)
{
    Sint32 stat;

    switch (tsk) {
    case TSK_ID_SETFLT:
	stat = TSK_SETFLT_STAT(gfcd_work);
	break;
    case TSK_ID_SETCON:
	stat = TSK_SETCON_STAT(gfcd_work);
	break;
    case TSK_ID_GETLEN:
	stat = TSK_GETLEN_STAT(gfcd_work);
	break;
    case TSK_ID_REQDAT:
	stat = TSK_REQDAT_STAT(gfcd_work);
	break;
    case TSK_ID_DELSCT:
	stat = TSK_DELSCT_STAT(gfcd_work);
	break;
    case TSK_ID_MOVSCT:
	stat = TSK_MOVSCT_STAT(gfcd_work);
	break;
    case TSK_ID_FLTCON:
	stat = TSK_FLTCON_STAT(gfcd_work);
	break;
    case TSK_ID_CHGDIR:
	stat = TSK_CHGDIR_STAT(gfcd_work);
	break;
    }
    return (stat == GFCD_TSK_READY)? TRUE: FALSE;
}


/* タスク実行 */
GFS_LOCAL Sint32	gfcd_doTask(Sint32 tsk, Sint32 *ncmd)
{
    switch (tsk) {
    case TSK_ID_SETFLT:
	return gfcd_doSetFiltTsk(ncmd);
    case TSK_ID_SETCON:
	return gfcd_doSetConTsk(ncmd);
    case TSK_ID_GETLEN:
	return gfcd_doGetLenDataTsk(ncmd);
    case TSK_ID_REQDAT:
	return gfcd_doReqDataTsk(ncmd);
    case TSK_ID_DELSCT:
	return gfcd_doDelSctDataTsk(ncmd);
    case TSK_ID_MOVSCT:
	return gfcd_doMoveSctTsk(ncmd);
    case TSK_ID_FLTCON:
	return gfcd_doSetFiltConTsk(ncmd);
    case TSK_ID_CHGDIR:
	return gfcd_doChgDirTsk(ncmd);
    }
}


/* タイムアウト設定 */
GFS_LOCAL void GFCD_SetTimer(Sint32 tm_count)
{
    CDBMNG_TIMER(gfcd_work) = tm_count;
}


/* GFCD_ExecServerのタスクが終了するのを待つ */
Sint32 GFCD_WaitServer(void)
{
    Sint32 err;

    GFCD_SetTimer(GFCD_TMOUT_COUNT);
    for (;;) {
	err = GFCD_ExecServer();
	if (err != GFCD_SVR_BUSY) {
	    break;
	}
    }
    GFCD_SetTimer(GFCD_NO_TMOUT);
    return err;
}


/* SetFiltタスク */
GFS_LOCAL Sint32	gfcd_doSetFiltTsk(Sint32 *ncmd)
{
    Sint32 i, j;
    Sint32 ret;

    for (i = 0; i < TSK_SETFLT_LEN(gfcd_work); ++i) {
	if (TSK_SETFLT_STAT(gfcd_work) == GFCD_TSK_STAT1) {
	    /* 絞りモード設定 */
	    ret = CDC_SetFiltMode(TSK_SETFLT_FLT(gfcd_work, i),
				  TSK_SETFLT_FMODE(gfcd_work, i));
	    if (ret != CDC_ERR_OK) {
		break;
	    }
	    gfcd_setStat();
	    ++*ncmd;
	    TSK_SETFLT_STAT(gfcd_work) = GFCD_TSK_STAT2;
	}
	if (TSK_SETFLT_STAT(gfcd_work) == GFCD_TSK_STAT2) {
	    /* 絞り条件設定 */
	    ret = CDC_SetFiltSubh(TSK_SETFLT_FLT(gfcd_work, i),
				  &TSK_SETFLT_SUBH(gfcd_work, i));
	    if (ret != CDC_ERR_OK) {
		break;
	    }
	    gfcd_setStat();
	    ++*ncmd;
	    if (TSK_SETFLT_SNUM(gfcd_work, i) != 0) {
		TSK_SETFLT_STAT(gfcd_work) = GFCD_TSK_STAT3;
	    } else {
		TSK_SETFLT_STAT(gfcd_work) = GFCD_TSK_STAT1;
	    }
	}
	if (TSK_SETFLT_STAT(gfcd_work) == GFCD_TSK_STAT3) {
	    /* 絞りFAD範囲の設定 */
	    ret = CDC_SetFiltRange(TSK_SETFLT_FLT(gfcd_work, i),
				   TSK_SETFLT_FAD(gfcd_work, i),
				   TSK_SETFLT_SNUM(gfcd_work, i));
	    if (ret != CDC_ERR_OK) {
		break;
	    }
	    gfcd_setStat();
	    ++*ncmd;
	    TSK_SETFLT_STAT(gfcd_work) = GFCD_TSK_STAT1;
	}
    }
    /* キューをつめる */
    if (i >= TSK_SETFLT_LEN(gfcd_work)) {
	TSK_SETFLT_LEN(gfcd_work) = 0;
    } else {
	/* まだ残りがある */
	for (j = 0; i < TSK_SETFLT_LEN(gfcd_work); ++i, ++j) {
	    TSK_SETFLT_QELT(gfcd_work, j) = TSK_SETFLT_QELT(gfcd_work, i);
	}
	TSK_SETFLT_LEN(gfcd_work) = j;
    }
    if (gfcd_checkEflag(CDC_HIRQ_ESEL)) {
	if (TSK_SETFLT_LEN(gfcd_work) == 0) {
	    TSK_SETFLT_STAT(gfcd_work) = GFCD_TSK_READY;
	}
    }
    return TSK_SETFLT_LEN(gfcd_work);
}


/* GFCD_SetConタスク */
GFS_LOCAL Sint32	gfcd_doSetConTsk(Sint32 *ncmd)
{
    Sint32 ret;

    if (TSK_SETCON_STAT(gfcd_work) == GFCD_TSK_STAT1) {
	ret = CDC_CdSetCon(TSK_SETCON_FLT(gfcd_work));
	++*ncmd;
	if (ret == CDC_ERR_OK) {
	    TSK_SETCON_STAT(gfcd_work) = GFCD_TSK_STAT2;
	}
	gfcd_setStat();
    }
    if (TSK_SETCON_STAT(gfcd_work) == GFCD_TSK_STAT2) {
	if (gfcd_checkEflag(CDC_HIRQ_ESEL)) {
	    TSK_SETCON_STAT(gfcd_work) = GFCD_TSK_READY;
	}
    }
    return TSK_SETCON_STAT(gfcd_work);
}


/* GFCD_GetLenDataタスク */
GFS_LOCAL Sint32 gfcd_doGetLenDataTsk(Sint32 *ncmd)
{
    Sint32	ret;
    Sint32	actwsiz, nsct, st;

    if (TSK_GETLEN_STAT(gfcd_work) == GFCD_TSK_STAT1) {
	ret = CDC_GetSctNum(TSK_GETLEN_BUFNO(gfcd_work), &nsct);
	gfcd_setStat();
	++*ncmd;
	nsct -= TSK_GETLEN_SPOS(gfcd_work);
	nsct =  MIN(nsct, TSK_GETLEN_USCT(gfcd_work));
	*TSK_GETLEN_NSCT(gfcd_work) = nsct;
	/* バイト数を求める必要があるか調べる */
	if (TSK_GETLEN_NBYTE(gfcd_work) == NULL) {
	    TSK_GETLEN_STAT(gfcd_work) = GFCD_TSK_READY;
	    return GFCD_TSK_READY;
	}
	/* セクタ数が０ならバイト数を求める必要はない */
	if (nsct <= 0) {
	    TSK_GETLEN_STAT(gfcd_work) = GFCD_TSK_READY;
	    *TSK_GETLEN_NBYTE(gfcd_work) = 0;
	    return GFCD_TSK_READY;
	}
	TSK_GETLEN_STAT(gfcd_work) = GFCD_TSK_STAT2;
    }
    if (TSK_GETLEN_STAT(gfcd_work) ==  GFCD_TSK_STAT2) {
	if (CDBMNG_TRBUFNO(gfcd_work) != TRREG_UNUSED) {
	    return GFCD_TSK_STAT2;
	}
	CDBMNG_TRBUFNO(gfcd_work) = TSK_GETLEN_BUFNO(gfcd_work);
	ret = CDC_CalActSiz(TSK_GETLEN_BUFNO(gfcd_work),
			    TSK_GETLEN_SPOS(gfcd_work),
			    *TSK_GETLEN_NSCT(gfcd_work));
	if (ret != CDC_ERR_OK) {
	    return GFCD_TSK_STAT2;
	}
	gfcd_setStat();
	++*ncmd;
	TSK_GETLEN_STAT(gfcd_work) = GFCD_TSK_STAT3;
    }
    if (TSK_GETLEN_STAT(gfcd_work) == GFCD_TSK_STAT3) {
	ret = CDC_GetActSiz(&actwsiz);
	if (ret != CDC_ERR_OK) {
	    return GFCD_TSK_STAT1;
	}
	gfcd_setStat();
	++*ncmd;
	TSK_GETLEN_STAT(gfcd_work) = GFCD_TSK_READY;
	*TSK_GETLEN_NBYTE(gfcd_work) =  GFS_WORD_BYTE(actwsiz);
	CDBMNG_TRBUFNO(gfcd_work) = TRREG_UNUSED;
	return GFCD_TSK_READY;
    }
    return GFCD_TSK_STAT1;
}


/* GFCD_ReqDataタスク */
GFS_LOCAL Sint32 gfcd_doReqDataTsk(Sint32 *ncmd)
{
    Sint32 ret;

    if (TSK_REQDAT_STAT(gfcd_work) == GFCD_TSK_STAT1) {
	ret = CDC_GetSctData(TSK_REQDAT_BUFNO(gfcd_work),
			     TSK_REQDAT_SCTPOS(gfcd_work),
			     TSK_REQDAT_NSCT(gfcd_work));
	if (ret != CDC_ERR_OK) {
	    return GFCD_TSK_STAT1;
	}
	gfcd_setStat();
	++*ncmd;
	TSK_REQDAT_STAT(gfcd_work) = GFCD_TSK_STAT2;
    }
    if (TSK_REQDAT_STAT(gfcd_work) == GFCD_TSK_STAT2) {
	ret = CDC_DataReady(CDC_DRDY_GET); /* 転送可能チェック */
	if (ret == CDC_ERR_OK) {
	    TSK_REQDAT_STAT(gfcd_work) = GFCD_TSK_READY;
	    return GFCD_TSK_READY;
	}
    }
    return GFCD_TSK_STAT1;
}


/* GFCD_DelSctDataタスク */
GFS_LOCAL Sint32 gfcd_doDelSctDataTsk(Sint32 *ncmd)
{
    Sint32 ret;
    Sint32 nsct;
    Bool tsk_stat;			/* タスクの実行状態 */

    tsk_stat = TRUE;		/* タスクは実行中 */
    if (TSK_DELSCT_STAT(gfcd_work) == GFCD_TSK_STAT1) {
	if (TSK_DELSCT_SCTPOS(gfcd_work) == 0 &&
	    TSK_DELSCT_NSCT(gfcd_work) == CDC_SNUM_END) {
	    /* 区画の全データが対象の時 */
	    ret = CDC_ResetSelector(0, TSK_DELSCT_BUFNO(gfcd_work));
	    if (ret != CDC_ERR_OK) {
		return GFCD_TSK_STAT1;
	    }
	    gfcd_setStat();
	    ++*ncmd;
	    TSK_DELSCT_STAT(gfcd_work) = GFCD_TSK_STAT4;
	} else {
	    nsct = TSK_DELSCT_NSCT(gfcd_work);
	    if (nsct == 0) {
		/* 区画内セクタ数が０の時 */
		tsk_stat = FALSE;
	    } else {
		TSK_DELSCT_STAT(gfcd_work) = GFCD_TSK_STAT2;
	    }
	}
    }
    if (TSK_DELSCT_STAT(gfcd_work) ==GFCD_TSK_STAT2) {
	ret = CDC_DelSctData(TSK_DELSCT_BUFNO(gfcd_work),
			     TSK_DELSCT_SCTPOS(gfcd_work),
			     TSK_DELSCT_NSCT(gfcd_work));
	++*ncmd;
	if (ret == CDC_ERR_OK) {
	    TSK_DELSCT_STAT(gfcd_work) = GFCD_TSK_STAT3;
	    gfcd_setStat();
	}
    }
    if (TSK_DELSCT_STAT(gfcd_work) == GFCD_TSK_STAT3) {
	/* DelSctの終了をチェック */
	if (gfcd_checkEflag(CDC_HIRQ_EHST)) {
	    tsk_stat = FALSE;
	}
    }
    if (TSK_DELSCT_STAT(gfcd_work) == GFCD_TSK_STAT4) {
	/* ResetSelectorの終了チェック */
	if (gfcd_checkEflag(CDC_HIRQ_ESEL)) {
	    tsk_stat = FALSE;
	}
    }
    if (tsk_stat == TRUE) {
	return GFCD_TSK_STAT1;
    }
    CDC_ClrHirqReq(~CDC_HIRQ_BFUL);		/* BFULクリア */
    TSK_DELSCT_STAT(gfcd_work) = GFCD_TSK_READY;
    return GFCD_TSK_READY;
}


/* GFCD_MoveSctDataタスク */
GFS_LOCAL Sint32 gfcd_doMoveSctTsk(Sint32 *ncmd)
{
    Sint32 ret;

    if (TSK_MOVSCT_STAT(gfcd_work) == GFCD_TSK_STAT1) {
	/* 現在の絞り条件を退避	*/
	ret = CDC_GetFiltMode(TSK_MOVSCT_DST(gfcd_work),
			      &TSK_MOVSCT_FMODE(gfcd_work));
	++*ncmd;
	gfcd_setStat();
	/* 絞り条件を全て無効にする */
	ret = CDC_SetFiltMode(TSK_MOVSCT_DST(gfcd_work), 0);
	if (ret != CDC_ERR_OK) {
	    return GFCD_TSK_STAT1;
	}
	gfcd_setStat();
	++*ncmd;
	TSK_MOVSCT_STAT(gfcd_work) = GFCD_TSK_STAT2;
    }
    if (TSK_MOVSCT_STAT(gfcd_work) == GFCD_TSK_STAT2) {
	/* セクタデータを移動する */
	ret = CDC_MoveSctData(TSK_MOVSCT_SRC(gfcd_work),
			      TSK_MOVSCT_SPOS(gfcd_work),
			      TSK_MOVSCT_SNUM(gfcd_work),
			      TSK_MOVSCT_DST(gfcd_work));
	if (ret != CDC_ERR_OK) {
	    return GFCD_TSK_STAT1;
	}
	gfcd_setStat();
	++*ncmd;
	TSK_MOVSCT_STAT(gfcd_work) = GFCD_TSK_STAT3;
    }
    if (TSK_MOVSCT_STAT(gfcd_work) == GFCD_TSK_STAT3) {
	/* コピー処理の終了チェック */
	if (gfcd_checkEflag(CDC_HIRQ_ECPY)) {
	    TSK_MOVSCT_STAT(gfcd_work) = GFCD_TSK_STAT4;
	}
    }
    if (TSK_MOVSCT_STAT(gfcd_work) == GFCD_TSK_STAT4) {
	/* 絞り条件を復帰する */
	ret = CDC_SetFiltMode(TSK_MOVSCT_DST(gfcd_work),
			      TSK_MOVSCT_FMODE(gfcd_work));
	if (ret != CDC_ERR_OK) {
	    return GFCD_TSK_STAT1;
	}
	++*ncmd;
	TSK_MOVSCT_STAT(gfcd_work) = GFCD_TSK_STAT5;
    }
    if (TSK_MOVSCT_STAT(gfcd_work) == GFCD_TSK_STAT5) {
	if (gfcd_checkEflag(CDC_HIRQ_ESEL) == TRUE) {
	    TSK_MOVSCT_STAT(gfcd_work) = GFCD_TSK_READY;
	}
    }
    return TSK_MOVSCT_STAT(gfcd_work);
}


/* ChgDirタスク */
GFS_LOCAL Sint32 gfcd_doChgDirTsk(Sint32 *ncmd)
{
    Sint32 base_fid, infnum, ret;
    Bool drend;
    Uint16 hirq;
    CdcStat stat;

    if (TSK_CHGDIR_STAT(gfcd_work) == GFCD_TSK_STAT1) {
	/* fidの範囲チェック状態 */
	ret = CDC_GetFileScope(&base_fid, &infnum, &drend);
	if (ret != CDC_ERR_OK) {
	    TSK_CHGDIR_STAT(gfcd_work) = GFCD_TSK_READY;
	    *TSK_CHGDIR_NDIR(gfcd_work) = GFS_ERR_CDRD;
	    return GFCD_ERR_CDBFS;	/* エラー */
	}
	++*ncmd;
	/* fidが有効範囲にはいっているか */
	if (TSK_CHGDIR_FID(gfcd_work) == 0 ||
	    TSK_CHGDIR_FID(gfcd_work) == 1 ||
	    (base_fid <= TSK_CHGDIR_FID(gfcd_work) &&
	     TSK_CHGDIR_FID(gfcd_work) < base_fid + infnum)) {
	    TSK_CHGDIR_STAT(gfcd_work) = GFCD_TSK_STAT3;
	} else {
	    ret = CDC_ReadDir(GFCD_SYS_SEL, TSK_CHGDIR_FID(gfcd_work));
	    if (ret != CDC_ERR_OK) {
		TSK_CHGDIR_STAT(gfcd_work) = GFCD_TSK_READY;
		*TSK_CHGDIR_NDIR(gfcd_work) = GFS_ERR_CDRD;
		return GFCD_ERR_CDBFS;	/* エラー */
	    }
	    ++*ncmd;
	    TSK_CHGDIR_STAT(gfcd_work) = GFCD_TSK_STAT2;
	}
    }
    if (TSK_CHGDIR_STAT(gfcd_work) == GFCD_TSK_STAT2) {
	/* ReadDir終了待ち */
	if (gfcd_checkEflag(CDC_HIRQ_EFLS)) {
	    ret = CDC_GetFileScope(&base_fid, &infnum, &drend);
	    if (ret != CDC_ERR_OK ||
		(TSK_CHGDIR_FID(gfcd_work) < base_fid &&
		 base_fid + infnum <= TSK_CHGDIR_FID(gfcd_work))) {
		TSK_CHGDIR_STAT(gfcd_work) = GFCD_TSK_READY;
		*TSK_CHGDIR_NDIR(gfcd_work) = GFS_ERR_CDRD;
		return GFCD_ERR_CDBFS;
	    }
	    TSK_CHGDIR_STAT(gfcd_work) = GFCD_TSK_STAT3;
	}
    }
    if (TSK_CHGDIR_STAT(gfcd_work) == GFCD_TSK_STAT3) {
	hirq = GFCD_GetStat(&stat);
	if (hirq & CDC_HIRQ_DCHG) {
	    return GFCD_ERR_OPEN;
	}
	if ((CDC_STAT_STATUS(&stat) & 0x0f) == CDC_ST_NODISC) {
	    return GFCD_ERR_NODISC;
	}
	ret = CDC_ChgDir(TSK_CHGDIR_WORK(gfcd_work),
			 TSK_CHGDIR_FID(gfcd_work));
	if (ret != CDC_ERR_OK) {
	    return GFCD_TSK_STAT1;
	}
	gfcd_setStat();
	++*ncmd;
	TSK_CHGDIR_STAT(gfcd_work) = GFCD_TSK_STAT4;
    }
    if (TSK_CHGDIR_STAT(gfcd_work) == GFCD_TSK_STAT4) {
	/* 終了をチェック */
	if (gfcd_checkEflag(CDC_HIRQ_EFLS)) {
	    TSK_CHGDIR_STAT(gfcd_work) = GFCD_TSK_READY;
	    ret = CDC_GetFileScope(&base_fid, &infnum, &drend);
	    if (ret != CDC_ERR_OK) {
		TSK_CHGDIR_STAT(gfcd_work) = GFCD_TSK_READY;
		*TSK_CHGDIR_NDIR(gfcd_work) = GFS_ERR_CDRD;
		return GFCD_ERR_CDBFS;	/* エラー */
	    }
	    *TSK_CHGDIR_NDIR(gfcd_work) = infnum;
	}
    }
    return TSK_CHGDIR_STAT(gfcd_work);
}


/* SetFiltConタスク */
GFS_LOCAL Sint32 gfcd_doSetFiltConTsk(Sint32 *ncmd)
{
    Sint32 ret;
    Sint32 i, j;
    Sint32 flag, flout, flnout;

    for (i = 0; i < TSK_FLTCON_LEN(gfcd_work); ++i) {
	/* フラグ作成 */
	if (TSK_FLTCON_BUF(gfcd_work, i) == GFCD_ANY_FLT) {
	    flag = 0;
	    flout = CDC_NUL_SEL;
	} else {
	    flag = FLTCON_BUF_SET;
	    flout = TSK_FLTCON_BUF(gfcd_work, i);
	}
	if (TSK_FLTCON_FLNOUT(gfcd_work, i) == GFCD_ANY_FLT) {
	    flnout = CDC_NUL_SEL;
	} else {
	    flag |= FLTCON_FLN_SET;
	    flnout = TSK_FLTCON_FLNOUT(gfcd_work, i);
	}
	/* コマンド発行 */
	ret = CDC_SetFiltCon(TSK_FLTCON_FLT(gfcd_work, i), flag, flout,
			     flnout);
	if (ret != CDC_ERR_OK) {
	    break;
	}
	gfcd_setStat();
	++*ncmd;
    }
    /* キューをつめる */
    if (i >= TSK_FLTCON_LEN(gfcd_work)) {
	TSK_FLTCON_LEN(gfcd_work) = 0;
    } else {
	/* まだ残りがある */
	for (j = 0; i < TSK_FLTCON_LEN(gfcd_work); ++i, ++j) {
	    TSK_FLTCON_FLT(gfcd_work, j) = TSK_FLTCON_FLT(gfcd_work, i);
	    TSK_FLTCON_BUF(gfcd_work, j) = TSK_FLTCON_BUF(gfcd_work, i);
	    TSK_FLTCON_FLNOUT(gfcd_work, j) = TSK_FLTCON_FLNOUT(gfcd_work, i);
	}
	TSK_FLTCON_LEN(gfcd_work) = j;
    }
    if (gfcd_checkEflag(CDC_HIRQ_ESEL)) {
	if (TSK_FLTCON_LEN(gfcd_work) == 0) {
	    TSK_FLTCON_STAT(gfcd_work) = GFCD_TSK_READY;
	}
    }
    return TSK_FLTCON_LEN(gfcd_work);
}


/* 終了フラグを調べる */
GFS_LOCAL Bool gfcd_checkEflag(Uint16 mask)
{
    Uint16 hirq;

    hirq = CDC_GetHirqReq();
    return (mask & hirq)? TRUE: FALSE;
}


/* CDブロック管理構造体初期化 */
GFS_LOCAL void gfcd_initCdbMng(void)
{
    Sint32 i;

    /* 各資源を未使用に初期化 */
    /*  CDBFS用セレクタは初期化しない */
    /*  GFCD_Init時にはこの後初期化し、タイムアウト時には変更しない */
    for (i = 0; i < GFS_OPEN_MAX - 1; ++i) {
	CDBMNG_USEBUF(gfcd_work, i) = UNUSED;
	CDBMNG_USEFILT(gfcd_work, i) = UNUSED;
    }
    CDBMNG_USEPU(gfcd_work) = UNUSED;
    CDBMNG_TRBUFNO(gfcd_work) = TRREG_UNUSED;
    CDBMNG_PUID(gfcd_work) = NOBODY;
    CDBMNG_TIMER(gfcd_work) = GFCD_NO_TMOUT;
    CDBMNG_FUNC(gfcd_work) = GFCD_NULLFUNC;
    CDBMNG_OBJ(gfcd_work) = NULL;
    /* 各タスクを初期化 */
    TSK_SETFLT_LEN(gfcd_work) = 0;
    TSK_SETFLT_STAT(gfcd_work) = GFCD_TSK_READY;
    TSK_SETCON_STAT(gfcd_work) = GFCD_TSK_READY;
    TSK_GETLEN_STAT(gfcd_work) = GFCD_TSK_READY;
    TSK_REQDAT_STAT(gfcd_work) = GFCD_TSK_READY;
    TSK_DELSCT_STAT(gfcd_work) = GFCD_TSK_READY;
    TSK_MOVSCT_STAT(gfcd_work) = GFCD_TSK_READY;
    TSK_CHGDIR_STAT(gfcd_work) = GFCD_TSK_READY;
    TSK_FLTCON_LEN(gfcd_work) = 0;
    TSK_FLTCON_STAT(gfcd_work) = GFCD_TSK_READY;
}


/* ステータスを更新する */
GFS_LOCAL void gfcd_setStat(void)
{
    CdcStat	stat;

    CDC_GetLastStat(&stat);
    CDC_STAT_STATUS(&CDBMNG_STAT(gfcd_work)) = CDC_STAT_STATUS(&stat);
}


/* CSCTビットが１になったら呼ぶ関数を登録 */
void GFCD_SetCsctFunc(void (*func)(void *), void *obj)
{
    CDBMNG_FUNC(gfcd_work) = func;
    CDBMNG_OBJ(gfcd_work) = obj;
}


/* SCDQフラグが立つのを待つ */
/* タイムアウトしたらTRUEを返す */
Bool GFCD_WaitScdqFlag(void)
{
    Sint32 timer;

    for (timer = 0; timer < SCDQ_TMOUT_COUNT; ++timer) {
	if (CDC_GetHirqReq() & CDC_HIRQ_SCDQ) {
	    CDC_ClrHirqReq(~CDC_HIRQ_SCDQ);
	    return FALSE;
	}
    }
    return TRUE;
}

/* end of file */
