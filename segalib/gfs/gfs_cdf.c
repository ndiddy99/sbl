/*****************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:ファイルシステム
 * Module :CDファイルアクセス
 * File	  :gfs_cdf.c
 * Date   :1994-11-11
 * Version:1.16
 * Auther :M.S.
 *
 *****************************************************************************/

/*****************************************************************************
 *	インクルードファイル
 *****************************************************************************/
#include	<limits.h>

#include	"sega_gfs.h"
#include	"gfs_def.h"
#include	"gfs_cdf.h"
#include	"gfs_cdb.h"
#include	"gfs_cdc.h"

/*****************************************************************************
 *	定数マクロ
 *****************************************************************************/

/*****************************************************************************
 *	列挙定数
 *****************************************************************************/

/*****************************************************************************
 *	処理マクロ
 *****************************************************************************/

/*****************************************************************************
 *	データ型の定義
 *****************************************************************************/

/*****************************************************************************
 *	変数の宣言
 *****************************************************************************/

/*****************************************************************************
 *	関数の宣言
 *****************************************************************************/
/*  セクタサイズの取得	*/
GFS_LOCAL Sint32 gfcf_getSctSize(Sint32 atr);

/* CD状態の監視 */
GFS_LOCAL Bool gfcf_isCdOk(GfsDtsrc *dts, Sint32 stat, Sint32 *err);

/* 再生範囲をflowに設定 */
GFS_LOCAL Bool gfcf_getPlayRange(GfsFlow *flow, Sint32 *nsct, Sint32 *fstat);

/*****************************************************************************
 *	変数の定義
 *****************************************************************************/

/*****************************************************************************
 *	関数の定義
 *****************************************************************************/

/* CDFモジュールの初期化 */
Sint32 GFCF_Init(Bool use_cdbfs)
{
    Sint32 ret;

    MNG_FLAGS(gfs_mng_ptr) &= ~MNG_CDCON_MSK;
    ret = GFCD_Init(&MNG_CDBMNG(gfs_mng_ptr), use_cdbfs);
    if (ret == GFCD_ERR_OK) {
	MNG_FLAGS(gfs_mng_ptr) |= MNG_CDCON_MSK;
    }
    switch (ret) {
    case GFCD_ERR_OK:
	return GFS_ERR_OK;
    case GFCD_ERR_OPEN:
	return GFS_ERR_CDOPEN;
    case GFCD_ERR_NODISC:
	return GFS_ERR_CDNODISC;
    case GFCD_ERR_CDROM:
	return GFS_ERR_CDROM;
    case GFCD_ERR_FATAL:
	return GFS_ERR_FATAL;
    default:
	return GFS_ERR_CDRD;
    }
}


/*****************************************************************************
 *  読み込み管理構造体の初期化
 *［入力］
 *  flow	読み込み管理構造体
 *  dirrec	ディレクトリレコード
 *  fid		ファイル識別子
 *［出力］
 *  
 *［関数値］
 *  正常終了ならTRUE
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
Bool GFCF_Setup(GfsFlow *flow, GfsDirId *dirrec, Sint32 fid)
{
    Sint32	fsize;
    GfsFinfo	*finfo = &GFS_FLW_FINFO(flow);

    /* ファイル情報設定 */
    if (dirrec == NULL) {
	/* CDブロックファイルシステム利用 */
	GFS_FI_FID(finfo) = fid;
	if (GFCD_GetFileInfo(fid, &GFS_FI_FINFO(finfo)) != GFCD_ERR_OK) {
	    return FALSE;
	}
	fsize = GFS_FI_SIZE(finfo);
    } else {
	fsize = GFS_DIR_SIZE(dirrec);
	GFS_FI_FID(finfo) = fid;
	GFS_FI_FAD(finfo) = GFS_DIR_FAD(dirrec);
	GFS_FI_SIZE(finfo) = fsize;
	GFS_FI_UNIT(finfo) = GFS_DIR_UNIT(dirrec);
	GFS_FI_GAP(finfo) = GFS_DIR_GAP(dirrec);
	GFS_FI_FN(finfo) = GFS_DIR_FN(dirrec);
	GFS_FI_ATR(finfo) = GFS_DIR_ATR(dirrec);
    }
    GFS_FI_SCTSZ(finfo) = gfcf_getSctSize(GFS_DIR_ATR(dirrec));
    GFS_FI_NSCT(finfo) = GFS_BYTE_SCT(fsize, GFS_MD1_SCTSIZ);
    if (GFS_FI_SCTSZ(finfo) == 0) {
	/* form1/2混在のとき */
	GFS_FI_LSTRM(finfo) = 0;
    } else {
	GFS_FI_LSTRM(finfo) = GFS_LAST_ROOM(fsize, GFS_MD1_SCTSIZ);
    }
    /* 読み込み元管理情報設定 */
    if (GFCB_Setup(flow) == FALSE) {	/* GFS_FLW_DTSRC(flow)初期化 */
	return FALSE;
    }
    /* その他読み込み管理データ設定 */
    GFS_FLW_GMODE(flow) = GFS_GMODE_ERASE;
    GFS_FLW_STAT(flow) = GFS_FIN_NOACT;
    GFS_FLW_SCT(flow) = GFS_FLW_SCTCNT(flow) = 0;
    GFS_FLW_SCTMAX(flow) = GFS_RPARA_DFL;
    return TRUE;
}


