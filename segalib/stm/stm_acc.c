/*****************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library: ストリームシステム
 * Module : その他の関数
 * File	  : stm_acc.c
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
#include	"..\gfs\gfs_def.h"
#include	"..\gfs\gfs_cdc.h"
#include	"..\gfs\gfs_cdf.h"
#include	"..\gfs\gfs_cdb.h"
#include	"..\gfs\gfs_trn.h"
#include	"..\gfs\gfs_buf.h"
#include	"..\gfs\gfs_dir.h"
#include	"stm_loc.h"
#include	"stm_def.h"

/*****************************************************************************
 *	関数の定義
 *****************************************************************************/

/*************************************************
 * ストリーム数の取得(2.3)
 * grp	: ストリームグループハンドル
 * 戻り値 : ストリーム数
 *************************************************/
Sint32	STM_GetStmNum(StmGrpHn grp)
{
    Sint32	nstm = 0;
    StmHn	stm;

    STMERR_SetFunc(STM_GETSTMNUM);
    if (grp == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (-1);
    }
    /* ストリームのリストをたぐる	*/
    for (stm = STM_GRP_SFADLST(grp); stm != NULL; stm = STM_SFADLST(stm)) {
	nstm++;
    }
    return (nstm);
}


/*************************************************
 * ストリームハンドルの取得(2.4)
 * grp	: ストリームグループハンドル
 * nstm	: 再生順番号(0 <= nstm < STM_GetStmNum)
 * 戻り値 : 指定した再生順のストリームハンドル
 *************************************************/
StmHn	STM_GetStmHndl(StmGrpHn grp, Sint32 nstm)
{
    Sint32	i;
    StmHn	stm = NULL;

    STMERR_SetFunc(STM_GETSTMHNDL);
    if (grp == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (NULL);
    }
    if (nstm < 0) {
	STMERR_SetCode(STM_ERR_NUM);
	STMERR_Exec();
	return (NULL);
    }
    for (i = 0, stm = STM_GRP_SFADLST(grp); (i < nstm)&&(stm != NULL);
	                                        stm = STM_SFADLST(stm)) {
	i++;
    }
    return (stm);
}


/*************************************************
 * バッファサイズ監視関数の登録(2.5)
 * grp	: ストリームグループハンドル
 * stm	: ストリームハンドル(空きセクタチェックの場合STM_CDBUF_REST)
 * nsct	: 空きセクタ数
 * func	: 呼びだし関数（取り消す場合 STM_CDBUF_NULLFUNC）
 * obj	: 呼びだし関数の第一引数
 *************************************************/
void	STM_SetCdbufFunc(StmGrpHn grp, StmHn stm,
			 Sint32 nsct, StmCdbufFunc func, void *obj)
{
    STMERR_SetFunc(STM_SETCDBUFFUNC);
    if (grp == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return ;
    }
    if (nsct < 0) {
	STMERR_SetCode(STM_ERR_NUM);
	STMERR_Exec();
	return ;
    }
    if (stm != STM_CDBUF_REST) {
	stm = STM_ALIAS(stm);
    }
    STM_GRP_CHKSTM(grp) = stm;
    STM_GRP_CHKSCTNUM(grp) = nsct;
    STM_GRP_CHKFUNC(grp) = func;
    STM_GRP_CHKOBJ(grp) = obj;
}


/*************************************************
 * ストリーム情報の取得(3.6)
 * stm	: ストリームハンドル
 * sfid	: ファイル識別子
 * frange : 再生範囲
 * bn	: バッファ区画番号
 * stmkey : ストリームキー
 * 戻り値 : 所属するストリームグループ
 *************************************************/
