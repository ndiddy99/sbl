/*****************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:ファイルシステム
 * Module :ライブラリインタフェース
 * File	  :gfs.c
 * Date   :1994-11-11
 * Version:1.16
 * Auther :M.S.
 *
 *****************************************************************************/

/*****************************************************************************
 *	インクルードファイル
 *****************************************************************************/
#include <string.h>

#include "sega_gfs.h"
#include <sega_int.h>
#include "gfs_buf.h"
#include "gfs_def.h"
#include "gfs_mmf.h"
#include "gfs_cdf.h"
#include "gfs_cdb.h"
#include "gfs_dir.h"
#include "gfs_trn.h"
#include "gfs_cdc.h"

#if defined(DEBUG_LIB)
#include "gfs_sf.h"
#endif

/*****************************************************************************
 *	定数マクロ
 *****************************************************************************/
/* 基本ボリュームディスクプリタに関する定数 */
#define PVD_FAD		166		/* FAD */
#define PVD_BYTSIZ	GFS_MD1_SCTSIZ	/* 大きさ（バイト単位）*/
#define PVD_SCTSIZ	1		/* 大きさ（セクタ単位） */

/* rootディレクトリが存在するFADの下限 */
#define ROOT_FAD_LOW_LIM	(150+16)

#define ROOT_RECPOS	156		/* ルートディレクトリの位置 */

/* SCDQが6.7ms間隔の時に1sのタイムアウト時間 */
#define FREAD_TMOUT_COUNT	3000

/* エラーコード */
#define SVR_NOHNDL	(-1)		/* サーバにハンドルが未登録 */

/*****************************************************************************
 *	列挙定数
 *****************************************************************************/

/*****************************************************************************
 *	処理マクロ
 *****************************************************************************/
/* LSN -> FAD変換 */
#define LSN_TO_FAD(lsn)	((lsn) += 150)

/* ディレクトリレコード読み込み		*/
#define DIRREC_EXT(rec)		((rec) + 6)
#define DIRREC_LEN(rec)		((rec) + 14)

/*****************************************************************************
 *	データ型の定義
 *****************************************************************************/

/*****************************************************************************
 *	関数の宣言
 *****************************************************************************/

/* ファイル操作関数テーブルの初期化 */
GFS_LOCAL void	gfs_mngSetupFuncTbl(void);

/* 管理テーブルの初期化 */
GFS_LOCAL void	gfs_mngInitWork(Sint32 open_max);

/* ファイル数のカウント */
GFS_LOCAL Sint32	gfs_mngGetNumFile(GfsDirTbl *dirtbl);

/* オープングループの解放 */
GFS_LOCAL void	gfs_mngFreeGrp(GfsFile *gfs);

/* ファイルアクセス完了処理 */
GFS_LOCAL void	gfs_mngTermAccess(GfsHn gfs);

/* エラー関数の呼びだし */
GFS_LOCAL Sint32	gfs_mngSetErrCode(Sint32 code);

/* アクセスサーバの検索 */
GFS_LOCAL Sint32	gfs_svrSearchHndl(GfsHn gfs);

/* アクセスサーバへの登録 */
GFS_LOCAL void	gfs_svrAddHndl(GfsHn gfs);

/* アクセスサーバから削除 */
GFS_LOCAL void	gfs_svrRemoveHndl(GfsHn gfs);

/* ボリュームディスクリプタの読み込み */
GFS_LOCAL Sint32	gfs_loadVol(Uint8 *buf);

/* ルートディレクトリ情報の取得 */
GFS_LOCAL Sint32	gfs_getRootDir(GfsDirId *dir);

/* ルートディレクトリレコードの読み込み */
GFS_LOCAL Sint32	gfs_loadRootDir(GfsDirTbl *dirtbl);

/* データをソースバッファに流し込む */
GFS_LOCAL Sint32	gfs_flowIn(GfsHn gfs);

/* ユーザ領域へのデータの転送 */
GFS_LOCAL Sint32	gfs_transData(GfsHn gfs);

/* 読み込み／書き込みセクタ数設定 */
GFS_LOCAL void	gfs_SetSct(GfsFlow *flow, Sint32 sct);

/* ファイル情報の取得 */
GFS_LOCAL void	gfs_GetFinfo(GfsFinfo *finfo, Sint32 *fid, Sint32 *fad,
			 Sint32 *sctsiz, Sint32 *nsct, Sint32 *lastsiz,
			 Sint32 *fno, Sint32 *atr);

/* ファイルクローズ下請け関数 */
GFS_LOCAL void	gfs_closeSub(GfsHn gfs);

/* 取り出しモードの取得 */
GFS_LOCAL Sint32	gfs_getGmode(GfsHn gfs);

/* 処理が完了したかを調べる */
GFS_LOCAL Bool	gfs_isTaskDone(GfsHn gfs);

/* flow inの状態をエラーコードに変換する */
GFS_LOCAL Sint32	gfs_convFinStat(GfsHn gfs, Sint32 fstat);


Sint32 bobtest;

/*****************************************************************************
 *	変数の定義
 *****************************************************************************/

/* 管理テーブル */
GfsMng *gfs_mng_ptr;

/* メモリファイルが使用可能の時は */
/* この変数にメモリファイル領域の先頭アドレスを設定する */
Sint8   *GFMC_base;

/*****************************************************************************
 *	関数の定義
 *****************************************************************************/

/*--- ＧＦＳインタフェース関数 ---------------------------------------------*/

/*****************************************************************************
 *  ファイルシステムの初期化、CD-ROMのマウント
 *［入力］
 *  open_max	オープンできるファイルの最大数
 *  work	ライブラリの作業領域
 *  dirtbl	ディレクトリ情報管理領域
 *［出力］
 *  なし
 *［関数値］
 *  ディレクトリ数。エラー発生時は負の値。
 *［機能］
 *  ファイルシステムの管理テーブルを初期化し、CD-ROMルートディレクトリを
 *  読み込む。
 *［備考］
 *  DEBUG_LIBではCDブロックファイルシステムが使えない。
 *  CDBFSを使うと、ファイル情報をデバッグファイルと混ぜられないため。
 *  
 *****************************************************************************/
Sint32 GFS_Init(Sint32 open_max, void *work, GfsDirTbl *dirtbl)
{
    Sint32	ndir, ret;
    
    /* 引数チェック */
    if (open_max < 1 || GFS_OPEN_MAX < open_max) {
	return GFS_ERR_OPENMAX;
    }
    if (((Uint32)work) & 3) {
	return GFS_ERR_ALIGN;
    }
    if (dirtbl != NULL) {
	if ((GFS_DIRTBL_TYPE(dirtbl) != GFS_DIR_ID &&
	     GFS_DIRTBL_TYPE(dirtbl) != GFS_DIR_NAME) ||
	    (GFS_DIRTBL_NDIR(dirtbl) < 2) ||
	    (GFS_DIRTBL_DIRID(dirtbl) == NULL)) {
	    return GFS_ERR_DIRTBL;
	}
    }
    /* SCU-DMA0終了割り込みを許可する */
    INT_ChgMsk(INT_MSK_DMA0, INT_MSK_NULL);

    gfs_mng_ptr = work;
    /* 各モジュール初期化 */
    gfs_mngInitWork(open_max);		/* マネージャ初期化 */
    ret = GFCF_Init((dirtbl == NULL)? TRUE: FALSE); /* CDF初期化 */
#if defined(DEBUG_LIB)
    if (dirtbl == NULL) {
	/* DEBUG_LIBではCDブロックファイルシステムを利用できない */
	return gfs_mngSetErrCode(GFS_ERR_CDBFS);
    }
    /* 簡易CDエミュレータドライバ初期化 */
    GFMF_Init();			/* MMF初期化 */
    GFSF_Init();			/* SCSIファイル初期化 */
#else
    if (ret != GFS_ERR_OK) {
	return gfs_mngSetErrCode(ret);
    }
    /* 製品版ではCD必須 */
    if (!MNG_CDCON(gfs_mng_ptr)) {
	return gfs_mngSetErrCode(GFS_ERR_CDNODISC);
    }
#endif /* defined(DEBUG_LIB) */
    /* ディレクトリ管理をどうするか */
    if (dirtbl == NULL) {
	/* CDブロックファイルシステムでディレクトリ管理を行なう */
	ndir = GFDR_ChgRoot();
	if (ndir < 0) {
	    return gfs_mngSetErrCode(GFS_ERR_CDBFS);
	}
    } else {
	/* GFSでディレクトリを管理する */
	ndir = gfs_loadRootDir(dirtbl); /* ルートディレクトリ読み込み	*/
	if (ndir <= 0) {
	    return gfs_mngSetErrCode(ndir);
	}
    }
    GFS_SetDir(dirtbl);		/* カレンントディレクトリ<-ルート */
    gfs_mngSetErrCode(GFS_ERR_OK);
    return ndir;
}


