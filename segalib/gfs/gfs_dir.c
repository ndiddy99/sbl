/*****************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:ファイルシステム
 * Module :ディレクトリ操作
 * File	  :gfs_dir.c
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
#include "gfs_def.h"
#include "gfs_mmc.h"
#include "gfs_dir.h"
#include "gfs_cdc.h"
#if defined(DEBUG_LIB)
#include "gfs_scsi.h"
#endif

/*****************************************************************************
 *	定数マクロ
 *****************************************************************************/
#define SECT_BUF_SIZ    2048		/* セクタバッファの大きさ */

#define MF_OVW      0x01	/* メモリファイルがＣＤファイルを上書きした */

/* 特別扱いするディレクトリ */
#define GFDR_CURRENT_DIR_ID	0	/* カレントディレクトリのID */
#define GFDR_PARENT_DIR_ID	1	/* 親ディレクトリのID */

#define GFDR_BASE_MAX	8		/* ファイル名の長さの最大 */
#define GFDR_EXT_MAX	3		/* 拡張子の長さの最大 */

/* 簡易入力ストリームのバッファにデータが読み込まれていない */
#define GFDR_FILE_UNREAD    -1

/*****************************************************************************
 *	列挙定数
 *****************************************************************************/

/*****************************************************************************
 *	処理マクロ
 *****************************************************************************/
#define LSN2FAD(lsn)	((lsn) + 150)
#define FAD2LSN(fad)	((fad) - 150)

/* 記録日時フィールドの長さ（バイト） */
#define RECORDING_DATE_LEN	7

#define GFS_DIR_DIRREC(rec)	((rec)->dirrec)

/* padding fieldが必要か */
#define NEEDS_PADDING(n)   ((n + 1) & 1)

/*****************************************************************************
 *	データ型の定義
 *****************************************************************************/
/* CD ROM XAのディレクトリ情報のうち使用するもの */
typedef struct {
    Uint32	pos;		/* エクステントの位置 */
    Uint32	len;		/* データ長 */
    Uint8	rsize;		/* ディレクトリレコードの大きさ */
    Uint8	atr;		/* ファイルの属性 */
    Uint8	unitsize;
    Uint8	gapsize;
    Sint8	fname[GFS_FNAME_LEN + 1]; /* ファイル名(+1は終端の0) */
    Uint8	fn;		/* ファイル番号 */
} XaDirEnt;

#define XA_DIR_RSIZE(dir)	((dir)->rsize)
#define XA_DIR_POS(dir)		((dir)->pos)
#define XA_DIR_LEN(dir)		((dir)->len)
#define XA_DIR_ATR(dir)		((dir)->atr)
#define XA_DIR_USIZE(dir)	((dir)->unitsize)
#define XA_DIR_GSIZE(dir)	((dir)->gapsize)
#define XA_DIR_FNAME(dir)	((dir)->fname)
#define XA_DIR_FN(dir)		((dir)->fn)

#define XA_ATR_DIR		0x80
#define XA_ATR_INTLV		0x20
#define XA_ATR_FORM2		0x10
#define XA_ATR_FORM1		0x08
#define XA_ATR_ERR		0x0080	/* ファイル名が規格にあってない */

/* 簡易入力ストリーム構造体 */
/* GfsHnからバイト単位の入力を可能にする */
typedef struct {
    GfsHn   gfs;
    Uint8   *buf;			/* １セクタ分のバッファ */
    Sint32  rp;				/* buf の次の読む位置(単位：バイト) */
    Sint32 ap;				/* 現在のセクタ位置 */
    Sint32 nsct;			/* ディレクトリファイルの大きさ */
} GfdrFile;

#define GFDR_FILE_GFS(fp)       ((fp)->gfs)
#define GFDR_FILE_BUF(fp)       ((fp)->buf)
#define GFDR_FILE_RP(fp)        ((fp)->rp)
#define GFDR_FILE_AP(fp)	((fp)->ap)
#define GFDR_FILE_NSCT(fp)	((fp)->nsct)

/*****************************************************************************
 *	変数の宣言
 *****************************************************************************/

/*****************************************************************************
 *	関数の宣言
 *****************************************************************************/

/* ディレクトリレコードテーブルを作成する */
GFS_LOCAL Sint32 gfdr_setupDirIdTbl(GfsHn gfs, GfsDirId *rectbl, Sint32 nrec);

/* ディレクトリレコードテーブルを作成する */
GFS_LOCAL Sint32 gfdr_setupDirNameTbl(GfsHn gfs, GfsDirName *rectbl, Sint32 nrec);

/* 簡易入力ストリームの初期化 */
GFS_LOCAL void gfdr_open(GfdrFile *fp, GfsHn gfs);