StmGrpHn STM_GetInfo(StmHn stm, Sint32 *fid, StmFrange *frange,
		                             Sint32 *bn, StmKey *stmkey)
{
    Sint32	sctsiz;
    Sint32	lastsiz;
    Sint32	atr;
    Sint32	fn;
    Sint32	fmode;
    Sint32	flt;
    CdcSubh	subh;

    STMERR_SetFunc(STM_GETINFO);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (NULL);
    }
    stm = STM_ALIAS(stm);
    /* ファイル情報	*/
    GFCF_GetFileInfo(STM_FLOW(stm), fid, &STM_FRANGE_SFAD(frange),
	&sctsiz, &STM_FRANGE_FASNUM(frange), &lastsiz, &fn, &atr);
    /* 再生範囲によりオープンされた場合	*/
    if (*fid == FID_OPEN_FRANGE) {
	*fid = -1;
    }
    /* 絞りモード	*/
    if (STM_IS_CDFILE(stm)) {
	*bn = GFS_CDR_BUFNO(STM_DTSRC(stm));	/* バッファ区画番号	*/
	flt = GFS_CDR_FLT(STM_DTSRC(stm));
	GFCD_GetFilt(flt, &fmode, &subh);
	STM_KEY_FN(stmkey) = (Sint16)CDC_SUBH_FN(&subh);
	STM_KEY_CN(stmkey) = (Sint16)CDC_SUBH_CN(&subh);
	STM_KEY_SMMSK(stmkey) = (Sint16)CDC_SUBH_SMMSK(&subh);
	STM_KEY_SMVAL(stmkey) = (Sint16)CDC_SUBH_SMVAL(&subh);
	STM_KEY_CIMSK(stmkey) = (Sint16)CDC_SUBH_CIMSK(&subh);
	STM_KEY_CIVAL(stmkey) = (Sint16)CDC_SUBH_CIVAL(&subh);
    } else {
	fmode = 0;
	*bn = -1;
    }
    if ((fmode & FMODE_FN) == 0) {		/* ファイル番号指定なし	*/
	STM_KEY_FN(stmkey) = STM_KEY_NONE;
    }
    if ((fmode & FMODE_CN) == 0) {		/* チャネル番号指定なし	*/
	STM_KEY_CN(stmkey) = STM_KEY_NONE;
    }
    if ((fmode & FMODE_SM) == 0) {		/* サブモード指定なし	*/
	STM_KEY_SMMSK(stmkey) = STM_KEY_SMVAL(stmkey) = STM_KEY_NONE;
    }
    if ((fmode & FMODE_CI) == 0) {		/* コーディング情報指定なし */
	STM_KEY_CIMSK(stmkey) = STM_KEY_CIVAL(stmkey) = STM_KEY_NONE;
    }
    return (STM_GRPHN(stm));
}


/*************************************************
 * ストリームの接続(8.1)
 * keystm : 接続元ストリームハンドル
 * bufstm : 接続先ストリームハンドル(ない場合NULL)
 *************************************************/
void	STM_ConnectCdbuf(StmHn keystm, StmHn bufstm)
{
    Sint32	bufno;
    Sint32	ret;

    STMERR_SetFunc(STM_CONNECTCDBUF);
    if (keystm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return ;
    }
    keystm = STM_ALIAS(keystm);
    if (bufstm == NULL) {
	bufno = CDC_NUL_SEL;
    } else {
	bufstm = STM_ALIAS(bufstm);
	bufno = GFS_CDR_BUFNO(STM_DTSRC(bufstm));
    }
    GFS_CDR_BUFNO(STM_DTSRC(keystm)) = bufno;
    GFCD_SetFiltCon(GFS_CDR_FLT(STM_DTSRC(keystm)), bufno, GFCD_ANY_FLT);
    ret = GFCD_WaitServer();
    if (ret != GFCD_ERR_OK) {
	STM_SetGfcdErr(ret);
	STMERR_Exec();
    }
}


/*************************************************
 * CDバッファデータの移動(8.2)
 * src	: 転送元ストリームハンドル
 * spos	: セクタ位置(先頭セクタは0)
 * snum : セクタ数
 * dst	: 転送先ストリームハンドル
 *************************************************/
