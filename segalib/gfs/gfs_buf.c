/*****************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:ファイルシステム
 * Module :バッファ操作
 * File	  :gfs_buf.c
 * Date   :1994-11-11
 * Version:1.16
 * Auther :M.S.
 *
 *****************************************************************************/

/*****************************************************************************
 *	インクルードファイル
 *****************************************************************************/
#include "sega_gfs.h"
#include "gfs_def.h"
#include "gfs_buf.h"
#include "gfs_cdb.h"
#if defined(DEBUG_LIB)
#include "gfs_sf.h"
#include "gfs_mmb.h"
#endif

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
/* バッファ操作関数とのインタフェース */
typedef struct {
    Sint32 (*getltran)(GfsFlow *flow, Sint32 usct, Sint32 *nsct);
    GfdpHn (*getsrcpk)(GfsFlow *flow, Sint32 nsct);
    Bool (*getsctdt)(GfdpHn dpk, GfsDtsrc *dts);
    void (*rtnpk)(GfsFlow *flow, GfdpHn dtpk, Sint32 nsct);
} GfsBufIf;


/*****************************************************************************
 *	変数の宣言
 *****************************************************************************/

/*****************************************************************************
 *	関数の宣言
 *****************************************************************************/
/* SCSI/SIMMファイル用セクタデータ取得 */
GFS_LOCAL Bool gfbf_getSctDt(GfdpHn dpk, GfsDtsrc *dts);

/*****************************************************************************
 *	変数の定義
 *****************************************************************************/

/* ファイル種別ごとのバッファ操作関数テーブル */
static const GfsBufIf bufif_func[FILE_TYPE] = {
    {GFCB_GetLenData, GFCB_GetSrcPack, GFCB_GetSctData, GFCB_RtnPk},
#if defined(DEBUG_LIB)
    {GFMB_GetLenData, GFMB_GetSrcPack, gfbf_getSctDt, GFMB_RtnPk},
    {GFSB_GetLenData, GFSB_GetSrcPack, gfbf_getSctDt, GFSB_RtnPk}
#endif
};

/*****************************************************************************
 *	関数の定義
 *****************************************************************************/

/* 転送可能データ長の取得 */
Sint32 GFBF_GetTranData(GfsHn gfs, Sint32 rsct, Sint32 *len)
{
    GfsFlow	*flow = &GFS_FILE_FLOW(gfs);
    Sint32	ftype = GFS_DTS_FTYPE(&GFS_FLW_DTSRC(flow));

    return bufif_func[ftype].getltran(flow, rsct, len);
}


/* 転送可能セクタ数の取得 */
Sint32 GFBF_GetNumData(GfsHn gfs)
{
    GfsFlow	*flow = &GFS_FILE_FLOW(gfs);
    Sint32	ftype = GFS_DTS_FTYPE(&GFS_FLW_DTSRC(flow));

    return bufif_func[ftype].getltran(flow, GFTB_BUF_END, NULL);
}


/* 転送元のデータパックの取得 */
GfdpHn GFBF_GetSrcPk(GfsHn gfs, Sint32 nsct)
{
    GfsFlow	*flow = &GFS_FILE_FLOW(gfs);
    Sint32	ftype = GFS_DTS_FTYPE(&GFS_FLW_DTSRC(flow));

    return bufif_func[ftype].getsrcpk(flow, nsct);
}


/* セクタデータ取得 */
Bool GFBF_GetSctData(GfdpHn dpk, GfsFlow *flow)
{
    Sint32	ftype = GFS_DTS_FTYPE(&GFS_FLW_DTSRC(flow));

    return bufif_func[ftype].getsctdt(dpk, &GFS_FLW_DTSRC(flow));
}


/* データパックの返還 */
void GFBF_RtnPk(GfsHn gfs, GfdpHn gpdp, Sint32 nsct)
{
    GfsFlow	*flow = &GFS_FILE_FLOW(gfs);
    Sint32	ftype = GFS_DTS_FTYPE(&GFS_FLW_DTSRC(flow));

    bufif_func[ftype].rtnpk(flow, gpdp, nsct);
}


/* SCSI/SIMMファイル用セクタデータ取得 */
GFS_LOCAL Bool gfbf_getSctDt(GfdpHn dpk, GfsDtsrc *dts)
{
    return TRUE;
}

/* end of file */