/* 指定されたバイト数だけバッファに読み込む */
GFS_LOCAL Sint32 gfdr_read(GfdrFile *fp, void *buf, Uint32 size);

/* 1 byte読み込む */
GFS_LOCAL Uint8 gfdr_readByte(GfdrFile *fp, Sint32 *ec);

/* 2 byte読み込む */
GFS_LOCAL Uint16 gfdr_readWord(GfdrFile *fp, Sint32 *ec);

/* NB型の2 byte整数を読み込む */
GFS_LOCAL Uint16 gfdr_readWordNB(GfdrFile *fp, Sint32 *ec);

/* NB型の4 byte整数を読み込む */
GFS_LOCAL Uint32 gfdr_readLongNB(GfdrFile *fp, Sint32 *ec);

/* NM型の4 byte整数を読み込む */
GFS_LOCAL Uint32 gfdr_readLongNM(GfdrFile *fp, Sint32 *ec);

/* 次のディレクトリレコードをバッファに詰め込む */
GFS_LOCAL Sint32 gfdr_setNextRec(GfdrFile *fp, Sint32 *ec);

/* CDからディレクトリエントリの必要な情報を読み込む */
GFS_LOCAL Sint32 gfdr_readDirEnt(GfdrFile *fp, XaDirEnt *dir);

/* CDのディレクトリ情報設定 */
GFS_LOCAL void gfdr_setDirrecCd(CdcFile *rec, XaDirEnt *dir);

#if defined(DEBUG_LIB)
/* SIMMファイルのディレクトリ情報設定 */
GFS_LOCAL void gfdr_setDirrecMem(CdcFile *rec, Sint32 fid, Sint32 len);

/* SCSIファイルのディレクトリ情報設定 */
GFS_LOCAL void gfdr_setDirrecScsi(CdcFile *rec, Sint32 fid, Sint32 len);

/* ファイル名でSCSIファイルを探す */
GFS_LOCAL Bool gfdr_getScsiFinfo(Sint8 *fname, Sint32 *fid, Sint32 *len);

/* 指定されたfidのSCSIファイルが登録されているか調べる */
GFS_LOCAL Bool gfdr_findScsiFile(GfsDirId *rec, GfsDirId *recend, Sint32 fid);

/* ファイル名で（ファイル名つき）ディレクトリレコードテーブルから検索する */
GFS_LOCAL GfsDirName *gfdr_findFile(GfsDirName *rectbl, Sint32 n_dirent,
				 Sint8 *fname);

/* メモリファイルのRSVビットを全クリア */
GFS_LOCAL void gfdr_clearMemRsv(Sint32 n_memfile);
#endif


/*****************************************************************************
 *	変数の定義
 *****************************************************************************/

/*****************************************************************************
 *	関数の定義
 *****************************************************************************/

/*****************************************************************************
 *  ディレクトリテーブル読み込み
 *［入力］
 *  gfs		ディレクトリファイルのハンドル
 *  dirtbl	ディレクトリ情報設定領域
 *  nsct	ディレクトリファイルの大きさ（単位：セクタ）
 *［出力］
 *  
 *［関数値］
 *  読み込んだディレクトリ数
 *［機能］
 *  
 *［備考］
 *  gfs == NULL のとき、CDからはディレクトリ情報を取得しない。
 *****************************************************************************/
Sint32 GFDR_SetupDirTbl(GfsHn gfs, GfsDirTbl *dirtbl, Sint32 nsct)
{
    Sint32	ndir, atr, ec;

#if !defined(DEBUG_LIB)
    if (gfs != NULL) {
	GFS_GetFileInfo(gfs, NULL, NULL, NULL, &atr);
	if ((atr & GFS_ATR_DIR) == 0) {
	    return GFS_ERR_DIR;
	}
	ec = GFS_NwCdRead(gfs, nsct);
	if (ec != GFS_ERR_OK) {
	    return ec;
	}
    }
#endif
    if (GFS_DIRTBL_TYPE(dirtbl) == GFS_DIR_ID) {
	ndir = gfdr_setupDirIdTbl(gfs, GFS_DIRTBL_DIRID(dirtbl),
				  GFS_DIRTBL_NDIR(dirtbl));
    } else {
	ndir = gfdr_setupDirNameTbl(gfs, GFS_DIRTBL_DIRNAME(dirtbl),
				    GFS_DIRTBL_NDIR(dirtbl));
    }
    return ndir;
}



/*****************************************************************************
 * ファイル名からファイル識別子を取得する 
 *［入力］
 *  
 *［出力］
 *  
 *［関数値］
 *  ファイル識別子
 *［機能］
 *  
 *［備考］
 *  
 *****************************************************************************/
