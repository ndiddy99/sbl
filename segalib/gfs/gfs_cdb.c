/*****************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:ファイルシステム
 * Module :CDバッファ操作
 * File	  :gfs_cdb.c
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
#include "gfs_cdf.h"
#include "gfs_cdb.h"
#include "gfs_cdc.h"
#include "gfs_trn.h"


/*****************************************************************************
 *	定数マクロ
 *****************************************************************************/
#define BUF_TOP         0		/* ＣＤバッファ先頭 */

/* リード絞りのフィルタモード	*/
#define FMODE_SELECT_FN		0x01	/* ファイル番号選択可能 */
#define FMODE_SELECT_CN		0x02	/* チャネル番号選択可能 */
#define FMODE_SELECT_SM		0x04	/* サブモード選択可能 */
#define FMODE_SELECT_CI		0x08	/* coding information 選択可能 */
#define FMODE_REV_SUBH		0x10	/* サブヘッダ条件を反転する */
#define FMODE_SELECT_FRNG	0x40	/* フレームアドレス範囲有効 */
#define FMODE_INIT_FMODE	0x80	/* 絞り条件を初期化する */

#define	CDB_ADDR_DLT	0

#define GFCB_NOSEL	0		/* モードを指定しない   */

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
 *	関数の宣言
 *****************************************************************************/

#if	0
/* CDバッファの空きセクタ数取得 */
GFS_LOCAL Sint32 gfcb_getSctRoom(void);
#endif

/*****************************************************************************
 *	変数の宣言
 *****************************************************************************/

/*****************************************************************************
 *	関数の定義
 *****************************************************************************/

/*****************************************************************************
 *  CDバッファ資源管理構造体の初期化
 *［入力］
 *  flow	読み込み管理構造体
 *［出力］
 *  なし
 *［関数値］
 *  成功すればTRUE
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
Bool GFCB_Setup(GfsFlow *flow)
{
    GfsFinfo	*finfo = &GFS_FLW_FINFO(flow);
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);
    Sint32	bufno, fltno;
    CdcSubh	subh;

    GFS_DTS_FTYPE(dts) = CD_FILE;
    GFS_CDR_FMODE(dts) = FMODE_SELECT_FN;
    /* 絞りモード設定 */
    CDC_SUBH_FN(&subh) = GFS_FI_FN(finfo);
    CDC_SUBH_CN(&subh) = GFCB_NOSEL;
    CDC_SUBH_SMMSK(&subh) = GFCB_NOSEL;
    CDC_SUBH_SMVAL(&subh) = GFCB_NOSEL;
    CDC_SUBH_CIMSK(&subh) = 0;
    CDC_SUBH_CIVAL(&subh) = 0;
    GFS_CDR_SUBH(dts) = subh;
    GFS_CDR_PUID(dts) = GFCD_UID_NONE;
    GFS_CDR_SCTPOS(dts) = 0;
    GFS_CDR_FILEPOS(dts) = 0;
    GFS_CDR_SFAD(dts) = GFS_CDR_EFAD(dts) = 0;
    /* CDバッファ区画割り当て */
    if (GFCD_AllocBuf(&bufno) != GFCD_ERR_OK) {
        return FALSE;
    }
    GFS_CDR_BUFNO(dts) = bufno;
    /* 絞り割り当て */
    if (GFCD_AllocFilt(&fltno) != GFCD_ERR_OK) {
	GFCD_FreeBuf(bufno);
	return FALSE;
    }
    GFS_CDR_FLT(dts) = fltno;
    if (GFCD_SetFilt(fltno, GFS_CDR_FMODE(dts), &subh, 0, 0) != GFCD_ERR_OK) {
	return FALSE;
    }
    /* 絞りの接続先設定 */
    if (GFCD_SetFiltCon(fltno, bufno, CDC_NUL_SEL) != GFCD_ERR_OK) {
	return FALSE;
    }
    if (GFCD_WaitServer() != GFCD_ERR_OK) {
	return FALSE;
    }
    return TRUE;
}


/* 使用資源の解放 */
void GFCB_Close(GfsDtsrc *dts)
{
    GFCD_FreeBuf(GFS_CDR_BUFNO(dts));
    GFCD_FreeFilt(GFS_CDR_FLT(dts));
}


/*--- ＣＤバッファ操作 -------------------------------------------------*/


