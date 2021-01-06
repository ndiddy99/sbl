/*****************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:ファイルシステム
 * Module :SIMMファイル管理
 * File	  :gfs_mmc.c
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
#include "gfs_mmc.h"

/*****************************************************************************
 *	定数マクロ
 *****************************************************************************/
#define GFMC_FNAME_LEN  12              /* ファイル名最大長 */

/*****************************************************************************
 *	列挙定数
 *****************************************************************************/

/*****************************************************************************
 *	処理マクロ
 *****************************************************************************/

/* メモリファイルヘッダ     */
typedef struct {
    Uint32      fblksiz;                /* 次のファイルヘッダまでのバイト数 */
    Sint8       fname[GFMC_FNAME_LEN];  /* ファイル名                       */
    Sint16      version;                /* バージョン                       */
    Sint16      ftype;                  /* ファイルタイプ                   */
    Sint32      filesiz;                /* サイズ                           */
    Uint32      datetime;               /* 更新日時                         */
    Sint16      sctsiz;                 /* セクタサイズ                     */
    Sint16      rsv;                    /* 予備                             */
} MmcHeader;

#define MMCH_FBLKSIZ(mmch)	DWORD(&((mmch)->fblksiz))
#define MMCH_FNAME(mmch)	((mmch)->fname)
#define MMCH_VERSION(mmch)	((mmch)->version)
#define MMCH_FTYPE(mmch)	((mmch)->ftype)
#define MMCH_FILESIZ(mmch)	((mmch)->filesiz)
#define MMCH_DATETIME(mmch)	((mmch)->datetime)
#define MMCH_SCTSIZ(mmch)	((mmch)->sctsiz)
#define MMCH_RSV(mmch)		((mmch)->rsv)

#define DWORD(x)    (*((Uint32 *)(x)))
#define WORD(x)     (*((Uint16 *)(x)))

/*****************************************************************************
 *	データ型の定義
 *****************************************************************************/

/*****************************************************************************
 *	変数の宣言
 *****************************************************************************/

/*****************************************************************************
 *	関数の宣言
 *****************************************************************************/
/*  SIMMファイルヘッダのアドレスを取得する */
GFS_LOCAL MmcHeader *gfmc_getHeaderAddr(Sint32 fid);

/*****************************************************************************
 *	変数の定義
 *****************************************************************************/

/*****************************************************************************
 *	関数の定義
 *****************************************************************************/

/* SIMMファイルイメージのロードアドレスを取得する */
void *GFMC_GetBase(void)
{
    return (void *)GFMC_base;
}



/*****************************************************************************
 *  SIMMファイル情報を取得する
 *［入力］
 *  fid		メモリファイル識別子
 *［出力］
 *  area	ファイルデータのアドレス
 *  size	ファイルサイズ
 *  sct_siz	セクタサイズ
 *［関数値］
 *  
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
void GFMC_GetFileInfo(Sint32 fid, void **area, Sint32 *size, Sint32 *sct_siz)
{
    MmcHeader   *hp;
    void	*a;
    Sint32	fsize;
    Sint32	ssize;

    hp = gfmc_getHeaderAddr(fid);
    if (hp == NULL) {                           /* 見つからない場合 */
        a = NULL;
        fsize = ssize = 0;
    } else {                                    /* 見つかった場合 */
        a = (Sint8 *)(hp + 1);
        fsize = DWORD(&(MMCH_FILESIZ(hp)));
        ssize = WORD(&(MMCH_SCTSIZ(hp)));
    }
    if (area != NULL) {
	*area = a;
    }
    if (size != NULL) {
	*size = fsize;
    }
    if (sct_siz != NULL) {
	*sct_siz = ssize;
    }
}


/* SIMMファイルの数を取得する */
Sint32 GFMC_GetNumFile(void)
{
    Sint8   *bp;
    Uint32  blksiz;
    Uint32  i;

    bp = GFMC_GetBase();
    if (bp == NULL) {
	return 0;
    }
    for (i = 0; (blksiz = MMCH_FBLKSIZ((MmcHeader *)bp)) != 0L; i++) {
        bp += blksiz;
    }
    return i;
}


/* SIMMファイル名を取得する */
Sint8 *GFMC_GetFname(Sint32 fid)
{
    MmcHeader   *hp;

    hp = gfmc_getHeaderAddr(fid);
    if (hp == NULL) {
        return NULL;
    } else {
        return MMCH_FNAME(hp);
    }
}


/* リザーブ領域を設定する */
void GFMC_SetRsv(Sint32 fid, Sint16 rsv)
{
    MmcHeader   *hp;
    
    hp = gfmc_getHeaderAddr(fid);
    if (hp != NULL) {
        MMCH_RSV(hp) = rsv;
    }
}


/* リザーブ領域を取得する */
Sint16 GFMC_GetRsv(Sint32 fid)
{
    MmcHeader   *hp;
    
    hp = gfmc_getHeaderAddr(fid);
    if (hp != NULL) {
        return MMCH_RSV(hp);
    } else {
        return MMC_ERROR;
    }
}


/*****************************************************************************
 *  SIMMファイルヘッダのアドレスを取得する
 *［入力］
 *  fid		ファイル識別子
 *［出力］
 *  
 *［関数値］
 *  ファイルヘッダのアドレス
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
GFS_LOCAL MmcHeader *gfmc_getHeaderAddr(Sint32 fid)
{
    Sint8       *bp;
    Uint32      blksiz;
    Sint32      i;
    MmcHeader   *hp;

    bp = (Sint8 *)GFMC_GetBase();
    if (bp == NULL) {
	return NULL;
    }
    for (i = 0; i < fid; i++) {
        hp = (MmcHeader *)bp;
        blksiz = MMCH_FBLKSIZ(hp);
        if (blksiz == 0L) {                 /* 見つからない場合 */
            return NULL;
        }
        bp += blksiz;
    }
    return (MmcHeader *)bp;
}

/* end of file */