/* 読み込みセクタ数設定 */
void GFCF_SetSct(GfsFlow *flow, Sint32 sct)
{
    Sint32	nsct;

    GFS_FLW_SCT(flow) = sct;
    nsct = GFCB_GetLenData(flow, GFS_RPARA_DFL, NULL);
    if (nsct < 0) {
	nsct = 0;
    }
    GFS_FLW_SCTCNT(flow) = nsct - GFS_CDR_SCTPOS(&GFS_FLW_DTSRC(flow));
}


/*****************************************************************************
 *  ファイル情報の取得
 *［入力］
 *  flow	読み込み管理
 *［出力］
 *  fid		ファイル識別子
 *  sctsiz	セクタサイズ
 *  nsct	セクタ数
 *  lastsiz	最終セクタ有効データ数
 *  fno		ファイル番号
 *  atr		属性
 *［関数値］
 *  
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
void GFCF_GetFileInfo(GfsFlow *flow,
		      Sint32 *fid, Sint32 *fad, Sint32 *sctsiz,
		      Sint32 *nsct, Sint32 *lastsiz, Sint32 *fno, Sint32 *atr)
{
    GfsFinfo	*finfo = &GFS_FLW_FINFO(flow);

    if (fid != NULL) {
	*fid = GFS_FI_FID(finfo);
    }
    if (fad != NULL) {
	*fad = GFS_FI_FAD(finfo);
    }
    if (fno != NULL) {
	*fno = GFS_FI_FN(finfo);
    }
    if (sctsiz != NULL) {
	*sctsiz = GFS_FI_SCTSZ(finfo);
    }
    if (nsct != NULL){
	*nsct = GFS_FI_NSCT(finfo);
    }
    if (atr != NULL) {
	*atr = GFS_FI_ATR(finfo);
    }
    if (lastsiz != NULL) {
	if (IS_FORM1(GFS_FI_ATR(finfo)) && IS_FORM2(GFS_FI_ATR(finfo))) {
	    *lastsiz = 0;
	} else {
	    *lastsiz = GFS_FI_SCTSZ(finfo) - GFS_FI_LSTRM(finfo);
	}
    }
}


/*****************************************************************************
 *  CDバッファへのデータ読み込み
 *［入力］
 *  flow	読み込み管理
 *［出力］
 *  なし
 *［関数値］
 *  読み込み状態(GFS_FIN_～)
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFCF_FlowInBuf(GfsFlow *flow)
{
    Sint32	stat, fad, err, nsct;
    CdcPly	pinfo;
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    stat = GFCF_GetStat(&fad, NULL);
    if (gfcf_isCdOk(dts, stat, &err) == FALSE) {
	return err;
    }
    if (GFCB_IsPuOwner(dts) == FALSE) {
	/* 制御権をまだ取得してないか解放した後 */
	if (gfcf_getPlayRange(flow, &nsct, &err) == FALSE) {
	    return err;
	}
	if (GFCB_GetPickup(dts) == FALSE) {
	    return GFS_FIN_ERROR;
	}
	if (GFCB_SetCon(dts) == FALSE) { /* CDデバイス接続 */
	    return GFS_FIN_ERROR;
	}
	/* CdPlayを発行する */
	CDC_PLY_STYPE(&pinfo) = CDC_PTYPE_FAD;
	CDC_PLY_SFAD(&pinfo) = GFS_CDR_SFAD(dts);
	CDC_PLY_ETYPE(&pinfo) = CDC_PTYPE_FAD;
	CDC_PLY_EFAS(&pinfo) = GFS_CDR_EFAD(dts) - CDC_PLY_SFAD(&pinfo);
	CDC_PLY_PMODE(&pinfo) = CDC_PM_DFL;
	err = GFCD_Play(&pinfo, GFS_CDR_PUID(dts));
	if (err != GFCD_ERR_OK) {
	    return GFS_FIN_ERROR;
	}
	GFS_FLW_SCTCNT(flow) += nsct;	/* 読み込みセクタ数加算 */
	MNG_SFAD(gfs_mng_ptr) = CDC_PLY_SFAD(&pinfo);
	MNG_EFAD(gfs_mng_ptr) = GFS_CDR_EFAD(dts);
	return GFS_FIN_START;		/* データを読み込んだ */
    }
    /* 再生が終了したか調べる */
    if (GFS_CDR_EFAD(dts) <= fad) {
	GFCB_UngetPickup(dts);
	if (GFS_FLW_SCT(flow) == GFS_FLW_SCTCNT(flow) &&
	    MNG_EFAD(gfs_mng_ptr) <= fad) {
	    /* すべての読み込みが完了した */
	    MNG_SFAD(gfs_mng_ptr) = MNG_EFAD(gfs_mng_ptr);
	    return GFS_FIN_END;
	}
	/* 読み込み単位のflowinは完了した */
	return GFS_FIN_PLAY;
    }
    return GFS_FIN_PLAY;		/* CD読み込み中 */
}