Sint32 GFDR_NameToId(GfsDirName *rectbl, Sint8 *fname, Sint32 max)
{
    Sint32	i;

    for (i = 0; i < max; ++i, ++rectbl) {
	if (XSTRNCMP(fname, GFS_DIR_FNAME(rectbl), GFS_FNAME_LEN) == 0) {
	    return i;
	}
	if (GFS_DIR_ATR(rectbl) & GFS_ATR_END_TBL) {
	    break;
	}
    }
    return -1;
}


/* ディレクトリレコードテーブルの情報を取得する */
GfsDirId *GFDR_GetInfoDirId(Sint32 fid, GfsDirId *rectbl)
{
    return &rectbl[fid];
}


/* ディレクトリレコードテーブルの情報を取得する */
GfsDirName *GFDR_GetInfoDirName(Sint32 fid, GfsDirName *rectbl)
{
    return &rectbl[fid];
}


/* ファイルの属性取得 */
Sint32 GFDR_GetAtrDirId(Sint32 fid, GfsDirId *rectbl)
{
    return GFS_DIR_ATR(&rectbl[fid]);
}


/* ファイル名取得 */
Sint32 GFDR_GetAtrDirName(Sint32 fid, GfsDirName *rectbl)
{
    return GFS_DIR_ATR(&rectbl[fid]);
}


/*****************************************************************************
 *  CDBFSでルートディレクトリに移動
 *［入力］
 *  
 *［出力］
 *  
 *［関数値］
 *  ディレクトリの個数を返す。エラーなら-1を返す。
 *［機能］
 *  CDブロックのカレントディレクトリをルートディレクトリに変更
 *［備考］
 *  終了時にディレクトリ道処理は終了している。完了復帰。
 *****************************************************************************/
Sint32 GFDR_ChgRoot(void)
{
    return GFDR_ChgDir(CDC_NUL_FID);
}


/*****************************************************************************
 *  CDBFSでカレントディレクトリ変更
 *［入力］
 *  
 *［出力］
 *  
 *［関数値］
 *  ディレクトリの個数を返す。エラーなら負のエラーコードを返す。
 *［機能］
 *  
 *［備考］
 *  終了時にディレクトリ道処理は終了している。完了復帰。
 *****************************************************************************/
Sint32 GFDR_ChgDir(Sint32 fid)
{
    Sint32 err, ndir;

    /* ChgDir発行 */
    GFCD_SetTimer(GFCD_TMOUT_COUNT);
    do {
	err = GFCD_ChgDir(fid, GFCD_SYS_SEL, &ndir);
	if (err == GFCD_ERR_INUSE) {
	    return GFS_ERR_CDBFS;	/* CDBFS使用宣言していない */
	}
	err = GFCD_ExecServer();
	if (err == GFCD_ERR_TMOUT) {
	    return GFS_ERR_TMOUT;
	} else if (err == GFCD_ERR_FATAL) {
	    return GFS_ERR_FATAL;
	}
    } while (err != GFCD_ERR_OK);
    /* ChgDirの処理待ち */
    err = GFCD_WaitServer();
    if (err == GFCD_ERR_FATAL) {
	return GFS_ERR_FATAL;
    } else if (err != GFCD_SVR_COMPLETED) {
	return GFS_ERR_CDRD;
    }
    return ndir;			/* CDBFSが保持している数 */
}

/*---------------------------------------------------------------------------
 * 非公開関数
 *---------------------------------------------------------------------------*/


