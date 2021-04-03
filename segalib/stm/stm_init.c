/*****************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library: ストリームシステム
 * Module : 初期化モジュール
 * File	  : stm_init.c
 * Date   : 1994-11-11
 * Version: 1.16
 * Auther : H.T
 *
 *****************************************************************************/

/*****************************************************************************
 *	インクルードファイル
 *****************************************************************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"sega_stm.h"
#include	"../gfs/gfs_def.h"
#include	"../gfs/gfs_cdc.h"
#include	"../gfs/gfs_cdf.h"
#include	"../gfs/gfs_cdb.h"
#include	"../gfs/gfs_trn.h"
#include	"../gfs/gfs_buf.h"
#include	"../gfs/gfs_dir.h"
#include	"stm_loc.h"
#include	"stm_def.h"

/*****************************************************************************
 *	変数の定義
 *****************************************************************************/

/* ストリーム管理領域	*/
StmMng	*stm_mng_ptr;


/*****************************************************************************
 *	関数の定義
 *****************************************************************************/

/*--- STMMNG functions -------------------------------------------------*/

/****************************************************
 * ストリームシステムマネージャ初期化
 * grpmax : 同時に開くストリームグループの最大数
 * stmmax : 同時に開くストリームの最大数
 * work   : 作業領域
 ****************************************************/
STM_LOCAL void	stmmng_init(Sint32 grpmax, Sint32 stmmax, void *work)
{
    stm_mng_ptr = (StmMng *)work;
    memset(stm_mng_ptr, SVR_UNUSE, STM_WORK_SIZE(grpmax, stmmax));
    MNG_GRPTBL(stm_mng_ptr) = (StmGrp *)(stm_mng_ptr + 1);
    MNG_GRPMAX(stm_mng_ptr) = grpmax;
    MNG_STMTBL(stm_mng_ptr) = (Stm *)(MNG_GRPTBL(stm_mng_ptr) + grpmax);
    MNG_STMMAX(stm_mng_ptr) = stmmax;
    MNG_SFAD(stm_mng_ptr) = -1;
    MNG_EFAD(stm_mng_ptr) = -1;
    MNG_SVREXEC(stm_mng_ptr) = FALSE;

    CMD_FUNC(&stm_cmd_area) = CMD_NULLFUNC;
    CMD_OBJ(&stm_cmd_area) = NULL;

    STM_ERR_CODE(&MNG_ERR(stm_mng_ptr)) = STM_ERR_OK;
    STM_ERR_WHERE(&MNG_ERR(stm_mng_ptr)) = STM_INIT;
    STM_ERR_FUNC(&MNG_ERR(stm_mng_ptr)) = STM_ERR_NULLFUNC;
    STM_ERR_OBJ(&MNG_ERR(stm_mng_ptr)) = NULL;
}



/****************************************************
 * 未使用グループ領域の取得
 * 戻り値 :	未使用ストリームグループ領域
 ****************************************************/
STM_LOCAL StmGrp	*stmmng_getUnuseGrp(void)
{
    Sint32	i;
    StmGrp	*grp;

    for (i = 0, grp = MNG_GRP(stm_mng_ptr, 0); i < MNG_GRPMAX(stm_mng_ptr); i++, grp++) {
	if (STM_GRP_SVRSTAT(grp) == SVR_UNUSE) {
	    STM_GRP_SFADLST(grp) = NULL;
	    STM_GRP_FLTLST(grp) = NULL;
	    STM_GRP_LOOPSTM(grp) = NULL;
	    STM_GRP_LSTSTM(grp) = NULL;
	    STM_GRP_NEXTTRNS(grp) = NULL;
	    STM_GRP_FAD(grp) = STM_FAD_CDTOP;
	    STM_GRP_ACSTAT(grp) = STM_EXEC_PAUSE;
	    STM_GRP_CHKSTM(grp) = STM_CDBUF_REST;
	    STM_GRP_CHKSCTNUM(grp) = 0;
	    STM_GRP_CHKFUNC(grp) = STM_CDBUF_NULLFUNC;
	    STM_GRP_CHKOBJ(grp) = NULL;
	    STM_GRP_LOOPMAX(grp) = STM_LOOP_ENDLESS;
	    STM_GRP_LOOPCNT(grp) = 0;
	    STM_GRP_PUID(grp) = -1;
	    STM_GRP_ATR(grp) = 0;
	    STM_GRP_SVRSTAT(grp) = SVR_USE;
	    return (grp);
	}
    }
    return (NULL);
}
    


/****************************************************
 * 未使用ストリーム領域の取得
 * 戻り値 :	未使用ストリーム領域
 ****************************************************/
