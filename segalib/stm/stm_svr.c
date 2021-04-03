/*****************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library: ストリームシステム
 * Module : サーバモジュール
 * File	  : stm_svr.c
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

/* コマンド登録領域	*/
StmCmd	stm_cmd_area;

/*****************************************************************************
 *	関数の定義
 *****************************************************************************/
/*--- STMMNG functions -------------------------------------------------*/
extern	Sint32	STMMNG_ResetLstStm(StmGrpHn grp);

/*****************************************************
 * CDストリームがないかどうかを調べる
 * grp	: ストリームグループ
 * 戻り値 : TRUE CDストリームはない
 *****************************************************/
STM_LOCAL Bool	stmmng_isNoCdFile(StmGrpHn grp)
{
    StmHn	stm;

    /* 常駐ストリームはループストリームにならない	*/
    if (STM_GRP_LOOPSTM(grp) != NULL) {
	return (FALSE);
    }
    for (stm = STM_GRP_SFADLST(grp); stm != NULL; stm = STM_SFADLST(stm)) {
	if (STM_IS_CDFILE(stm) == TRUE) {
	    return (FALSE);
	}
    }
    return (TRUE);
}


/*************************************************
 * CSCTビット関数の登録
 * grp : 実行中ストリームグループ
 *************************************************/
STM_LOCAL void	stm_execCsct(void *grp)
{
    Sint32	nsct;

    STM_GRP_ATR((StmGrpHn)grp) |= STMGRP_ATR_READ;
    if (STM_GRP_CHKFUNC((StmGrpHn)grp) != STM_CDBUF_NULLFUNC) {
	if (STM_GRP_CHKSTM((StmGrpHn)grp) == STM_CDBUF_REST) {
	    nsct = STL_CsctGetFreeSctnum();
	    if (nsct < STM_GRP_CHKSCTNUM((StmGrpHn)grp)) {
		STM_GRP_ATR((StmGrpHn)grp) |= STMGRP_ATR_CHKBUF;
	    }
	} else {
	    nsct = STL_CsctGetSctNum(STM_FLOW(STM_GRP_CHKSTM((StmGrpHn)grp)));
	    if (nsct >= STM_GRP_CHKSCTNUM((StmGrpHn)grp)) {
		STM_GRP_ATR((StmGrpHn)grp) |= STMGRP_ATR_CHKBUF;
	    }
	}
    }
}


/*************************************************
 * ステータスによるエラーの設定
 * stat	: ドライブのステータス
 *************************************************/
STM_LOCAL void	stm_setErr(Sint32 stat)
{
    Sint32	code = STM_ERR_OK;

    if (stat == GFS_STAT_OPEN) {
	code = STM_ERR_CDOPEN;
    } else if (stat == GFS_STAT_NODISC) {
	code = STM_ERR_CDNODISC;
    } else if (stat == GFS_STAT_FATAL) {
	code = STM_ERR_FATAL;
    } else if (stat == GFS_STAT_ERROR) {
	code = STM_ERR_CDRD;
    }
    if (code != STM_ERR_OK) {
	STMERR_SetCode(code);
    }
}
	

/*--- stmtsk functions -------------------------------------------------*/

/*************************************************
 * サーバ実行グループの変更
 * grp	  : 変更後のストリームグループハンドル
 * 戻り値 : TRUE	処理完了
            FALSE	処理中
 *************************************************/
STM_LOCAL Bool	stmtsk_setExecGrp(void *_grp)
{
    StmGrpHn	grp, oldgrp;
    StmHn	stm;
    Sint32	fltno;
    Sint32	fad;
    Sint32	lst_fad;
    Sint32	puid;
    Sint32	stat;
    Sint32	ret;
    StmHn	next;
    Uint16	hirq;
    Bool	result = TRUE;

    grp = (StmGrpHn)_grp;
    if (MNG_CURGRP(stm_mng_ptr) != grp) {
	oldgrp = MNG_CURGRP(stm_mng_ptr);
	/* 読み込み中のストリームグループを停止する	*/
	if (oldgrp != NULL) {
	    stat = GFCF_GetStat(&fad, &hirq);
	    stm_setErr(stat);			/* エラーコードの設定	*/
	    /* ドライブを停止する	*/
	    if ((IS_PLAYING(oldgrp) == TRUE)&&
		(IS_PAUSING(oldgrp) == FALSE)) {
		ret = STL_StopIn(oldgrp);
		if (ret != STM_ERR_OK) {
		    STMERR_SetCode(ret);
		}
		STM_GRP_ATR(oldgrp) |= STMGRP_ATR_PAUSE;
	    }
	    /* ピックアップを解放する	*/
	    if (STM_GRP_PUID(oldgrp) >= 0) {
		GFCD_UngetPickup(STM_GRP_PUID(oldgrp));
		STM_GRP_PUID(oldgrp) = -1;
		GFCD_SetCsctFunc(GFCD_NULLFUNC, NULL);
	    }
	    /* グループが処理中の場合	*/
	    if (STM_GRP_ACSTAT(oldgrp) != STM_EXEC_COMPLETED) {
		STM_GRP_ACSTAT(oldgrp) = STM_EXEC_PAUSE;	/* ポーズ設定	*/
	        /* COMPLETEDの場合ステータスは変化しない	*/
	    }
	    /* 停止完了			*/
	    if (stat == GFS_STAT_NOACT) {
		next = STM_GRP_NEXTTRNS(oldgrp);
		if (next != NULL) {
		    GFTR_Stop(STM_GFS(next), GFTR_ALLSCT);
		    GFS_TRN_STAT(&GFS_FILE_TRANS(STM_GFS(next))) = GFTR_ST_NOACT;
                                      /* END 状態にならないようにする	*/
		}
		if (IS_PAUSING(oldgrp) == TRUE) {
		    lst_fad = STMMNG_GetLstFad(oldgrp);
		    /* 読み込みを完了していない場合FADを更新する	*/
		    fad = STL_GetCurFad();
		    if (STM_GRP_FAD(oldgrp) <= lst_fad) {
			STM_GRP_FAD(oldgrp) = fad;
		    }
		}
		STM_GRP_ATR(oldgrp) &= ~(STMGRP_ATR_PLAY|STMGRP_ATR_READ|STMGRP_ATR_SEEK|STMGRP_ATR_PAUSE);
		oldgrp = NULL;
	    }
	}

	/* 新しいグループを設定する			*/
	if (oldgrp == NULL) {
	    if (grp != NULL) {
		if (STM_GRP_PUID(grp) < 0) {
		    puid = GFCD_GetPickup();
		    if (puid >= GFCD_ERR_OK) {
			STM_GRP_PUID(grp) = puid;
		    }
		    GFCD_SetCsctFunc(stm_execCsct, grp);
		}
		if (STM_GRP_PUID(grp) >= 0) {
		    /* CDデバイスの接続変更	*/
		    stm = STM_GRP_FLTLST(grp);
		    if (stm != NULL) {
			fltno = GFS_CDR_FLT(STM_DTSRC(stm));
		    } else {
			fltno = CDC_NUL_SEL;
		    }
		    GFCD_SetCon(fltno, STM_GRP_PUID(grp));
		    ret = GFCD_WaitServer();
		    if (ret != GFCD_ERR_OK) {
			STM_SetGfcdErr(ret);
		    }
		    STM_GRP_ATR(grp) &= ~(STMGRP_ATR_PLAY|STMGRP_ATR_READ|STMGRP_ATR_SEEK);
		} else {
		    STMERR_SetCode(STM_ERR_PUINUSE);
		}
	    }
	    MNG_CURGRP(stm_mng_ptr) = grp;
	    result = TRUE;
	} else {
	    result = FALSE;
	}
    }
    return (result);
}



