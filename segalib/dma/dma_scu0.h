/*-----------------------------------------------------------------------------
 *  FILE: dma_scu0.h
 *
 *      Copyright(c) 1994 SEGA.
 *
 *  PURPOSE:
 *
 *      SCU DMA低水準ヘッダファイル。
 *
 *  DESCRIPTION:
 *
 *      SCU DMA低水準転送機能。
 *
 *  AUTHOR(S)
 *
 *      1994-05-09  N.T Ver.0.90
 *
 *  MOD HISTORY:
 *
 *      1994-10-12  N.T Ver.1.01
 *
 *-----------------------------------------------------------------------------
 */

#ifndef DMA_SCU0_H
#define DMA_SCU0_H

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
#include "sega_xpt.h"

/*
 * GLOBAL DEFINES/MACROS DEFINES
 */

/* 定数 */
/*****************************************************************************/
/**** DMAチャネル ************************************************************/
/*****************************************************************************/
#define DMA_SCU_CH0 0x0                         /* チャネル0                 */
#define DMA_SCU_CH1 0x1                         /* チャネル1                 */
#define DMA_SCU_CH2 0x2                         /* チャネル2                 */

/**** DMAレジスタビット位置 **************************************************/
/******** 加算値レジスタ *****************************************************/
#define DMA_DXAD_DXRAD  8                       /* 読み込みアドレス加算値    */
#define DMA_DXAD_DXWAD  0                       /* 書き込みアドレス加算値    */
/******** DMA許可レジスタ ****************************************************/
#define DMA_DXEN_DXGO   0                       /* DMA起動ビット             */
#define DMA_DXEN_DXEN   8                       /* DMA許可ビット             */
/******** DMAモード、アドレス更新、起動要因レジスタ **************************/
#define DMA_DXMD_DXMOD  24                      /* モードビット              */
#define DMA_DXMD_DXRUP  16                      /* 読み込みアドレス更新ビット*/
#define DMA_DXMD_DXWUP   8                      /* 書き出しアドレス更新ビット*/
#define DMA_DXMD_DXFT    0                      /* 起動要因選択ビット        */


/*****************************************************************************/
/**** (DmaScuPrm)転送パラメータ **********************************************/
/*****************************************************************************/
/******** (dxad_r)読み込みアドレス加算値 *************************************/
#define DMA_SCU_R0  (0x0 << DMA_DXAD_DXRAD)     /* 加算しない                */
#define DMA_SCU_R4  (0x1 << DMA_DXAD_DXRAD)     /* 4バイト加算する           */
/******** (dxad_w)書き込みアドレス加算値 *************************************/
#define DMA_SCU_W0     (0x0 << DMA_DXAD_DXWAD)  /* 加算しない                */
#define DMA_SCU_W2     (0x1 << DMA_DXAD_DXWAD)  /* 2バイト加算する           */
#define DMA_SCU_W4     (0x2 << DMA_DXAD_DXWAD)  /* 4バイト加算する           */
#define DMA_SCU_W8     (0x3 << DMA_DXAD_DXWAD)  /* 8バイト加算する           */
#define DMA_SCU_W16    (0x4 << DMA_DXAD_DXWAD)  /* 16バイト加算する          */
#define DMA_SCU_W32    (0x5 << DMA_DXAD_DXWAD)  /* 32バイト加算する          */
#define DMA_SCU_W64    (0x6 << DMA_DXAD_DXWAD)  /* 64バイト加算する          */
#define DMA_SCU_W128   (0x7 << DMA_DXAD_DXWAD)  /* 128バイト加算する         */
/******** (dxmod)モードビット ************************************************/
#define DMA_SCU_DIR    (0x0 << DMA_DXMD_DXMOD)  /* 直接モード                */
#define DMA_SCU_IN_DIR (0x1 << DMA_DXMD_DXMOD)  /* 間接モード                */
/******** (dxrup)読み込みアドレス更新ビット **********************************/
/******** (dxwup)書き出しアドレス更新ビット **********************************/
#define DMA_SCU_KEEP    0x0                     /* 保持する                  */
#define DMA_SCU_REN     0x1                     /* 更新する                  */

/******** (dxft)起動要因選択ビット *******************************************/
#define DMA_SCU_F_VBLK_IN  (0x0 << DMA_DXMD_DXFT)/* V-ブランク-IN            */
#define DMA_SCU_F_VBLK_OUT (0x1 << DMA_DXMD_DXFT)/* V-ブランク-OUT           */
#define DMA_SCU_F_HBLK_IN  (0x2 << DMA_DXMD_DXFT)/* H-ブランク-IN            */
#define DMA_SCU_F_TIM0     (0x3 << DMA_DXMD_DXFT)/* タイマ0                  */
#define DMA_SCU_F_TIM1     (0x4 << DMA_DXMD_DXFT)/* タイマ1                  */
#define DMA_SCU_F_SND      (0x5 << DMA_DXMD_DXFT)/* サウンド-Req             */
#define DMA_SCU_F_SPR      (0x6 << DMA_DXMD_DXFT)/* スプライト描画           */
#define DMA_SCU_F_DMA      (0x7 << DMA_DXMD_DXFT)/* DMA起動要因ビットのセット*/
/******** (msk)マスクビット *************************************************/
#define DMA_SCU_M_DXR      (1 << 0)             /* 読み込みアドレス          */
#define DMA_SCU_M_DXW      (1 << 1)             /* 書き出しアドレス          */

/*****************************************************************************/
/**** (DmaScuStatus)ステータス ***********************************************/
/*****************************************************************************/
/******** (dxbkr)DMA動作中フラグ *********************************************/
/******** (ddmv)DSP側DMA動作中フラグ *****************************************/
#define DMA_SCU_MV      0x1                     /* 動作中                    */
#define DMA_SCU_NO_MV   0x0                     /* 動作中でない              */

/*
 * STRUCTURE DECLARATIONS
 */

/*
 * TYPEDEFS
 */
typedef struct {
    Uint32 dxr;                                 /* 読み込みアドレス          */
    Uint32 dxw;                                 /* 書き出しアドレス          */
    Uint32 dxc;                                 /* 転送バイト数              */
    Uint32 dxad_r;                              /* 読み込みアドレス加算値    */
    Uint32 dxad_w;                              /* 書き込みアドレス加算値    */
    Uint32 dxmod;                               /* モードビット              */
    Uint32 dxrup;                               /* 読み込みアドレス更新ビット*/
    Uint32 dxwup;                               /* 書き込みアドレス更新ビット*/
    Uint32 dxft;                                /* 起動要因選択ビット        */
    Uint32 msk;                                 /* マスクビット              */
}DmaScuPrm;                                     /* 転送パラメータ            */

typedef struct {
    Uint32 dxmv;                            /* DMA動作中フラグ               */
}DmaScuStatus;                              /* ステータス                    */

/*
 * EXTERNAL VARIABLE DECLARATIONS
 */

/*
 * EXTERNAL FUNCTION PROTOTYPE  DECLARATIONS
 */
void DMA_ScuSetPrm(DmaScuPrm *, Uint32);    /* DMA転送パラメータ             */
void DMA_ScuStart(Uint32);                  /* DMA転送開始                   */
void DMA_ScuGetStatus(DmaScuStatus *, Uint32);/* DMAステータス取得           */

#endif  /* ifndef DMA_SCU0_H */