/* ディレクトリレコードテーブルを作成する */
GFS_LOCAL Sint32 gfdr_setupDirIdTbl(GfsHn gfs, GfsDirId *rectbl, Sint32 nrec)
{
    GfsDirId	*r = rectbl;
    Sint32      n_dirent, ec;
    GfdrFile    fp;
    XaDirEnt    dir_ent;    /* 注目しているディレクトリエントリ */
#if defined(DEBUG_LIB)
    Sint32	n_memfile = GFMC_GetNumFile();
    Sint32	i, n_scsifile;
    Uint8       *dummy1;
    Sint32	dummy2, len;
    GfsdInfo	info;
#endif

#if defined(DEBUG_LIB)
    for (i = 0; i < n_memfile; ++i) {
	GFMC_SetRsv(i, 0);
    }
#endif
    gfdr_open(&fp, gfs);
    for (n_dirent = 0; nrec > 0; ++r, ++n_dirent, --nrec) {
        ec = gfdr_readDirEnt(&fp, &dir_ent);
	if (ec != GFS_ERR_OK) {
	    return ec;
	}
        /* ディレクトリレコードの末尾チェック */
        if (XA_DIR_RSIZE(&dir_ent) == 0) {  /* ディレクトリレコードの大きさ */
            break;
        }
#if defined(DEBUG_LIB)
        /* メモリファイルに存在するか */
        for (i = 0; i < n_memfile; ++i) {
            if (XSTRNCPY(XA_DIR_FNAME(&dir_ent), GFMC_GetFname(i),
                        GFS_FNAME_LEN) == 0) {
                GFMC_GetFileInfo(i, (void **)&dummy1, &len, &dummy2);
		gfdr_setDirrecMem(&GFS_DIR_DIRREC(r), i, len);
		break;
            }
        }
        if (i == n_memfile) {
            /* 同名のメモリファイルは存在しなかった */
	    if (gfdr_getScsiFinfo(XA_DIR_FNAME(&dir_ent), &i, &len) == TRUE) {
		gfdr_setDirrecScsi(&GFS_DIR_DIRREC(r), i, len);
	    } else {
		/* 同名のSCSIファイルも存在しなかった */
#endif
		gfdr_setDirrecCd(&GFS_DIR_DIRREC(r), &dir_ent);
#if defined(DEBUG_LIB)
	    }
        }
#endif
    }
#if defined(DEBUG_LIB)
    /* メモリファイル上の未登録レコードを登録 */
    for (i = 0; i < n_memfile && nrec > 0; ++i) {
        if (GFMC_GetRsv(i) != MF_OVW) {
            --nrec;
            GFMC_GetFileInfo(i, (void **)&dummy1, &len, &dummy2);
	    gfdr_setDirrecMem(&GFS_DIR_DIRREC(r), i, len);
            ++r;
	    ++n_dirent;
        }
    }
    gfdr_clearMemRsv(n_memfile);
    if (MNG_SCSICON(gfs_mng_ptr)) {
	/* 未登録のSCSIファイルレコードを登録 */
	GFSD_GetFileNum(&n_scsifile);
	for (i = 0; i < n_scsifile && nrec > 0; ++i) {
	    if (gfdr_findScsiFile(rectbl, r, i) != TRUE) {
		GFSD_GetFileInfo(i, &info);
		gfdr_setDirrecScsi(&GFS_DIR_DIRREC(r), i, GFSD_INFO_LENGTH(&info));
	    }
	    ++r;
	    ++n_dirent;
	}
    }
#endif
    --r;
    GFS_DIR_ATR(r) |= GFS_ATR_END_TBL;
    return n_dirent;
}


/* ディレクトリレコードテーブルを作成する */
GFS_LOCAL Sint32 gfdr_setupDirNameTbl(GfsHn gfs, GfsDirName *rectbl, Sint32 nrec)
{
    GfsDirName	*r = rectbl;
    Sint32      n_dirent, ec;
    GfdrFile   fp;
    XaDirEnt    dir_ent;    /* 注目しているディレクトリエントリ */
#ifdef DEBUG_LIB
    Sint32	n_memfile;
    Uint8       *dummy1;
    Sint32	dummy2, len, i, n_scsifile, n_new_scsi, n_new_memf;
    GfsdInfo	info;
#endif

    /* CD上のディレクトリレコードを読み込む */
    gfdr_open(&fp, gfs);
    for (n_dirent = 0; nrec > 0; ++r, ++n_dirent, --nrec) {
        ec = gfdr_readDirEnt(&fp, &dir_ent);
	if (ec != GFS_ERR_OK) {
	    return ec;
	}
        /* ディレクトリレコードの末尾チェック */
        if (XA_DIR_RSIZE(&dir_ent) == 0) {  /* ディレクトリレコードの大きさ */
            break;
        }
	/* 前のファイル名と同じならディレクトリレコードを捨てる */
	if (n_dirent > 2 &&
	    XSTRNCMP(GFS_DIR_FNAME(&r[-1]),
		    XA_DIR_FNAME(&dir_ent), GFS_FNAME_LEN) == 0) {
	    continue;
	}
	gfdr_setDirrecCd(&GFS_DIR_DIRREC(r), &dir_ent);
	XSTRNCPY(GFS_DIR_FNAME(r), XA_DIR_FNAME(&dir_ent), GFS_FNAME_LEN);
    }
#ifdef DEBUG_LIB
    if (MNG_SCSICON(gfs_mng_ptr)) {
	/* SCSIファイルのディレクトリレコードを上書きする */
	GFSD_GetFileNum(&n_scsifile);
	n_new_scsi = 0;
	for (i = 0; i < n_scsifile && n_new_scsi < nrec; ++i) {
	    GFSD_GetFileInfo(i, &info);
	    r = gfdr_findFile(rectbl, n_dirent, GFSD_INFO_FNAME(&info));
	    if (r == NULL) {
		/* 見つからなければ末尾に追加 */
		r = &rectbl[n_dirent + n_new_scsi];
		gfdr_setDirrecScsi(&GFS_DIR_DIRREC(r), i, GFSD_INFO_LENGTH(&info));
		XSTRNCPY(GFS_DIR_FNAME(r), GFSD_INFO_FNAME(&info), GFS_FNAME_LEN);
		++n_new_scsi;
	    } else {
		gfdr_setDirrecScsi(&GFS_DIR_DIRREC(r), i, GFSD_INFO_LENGTH(&info));
	    }
	}
	n_dirent += n_new_scsi;
    }
    /* メモリファイルのディレクトリレコードを上書きする */
    n_memfile = GFMC_GetNumFile();
    n_new_memf = 0;
    /* メモリファイル上の未登録レコードを登録 */
    for (i = 0; i < n_memfile && n_new_memf < nrec; ++i) {
	r = gfdr_findFile(rectbl, n_dirent, GFMC_GetFname(i));
	GFMC_GetFileInfo(i, (void **)&dummy1, &len, &dummy2);
	if (r == NULL) {
	    /* 見つからなければ末尾に追加 */
	    r = &rectbl[n_dirent + n_new_memf];
	    gfdr_setDirrecMem(&GFS_DIR_DIRREC(r), i, len);
            XSTRNCPY(GFS_DIR_FNAME(r), GFMC_GetFname(i), GFS_FNAME_LEN);
	    ++n_new_memf;
	} else {
	    gfdr_setDirrecMem(&GFS_DIR_DIRREC(r), i, len);
	}
    }
    gfdr_clearMemRsv(n_memfile);
    n_dirent += n_new_memf;
    r = &rectbl[n_dirent];
#endif
    --r;
    GFS_DIR_ATR(r) |= GFS_ATR_END_TBL;
    return n_dirent;
}


