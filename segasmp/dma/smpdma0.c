/****************************************************************************/
/*  filename "smpdma0.c"                                                     */
/*  ＤＭＡサンプルプログラム                                                */
/*  Copyright(c) 1994 SEGA                                                  */
/*  Written by N.T on 1994-05-14 Ver.0.90                                   */
/*  Updated by N.T on 1994-05-14 Ver.0.90                                   */
/****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "sega_xpt.h"
#include "sega_csh.h"
#include "sega_dma.h"

static Sint8 *pbuf;                         /*  結果格納バッファポインタ    */

void SDMA_CpuAllStart(void);
void SDMA_CpuCst(Uint32 , Uint32 ,void *, void *, Uint32);
Uint32 SDMA_CpuResult(Uint32);
Uint32 hikaku(void *, void *, Uint32, Uint32, Sint8 *); /*  結果確認関数    */
void rev(void *, void *, Uint32, Uint32);   /*  転送先データ破壊関数        */

void main(void)
{
    void *src, *dst;
    Uint32 cnt;
    Uint32 status;
    Uint32 *work;

    pbuf = (Sint8 *)0x06040000;             /*  結果格納先頭アドレス        */

/*  BOOT to WORK    */
    DMA_CpuAllStop();                       /* 全チャネルDMA中止             */

    src = (void *)0x0000000;   /*  BOOT address    */
    dst = (void *)0x6050000;   /*  WORK address    */
    cnt = 0x200;

    SDMA_CpuCst(DMA_CPU_CH0 ,DMA_CPU_1 ,dst ,src ,cnt);
    src = (void *)0x0000200;   /*  BOOT address    */
    dst = (void *)0x6050200;   /*  WORK address    */
    cnt = 0x200;
    SDMA_CpuCst(DMA_CPU_CH1 ,DMA_CPU_1 ,dst ,src ,cnt);

    SDMA_CpuAllStart();                     /* 全チャネルDMA開始              */
    while((SDMA_CpuResult(DMA_CPU_CH0) != DMA_CPU_END) ||
    	  (SDMA_CpuResult(DMA_CPU_CH1) != DMA_CPU_END));
    CSH_Purge((void *)0x6050000, 0x400);

    hikaku((void *)0x00000, (void *)0x6050000, 0x400, 1, "BOOT to WORK");

    for(;;);
}
/****************************************************************************/
/*  全チャネルDMA開始                                                       */
/****************************************************************************/
void SDMA_CpuAllStart(void)
{
    DmaCpuComPrm com_prm;                       /* 共通転送パラメータ        */
    com_prm.dme = DMA_CPU_ENA;                  /* DMAﾏｽﾀｲﾈｰﾌﾞﾙ設定(許可)    */
    com_prm.msk = DMA_CPU_M_DME;                /* マスク設定(ﾌﾟﾗｲｵﾘﾃｨﾓｰﾄﾞ)  */
    DMA_CpuSetComPrm(&com_prm);                 /* DMA共通転送パラメータ設定 */
}
/****************************************************************************/
/*  ステータス取得(チャネル指定)                                            */
/****************************************************************************/
Uint32 SDMA_CpuResult(Uint32 ch)
{
    DmaCpuComStatus com_status;             /* 共通ステータス                */
    DmaCpuStatus status;                    /* ステータス                    */
                                            /*********************************/
    DMA_CpuGetComStatus(&com_status);       /* 共通ステータス取得            */
    if(com_status.ae == DMA_CPU_ADR_ERR){   /* ｱﾄﾞﾚｽｴﾗｰが発生した場合        */
        return(DMA_CPU_FAIL);               /* 異常終了をリターン            */
    }
    status = DMA_CpuGetStatus(ch);          /* ステータス取得                */
    if(status == DMA_CPU_TE_MV){            /* 動作中である場合              */
        return(DMA_CPU_BUSY);               /* 実行中をリターン              */
    }
    return(DMA_CPU_END);                    /* 正常終了をリターン            */
}
/****************************************************************************/
/*  チャネル、バイト数指定転送関数                                          */
/*    （注）DMAマスタイネーブルは保持                                       */
/****************************************************************************/
void SDMA_CpuCst(Uint32 ch, Uint32 byte ,void *dst, void *src, Uint32 cnt)
{
    DmaCpuComPrm com_prm;                       /* 共通転送パラメータ        */
    DmaCpuPrm prm;                              /* 転送パラメータ            */
                                                /*****************************/
    DMA_CpuStop(ch);                            /* DMA転送中止               */
    com_prm.pr = DMA_CPU_FIX;                   /* 優先順位設定(ﾗｳﾝﾄﾞﾛﾋﾞﾝ)   */
    com_prm.msk = DMA_CPU_M_PR |                /* マスク設定(ﾌﾟﾗｲｵﾘﾃｨﾓｰﾄﾞ)  */
                  DMA_CPU_M_AE |                /* (アドレスエラーフラグ)    */
                  DMA_CPU_M_NMIF;               /* (NMIフラグ)               */

    DMA_CpuSetComPrm(&com_prm);                 /* DMA共通転送パラメータ設定 */

    prm.sar = (Uint32)src;                      /* ソースアドレス設定        */
    prm.dar = (Uint32)dst;                      /* ﾃﾞｨｽﾃｨﾈｰｼｮﾝｱﾄﾞﾚｽ設定      */
    prm.tcr = cnt;                              /* トランスファカウント設定  */
    prm.dm = DMA_CPU_AM_ADD;                    /* ﾃﾞｨｽﾃｨﾈｰｼｮﾝｱﾄﾞﾚｽﾓｰﾄﾞ設定  */
    prm.sm = DMA_CPU_AM_ADD;                    /* ｿｰｽｱﾄﾞﾚｽﾓｰﾄﾞ設定          */
    switch(byte){
        case DMA_CPU_1:
            prm.ts = DMA_CPU_1;                 /* ﾄﾗﾝｽﾌｧｻｲｽﾞ設定            */
            break;
        case DMA_CPU_2:
            prm.ts = DMA_CPU_2;                 /* ﾄﾗﾝｽﾌｧｻｲｽﾞ設定            */
            break;
        case DMA_CPU_4:
            prm.ts = DMA_CPU_4;                 /* ﾄﾗﾝｽﾌｧｻｲｽﾞ設定            */
            break;
        case DMA_CPU_16:
            prm.ts = DMA_CPU_16;                /* ﾄﾗﾝｽﾌｧｻｲｽﾞ設定            */
            break;
    }
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

    DMA_CpuSetPrm(&prm, ch);                    /* DMA転送パラメータ設定     */
    
    DMA_CpuStart(ch);                           /* DMA転送開始               */
}
/****************************************************************************/
/*  結果比較関数                                                            */
/****************************************************************************/
Uint32 hikaku(void *src, void *dst, Uint32 cnt, Uint32 size, Sint8 *msg)
{
    Uint32 i, p;
    
    sprintf(pbuf, "[[ \'%s\' %08X >> %08X : %08X Count, %01X Byte ",
     msg, src, dst, cnt, size);
    pbuf += strlen(pbuf);
    for(i = 0; i < cnt; i++) {
        switch(size) {
            case 1:
                if(*(Uint8 *)dst != *(Uint8 *)src) {
                    sprintf(pbuf, "Error %08X(%02X,%02X) ]] \n",
                     dst, *(Uint8 *)src, *(Uint8 *)dst);
                    pbuf += strlen(pbuf);
                    return 1;
                }
		*(Uint32 *)0x6040200 = cnt;
                break;
            case 2:
                if(*(Uint16 *)dst != *(Uint16 *)src) {
                    sprintf(pbuf, "Error %08X(%04X,%04X) ]] \n",
                     dst, *(Uint16 *)src, *(Uint16 *)dst);
                    pbuf += strlen(pbuf);
                    return 1;
                }
                break;
            case 4:
                if(*(Uint32 *)dst != *(Uint32 *)src) {
                    sprintf(pbuf, "Error %08X(%08X,%08X) ]] \n",
                     dst, *(Uint32 *)src, *(Uint32 *)dst);
                    pbuf += strlen(pbuf);
                    return 1;
                }
                break;
            case 16:
                if((*(Uint32 *)dst != *(Uint32 *)src)
                 || (*((Uint32 *)dst + 1) != *((Uint32 *)src + 1))
                 || (*((Uint32 *)dst + 2) != *((Uint32 *)src + 2))
                 || (*((Uint32 *)dst + 3) != *((Uint32 *)src + 3))) {
                    sprintf(pbuf,
                     "Error %08X(%08X%08X%08X%08X,%08X%08X%08X%08X) ]] \n",
                      dst, *(Uint32 *)src, *(Uint32 *)((Uint32)src + 4),
                      *(Uint32 *)((Uint32)src + 8),
                      *(Uint32 *)((Uint32)src + 12),
                      *(Uint32 *)dst, *(Uint32 *)((Uint32)dst + 4),
                      *(Uint32 *)((Uint32)dst + 8),
                      *(Uint32 *)((Uint32)dst + 12));
                    pbuf += strlen(pbuf);
                    return 1;
                }
                i += 3;
                break;
            default:
                break;
        }
        dst = (void *)((Uint32)dst + size);
        src = (void *)((Uint32)src + size);
    }
    sprintf(pbuf, "OK ]] \n");
    pbuf += strlen(pbuf);
    return 0;
}

/****************************************************************************/
/*  転送先内容破壊関数                                                      */
/****************************************************************************/
void rev(void *src, void *dst, Uint32 cnt, Uint32 size)
{
    Uint32 i;
    
    for(i = 0; i < cnt; i++) {
        switch(size) {
            case 1:
                *(Uint8 *)dst = ~*(Uint8 *)src;
                break;
            case 2:
                *(Uint16 *)dst = ~*(Uint16 *)src;
                break;
            case 4:
                *(Uint32 *)dst = ~*(Uint32 *)src;
                break;
            case 16:
                *(Uint32 *)dst = ~*(Uint32 *)src;
                *((Uint32 *)dst + 1) = ~*((Uint32 *)src + 1);
                *((Uint32 *)dst + 2) = ~*((Uint32 *)src + 2);
                *((Uint32 *)dst + 3) = ~*((Uint32 *)src + 3);
                i += 3;
                break;
            default:
                break;
        }
        src = (void *)((Uint32)src + size);
        dst = (void *)((Uint32)dst + size);
    }
}

/****************************************************************************/
/*  End of File                                                             */
/****************************************************************************/