STM_LOCAL Stm	*stmmng_getUnuseStm(void)
{
    Sint32	i;
    Stm		*stm;

    for (i = 0, stm = MNG_STM(stm_mng_ptr, 0); i < MNG_STMMAX(stm_mng_ptr); i++, stm++) {
	if (STM_SVRSTAT(stm) == SVR_UNUSE) {
	    return (stm);
	}
    }
    return (NULL);
}
    


/*****************************************************
 * 最終FADの取得
 * grp	: ストリームグループ
 * 戻り値: 最終FAD
 *****************************************************/
Sint32	STMMNG_GetLstFad(StmGrpHn grp)
{
    Sint32	sfad, efad;
    StmHn	lststm;

    lststm = STM_GRP_LSTSTM(grp);
    if (lststm == NULL) {
	return (STM_FAD_CDTOP);
    } else {
	STL_GetPlayArea(STM_FLOW(lststm), &sfad, &efad);
	return (efad);
    }
}



/*****************************************************
 * GFCDのエラーをストリームのエラーに変換し設定する
 * code	: エラーコード
 *****************************************************/
void	STM_SetGfcdErr(Sint32 code)
{
    Sint32	stmerr;

    switch (code) {
    case	GFCD_ERR_TMOUT:
	stmerr = STM_ERR_TMOUT;
	break;
    case	GFCD_ERR_OPEN:
	stmerr = STM_ERR_CDOPEN;
	break;
    case	GFCD_ERR_NODISC:
	stmerr = STM_ERR_CDNODISC;
	break;
    case	GFCD_ERR_FATAL:
	stmerr = STM_ERR_FATAL;
	break;
    default:
	stmerr = STM_ERR_CDRD;
    }
    STMERR_SetCode(stmerr);
}



/*****************************************************
 * ストリームリストの設定
 * grp	  : ストリームグループ
 * stm	  : ストリームハンドラ
 * atr	  : アトリビュート
 *****************************************************/
STM_LOCAL Bool	stmmng_setStmLst(StmGrpHn grp, StmHn stm, Uint8 atr)
{
    StmHn	*lst_stm;
    StmHn	before;
    Sint32	next;
    Sint32	stm_sfad, stm_efad;
    Sint32	lst_sfad, lst_efad;
    Sint32	ret;

    /* ストリームの開始FADを取得	*/
    STL_GetPlayArea(STM_FLOW(stm), &stm_sfad, &stm_efad);

    /* 開始FADが昇順になるように挿入位置を求める	*/
    for (lst_stm = &STM_GRP_SFADLST(grp); *lst_stm != NULL; 
	                                    lst_stm = &STM_SFADLST(*lst_stm)) {
	STL_GetPlayArea(STM_FLOW(*lst_stm), &lst_sfad, &lst_efad);
	if (stm_sfad < lst_sfad) {
	    break;
	}
    }
    /* ストリーム挿入	*/
    STM_SFADLST(stm) = *lst_stm;	/* 次のストリームの設定		*/
    *lst_stm = stm;			/* 前のストリームの設定		*/

    if (STM_IS_CDFILE(stm)) {
	/* 最終ストリーム設定	*/
	lst_efad = STMMNG_GetLstFad(grp);
	if (stm_efad > lst_efad) {
	    STM_GRP_LSTSTM(grp) = stm;
	}

	/* 絞りの再後尾を求める	*/
	before = NULL;
	for (lst_stm = &STM_GRP_FLTLST(grp); *lst_stm != NULL; 
	                                    lst_stm = &STM_FLTLST(*lst_stm)) {
	    before = *lst_stm;
	}
	GFCD_SetFiltCon(GFS_CDR_FLT(STM_DTSRC(stm)), 
			GFS_CDR_BUFNO(STM_DTSRC(stm)), CDC_NUL_SEL);
	/* 絞り接続設定	*/
	if (before != NULL) {
	    GFCD_SetFiltCon(GFS_CDR_FLT(STM_DTSRC(before)), 
			    GFS_CDR_BUFNO(STM_DTSRC(before)),
			    GFS_CDR_BUFNO(STM_DTSRC(stm)));
	} else if (STM_GRP_PUID(grp) >= 0) {
	    GFCD_SetCon(GFS_CDR_FLT(STM_DTSRC(stm)), STM_GRP_PUID(grp));
	}
	/* ストリーム追加	*/
	*lst_stm = stm;
	STM_FLTLST(stm) = NULL;
	ret = GFCD_WaitServer();
	if (ret != GFCD_ERR_OK) {
	    STM_SetGfcdErr(ret);
	    return (FALSE);
	}
    }

    /* ループストリーム設定	*/
    if ((IS_LOOPSPECIFY(grp) == FALSE)&&
	((atr & STM_ATR_RESI) == 0)&&
	(stm == STM_GRP_SFADLST(grp))) {
	STM_GRP_LOOPSTM(grp) = stm;
    }
    return (TRUE);
}



