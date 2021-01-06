/*****************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:ファイルシステム
 * Module :SCSIバッファ管理
 * File	  :gfs_sfb.c
 * Date   :1994-11-11
 * Version:1.16
 * Auther :M.S.
 *
 *****************************************************************************/

/*****************************************************************************
 *	インクルードファイル
 *****************************************************************************/
#include <limits.h>
#include "sega_gfs.h"
#include "gfs_def.h"
#include "gfs_sf.h"
#include "gfs_scsi.h"
#include "gfs_trn.h"

/*****************************************************************************
 *	定数マクロ
 *****************************************************************************/
#define GFSB_TPARA_DFL	1		/* SCSIでは1セクタずつ転送する */

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

/* アクセス位置から指定セクタ数の有効データバイト数を取得 */
GFS_LOCAL Sint32 gfsb_getActSiz(GfsFlow *flow, Sint32 usct, Sint32 *len);

/*****************************************************************************
 *	変数の定義
 *****************************************************************************/

/* データパック管理     */
GfsDataPack gfsb_dtpk;


/*****************************************************************************
 *	関数の定義
 *****************************************************************************/
/* SCSI用データパックの初期化 */
void GFSB_Init(void)
{
    GfdpHn      dpk = &gfsb_dtpk;
    
    GFDP_DATA(dpk) = NULL;
    GFDP_ADLT(dpk) = 0;
    GFDP_LEN(dpk) = 0;
    GFDP_NSCT(dpk) = 0;
    GFDP_USE(dpk) = FALSE;
}


/* SCSIファイルバッファの設定 */
void GFSB_Setup(GfsFlow *flow, Sint32 fid)
{
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    GFS_DTS_FTYPE(dts) = SCSI_FILE;
    GFS_SCR_FID(dts) = fid;
    GFS_SCR_FILEPOS(dts) = GFS_SCR_SCTPOS(dts) = GFS_SCR_SCTNUM(dts) = 0;
}


/*****************************************************************************
 *  SCSIファイルの有効セクタ数を求める
 *［入力］
 *  flow
 *  usct	転送要求セクタ数
 *  nsct	転送可能バイト数
 *［出力］
 *  
 *［関数値］
 *  転送可能セクタ数
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFSB_GetLenData(GfsFlow *flow, Sint32 usct, Sint32 *len)
{
    Sint32      nsct, lastsize, ap;
    GfsFinfo	*finfo = &GFS_FLW_FINFO(flow);
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    if (usct == GFTB_BUF_END) {
	usct = LONG_MAX;
    }
    nsct = GFS_SCR_SCTNUM(dts) - GFS_SCR_SCTPOS(dts);
    nsct = MIN(nsct, usct);
    if (len != NULL) {
	*len = nsct * SFM_SCTSIZE;
	ap = GFS_SCR_FILEPOS(dts) + GFS_SCR_SCTPOS(dts); 
	/* 最終セクタを含む場合 */
	if (ap > GFS_FI_NSCT(finfo)) {
	    lastsize = GFS_FI_SIZE(finfo) % SFM_SCTSIZE;
	    if ((*len != 0) && (lastsize != 0)) {
		*len -= SFM_SCTSIZE - lastsize;
	    }
	}
    }
    return nsct;
}


/* 転送元データパック取得 */
GfdpHn GFSB_GetSrcPack(GfsFlow *flow, Sint32 usct)
{
    Sint32      len, nsct;
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);
    GfdpHn      dpk = &gfsb_dtpk;
    
    /* 使用中の場合 */
    if (GFDP_USE(dpk)) {
        return NULL;
    }
    GFDP_DATA(dpk) = GFSD_GetBuf();
    GFDP_ADLT(dpk) = sizeof(Uint8);	/* アドレス加算分 */
    nsct = gfsb_getActSiz(flow, usct, &len);
    /* SCSIバッファに１セクタ読み込む */
    GFSD_Read(GFS_SCR_FID(dts), GFS_SCR_FILEPOS(dts) + GFS_SCR_SCTPOS(dts));
    GFDP_LEN(dpk) = len;
    GFDP_NSCT(dpk) = nsct;		/* 転送セクタ数 */
    GFDP_USE(dpk) = TRUE;		/* データパック使用中 */
    return dpk;
}


/* データパック返還 */
void GFSB_RtnPk(GfsFlow *flow, GfdpHn dtpk, Sint32 nsct)
{
    Sint32      seekpos;
    GfdpHn      dpk = &gfsb_dtpk;
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    /* 使用中のデータパックを変換することを確認 */
    if (GFDP_USE(dpk)) {
        /* 転送が終了している場合,アクセスポインタを進める  */
	if (nsct == GFTR_ALLSCT) {
	    nsct = GFDP_NSCT(dpk);
	}
	GFSF_Seek(flow, GFSF_Tell(flow) + nsct);
	/* データパック返還 */
	GFDP_USE(dpk) = FALSE;		/* データパック使用中解除 */
    }
}


/*---------------------------------------------------------------------------
 * 非公開関数
 *---------------------------------------------------------------------------*/


/* アクセス位置から指定セクタ数の有効データバイト数を取得 */
GFS_LOCAL Sint32 gfsb_getActSiz(GfsFlow *flow, Sint32 usct, Sint32 *len)
{
    Sint32	nsct;
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    nsct = GFS_SCR_SCTNUM(dts) - GFS_SCR_SCTPOS(dts);
    nsct = MIN(nsct, usct);
    nsct = MIN(nsct, GFSB_TPARA_DFL);
    *len = nsct * SFM_SCTSIZE;
    return nsct;
}

/* end of file */