/*--- STM functions ----------------------------------------------------*/

/*************************************************
 * サーバ実行グループの指定(7.1)
 * grp	: ストリームグループハンドル
 *************************************************/
Bool	STM_NwSetExecGrp(StmGrpHn grp)
{
    if (CMD_FUNC(&stm_cmd_area) != CMD_NULLFUNC) {
	return (FALSE);
    }
    STMERR_SetFunc(STM_SETEXECGRP);
    CMD_FUNC(&stm_cmd_area) = stmtsk_setExecGrp;
    CMD_OBJ(&stm_cmd_area) = grp;
    return (TRUE);
}


/*************************************************
 * サーバ実行グループの指定(7.2)
 * grp	: ストリームグループハンドル
 *************************************************/
Bool	STM_SetExecGrp(StmGrpHn grp)
{
    Bool	complete;
    Sint32	stat;

    complete = STM_NwSetExecGrp(grp);
    if (complete == FALSE) {
	return (FALSE);
    }
    do {
	stat = STM_ExecServer();
    } while (stat != STM_EXEC_TSKEND);
    return (TRUE);
}



/*************************************************
 * ストリームデータ転送中の取得(6.3)
 * stm	: ストリームハンドル
 * 戻り値 : TRUE	転送中
 *	  : FALSE	転送していない
 *************************************************/
STM_LOCAL Bool	stmsvr_isTrans(StmHn stm)
{
    Bool	trans;

    if (GFS_TRN_DPS(&(GFS_FILE_TRANS(STM_GFS(stm)))) != NULL) {
	trans = TRUE;
    } else {
	trans = FALSE;
    }
    return (trans);
}


Bool	STM_IsTrans(StmHn stm)
{
    Bool	trans;

    STMERR_SetFunc(STM_ISTRANS);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (FALSE);
    }
    stm = STM_ALIAS(stm);
    trans = stmsvr_isTrans(stm);
    return (trans);
}



/*************************************************
 * 読み込みフラグの解除
 * grp	: ストリームグループハンドル
 *************************************************/
STM_LOCAL void	stm_setReadFalse(StmGrpHn grp)
{
    Sint32	fad;
    StmHn	stm;
    Sint32	sfad, efad;

    fad = STM_GRP_FAD(grp);
    for (stm = STM_GRP_SFADLST(grp); stm != NULL; stm = STM_SFADLST(stm)) {
	/* ストリームの終了FADを取得	*/
	STL_GetPlayArea(STM_FLOW(stm), &sfad, &efad);
	if (sfad > fad) {	/* 開始セクタが現在位置以降の場合    */
	    GFS_FILE_ASTAT(STM_GFS(stm)) = GFS_SVR_COMPLETED;
	    STM_ATR(stm) &= ~STM_ATR_READ;
	}
    }
}


/*************************************************
 * 転送領域の初期化
 * grp	: ストリームグループハンドル
 *************************************************/
STM_LOCAL void	stm_resetTrbuf(StmGrpHn grp)
{
    StmHn	stm;

    for (stm = STM_GRP_SFADLST(grp); stm != NULL; stm = STM_SFADLST(stm)) {
	GFS_Seek(STM_GFS(stm), 0, GFS_SEEK_SET);
	STMTRN_ResetTrBuf(stm);
    }
}