/*************************************************
 * 絞りモードの取得
 * subh	: 絞りパターン
 * 戻り値 : 絞りモード
 *************************************************/
STM_LOCAL Sint32	stm_getFmode(StmKey *key)
{
    Sint32	fmode = FMODE_FAD;

    if (STM_KEY_FN(key) != STM_KEY_NONE) {
	fmode |= FMODE_FN;
    }
    if (STM_KEY_CN(key) != STM_KEY_NONE) {
	fmode |= FMODE_CN;
    }
    if (STM_KEY_SMMSK(key) != STM_KEY_NONE) {
	fmode |= FMODE_SM;
    }
    if (STM_KEY_CIMSK(key) != STM_KEY_NONE) {
	fmode |= FMODE_CI;
    }
    return (fmode);
}



/*************************************************
 * デフォルト絞りの取得
 * key	: ストリームキー
 * dirrec : ディレクトリレコード
 * subh	: 絞りパターン設定領域
 * 戻り値 : 絞りモード
 *************************************************/
STM_LOCAL Sint32	stm_getDefaultFilt(StmKey *key, GfsDirId *dirrec, CdcSubh *subh)
{
    Sint32	fmode;

    CDC_SUBH_FN(subh) = (Uint8)STM_KEY_FN(key);
    CDC_SUBH_CN(subh) = (Uint8)STM_KEY_CN(key);
    CDC_SUBH_SMMSK(subh) = (Uint8)STM_KEY_SMMSK(key);
    CDC_SUBH_SMVAL(subh) = (Uint8)STM_KEY_SMVAL(key);
    CDC_SUBH_CIMSK(subh) = (Uint8)STM_KEY_CIMSK(key);
    CDC_SUBH_CIVAL(subh) = (Uint8)STM_KEY_CIVAL(key);
    fmode = stm_getFmode(key);
    return (fmode);
}

    

/*****************************************************
 * ストリーム領域の初期化
 * stm	  : 領域
 * grp	  : ストリームグループ
 * gfs	  : ファイルハンドル
 * dirrec : ディレクトリレコード
 * key    : ストリームキー
 * atr	  : アトリビュート
 * 戻り値 : ストリームハンドル
 *****************************************************/
STM_LOCAL StmHn	stmmng_initStm(Stm *stm, StmGrpHn grp, GfsHn gfs, 
		       GfsDirId *dirrec, StmKey *key, Uint8 atr)
{
    GfsFlow	*flow;
    GfsFinfo	*finfo;
    Sint32	nsct;
    Sint32	sfad, efad;
    Sint32	fmode;
    Sint32	ret;
    Bool	lst;
    CdcSubh	subh;
    Sint32	fad;

    flow = &(GFS_FILE_FLOW(gfs));
    finfo = &(GFS_FLW_FINFO(flow));

    /* 再生セクタ数をインタリーブギャプを含めたものに変換	*/
    nsct = GFCF_cnvIntlvOfs(finfo, GFS_FI_NSCT(finfo));
    GFS_FI_NSCT(finfo) = nsct;
    GFS_FI_LSTRM(finfo) = 0;  	/* 最終セクタは全て有効にする	*/

    STM_GRP(stm) = grp;
    STM_ALIAS(stm) = stm;
    STM_GFS(stm) = gfs;
    STM_FLOW(stm) = flow;
    STM_TRFUNC(stm) = STM_TR_NULLFUNC;
    STM_OBJ(stm) = NULL;
    STM_ATR(stm) = atr;
    STL_GetPlayArea(flow, &sfad, &efad);
    /* 読み込み中にオープンされた場合読み込みフラグをたてる	*/
    if ((IS_PLAYING(grp) == TRUE)&&(sfad >= STM_GRP_FAD(grp))) {
	STM_ATR(stm) |= STM_ATR_READ;
    }
    STM_SVRSTAT(stm) = SVR_USE;
    GFS_TRN_STAT(&GFS_FILE_TRANS(gfs)) = GFTR_ST_END;
    STM_TRFAD(stm) = STM_FAD_CDTOP;		/* 転送FADの設定	*/

    /* 絞りモード設定			*/
    if (STM_IS_CDFILE(stm)) {
	fmode = stm_getDefaultFilt(key, dirrec, &subh);		/* デフォルト絞り */
	GFCD_SetFilt(GFS_CDR_FLT(&(GFS_FLW_DTSRC(flow))), fmode, &subh,
		     GFS_FI_FAD(finfo), GFS_FI_NSCT(finfo)); 	/* 絞り設定	  */
	ret = GFCD_WaitServer();
	if (ret != GFCD_ERR_OK) {
	    STM_SetGfcdErr(ret);
	    return (NULL);
	}
	GFCF_SetSct(flow, LONG_MAX);				/* 読込みセクタ数 */
    }

    lst = stmmng_setStmLst(grp, stm, atr);
    if (lst == FALSE) {
	return (NULL);
    }
    /* 再生中に最終ストリームが変わった場合	*/
    if ((IS_PLAYING(grp) == TRUE)&&		/* 読み込み中		*/
	(IS_SEEKING(grp) == FALSE)&&		/* シーク中でない	*/
	(STM_GRP_LSTSTM(grp) == stm)) {
	GFCF_GetStat(&fad, NULL);
	/* 再生範囲の終了より10セクタ以上手前なら終了位置を変更する	*/
	/* 10セクタ以下ならポーズを待ってstm_readで再度PLAYを発行する	*/
	if (fad < (MNG_EFAD(stm_mng_ptr) - 10)) {			/* ビジーでない		*/
	    ret = STL_ChgPlayEnd(grp, efad);	/* 再生範囲の変更	*/
	    if (ret != STM_ERR_OK) {
		STMERR_SetCode(ret);
	    }
	}
    }
    return (stm);
}
   