void	STM_MoveCdbuf(StmHn src, Sint32 spos, Sint32 snum, StmHn dst)
{
    Sint32	ret;

    STMERR_SetFunc(STM_MOVECDBUF);
    if ((src == NULL)||(dst == NULL)) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return ;
    }
    if ((spos < 0)||(snum < 0)) {
	STMERR_SetCode(STM_ERR_NUM);
	STMERR_Exec();
	return ;
    }
    src = STM_ALIAS(src);
    dst = STM_ALIAS(dst);
    GFCD_MoveSctData(GFS_CDR_BUFNO(STM_DTSRC(dst)), 
		     GFS_CDR_BUFNO(STM_DTSRC(src)), spos, snum);

    /* TRENDの場合、転送セクタ数が設定されているので一旦クリアする	*/
    STM_ATR(dst) &= ~STM_ATR_TREND;
    ret = GFCD_WaitServer();
    if (ret != GFCD_ERR_OK) {
	STM_SetGfcdErr(ret);
	STMERR_Exec();
    }
}


/*************************************************
 * CDバッファデータの消去(8.3)
 * stm	: ストリームハンドル
 * spos	: 消去位置
 * snum	: 消去セクタ数
 *************************************************/
void	STM_EraseCdbuf(StmHn stm, Sint32 spos, Sint32 snum)
{
    Sint32	ret;

    STMERR_SetFunc(STM_ERASECDBUF);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return ;
    }
    if ((spos < 0)||(snum < 0)) {
	STMERR_SetCode(STM_ERR_NUM);
	STMERR_Exec();
	return ;
    }
    stm = STM_ALIAS(stm);
    GFTR_Stop(STM_GFS(stm), 0);			/* データパック返還	*/
    GFS_TRN_STAT(&GFS_FILE_TRANS(STM_GFS(stm))) = GFTR_ST_NOACT;
                                      /* END 状態にならないようにする	*/
    if (STM_IS_CDFILE(stm)) {
	STL_DeleteSctData(STM_FLOW(stm), spos, snum);
                                                /* バッファデータ消去	*/
	ret = GFCD_WaitServer();
	if (ret != GFCD_ERR_OK) {
	    STM_SetGfcdErr(ret);
	    STMERR_Exec();
	}
    } else {
	GFS_Seek(STM_GFS(stm), 0, GFS_SEEK_SET);
    }
}


/*************************************************
 * セクタ数からバイト数への変換(3.7)
 * stm	: ストリームハンドル
 * nsct	: セクタ数
 * 戻り値 : バイト数
 *************************************************/
Sint32	STM_SctToWord(StmHn stm, Sint32 nsct)
{
    Sint32	nbyte;

    STMERR_SetFunc(STM_SCTTOWORD);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (-1);
    }
    if (nsct < 0) {
	STMERR_SetCode(STM_ERR_NUM);
	STMERR_Exec();
	return (-1);
    }
    stm = STM_ALIAS(stm);
    GFBF_GetTranData(STM_GFS(stm), nsct, &nbyte);
    return (nbyte / sizeof(Uint16));
}


/*************************************************
 * ワード数からセクタ数への変換(3.8)
 * stm	: ストリームハンドル
 * nword: ワード数
 * 戻り値 : セクタ数
 *************************************************/
