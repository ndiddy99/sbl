/*-----------------------------------------------------------------------------
 *  FILE: dma_cpu0.c
 *
 *      Copyright(c) 1994 SEGA.
 *
 *  PURPOSE:
 *
 *      CPU DMA低水準。
 *
 *  DESCRIPTION:
 *
 *      CPU DMA低水準転送機能。
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *          (1) DMA_CpuSetComPrm        -   DMA共通転送パラメータ設定
 *          (2) DMA_CpuSetPrm           -   DMA転送パラメータ設定
 *          (3) DMA_CpuStart            -   DMA転送開始
 *          (4) DMA_CpuStop             -   DMA転送中止
 *          (5) DMA_CpuAllStop          -   DMA全チャネル転送中止
 *          (6) DMA_CpuGetComStatus     -   DMA共通ステータス取得
 *          (7) DMA_CpuGetStatus        -   DMAステータス取得
 *
 *  CAVEATS:
 *
 *
 *  AUTHOR(S)
 *
 *      1994-07-05  N.T Ver.1.00
 *
 *  MOD HISTORY:
 *
 *-----------------------------------------------------------------------------
 */

/*
 * C STANDARD LIBRARY FUNCTIONS/MACROS DEFINES
 */

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
#include "sega_xpt.h"

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include "dma_xpt.h"
#include "dma_cpu0.h"

/*
 * GLOBAL DECLARATIONS
 */

/*
 * LOCAL DEFINES/MACROS
 */

/* 定数 */
/**** DMAレジスタアドレス ****************************************************/
/******** チャネル0 **********************************************************/
#define REG_SAR     ((Uint32 *)0xffffff80)  /* ソースアドレスレジスタ        */
#define REG_DAR     ((Uint32 *)0xffffff84)  /* ディスティネーションアドレス- */
                                            /* レジスタ                      */
#define REG_TCR     ((Uint32 *)0xffffff88)  /* トランスファカウントレジスタ  */
#define REG_CHCR    ((Uint32 *)0xffffff8C)  /* チャネルコントロールレジスタ  */
#define REG_DRCR    ((Uint8  *)0xfffffe71)  /* DMA要求／応答選択制御レジスタ */
/******** チャネルオフセット *************************************************/
#define OFSET0_REG  0x00000004              /* REG_SAR   オフセット          */
                                            /* REG_DAR                       */
                                            /* REG_TCR                       */
                                            /* REG_CHCR                      */
#define OFSET1_REG  0x00000001              /* REG_DRCR  オフセット          */
/******** 共通 ***************************************************************/
#define REG_DMAOR   ((Uint32 *)0xffffffb0)  /* DMAオペレーションレジスタ     */

/***** DMAレジスタマスクビット ***********************************************/
/******** チャネルコントロールレジスタ ***************************************/
#define R_CHCR_DE    (1 << DMA_CHCR_DE)   /* DMAイネーブルビット             */
#define R_CHCR_TE    (1 << DMA_CHCR_TE)   /* トランスファエンドフラグビット  */
#define R_CHCR_IE    (1 << DMA_CHCR_IE)   /* インタラプトイネーブルビット    */
#define R_CHCR_TA    (1 << DMA_CHCR_TA)   /* トランスファアドレスモードビット*/
#define R_CHCR_TB    (1 << DMA_CHCR_TB)   /* トランスファバスモードビット    */
#define R_CHCR_DL    (1 << DMA_CHCR_DL)   /* DREQレベルビット　　            */
#define R_CHCR_DS    (1 << DMA_CHCR_DS)   /* DREQセレクトビット              */
#define R_CHCR_AL    (1 << DMA_CHCR_AL)   /* アクノリッジレベルビット        */
#define R_CHCR_AM    (1 << DMA_CHCR_AM)   /* ｱｸﾉﾘｯｼﾞ/ﾄﾗﾝｽﾌｧﾓｰﾄﾞﾋﾞｯﾄ          */
#define R_CHCR_AR    (1 << DMA_CHCR_AR)   /* オートリクエストビット          */
#define R_CHCR_TS    (3 << DMA_CHCR_TS)   /* トランスファサイズビット        */
#define R_CHCR_SM    (3 << DMA_CHCR_SM)   /* ソースアドレスモードビット      */
#define R_CHCR_DM    (3 << DMA_CHCR_DM)   /* ﾃﾞｨｽﾃｨﾈｰｼｮﾝｱﾄﾞﾚｽﾓｰﾄﾞﾋﾞｯﾄ        */
/******** DMAオペレーションレジスタ ******************************************/
#define R_DMAOR_DME  (1 << DMA_DMAOR_DME) /* DMAマスタイネーブルビット       */
#define R_DMAOR_NMIF (1 << DMA_DMAOR_NMIF)/* NMIフラグビット                 */
#define R_DMAOR_AE   (1 << DMA_DMAOR_AE)  /* アドレスエラーフラグビット      */
#define R_DMAOR_PR   (1 << DMA_DMAOR_PR)  /* プライオリティモードビット      */