/* 簡易入力ストリームの初期化 */
GFS_LOCAL void gfdr_open(GfdrFile *fp, GfsHn gfs)
{
    GfsFinfo	*finfo = &GFS_FLW_FINFO(&GFS_FILE_FLOW(gfs));

    GFDR_FILE_GFS(fp) = gfs;
    GFDR_FILE_BUF(fp) = MNG_SECTBUF(gfs_mng_ptr);
    GFDR_FILE_RP(fp) = GFDR_FILE_UNREAD;
    GFDR_FILE_NSCT(fp) = GFS_FI_NSCT(finfo);
    GFDR_FILE_AP(fp) = 0;
}


/* 指定されたバイト数だけバッファに読み込む */
/* エラーコードを返す */
GFS_LOCAL Sint32 gfdr_read(GfdrFile *fp, void *buf, Uint32 size)
{
    Sint32  i, ret;

    /* メモリファイルからだけ読む時 */
    if (GFDR_FILE_GFS(fp) == NULL) {
	memset(buf, 0, size);
	return GFS_ERR_OK;
    }
    if ((GFDR_FILE_RP(fp) == GFDR_FILE_UNREAD) ||
        (GFDR_FILE_RP(fp) >= SECT_BUF_SIZ)) {
        ret = GFS_Fread(GFDR_FILE_GFS(fp), 1, GFDR_FILE_BUF(fp), SECT_BUF_SIZ);
	if (ret != SECT_BUF_SIZ) {
	    if (ret < GFS_ERR_OK) {
		return ret;
	    }
	    return GFS_ERR_CDRD;
	}
        GFDR_FILE_RP(fp) = 0;
    }
    /* 読み込むデータのうちバッファ上にあるものの大きさ */
    i = SECT_BUF_SIZ - GFDR_FILE_RP(fp);
    i = MIN(i, size);
    memcpy(buf, GFDR_FILE_BUF(fp) + GFDR_FILE_RP(fp), i);
    GFDR_FILE_RP(fp) += i;
    if (i < size) {
        size -= i;
        ret = GFS_Fread(GFDR_FILE_GFS(fp), 1, GFDR_FILE_BUF(fp), SECT_BUF_SIZ);
	if (ret != SECT_BUF_SIZ) {
	    if (ret < GFS_ERR_OK) {
		return ret;
	    }
	    return GFS_ERR_CDRD;
	}
        memcpy((Uint8 *)buf + i, GFDR_FILE_BUF(fp), size);
        GFDR_FILE_RP(fp) = size;
    }
    return GFS_ERR_OK;
}