/*************************************************
 * ループ開始ストリームの指定(7.5)
 * grp	: ストリームグループハンドル
 * stm	: ループ開始ストリーム
 * loopmax : ループ回数(無限回の場合STM_LOOP_ENDLESS)
 *************************************************/
STM_LOCAL Bool	stmsvr_setLoop(StmGrpHn grp, StmHn stm, Sint32 loopmax)
{
    Bool	ret = TRUE;

    if (STM_GRP_ACSTAT(grp) == STM_EXEC_COMPLETED) {
	stm_resetTrbuf(grp);
    }
    if (stm == STM_LOOP_DFL) {
	/* 読み込みの終っていない先頭のストリームをサーチする	*/
	for (stm = STM_GRP_SFADLST(grp); stm != NULL; stm = STM_SFADLST(stm)) {
	    if (IS_STM_READEND(stm) == FALSE) {
		break;
	    }
	}
	STM_GRP_LOOPSTM(grp) = stm;
	CLR_LOOPSPECIFY(grp);
    } else if ((stm == NULL)||(IS_STM_RESI(stm) == FALSE)) {
	if (stm != NULL) {
	    stm = STM_ALIAS(stm);
	}
	STM_GRP_LOOPSTM(grp) = stm;
	SET_LOOPSPECIFY(grp);
    } else {
	ret = FALSE;
    }
    STM_GRP_LOOPMAX(grp) = loopmax;
    STM_GRP_LOOPCNT(grp) = 0;
    return (ret);
}


Bool	STM_SetLoop(StmGrpHn grp, StmHn stm, Sint32 loopmax)
{
    Bool	ret;

    STMERR_SetFunc(STM_SETLOOP);
    if (grp == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (FALSE);
    }
    if (loopmax < 0) {
	STMERR_SetCode(STM_ERR_NUM);
	STMERR_Exec();
	return (FALSE);
    }
    ret = stmsvr_setLoop(grp, stm, loopmax);
    return (ret);
}


/*************************************************
 * 読み込みフラグの設定
 * grp	: ストリームグループハンドル
 *************************************************/
STM_LOCAL void	stm_setReadTrue(StmGrpHn grp)
{
    Sint32	fad;
    StmHn	stm;
    Sint32	sfad, efad;

    fad = STM_GRP_FAD(grp);
    for (stm = STM_GRP_SFADLST(grp); stm != NULL; stm = STM_SFADLST(stm)) {
	/* ストリームの開始FADを取得	*/
	STL_GetPlayArea(STM_FLOW(stm), &sfad, &efad);
	/* 開始セクタが現在位置以降,または常駐ストリームの場合    */
	if (((sfad >= fad)&&(IS_STM_READEND(stm) == FALSE))||
	    (IS_STM_RESI(stm) == TRUE)) {
	    STM_ATR(stm) |= STM_ATR_READ;
	    GFS_FILE_ASTAT(STM_GFS(stm)) = GFS_SVR_BUSY;
	}
    }
}



/*************************************************
 * 読み込み終了フラグの設定
 * grp	: ストリームグループハンドル
 *************************************************/
void	STMSVR_SetReadEnd(StmGrpHn grp)
{
    Sint32	fad;
    StmHn	stm;
    Sint32	sfad, efad;
    Sint32	nsct;
    Sint32	loopmax, loopcnt;

    fad = STM_GRP_FAD(grp);
    for (stm = STM_GRP_FLTLST(grp); stm != NULL; stm = STM_FLTLST(stm)) {
	/* ストリームの開始FADを取得	*/
	STL_GetPlayArea(STM_FLOW(stm), &sfad, &efad);
	/* 終了セクタが現在位置以前ですでに読み込まれている場合	*/
	if ((efad < fad)&&(IS_STM_READ(stm) == TRUE)&&
	    (IS_STM_LOOPNOREAD(stm) == TRUE)) {
	    STM_ATR(stm) |= STM_ATR_READEND;
	    STMMNG_RemoveFlt(stm);
	    if (STM_GRP_LOOPSTM(grp) == stm) {
		loopmax = STM_GRP_LOOPMAX(grp);
		loopcnt = STM_GRP_LOOPCNT(grp);
		stmsvr_setLoop(grp, STM_LOOP_DFL, loopmax);
		STM_GRP_LOOPCNT(grp) = loopcnt;
	    }
	    if (STM_GRP_LSTSTM(grp) == stm) {
		STMMNG_ResetLstStm(grp);
	    }
	}
    }
}


/*************************************************
 * 再生範囲のチェック
 * grp	: ストリームグループ
 * fad	: 現在の再生FAD
 * 戻り値: 正  移動先FAD
 *	   負  移動不用
 *************************************************/
STM_LOCAL Sint32	stm_checkArea(StmGrpHn grp, Sint32 fad)
{
    StmHn	stm;
    Sint32	sfad, efad;

    if (fad < STM_GRP_FAD(grp)) {
	fad = STM_GRP_FAD(grp);
    }
    for (stm = STM_GRP_SFADLST(grp); stm != NULL; stm = STM_SFADLST(stm)) {
	if (IS_STM_READEND(stm) == FALSE) {
	    STL_GetPlayArea(STM_FLOW(stm), &sfad, &efad);
	    /* 再生位置が有効範囲内の場合		*/
	    if ((sfad <= fad)&&(efad >= fad)) {
		break;
	    }
	    /* 次のストリームの開始位置に移動する	*/
	    if (sfad > fad) {
		return (sfad);
	    }
	}
    }
    return (-1);			/* 移動する必要なし		*/
}

	

/*************************************************
 * ループストリームのFADの取得
 * grp	: ストリームグループ
 *************************************************/