/*
 * STATIC DECLARATIONS
 */

/*
 * STATIC FUNCTION PROTOTYPE DECLARATIONS
 */


/******************************************************************************
 *
 * NAME:    DMA_CpuSetComPrm()  -   DMA共通転送パラメータ設定
 *
 * PARAMETERS :
 *      (1) DmaCpuComPrm *com_prm   <i>   共通転送パラメータ.
 *
 * DESCRIPTION:
 *      指定された共通転送パラメータを設定します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし。
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */

void DMA_CpuSetComPrm(DmaCpuComPrm *com_prm)
{
    Uint32 work_reg;
    work_reg = DMA_PEEK_L(REG_DMAOR);
                                    /* DMAオペレーションレジスタの読み込み   */

    if((com_prm->msk & DMA_CPU_M_PR) == DMA_CPU_M_PR){
        work_reg &= ~R_DMAOR_PR;
        work_reg |= com_prm->pr;
                                            /* プライオリティモードセット    */
    }
    if((com_prm->msk & DMA_CPU_M_DME) == DMA_CPU_M_DME){
        work_reg &= ~R_DMAOR_DME;
        work_reg |= com_prm->dme;
                                            /* DMAマスタイネーブルセット     */
    }
    if((com_prm->msk & DMA_CPU_M_AE) == DMA_CPU_M_AE){
        
        work_reg &= ~R_DMAOR_AE;            /* アドレスエラービットのクリア  */
    }
    if((com_prm->msk & DMA_CPU_M_NMIF) == DMA_CPU_M_NMIF){
        work_reg &= ~R_DMAOR_NMIF;              /* NMIエラービットのクリア   */
    }
    
    DMA_POKE_L(REG_DMAOR, work_reg);
                                   /* DMAオペレーションレジスタの書き込み    */
}

/******************************************************************************
 *
 * NAME:    DMA_CpuSetPrm() -   DMA転送パラメータ設定
 *
 * PARAMETERS :
 *      (1) DmaCpuPrm *prm      <i>   転送パラメータ.
 *      (2) Uint32 ch           <i>   DMAチャネル.
 *
 * DESCRIPTION:
 *      指定されたDMAチャネルへ転送パラメータを設定します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし。
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */

void DMA_CpuSetPrm(DmaCpuPrm *prm, Uint32 ch)
{
    Uint32 work_reg;

    if((prm->msk & DMA_CPU_M_SAR) == DMA_CPU_M_SAR){
        DMA_POKE_L((REG_SAR + OFSET0_REG * ch), prm->sar);
                                                /* ソースアドレスセット      */
    }
    if((prm->msk & DMA_CPU_M_DAR) == DMA_CPU_M_DAR){
        DMA_POKE_L((REG_DAR + OFSET0_REG * ch), prm->dar);
                                       /* ディスティネーションアドレスセット */
    }
    if((prm->msk & DMA_CPU_M_TCR) == DMA_CPU_M_TCR){
        DMA_POKE_L((REG_TCR + OFSET0_REG * ch), prm->tcr);
                                       /* トランスファカウントセット         */
    }


    work_reg = DMA_PEEK_L(REG_CHCR + OFSET0_REG * ch);
                                /* チャネルコントロールレジスタの読み込み    */
    
    if((prm->msk & DMA_CPU_M_DM) == DMA_CPU_M_DM){
        work_reg &= ~R_CHCR_DM;
        work_reg |= prm->dm << DMA_CHCR_DM;
                           /* ディスティネーションアドレスモードビットセット */
    }
    if((prm->msk & DMA_CPU_M_SM) == DMA_CPU_M_SM){
        work_reg &= ~R_CHCR_SM;
        work_reg |= prm->sm << DMA_CHCR_SM;
                                        /* ソースアドレスモードビットセット  */
    }
    if((prm->msk & DMA_CPU_M_TS) == DMA_CPU_M_TS){
        work_reg &= ~R_CHCR_TS;
        work_reg |= prm->ts;                    /* トランスファサイズセット  */
    }
    if((prm->msk & DMA_CPU_M_AR) == DMA_CPU_M_AR){
        work_reg &= ~R_CHCR_AR;
        work_reg |= prm->ar;                /* オートリクエストモードセット  */
    }
    work_reg &= ~R_CHCR_TB;                 /* トランスファバスモードセット  */
                                            /* (サイクルスチールモード固定)  */
    work_reg &= ~R_CHCR_TA;                 /* ﾄﾗﾝｽﾌｧｱﾄﾞﾚｽﾓｰﾄﾞﾋﾞｯﾄｾｯﾄ　　　　*/
                                            /* (デュアルアドレスモード固定)  */
    if((prm->msk & DMA_CPU_M_IE) == DMA_CPU_M_IE){
        work_reg &= ~R_CHCR_IE;             
        work_reg |= prm->ie;                /* インタラプトイネーブルセット  */
    }
    if((prm->msk & DMA_CPU_M_TE) == DMA_CPU_M_TE){
        work_reg &= ~R_CHCR_TE;             /* ﾄﾗﾝｽﾌｧｴﾝﾄﾞﾌﾗｸﾞのｸﾘｱ           */
    }
    DMA_POKE_L((REG_CHCR + OFSET0_REG * ch), work_reg);
                                /* チャネルコントロールレジスタの書き込み    */

    if((prm->msk & DMA_CPU_M_DRCR) == DMA_CPU_M_DRCR){
        DMA_POKE_B((REG_DRCR + OFSET1_REG * ch), prm->drcr);
                                /* DMA要求／応答選択制御レジスタの書き込み   */
    }
}