/* 1 byte読み込む */
GFS_LOCAL Uint8 gfdr_readByte(GfdrFile *fp, Sint32 *ec)
{
    Uint8   tmp;

    *ec = gfdr_read(fp, &tmp, sizeof(tmp));
    return tmp;
}


/* 2 byte読み込む */
GFS_LOCAL Uint16 gfdr_readWord(GfdrFile *fp, Sint32 *ec)
{
    Uint16  tmp;

    *ec = gfdr_read(fp, &tmp, sizeof(tmp));
    return tmp;
}


/* NB型の2 byte整数を読み込む */
GFS_LOCAL Uint16 gfdr_readWordNB(GfdrFile *fp, Sint32 *ec)
{
    Uint16  tmp, dummy;

    /* LSBF表記部分の読み捨て */
    *ec = gfdr_read(fp, &dummy, sizeof(dummy));
    if (*ec != GFS_ERR_OK) {
	return 0;
    }
    /* MSBF表記部分の読み込み */
    *ec = gfdr_read(fp, &tmp, sizeof(tmp));
    if (*ec != GFS_ERR_OK) {
	return 0;
    }
    return tmp;
}


/* NB型の4 byte整数を読み込む */
GFS_LOCAL Uint32 gfdr_readLongNB(GfdrFile *fp, Sint32 *ec)
{
    Uint32  tmp, dummy;

    /* LSBF表記部分の読み捨て */
    *ec = gfdr_read(fp, &dummy, sizeof(dummy));
    if (*ec != GFS_ERR_OK) {
	return 0;
    }
    /* MSBF表記部分の読み込み */
    *ec = gfdr_read(fp, &tmp, sizeof(tmp));
    if (*ec != GFS_ERR_OK) {
	return 0;
    }
    return tmp;
}


/* NM型の4 byte整数を読み込む */
GFS_LOCAL Uint32 gfdr_readLongNM(GfdrFile *fp, Sint32 *ec)
{
    Uint32  tmp;

    *ec = gfdr_read(fp, &tmp, sizeof(tmp));
    return tmp;
}


/* 次のディレクトリレコードをバッファに詰め込む */
GFS_LOCAL Sint32 gfdr_setNextRec(GfdrFile *fp, Sint32 *ec)
{
    Sint32 rsize, ret;

    rsize = gfdr_readByte(fp, ec);
    if (*ec != GFS_ERR_OK) {
	return 0;
    }
    if (rsize != 0) {
	return rsize;
    }

    /* 読み込めなかった場合 */
    ++GFDR_FILE_AP(fp);
    if (GFDR_FILE_AP(fp) >= GFDR_FILE_NSCT(fp)) {
	return rsize;
    }
    ret = GFS_Fread(GFDR_FILE_GFS(fp), 1, GFDR_FILE_BUF(fp), SECT_BUF_SIZ);
    if (ret != SECT_BUF_SIZ) {
	if (ret > 0) {
	    *ec = GFS_ERR_CDRD;
	} else {
	    *ec = ret;
	}
	return 0;
    }
    GFDR_FILE_RP(fp) = 0;
    rsize = gfdr_readByte(fp, ec);
    if (*ec != GFS_ERR_OK) {
	return 0;
    }
    return rsize;
}