/*****************************************************************************
 *  ディレクトリ情報の読み込み
 *［入力］
 *  fid		ディレクトリの識別子
 *  dirtbl	ディレクトリ情報管理構造体
 *［出力］
 *  なし
 *［関数値］
 *  読み込んだディレクトリ数を返す。エラーの時は負のエラーコードを返す。
 *［機能］
 *  指定したディレクトリ情報をdirtblに読み込む。
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFS_LoadDir(Sint32 fid, GfsDirTbl *dirtbl)
{
    GfsHn	gfs;
    Sint32	nsct;
    Sint32	readdir;

    if (dirtbl == NULL) {
	/* GFS_InitでdirtblにNULLを設定したかチェック */
	if (DIRMNG_DIRID(&MNG_CURDIR(gfs_mng_ptr)) != NULL) {
	    return gfs_mngSetErrCode(GFS_ERR_CDBFS);
	}
	/* CDブロック内のディレクトリ情報にfidのものを設定 */
	readdir = GFDR_ChgDir(fid);
	if (readdir < 0) {
	    return gfs_mngSetErrCode(readdir);
	}
	return readdir;
    }
    gfs = GFS_Open(fid);
    if (gfs == NULL) {
	return GFS_ERR_CODE(&MNG_ERROR(gfs_mng_ptr));
    }
    GFS_GetFileSize(gfs, NULL, &nsct, NULL);
    /* ディレクトリ情報の読み込み */
    readdir = GFDR_SetupDirTbl(gfs, dirtbl, nsct);
    GFS_Close(gfs);
    return readdir;
}


/*****************************************************************************
 *  カレントディレクトリの設定
 *［入力］
 *  dirtbl	ディレクトリ情報管理領域
 *［出力］
 *  なし
 *［関数値］
 *  エラーコード
 *［機能］
 *  指定されたディレクトリ情報をカレントディレクトリに設定する、
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFS_SetDir(GfsDirTbl *dirtbl)
{
    GfsDirMng   *mng;

    mng = &MNG_CURDIR(gfs_mng_ptr);
    if (dirtbl == NULL) {
	/* GFS_InitでdirtblにNULLを設定したかチェック */
	if (DIRMNG_DIRID(mng) != NULL) {
	    return gfs_mngSetErrCode(GFS_ERR_CDBFS);
	}
	/* CDBFSでディレクトリ管理を行なう */
	DIRMNG_DIRID(mng) = NULL;
	DIRMNG_NDIR(mng) = DIRMNG_NFILE(mng) = 0;
    } else {
	/* GFSでディレクトリ管理を行なう */
	DIRMNG_DIRTBL(mng) = *dirtbl;
	DIRMNG_NFILE(mng) = gfs_mngGetNumFile(dirtbl); /* ファイル数設定 */
    }
    return gfs_mngSetErrCode(GFS_ERR_OK);
}


/*****************************************************************************
 *  ファイル名からファイル識別子に変換
 *［入力］
 *  fname	ファイル名
 *［出力］
 *  なし
 *［関数値］
 *  変換したファイル識別子。エラー時は負のエラーコード。
 *［機能］
 *  ファイル名をファイル識別子に変換する。
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFS_NameToId(Sint8 *fname)
{
    Sint32	fid;
    GfsDirMng	*cdir = &MNG_CURDIR(gfs_mng_ptr);

    if (DIRMNG_DIRID(cdir) == NULL ||
	DIRMNG_TYPE(cdir) != GFS_DIR_NAME) {
	return gfs_mngSetErrCode(GFS_ERR_NONAME);
    }
    fid = GFDR_NameToId(DIRMNG_DIRNAME(cdir), fname, DIRMNG_NDIR(cdir));
    if (fid < 0) {
	return gfs_mngSetErrCode(GFS_ERR_NEXIST);
    }
    gfs_mngSetErrCode(GFS_ERR_OK);
    return fid;
}


/*****************************************************************************
 *  識別子からファイル名への変換
 *［入力］
 *  fid		ファイル識別子
 *［出力］
 *  なし
 *［関数値］
 *  変換したファイル名へのポインタ。エラー時はNULL。
 *［機能］
 *  ファイル識別子をファイル名に変換する。
 *［備考］
 *  
 *****************************************************************************/
Sint8 *GFS_IdToName(Sint32 fid)
{
    GfsDirName	*pdir;
    GfsDirMng	*cdir = &MNG_CURDIR(gfs_mng_ptr);

    /* CDブロック内のファイル情報を使う時 */
    if (DIRMNG_DIRID(cdir) == NULL ||
	DIRMNG_TYPE(cdir) != GFS_DIR_NAME) {
	gfs_mngSetErrCode(GFS_ERR_NONAME);
	return NULL;
    }
    /* ファイル識別子の範囲をチェック */
    if ((fid < 0) || (DIRMNG_NFILE(cdir) <= fid)) {
	gfs_mngSetErrCode(GFS_ERR_NEXIST);
	return NULL;
    }
    pdir = GFDR_GetInfoDirName(fid, DIRMNG_DIRNAME(cdir));
    gfs_mngSetErrCode(GFS_ERR_OK);
    return GFS_DIR_FNAME(pdir);
}


/*****************************************************************************
 *  ファイルのオープン
 *［入力］
 *  fid		ファイル識別子
 *［出力］
 *  なし
 *［関数値］
 *  ファイルハンドル。エラー時はNULL。
 *［機能］
 *  指定したファイルを開く。
 *［備考］
 *  
 *****************************************************************************/
GfsHn GFS_Open(Sint32 fid)
{
    GfsDirId	*dirrec;
    GfsHn	gfs;
    GfsDirMng	*cdir = &MNG_CURDIR(gfs_mng_ptr);

    if (DIRMNG_DIRID(cdir) != NULL &&
	((fid < 0) || (fid >= DIRMNG_NFILE(cdir)))) {
	gfs_mngSetErrCode(GFS_ERR_FID);
        return NULL;
    }
    gfs = GFS_mngAllocGrp();		/* 空きグループ取得     */
    if (gfs  == NULL) {			/* 空き領域が無い場合   */
	gfs_mngSetErrCode(GFS_ERR_NOHNDL);
        return NULL;
    }
    /* ファイル情報取得 */
    if (DIRMNG_DIRID(cdir) == NULL) {
	dirrec = NULL;
    } else if (DIRMNG_TYPE(cdir) == GFS_DIR_ID) {
	dirrec = GFDR_GetInfoDirId(fid, DIRMNG_DIRID(cdir));
    } else {
	dirrec = (GfsDirId *)GFDR_GetInfoDirName(fid, DIRMNG_DIRNAME(cdir));
    }
    gfs = GFS_OpenSub(gfs, dirrec, fid);
    if (gfs == NULL) {
	gfs_mngSetErrCode(GFS_ERR_FID);
    } else {
	gfs_mngSetErrCode(GFS_ERR_OK);
    }
    return gfs;
}


/*****************************************************************************
 *  ファイルのクローズ
 *［入力］
 *  gfs		ファイルハンドル
 *［出力］
 *  なし
 *［関数値］
 *  なし
 *［機能］
 *  指定したファイルを閉じる。
 *［備考］
 *  
 *****************************************************************************/
void GFS_Close(GfsHn gfs)
{
    if (gfs != NULL) {
	gfs_mngTermAccess(gfs);		/* アクセス終了 */
	gfs_closeSub(gfs);
    }
    gfs_mngSetErrCode(GFS_ERR_OK);
}


/*****************************************************************************
 *  アクセスポインタの移動
 *［入力］
 *  gfs		ファイルハンドル
 *  ofs		移動オフセット
 *  org		基準位置
 *［出力］
 *  なし
 *［関数値］
 *  移動後のアクセスポインタを返す。
 *［機能］
 *  アクセスポインタを移動する。
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFS_Seek(GfsHn gfs, Sint32 ofs, Sint32 org)
{
    GfsFileFunc	*func;
    Sint32 nsct;
    GfsFlow	*flow;
    GfsDtsrc	*dts;

    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    flow = &GFS_FILE_FLOW(gfs);
    dts = &GFS_FLW_DTSRC(flow);
    func = &MNG_FUNCTBL(gfs_mng_ptr, GFS_DTS_FTYPE(dts));
    GFS_GetFileSize(gfs, NULL, &nsct, NULL);
    if (org == GFS_SEEK_CUR) {		/* 現在位置からシーク */
	ofs += (FUNC_TELL(func))(flow);
    } else if (org == GFS_SEEK_END) {	/* 終端からシーク */
	ofs = nsct + ofs;
    } else if (org != GFS_SEEK_SET) {
	/* 不正な基準位置 */
	return gfs_mngSetErrCode(GFS_ERR_ORG);
    }
    if (ofs < 0) {
	return gfs_mngSetErrCode(GFS_ERR_SEEK);
    }
    if (ofs > nsct) {
	return gfs_mngSetErrCode(GFS_ERR_SEEK);
    }
    ofs = (FUNC_SEEK(func))(flow, ofs);
    gfs_mngSetErrCode(GFS_ERR_OK);
    return ofs;
}