/* CDバッファへのデータ読み込み中止 */
void GFCF_StopInBuf(GfsFlow *flow, Bool stop_flag)
{
    CdcPos	pos;
    Sint32	fad;
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    if (stop_flag) {
	MNG_SFAD(gfs_mng_ptr) = MNG_EFAD(gfs_mng_ptr) = 0;
	GFCB_DeleteAllData(dts);
	CDC_POS_PTYPE(&pos) = CDC_PTYPE_NOCHG;
	GFCB_MovePickup(dts, &pos);
    }
    if (GFCB_IsPuOwner(dts)) {
	GFCB_UngetPickup(dts);
	/* 再生範囲を通過したらクリア */
	GFCF_GetStat(&fad, NULL);
	if (MNG_EFAD(gfs_mng_ptr) <= fad) {
	    MNG_SFAD(gfs_mng_ptr) = MNG_EFAD(gfs_mng_ptr);
	}
    }
}


/* リードエラー発生時のリカバリ		*/
Sint32	GFCF_Recover(GfsFlow *flow)
{
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);
    CdcPly	pinfo;
    Sint32	err;

    if (GFCB_IsPuOwner(dts)) {
	/* CdPlayを発行する */
	CDC_PLY_STYPE(&pinfo) = CDC_PTYPE_NOCHG;
	CDC_PLY_ETYPE(&pinfo) = CDC_PTYPE_NOCHG;
	CDC_PLY_PMODE(&pinfo) = CDC_PM_NOCHG;
	err = GFCD_Play(&pinfo, GFS_CDR_PUID(dts));
	if (err != GFCD_ERR_OK) {
	    return GFS_FIN_ERROR;
	}
    } else {
	return GFS_FIN_ERROR;
    }
    return GFS_FIN_PLAY;		/* CD読み込み中 */
}
    

