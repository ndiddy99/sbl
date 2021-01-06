/*****************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library: ストリームシステム
 * Module : ローカル関数用ヘッダファイル
 * File	  : stm_loc.h
 * Date   : 1994-11-11
 * Version: 1.16
 * Auther : H.T
 *
 *****************************************************************************/
#if	!defined(STM_LOC_H)
#define	STM_LOC_H	1

/*****************************************************************************
 *	列挙定数
 *****************************************************************************/

/* 絞りモード			*/
enum FiltMode {
    FMODE_FN	= 0x01,			/* ファイル番号選択有効		*/
    FMODE_CN	= 0x02,			/* チャネル番号選択有効		*/
    FMODE_SM	= 0x04,			/* サブモード選択有効		*/
    FMODE_CI	= 0x08,			/* CI選択有効			*/
    FMODE_REVSH	= 0x10,			/* サブヘッダ選択条件反転	*/
    FMODE_FAD	= 0x40,			/* セクタ範囲有効		*/
    FMODE_INIT	= 0x80,			/* 絞り条件初期化		*/
    FMODE_END
};


/*****************************************************************************
 *	関数の宣言
 *****************************************************************************/

/* 現在のFADの取得		*/
Sint32	STL_GetCurFad(void);

/* 停止待ち			*/
void	STL_WaitStop(void);

/* 再生範囲の取得		*/
void	STL_GetPlayArea(GfsFlow *flow, Sint32 *sfad, Sint32 *efad);

/* ストリームグループ再生	*/
Sint32	STL_FlowIn(StmGrpHn grp);

/* 読み込み停止			*/
Sint32	STL_StopIn(StmGrpHn grp);

/* リードエラー発生時のリカバリ	*/
Sint32	STL_Recover(StmGrpHn grp);

/* ピックアップの移動(再生継続)	*/
Sint32	STL_MovePlaypos(StmGrpHn grp, Sint32 fad);

/* 再生終了位置の変更		*/
Sint32	STL_ChgPlayEnd(StmGrpHn grp, Sint32 efad);

/* 読み込みセクタ情報の取得	*/
Bool	STL_GetSctInfo(GfsFlow *flow, Sint32 sn, StmSct *sinfo);

/* セクタの実データサイズの取得	*/
Sint32	STL_GetActSiz(GfsFlow *flow, Sint32 spos);

/* CDバッファデータの消去	*/
void	STL_DeleteSctData(GfsFlow *flow, Sint32 spos, Sint32 snum);

/* CSCT関数用CDバッファ使用セクタ数の取得	*/
Sint32	STL_CsctGetSctNum(GfsFlow *flow);

/* CSCT関数用CDバッファ空きサイズの取得	*/
Sint32	STL_CsctGetFreeSctnum(void);

/* CSCTビットが立つのを待つ	*/
void	STL_WaitCsct(void);

/* end of stm_loc.h	*/

#endif