/*****************************************************************************
 *  アクセスポインタの取得
 *［入力］
 *  gfs		ファイルハンドル
 *［出力］
 *  なし
 *［関数値］
 *  アクセスポインタ
 *［機能］
 *  アクセスポインタを返す
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFS_Tell(GfsHn gfs)
{
    GfsFileFunc	*func;
    Sint32	ofs;
    GfsFlow	*flow;
    GfsDtsrc	*dts;

    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    flow = &GFS_FILE_FLOW(gfs);
    dts = &GFS_FLW_DTSRC(flow);
    func = &MNG_FUNCTBL(gfs_mng_ptr, GFS_DTS_FTYPE(dts));
    ofs = (FUNC_TELL(func))(flow);
    gfs_mngSetErrCode(GFS_ERR_OK);
    return ofs;
}


/*****************************************************************************
 *  ファイル終了のチェック
 *［入力］
 *  gfs		ファイルハンドル
 *［出力］
 *  なし
 *［関数値］
 *  TRUE	アクセスポインタがファイル末以降にある
 *  FALSE	その他
 *［機能］
 *  アクセスポインタファイル末に達したかどうか調べる。
 *［備考］
 *  
 *****************************************************************************/
Bool GFS_IsEof(GfsHn gfs)
{
    Sint32	pos, nsct;
	
    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    pos = GFS_Tell(gfs);
    GFS_GetFileSize(gfs, NULL, &nsct, NULL);
    gfs_mngSetErrCode(GFS_ERR_OK);
    /* ファイルの終端に達している場合	*/
    return (pos >= nsct)? TRUE: FALSE;
}


/*****************************************************************************
 *  バイト長からセクタ長への変換
 *［入力］
 *  gfs		ファイルハンドル
 *  nbyte	バイト数
 *［出力］
 *  なし
 *［関数値］
 *  セクタ単位に変換した結果
 *［機能］
 *  バイトからセクタへの単位の変換を行なう。
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFS_ByteToSct(GfsHn gfs, Sint32 nbyte)
{
    Sint32	sctsz;

    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    GFS_GetFileSize(gfs, &sctsz, NULL, NULL);
    if (sctsz == 0) {
	return 0;
    }
    gfs_mngSetErrCode(GFS_ERR_OK);
    return GFS_BYTE_SCT(nbyte, sctsz);
}


/*****************************************************************************
 *  ファイルサイズの取得
 *［入力］
 *  gfs		ファイルサイズ
 *［出力］
 *  sctsz	セクタサイズ
 *  nsct	総セクタ数
 *  lstlen	最終セクタ中有効データ数
 *［関数値］
 *  なし
 *［機能］
 *  ファイルの大きさに関する情報を取得する。
 *［備考］
 *  
 *****************************************************************************/
void GFS_GetFileSize(GfsHn gfs, Sint32 *sctsz, Sint32 *nsct, Sint32 *lstlen)
{
    Sint32	atr;
    GfsFlow	*flow;
	
    if (gfs == NULL) {
	gfs_mngSetErrCode(GFS_ERR_HNDL);
    } else {
	flow = &GFS_FILE_FLOW(gfs);
	gfs_GetFinfo(&GFS_FLW_FINFO(flow), NULL, NULL,
		     sctsz, nsct, lstlen, NULL, &atr);
	/* FORM1,FORM2が混在している場合はセクタサイズを設定しない */
	if (IS_FORM1(atr) && IS_FORM2(atr)) {
	    *sctsz = 0;
	}
    }
    gfs_mngSetErrCode(GFS_ERR_OK);
}


/*****************************************************************************
 *  ファイル情報の取得
 *［入力］
 *  gfs		ファイルハンドル
 *［出力］
 *  fid		ファイル識別子
 *  fn		ファイル番号
 *  fsize	大きさ（単位：バイト）
 *  atr		属性
 *［関数値］
 *  なし
 *［機能］
 *  ファイル情報を取得する。
 *［備考］
 *  
 *****************************************************************************/
void GFS_GetFileInfo(GfsHn gfs, Sint32 *fid, Sint32 *fn, Sint32 *fsize,
		     Sint32 *atr)
{
    Sint32	nsct;
    Sint32	lstlen;
    GfsFlow	*flow;

    if (gfs == NULL) {
	gfs_mngSetErrCode(GFS_ERR_HNDL);
    } else {
	flow = &GFS_FILE_FLOW(gfs);
	gfs_GetFinfo(&GFS_FLW_FINFO(flow), fid, NULL, NULL,
		     &nsct, &lstlen, fn, atr);
	if (atr != NULL) {
	    *atr &= ~GFS_ATR_FTYPE_MASK;	/* ファイル種別除去 */
	}
	if (fsize != NULL) {
	    *fsize = (nsct - 1) * GFS_MD1_SCTSIZ + lstlen;
	}
    }
    gfs_mngSetErrCode(GFS_ERR_OK);
}


/*****************************************************************************
 *  ファイルの一括読み込み
 *［入力］
 *  fid		ファイル識別子
 *  ofs		読み込み開始位置
 *  buf		読み込み領域
 *  bsize	読み込み領域サイズ
 *［出力］
 *  なし
 *［関数値］
 *  読み込みバイト数。エラー時は負の値。
 *［機能］
 *  ファイル識別子で指定されたファイルを読み込む。
 *  ファイルのオープンとクローズもこの関数で行なう。
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFS_Load(Sint32 fid, Sint32 ofs, void *buf, Sint32 bsize)
{
    GfsHn	gfs;
    Sint32	nsct;		/* 開始位置から終端までのセクタ数 */
    Sint32	bsct;
    Sint32	sctsiz1, sctsiz2;
    Sint32	readlen;
    Sint32	ap;
    GfsFinfo	*finfo;

    gfs = GFS_Open(fid);
    if (gfs == NULL) {
	return GFS_ERR_CODE(&MNG_ERROR(gfs_mng_ptr));
    }
    ap = GFS_Seek(gfs, ofs, GFS_SEEK_SET);
    if (ap < 0) {
	return gfs_mngSetErrCode(GFS_ERR_OFS);
    }
    finfo = &GFS_FLW_FINFO(&GFS_FILE_FLOW(gfs));
    sctsiz1 = sctsiz2 = GFS_FI_SCTSZ(finfo);
    /* Form1, Form2混在の場合バッファサイズ->セクタ数変換を最大値にする	*/
    if (sctsiz1 == 0) {
	sctsiz1 = GFS_FM2_SCTSIZ;
	sctsiz2 = GFS_FM1_SCTSIZ;
    }
    nsct = GFS_FI_NSCT(finfo) - ofs;	/* ファイル末までのセクタ数	*/
    /* GFS_BUFSIZ_INFが指定されたらファイル末まで読み込む */
    if (bsize == GFS_BUFSIZ_INF) {
	bsize = nsct * sctsiz1;
    }
    bsct = (bsize + sctsiz1 - 1) / sctsiz2;	/* バッファサイズに対応するセクタ数	*/
    nsct = MIN(nsct, bsct);			/* 読み込むセクタ数			*/
    readlen = GFS_Fread(gfs, nsct, buf, bsize);
    GFS_Close(gfs);
    if (readlen >= 0) {
	gfs_mngSetErrCode(GFS_ERR_OK);
    }
    return readlen;
}
	    

/*****************************************************************************
 *  データの読み込み
 *［入力］
 *  gfs		ファイルハンドル
 *  nsct	転送セクタ数
 *  buf		読み込み領域
 *  bsize	読み込み領域サイズ
 *［出力］
 *  なし
 *［関数値］
 *  読み込みバイト数
 *［機能］
 *  オープンされたファイルからデータを読み込む。
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFS_Fread(GfsHn gfs, Sint32 nsct, void *buf, Sint32 bsize)
{
    Sint32	ndata, err, amode, rpara_old, timer, atr;
    GfsFlow	*flow;
    GfsDtsrc	*dts;
    GfsTrans	*trn;
    CdcStat	stat;

    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    flow = &GFS_FILE_FLOW(gfs);
    /* デフォルトの読み込みパラメータを使用 */
    rpara_old = GFS_SetReadPara(gfs, GFS_RPARA_DFL);
    if (rpara_old != GFS_ERR_OK) {
	rpara_old = GFS_RPARA_DFL;
    }
    err = GFS_NwFread(gfs, nsct, buf, bsize);
    if (err != GFS_ERR_OK) {
	GFS_FLW_SCTMAX(flow) = rpara_old;
	return err;
    }
    dts = &GFS_FLW_DTSRC(flow);
    trn = &GFS_FILE_TRANS(gfs);
    GFS_GetFileInfo(gfs, NULL, NULL, NULL, &atr);
    for (timer = -1;;) {
	err = GFS_NwExecOne(gfs);
	if (err == GFS_SVR_COMPLETED) {
	    break;
	}
	if (err < 0) {
	    GFS_FLW_SCTMAX(flow) = rpara_old;
	    return err;
	}
	/* 転送が先に終った時 */
	if (GFS_TRN_STAT(trn) == GFTR_ST_END) {
	    /* nsctより大きい値をGFS_NwCdReadすると */
	    /* 転送だけ先に終る場合がある */
	    break;
	}
	if (GFS_DTS_FTYPE(dts) == CD_FILE) {
	    if (timer < 0) {
		/* まだタイムアウト計測が始まっていない */
		GFCD_GetStat(&stat);
		if ((CDC_STAT_STATUS(&stat) & 0x0f) != CDC_ST_STANDBY &&
		    (atr & GFS_ATR_CDDA) == 0) {
		    /* <STANDBY>でなくなったらタイムアウト計測開始 */
		    /* CDDAファイルを除く */
		    timer = 0;
		}
	    }
	    /* SCDQフラグが立つのを待って、GFS_NwExecOneの実行間隔を */
	    /* ある程度空ける */
	    GFCD_WaitScdqFlag();
	    if (timer > FREAD_TMOUT_COUNT) {
		GFS_NwGetStat(gfs, &amode, &ndata); /* 転送バイト数取得 */
		if (ndata <= 0) {
		    GFS_FLW_SCTMAX(flow) = rpara_old;
		    return gfs_mngSetErrCode(GFS_ERR_TMOUT);
		}
	    }
	    if (timer >= 0) {
		++timer;
	    }
	}
    }
    GFS_NwGetStat(gfs, &amode, &ndata); /* 転送バイト数取得 */
    gfs_mngSetErrCode(GFS_ERR_OK);
    GFS_FLW_SCTMAX(flow) = rpara_old;
    return ndata;
}