/* アクセスポインタ取得 */
Sint32 GFCF_Tell(GfsFlow *flow)
{
    return GFCB_Tell(&GFS_FLW_DTSRC(flow));
}
 
 
/*****************************************************************************
 *  アクセスポインタ設定
 *［入力］
 *  flow	読み込み管理
 *  off		オフセット
 *［出力］
 *  
 *［関数値］
 *  設定後のアクセスポインタ
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFCF_Seek(GfsFlow *flow, Sint32 off)
{
    return GFCB_Seek(flow, off, GFS_SEEK_SET);
}


/*****************************************************************************
 *  ピックアップ移動
 *［入力］
 *  flow	読み込み管理
 *  off		ファイル先頭からの移動位置
 *［出力］
 *  
 *［関数値］
 *  エラーコード
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFCF_MovePickup(GfsFlow *flow, Sint32 off)
{
    Sint32	err;
    CdcPos	pos;
    GfsFinfo	*finfo = &GFS_FLW_FINFO(flow);

    if (GFCF_GetStat(NULL, NULL) != GFS_STAT_NOACT) {
	return GFS_ERR_PUINUSE;
    }
    /* ピックアップの制御権を取得することによってflow in中でないことを確認 */
    if (GFCB_GetPickup(&GFS_FLW_DTSRC(flow)) == FALSE) {
	return GFS_ERR_PUINUSE;
    }
    CDC_POS_PTYPE(&pos) = CDC_PTYPE_FAD;
    CDC_POS_FAD(&pos) = GFCF_cnvIntlvOfs(finfo, off) + GFS_FI_FAD(finfo);
    switch (GFCB_MovePickup(&GFS_FLW_DTSRC(flow), &pos)) {
    case GFCD_ERR_OK:
	err = GFS_ERR_OK;
	break;
    case GFCD_ERR_NOTOWNER:
	err = GFS_ERR_PUINUSE;
	break;
    case GFCD_ERR_OPEN:
	err = GFS_ERR_CDOPEN;
	break;
    default:
	err = GFS_ERR_CDRD;
	break;
    }
    GFCB_UngetPickup(&GFS_FLW_DTSRC(flow));
    return err;
}


/* インターリーブのギャップを含めたオフセットの計算 */
Sint32 GFCF_cnvIntlvOfs(GfsFinfo *finfo, Sint32 ofs)
{
    if (GFS_FI_UNIT(finfo) == 0) {
	return ofs;
    }
    return (ofs / GFS_FI_UNIT(finfo) *
	    (GFS_FI_UNIT(finfo) + GFS_FI_GAP(finfo))) +
		ofs % GFS_FI_UNIT(finfo);
}


/*****************************************************************************
 *  ステータス取得
 *［入力］
 *  なし
 *［出力］
 *  fad		現在のフレームアドレス
 *［関数値］
 *   GFS_STAT_FREAD,GFS_STAT_SEEK,GFS_STAT_NOACTのどれか
 *［機能］
 *   フレームアドレスが無効の時は*fad == -1
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFCF_GetStat(Sint32 *fad, Uint16 *hirq)
{
    CdcStat	stat;
    Uint16	tmp;

    tmp = GFCD_GetStat(&stat);
    if (hirq != NULL) {
	*hirq = tmp;
    }
    if (fad != NULL) {
	*fad = CDC_STAT_FAD(&stat);
    }
    if (tmp & CDC_HIRQ_DCHG) {
	return GFS_STAT_OPEN;
    }
    switch (CDC_STAT_STATUS(&stat) & 0xf) {
    case CDC_ST_BUSY:			/* 状態遷移中 */
	return GFS_STAT_SEEK;
    case CDC_ST_PAUSE:			/* ポーズ中 */
    case CDC_ST_STANDBY:		/* スタンバイ（ドライブ停止状態） */
	return GFS_STAT_NOACT;
    case CDC_ST_PLAY:			/* ＣＤ再生中 */
    case CDC_ST_RETRY:			/* リードリトライ処理中 */
	return GFS_STAT_READ;
    case CDC_ST_SEEK:			/* シーク中 */
	return GFS_STAT_SEEK;
    case CDC_ST_SCAN:			/* スキャン再生中 */
	return GFS_STAT_READ;
    case CDC_ST_OPEN:			/* トレイが開いている */
	return GFS_STAT_OPEN;
    case CDC_ST_NODISC:			/* ディスクがない */
	return GFS_STAT_NODISC;
    case CDC_ST_FATAL:			/* 致命的エラーが発生した */
	return GFS_STAT_FATAL;
    case CDC_ST_ERROR:			/* リードデータエラーが発生した */
    default:
	return GFS_STAT_ERROR;
    }
}

/*---------------------------------------------------------------------------
 * 非公開関数
 *---------------------------------------------------------------------------*/

/* セクタサイズの取得 */
GFS_LOCAL Sint32 gfcf_getSctSize(Sint32 atr)
{
    Sint32      sctsz;

    if (IS_CDDA(atr)) {
	sctsz = GFS_CDDA_SCTSIZ;
    } else if (IS_FORM2(atr)) {
	if (IS_FORM1(atr)) {
	    sctsz = 0;
	} else {
	    sctsz = GFS_FM2_SCTSIZ;
	}
    } else if (IS_FORM1(atr)) {
        sctsz = GFS_FM1_SCTSIZ;
    } else {
        sctsz = GFS_MD1_SCTSIZ;
    }
    return sctsz;
}