/*****************************************************
 * ストリームグループ領域解放
 * grp	:	グループ領域
 *****************************************************/
STM_LOCAL void	stmmng_unuseGrp(StmGrpHn grp)
{
    STM_GRP_SFADLST(grp) = NULL;
    STM_GRP_FLTLST(grp) = NULL;
    STM_GRP_SVRSTAT(grp) = SVR_UNUSE;
}


/*****************************************************
 * 絞り接続順リストの切断
 * stm	: ストリーム
 *****************************************************/
Bool	STMMNG_RemoveFlt(StmHn stm)
{
    StmGrpHn	grp;
    StmHn	*next;
    StmHn	before;
    Sint32	fltno;
    Sint32	ret;

    if (STM_IS_CDFILE(stm)) {
	grp = STM_GRPHN(stm);
	before = NULL;
	for (next = &STM_GRP_FLTLST(grp); (*next != NULL)&&(*next != stm); 
	                                      next = &STM_FLTLST(*next)) {
	    before = *next;
	}
	if (*next != NULL) {
	    *next = STM_FLTLST(stm);
	    if (*next == NULL) {
		fltno = CDC_NUL_SEL;
	    } else {
		fltno = GFS_CDR_FLT(STM_DTSRC(*next));
	    }
	    if (before == NULL) {
		GFCD_SetCon(fltno, STM_GRP_PUID(grp));
	    } else {
		GFCD_SetFiltCon(GFS_CDR_FLT(STM_DTSRC(before)), 
				GFS_CDR_BUFNO(STM_DTSRC(before)), fltno);
	    }
	    ret = GFCD_WaitServer();
	    if (ret != GFCD_ERR_OK) {
		STM_SetGfcdErr(ret);
		return (FALSE);
	    }
	}
    }
    return (TRUE);
}



/*****************************************************
 * ストリーム領域解放
 * stm	: ストリーム
 *****************************************************/
STM_LOCAL Bool	stmmng_unuseStm(StmHn stm)
{
    StmGrpHn	grp;
    StmHn	*next;
    StmHn	before;
    Sint32	bufno;
    Bool	ret = TRUE;;

    if (STM_SVRSTAT(stm) == SVR_USE) {
	STM_SVRSTAT(stm) = SVR_UNUSE;
	grp = STM_GRPHN(stm);
	if (grp == NULL) {
	    return FALSE;
	}
	/* 再生順リストからストリームを切断する		*/
	for (next = &STM_GRP_SFADLST(grp); (*next != NULL)&&(*next != stm); 
	                                      next = &STM_SFADLST(*next))
	    ;
	if (*next != NULL) {
	    *next = STM_SFADLST(stm);
	}

	/* 絞り接続順リストからストリームを切断する	*/
	ret = STMMNG_RemoveFlt(stm);
	STM_GRP(stm) = NULL;
    }
    return (ret);
}


/*****************************************************
 * 最終ストリームの再設定
 * grp	: ストリームグループ
 *****************************************************/
void	STMMNG_ResetLstStm(StmGrpHn grp)
{
    StmHn	stm;
    Sint32	sfad, efad;
    StmHn	lst_stm = NULL;
    Sint32	lst_efad = STM_FAD_CDTOP;

    for (stm = STM_GRP_FLTLST(grp); stm != NULL; stm = STM_FLTLST(stm)) {
	if (STM_IS_CDFILE(stm)) {
	    STL_GetPlayArea(STM_FLOW(stm), &sfad, &efad);
	    if (lst_efad < efad) {
		lst_efad = efad;
		lst_stm = stm;
	    }
	}
    }
    STM_GRP_LSTSTM(grp) = lst_stm;
}