/*****************************************************************************
 *  データ読み込みの開始
 *［入力］
 *  gfs		ファイルハンドル
 *  nsct	読み込みセクタ数
 *  buf		読み込み領域
 *  bsize	読み込み領域の大きさ（単位：バイト）
 *［出力］
 *  なし
 *［関数値］
 *  エラーコード
 *［機能］
 *  読み込みリクエストを登録する。
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFS_NwFread(GfsHn gfs, Sint32 nsct, void *buf, Sint32 bsize)
{
    Sint32	remain, atr;
    Bool	add_flag = FALSE;
    GfsFlow	*flow;
    GfsFinfo	*finfo;
    GfsTrans	*trn;

    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    flow = &GFS_FILE_FLOW(gfs);
    finfo = &GFS_FLW_FINFO(flow);
    trn = &GFS_FILE_TRANS(gfs);
    remain = GFS_FI_NSCT(finfo) - GFS_Tell(gfs);
    remain = MIN(remain, nsct);		/* 読み込みセクタ数 */
    if (remain < 0) {
	return gfs_mngSetErrCode(GFS_ERR_PARA);
    }
    if (GFS_FILE_AMODE(gfs) == GFS_NWSTAT_NOACT) {
	/* flow inタスクを登録 */
	gfs_SetSct(flow, remain);
	add_flag = TRUE;
    }
    GFS_FILE_AMODE(gfs) = GFS_NWSTAT_FREAD; /* アクセスモード */
    GFS_GetFileInfo(gfs, NULL, NULL, NULL, &atr);
    if (atr & GFS_ATR_CDDA) {
	/* CDDAファイルは転送しない */
	GFTR_SetTransPara(trn, 0);	/* 転送セクタ数 */
	GFTR_SetData(trn, NULL, 0, 1);	/* 転送先設定 */
    } else if (GFS_TRN_STAT(trn) == GFTR_ST_NOACT ||
	       GFS_TRN_STAT(trn) == GFTR_ST_END) {
	/* transタスクを登録 */
	GFTR_SetTransPara(trn, remain); /* 転送セクタ数 */
	GFTR_SetData(trn, buf, bsize, 1); /* 転送先設定 */
	GFTR_Start(trn);
	add_flag = TRUE;
    }
    if (add_flag == FALSE) {
	return gfs_mngSetErrCode(GFS_ERR_FBUSY);
    }
    gfs_svrAddHndl(gfs);	    	/* アクセスサーバへの登録 */
    GFS_FILE_ASTAT(gfs) = GFS_SVR_BUSY;	/* アクセス状態 */
    return gfs_mngSetErrCode(GFS_ERR_OK);
}


/*****************************************************************************
 *  CDバッファへの先読みの開始
 *［入力］
 *  gfs		ファイルハンドル
 *  nsct	読み込みセクタ数
 *［出力］
 *  なし
 *［関数値］
 *  エラーコード
 *［機能］
 *  CDバッファへの先読みリクエストを登録する。
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFS_NwCdRead(GfsHn gfs, Sint32 nsct)
{
    Sint32	remain;
    GfsFlow	*flow;
    GfsFinfo	*finfo;

    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    flow = &GFS_FILE_FLOW(gfs);
    finfo = &GFS_FLW_FINFO(flow);
    if (GFS_FILE_AMODE(gfs) != GFS_NWSTAT_NOACT) {
	return gfs_mngSetErrCode(GFS_ERR_FBUSY);
    }
    /* flow inタスク登録 */
    remain = GFS_FI_NSCT(finfo) - GFS_Tell(gfs);
    remain = MIN(remain, nsct);		/* 読み込みセクタ数 */
    gfs_SetSct(flow, remain);
    GFS_FILE_AMODE(gfs) = GFS_NWSTAT_CDREAD; /* アクセスモード */
    GFS_FILE_ASTAT(gfs) = GFS_SVR_BUSY; /* 処理フラグ */
    gfs_svrAddHndl(gfs);		/* アクセスサーバへの登録 */
    return gfs_mngSetErrCode(GFS_ERR_OK);
}


/*****************************************************************************
 *  アクセス動作完了チェック
 *［入力］
 *  gfs		ファイルハンドル
 *［出力］
 *  なし
 *［関数値］
 *  TRUE	完了
 *  FALSE	動作中
 *［機能］
 *  アクセス動作が完了したか調べる
 *［備考］
 *  
 *****************************************************************************/
Bool GFS_NwIsComplete(GfsHn gfs)
{
    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    gfs_mngSetErrCode(GFS_ERR_OK);
    return (GFS_FILE_ASTAT(gfs) == GFS_SVR_COMPLETED)? TRUE: FALSE;
}


/*****************************************************************************
 *  アクセス動作の中止
 *［入力］
 *  gfs		ファイルハンドル
 *［出力］
 *  なし
 *［関数値］
 *  現在のアクセスポインタ
 *［機能］
 *  アクセス動作を停止させる。
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFS_NwStop(GfsHn gfs)
{
    Sint32	ap;
	
    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    GFS_FILE_AMODE(gfs) = GFS_NWSTAT_NOACT;
    GFS_FILE_ASTAT(gfs) = GFS_SVR_COMPLETED; /* 処理終了 */
    gfs_svrRemoveHndl(gfs);		/* サーバから削除 */
    gfs_mngTermAccess(gfs);		/* 完了処理 */
    ap = GFS_Tell(gfs);
    gfs_mngSetErrCode(GFS_ERR_OK);
    return ap;
}


/*****************************************************************************
 *  アクセス状態の取得
 *［入力］
 *  gfs		ファイルハンドル
 *［出力］
 *  amode	アクセスモード
 *  ndata	転送データ数
 *［関数値］
 *  なし
 *［機能］
 *  サーバ関数のアクセスモードと転送データ数を出力する。
 *  転送データ数はアクセスモードが、
 *	GFS_NWSTAT_FREADのとき、転送バイト数
 *	GFS_NWSTAT_CDREADのとき、転送可能セクタ数
 *［備考］
 *  
 *****************************************************************************/
void GFS_NwGetStat(GfsHn gfs, Sint32 *amode, Sint32 *ndata)
{
    GfsTrans	*trn;
    Sint32	atr;

    if (gfs == NULL) {
	gfs_mngSetErrCode(GFS_ERR_HNDL);
    } else {
	trn = &GFS_FILE_TRANS(gfs);
	*amode = GFS_FILE_AMODE(gfs);
	GFS_GetFileInfo(gfs, NULL, NULL, NULL, &atr);
	if (atr & GFS_ATR_CDDA) {
	    /* CDDAファイルは転送を行なわない */
	    *ndata = 0;
	} else if (GFS_FILE_AMODE(gfs) == GFS_NWSTAT_CDREAD) {
	    /* CDバッファ内セクタ数を出力 */
	    *ndata = GFBF_GetNumData(gfs);
	} else {
	    /* 転送バイト数を出力 */
	    *ndata = GFS_TRN_WP(trn);
	}
    }
    gfs_mngSetErrCode(GFS_ERR_OK);
}


/*****************************************************************************
 *  リードエラー発生時のリカバリ処理
 *［入力］
 *  gfs		ファイルハンドル
 *［出力］
 *  なし
 *［関数値］
 *  処理状態
 *［機能］
 *  リードエラー発生時のプレイ継続
 *［備考］
 *  
 *****************************************************************************/
GFS_LOCAL Sint32	gfs_recovRdErr(GfsHn gfs)
{
    Sint32	fstat;
    GfsDtsrc	*dtsrc;
    GfsFlow	*flow;

    flow = &GFS_FILE_FLOW(gfs);
    dtsrc = &GFS_FLW_DTSRC(flow);
    if (GFS_DTS_FTYPE(dtsrc) == CD_FILE) {
	fstat = GFCF_Recover(flow);
	if (fstat == GFS_FIN_PLAY) {
	    GFS_FILE_ASTAT(gfs) = GFS_SVR_BUSY;	/* リカバリ成功	*/
	}
    }
    return GFS_FILE_ASTAT(gfs);
}



