/*****************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library: ストリームシステム
 * Module : ローカル用ヘッダファイル
 * File	  : stm_def.h
 * Date   : 1994-11-11
 * Version: 1.16
 * Auther : H.T
 *
 *****************************************************************************/
#if	!defined(STM_DEF_H)
#define	STM_DEF_H	1

#define	STM_LOCAL

/*****************************************************************************
 *	定数マクロ
 *****************************************************************************/

/* サーバ処理状態		*/
enum SvrStat {
    SVR_UNUSE	= FALSE,		/* オブジェクト使用中でない	*/
    SVR_USE	= TRUE,			/* オブジェクト使用中		*/
    SVR_END
};


/* ストリームのステータス	*/
enum StmStat {
    STAT_NOREAD	= 0x20,			/* アクセスされない		*/
    STAT_DOING	= STM_EXEC_DOING,	/* 処理中			*/
    STAT_WAIT	= STM_EXEC_WAIT,	/* 転送待ち(ゲート閉, 領域フル)	*/
    STAT_COMPLETED = STM_EXEC_COMPLETED,	/* アクセス完了		*/
    STAT_END
};


/* ストリームアトリビュート	*/
enum StmAtr {
    STM_ATR_READ	= 0x01,		/* 読み込みストリーム		*/
    STM_ATR_RESI	= 0x02,		/* 常駐ストリーム		*/
    STM_ATR_LOOPNOREAD	= 0x04,		/* ループ時読み込まない		*/
    STM_ATR_READEND	= 0x08,		/* 読み込み済み			*/
    STM_ATR_TRSTART	= 0x10,		/* 転送開始設定済み		*/
    STM_ATR_TREND	= 0x20,		/* 転送終了設定済み		*/
    STM_ATR_END
};


/* ストリームグループアトリビュート	*/
enum StmGrpAtr {
    STMGRP_ATR_LOOPSPECIFY	= 0x01,	/* ループストリームを定義された	*/
    STMGRP_ATR_PLAY		= 0x02,	/* 再生中			*/
    STMGRP_ATR_READ		= 0x04,	/* 読み込み中			*/
    STMGRP_ATR_CHKBUF		= 0x08,	/* バッファサイズチェック	*/
    STMGRP_ATR_PAUSE		= 0x10,	/* ポーズ中			*/
    STMGRP_ATR_SEEK		= 0x20,	/* シーク中			*/
    STMGRP_ATR_END
};


#define	FID_OPEN_FRANGE	0x3fffffff	/* セクタ位置オープン時のFID	*/

#define	TR_CONT		1		/* 転送中			*/

#define	TMODE_DFL	STM_TR_CPU	/* デフォルト転送モード		*/

/*****************************************************************************
 *	データ型の定義
 *****************************************************************************/

/* コマンド関数			*/
typedef	Bool	(*stmCmdFunc)(void *obj);

#define	CMD_NULLFUNC	((stmCmdFunc)0)	/* NULL関数			*/

/* コマンド			*/
typedef struct {
    stmCmdFunc	func;			/* 処理中コマンド		*/
    void	*obj;			/* 処理中オブジェクト		*/
} StmCmd;

#define	CMD_FUNC(cmd)	((cmd)->func)
#define	CMD_OBJ(cmd)	((cmd)->obj)


#define	MNG_GRP(mng, i)		((mng)->stmgrptbl + i)
#define	MNG_GRPTBL(mng)		((mng)->stmgrptbl)
#define	MNG_GRPMAX(mng)		((mng)->stmgrpmax)
#define	MNG_STM(mng, i)		((mng)->stmtbl + i)
#define	MNG_STMTBL(mng)		((mng)->stmtbl)
#define	MNG_STMMAX(mng)		((mng)->stmmax)
#define	MNG_CURGRP(mng)		((mng)->curgrp)
#define	MNG_SFAD(mng)		((mng)->sfad)
#define	MNG_EFAD(mng)		((mng)->efad)
#define	MNG_ERR(mng)		((mng)->err)
#define	MNG_SVREXEC(mng)	((mng)->svrexec)

#define	STM_GRP(stm)		((stm)->grp)
#define	STM_GRPHN(stm)		((StmGrpHn)(STM_GRP(stm)))
#define	STM_ALIAS(stm)		((stm)->alias)
#define	STM_SFADLST(stm)	((stm)->sfadlst)
#define	STM_FLTLST(stm)		((stm)->fltlst)
#define	STM_GFS(stm)		((stm)->gfs)
#define	STM_FLOW(stm)		((stm)->flow)
#define	STM_DTSRC(stm)		(&((stm)->flow->dtsrc))
#define	STM_TRFUNC(stm)		((stm)->trfunc)
#define	STM_OBJ(stm)		((stm)->obj)
#define	STM_TRFAD(stm)		((stm)->trfad)
#define	STM_ATR(stm)		((stm)->atr)
#define	STM_SVRSTAT(stm)	((stm)->svrstat)

