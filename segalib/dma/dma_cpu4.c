/*-----------------------------------------------------------------------------  *  FILE: dma_cpu4.c
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
 *          (4) DMA_CpuMemCopy16        -   データ転送(16バイト単位)
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
extern void *dma_cpu_dis_adr;                   /* ﾊﾟｰｼﾞ対象ﾃﾞｨｽﾃｨﾈｰｼｮﾝｱﾄﾞﾚｽ */
extern Uint32 dma_cpu_cnt;                      /* ﾊﾟｰｼﾞ対象ﾃﾞｨｽﾃｨﾈｰｼｮﾝｶｳﾝﾄ  */

/*
 * STATIC FUNCTION PROTOTYPE DECLARATIONS
 */


/******************************************************************************
 *
 * NAME:    DMA_CpuMemCopy16()  -   データ転送(16バイト単位)
 *
 * PARAMETERS :
 *      (1) void *dst       <i>   ディスティネーションアドレス.
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
 *      なし。
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */

void DMA_CpuMemCopy16(void *dst, void *src, Uint32 cnt)
{
    DmaCpuComPrm com_prm;                       /* 共通転送パラメータ        */
    DmaCpuPrm prm;                              /* 転送パラメータ            */
                                                /*****************************/
    DMA_CpuStop(DMA_CPU_CH0);                   /* DMA転送中止               */

    com_prm.pr = DMA_CPU_FIX;                   /* 優先順位設定(ﾗｳﾝﾄﾞﾛﾋﾞﾝ)   */
    com_prm.dme = DMA_CPU_ENA;                  /* DMAﾏｽﾀｲﾈｰﾌﾞﾙ設定(許可)    */
    com_prm.msk = DMA_CPU_M_PR |                /* マスク設定(ﾌﾟﾗｲｵﾘﾃｨﾓｰﾄﾞ)  */
                  DMA_CPU_M_AE |                /* (アドレスエラーフラグ)    */
                  DMA_CPU_M_NMIF |              /* (NMIフラグ)               */
                  DMA_CPU_M_DME;                /* (DMAマスタイネーブル)     */

    DMA_CpuSetComPrm(&com_prm);                 /* DMA共通転送パラメータ設定 */

    prm.sar = (Uint32)src;                      /* ソースアドレス設定        */
    prm.dar = (Uint32)dst;                      /* ﾃﾞｨｽﾃｨﾈｰｼｮﾝｱﾄﾞﾚｽ設定      */
    prm.tcr = cnt;                              /* トランスファカウント設定  */
    prm.dm = DMA_CPU_AM_ADD;                    /* ﾃﾞｨｽﾃｨﾈｰｼｮﾝｱﾄﾞﾚｽﾓｰﾄﾞ設定  */
    prm.sm = DMA_CPU_AM_ADD;                    /* ｿｰｽｱﾄﾞﾚｽﾓｰﾄﾞ設定          */
    prm.ts = DMA_CPU_16;                        /* ﾄﾗﾝｽﾌｧｻｲｽﾞ設定            */
    prm.ar = DMA_CPU_AUTO;                      /* ｵｰﾄﾘｸｴｽﾄﾓｰﾄﾞ設定          */
    prm.ie = DMA_CPU_INT_DIS;                   /* ｲﾝﾀﾗﾌﾟﾄｲﾈｰﾌﾞﾙ設定         */
    
    prm.msk = DMA_CPU_M_SAR |                   /* マスク設定                */
              DMA_CPU_M_DAR |
              DMA_CPU_M_TCR |
              DMA_CPU_M_DM  |
              DMA_CPU_M_SM  |
              DMA_CPU_M_TS  |
              DMA_CPU_M_AR  |
              DMA_CPU_M_IE  |
              DMA_CPU_M_TE;                     /* ﾄﾗﾝｽﾌｧｴﾝﾄﾞﾋﾞｯﾄのクリア指定*/

    DMA_CpuSetPrm(&prm, DMA_CPU_CH0);           /* DMA転送パラメータ設定     */
    
    DMA_CpuStart(DMA_CPU_CH0);                  /* DMA転送開始               */
    dma_cpu_dis_adr = dst;
    dma_cpu_cnt = cnt * 4;
}
