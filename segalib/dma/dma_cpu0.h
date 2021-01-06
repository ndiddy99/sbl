/*-----------------------------------------------------------------------------
 *  FILE: dma_cpu0.h
 *
 *      Copyright(c) 1994 SEGA.
 *
 *  PURPOSE:
 *
 *      CPU DMA低水準ヘッダファイル。
 *
 *  DESCRIPTION:
 *
 *      CPU DMA低水準転送機能。
 *
 *  AUTHOR(S)
 *
 *      1994-07-05  N.T Ver.1.00
 *
 *  MOD HISTORY:
 *
 *      1994-10-07  N.T Ver.1.01
 *
 *-----------------------------------------------------------------------------
 */

#ifndef DMA_CPU0_H
#define DMA_CPU0_H

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
#include "sega_xpt.h"

/*
 * GLOBAL DEFINES/MACROS DEFINES
 */

/* 定数 */
/**** DMAレジスタビット位置 **************************************************/
/******** チャネルコントロールレジスタ ***************************************/
#define DMA_CHCR_DE     0               /* DMAイネーブルビット               */
#define DMA_CHCR_TE     1               /* トランスファエンドフラグビット    */
#define DMA_CHCR_IE     2               /* インタラプトイネーブルビット      */
#define DMA_CHCR_TA     3               /* トランスファアドレスモードビット  */
#define DMA_CHCR_TB     4               /* トランスファバスモードビット      */
#define DMA_CHCR_DL     5               /* DREQレベルビット　　              */
#define DMA_CHCR_DS     6               /* DREQセレクトビット                */
#define DMA_CHCR_AL     7               /* アクノリッジレベルビット          */
#define DMA_CHCR_AM     8               /* ｱｸﾉﾘｯｼﾞ/ﾄﾗﾝｽﾌｧﾓｰﾄﾞﾋﾞｯﾄ            */
#define DMA_CHCR_AR     9               /* オートリクエストビット            */
#define DMA_CHCR_TS     10              /* トランスファサイズビット          */
#define DMA_CHCR_SM     12              /* ソースアドレスモードビット        */
#define DMA_CHCR_DM     14              /* ﾃﾞｨｽﾃｨﾈｰｼｮﾝｱﾄﾞﾚｽﾓｰﾄﾞﾋﾞｯﾄ          */
/******** DMAオペレーションレジスタ ******************************************/
#define DMA_DMAOR_DME   0               /* DMAマスタイネーブルビット         */
#define DMA_DMAOR_NMIF  1               /* NMIフラグビット                   */
#define DMA_DMAOR_AE    2               /* アドレスエラーフラグビット        */
#define DMA_DMAOR_PR    3               /* プライオリティモードビット        */

/*****************************************************************************/
/**** DMAチャネル ************************************************************/
/*****************************************************************************/
#define DMA_CPU_CH0 0                           /* チャネル0                 */
#define DMA_CPU_CH1 1                           /* チャネル1                 */

/*****************************************************************************/
/**** (DmaCpuComPrm)共通転送パラメータ ***************************************/
/*****************************************************************************/
/******** (pr)プライオリティモード  ******************************************/
#define DMA_CPU_FIX (0 << DMA_DMAOR_PR)         /* 優先順位は固定            */
#define DMA_CPU_ROR (1 << DMA_DMAOR_PR)         /* 優先順位はラウンドロビン- */
                                                /* による                    */
/******** (dme)DMAマスタイネーブル *******************************************/
#define DMA_CPU_DIS     (0 << DMA_DMAOR_DME)    /* 全チャネルのDMA転送を禁止 */
#define DMA_CPU_ENA     (1 << DMA_DMAOR_DME)    /* 全チャネルのDMA転送を許可 */
/******** (msk)マスクビット **************************************************/
#define DMA_CPU_M_PR    (1 << 0)                /* プライオリティモード　　  */
#define DMA_CPU_M_AE    (1 << 1)                /* アドレスエラーフラグ　　  */
#define DMA_CPU_M_NMIF  (1 << 2)                /* NMIフラグ           　　  */
#define DMA_CPU_M_DME   (1 << 3)                /* DMAマスタイネーブル 　　  */

/*****************************************************************************/
/**** (DmaCpuPrm)転送パラメータ **********************************************/
/*****************************************************************************/
/******** (dm)ディスティネーションアドレスモードビット ***********************/
/******** (sm)ソースアドレスモードビット *************************************/
#define DMA_CPU_AM_NOM  0x0                    /* 固定                      */
#define DMA_CPU_AM_ADD  0x1                    /* 増加                      */
#define DMA_CPU_AM_SUB  0x2                    /* 減少                      */
/******** (ts)トランスファサイズ *********************************************/
#define DMA_CPU_1   (0 << DMA_CHCR_TS)          /* バイト単位                */
#define DMA_CPU_2   (1 << DMA_CHCR_TS)          /* ワード(2バイト)単位       */
#define DMA_CPU_4   (2 << DMA_CHCR_TS)          /* ロングワード(4バイト)単位 */
#define DMA_CPU_16  (3 << DMA_CHCR_TS)          /* 16バイト転送              */
/******** (ar)オートリクエストモード *****************************************/
#define DMA_CPU_MOD     (0 << DMA_CHCR_AR)      /* モジュールリクエスト      */
#define DMA_CPU_AUTO    (1 << DMA_CHCR_AR)      /* オートリクエスト          */
/******** (ie)インタラプトイネーブル *****************************************/
#define DMA_CPU_INT_ENA (1 << DMA_CHCR_IE)      /* 割り込み要求を許可        */
#define DMA_CPU_INT_DIS (0 << DMA_CHCR_IE)      /* 割り込み要求を禁止(初期値)*/
/******** (drcr)DMA要求／応答選択制御 ****************************************/
#define DMA_CPU_DREQ    0x0                     /* DREQ(外部リクエスト)      */
#define DMA_CPU_RXI     0x1                     /* RXI(内臓SCIの受信データ-  */
                                                /* フル割り込み転送要求)     */