#define	STM_GRP_SFADLST(grp)	((grp)->sfadlst)
#define	STM_GRP_FLTLST(grp)	((grp)->fltlst)
#define	STM_GRP_LOOPSTM(grp)	((grp)->loopstm)
#define	STM_GRP_LSTSTM(grp)	((grp)->lststm)
#define	STM_GRP_NEXTTRNS(grp)	((grp)->nexttrns)
#define	STM_GRP_FAD(grp)	((grp)->fad)
#define	STM_GRP_ACSTAT(grp)	((grp)->acstat)
#define	STM_GRP_CHKSTM(grp)	((grp)->chkstm)
#define	STM_GRP_CHKSCTNUM(grp)	((grp)->chksctnum)
#define	STM_GRP_CHKFUNC(grp)	((grp)->chkfunc)
#define	STM_GRP_CHKOBJ(grp)	((grp)->chkobj)
#define	STM_GRP_LOOPMAX(grp)	((grp)->loopmax)
#define	STM_GRP_LOOPCNT(grp)	((grp)->loopcnt)
#define	STM_GRP_PUID(grp)	((grp)->puid)
#define	STM_GRP_ATR(grp)	((grp)->atr)
#define	STM_GRP_SVRSTAT(grp)	((grp)->svrstat)


/*****************************************************************************
 *	処理マクロ
 *****************************************************************************/

/* CDファイルの判定 */
#define STM_IS_CDFILE(stm)	\
    ((GFS_DTS_FTYPE(STM_DTSRC(stm)) == CD_FILE) ? TRUE: FALSE)

/* 読み込みストリームの判定	*/
#define	IS_STM_READ(stm)	\
    (((STM_ATR(stm) & STM_ATR_READ) != 0) ? TRUE : FALSE)

/* 常駐ストリームの判定	*/
#define	IS_STM_RESI(stm) \
    (((STM_ATR(stm) & STM_ATR_RESI) != 0) ? TRUE : FALSE)

/* ループ時読み込まない判定	*/
#define	IS_STM_LOOPNOREAD(stm) \
    (((STM_ATR(stm) & STM_ATR_LOOPNOREAD) != 0) ? TRUE : FALSE)

/* 読み込み済みの判定		*/
#define	IS_STM_READEND(stm) \
    (((STM_ATR(stm) & STM_ATR_READEND) != 0) ? TRUE : FALSE)

/* 再生中かどうかの判定		*/
#define	IS_PLAYING(grp)	\
    (((STM_GRP_ATR(grp) & STMGRP_ATR_PLAY) != 0) ? TRUE : FALSE)

/* 読み込み中かどうかの判定	*/
#define	IS_READING(grp)	\
    (((STM_GRP_ATR(grp) & STMGRP_ATR_READ) != 0) ? TRUE : FALSE)

/* シーク中かどうかの判定	*/
#define	IS_SEEKING(grp)	\
    (((STM_GRP_ATR(grp) & STMGRP_ATR_SEEK) != 0) ? TRUE : FALSE)

/* ポーズ中かどうかの判定	*/
#define	IS_PAUSING(grp)	\
    (((STM_GRP_ATR(grp) & STMGRP_ATR_PAUSE) != 0) ? TRUE : FALSE)

/* ループストリームが設定されたかどうかの判定	*/
#define	IS_LOOPSPECIFY(grp)	\
    (((STM_GRP_ATR(grp) & STMGRP_ATR_LOOPSPECIFY) != 0) ? TRUE : FALSE)

/* ループストリーム指定フラグの設定		*/
#define	SET_LOOPSPECIFY(grp)	(STM_GRP_ATR(grp) |= STMGRP_ATR_LOOPSPECIFY)

/* ループストリーム指定フラグの解除		*/
#define	CLR_LOOPSPECIFY(grp)	(STM_GRP_ATR(grp) &= ~STMGRP_ATR_LOOPSPECIFY)

/* エラーが設定されているかどうかの判定		*/
#define	IS_SET_ERRCODE(mng)	\
    ((STM_ERR_CODE(&MNG_ERR(mng)) != STM_ERR_OK) ? TRUE : FALSE)

/*****************************************************************************
 *	変数の宣言
 *****************************************************************************/

/* ストリーム管理領域	*/
extern StmMng	*stm_mng_ptr;

/* コマンド登録領域	*/
extern StmCmd	stm_cmd_area;

/*****************************************************************************
 *	関数の宣言
 *****************************************************************************/

/*--- STMMNG functions -------------------------------------------------*/

/* 最終FADの取得			*/
extern Sint32	STMMNG_GetLstFad(StmGrpHn grp);

/* 絞り接続順リストの切断		*/
extern Bool	STMMNG_RemoveFlt(StmHn stm);

/* GFCDのエラーをストリームのエラーに変換し設定する	*/
extern void	STM_SetGfcdErr(Sint32 code);

/* 読み込み終了フラグの設定		*/
extern void	STMSVR_SetReadEnd(StmGrpHn grp);

/* CDバッファ内セクタ数の取得		*/
extern Sint32	STMSVR_GetNumCdbuf(StmHn stm);

/* 転送領域を初期化する			*/
extern void	STMTRN_ResetTrBuf(StmHn stm);

/* 転送領域フルのチェック		*/
extern Bool	STMTRN_IsTrBufFull(StmHn stm);

/*--- STMERR functions -------------------------------------------------*/

/* 処理関数の設定			*/
extern void	STMERR_SetFunc(Sint32 funcid);

/* エラーコードの設定			*/
extern void	STMERR_SetCode(Sint32 code);

/* エラー関数の呼びだし			*/
extern void	STMERR_Exec(void);

#endif


