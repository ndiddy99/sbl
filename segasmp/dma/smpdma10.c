/****************************************************************************/
/*  filename "smpdma10.c"                                                   */
/*  ＤＭＡサンプルプログラム                                                */
/*  Copyright(c) 1994 SEGA                                                  */
/*  Written by N.T on 1994-05-14 Ver.0.90                                   */
/*  Updated by N.T on 1994-10-12 Ver.1.00                                   */
/****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "sega_xpt.h"
#include "sega_csh.h"
#include "sega_dma.h"
#include "sega_int.h"

static Sint8 *pbuf;                         /*  結果格納バッファポインタ    */

void SDMA_ScuCst(Uint32 ,void *, void *, Uint32);
Uint32 SDMA_ScuResult(Uint32);
Uint32 hikaku(void *, void *, Uint32, Uint32, Sint8 *); /*  結果確認関数    */

void main(void)
{
    void *src, *dst;
    Uint32 cnt;
    Uint32 status;
    Uint32 *work;

    INT_ChgMsk(INT_MSK_DMA0, INT_MSK_NULL);         /* DMA割り込み終了許可  */
    INT_ChgMsk(INT_MSK_DMA1, INT_MSK_NULL);
    INT_ChgMsk(INT_MSK_DMA2, INT_MSK_NULL);

    pbuf = (Sint8 *)0x06040000;             /*  結果格納先頭アドレス        */

    src = (void *)0x6050000;   /*   address    */
    dst = (void *)0x25c00000;   /*   address    */
    cnt = 0x200;
    SDMA_ScuCst(DMA_SCU_CH2 ,dst ,src ,cnt);
    while(SDMA_ScuResult(DMA_SCU_CH2) != DMA_SCU_END);

    src = (void *)0x6050200;   /*   address    */
    dst = (void *)0x25c00200;   /*   address    */
    SDMA_ScuCst(DMA_SCU_CH1 ,dst ,src ,cnt);
    while(SDMA_ScuResult(DMA_SCU_CH1) != DMA_SCU_END);

    src = (void *)0x6050400;   /*   address    */
    dst = (void *)0x25c00400;   /*   address    */
    SDMA_ScuCst(DMA_SCU_CH0 ,dst ,src ,cnt);
    while(SDMA_ScuResult(DMA_SCU_CH0) != DMA_SCU_END);

    CSH_Purge((void *)0x25c00000, 0x8000);

    hikaku((void *)0x06050000, (void *)0x25c00000, 0x0600, 1, "ADR to ADR");

    for(;;);
}
/****************************************************************************/
/*  ステータス取得(チャネル指定)                                            */
/****************************************************************************/
Uint32 SDMA_ScuResult(Uint32 ch)
{
    DmaScuStatus status;

    DMA_ScuGetStatus(&status, ch);
        if(status.dxmv == DMA_SCU_MV)
            return(DMA_SCU_BUSY);
        return(DMA_SCU_END);
}
/****************************************************************************/
/*  チャネル、バイト数指定転送関数                                          */
/****************************************************************************/
void SDMA_ScuCst(Uint32 ch, void *dst, void *src, Uint32 cnt)
{
    DmaScuPrm prm;                              /* 転送パラメータ            */
                                                /*****************************/

    prm.dxr = (Uint32)src;
    prm.dxw = (Uint32)dst;
    prm.dxc = cnt;
    prm.dxad_r = DMA_SCU_R4;
    prm.dxad_w = DMA_SCU_W2;
    prm.dxmod = DMA_SCU_DIR;
    prm.dxrup = DMA_SCU_KEEP;
    prm.dxwup = DMA_SCU_REN;
    prm.dxft = DMA_SCU_F_DMA;
    prm.msk = DMA_SCU_M_DXR   |
              DMA_SCU_M_DXW;

    DMA_ScuSetPrm(&prm, ch);
    DMA_ScuStart(ch);
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
/*  End of File                                                             */
/****************************************************************************/