/****************************************************
 * ストリームシステム初期化(1.1)
 * grpmax : 同時に開くストリームグループの最大数
 * stmmax : 同時に開くストリームの最大数
 * work   : 作業領域
 * 戻り値 :	TRUE	正常終了
 *		FALSE	以上終了
 ***************************************************/
Bool	STM_Init(Sint32 grpmax, Sint32 stmmax, void *work)
{
    /* 作業領域未設定のため、エラー設定は不可	*/

    if (work == NULL) {
	return (FALSE);
    } else {
	stmmng_init(grpmax, stmmax, work);	/* ストリームシステムマネージャ初期化	*/
	return (TRUE);
    }
}



/**************************************************
 * ストリームグループのオープン(2.1)
 * 戻り値 :	ストリームグループハンドル(割り当てられない場合NULL)
 **************************************************/
StmGrpHn	STM_OpenGrp(void)
{
    StmGrp	*grp;

    STMERR_SetFunc(STM_OPENGRP);
    grp = stmmng_getUnuseGrp();		/* 未使用領域の取得		*/
    if (grp == NULL) {			/* 割り当て領域がない場合	*/
	return (NULL);
    }
    return (grp);
}



/*****************************************************
 * ストリームのクローズ処理
 * stm	: ストリーム
 *****************************************************/
STM_LOCAL void	stminit_close(StmHn stm)
{
    StmHn	next;
    StmGrpHn	grp;
    Sint32	sfad, efad;
    Sint32	ret;

    grp = STM_GRPHN(stm);
    if (STM_SVRSTAT(stm) == SVR_USE) {    /* オープンストリーム  */
	/* ループストリームの場合は常駐でない先頭ストリームを設定	*/
	if (stm == STM_GRP_LOOPSTM(grp)) {
	    for (next = STM_GRP_SFADLST(grp);
		 next != NULL; next = STM_SFADLST(next)) {
		if ((next != stm)&&(IS_STM_RESI(next) == FALSE)) {
		    break;
		}
	    }
	    STM_GRP_LOOPSTM(grp) = next;
	    CLR_LOOPSPECIFY(grp);
	}
	/* 転送ストリームの場合は次のストリームを設定	*/
	if (stm == STM_GRP_NEXTTRNS(grp)) {
	    GFTR_Stop(STM_GFS(stm), GFTR_ALLSCT);
	    GFS_TRN_STAT(&GFS_FILE_TRANS(STM_GFS(stm))) = GFTR_ST_NOACT;
                                        /* END 状態にならないようにする	*/
	    STM_GRP_NEXTTRNS(grp) = STM_SFADLST(stm);
	    /* 次のストリームがない場合は先頭のストリーム	*/
	    if (STM_GRP_NEXTTRNS(grp) == NULL) {
		STM_GRP_NEXTTRNS(grp) = STM_GRP_SFADLST(grp);
	    }
	    /* 先頭が自分自身の場合は転送ストリームを設定しない	*/
	    if (STM_GRP_NEXTTRNS(grp) == stm){
		STM_GRP_NEXTTRNS(grp) = NULL;
	    }
	}
	/* ストリームが接続されていることがあるのでデフォルト状態に戻す	*/
	GFS_CDR_BUFNO(STM_DTSRC(stm)) = GFS_CDR_FLT(STM_DTSRC(stm));
	stmmng_unuseStm(stm);			/* 領域解放		*/
	/* 再生中の場合、絞りを切った後CSCTを待ってバッファをクリアする	*/
	if (IS_PLAYING(grp) == TRUE) {
	    STL_WaitCsct();
	}
	GFS_Close(STM_GFS(stm));		/* ファイルハンドラ解放	*/
    }

    /* 最終ストリームの場合		*/
    if (stm == STM_GRP_LSTSTM(grp)) {
	STMMNG_ResetLstStm(grp);	/* 最終ストリームの再設定	*/
	if ((STM_GRP_LSTSTM(grp) != NULL)&&
	    (IS_PLAYING(grp) == TRUE)) {
	    STL_GetPlayArea(STM_FLOW(STM_GRP_LSTSTM(grp)), &sfad, &efad);
	    ret = STL_ChgPlayEnd(grp, efad);
	    if (ret != STM_ERR_OK) {
		STMERR_SetCode(ret);
	    }
	}
    }
    /* 再生中にすべての読み込むべきストリームをクローズした場合	*/
    if ((STM_GRP_FLTLST(grp) == NULL)&&
	(IS_PLAYING(grp) == TRUE)) {
	ret = STL_StopIn(grp);				/* 読み込み停止	*/
	if (ret != STM_ERR_OK) {
	    STMERR_SetCode(ret);
	} else {
	    STMSVR_SetReadEnd(grp);		/* 読み込み済みフラグの設定	*/
	    STM_GRP_LOOPCNT(grp)++;
	    STM_GRP_FAD(grp) = STM_FAD_CDTOP;
	    STM_GRP_ACSTAT(grp) = STM_EXEC_PAUSE;
	    /* 再生終了,読み込み終了	*/
	    STM_GRP_ATR(grp) &= ~(STMGRP_ATR_PLAY|STMGRP_ATR_READ|STMGRP_ATR_SEEK);
	}
    }

}