/* CDからディレクトリエントリの必要な情報を読み込む */
GFS_LOCAL Sint32 gfdr_readDirEnt(GfdrFile *fp, XaDirEnt *dir)
{
    Sint32	ec, i, rdsz, fn_err = FALSE;
    Uint8       fn_len, file_flg;
    Sint8	ch, *fn_buf, *ext_pos;

    if (GFDR_FILE_GFS(fp) == NULL) {
	XA_DIR_RSIZE(dir) = 0;
	return GFS_ERR_OK;
    }
    XA_DIR_RSIZE(dir) = gfdr_setNextRec(fp, &ec);
    if (ec != GFS_ERR_OK) {
	return ec;
    }
    if (XA_DIR_RSIZE(dir) == 0) {
	return GFS_ERR_OK;
    }
    gfdr_readByte(fp, &ec);		/* 拡張属性レコード長読み捨て */
    if (ec != GFS_ERR_OK) {
	return ec;
    }
    XA_DIR_POS(dir) = LSN2FAD(gfdr_readLongNB(fp, &ec));
    if (ec != GFS_ERR_OK) {
	return ec;
    }
    XA_DIR_LEN(dir) = gfdr_readLongNB(fp, &ec);
    if (ec != GFS_ERR_OK) {
	return ec;
    }
    /* 日付データ読み捨て */
    for (i =0; i < RECORDING_DATE_LEN; ++i) {
	gfdr_readByte(fp, &ec);
	if (ec != GFS_ERR_OK) {
	    return ec;
	}
    }
    file_flg = gfdr_readByte(fp, &ec);
    if (ec != GFS_ERR_OK) {
	return ec;
    }
    XA_DIR_USIZE(dir) = gfdr_readByte(fp, &ec); /* File Unit Size */
    if (ec != GFS_ERR_OK) {
	return ec;
    }
    XA_DIR_GSIZE(dir) = gfdr_readByte(fp, &ec); /* interleave gap size */
    if (ec != GFS_ERR_OK) {
	return ec;
    }
    gfdr_readWordNB(fp, &ec);		/* ボリューム順序番号読み捨て */
    if (ec != GFS_ERR_OK) {
	return ec;
    }
    fn_len = gfdr_readByte(fp, &ec);
    if (ec != GFS_ERR_OK) {
	return ec;
    }
    /* ここまで固定長 */
    rdsz = 33 + fn_len;
    fn_buf = XA_DIR_FNAME(dir);
    i = 0;
    while (i < MIN(GFDR_BASE_MAX, fn_len)) {
	/* ファイル名部分 */
	ch = gfdr_readByte(fp, &ec), ++i;
	if (ec != GFS_ERR_OK) {
	    return ec;
	}
	if (ch == ';') {
	    break;
	}
	*fn_buf++ = ch;
	if (ch == '.') {
	    ext_pos = fn_buf;		/* 拡張子の開始位置保存 */
	    break;
	}
    }
    if (ch == ';') {
	/* 拡張子なし、バージョン番号読み捨て */
	while (i < fn_len) {
	    gfdr_readByte(fp, &ec), ++i;
	    if (ec != GFS_ERR_OK) {
		return ec;
	    }
	}
    } else if (ch != '.') {
	/* 長過ぎるファイル名を読み捨て */
	while (i < fn_len) {
	    ch = gfdr_readByte(fp, &ec), ++i;
	    if (ec != GFS_ERR_OK) {
		return ec;
	    }
	    if (ch == '.') {
		*fn_buf++ = ch;
		ext_pos = fn_buf;	/* 拡張子の開始位置保存 */
		break;
	    }
	    fn_err = TRUE;
	}
    }
    while (i < fn_len) {
	/* 拡張子部分 */
	ch = gfdr_readByte(fp, &ec), ++i;
	if (ec != GFS_ERR_OK) {
	    return ec;
	}
	if (ch == ';') {
	    /* バージョン番号読み捨て */
	    while (i < fn_len) {
		gfdr_readByte(fp, &ec), ++i;
		if (ec != GFS_ERR_OK) {
		    return ec;
		}
	    }
	    break;
	}
	if (fn_buf - ext_pos > GFDR_EXT_MAX) {
	    fn_err = TRUE;
	    continue;
	}
	if (fn_buf - XA_DIR_FNAME(dir) < GFS_FNAME_LEN) {
	    *fn_buf++ = ch;
	}
    }
    if (fn_buf - XA_DIR_FNAME(dir) < GFS_FNAME_LEN) {
	*fn_buf = '\0';
    }
    /* 詰め込み０をスキップ */
    if (NEEDS_PADDING(fn_len)) {
        gfdr_readByte(fp, &ec);
	if (ec != GFS_ERR_OK) {
	    return ec;
	}
	++rdsz;
    }
    /* 特殊なファイル名の変換 */
    if (XA_DIR_FNAME(dir)[0] == GFDR_CURRENT_DIR_ID) {
	XSTRCPY(XA_DIR_FNAME(dir), ".");
    } else if (XA_DIR_FNAME(dir)[0] == GFDR_PARENT_DIR_ID) {
	XSTRCPY(XA_DIR_FNAME(dir), "..");
    }
    /* システム情報があれば読む */
    if (rdsz < XA_DIR_RSIZE(dir)) {
	gfdr_readLongNM(fp, &ec);	/* skip Owner ID */
	if (ec != GFS_ERR_OK) {
	    return ec;
	}
	XA_DIR_ATR(dir) = gfdr_readByte(fp, &ec) &
	    (GFS_ATR_DIR|GFS_ATR_CDDA|GFS_ATR_INTLV|
	     GFS_ATR_FORM2|GFS_ATR_FORM1);
	if (ec != GFS_ERR_OK) {
	    return ec;
	}
	gfdr_readByte(fp, &ec);		/* 属性下位１バイト読み捨て */
	if (ec != GFS_ERR_OK) {
	    return ec;
	}
	gfdr_readWord(fp, &ec);		/* "XA"読み */
	if (ec != GFS_ERR_OK) {
	    return ec;
	}
	XA_DIR_FN(dir) = gfdr_readByte(fp, &ec);
	if (ec != GFS_ERR_OK) {
	    return ec;
	}
	/* 残りのディレクトリレコード読み捨て */
	for (i = rdsz + 9; i < XA_DIR_RSIZE(dir); ++i) {
	    gfdr_readByte(fp, &ec);
	    if (ec != GFS_ERR_OK) {
		return ec;
	    }
	}
    } else {
	/* システム情報がないなら */
	/* ファイルフラグを見てディレクトリか判断する */
	XA_DIR_ATR(dir) = (file_flg & 2)? XA_ATR_DIR: 0;
	XA_DIR_FN(dir) = 0;		/* ファイル番号なし */
    }
    return GFS_ERR_OK;
}