/*****************************************************************************
 *  CDバッファの有効セクタ数取得
 *［入力］
 *  flow	読み込み管理領域
 *  usct	転送要求セクタ数
 *［出力］
 *  asiz	転送可能バイト数
 *［関数値］
 *  転送可能セクタ数
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFCB_GetLenData(GfsFlow *flow, Sint32 usct, Sint32 *asiz)
{
    Sint32	ret, nsct;
    Sint32	sctsiz, fsct, lastsiz;
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    ret = GFCD_GetLenData(GFS_CDR_BUFNO(dts), GFS_CDR_SCTPOS(dts),
			  usct, &nsct, asiz);
    if (ret != GFCD_ERR_OK) {
	return 0;
    }
    if (GFCD_WaitServer() != GFCD_ERR_OK) {
	return 0;
    }
    if (asiz != NULL) {
	GFCF_GetFileInfo(flow,
			 NULL, NULL, &sctsiz, &fsct, &lastsiz, NULL, NULL);
	/* 最終セクタを含む場合 */
	if ((GFCB_Tell(dts) + nsct >= fsct) &&
	    (lastsiz != 0) && (nsct > 0)) {
	    *asiz -= sctsiz - lastsiz;
	}
    }
    return nsct;
}


/* CDバッファの空きバイト数取得 */
Sint32 GFCB_GetLenRoom(GfsDtsrc *dts)
{
/*    return GFCB_GetSctRoom() * GFCF_MD1_SCTSIZ;*/
    return 0;				/* 書き込みできないから */
}


/* 絞りの設定 */
void GFCB_SetFilt(GfsDtsrc *dts, Sint32 fmode, CdcSubh *subh)
{
    Sint32 ret;

    GFS_CDR_FMODE(dts) = fmode;
    GFS_CDR_SUBH(dts) = *subh;
    ret = GFCD_SetFilt(GFS_CDR_FLT(dts), fmode, subh, 0, 0);
    if (ret != GFCD_ERR_OK) {
	return;
    }
    GFCD_WaitServer();
}


/* CDバッファ全消去 */
Sint32 GFCB_DeleteAllData(GfsDtsrc *dts)
{
    GFCD_DelSctData(GFS_CDR_BUFNO(dts), BUF_TOP, CDC_SNUM_END);
    return GFCD_WaitServer();
}


/* アクセスポインタ取得 */
Sint32 GFCB_Tell(GfsDtsrc *dts)
{
    return GFS_CDR_FILEPOS(dts) + GFS_CDR_SCTPOS(dts);
}
 
 
/*****************************************************************************
 *  アクセスポインタ設定
 *［入力］
 *  cdb		ＣＤバッファハンドル
 *  off		オフセット
 *  org		移動の基準(GFS_SEEK_~)
 *［出力］
 *  なし
 *［関数値］
 *  アクセスポインタ
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFCB_Seek(GfsFlow *flow, Sint32 off, Sint32 org)
{
    Sint32	nsct;
    Sint32	ret;
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    if (off < 0) {
        off = 0;
    }
    if (org == GFS_SEEK_CUR) {
	nsct = off;
	off = GFCB_Tell(dts) + nsct;
    } else {
	if (GFCB_Tell(dts) == off) {
	    return off;
	}
	/* バッファにあるセクタ数のみを取得する */
	ret = GFCD_GetLenData(GFS_CDR_BUFNO(dts), BUF_TOP, CDC_SNUM_END, &nsct,
			      NULL);
	if (ret != GFCD_ERR_OK) {
	    return -1;
	}
	if (GFCD_WaitServer() != GFCD_ERR_OK) {
	    return -1;
	}
	if (nsct < 0) {
	    nsct = 0;
	}
    }
    if (GFS_FLW_GMODE(flow) == GFS_GMODE_ERASE) { /* 削除モード */
	/* 削除モードではアクセスポインタより前のセクタデータは削除する */
        /* バッファに読み込み済みの位置に移動する場合 */
        if ((GFS_CDR_FILEPOS(dts) <= off) &&
	    off < GFCB_Tell(dts)) {
            nsct = off - GFS_CDR_FILEPOS(dts);
        }
	if (nsct > 0) {
	    /* バッファデータ消去 */
	    GFCD_DelSctData(GFS_CDR_BUFNO(dts), BUF_TOP, nsct);
	    if (GFCD_WaitServer() != GFCD_ERR_OK) {
		return -1;
	    }
	}
	GFS_CDR_FILEPOS(dts) = off;		/* バッファ先頭位置 */
	GFS_CDR_SCTPOS(dts) = 0;		/* 転送位置 */
    } else if (GFS_FLW_GMODE(flow) == GFS_GMODE_RESIDENT) {    /* 常駐モード */
        /* ＣＤバッファに読み込まれていない位置に移動する場合 */
	/* 1.バッファ内データより前にシーク */
	/* 2.バッファ内データより後ろで連続でない（１セクタ以上すき間が空く）*/
        if ((off < GFS_CDR_FILEPOS(dts)) ||
	    (off > (GFCB_Tell(dts) + 1))) {
	    /* ＣＤバッファクリア */
            if (GFCB_DeleteAllData(dts) == GFCD_ERR_OK) {
		return -1;
	    }
            GFS_CDR_FILEPOS(dts) = off;
        }
        GFS_CDR_SCTPOS(dts) = off - GFS_CDR_FILEPOS(dts);
    }
    return GFCB_Tell(dts);
}
 
 

/*--- データ転送 -----------------------------------------------------*/