/*****************************************************************************
 *  ファイル単位でのアクセス動作の実行
 *［入力］
 *  gfs		ファイルハンドル
 *［出力］
 *  なし
 *［関数値］
 *  処理状態
 *［機能］
 *  リクエストに対するアクセス処理を行なう。
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFS_NwExecOne(GfsHn gfs)
{
    Sint32	tstat, ec;

    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    if (GFS_FILE_ASTAT(gfs) != GFS_SVR_COMPLETED) {
	if (GFS_FILE_ASTAT(gfs) == GFS_ERR_CDRD) {
	    ec = gfs_recovRdErr(gfs);
	    return ec;
	}
	ec = gfs_flowIn(gfs);
	if (ec < 0) {
	    return ec;
	}
	tstat = gfs_transData(gfs);
	if (tstat == GFTR_ST_END) {
	    /* 転送中にflowinの状態が遷移する場合もあるので */
	    ec = gfs_flowIn(gfs);
	    if (ec < 0) {
		return ec;
	    }
	    if (gfs_isTaskDone(gfs)) {
		GFS_FILE_AMODE(gfs) = GFS_NWSTAT_NOACT;
		GFS_FILE_ASTAT(gfs) = GFS_SVR_COMPLETED;
		gfs_svrRemoveHndl(gfs);	/* サーバから削除 */
	    }
	}
    }
    gfs_mngSetErrCode(GFS_ERR_OK);
    return GFS_FILE_ASTAT(gfs);
}


/*****************************************************************************
 *  複数ファイルのアクセス動作の実行
 *［入力］
 *  なし
 *［出力］
 *  now_gfs	アクセス中ファイルのハンドル
 *［関数値］
 *  アクセスサーバの状態
 *［機能］
 *  リクエスト関数に対するファイルアクセス処理を行なう。
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFS_NwExecServer(GfsHn *now_gfs)
{
    GfsSvr	*svr;
    Sint32	stat, i;

    svr = &MNG_SVR(gfs_mng_ptr);
    for (i = 0; i < SVR_NFILE(svr); ++i) {
	stat = GFS_NwExecOne(SVR_ACFILE(svr, i));
	if (stat < 0) {
	    break;
	}
	/* 次のファイルに対して先読みを試みる */
	if (stat == GFS_SVR_COMPLETED) {
	    /* 待ち行列が詰められた */
	    --i;
	}
    }
    if (SVR_NFILE(svr) == 0) {
	*now_gfs = NULL;
	stat = GFS_SVR_COMPLETED;	/* 処理終了 */
    } else {
	*now_gfs = SVR_ACFILE(svr, 0);	/* 処理中ハンドル */
	if (stat == GFS_ERR_PUINUSE && i > 0) {
	    stat = GFS_SVR_BUSY;
	}
    }
    return stat;
}


/*****************************************************************************
 *  取り出しモードの設定
 *［入力］
 *  gfs		ファイルハンドル
 *  gmode	取り出しモード（GFS_GMODE_ERASE, GFS_GMODE_RESIDENT）
 *［出力］
 *  なし
 *［関数値］
 *  設定前の取り出しモード
 *［機能］
 *  取り出しモードを設定する。
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFS_SetGmode(GfsHn gfs, Sint32 gmode)
{
    Sint32 old_mode, atr;
    GfsFlow	*flow;
    GfsDtsrc	*dtsrc;

    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    /* CDDAファイルに対しては取り出しモードを変更できない */
    GFS_GetFileInfo(gfs, NULL, NULL, NULL, &atr);
    if (atr & GFS_ATR_CDDA) {
	return GFS_ERR_PARA;
    }
    flow = &GFS_FILE_FLOW(gfs);
    dtsrc = &GFS_FLW_DTSRC(flow);
    old_mode = GFS_FLW_GMODE(flow);
    GFS_FLW_GMODE(flow) = gmode;
    gfs_mngSetErrCode(GFS_ERR_OK);
    return old_mode;
}



/*****************************************************************************
 *  転送モードの設定
 *［入力］
 *  gfs		ファイルハンドル
 *  tmode	転送モード（GFS_TMODE_SCU,GFS_TMODE_SDMA[01],GFS_TMODE_CPU）
 *［出力］
 *  なし
 *［関数値］
 *  設定前の転送モード
 *［機能］
 *  転送モードを設定する
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFS_SetTmode(GfsHn gfs, Sint32 tmode)
{
    GfsFlow	*flow;
    GfsDtsrc	*dtsrc;
    GfsTrans	*trans;
    Sint32	old_mode, atr;

    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    /* CDDAファイルに対しては転送モードを変更できない */
    GFS_GetFileInfo(gfs, NULL, NULL, NULL, &atr);
    if (atr & GFS_ATR_CDDA) {
	return GFS_ERR_PARA;
    }
    flow = &GFS_FILE_FLOW(gfs);
    dtsrc = &GFS_FLW_DTSRC(flow);
    trans = &GFS_FILE_TRANS(gfs);
    old_mode = GFS_TRN_MODE(trans);
    GFS_TRN_MODE(trans) = tmode;
    old_mode = GFTR_SetMode(trans, tmode);
    gfs_mngSetErrCode(GFS_ERR_OK);
    return old_mode;
}


/*****************************************************************************
 *  CDバッファへの読み込み量の設定
 *［入力］
 *  gfs		ファイルハンドル
 *  nsct	１回の最大読み込み量（単位：セクタ）
 *［出力］
 *  なし
 *［関数値］
 *  設定前の読み込み量
 *［機能］
 *  １回の読み込み量を設定する
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFS_SetReadPara(GfsHn gfs, Sint32 nsct)
{
    GfsFlow	*flow;
    Sint32	old_para, atr;

    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    /* CDDAファイルに対しては読み込み量を変更できない */
    GFS_GetFileInfo(gfs, NULL, NULL, NULL, &atr);
    if (atr & GFS_ATR_CDDA) {
	return GFS_ERR_PARA;
    }
    flow = &GFS_FILE_FLOW(gfs);
    old_para = GFS_FLW_SCTMAX(flow);
    GFS_FLW_SCTMAX(flow) = nsct;
    gfs_mngSetErrCode(GFS_ERR_OK);
    return old_para;
}


/*****************************************************************************
 *  CDバッファからの転送量の設定
 *［入力］
 *  gfs		ファイルハンドル
 *  tsize	１回の転送量（単位：セクタ）
 *［出力］
 *  なし
 *［関数値］
 *  設定前の転送量
 *［機能］
 *  １回の最大転送量を設定する
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFS_SetTransPara(GfsHn gfs, Sint32 tsize)
{
    Sint32 old_para, atr;

    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    /* CDDAファイルに対しては転送量を変更できない */
    GFS_GetFileInfo(gfs, NULL, NULL, NULL, &atr);
    if (atr & GFS_ATR_CDDA) {
	return GFS_ERR_PARA;
    }
    old_para = GFTR_SetTransMax(&GFS_FILE_TRANS(gfs), tsize);
    gfs_mngSetErrCode(GFS_ERR_OK);
    return old_para;
}



/*****************************************************************************
 *  CDのピックアップの移動
 *［入力］
 *  gfs		ファイルハンドル
 *［出力］
 *  なし
 *［関数値］
 *  エラーコード
 *［機能］
 *  ピックアップをアクセスポインタの位置に移動する。
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFS_CdMovePickup(GfsHn gfs)
{
    Sint32	ap, err;
    GfsFlow	*flow;
    GfsDtsrc	*dtsrc;

    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    flow = &GFS_FILE_FLOW(gfs);
    dtsrc = &GFS_FLW_DTSRC(flow);
    if (GFS_DTS_FTYPE(dtsrc) == CD_FILE) {
	ap = GFS_Tell(gfs);
	err = GFCF_MovePickup(flow, ap);
	if (err != GFS_ERR_OK) {
	    return gfs_mngSetErrCode(err);
	}
	MNG_PICKUP(gfs_mng_ptr) = gfs;
    }
    return gfs_mngSetErrCode(GFS_ERR_OK);
}


/*****************************************************************************
 *  エラー処理関数の設定
 *［入力］
 *  func	エラー処理関数
 *  obj		funcの第１引数
 *［出力］
 *  なし
 *［関数値］
 *  なし
 *［機能］
 *  エラー処理関数を登録する
 *［備考］
 *  
 *****************************************************************************/
void GFS_SetErrFunc(GfsErrFunc func, void *obj)
{
    GFS_ERR_FUNC(&MNG_ERROR(gfs_mng_ptr)) = func;
    GFS_ERR_OBJ(&MNG_ERROR(gfs_mng_ptr)) = obj;
    gfs_mngSetErrCode(GFS_ERR_OK);
}


