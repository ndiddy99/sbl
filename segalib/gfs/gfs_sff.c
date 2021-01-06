/*****************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:ファイルシステム
 * Module :SCSIファイル
 * File	  :gfs_sff.c
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
#include "gfs_scsi.h"
#include "gfs_sf.h"

/*****************************************************************************
 *	定数マクロ
 *****************************************************************************/
#define GFSF_RPARA_DFL	1		/* SCSIでは1セクタずつflow inする */

/* ディップスイッチの設定を得るためのアドレス */
#define BOOT_WORK_DIPSW	(*(Sint32 *)0x6000248)

/* SCSI使用ビット:ディップスイッチのbit4 */
#define DIP_USE_SCSI	0x10

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

/*****************************************************************************
 *	関数の定義
 *****************************************************************************/

/* SCSIファイル管理の初期化 */
void GFSF_Init(void)
{
    MNG_FLAGS(gfs_mng_ptr) &= ~MNG_SCSICON_MSK;
    if (BOOT_WORK_DIPSW & DIP_USE_SCSI) {
	MNG_FLAGS(gfs_mng_ptr) |= MNG_SCSICON_MSK;
    }
    GFSB_Init();
}


/*****************************************************************************
 *  SCSIファイルハンドルの初期化
 *［入力］
 *  flow	flow-in管理データ
 *  dirrec	ディレクトリレコード領域
 *  fid		ファイル識別子
 *［出力］
 *  
 *［関数値］
 *  SCSIファイルハンドル
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
Bool GFSF_Setup(GfsFlow *flow, GfsDirId *dirrec, Sint32 fid)
{
    Sint32	fsize;
    GfsFinfo	*finfo = &GFS_FLW_FINFO(flow);

    if (GFSD_Open(GFS_DIR_FAD(dirrec)) != GFSD_ERR_OK) {
	return FALSE;
    }
    fsize = GFS_DIR_SIZE(dirrec);
    /* ファイル情報設定 */
    GFS_FI_FID(finfo) = fid;
    GFS_FI_FAD(finfo) = 0;
    GFS_FI_SIZE(finfo) = fsize;
    GFS_FI_UNIT(finfo) = GFS_DIR_UNIT(dirrec);
    GFS_FI_GAP(finfo) = GFS_DIR_GAP(dirrec);
    GFS_FI_FN(finfo) = 0;
    GFS_FI_ATR(finfo) = GFS_DIR_ATR(dirrec);
    GFS_FI_SCTSZ(finfo) = SFM_SCTSIZE;
    GFS_FI_NSCT(finfo) = GFS_BYTE_SCT(fsize, SFM_SCTSIZE);
    GFS_FI_LSTRM(finfo) = GFS_LAST_ROOM(fsize, SFM_SCTSIZE);
    /* SCSIファイル管理情報 */
    GFSB_Setup(flow, GFS_DIR_FAD(dirrec));
    /* flow-in管理データ */
    GFS_FLW_GMODE(flow) = GFS_GMODE_ERASE;
    GFS_FLW_SCT(flow) = GFS_FLW_SCTCNT(flow) = 0;
    GFS_FLW_SCTMAX(flow) = GFS_RPARA_DFL;
    return TRUE;
}



/*****************************************************************************
 *  SCSIファイルからのデータ読み込み
 *［入力］
 *  flow	flow-in管理構造体
 *［出力］
 *  
 *［関数値］
 *  	読み込み状態(常にGFS_FIN_END)
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFSF_FlowInBuf(GfsFlow *flow)
{
    Sint32	remain;
    Sint32	nsct;
    GfsFinfo	*finfo = &GFS_FLW_FINFO(flow);
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);
    
    remain = GFS_FLW_SCT(flow) - GFS_FLW_SCTCNT(flow);
    if (remain <= 0) {
	return GFS_FIN_END;
    }
    nsct = MIN(remain, GFS_FLW_SCTMAX(flow));
    GFS_FLW_SCTCNT(flow) += nsct;
    /* バッファ内に入っているセクタを考慮 */
    nsct -= GFS_SCR_SCTNUM(dts) - GFS_SCR_SCTPOS(dts);
    if (nsct > 0) {
	GFS_SCR_SCTNUM(dts) += nsct;
    }
    /* 読み込み終了の場合 */
    if (GFS_FLW_SCTCNT(flow) >= GFS_FLW_SCT(flow)) {
	return GFS_FIN_END;		/* 読み込みを完了した */
    } else {
	return GFS_FIN_START;		/* データを読み込んだ */
    }
}


void GFSF_StopInBuf(GfsFlow *flow, Bool stop_flag)
{
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    if (stop_flag) {
	GFS_SCR_SCTPOS(dts) = GFS_SCR_SCTNUM(dts) = 0;
    }
}


/* アクセスポインタ設定 */
Sint32 GFSF_Seek(GfsFlow *flow, Sint32 pos)
{
    Sint32      nsct, ofs;
    GfsFinfo	*finfo = &GFS_FLW_FINFO(flow);
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);
 
    nsct = GFS_FI_NSCT(finfo);		/* ファイルサイズ   */
    /* ファイルサイズを越えてシークしないようにする */
    if (pos > nsct) {
        pos = nsct;
    }
    if (pos < 0) {
        pos = 0;
    }
    if (GFS_SCR_FILEPOS(dts) <= pos &&
	pos <= GFS_SCR_FILEPOS(dts) + GFS_SCR_SCTNUM(dts)) {
	/* CDバッファ内へのシーク */
	ofs = pos - GFS_SCR_FILEPOS(dts);
	if (GFS_FLW_GMODE(flow) == GFS_GMODE_RESIDENT) {
	    GFS_SCR_SCTPOS(dts) = ofs;
	} else {
	    GFS_SCR_FILEPOS(dts) = pos;
	    GFS_SCR_SCTNUM(dts) -= ofs;
	}
    } else {
	/* CDバッファ外へのシーク */
	GFS_SCR_FILEPOS(dts) = pos;
	GFS_SCR_SCTPOS(dts) = 0;
	GFS_SCR_SCTNUM(dts) = 0;
    }
    return pos;
}


/* アクセスポインタ取得 */
Sint32 GFSF_Tell(GfsFlow *flow)
{
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);
 
    return GFS_SCR_FILEPOS(dts) + GFS_SCR_SCTPOS(dts);
}


/* SCSIファイルのクローズ処理 */
void GFSF_Close(GfsDtsrc *dts)
{
    GFSD_Close(GFS_SCR_FID(dts));
}


/* end of file */