/*****************************************************************************
 *  転送元データパック取得
 *［入力］
 *  flow
 *  nsct	転送要求セクタ数
 *［出力］
 *  なし
 *［関数値］
 *  データパックハンドル
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
GfdpHn GFCB_GetSrcPack(GfsFlow *flow, Sint32 usct)
{
    Sint32      len, ret, nsct;
    Sint32	sctsiz, fsct, lastsiz;
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);
    GfdpHn      dpk = &MNG_SRCPK(gfs_mng_ptr);

    /* 使用中の場合 */
    if (GFDP_USE(dpk)) {
        return NULL;
    }

    /* 転送サイズを求める */
    if (GFCD_GetLenData(GFS_CDR_BUFNO(dts), GFS_CDR_SCTPOS(dts),
			usct, &nsct, &len) != GFCD_ERR_OK) {
	return NULL;
    }
    if (GFCD_WaitServer() != GFCD_ERR_OK) {
	return NULL;
    }
    if (len <= 0) {
	return NULL;
    }
    GFCF_GetFileInfo(flow, 
		     NULL, NULL, &sctsiz, &fsct, &lastsiz, NULL, NULL);
    /* 最終セクタを含む場合 */
    if ((GFCB_Tell(dts) + nsct >= fsct) &&
	(lastsiz != 0) && (nsct > 0)) {
	len -= sctsiz - lastsiz;
    }
    GFDP_DATA(dpk) = CDC_GetDataPtr();	/* 転送アドレス     */
    GFDP_ADLT(dpk) = CDB_ADDR_DLT;	/* アドレス加算分   */
    GFDP_LEN(dpk) = len;		/* 転送バイト数     */
    GFDP_NSCT(dpk) = nsct;		/* 転送セクタ数     */
    GFDP_USE(dpk) = TRUE;		/* データパック使用中   */
    return dpk;
}


/* データパックを取得した後のデータ要求 */
Bool GFCB_GetSctData(GfdpHn dpk, GfsDtsrc *dts)
{
    Sint32 ret;

    ret = GFCD_ReqData(GFS_CDR_BUFNO(dts), GFS_CDR_SCTPOS(dts),
		       GFDP_NSCT(dpk));
    if (ret != GFCD_ERR_OK) {
	return FALSE;
    }
    if (GFCD_WaitServer() != GFCD_ERR_OK) {
	return FALSE;
    }
    return TRUE;
}


/* 転送先データパック取得 */
GfdpHn GFCB_GetDstPack(GfsDtsrc *dts, Sint32 len)
{
    return NULL;
}


/*  データパック返還 */
void GFCB_RtnPk(GfsFlow *flow, GfdpHn dpk, Sint32 nsct)
{
    Sint32	txw;
    Sint32      seekpos;
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    GFCD_EndData(&txw);
    /* 転送が終了しているので,アクセスポインタを進める  */
    if (nsct == GFTR_ALLSCT) {
	nsct = GFDP_NSCT(dpk);
    }
    GFCB_Seek(flow, nsct, GFS_SEEK_CUR);
    /* データパック返還 */
    GFDP_USE(dpk) = FALSE;		/* データパック使用中解除 */
}


/* ピックアップの移動 */
Sint32 GFCB_MovePickup(GfsDtsrc *dts, CdcPos *pos)
{
    return GFCD_MovePickup(pos, GFS_CDR_PUID(dts));
}


/* CDデバイス接続 */
Bool GFCB_SetCon(GfsDtsrc *dts)
{
    Sint32 ec;

    ec = GFCD_SetCon(GFS_CDR_FLT(dts), GFS_CDR_PUID(dts));
    if (ec != GFCD_ERR_OK) {
	return FALSE;
    }
    ec = GFCD_WaitServer();
    if (ec != GFCD_ERR_OK) {
	return FALSE;
    }
    return TRUE;
}


/* ピックアップの制御権を確保する */
Bool GFCB_GetPickup(GfsDtsrc *dts)
{
    Sint32	puid = GFCD_GetPickup();
    
    if (puid < GFCD_ERR_OK) {
	return FALSE;
    }
    GFS_CDR_PUID(dts) = puid;
    return TRUE;
}


/* ピックアップを確保しているか調べる */
Bool GFCB_IsPuOwner(GfsDtsrc *dts)
{
    return GFCD_IsPuOwner(GFS_CDR_PUID(dts));
}


/* ピックアップを解放する */
void GFCB_UngetPickup(GfsDtsrc *dts)
{
    GFCD_UngetPickup(GFS_CDR_PUID(dts));
}


#if	0
/* CDバッファの空きセクタ数取得 */
Sint32 GFCB_GetSctRoom(void)
{
    Sint32      freesiz;		/* セクタ単位   */

    freesiz = GFCD_GetBufSiz();
    if (freesiz < 0) {
	freesiz = 0;
    }
    return freesiz;
}
#endif

/* end of file */
