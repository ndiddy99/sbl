/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:ファイルシステム
 * Module :CDCとのインタフェースモジュールヘッダ
 * File	  :gfs_cdc.h
 * Date   :1994-11-11
 * Version:1.16
 * Auther :M.S.
 *
 *************************************************************************** */

/*****************************************************************************
 *	インクルードファイル
 *****************************************************************************/

/*****************************************************************************
 *	定数マクロ
 *****************************************************************************/
/* ピックアップの制御権を取得していない */
#define GFCD_UID_NONE	-1

#define GFCD_ANY_FLT	-2		/* 偽出力コネクタの設定を変更しない */

/* タイムアウト関係 */
#define GFCD_TMOUT_COUNT	0x30000	/* デフォルトタイムアウト回数 */
#define GFCD_NO_TMOUT		0	/* タイムアウト判定なし指定 */

/* エラーコード */
#define GFCD_ERR_OK		0	/* 正常終了 */
#define GFCD_ERR_WAIT		-1	/* 処理待ち */
#define GFCD_ERR_NOCDBLK	-2	/* CDブロックが接続されていない */
#define GFCD_ERR_NOFILT		-3	/* 空き絞りがない */
#define GFCD_ERR_NOBUF		-4	/* 空き区画がない */
#define GFCD_ERR_INUSE		-5	/* 指定された資源が使用中 */
#define GFCD_ERR_RANGE		-6	/* 引数が範囲外 */
#define GFCD_ERR_UNUSE		-7	/* 未確保のものを操作しようとした */
#define GFCD_ERR_QFULL		-8	/* コマンドキューがいっぱい */
#define GFCD_ERR_NOTOWNER	-9	/* 非所有者が資源を操作しようとした */
#define GFCD_ERR_CDC		-10	/* CDCからのエラー */
#define GFCD_ERR_CDBFS		-11	/* CDブロックファイルシステムエラー */
#define GFCD_ERR_TMOUT		-12	/* タイムアウト */
#define GFCD_ERR_OPEN		-13	/* トレイが開いている */
#define GFCD_ERR_NODISC		-14	/* ディスクが入っていない */
#define GFCD_ERR_CDROM		-15	/* CD-ROMでないディスクが入っている */
#define GFCD_ERR_FATAL		-16	/* ステータスがFATAL */

/* cdbfs用セレクタ番号 */
#define GFCD_SYS_SEL	(GFS_OPEN_MAX - 1)

/* GFCD_SetCsctFunc用定数 */
#define GFCD_NULLFUNC	((void (*)(void *))0)

/*****************************************************************************
 *	列挙定数
 *****************************************************************************/
/* GFCD_ExecServerの状態 */
enum GfcdExecServerStat {
    GFCD_SVR_COMPLETED,			/* 実行完了 */
    GFCD_SVR_BUSY,			/* 処理中 */
    GFCD_SVR_ERROR,			/* CDCのエラー発生 */
    GFCD_SVR_TMOUT			/* タイムアウト */
};


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

/* GFCDモジュールの初期化 */
Sint32 GFCD_Init(GfsCdbMng *work, Bool use_cdbfs);

/* バッファ区画割り当て */
Sint32 GFCD_AllocBuf(Sint32 *buf_no);

/* バッファ区画解放 */
Sint32 GFCD_FreeBuf(Sint32 buf_no);

/* 絞り割り当て */
Sint32 GFCD_AllocFilt(Sint32 *flt_no);

/* 絞り解放 */
Sint32 GFCD_FreeFilt(Sint32 flt_no);

/* 最終セッションの開始FADを取得する */
Sint32 GFCD_GetBaseFad(void);

/* ピックアップ制御権取得 */
Sint32 GFCD_GetPickup(void);

/* ピックアップ制御権解放 */
Sint32 GFCD_UngetPickup(Sint32 puid);

/* ピックアップの制御権を取得しているか調べる */
Sint32 GFCD_IsPuOwner(Sint32 puid);

/* 絞り設定 */
Sint32 GFCD_SetFilt(Sint32 flt, Sint32 fmode, CdcSubh *subh,
		    Sint32 fad, Sint32 snum);

/* 絞り取得 */
Sint32 GFCD_GetFilt(Sint32 flt, Sint32 *fmode, CdcSubh *subh);

/* CDデバイスの接続 */
Sint32 GFCD_SetCon(Sint32 flt, Sint32 puid);

/* CD再生 */
Sint32 GFCD_Play(CdcPly *pinfo, Sint32 puid);

/* 区画内データ数取得 */
Sint32 GFCD_GetLenData(Sint32 bufno, Sint32 spos, Sint32 usct, Sint32 *nsct,
		       Sint32 *nbyte);

/* データ転送要求 */
Sint32 GFCD_ReqData(Sint32 bufno, Sint32 sctpos, Sint32 nsct);

/* データ転送終了の通知 */
Sint32 GFCD_EndData(Sint32 *nsct);

/* バッファデータ消去 */
Sint32 GFCD_DelSctData(Sint32 bufno, Sint32 sctpos, Sint32 nsct);

/* バッファデータ消去チェック */
Bool GFCD_CheckDelSctData(Sint32 bufno);

/* CDステータス取得 */
Uint16 GFCD_GetStat(CdcStat *stat);

/* ピックアップの移動 */
Sint32 GFCD_MovePickup(CdcPos *pos, Sint32 puid);

/* バッファデータの移動 */
Sint32 GFCD_MoveSctData(Sint32 dst, Sint32 src, Sint32 spos, Sint32 snum);

/* 移動完了のチェック */
Bool GFCD_CheckMove(void);

/* セクタ情報の取得 */
Sint32 GFCD_GetSctInfo(Sint32 bufno, Sint32 sn, CdcSct *info);

/* ファイル情報取得 */
Sint32 GFCD_GetFileInfo(Sint32 fid, CdcFile *finfo);

/* バッファの空きセクタ数取得 */
Sint32 GFCD_GetBufSiz(void);

/* CDブロックとの通信 */
Sint32 GFCD_ExecServer(void);

/* タイムアウト設定 */
void GFCD_SetTimer(Sint32 tm_count);

/* GFCD_ExecServer終了待ち */
Sint32 GFCD_WaitServer(void);

/* カレントディレクトリの設定 */
Sint32 GFCD_ChgDir(Sint32 fid, Sint32 work, Sint32 *ndir);

/* CDBFSを使ったファイルの読み込み */
Sint32 GFCD_ReadFile(Sint32 fltno, Sint32 fid, Sint32 ofs, Sint32 puid);

/* 絞りとバッファの接続 */
Sint32 GFCD_SetFiltCon(Sint32 flt, Sint32 buf, Sint32 flnout);

/* CSCTビットが１になったら呼ぶ関数を登録 */
void GFCD_SetCsctFunc(void (*func)(void *), void *obj);

/* SCDQフラグが立つのを待つ */
Bool GFCD_WaitScdqFlag(void);

/* end of file */