STM_LOCAL Sint32	stm_getLoopFad(StmGrpHn grp)
{
    Sint32	sfad, efad;

    /* ループストリームの開始FADを取得	*/
    STL_GetPlayArea(STM_FLOW(STM_GRP_LOOPSTM(grp)), &sfad, &efad);
    return (sfad);
}



/*************************************************
 * 転送フラグのリセット
 * grp	: ストリームグループ
 *************************************************/
STM_LOCAL void	stm_resetTrflag(StmGrpHn grp)
{
    StmHn	stm;

    for (stm = STM_GRP_FLTLST(grp); stm != NULL; stm = STM_FLTLST(stm)) {
	STM_ATR(stm) &= ~STM_ATR_TRSTART;
	STM_ATR(stm) &= ~STM_ATR_TREND;
    }
}



/*************************************************
 * 再生パラメータの取得
 * grp	: ストリームグループ
 * fad	: 現在のFAD
 * efad	: ストリームグループの終了FAD
 * seekpos : ピックアップの移動先
 * start : 再生を開始するかどうか
 * 戻り値 : TRUE 読み込み終了  FALSE 読み込み中
 *************************************************/
STM_LOCAL Bool	stm_getReadParam(StmGrpHn grp, Sint32 fad, Sint32 efad, Sint32 stat,
			                         Sint32 *seekpos, Bool *start)
{
    Sint32	ret = FALSE;
    Uint16	hirq;
    Sint32	cdstat;

    if (STM_GRP_LOOPCNT(grp) == STM_GRP_LOOPMAX(grp)) {
	if (STM_GRP_FAD(grp) > efad) {
	    /* 全ての読み込みを終了している場合		*/
	    *seekpos = -1;
	    *start = FALSE;
	    ret = TRUE;
	    return (ret);
	} else if (stat == GFS_STAT_NOACT) {
	    /* 再生範囲を越えた後,ストリームが追加された場合	*/
	    cdstat = GFCF_GetStat(NULL, &hirq);
	    stm_setErr(cdstat);			/* エラーコードの設定	*/
	    /* 再生を終了している場合		*/
	    if (fad > MNG_EFAD(stm_mng_ptr)) {
		STM_GRP_FAD(grp) = STL_GetCurFad();
		STM_GRP_LOOPCNT(grp)--;
	    }
	}
    } else {
	if ((STM_GRP_FAD(grp) > efad)&&
	    (IS_PLAYING(grp) == FALSE)) {
	    if (STM_GRP_LOOPSTM(grp) == NULL) {
		/* 全ての読み込みを終了している場合		*/
		*seekpos = -1;
		*start = FALSE;
		ret = TRUE;
		return (ret);
	    }
	    STM_GRP_FAD(grp) = stm_getLoopFad(grp);
	    stm_resetTrflag(grp);
	}
    }
    /* 読み込みを開始していない場合	*/
    if (IS_PLAYING(grp) == FALSE) {
	if (STM_GRP_PUID(grp) >= 0) {
	    /* ピックアップを使用できる場合	*/
	    *seekpos = stm_checkArea(grp, STM_GRP_FAD(grp));
	    if (*seekpos >= 0) {
		STM_GRP_FAD(grp) = *seekpos;
	    }

	    *start = TRUE;			/* 読み込みを開始する	*/
	    STM_GRP_ACSTAT(grp) = STM_EXEC_DOING;
	    *seekpos = -1;
	} else {
	    /* ファイルシステム使用中に呼ばれた場合など	*/
	    *seekpos = -1;
	    *start = FALSE;			/* 読み込みを開始しない	*/
	}
    } else if (IS_READING(grp) == TRUE) {
	if (fad <= efad) {
	    cdstat = GFCF_GetStat(NULL, &hirq);
	    stm_setErr(cdstat);			/* エラーコードの設定	*/
	    /* 再生を終了している場合	*/
	    if ((stat == GFS_STAT_NOACT)&&(fad > MNG_EFAD(stm_mng_ptr))) {
		/* 次回再生開始させる		*/
		STM_GRP_FAD(grp) = fad;
		/* 再生終了,読み込み終了	*/
		STM_GRP_ATR(grp) &= ~(STMGRP_ATR_PLAY|STMGRP_ATR_READ|STMGRP_ATR_SEEK);
		*seekpos = -1;
	    } else {
		/* 読み込み中		*/
		STM_GRP_FAD(grp) = fad;
		if (IS_SEEKING(grp) == FALSE) {
		    *seekpos = stm_checkArea(grp, fad);
		    if (*seekpos >= 0) {
			STM_GRP_ATR(grp) |= STMGRP_ATR_SEEK;
		    }
		} else {
		    if (stat != GFS_STAT_SEEK) {
			STM_GRP_ATR(grp) &= ~STMGRP_ATR_SEEK;
		    }
		    *seekpos = -1;
		}
	    }
	    *start = FALSE;			/* 読み込みを開始しない	*/
	} else if (stat == GFS_STAT_NOACT) {
	    /* 読み込み終了		*/
	    STM_GRP_FAD(grp) = fad;
	    /* 再生終了,読み込み終了	*/
	    STM_GRP_ATR(grp) &= ~(STMGRP_ATR_PLAY|STMGRP_ATR_READ|STMGRP_ATR_SEEK);
	    STMSVR_SetReadEnd(grp);		/* 読み込み済みフラグの設定	*/
	    STM_GRP_LOOPCNT(grp)++;
	    *start = FALSE;
	    *seekpos = -1;
	    if ((STM_GRP_LOOPSTM(grp) == NULL)||
		(STM_GRP_LOOPCNT(grp) >= STM_GRP_LOOPMAX(grp))){
		ret = TRUE;
	    }
	} else {
	    /* 読み込み終了後のビジー状態	*/
	    *seekpos = -1;
	    *start = FALSE;
	}
    } else {	/* 再生開始後のビジー状態	*/
	*seekpos = -1;
	*start = FALSE;
    }
    return (ret);
}