/*************************************************
 * ストリームグループのクローズ(2.2)
 * grp	: ストリームグループハンドル
 *************************************************/
void	STM_CloseGrp(StmGrpHn grp)
{
    StmHn		stm;
    Bool		close_flag = TRUE;

    STMERR_SetFunc(STM_CLOSEGRP);
    if (grp == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return;
    }
    /* すべてのストリームをクローズする		*/
    while ((stm = STM_GRP_SFADLST(grp)) != NULL) {
	stminit_close(stm);
    }
    /* 実行中のストリームグループの場合		*/
    if (STM_GRP_PUID(grp) >= 0) {
	MNG_CURGRP(stm_mng_ptr) = NULL;
	GFCD_UngetPickup(STM_GRP_PUID(grp));
	STM_GRP_PUID(grp) = -1;
	GFCD_SetCsctFunc(GFCD_NULLFUNC, NULL);
	STL_WaitStop();		/* ドライブが停止するのを待つ	*/
    }
    stmmng_unuseGrp(grp);
    if (IS_SET_ERRCODE(stm_mng_ptr) == TRUE) {
	STMERR_Exec();
    }
}


/*************************************************
 * ストリームオープンのサブルーチン
 * grp	  : ストリームグループ
 * dirrec : ディレクトリレコード
 * fid	  : ファイル識別子
 * key	  : ストリームキー
 * atr	  : アトリビュート
 * 戻り値 : ストリームハンドル
 ************************************************/
STM_LOCAL StmHn	stminit_open(StmGrpHn grp, GfsDirId *dirrec, Sint32 fid, StmKey *key,
		                                                    Uint8 atr)
{
    Stm		*stm;
    GfsHn	gfs;
    StmHn	stmHn;
    Sint32	stat;
    Sint32	nsct, rdsct;

    /* ストリーム未使用領域の取得	*/
    stm = stmmng_getUnuseStm();
    if (stm == NULL) {			/* 領域がない		*/
	STMERR_SetCode(STM_ERR_NOHNDL);
	STMERR_Exec();
	return (NULL);
    }
    /* ファイルシステムのオープン領域の取得	*/
    gfs = GFS_mngAllocGrp();
    if (gfs == NULL) {
	stmmng_unuseStm(stm);		/* 取得したストリーム領域の解放	*/
	STMERR_SetCode(STM_ERR_NOHNDL);
	STMERR_Exec();
	return (NULL);
    }
    /* オープン処理			*/
    gfs = GFS_OpenSub(gfs, dirrec, fid);	        /* ファイルオープン */
    stmHn = stmmng_initStm(stm, grp, gfs, dirrec, key, atr);	/* ハンドル初期化 */
    if (stmHn == NULL) {
	STMERR_Exec();
	return (NULL);
    }

    /* 常駐ストリームの場合		*/
    if ((atr & STM_ATR_RESI) != 0) {
	GFS_SetGmode(gfs, GFS_GMODE_RESIDENT);	/* 常駐モード		*/
    }

    /* CDファイル以外は読み込みを先に行なう	*/
    if (!(STM_IS_CDFILE(stm))) {
	GFS_GetFileSize(gfs, NULL, &nsct, NULL);
	GFS_NwCdRead(gfs, nsct);
	while (1) {
	    GFS_NwExecOne(gfs);
	    GFS_NwGetStat(gfs, &stat, &rdsct);
	    if (rdsct >= nsct) {
		break;
	    }
	}
    }

    /* パラメータ設定               */
    GFS_FILE_AMODE(STM_GFS(stmHn)) = GFS_NWSTAT_FREAD;	/* アクセスモード */
    GFS_FILE_ASTAT(STM_GFS(stmHn)) = GFS_SVR_BUSY;		/* 処理フラグ     */
    GFTR_Start(&(GFS_FILE_TRANS(gfs)));
    return (stmHn);
}



/***********************************************
 * ファイル識別子によるストリームのオープン(サブルーチン)
 * grp	  : ストリームグループ
 * fid	  : ファイル識別子
 * key	  : ストリームキー
 * atr	  : アトリビュート
 * 戻り値 : ストリームハンドル(オープンできない場合NULL)
 ***********************************************/
