/*-----------------------------------------------------------------------------  *  FILE: dma_cpu5.c
 *
 *      Copyright(c) 1994 SEGA.
 *
 *  PURPOSE:
 *
 *      CPU DMA高水準。
 *
 *  DESCRIPTION:
 *
 *      CPU DMA高水準転送機能。
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *          (5) DMA_CpuResult           -   データ転送終了チェック
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
#include "sega_mth.h"
#include "sega_csh.h"
#include "sega_xpt.h"

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include "dma_cpu0.h"
#include "dma_cpum.h"

/*
 * GLOBAL DECLARATIONS
 */

/*
 * LOCAL DEFINES/MACROS
 */

/*
 * STATIC DECLARATIONS
 */
void *dma_cpu_dis_adr;                   /* ﾊﾟｰｼﾞ対象ﾃﾞｨｽﾃｨﾈｰｼｮﾝｱﾄﾞﾚｽ */
Uint32 dma_cpu_cnt;                      /* ﾊﾟｰｼﾞ対象ﾃﾞｨｽﾃｨﾈｰｼｮﾝｶｳﾝﾄ  */

/*
 * STATIC FUNCTION PROTOTYPE DECLARATIONS
 */


/******************************************************************************
 *
 * NAME:    DMA_CpuResult()     -   データ転送終了チェック
 *
 * PARAMETERS :
 *      (2) void *src       <i>   ソースアドレス.
 *      (3) Uint32 cnt      <i>   転送回数(0～16777215)
 *
 * DESCRIPTION:
 *      srcアドレスからdstアドレスへ16バイトデータをcnt回数分転送します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      Uint32 DMA_CpuMemCopy1,2,4,16()の結果.
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */

Uint32 DMA_CpuResult(void)
{
    DmaCpuComStatus com_status;             /* 共通ステータス                */
    DmaCpuStatus status;                    /* ステータス                    */
                                            /*********************************/
    DMA_CpuGetComStatus(&com_status);       /* 共通ステータス取得            */
    if(com_status.ae == DMA_CPU_ADR_ERR){   /* ｱﾄﾞﾚｽｴﾗｰが発生した場合        */
        return(DMA_CPU_FAIL);               /* 異常終了をリターン            */
    }
    status = DMA_CpuGetStatus(DMA_CPU_CH0); /* ステータス取得                */
    if(status == DMA_CPU_TE_MV){            /* 動作中である場合              */
        return(DMA_CPU_BUSY);               /* 実行中をリターン              */
    }
    CSH_Purge(dma_cpu_dis_adr, dma_cpu_cnt);
    return(DMA_CPU_END);                    /* 正常終了をリターン            */
}