/*************************************************
 * 再生コマンドの発行
 * grp	: ストリームグループ
 * seekpos : ピックアップの移動先
 * efad : ストリームグループの最終FAD
 * start : 再生を開始するかどうか
 *************************************************/
STM_LOCAL Sint32	stm_putPlayCmd(StmGrpHn grp, Sint32 seekpos, Sint32 efad, Bool start)
{
    Sint32	ret = STM_ERR_OK;

    if (seekpos >= 0) {			/* 次のストリームに移動	*/
	ret = STL_MovePlaypos(grp, seekpos);
	STM_GRP_FAD(grp) = seekpos;		/* 現在のFAD		*/
    } else if (STM_GRP_FAD(grp) <= efad){
	/* まだ読み込み状態になっていない場合	*/
	if ((start == TRUE)&&(IS_PLAYING(grp) == FALSE)) {
	    stm_setReadTrue(grp);		/* 読み込みフラグの設定 */
	    ret = STL_FlowIn(grp);		/* 読み込み開始		*/
	    if (ret == STM_ERR_OK) {
		STM_GRP_ATR(grp) |= STMGRP_ATR_PLAY;
	    }
	}
    }
    return (ret);
}



/*************************************************
 * ストリーム読み込み
 * grp	: 再生ストリームグループ
 * fad	: 現在のFAD
 * efad : ストリームグループの最終FAD
 * stat	: CDブロックの状態
 * 戻り値 : TRUE 読み込み終了    FALSE 読み込み中
 *************************************************/
STM_LOCAL Bool	stm_read(StmGrpHn grp, Sint32 fad, Sint32 efad, Sint32 stat)
{
    Sint32	seekpos;
    Bool	start;
    Bool	ret;
    Sint32	err;

    /* 全ての読み込みを終了した場合			*/
    if (STM_GRP_FLTLST(grp) == NULL) {
	/* 全て常駐ストリームの場合、転送のみ実行する	*/
	if (((STM_GRP_ACSTAT(grp) == STM_EXEC_PAUSE)||
	    (STM_GRP_ACSTAT(grp) == STM_EXEC_COMPLETED))&&
	    (STM_GRP_SFADLST(grp) != NULL)){
	    stm_setReadTrue(grp);
	    STM_GRP_FAD(grp) = STM_FAD_CDEND;
	    STM_GRP_ACSTAT(grp) = STM_EXEC_DOING;
	}
	return (TRUE);
    }

    /* 再生パラメータの取得				*/
    ret = stm_getReadParam(grp, fad, efad, stat, &seekpos, &start);

    /* 再生コマンドの発行				*/
    err = stm_putPlayCmd(grp, seekpos, efad, start);
    if (err != STM_ERR_OK) {
	STMERR_SetCode(err);
    }
    return (ret);
}



/*************************************************
 * CDバッファ内セクタ数の取得(5.1)
 * stm	: ストリームハンドル
 * 戻り値: CDバッファ内のセクタ数
 *************************************************/
Sint32	STMSVR_GetNumCdbuf(StmHn stm)
{
    Sint32	nsct;
    Sint32	ret;
    
    if (STM_IS_CDFILE(stm)) {
	ret = GFCD_GetLenData(GFS_CDR_BUFNO(STM_DTSRC(stm)), 0, 
			      LONG_MAX, &nsct, NULL);
	if (ret != GFCD_ERR_OK) {
	    STM_SetGfcdErr(ret);
	    return 0;
	}
	ret = GFCD_WaitServer();
	if (ret != GFCD_ERR_OK) {
	    STM_SetGfcdErr(ret);
	    return 0;
	}
    } else {
	nsct = GFBF_GetNumData(STM_GFS(stm));
    }
    return (nsct);
}


Sint32	STM_GetNumCdbuf(StmHn stm)
{
    Sint32	nsct;
    Sint32	ret;
    
    STMERR_SetFunc(STM_GETNUMCDBUF);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (-1);
    }
    stm = STM_ALIAS(stm);
    nsct = STMSVR_GetNumCdbuf(stm);
    return (nsct);
}


/*************************************************
 * ストリーム状態の取得
 * stm	: ストリームハンドル
 * sfad	: ストリームの開始FAD
 * efad	: ストリームの終了FAD
 * nsct : CDバッファのセクタ数
 * 戻り値: STAT_NOREAD		アクセスされない
 *	   STAT_DOING		処理中
 *	   STAT_WAIT		転送待ち(ゲート閉, 領域フル)
 *	   STAT_COMPLETED	アクセス完了
 *************************************************/
