/****************************************************************************/
/*  filename "smpdma.c"                                                     */
/*  ＤＭＡサンプルプログラム                                                */
/*  Copyright(c) 1994 SEGA                                                  */
/*  Written by Y.K on 1994-02-09 Ver.0.79                                   */
/*  Updated by Y.K on 1994-02-17 Ver.0.80                                   */
/****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "sega_xpt.h"
#include "sega_dma.h"

static Sint8 *pbuf;                         /*  結果格納バッファポインタ    */

Uint32 hikaku(void *, void *, Uint32, Uint32, Sint8 *); /*  結果確認関数    */
void rev(void *, void *, Uint32, Uint32);   /*  転送先データ破壊関数        */

void main(void)
{
    void *src, *dst;
    Uint32 cnt;

    pbuf = (Sint8 *)0x06040000;             /*  結果格納先頭アドレス        */

/*  BOOT to WORK    */
    src = (void *)0x00000000;   /*  BOOT address    */
    dst = (void *)0x06050000;   /*  WORK address    */
    cnt = 0x100;

    rev(src, dst, cnt, 1);
    DMA_CpuMemCopy1(dst, src, cnt);
    while(DMA_CPU_END != DMA_CpuResult());
    hikaku(src, dst, cnt, 1, "BOOT to WORK");

    dst = (void *)0x06050100;
    rev(src, dst, cnt, 2);
    DMA_CpuMemCopy2(dst, src, cnt);
    while(DMA_CPU_END != DMA_CpuResult());
    hikaku(src, dst, cnt, 2, "BOOT to WORK");

    dst = (void *)0x06050300;
    rev(src, dst, cnt, 4);
    DMA_CpuMemCopy4(dst, src, cnt);
    while(DMA_CPU_END != DMA_CpuResult());
    hikaku(src, dst, cnt, 4, "BOOT to WORK");

    dst = (void *)0x06050700;
    rev(src, dst, cnt, 16);
    DMA_CpuMemCopy16(dst, src, cnt);
    while(DMA_CPU_END != DMA_CpuResult());
    hikaku(src, dst, cnt, 16, "BOOT to WORK");

/*  WORK to WORK    */
    src = (void *)0x06050300;   /*  WORK address    */
    dst = (void *)0x06060000;   /*  WORK address    */
    cnt = 0x100;

    rev(src, dst, cnt, 1);
    DMA_CpuMemCopy1(dst, src, cnt);
    while(DMA_CPU_END != DMA_CpuResult());
    hikaku(src, dst, cnt, 1, "WORK to WORK");

    dst = (void *)0x06060100;
    rev(src, dst, cnt, 2);
    DMA_CpuMemCopy2(dst, src, cnt);
    while(DMA_CPU_END != DMA_CpuResult());
    hikaku(src, dst, cnt, 2, "WORK to WORK");

    dst = (void *)0x06060300;
    rev(src, dst, cnt, 4);
    DMA_CpuMemCopy4(dst, src, cnt);
    while(DMA_CPU_END != DMA_CpuResult());
    hikaku(src, dst, cnt, 4, "WORK to WORK");

    dst = (void *)0x06060700;
    rev(src, dst, cnt, 16);
    DMA_CpuMemCopy16(dst, src, cnt);
    while(DMA_CPU_END != DMA_CpuResult());
    hikaku(src, dst, cnt, 16, "WORK to WORK");

/*  WORK to VDP1    */
    src = (void *)0x06050300;   /*  WORK address    */
    dst = (void *)0x25c00000;   /*  VDP1 address    */
    cnt = 0x100;

    rev(src, dst, cnt, 1);
    DMA_CpuMemCopy1(dst, src, cnt);
    while(DMA_CPU_END != DMA_CpuResult());
    hikaku(src, dst, cnt, 1, "WORK to VDP1");

    dst = (void *)0x25c00100;
    rev(src, dst, cnt, 2);
    DMA_CpuMemCopy2(dst, src, cnt);
    while(DMA_CPU_END != DMA_CpuResult());
    hikaku(src, dst, cnt, 2, "WORK to VDP1");

    dst = (void *)0x25c00300;
    rev(src, dst, cnt, 4);
    DMA_CpuMemCopy4(dst, src, cnt);
    while(DMA_CPU_END != DMA_CpuResult());
    hikaku(src, dst, cnt, 4, "WORK to VDP1");

    dst = (void *)0x25c00700;
    rev(src, dst, cnt, 16);
    DMA_CpuMemCopy16(dst, src, cnt);
    while(DMA_CPU_END != DMA_CpuResult());
    hikaku(src, dst, cnt, 16, "WORK to VDP1");

/*  BOOT to all VDP2    */
    src = (void *)0x00000000;   /*  BOOT    */
    dst = (void *)0x25e00000;   /*  VDP2    */
    cnt = 0x00080000;
    DMA_CpuMemCopy1(dst, src, cnt);
    while(DMA_CPU_END != DMA_CpuResult());
    hikaku(src, dst, cnt, 1, "BOOT to all VDP2");

    for(;;) ;
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