/******************************************************************************
 *
 * NAME:    DMA_CpuStart()  -   DMA転送開始
 *
 * PARAMETERS :
 *      (1) Uint32 ch           <i>   DMAチャネル.
 *
 * DESCRIPTION:
 *      指定されたDMAチャネルのDMA転送を開始します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし。
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */

void DMA_CpuStart(Uint32 ch)
{
    Uint32 work_reg;

    work_reg = DMA_PEEK_L(REG_CHCR + OFSET0_REG * ch);
                                /* チャネルコントロールレジスタの読み込み    */
    work_reg |= R_CHCR_DE;      /* DMAイネーブルビットを許可                 */
    DMA_POKE_L((REG_CHCR + OFSET0_REG * ch), work_reg);
                                /* チャネルコントロールレジスタの書き込み    */
}

/******************************************************************************
 *
 * NAME:    DMA_CpuStop()   -   DMA転送中止
 *
 * PARAMETERS :
 *      (1) Uint32 ch           <i>   DMAチャネル.
 *
 * DESCRIPTION:
 *      指定されたDMAチャネルのDMA転送を中止します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし。
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */

void DMA_CpuStop(Uint32 ch)
{
    Uint32 work_reg;

    work_reg = DMA_PEEK_L(REG_CHCR + OFSET0_REG * ch);
                                /* チャネルコントロールレジスタの読み込み    */
    work_reg &= ~R_CHCR_DE;     /* DMAイネーブルビットを禁止                 */
    DMA_POKE_L((REG_CHCR + OFSET0_REG * ch), work_reg);
                                /* チャネルコントロールレジスタの書き込み    */
}

/******************************************************************************
 *
 * NAME:    DMA_CpuAllStop()    -   DMA全チャネル転送中止
 *
 * PARAMETERS :
 *      なし。
 *
 * DESCRIPTION:
 *      全チャネルのDMA転送を中止します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし。
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */

void DMA_CpuAllStop(void)
{
    Uint32 work_reg;
    work_reg = DMA_PEEK_L(REG_DMAOR);
                                    /* DMAオペレーションレジスタの読み込み   */
    work_reg &= ~R_DMAOR_DME;       /* DMAマスタイネーブルビットを禁止       */
    DMA_POKE_L(REG_DMAOR, work_reg);
                                   /* DMAオペレーションレジスタの書き込み    */
}

/******************************************************************************
 *
 * NAME:    DMA_CpuGetComStatus()   -   DMA共通ステータス取得
 *
 * PARAMETERS :
 *      (1) DmaCpuComStatus *status     <o>   共通ステータスポインタ.
 *
 * DESCRIPTION:
 *      共通ステータスを取得します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし。
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */

void DMA_CpuGetComStatus(DmaCpuComStatus *status)
{
    Uint32 work_reg;
    
    work_reg = DMA_PEEK_L(REG_DMAOR);
                                    /* DMAオペレーションレジスタの読み込み   */
    status->ae = work_reg & R_DMAOR_AE; /* ｱﾄﾞﾚｽﾓｰﾄﾞｴﾗｰﾌﾗｸﾞの取得            */
    status->nmif = work_reg & R_DMAOR_NMIF; /* NMIフラグの取得               */
}

/******************************************************************************
 *
 * NAME:    DMA_CpuGetStatus()  -   DMAステータス取得
 *
 * PARAMETERS :
 *      (1) Uint32 ch       <o>   DMAチャネル.
 *
 * DESCRIPTION:
 *      ステータスを取得します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      (1) DmaCpuStatus    <o>   ステータスポインタ.
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */

DmaCpuStatus DMA_CpuGetStatus(Uint32 ch)
{
    Uint32 work_reg;
    
    work_reg = DMA_PEEK_L(REG_CHCR + OFSET0_REG * ch);
                                        /* DMAﾁｬﾈﾙｺﾝﾄﾛｰﾙﾚｼﾞｽﾀの読み込み      */
    return(work_reg & R_CHCR_TE);       /* ﾄﾗﾝｽﾌｧｴﾝﾄﾞﾌﾗｸﾞの取得              */
}