STM_LOCAL Sint32	stm_getStat(StmHn stm, Sint32 sfad, Sint32 efad, Sint32 nsct)
{
    Sint32	fad;
    Sint32	trfad;
    Uint16	hirq;
    Sint32	stat;

    fad = STM_GRP_FAD(STM_GRPHN(stm));
    if ((IS_STM_READEND(stm) == TRUE)&&(nsct == 0)) {
	/* アクセスが完了したストリーム */
	return (STAT_COMPLETED);
    }
    if ((fad <= efad)&&(STM_IS_CDFILE(stm))) {		/* 読み込み中	*/
	if ((IS_STM_READ(stm) == TRUE)||
	    (IS_STM_LOOPNOREAD(stm) == FALSE)) {
	    stat = GFCF_GetStat(NULL, &hirq);
	    stm_setErr(stat);			/* エラーコードの設定	*/
	    if ((hirq & CDC_HIRQ_BFUL) == 0) {
		/* CDバッファに空き領域あり	*/
		return (STAT_DOING);
	    } else if (nsct == 0) {		/* CDバッファのデータは全て他の	*/
		return (STAT_WAIT);		/* ストリームのデータ		*/
	    }
	} else {
	    return (STAT_NOREAD);
	}
    } else if ((IS_STM_READ(stm) == TRUE)&&(nsct == 0)) {
	/* 読み込み完了後,CDバッファにデータがない	*/
	return (STAT_COMPLETED);	/* 転送完了			*/
    }
    if (STM_TRFAD(stm) == STM_FAD_CDEND) {
	trfad = STMMNG_GetLstFad(STM_GRPHN(stm));
    } else {
	trfad = STM_TRFAD(stm);
    }
    /* 転送ゲートが閉じている,転送FAD未満,バッファフル	*/
    if ((!GFS_TRN_ACTIVE(&(GFS_FILE_TRANS(STM_GFS(stm)))))||
	((fad >= sfad)&&(fad < trfad))||
	(STMTRN_IsTrBufFull(stm) == TRUE)) {
	return (STAT_WAIT);
    }
    return (STAT_DOING);			/* 転送中			*/
}



/*************************************************
 * ストリームデータの転送
 * stm	: ストリームハンドル
 * 戻り値 : TRUE  転送した
 *	    FALSE 転送できなかった
 *************************************************/
STM_LOCAL Bool	stmsvr_execTrans(StmHn stm)
{
    GfsHn	gfs;
    Bool	trans;

    gfs = STM_GFS(stm);
    if (((STM_TRFUNC(stm) != STM_TR_NULLFUNC)||
	(GFS_TRN_BUF(&GFS_FILE_TRANS(gfs)) != NULL))&&
	(GFTR_GetLenRoom(&GFS_FILE_TRANS(gfs)) > 0)) {
	GFTR_Exec(gfs, &trans);
    } else {
	trans = FALSE;
    }
    return (trans);
}



/*************************************************
 * バッファサイズ監視関数の実行
 * grp	: ストリームグループハンドル
 *************************************************/
STM_LOCAL void	stm_execCdbufFunc(StmGrpHn grp)
{

    if ((STM_GRP_CHKFUNC(grp) != STM_CDBUF_NULLFUNC)&&
	((STM_GRP_ATR(grp) & STMGRP_ATR_CHKBUF) != 0)) {
	STM_GRP_CHKFUNC(grp)(STM_GRP_CHKOBJ(grp));
	STM_GRP_ATR(grp) &= ~STMGRP_ATR_CHKBUF;
    }
}
    

/*************************************************
 * データの転送
 * grp	: ストリームグループ
 * first : 最初に転送したストリーム
 * cdstat : CDのステータス
 * bflag  : 次のストリームの転送を行なわない場合  TRUE
 * stat	  : 次に転送するストリームのステータス
 * 戻り値 : 全ストリームの転送を完了した場合 TRUE
 *************************************************/
STM_LOCAL Bool	stm_trans_sub(StmGrpHn grp, StmHn first, Uint8 cdstat, 
		                                Bool *bflag, Sint32 *stat)
{
    Sint32	i;
    StmHn	stm, _stm;
    Sint32	nsct;
    Sint32	sfad, efad;
    Sint32	trfad;
    Bool	req;
    Bool	complete = TRUE;
    Bool	set = FALSE;
    Uint8	atr;

    *bflag = FALSE;
    stm = STM_GRP_NEXTTRNS(grp);		/* 転送ストリーム	*/
    if (STM_IS_CDFILE(stm)) {
	STL_GetPlayArea(STM_FLOW(stm), &sfad, &efad);
    } else {
	sfad = efad = STM_FAD_CDTOP;
    }
    atr = STM_ATR(stm);
    if (STM_TRFAD(stm) == STM_FAD_CDEND) {
	trfad = STMMNG_GetLstFad(grp);
    } else {
	trfad = STM_TRFAD(stm);
    }
    if (((atr & STM_ATR_TRSTART) == 0)&&(STM_GRP_FAD(grp) > sfad)&&
	(STM_GRP_FAD(grp) >= trfad)) {
	STM_ATR(stm) |= STM_ATR_TRSTART;
    }
    if (((atr & STM_ATR_TREND) == 0)&&(STM_GRP_FAD(grp) > efad)&&
	(STM_GRP_FAD(grp) >= trfad)) {
	STM_ATR(stm) |= STM_ATR_TREND;
    }
    if (((atr & STM_ATR_TREND) == 0)&&((STM_ATR(stm) & STM_ATR_TREND) != 0)) {
	/* 再生範囲を越えたらCDバッファにある分だけを転送する	*/
	nsct = GFBF_GetNumData(STM_GFS(stm));
	GFTR_SetTransPara(&(GFS_FILE_TRANS(STM_GFS(stm))), nsct);
    } else if (((atr & STM_ATR_TRSTART) == 0)&&((STM_ATR(stm) & STM_ATR_TRSTART) != 0)) {
	/* 再生範囲になったらすべてのデータを転送する		*/
	GFTR_SetTransPara(&(GFS_FILE_TRANS(STM_GFS(stm))), LONG_MAX);
    }
    stmsvr_execTrans(stm);			/* 転送実行		*/
    if (stmsvr_isTrans(stm) == TRUE) {
	/* 転送中の場合		*/
	*bflag = TRUE;
	*stat = stm_getStat(stm, sfad, efad, 1);
	                                        /* 転送中はデータがある	*/
	return (FALSE);
    }
    /* 次の転送ストリームの設定		*/
    _stm = stm;
    for (i = 0; (i < 2)&&(set != TRUE); i++) {
	for (stm = STM_SFADLST(stm); ; stm = STM_SFADLST(stm)) {
	    /* 最後なら先頭に戻す	*/
	    if (stm == NULL) {
		stm = STM_GRP_SFADLST(grp);
	    }
	    if (stm == first) {
		*bflag = TRUE;
	    }
	    nsct = GFBF_GetNumData(STM_GFS(stm));
	    if (STM_IS_CDFILE(stm)) {
		STL_GetPlayArea(STM_FLOW(stm), &sfad, &efad);
	    } else {
		sfad = efad = STM_FAD_CDTOP;
	    }
	    *stat = stm_getStat(stm, sfad, efad, nsct);
	    /* 1回目はアクセス中のストリームを探す	*/
	    /* 2回目は転送待ちのストリームを探す	*/
	    if (((nsct > 0)||(cdstat == GFS_STAT_SEEK)||(cdstat == GFS_STAT_READ))&&
		(((i == 0)&&(*stat == STAT_DOING))||
		 ((i == 1)&&(*stat == STAT_WAIT)))) {
		complete = FALSE;
		set = TRUE;
		break;
	    }
	    if ((*stat == STAT_DOING)||(*stat == STAT_WAIT)) {
		complete = FALSE;
	    }
	    if (stm == _stm) {
		break;
	    }
	}
    }
    STM_GRP_NEXTTRNS(grp) = stm;
    if ((set == FALSE)&&(complete == FALSE)) {
	*stat = STAT_DOING;
    }
    return (complete);
}


