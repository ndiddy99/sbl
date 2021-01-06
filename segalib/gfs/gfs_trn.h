/******************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:ファイルシステム
 * Module :転送モジュールヘッダ
 * File	  :gfs_trn.h
 * Date   :1994-11-11
 * Version:1.16
 * Auther :M.S.
 *
 *************************************************************************** */

#if !defined(GFS_TRN_H)
#define GFS_TRN_H

/*****************************************************************************
 *	インクルードファイル
 *****************************************************************************/

/*****************************************************************************
 *	定数マクロ
 *****************************************************************************/
/* RtnPkの引数 */
#define GFTR_ALLSCT	-1	/* GetSrcPackで要求したセクタ数APを進める */

#define GFTR_NULLFUNC	((GfsTransFunc)0)

/*****************************************************************************
 *	列挙定数
 *****************************************************************************/
/* GFTR_Exec の処理状態 */
enum {
    GFTR_ST_NOACT = 0,			/* 停止状態	*/
    GFTR_ST_STAT1,			/* 状態1	*/
    GFTR_ST_STAT2,			/* 状態2	*/
    GFTR_ST_STAT3,			/* 状態3	*/
    GFTR_ST_STAT4,			/* 状態4	*/
    GFTR_ST_STAT5,			/* 状態5	*/
    GFTR_ST_END,			/* 転送終了状態	*/
    GFTR_ST_ERROR			/* エラー状態 */
    };


/*****************************************************************************
 *	変数の宣言
 *****************************************************************************/

/*****************************************************************************
 *	関数の宣言
 *****************************************************************************/
/* 転送器の設定  */
GftrHn  GFTR_Setup(GfsTrans *gftr);

/* 転送モードの設定 */
Sint32 GFTR_SetMode(GftrHn gftr, Sint32 tmode);

/* 転送単位の設定 */
void GFTR_SetUnit(GftrHn gftr, Sint32 unit);

/* 転送セクタ数の設定 */
void GFTR_SetTransPara(GftrHn gftr, Sint32 tsct);

/* 全転送セクタ数の取得 */
Sint32 GFTR_GetTransPara(GftrHn gftr);

/* 現在までに転送されたセクタ数の取得 */
Sint32 GFTR_GetNumTrans(GftrHn gftr);

/* １回の転送における最大転送セクタ数の設定 */
Sint32 GFTR_SetTransMax(GftrHn gftr, Sint32 tmax);

/* 転送開始 */
void GFTR_Start(GftrHn gftr);

/* 転送一時停止 */
void GFTR_Pause(GftrHn gftr);

/* 転送中止 */
void GFTR_Stop(GfsHn gfs, Sint32 nsct);

/* 転送の実行 */
Sint32 GFTR_Exec(GfsHn gfs, Bool *flag);

/* ホスト側バッファフル後の空送り転送 */
Sint32 GFTR_Idle(GfsHn gfs);

/* 転送したバイト数を返す */
Sint32 GFTR_GetLenData(GfsTrans *trn);

/* メモリバッファの空き容量を求める */
Sint32 GFTR_GetLenRoom(GfsTrans *trn);

/* 転送先データパックの取得 */
GfdpHn GFTR_GetDstPack(GfsTrans *trn, Sint32 len);

/* 転送先データパック返還 */
void GFTR_RtnDstPk(GfsTrans *trn, GfdpHn dpk);

/* 転送領域情報取得 */
void GFTR_GetData(GfsTrans *trn, Uint8 **data, Sint32 *bsize, Sint32 *unit);

/* データ領域の設定 */
void GFTR_SetData(GfsTrans *trn, Uint8 *data, Sint32 bsize, Sint32 unit);

/* ユーザ転送関数の登録 */
void GFTR_SetTransFunc(GfsHn gfs, GfsTransFunc func, void *obj);

#endif      /* !defined(GFS_TRN_H) */

/* end of file */