STM_LOCAL StmHn	stminit_openFid(StmGrpHn grp, Sint32 fid, StmKey *key, Uint8 atr)
{
    GfsDirId	dirrec;
    StmKey	_key;
    StmHn	stm;
    Sint32	err;

    err = GFS_GetDirInfo(fid, &dirrec);		/* ディレクトリ情報取得	*/
    if (err == GFS_ERR_OK) {
	memcpy(&_key, key, sizeof(StmKey));
	STM_KEY_FN(&_key) = GFS_DIR_FN(&dirrec);
	stm = stminit_open(grp, &dirrec, fid, &_key, atr);  /* オープン	*/
    } else {
	STMERR_SetCode(err);
	STMERR_Exec();
	stm = NULL;
    }
    return (stm);
}
    

/***********************************************
 * ファイル識別子によるストリームのオープン(3.1)
 * grp	  : ストリームグループ
 * fid	  : ファイル識別子
 * key	  : ストリームキー
 * lpread : ループ再生時の読み込み指定
 * 戻り値 : ストリームハンドル(オープンできない場合NULL)
 ***********************************************/
StmHn	STM_OpenFid(StmGrpHn grp, Sint32 fid, StmKey *key, Sint32 lpread)
{
    StmHn	stm;
    Uint8	atr;

    STMERR_SetFunc(STM_OPENFID);
    if (grp == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (NULL);
    }
    if (lpread == STM_LOOP_NOREAD) {
	atr = STM_ATR_LOOPNOREAD;
    } else if (lpread == STM_LOOP_READ) {
	atr = 0;
    } else {
	STMERR_SetCode(STM_ERR_PARA);
	STMERR_Exec();
	return (NULL);
    }
    stm = stminit_openFid(grp, fid, key, atr);
    return (stm);
}



/***********************************************
 * セクタ位置によるストリームのオープン(3.2)
 * grp	  : ストリームグループハンドル
 * frange : 再生範囲
 * key	  : ストリームキー
 * lpread : ループ再生時の読み込み指定
 * 戻り値 : ストリームハンドル(オープンできない場合NULL)
 ***********************************************/
StmHn	STM_OpenFrange(StmGrpHn grp, StmFrange *frange, StmKey *key, Sint32 lpread)
{
    GfsDirId	dirrec;
    StmHn	stm;
    Uint8	atr;

    STMERR_SetFunc(STM_OPENFRANGE);
    if (grp == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (NULL);
    }
    if ((STM_FRANGE_SFAD(frange) < STM_FAD_CDTOP)||
	(STM_FRANGE_FASNUM(frange) < 0)) {
	STMERR_SetCode(STM_ERR_NUM);
	STMERR_Exec();
	return (NULL);
    }
	
    /* ディレクトリレコードセットアップ	*/
    GFS_DIR_FAD(&dirrec) = STM_FRANGE_SFAD(frange);
    GFS_DIR_SIZE(&dirrec) = STM_FRANGE_FASNUM(frange) * GFS_MD1_SCTSIZ;
    GFS_DIR_FN(&dirrec) = (Uint8)STM_KEY_FN(key);
    GFS_DIR_ATR(&dirrec) = 0;
    GFS_DIR_UNIT(&dirrec) = 1;
    GFS_DIR_GAP(&dirrec) = 0;
    if (lpread == STM_LOOP_NOREAD) {
	atr = STM_ATR_LOOPNOREAD;
    } else if (lpread == STM_LOOP_READ) {
	atr = 0;
    } else {
	STMERR_SetCode(STM_ERR_PARA);
	STMERR_Exec();
	return (NULL);
    }

    /* オープン		*/
    stm = stminit_open(grp, &dirrec, FID_OPEN_FRANGE, key, atr);
    return (stm);
}



/*************************************************
 * 転送開始FADの設定(4.6)
 * stm	: ストリームハンドル
 * fad	: 転送開始FAD
 *************************************************/
STM_LOCAL void	stminit_setTrFad(StmHn stm, Sint32 fad)
{
    if (IS_STM_RESI(stm)) {
	GFS_Seek(STM_GFS(stm), 0, GFS_SEEK_SET);
    }
    GFS_TRN_STAT(&GFS_FILE_TRANS(STM_GFS(stm))) = GFTR_ST_NOACT;
                                      /* END 状態にならないようにする	*/
    STM_TRFAD(stm) = fad;
}


void	STM_SetTrFad(StmHn stm, Sint32 fad)
{
    STMERR_SetFunc(STM_SETTRFAD);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return;
    }
    if (fad < 0) {
	STMERR_SetCode(STM_ERR_NUM);
	STMERR_Exec();
	return ;
    }
    stm = STM_ALIAS(stm);
    stminit_setTrFad(stm, fad);
}