/*****************************************************************************
 *  エラー状態の取得
 *［入力］
 *  なし
 *［出力］
 *  stat	エラー情報
 *［関数値］
 *  なし
 *［機能］
 *  エラー状態を取得する
 *［備考］
 *  
 *****************************************************************************/
void GFS_GetErrStat(GfsErrStat *stat)
{
    *stat = MNG_ERROR(gfs_mng_ptr);
}


/*****************************************************************************
 *  ファイル識別子からディレクトリレコード取得
 *［入力］
 *  fid		ファイル識別子
 *［出力］
 *  dirrec	ディレクトリレコード
 *［関数値］
 *  エラーコード
 *［機能］
 *  ファイル識別子からディレクトリ情報を取得する
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFS_GetDirInfo(Sint32 fid, GfsDirId *dirrec)
{
    GfsDirName	*pdirn;
    GfsDirId	*pdir;
    GfsDirMng	*cdir = &MNG_CURDIR(gfs_mng_ptr);

    /* CDブロック内のファイル情報を使う時 */
    /* 範囲外の時 */
    if ((DIRMNG_DIRID(cdir) == NULL) ||
	(fid < 0) || (fid >= DIRMNG_NFILE(cdir))) {
	return gfs_mngSetErrCode(GFS_ERR_FID);
    }
    if ((DIRMNG_TYPE(cdir) == GFS_DIR_NAME)) {
	pdirn = GFDR_GetInfoDirName(fid, DIRMNG_DIRNAME(cdir));
	memcpy(dirrec, pdirn, sizeof(GfsDirId));
    } else {
	pdir = GFDR_GetInfoDirId(fid, DIRMNG_DIRID(cdir));
	memcpy(dirrec, pdir, sizeof(GfsDirId));
    }
    return gfs_mngSetErrCode(GFS_ERR_OK);
}


/*--- ＧＦＳマネージャ -----------------------------------------------------*/

/*****************************************************************************
 *  ファイル操作関数テーブルの初期化
 *［入力］
 *  なし
 *［出力］
 *  なし
 *［関数値］
 *  なし
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
GFS_LOCAL void gfs_mngSetupFuncTbl(void)
{
    GfsFileFunc		*func;

    /* CDファイル操作関数の設定	*/
    func = &MNG_FUNCTBL(gfs_mng_ptr, CD_FILE);
    FUNC_FLOWIN(func) = GFCF_FlowInBuf;
    FUNC_STOPIN(func) = GFCF_StopInBuf;
    FUNC_SEEK(func) = GFCF_Seek;
    FUNC_TELL(func) = GFCF_Tell;
#if defined(DEBUG_LIB)
    /* メモリファイル操作関数の設定	*/
    func = &MNG_FUNCTBL(gfs_mng_ptr, MEM_FILE);
    FUNC_FLOWIN(func) = GFMF_FlowInBuf;
    FUNC_STOPIN(func) = GFMF_StopInBuf;
    FUNC_SEEK(func) = GFMF_Seek;
    FUNC_TELL(func) = GFMF_Tell;
    /* SCSIファイル操作関数の設定 */
    func = &MNG_FUNCTBL(gfs_mng_ptr, SCSI_FILE);
    FUNC_FLOWIN(func) = GFSF_FlowInBuf;
    FUNC_STOPIN(func) = GFSF_StopInBuf;
    FUNC_SEEK(func) = GFSF_Seek;
    FUNC_TELL(func) = GFSF_Tell;
#endif
}


/*****************************************************************************
 *  管理テーブルの初期化
 *［入力］
 *  open_max	同時オープン最大数
 *［出力］
 *  なし
 *［関数値］
 *  なし
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
GFS_LOCAL void gfs_mngInitWork(Sint32 open_max)
{
    Sint32      i;
    GfsFile	*fp;
    GfdpHn	dpk;

    MNG_OPENMAX(gfs_mng_ptr) = open_max;
    /* ファイル操作関数の設定 */
    gfs_mngSetupFuncTbl();
    SVR_NFILE(&MNG_SVR(gfs_mng_ptr)) = 0;	/* キューの長さは０ */
    MNG_PICKUP(gfs_mng_ptr) = MNG_TRANS(gfs_mng_ptr) = NULL;
    MNG_SFAD(gfs_mng_ptr) = MNG_EFAD(gfs_mng_ptr) = 0;
    GFS_ERR_FUNC(&MNG_ERROR(gfs_mng_ptr)) = NULL;
    MNG_FLAGS(gfs_mng_ptr) = 0;
    fp = MNG_FILE(gfs_mng_ptr);
    for (i = 0; i < open_max; i++) {
        GFS_FILE_USED(fp) = FALSE;
	++fp;
    }
    dpk = &MNG_SRCPK(gfs_mng_ptr);
    GFDP_DATA(dpk) = NULL;
    GFDP_ADLT(dpk) = GFDP_LEN(dpk) =  GFDP_NSCT(dpk) = 0;
    GFDP_USE(dpk) = FALSE;
    dpk = &MNG_DSTPK(gfs_mng_ptr);
    GFDP_DATA(dpk) = NULL;
    GFDP_ADLT(dpk) = GFDP_LEN(dpk) =  GFDP_NSCT(dpk) = 0;
    GFDP_USE(dpk) = FALSE;
}


/*****************************************************************************
 *  ファイル数のカウント
 *［入力］
 *  dirtbl	ディレクトリ情報管理領域
 *［出力］
 *  なし
 *［関数値］
 *  ディレクトリエントリの数
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
GFS_LOCAL Sint32 gfs_mngGetNumFile(GfsDirTbl *dirtbl)
{
    Sint32	i;
    Sint32	atr;

    for (i = 0; i < GFS_DIRTBL_NDIR(dirtbl); i++) {
	if (GFS_DIRTBL_TYPE(dirtbl) == GFS_DIR_ID) {
	    /* ファイル識別子を使用 */
	    atr = GFDR_GetAtrDirId(i, GFS_DIRTBL_DIRID(dirtbl));
	} else {
	    /* ファイル名を使用 */
	    atr = GFDR_GetAtrDirName(i, GFS_DIRTBL_DIRNAME(dirtbl));
	}
	if (IS_ENDTBL(atr)) {
	    break;
	}
    }
    if (i > GFS_DIRTBL_NDIR(dirtbl)) {
	return 0;
    }
    return i + 1;
}

 
/* 使用していないファイルハンドルの取得 */
GfsFile *GFS_mngAllocGrp(void)
{
    Sint32      i;
    GfsFile	*gfs;

    /* 使用していないハンドルを探す */
    gfs = MNG_FILE(gfs_mng_ptr);
    for (i = 0; i < MNG_OPENMAX(gfs_mng_ptr); i++) {
        if (GFS_FILE_USED(gfs) == FALSE) {
            break;
        }
	++gfs;
    }
    /* すべて使用している場合 */
    if (i == MNG_OPENMAX(gfs_mng_ptr)) {
        gfs = NULL;
    } else {
	GFS_FILE_USED(gfs) = TRUE;	/* 使用中フラグ設定 */
    }
    return gfs;
}


/* オープングループの解放 */
GFS_LOCAL void gfs_mngFreeGrp(GfsFile *gfs)
{
    GFS_FILE_USED(gfs) = FALSE;		/* 使用中フラグ解除 */
}


/* ファイルアクセス完了処理 */
GFS_LOCAL void gfs_mngTermAccess(GfsHn gfs)
{
    GfsFileFunc	*func;
    GfsFlow	*flow = &GFS_FILE_FLOW(gfs);

    if (MNG_PICKUP(gfs_mng_ptr) == gfs ||
	MNG_PICKUP(gfs_mng_ptr) == NULL) {	/* ピックアップ登録解除	*/
	MNG_PICKUP(gfs_mng_ptr) = NULL;
	func = &MNG_FUNCTBL(gfs_mng_ptr, GFS_DTS_FTYPE(&GFS_FLW_DTSRC(flow)));
	(FUNC_STOPIN(func))(flow, TRUE);
	GFS_FLW_STAT(flow) = GFS_FIN_NOACT;
	GFS_FLW_SCTCNT(flow) = 0;
    }
    if (MNG_TRANS(gfs_mng_ptr) == gfs) {	/* 転送解除 */
	MNG_TRANS(gfs_mng_ptr) = NULL;
	GFTR_Stop(gfs, GFTR_ALLSCT);	/* 転送停止 */
    }
}


/* エラー関数の呼びだし */
GFS_LOCAL Sint32 gfs_mngSetErrCode(Sint32 code)
{
    GfsErrStat	*err;

    err = &MNG_ERROR(gfs_mng_ptr);
    GFS_ERR_CODE(err) = code;
    /* エラー関数が登録されている場合 */
    if (code != GFS_ERR_OK && GFS_ERR_FUNC(err) != NULL) {
	GFS_ERR_FUNC(err)(GFS_ERR_OBJ(err), code); /* エラー処理関数実行 */
    }
    return code;
}


/*--- アクセスサーバ -------------------------------------------------------*/