Sint32	STM_WordToSct(StmHn stm, Sint32 nword)
{
    Sint32	i;
    Sint32	nsct;
    Sint32	nw = 0;

    STMERR_SetFunc(STM_WORDTOSCT);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (-1);
    }
    if (nword < 0) {
	STMERR_SetCode(STM_ERR_NUM);
	STMERR_Exec();
	return (-1);
    }
    stm = STM_ALIAS(stm);
    if (nword == 0) {
	return (0);
    }
    if (STM_IS_CDFILE(stm)) {
	nsct = GFCB_GetLenData(STM_FLOW(stm), LONG_MAX, NULL);	
	                                              /* 総セクタ数取得	*/
	/* 実データサイズ取得 */
	for (i = 0; i < nsct; i++) {
	    nw += STL_GetActSiz(STM_FLOW(stm), i);
	    if (nw >= nword) {
		i++;
		break;
	    }
	}
    } else {
	for (i = 1; ; i++) {
	    nsct = GFBF_GetTranData(STM_GFS(stm), i, &nw);
	    nw /= sizeof(Uint16);
	    if (nw >= nword) {
		break;
	    }
	    if (i > nsct) {
		i--;
		break;
	    }
	}
    }
    return (i);
}


/*************************************************
 * ストリームハンドルのエイリアス設定(3.9)
 * alias : エイリアスを設定するストリームハンドル
 * stm	 : アクセスするストリームハンドル
 *************************************************/
Bool	STM_SetAlias(StmHn alias, StmHn stm)
{
    StmHn	chk;
    Sint32	i;

    STMERR_SetFunc(STM_SETALIAS);
    if (alias == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (FALSE);
    }
    /* 既にオープンされているストリームであることを確認する		*/
    if (STM_SVRSTAT(stm) == SVR_UNUSE) {
	return (FALSE);
    }
    /* 既にエイリアスが設定されているストリームでないことを確認する	*/
    for (i = 0, chk = MNG_STM(stm_mng_ptr, 0); i < MNG_STMMAX(stm_mng_ptr); i++, chk++) {
	if ((STM_SVRSTAT(chk) == SVR_USE)&&
	    (chk != stm)&&(STM_ALIAS(chk) == stm)) {
	    return (FALSE);
	}
    }
    STM_ALIAS(alias) = stm;
    return (TRUE);
}


/*************************************************
 * ストリームハンドルのエイリアス解除(3.10)
 * alias : エイリアスを解除するストリームハンドル
 *************************************************/
void	STM_UnsetAlias(StmHn alias)
{
    STMERR_SetFunc(STM_UNSETALIAS);
    if (alias == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return ;
    }
    STM_ALIAS(alias) = alias;
}


/*************************************************
 * 転送ゲートの開閉(4.4)
 * stm	: ストリームハンドル
 * date	: ゲートの状態
 *************************************************/
void	STM_SetTrGate(StmHn stm, Sint32 gate)
{
    STMERR_SetFunc(STM_SETTRGATE);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return ;
    }
    stm = STM_ALIAS(stm);
    if (gate == STM_GATE_OPEN) {
	GFTR_Start(&(GFS_FILE_TRANS(STM_GFS(stm))));
    } else {
	GFTR_Pause(&(GFS_FILE_TRANS(STM_GFS(stm))));
    }
}


/*************************************************
 * 最大転送セクタ数の設定(4.5)
 * stm	: ストリームハンドル
 * tsct	: 最大転送セクタ数
 *************************************************/
void	STM_SetTrPara(StmHn stm, Sint32 tsct)
{
    STMERR_SetFunc(STM_SETTRPARA);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return ;
    }
    if (tsct < 0) {
	STMERR_SetCode(STM_ERR_NUM);
	STMERR_Exec();
	return ;
    }
    stm = STM_ALIAS(stm);
    GFTR_SetTransMax(&(GFS_FILE_TRANS(STM_GFS(stm))), tsct);
}


/*************************************************
 * 転送モードの設定(4.7)
 * stm	: ストリームハンドル
 * tmode: 転送モード
 *************************************************/
void	STM_SetTrMode(StmHn stm, Sint32 tmode)
{
    STMERR_SetFunc(STM_SETTRMODE);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return ;
    }
    if ((tmode != STM_TR_SCU)&&(tmode != STM_TR_SDMA0)&&
	(tmode != STM_TR_SDMA1)&&(tmode != STM_TR_CPU)) {
	STMERR_SetCode(STM_ERR_PARA);
	STMERR_Exec();
	return ;
    }
    stm = STM_ALIAS(stm);
    GFTR_SetMode(&(GFS_FILE_TRANS(STM_GFS(stm))), tmode);
}