/*************************************************
 * データの転送
 * grp	: ストリームグループ
 * cdstat : CDのステータス
 * 戻り値 : TRUE 転送終了  FALSE 転送中
 *************************************************/
STM_LOCAL Bool	stm_trans(StmGrpHn grp, Uint8 cdstat)
{
    StmHn	stm, first;
    Sint32	stat;
    Bool	bflag;
    Bool	ret;

    /* 転送ストリームが未設定の場合		*/
    if (STM_GRP_NEXTTRNS(grp) == NULL) {
	STM_GRP_NEXTTRNS(grp) = STM_GRP_SFADLST(grp);
    }
    first = STM_GRP_NEXTTRNS(grp);

    /* 各ストリームを１度づつ実行を試みる	*/
    /* 転送中または１周まわったらおわり		*/
    do {
	stm = STM_GRP_NEXTTRNS(grp);
	ret = stm_trans_sub(grp, first, cdstat, &bflag, &stat);
    } while(bflag == FALSE);

    /* WAIT <-> DOINGの移行	*/
    if ((stat == STM_EXEC_WAIT)||(STM_GRP_ACSTAT(grp) == STM_EXEC_WAIT)) {
	STM_GRP_ACSTAT(grp) = stat;
    } else if (STM_GRP_FLTLST(grp) == NULL) {
	/* 常駐のみの場合全ての転送が終了したらループしたことにする	*/
	STM_GRP_ACSTAT(grp) = stat;
	if (stat == STM_EXEC_COMPLETED) {
	    STM_GRP_LOOPCNT(grp)++;
	    if (STM_GRP_LOOPCNT(grp) < STM_GRP_LOOPMAX(grp)) {
		STM_GRP_FAD(grp) = STM_FAD_CDTOP;
	    }
	}
    }
    return (ret);
}

    

/*************************************************
 * アクセスポインタの初期化
 * grp	: ストリームグループハンドル
 *************************************************/
STM_LOCAL void	stm_resetAcp(StmGrpHn grp)
{
    StmHn	stm;

    for (stm = STM_GRP_SFADLST(grp); stm != NULL; stm = STM_SFADLST(stm)) {
	GFS_Seek(STM_GFS(stm), 0, GFS_SEEK_SET);
    }
}


/*************************************************
 * サーバの実行(7.3)
 * 戻り値: STM_EXEC_COMPLETED	アクセス終了
 *	   STM_EXEC_PAUSE	一時停止
 *	   STM_EXEC_DOING	アクセス中
 *	   STM_EXEC_WAIT	転送待ち状態
 *************************************************/