/* CD状態の監視 */
GFS_LOCAL Bool gfcf_isCdOk(GfsDtsrc *dts, Sint32 stat,  Sint32 *err)
{
    Sint32	fad;

    switch (stat) {
    case GFS_STAT_SEEK:
    case GFS_STAT_READ:
	break;				/* 正常 */
    case GFS_STAT_NOACT:
	if (GFCD_GetBufSiz() == 0) {
	    /* 空きセクタは０だが、BFULフラグが１とは限らない */
	    *err = GFS_FIN_PAUSE;	/* バッファフルでポーズ */
	    return FALSE;
	}
	break;
    case GFS_STAT_OPEN:
	*err = GFS_FIN_OPEN;
	return FALSE;
    case GFS_STAT_NODISC:
	*err = GFS_FIN_NODISC;
	return FALSE;
    case GFS_STAT_FATAL:
	*err = GFS_FIN_FATAL;
	return FALSE;
    default:
	*err = GFS_FIN_ERROR;
	return FALSE;
    }
    *err = GFS_FIN_START;
    return TRUE;
}


/* 再生範囲をflowに設定 */
/* playを発行する必要があれば TRUEを返す */
GFS_LOCAL Bool gfcf_getPlayRange(GfsFlow *flow, Sint32 *nsct, Sint32 *fstat)
{
    Sint32	ap_fad, bsct;
    GfsFinfo	*finfo = &GFS_FLW_FINFO(flow);
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    *nsct = GFS_FLW_SCT(flow) - GFS_FLW_SCTCNT(flow);
    /* 残りflowinセクタ数から今回のflowinセクタ数を得る */
    *nsct = MIN(*nsct, GFS_FLW_SCTMAX(flow));
    ap_fad = GFS_FI_FAD(finfo) + GFCF_cnvIntlvOfs(finfo, GFCB_Tell(dts));
    if (GFS_FLW_GMODE(flow) == GFS_GMODE_RESIDENT) {
	/* バッファに必要なデータが入っているか調べる */
	bsct = GFCB_GetLenData(flow, LONG_MAX, NULL);
	if (bsct < 0) {
	    bsct = 0;
	}
	/* まだ転送されていないセクタデータの分を除く */
	if (bsct > 0 && ap_fad < MNG_SFAD(gfs_mng_ptr)) {
	    bsct -= MNG_SFAD(gfs_mng_ptr) - ap_fad;
	}
    } else {
	bsct = 0;
    }
    if (bsct >= *nsct) {
	/* バッファ内のセクタ数が必要なセクタ数を上回っている */
	GFS_FLW_SCTCNT(flow) += *nsct; /* 読み込みセクタ数加算 */
	if (GFS_FLW_SCT(flow) == GFS_FLW_SCTCNT(flow)) {
	    /* 全セクタのflowinが終了した */
	    *fstat = GFS_FIN_END;
	    MNG_SFAD(gfs_mng_ptr) = MNG_EFAD(gfs_mng_ptr);
	} else {
	    /* 読み込み単位のflowinが終了した */
	    *fstat = GFS_FIN_PLAY;
	    MNG_SFAD(gfs_mng_ptr) = ap_fad;
	}
	return FALSE;
    }
    *nsct -= bsct;
    GFS_CDR_SFAD(dts) = ap_fad;
    GFS_CDR_EFAD(dts) = GFS_FI_FAD(finfo) +
	GFCF_cnvIntlvOfs(finfo, *nsct + GFCB_Tell(dts));
    /* flowinしようとする範囲が再生範囲に完全に含まれるか */
    if (MNG_SFAD(gfs_mng_ptr) <= GFS_CDR_SFAD(dts) &&
	GFS_CDR_SFAD(dts) < MNG_EFAD(gfs_mng_ptr) &&
	MNG_SFAD(gfs_mng_ptr) < GFS_CDR_EFAD(dts) &&
	GFS_CDR_EFAD(dts) <= MNG_EFAD(gfs_mng_ptr)) {
	/* ならばCdPlayを発行する必要はない */
	MNG_SFAD(gfs_mng_ptr) = GFS_CDR_SFAD(dts);
	*fstat = GFS_FIN_START;
	return FALSE;
    }
    return TRUE;
}

/* end of file */
