/*****************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:ファイルシステム
 * Module :SIMMファイルバッファ管理
 * File	  :gfs_mmb.c
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
#include "gfs_mmc.h"
#include "gfs_mmb.h"
#include "gfs_trn.h"

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

/*****************************************************************************
 *	変数の定義
 *****************************************************************************/

/* データパック管理     */
GfsDataPack gfmb_dtpk;

/*****************************************************************************
 *	関数の定義
 *****************************************************************************/
/* メモリファイル用データパックの初期化 */
void GFMB_Init(void)
{
    GfdpHn	dtk = &gfmb_dtpk;
    
    GFDP_DATA(dtk) = NULL;
    GFDP_ADLT(dtk) = 0;
    GFDP_LEN(dtk) = 0;
    GFDP_NSCT(dtk) = 0;
    GFDP_USE(dtk) = FALSE;
}


/* メモリバッファの設定 */
void GFMB_Setup(GfsFlow *flow, Sint32 fid)
{
    void	*area;
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);
    
    if (fid >= 0) {
	GFMC_GetFileInfo(fid, &area, NULL, NULL);
    } else {
	area = NULL;
    }
    GFS_DTS_FTYPE(dts) = MEM_FILE;
    GFS_MEMR_DATA(dts) = area;
    GFS_MEMR_FILEPOS(dts) = GFS_MEMR_SCTPOS(dts) = GFS_MEMR_SCTNUM(dts) = 0;
}


/*****************************************************************************
 *  メモリバッファの有効セクタ数を求める
 *［入力］
 *  flow	flow-in管理構造体
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
Sint32 GFMB_GetLenData(GfsFlow *flow, Sint32 usct, Sint32 *len)
{
    Sint32	ap;
    Sint32      nsct;
    Sint32	lastsize;
    GfsFinfo	*finfo = &GFS_FLW_FINFO(flow);
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    if (usct == GFTB_BUF_END) {
	usct = LONG_MAX;
    }
    nsct = GFS_MEMR_SCTNUM(dts) - GFS_MEMR_SCTPOS(dts);
    nsct = MIN(nsct, usct);
    if (len != NULL) {
	*len = nsct * MMF_SCTSIZE;
	ap = GFS_MEMR_FILEPOS(dts) + GFS_MEMR_SCTPOS(dts);
	/* 最終セクタを含む場合 */
	if (ap > GFS_FI_NSCT(finfo) &&
	    lastsize != 0 && GFS_MEMR_SCTPOS(dts) > 0) {
	    lastsize = GFS_FI_SIZE(finfo) % MMF_SCTSIZE;
	    if ((*len != 0) && (lastsize != 0)) {
		*len -= MMF_SCTSIZE - lastsize;
	    }
	}
    }
    return nsct;
}


/* 転送元データパック取得 */
GfdpHn GFMB_GetSrcPack(GfsFlow *flow, Sint32 nsct)
{
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);
    GfdpHn	dtk = &gfmb_dtpk;
    
    /* データパックが使用中の場合   */
    if (GFDP_USE(dtk)) {
        return NULL;
    }
    /* データ領域 */
    GFDP_DATA(dtk) = (Uint8 *)GFS_MEMR_DATA(dts) +
	(GFS_MEMR_FILEPOS(dts) +GFS_MEMR_SCTPOS(dts)) * MMF_SCTSIZE;
    GFDP_ADLT(dtk) = sizeof(Uint8);	/* アドレス加算分 */
    GFDP_LEN(dtk) = nsct * MMF_SCTSIZE;	/* バイト数 */
    GFDP_NSCT(dtk) = nsct;		/* セクタ数 */
    GFDP_USE(dtk) = TRUE;		/* 使用中フラグ設定 */
    return dtk;
}


/* 転送元データパック返還 */
void GFMB_RtnPk(GfsFlow *flow, GfdpHn dpk, Sint32 nsct)
{
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);
    GfdpHn	dtk = &gfmb_dtpk;

    /* 転送が終了している場合   */
    if (nsct == GFTR_ALLSCT) {
	nsct = GFDP_LEN(dpk) / MMF_SCTSIZE;
    }
    GFMF_Seek(flow, GFMF_Tell(flow) + nsct);
    GFDP_USE(dtk) = FALSE;		/* フラグ解除 */
}


/* end of file */