Sint32	STM_ExecServer(void)
{
    StmGrpHn	grp;
    Sint32	fad, efad;
    Sint32	ret;
    Bool	read, trans;
    Bool	complete;
    Uint16	hirq;
    Sint32	stat;

    /* 処理中のタスクがある場合	*/
    if (CMD_FUNC(&stm_cmd_area) != CMD_NULLFUNC) {
	complete = CMD_FUNC(&stm_cmd_area)(CMD_OBJ(&stm_cmd_area));
	if (complete == TRUE) {
	    CMD_FUNC(&stm_cmd_area) = CMD_NULLFUNC;
	    return (STM_EXEC_TSKEND);
	}
	return (STM_ERR_WHERE(&MNG_ERR(stm_mng_ptr)));
    }

    /* ストリームアクセス	*/
    STMERR_SetFunc(STM_EXECSERVER);
    grp = MNG_CURGRP(stm_mng_ptr);
    /* 処理中のストリームがある場合	*/
    if ((grp != NULL)&&(STM_GRP_SFADLST(grp) != NULL)) {
	if (MNG_SVREXEC(stm_mng_ptr) == FALSE) {
	    MNG_SVREXEC(stm_mng_ptr) = TRUE;
	    /* ストリームアクセス処理	*/
	    stm_execCdbufFunc(grp);		/* バッファサイズ監視関数の実行	*/
	    stat = GFCF_GetStat(&fad, &hirq);	/* 現在のFAD取得	*/
	    stm_setErr(stat);			/* エラーコードの設定	*/
	    if (IS_SET_ERRCODE(stm_mng_ptr) == TRUE) {
		STMERR_Exec();
	    } else {
		efad = STMMNG_GetLstFad(grp);		/* 最終FADの取得	*/
		read = stm_read(grp, fad, efad, stat);	/* ストリームの読み込み	*/
		trans = stm_trans(grp, stat);		/* ストリームの取り出し	*/
		if ((read == TRUE)&&(trans == TRUE)) {
		    STM_GRP_ACSTAT(grp) = STM_EXEC_COMPLETED;
		    stm_resetAcp(grp);
		}
	    }
	    MNG_SVREXEC(stm_mng_ptr) = FALSE;
	}
	ret = STM_GRP_ACSTAT(grp);
    } else {				/* 処理中のストリームがない場合	*/
	ret = STM_EXEC_PAUSE;
    }
    return (ret);
}



/*************************************************
 * ピックアップの移動(7.4)
 * stm	: ストリームハンドル
 * off	: ストリーム先頭からのオフセット
 *************************************************/
void	STM_MovePickup(StmHn stm, Sint32 off)
{
    StmGrpHn	grp;
    Sint32	sfad, efad;
    Sint32	ret;
    Uint16	hirq;
    Sint32	stat;

    STMERR_SetFunc(STM_MOVEPICKUP);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return ;
    }
    stm = STM_ALIAS(stm);
    grp = STM_GRPHN(stm);
    if (MNG_CURGRP(stm_mng_ptr) == grp) {		/* 処理中グループの場合	*/
	if (IS_PLAYING(grp) == TRUE) {
	    ret = STL_StopIn(STM_GRPHN(stm));	/* 読み込み停止		*/
	    if (ret != STM_ERR_OK) {
		STMERR_SetCode(ret);
	    }
	    STM_GRP_ACSTAT(grp) = STM_EXEC_PAUSE;	/* 次回サーバにて移動	*/
	}
	stat = GFCF_GetStat(&sfad, &hirq);	/* 現在のFADを取得	*/
	stm_setErr(stat);			/* エラーコードの設定	*/
	if (IS_SET_ERRCODE(stm_mng_ptr) == TRUE) {
	    STMERR_Exec();
	    return;
	}
	if (sfad >= 0) {
	    STM_GRP_FAD(grp) = sfad;		/* 現在位置を設定	*/
	}
	/* 再生終了,読み込み終了	*/
	STM_GRP_ATR(grp) &= ~(STMGRP_ATR_PLAY|STMGRP_ATR_READ|STMGRP_ATR_SEEK);
    }
    STMSVR_SetReadEnd(grp);	/* 読み込み済みフラグの設定		*/
    stm_setReadFalse(grp);	/* 読み込み位置以降のフラグをFALSEに設定  */

    /* 移動先FADを取得	*/
    STL_GetPlayArea(STM_FLOW(stm), &sfad, &efad);

    STM_GRP_FAD(grp) = sfad + off;
}


/*************************************************
 * リードエラー発生時のリカバリ(9.3)
 * 戻り値 : STM_ERR_CDRD  リカバリ失敗
 *	    STM_ERR_OK	  リカバリ成功
 *************************************************/
Sint32	STM_Recover(void)
{
    Sint32	ret;

    ret = STL_Recover(MNG_CURGRP(stm_mng_ptr));
    return (ret);
}


/*************************************************
 * アクセス終了のチェック(7.8)
 * stm	: ストリームハンドル
 * 戻り値 : TRUE  ストリームアクセスを終了した
 *	    FALSE ストリームアクセスを終了していない
 *************************************************/
Bool	STM_IsComplete(StmHn stm)
{
    Sint32	nsct;
    Sint32	stat;
    Sint32	sfad, efad;
    Uint16	hirq;

    STMERR_SetFunc(STM_ISCOMPLETE);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (FALSE);
    }
    stm = STM_ALIAS(stm);
    nsct = STMSVR_GetNumCdbuf(stm);		/* バッファセクタ数	*/
    STL_GetPlayArea(STM_FLOW(stm), &sfad, &efad);
    stat = stm_getStat(stm, sfad, efad, nsct);		/* ストリームステータス	*/
    if (stat == STAT_COMPLETED) {		/* アクセス完了の場合	*/
	return (TRUE);
    }
    if ((stat == STAT_WAIT)&&(!GFS_TRN_ACTIVE(&(GFS_FILE_TRANS(STM_GFS(stm)))))) {
	                                      /* ウエイト状態、ゲート閉	*/
	return (TRUE);
    }
    return (FALSE);
}


/*************************************************
 * ストリームデータの転送(7.9)
 * stm	: ストリームハンドル
 * 戻り値 : TRUE  転送した
 *	    FALSE 転送できなかった
 *************************************************/
Bool	STM_ExecTrans(StmHn stm)
{

    GfsHn	gfs;
    Bool	trans;

    STMERR_SetFunc(STM_EXECTRANS);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (FALSE);
    }
    stm = STM_ALIAS(stm);
    trans = stmsvr_execTrans(stm);
    return (trans);
}



/* end of stm_svr.c	*/