/*************************************************
 * 読み込みセクタ情報の取得(5.2)
 * stm	: ストリームハンドル
 * sn	: セクタ番号（先頭セクタは０）
 * sinfo: セクタ情報
 * 戻り値: TRUE 指定セクタあり  FALSE 指定セクタなし
 *************************************************/
Bool	STM_GetSctInfo(StmHn stm, Sint32 sn, StmSct *sinfo)
{
    Bool	ret;

    STMERR_SetFunc(STM_GETSCTINFO);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (FALSE);
    }
    if (sn < 0) {
	STMERR_SetCode(STM_ERR_NUM);
	STMERR_Exec();
	return (FALSE);
    }
    stm = STM_ALIAS(stm);
    if (STM_IS_CDFILE(stm)) {
	ret = STL_GetSctInfo(STM_FLOW(stm), sn, sinfo);
    } else {
	if (sn < STMSVR_GetNumCdbuf(stm)) {
	    STM_SCT_FAD(sinfo) = LONG_MAX;
	    STM_SCT_FN(sinfo) = 0;
	    STM_SCT_CN(sinfo) = 0;
	    STM_SCT_SM(sinfo) = 0;
	    STM_SCT_CI(sinfo) = 0;
	    ret = TRUE;
	} else {
	    ret = FALSE;
	}
    }
    return (ret);
}


/*************************************************
 * ループカウンタの取得(7.6)
 * grp	  : ストリームグループハンドル
 * 戻り値 : ループカウンタ
 *************************************************/
Sint32	STM_GetLoopCnt(StmGrpHn grp)
{
    STMERR_SetFunc(STM_GETLOOPCNT);
    if (grp == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (-1);
    }
    return (STM_GRP_LOOPCNT(grp));
}


/*************************************************
 * 実行状態の取得(7.7)
 * grp	: ストリームグループハンドル
 * fad	: 再生中のFAD
 *************************************************/
Sint32	STM_GetExecStat(StmGrpHn grp, Sint32 *fad)
{
    Sint32	pos;
    Uint16	hirq;

    STMERR_SetFunc(STM_GETEXECSTAT);
    if (grp == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (-1);
    }
    /* 処理中グループの場合	*/
    if (MNG_CURGRP(stm_mng_ptr) == grp) {
	if ((IS_PLAYING(grp) == TRUE)&&
	    (IS_READING(grp) == TRUE)) {
	    GFCF_GetStat(&pos, &hirq);	/* 再生FADを取得	*/
	    if (pos >= 0) {
		STM_GRP_FAD(grp) = pos;
	    }
	}
    }
    *fad = STM_GRP_FAD(grp);
    return (STM_GRP_ACSTAT(grp));
}


/*************************************************
 * エラー発生時の呼びだし関数の登録(9.1)
 * func	: 呼びだし関数
 * obj	: 呼びだし関数第一引数
 *************************************************/
void	STM_SetErrFunc(StmErrFunc func, void *obj)
{
    STM_ERR_CODE(&MNG_ERR(stm_mng_ptr)) = STM_ERR_OK;
    STM_ERR_FUNC(&MNG_ERR(stm_mng_ptr)) = func;
    STM_ERR_OBJ(&MNG_ERR(stm_mng_ptr)) = obj;
}


/*************************************************
 * エラー状態の取得(9.2)
 * stat : エラー状態
 *************************************************/
void	STM_GetErrStat(StmErrStat *stat)
{
    memcpy(stat, &MNG_ERR(stm_mng_ptr), sizeof(StmErrStat));
}


/* end of stm_acc.c	*/