/* CDのディレクトリ情報設定 */
GFS_LOCAL void gfdr_setDirrecCd(CdcFile *rec, XaDirEnt *dir)
{
    CDC_FILE_FAD(rec) = XA_DIR_POS(dir);
    CDC_FILE_SIZE(rec) = XA_DIR_LEN(dir);
    CDC_FILE_FN(rec) = XA_DIR_FN(dir);
    CDC_FILE_ATR(rec) = XA_DIR_ATR(dir);
    CDC_FILE_UNIT(rec) = XA_DIR_USIZE(dir);
    CDC_FILE_GAP(rec) = XA_DIR_GSIZE(dir);
}


#if defined(DEBUG_LIB)
/* SIMMファイルのディレクトリ情報設定 */
GFS_LOCAL void gfdr_setDirrecMem(CdcFile *rec, Sint32 fid, Sint32 len)
{
    CDC_FILE_FAD(rec) = fid;
    CDC_FILE_SIZE(rec) = len;
    CDC_FILE_FN(rec) = 0;
    CDC_FILE_ATR(rec) = GFS_ATR_MEM_FILE;
    CDC_FILE_UNIT(rec) = 0;
    CDC_FILE_GAP(rec) = 0;
    GFMC_SetRsv(fid, MF_OVW);
}


/* SCSIファイルのディレクトリ情報設定 */
GFS_LOCAL void gfdr_setDirrecScsi(CdcFile *rec, Sint32 fid, Sint32 len)
{
    CDC_FILE_FAD(rec) = fid;
    CDC_FILE_SIZE(rec) = len;
    CDC_FILE_FN(rec) = 0;
    CDC_FILE_ATR(rec) = GFS_ATR_SCSI_FILE;
    CDC_FILE_UNIT(rec) = 0;
    CDC_FILE_GAP(rec) = 0;
}


/* ファイル名でSCSIファイルを探す */
GFS_LOCAL Bool gfdr_getScsiFinfo(Sint8 *fname, Sint32 *fid, Sint32 *len)
{
    Sint32	i, n_scsi_file;
    GfsdInfo	info;

    if (MNG_SCSICON(gfs_mng_ptr)) {
	GFSD_GetFileNum(&n_scsi_file);
	for (i = 0; i < n_scsi_file; ++i) {
	    GFSD_GetFileInfo(i, &info);
	    if (XSTRNCMP(fname, GFSD_INFO_FNAME(&info), GFS_FNAME_LEN) == 0) {
		*fid = i;
		*len = GFSD_INFO_LENGTH(&info);
		return TRUE;
	    }
	}
    }
    return FALSE;
}


/* 指定されたfidのSCSIファイルが登録されているか調べる */
GFS_LOCAL Bool gfdr_findScsiFile(GfsDirId *rec, GfsDirId *recend, Sint32 fid)
{
    while (rec < recend) {
	if (GFS_DIR_ATR(rec) == GFS_ATR_SCSI_FILE && GFS_DIR_FAD(rec) == fid) {
	    return TRUE;
	}
	++rec;
    }
    return FALSE;
}


/* ファイル名で（ファイル名つき）ディレクトリレコードテーブルから検索する */
GFS_LOCAL GfsDirName *gfdr_findFile(GfsDirName *rectbl, Sint32 n_dirent,
				 Sint8 *fname)
{
    Sint32	i;

    for (i = 0; i < n_dirent; ++i) {
	if (XSTRNCMP(GFS_DIR_FNAME(rectbl), fname, GFS_FNAME_LEN) == 0) {
	    return rectbl;
	}
	++rectbl;
    }
    return NULL;
}


/* メモリファイルのRSVビットを全クリア */
GFS_LOCAL void gfdr_clearMemRsv(Sint32 n_memfile)
{
    Sint32 i;

    for (i = 0; i < n_memfile; ++i) {
	GFMC_SetRsv(i, 0);
    }
}

#endif /* defined(DEBUG_LIB) */

/* end of file */