/*************************************************
 * 常駐ストリームのオープン(3.3)
 * grp	: ストリームグープ
 * fid	: ファイル識別子
 * key	: ストリームキー
 * fad	: 転送開始FAD
 * 戻り値 : ストリームハンドル(オープンできない場合NULL)
 ***********************************************/
StmHn	STM_OpenResi(StmGrpHn grp, Sint32 fid, StmKey *key, Sint32 fad)
{
    StmHn	stm;

    STMERR_SetFunc(STM_OPENRESI);
    if (grp == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (NULL);
    }
    if (fad < 0) {
	STMERR_SetCode(STM_ERR_NUM);
	STMERR_Exec();
	return (NULL);
    }
    stm = stminit_openFid(grp, fid, key, STM_ATR_LOOPNOREAD|STM_ATR_RESI);
                                       /* ファイル識別子によるオープン	*/
    if (stm == NULL) {
	return (NULL);
    }

    /* 転送開始FADの設定	*/
    stminit_setTrFad(stm, fad);
    return (stm);
}



/*************************************************
 * ストリームのクローズ(3.4)
 * stm	: ストリームハンドル
 *************************************************/
void	STM_Close(StmHn stm)
{
    STMERR_SetFunc(STM_CLOSE);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
    }
    stm = STM_ALIAS(stm);
    stminit_close(stm);
    if (IS_SET_ERRCODE(stm_mng_ptr) == TRUE) {
	STMERR_Exec();
    }
}


/*************************************************
 * ストリームキーの設定(3.5)
 * stm	: ストリームハンドル
 * stmkey : ストリームキー
 *************************************************/
void	STM_SetKey(StmHn stm, StmKey *stmkey)
{
    CdcSubh	subh;
    Sint32	fmode;
    GfsFinfo	*finfo;
    Sint32	ret;

    STMERR_SetFunc(STM_SETKEY);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return ;
    }

    stm = STM_ALIAS(stm);
    if (STM_IS_CDFILE(stm)) {
	/* サブヘッダ情報設定	*/
	CDC_SUBH_FN(&subh) = (Uint8)STM_KEY_FN(stmkey);
	CDC_SUBH_CN(&subh) = (Uint8)STM_KEY_CN(stmkey);
	CDC_SUBH_SMMSK(&subh) = (Uint8)STM_KEY_SMMSK(stmkey);
	CDC_SUBH_SMVAL(&subh) = (Uint8)STM_KEY_SMVAL(stmkey);
	CDC_SUBH_CIMSK(&subh) = (Uint8)STM_KEY_CIMSK(stmkey);
	CDC_SUBH_CIVAL(&subh) = (Uint8)STM_KEY_CIVAL(stmkey);
	fmode = stm_getFmode(stmkey);		/* 絞りモード取得	*/

	finfo = &(GFS_FLW_FINFO(STM_FLOW(stm)));
	GFCD_SetFilt(GFS_CDR_FLT(&GFS_FLW_DTSRC(STM_FLOW(stm))), fmode, &subh,
		     GFS_FI_FAD(finfo), GFS_FI_NSCT(finfo)); 	/* 絞り設定	  */
	ret = GFCD_WaitServer();
	if (ret != GFCD_ERR_OK) {
	    STM_SetGfcdErr(ret);
	    STMERR_Exec();
	}
    }
}


/*--- STMERR functions -------------------------------------------------*/

/*****************************************************
 * 処理関数の設定
 * funcid : 関数ID
 *****************************************************/
void	STMERR_SetFunc(Sint32 funcid)
{
    STM_ERR_WHERE(&MNG_ERR(stm_mng_ptr)) = funcid;
}


/*****************************************************
 * エラーコードの設定
 * code	: エラーコード
 *****************************************************/
void	STMERR_SetCode(Sint32 code)
{
    if (IS_SET_ERRCODE(stm_mng_ptr) == FALSE) {
	STM_ERR_CODE(&MNG_ERR(stm_mng_ptr)) = code;
    }
}


/*****************************************************
 * エラー関数の呼びだし
 * code	: エラーコード
 *****************************************************/
void	STMERR_Exec(void)
{
    if ((STM_ERR_FUNC(&MNG_ERR(stm_mng_ptr)) != STM_ERR_NULLFUNC)&&
	(IS_SET_ERRCODE(stm_mng_ptr) == TRUE)) {
	STM_ERR_FUNC(&MNG_ERR(stm_mng_ptr))(STM_ERR_OBJ(&MNG_ERR(stm_mng_ptr)), 
					STM_ERR_CODE(&MNG_ERR(stm_mng_ptr)));
    }
    STM_ERR_CODE(&MNG_ERR(stm_mng_ptr)) = STM_ERR_OK;
}


/* end of stm_init.c	*/