#define DMA_CPU_TXI     0x2                     /* TXI(内臓SCIの送信データ-  */
                                                /* エンプティ割り込み転送要求*/
/******** (msk)マスクビット **************************************************/
#define DMA_CPU_M_SAR   (1 << 0)                /* DMAソースアドレス         */
#define DMA_CPU_M_DAR   (1 << 1)                /* DMAディスティネーション-  */
                                                /* アドレス                  */
#define DMA_CPU_M_TCR   (1 << 2)                /* DMAトランスファカウント   */
#define DMA_CPU_M_DM    (1 << 3)                /* DMAディスティネーション-  */
                                                /* アドレスモードビット      */
#define DMA_CPU_M_SM    (1 << 4)                /* DMAソースアドレスモード-  */
                                                /* ビット                    */
#define DMA_CPU_M_TS    (1 << 5)                /* トランスファサイズ        */
#define DMA_CPU_M_AR    (1 << 6)                /* オートリクエストモード    */
#define DMA_CPU_M_IE    (1 << 7)                /* インタラプトイネーブル    */
#define DMA_CPU_M_DRCR  (1 << 8)                /* DMA要求／応答選択制御     */
#define DMA_CPU_M_TE    (1 << 9)                /* トランスファエンドフラグ  */

/*****************************************************************************/
/**** (DmaCpuComStatus)共通ステータス ****************************************/
/*****************************************************************************/
/******** (ae)アドレスエラーフラグ *******************************************/
#define DMA_CPU_ADR_ERR     (1 << DMA_DMAOR_AE) /* DMACによるアドレスエラー- */
                                                /* 発生                      */
#define DMA_CPU_ADR_NO_ERR  (0 << DMA_DMAOR_AE) /* DMACによるアドレスエラー- */
                                                /* なし                      */
                                                
/******** (nmif)NMIフラグ ****************************************************/
#define DMA_CPU_NMI_ON  (1 << DMA_DMAOR_NMIF)   /* NMI割り込み発生           */
#define DMA_CPU_NMI_OFF (0 << DMA_DMAOR_NMIF)   /* NMI割り込みなし           */

/*****************************************************************************/
/**** (DmaCpuComStatus)共通ステータス ****************************************/
/*****************************************************************************/
#define DMA_CPU_TE_MV   (0 << DMA_CHCR_TE)      /* DMA転送中またはDMA転送中断*/
#define DMA_CPU_TE_SP   (1 << DMA_CHCR_TE)      /* DMA転送正常終了           */

/*
 * STRUCTURE DECLARATIONS
 */

/*
 * TYPEDEFS
 */
typedef struct {
    Uint32 pr;                                  /* プライオリティモード      */
    Uint32 dme;                                 /* DMAマスタイネーブル       */
    Uint32 msk;                                 /* マスクビット              */
}DmaCpuComPrm;                                  /* 共通転送パラメータ        */

typedef struct {
    Uint32 sar;                                 /* DMAソースアドレス         */
    Uint32 dar;                                 /* DMAディスティネーション-  */
                                                /* アドレス                  */
    Uint32 tcr;                                 /* DMAトランスファカウント   */
    Uint32 dm;                                  /* ディスティネーションアド- */
                                                /* レスモードビット          */
    Uint32 sm;                                  /* ソースアドレスモードビット*/
    Uint32 ts;                                  /* トランスファサイズ        */
    Uint32 ar;                                  /* オートリクエストモード    */
    Uint32 ie;                                  /* インタラプトイネーブル    */
    Uint32 drcr;                                /* DMA要求／応答選択制御     */
    Uint32 msk;                                 /* マスクビット              */
}DmaCpuPrm;                                     /* 転送パラメータ            */

typedef struct {
    Uint32 ae;                                  /* アドレスエラーフラグ      */
    Uint32 nmif;                                /* NMIフラグ                 */
}DmaCpuComStatus;                               /* 共通ステータス            */

typedef Uint32 DmaCpuStatus;                    /* ステータス                */

/*
 * EXTERNAL VARIABLE DECLARATIONS
 */

/*
 * EXTERNAL FUNCTION PROTOTYPE  DECLARATIONS
 */
void DMA_CpuSetComPrm(DmaCpuComPrm *);          /* DMA共通転送パラメータ設定 */
void DMA_CpuSetPrm(DmaCpuPrm *, Uint32);        /* DMA転送パラメータ設定     */
void DMA_CpuStart(Uint32);                      /* DMA転送開始               */
void DMA_CpuStop(Uint32);                       /* DMA転送中止               */
void DMA_CpuAllStop(void);                      /* DMA全チャネル転送中止     */
void DMA_CpuGetComStatus(DmaCpuComStatus *);    /* DMA共通ステータス取得     */
DmaCpuStatus DMA_CpuGetStatus(Uint32);          /* DMAステータス取得         */

#endif  /* ifndef DMA_CPU0_H */
