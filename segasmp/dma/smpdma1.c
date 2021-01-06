/****************************************************************************/
/*  filename "smpdma1.c"                                                    */
/*  �c�l�`�T���v���v���O����                                                */
/*  Copyright(c) 1994 SEGA                                                  */
/*  Written by N.T on 1994-05-14 Ver.0.90                                   */
/*  Updated by N.T on 1994-05-14 Ver.0.90                                   */
/****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "sega_xpt.h"
#include "sega_csh.h"
#include "sega_dma.h"

static Sint8 *pbuf;                         /*  ���ʊi�[�o�b�t�@�|�C���^    */

Uint32 hikaku(void *, void *, Uint32, Uint32, Sint8 *); /*  ���ʊm�F�֐�    */
void rev(void *, void *, Uint32, Uint32);   /*  �]����f�[�^�j��֐�        */

void main(void)
{
    void *src, *dst;
    Uint32 cnt;
    Uint32 status;
    Uint32 *work;

    pbuf = (Sint8 *)0x06040000;             /*  ���ʊi�[�擪�A�h���X        */

/*  BOOT to WORK    */
    src = (void *)0x00000000;   /*  WORK address    */
    dst = (void *)0x6050000;   /*  WORK address    */
    cnt = 0x100;

    rev(src, dst, cnt, 1);
    DMA_CpuMemCopy1(dst, src, cnt);
    CSH_Purge(dst, cnt);

    hikaku(src, dst, cnt, 1, "BOOT to WORK");

    for(;;) ;
}
/****************************************************************************/
/*  ���ʔ�r�֐�                                                            */
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
/*  �]������e�j��֐�                                                      */
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