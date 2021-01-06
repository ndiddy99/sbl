/*****************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:ファイルシステム
 * Module :SIMMファイルアクセス
 * File	  :gfs_mmf.c
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
#include "gfs_mmf.h"
#include "gfs_mmb.h"
#include "gfs_mmc.h"

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

/*****************************************************************************
 *	関数の定義
 *****************************************************************************/
/* MMFの初期化 */
Bool GFMF_Init(void)
{
    GFMB_Init();
    return (GFMC_GetBase() == NULL)? FALSE: TRUE;
}


/*****************************************************************************
 *  flow-in管理構造体の初期化
 *［入力］
 *  flow	flow-in管理構造体
 *  dirrec	ディレクトリレコード
 *  fid		ファイル識別子
 *［出力］
 *  
 *［関数値］
 *  
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
void GFMF_Setup(GfsFlow *flow, GfsDirId *dirrec, Sint32 fid)
{
    Sint32      fsize;
    GfsFinfo	*finfo = &GFS_FLW_FINFO(flow);
    
    /* ファイル情報設定 */
    fsize = GFS_DIR_SIZE(dirrec);
    GFS_FI_FID(finfo) = fid;
    GFS_FI_FAD(finfo) = 0;
    GFS_FI_SIZE(finfo) = fsize;
    GFS_FI_UNIT(finfo) = GFS_DIR_UNIT(dirrec);
    GFS_FI_GAP(finfo) = GFS_DIR_GAP(dirrec);
    GFS_FI_FN(finfo) = 0;
    GFS_FI_ATR(finfo) = GFS_DIR_ATR(dirrec);
    GFS_FI_SCTSZ(finfo) = MMF_SCTSIZE;
    GFS_FI_NSCT(finfo) = GFS_BYTE_SCT(fsize, MMF_SCTSIZE);
    GFS_FI_LSTRM(finfo) = GFS_LAST_ROOM(fsize, MMF_SCTSIZE);
    /* 読み込み元管理情報設定 */
    GFMB_Setup(flow, GFS_DIR_FAD(dirrec));
    /* その他読み込み管理データ設定 */
    GFS_FLW_GMODE(flow) = GFS_GMODE_ERASE;
    GFS_FLW_SCT(flow) = GFS_FLW_SCTCNT(flow) = 0;
    GFS_FLW_SCTMAX(flow) = GFS_RPARA_DFL;
}


/*****************************************************************************
 *  メモリバッファへのデータ読み込み
 *［入力］
 *  flow	flow-in管理構造体
 *［出力］
 *  
 *［関数値］
 *  読み込み状態(GFS_FIN_START)
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFMF_FlowInBuf(GfsFlow *flow)
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
    nsct -= GFS_MEMR_SCTNUM(dts) - GFS_MEMR_SCTPOS(dts);
    if (nsct > 0) {
	GFS_MEMR_SCTNUM(dts) += nsct;
    }
    /* 読み込み終了の場合 */
    if (GFS_FLW_SCTCNT(flow) >= GFS_FLW_SCT(flow)) {
	return GFS_FIN_END;		/* 読み込みを完了した */
    } else {
	return GFS_FIN_START;		/* データを読み込んだ */
    }
}


void GFMF_StopInBuf(GfsFlow *flow, Bool stop_flag)
{
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    if (stop_flag) {
	GFS_MEMR_SCTPOS(dts) = GFS_MEMR_SCTNUM(dts) = 0;
    }
}


/* アクセスポインタ設定 */
Sint32 GFMF_Seek(GfsFlow *flow, Sint32 pos)
{
    Sint32      nsct, ofs;
    GfsFinfo	*finfo = &GFS_FLW_FINFO(flow);
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);
 
    nsct = GFS_FI_NSCT(finfo);		/* ファイルサイズ */
    /* ファイルサイズを越えてシークしないようにする */
    if (pos > nsct) {
        pos = nsct;
    }
    if (pos < 0) {
        pos = 0;
    }
    if (GFS_MEMR_FILEPOS(dts) <= pos &&
	pos <= GFS_MEMR_FILEPOS(dts) + GFS_MEMR_SCTNUM(dts)) {
	/* CDバッファ内へのシーク */
	ofs = pos - GFS_MEMR_FILEPOS(dts);
	if (GFS_FLW_GMODE(flow) == GFS_GMODE_RESIDENT) {
	    GFS_MEMR_SCTPOS(dts) = ofs;
	} else {
	    GFS_MEMR_FILEPOS(dts) = pos;
	    GFS_MEMR_SCTNUM(dts) -= ofs;
	}
    } else {
	/* CDバッファ外へのシーク */
	GFS_MEMR_FILEPOS(dts) = pos;
	GFS_MEMR_SCTPOS(dts) = 0;
	GFS_MEMR_SCTNUM(dts) = 0;
    }
    return pos;
}


/* アクセスポインタ取得 */
Sint32 GFMF_Tell(GfsFlow *flow)
{
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    return GFS_MEMR_FILEPOS(dts) + GFS_MEMR_SCTPOS(dts);
}


/* end of file */