/* アクセスサーバの検索 */
GFS_LOCAL Sint32 gfs_svrSearchHndl(GfsHn gfs)
{
    Sint32      i;
    GfsSvr      *svr;

    svr = &MNG_SVR(gfs_mng_ptr);
    /* サーバにgfsが登録されているかサーチする */
    for (i = 0; i < SVR_NFILE(svr); i++) {
        if (gfs == SVR_ACFILE(svr, i)) {
            break;
        }
    }
    if (i == SVR_NFILE(svr)) {		/* 登録されていない場合	*/
        i = SVR_NOHNDL;
    }
    return i;
}


/* アクセスサーバへの登録 */
GFS_LOCAL void gfs_svrAddHndl(GfsHn gfs)
{
    GfsSvr      *svr;
    Sint32      i;
    
    i = gfs_svrSearchHndl(gfs);     /* ハンドルが登録済みかどうか調べる */
    /* 無い場合登録 */
    if (i == SVR_NOHNDL) {
        svr = &MNG_SVR(gfs_mng_ptr);
        SVR_ACFILE(svr, SVR_NFILE(svr)) = gfs;
        ++SVR_NFILE(svr);
    }
}


/* アクセスサーバから削除 */
GFS_LOCAL void gfs_svrRemoveHndl(GfsHn gfs)
{
    GfsSvr      *svr;
    Sint32      i, nfile;

    if (gfs == NULL) {
	return;
    }
    i = gfs_svrSearchHndl(gfs);		/* ハンドルが登録済みかどうか調べる */
    /* 登録されている場合 */
    if (i != SVR_NOHNDL) {
	svr = &MNG_SVR(gfs_mng_ptr);
        --SVR_NFILE(svr);		/* 登録個数を減らす */
        nfile = SVR_NFILE(svr);
        for ( ; i < nfile; i++) {       /* 後ろをつめる */
            SVR_ACFILE(svr, i) = SVR_ACFILE(svr, i + 1);
        }
	SVR_ACFILE(svr, i) = NULL;
    }
}


/*--- その他 -----------------------------------------------------*/

/*****************************************************************************
 *  ファイルハンドルの割当てサブ関数
 *［入力］
 *  opngrp	割り当てるファイル管理構造体
 *  dirrec	ディレクトリレコード
 *［出力］
 *  なし
 *［関数値］
 *  割り当てられたＧＦＳ領域
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
GfsHn GFS_OpenSub(GfsFile *gfs, GfsDirId *dirrec, Sint32 fid)
{
    GftrHn	gftr;
    Bool	ret;

    if (gfs == NULL) {
	return NULL;
    }
    gftr = GFTR_Setup(&GFS_FILE_TRANS(gfs));
#if defined(DEBUG_LIB)
    if (dirrec != NULL) {
	/* ソースバッファの設定 */
	if (IS_MEMFILE(GFS_DIR_ATR(dirrec))) {
	    /* メモリファイルの場合 */
	    GFMF_Setup(&GFS_FILE_FLOW(gfs), dirrec, fid);
	    ret = TRUE;
	} else if (IS_SCSIFILE(GFS_DIR_ATR(dirrec))) {
	    /* SCSIファイルの場合 */
	    ret = GFSF_Setup(&GFS_FILE_FLOW(gfs), dirrec, fid);
	} else {
	    /* ＣＤファイルの場合   */
	    ret = GFCF_Setup(&GFS_FILE_FLOW(gfs), dirrec, fid);
	}
    } else {
	/* CDブロックファイルシステム使用 */
	ret = GFCF_Setup(&GFS_FILE_FLOW(gfs), dirrec, fid);
    }
#else
    ret = GFCF_Setup(&GFS_FILE_FLOW(gfs), dirrec, fid);
#endif /* defined(DEBUG_LIB) */
    if (ret == FALSE) {
	return NULL;
    }
    GFS_FILE_AMODE(gfs) = GFS_NWSTAT_NOACT; /* アクセスモード */
    GFS_FILE_ASTAT(gfs) = GFS_SVR_COMPLETED; /* 処理フラグ */
    return gfs;
}


/*--- ディレクトリサブ関数 -------------------------------------------------*/

/*****************************************************************************
 *  ボリュームディスクリプタの読み込み
 *［入力］
 *  buf		読み込み領域
 *［出力］
 *  
 *［関数値］
 *  
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
GFS_LOCAL Sint32 gfs_loadVol(Uint8 *buf)
{
    GfsDirId	dirrec;
    GfsHn	gfs;
    Sint32	ret;
    GfsErrStat	stat;
    static const Sint8 pvd_id[] = {1, 'C', 'D', '0', '0', '1'};

    switch (GFCF_GetStat(NULL, NULL)) {
    case GFS_STAT_NODISC:
	/* ディスクがセットされていない場合 */
	return GFS_ERR_CDNODISC;
    case GFS_STAT_ERROR:
	return GFS_ERR_CDRD;
    case GFS_STAT_FATAL:
	return GFS_ERR_FATAL;
    }
    GFS_DIR_FAD(&dirrec) = PVD_FAD + GFCD_GetBaseFad();
    /* ボリュームディスクプリタをファイルとして開く */
    GFS_DIR_SIZE(&dirrec) = PVD_BYTSIZ; /* サイズ設定 */
    GFS_DIR_FN(&dirrec) = 0;
    GFS_DIR_ATR(&dirrec) = 0;
    GFS_DIR_UNIT(&dirrec) = 0;
    GFS_DIR_GAP(&dirrec) = 0;
    gfs = GFS_mngAllocGrp();
    gfs = GFS_OpenSub(gfs, &dirrec, 0);
    if (gfs != NULL) {
	ret = GFS_Fread(gfs, PVD_SCTSIZ, buf, PVD_BYTSIZ);
	if (ret != PVD_BYTSIZ) {
	    return GFS_ERR_CDRD;
	}
	GFS_GetErrStat(&stat);
	ret = GFS_ERR_CODE(&stat);
    }
    if (ret == GFS_ERR_OK) {
	/* 規格識別子のチェック */
	if (memcmp(buf, pvd_id, sizeof(pvd_id))) {
	    ret = GFS_ERR_CDROM;
	}
    }
    GFS_Close(gfs);
    return ret;
}


/*****************************************************************************
 *  ルートディレクトリ情報取得
 *［入力］
 *  dir		ルートディレクトリ格納領域
 *［出力］
 *  なし
 *［関数値］
 *  エラーコード
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
GFS_LOCAL Sint32 gfs_getRootDir(GfsDirId *dir)
{
    Sint32	ret;
    Uint8	*rec;			/* ディレクトリレコード領域 */

    if (!MNG_CDCON(gfs_mng_ptr)) {
	return GFS_ERR_CDNODISC;
    }
    /* ボリューム記述子読み込み */
    ret = gfs_loadVol(MNG_SECTBUF(gfs_mng_ptr));
    if (ret != GFS_ERR_OK) {
	return ret;
    }
    rec = MNG_SECTBUF(gfs_mng_ptr) + ROOT_RECPOS;
    memcpy(&GFS_DIR_FAD(dir), DIRREC_EXT(rec), sizeof(Sint32)); /* FAD設定 */
    LSN_TO_FAD(GFS_DIR_FAD(dir));
    if (GFS_DIR_FAD(dir) < ROOT_FAD_LOW_LIM) {
	return GFS_ERR_CDROM;
    }
    /* サイズ取得 */
    memcpy(&GFS_DIR_SIZE(dir), DIRREC_LEN(rec), sizeof(Sint32));
    if (GFS_DIR_SIZE(dir) <= 0) {
	return GFS_ERR_CDROM;
    }
    GFS_DIR_FN(dir) = 0;
    GFS_DIR_ATR(dir) = GFS_ATR_DIR;
    GFS_DIR_UNIT(dir) = 0;
    GFS_DIR_GAP(dir) = 0;
    return GFS_ERR_OK;
}


/*****************************************************************************
 *  ルートディレクトリレコードの読み込み
 *［入力］
 *  dirtbl	ディレクトリレコード設定領域
 *［出力］
 *  
 *［関数値］
 *  
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
GFS_LOCAL Sint32 gfs_loadRootDir(GfsDirTbl *dirtbl)
{
    GfsHn	gfs;
    Sint32	ndir, ret;
    GfsDirId	dir;

    ret = gfs_getRootDir(&dir);
    if (ret != GFS_ERR_OK) {
	gfs = NULL;
    } else {
	gfs = GFS_mngAllocGrp();
	gfs = GFS_OpenSub(gfs, &dir, 0);
    }
    /* CDが接続されているのにオープンできなかったらエラー */
    if (MNG_CDCON(gfs_mng_ptr) && (gfs == NULL)) {
	return ret;
    }
    /* ディレクトリレコード読み込み */
    ndir = GFDR_SetupDirTbl(gfs, dirtbl, GFS_DIR_SIZE(&dir));
    if (gfs != NULL) {
	GFS_Close(gfs);
    }
    bobtest = ndir;
    return ndir;
}


/*--- ファイルアクセス -----------------------------------------------------*/

/*****************************************************************************
 *  データをソースバッファに流し込む
 *［入力］
 *  gfs		ファイルハンドル
 *［出力］
 *  なし
 *［関数値］
 *  読み込み状態 GFS_FIN_START, GFS_FIN_PAUSE, GFS_FIN_END
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
GFS_LOCAL Sint32 gfs_flowIn(GfsHn gfs)
{
    Sint32	fstat;
    GfsFileFunc	*func;
    GfsFlow	*flow;

    flow = &GFS_FILE_FLOW(gfs);
    /* 終了していたら何もしない */
    if (GFS_FLW_STAT(flow) == GFS_FIN_END) {
	return GFS_FLW_STAT(flow);
    }
    /* ファイルハンドルに使われていないことを確認 */
    if (MNG_PICKUP(gfs_mng_ptr) != NULL && MNG_PICKUP(gfs_mng_ptr) != gfs) {
	return GFS_FIN_PUINUSE;
    }
    /* 読み込み関数の実行 */
    func = &MNG_FUNCTBL(gfs_mng_ptr, GFS_DTS_FTYPE(&GFS_FLW_DTSRC(flow)));
    fstat = (FUNC_FLOWIN(func))(flow);
    if (fstat == GFS_FIN_START) {	/* 読み込み開始した場合 */
	MNG_PICKUP(gfs_mng_ptr) = gfs;
    } else if (fstat == GFS_FIN_END) {	/* 全ての読み込みを終了した場合	*/
	if (MNG_PICKUP(gfs_mng_ptr) == gfs) {
	    MNG_PICKUP(gfs_mng_ptr) = NULL;
	    MNG_SFAD(gfs_mng_ptr) = MNG_EFAD(gfs_mng_ptr) = 0;
	}
    }
    GFS_FLW_STAT(flow) = fstat;
    return gfs_convFinStat(gfs, fstat);
}


/*****************************************************************************
 *  ユーザ領域へのデータの転送
 *［入力］
 *  gfs		ファイルハンドル
 *［出力］
 *  なし
 *［関数値］
 *  転送状態
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
GFS_LOCAL Sint32 gfs_transData(GfsHn gfs)
{
    Sint32	stat, atr, room;
    Bool	flag;
    GfsTrans	*trn = &GFS_FILE_TRANS(gfs);

    /* 他のファイルが転送中か調べる */
    if (MNG_TRANS(gfs_mng_ptr) != gfs && MNG_TRANS(gfs_mng_ptr) != NULL) {
	return GFS_SVR_BUSY;
    }
    GFS_GetFileInfo(gfs, NULL, NULL, NULL, &atr);
    if (atr & GFS_ATR_CDDA) {
	return GFTR_ST_END;
    }
    room = GFTR_GetLenRoom(&GFS_FILE_TRANS(gfs));
    if (room == 0) {
	stat = GFTR_Idle(gfs);
    } else {
	stat = GFTR_Exec(gfs, &flag);
    }
    /* 転送終了 */
    if (stat == GFTR_ST_END) {
	if (MNG_TRANS(gfs_mng_ptr) == gfs) {
	    MNG_TRANS(gfs_mng_ptr) = NULL;
	}
    } else if (MNG_TRANS(gfs_mng_ptr) == NULL) {
	MNG_TRANS(gfs_mng_ptr) = gfs;
    }
    return stat;
}




/*****************************************************************************
 *  フレームアドレスを取得する
 *［入力］
 *  fid		ファイル識別子
 *  ofs		オフセット
 *［出力］
 *  なし
 *［関数値］
 *  フレームアドレス
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFS_GetFad(Sint32 fid, Sint32 ofs)
{
    GfsDirId	dir;
    Sint32	unit;

    GFS_GetDirInfo(fid, &dir);
    unit = GFS_DIR_UNIT(&dir);
    if (unit == 0) {
	return GFS_DIR_FAD(&dir) + ofs;
    } else {
	return GFS_DIR_FAD(&dir) + (ofs / unit * (unit + GFS_DIR_GAP(&dir)));
    }
}



/* 読み込みセクタ数設定 */
GFS_LOCAL void gfs_SetSct(GfsFlow *flow, Sint32 sct)
{
    GfsDtsrc	*dtsrc = &GFS_FLW_DTSRC(flow);

    if (GFS_DTS_FTYPE(dtsrc) == CD_FILE) {
	GFCF_SetSct(flow, sct);
    } else {
	GFS_FLW_SCT(flow) = sct;
	GFS_FLW_SCTCNT(flow) = 0;
    }
    GFS_FLW_STAT(flow) = GFS_FIN_NOACT;
}


/*****************************************************************************
 *  ファイル情報の取得
 *［入力］
 *  finfo	ファイル情報
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
void gfs_GetFinfo(GfsFinfo *finfo, Sint32 *fid, Sint32 *fad,
			 Sint32 *sctsiz, Sint32 *nsct, Sint32 *lastsiz,
			 Sint32 *fno, Sint32 *atr)
{
    if (fid != NULL) {
	*fid = GFS_FI_FID(finfo);
    }
    if (fad != NULL) {
	*fad = GFS_FI_FAD(finfo);
    }
    if (sctsiz != NULL) {
	*sctsiz = GFS_FI_SCTSZ(finfo);
    }
    if (nsct != NULL) {
	*nsct = GFS_FI_NSCT(finfo);
    }
    if (lastsiz != NULL) {
	*lastsiz = GFS_FI_SCTSZ(finfo) - GFS_FI_LSTRM(finfo);
    }
    if (fno != NULL) {
	*fno = GFS_FI_FN(finfo);
    }
    if (atr != NULL) {
	*atr = GFS_FI_ATR(finfo);
    }
}


/* ファイルクローズ下請け関数 */
GFS_LOCAL void gfs_closeSub(GfsHn gfs)
{
    GfsFlow	*flow;
    GfsDtsrc	*dts;

    if (gfs == NULL) {
	return;
    }
    flow = &GFS_FILE_FLOW(gfs);
    dts = &GFS_FLW_DTSRC(flow);
    gfs_svrRemoveHndl(gfs);		/* アクセスサーバから削除 */
    gfs_mngFreeGrp(gfs);
#if defined(DEBUG_LIB)
    if (GFS_DTS_FTYPE(dts) == CD_FILE) {
#endif
	GFCB_Close(dts);
#if defined(DEBUG_LIB)
    } else if (GFS_DTS_FTYPE(dts) == SCSI_FILE) {
	/* SCSIファイルのクローズ処理 */
	GFSF_Close(dts);
    }
#endif
}    



/* 取り出しモードの取得 */
GFS_LOCAL Sint32 gfs_getGmode(GfsHn gfs)
{
    GfsFlow	*flow = &GFS_FILE_FLOW(gfs);

    return GFS_FLW_GMODE(flow);
}


/* 処理が完了したかを調べる */
GFS_LOCAL Bool gfs_isTaskDone(GfsHn gfs)
{
    Sint32	atr;
    GfsFlow	*flow = &GFS_FILE_FLOW(gfs);
    GfsTrans	*tran = &GFS_FILE_TRANS(gfs);

    GFS_GetFileInfo(gfs, NULL, NULL, NULL, &atr);
    if (GFS_FLW_STAT(flow) == GFS_FIN_END &&
	((GFBF_GetNumData(gfs) == 0) ||
	 (atr & GFS_ATR_CDDA))) {
	return TRUE;
    }
    return FALSE;
}


/* flow inの状態をエラーコードに変換する */
GFS_LOCAL Sint32 gfs_convFinStat(GfsHn gfs, Sint32 fstat)
{
    switch (fstat) {
    case GFS_FIN_FATAL:
	GFS_FILE_ASTAT(gfs) = GFS_ERR_FATAL;
	return gfs_mngSetErrCode(GFS_ERR_FATAL);
    case GFS_FIN_ERROR:
	GFS_FILE_ASTAT(gfs) = GFS_ERR_CDRD;
	return gfs_mngSetErrCode(GFS_ERR_CDRD);
    case GFS_FIN_OPEN:
	GFS_FILE_ASTAT(gfs) = GFS_ERR_CDOPEN;
	return gfs_mngSetErrCode(GFS_ERR_CDOPEN);
    case GFS_FIN_NODISC:
	GFS_FILE_ASTAT(gfs) = GFS_ERR_CDNODISC;
	return gfs_mngSetErrCode(GFS_ERR_CDNODISC);
    case GFS_FIN_PUINUSE:
	GFS_FILE_ASTAT(gfs) = GFS_ERR_PUINUSE;
	return gfs_mngSetErrCode(GFS_ERR_PUINUSE);
    case GFS_FIN_PAUSE:
	/* 常駐モードでバッファフルになったらエラー */
	if (gfs_getGmode(gfs) == GFS_GMODE_RESIDENT) {
	    GFS_FILE_ASTAT(gfs) = GFS_ERR_BFUL;
	    return gfs_mngSetErrCode(GFS_ERR_BFUL);
	}
	break;
    default:
	GFS_FILE_ASTAT(gfs) = GFS_SVR_BUSY;
	break;
    }
    return GFS_ERR_OK;
}

/* end of file */
