/****************************************************************************/
/*  filename "smpdma0.c"                                                     */
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

void SDMA_CpuAllStart(void);
void SDMA_CpuCst(Uint32 , Uint32 ,void *, void *, Uint32);
Uint32 SDMA_CpuResult(Uint32);
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
    DMA_CpuAllStop();                       /* �S�`���l��DMA���~             */

    src = (void *)0x0000000;   /*  BOOT address    */
    dst = (void *)0x6050000;   /*  WORK address    */
    cnt = 0x200;

    SDMA_CpuCst(DMA_CPU_CH0 ,DMA_CPU_1 ,dst ,src ,cnt);
    src = (void *)0x0000200;   /*  BOOT address    */
    dst = (void *)0x6050200;   /*  WORK address    */
    cnt = 0x200;
    SDMA_CpuCst(DMA_CPU_CH1 ,DMA_CPU_1 ,dst ,src ,cnt);

    SDMA_CpuAllStart();                     /* �S�`���l��DMA�J�n              */
    while((SDMA_CpuResult(DMA_CPU_CH0) != DMA_CPU_END) ||
    	  (SDMA_CpuResult(DMA_CPU_CH1) != DMA_CPU_END));
    CSH_Purge((void *)0x6050000, 0x400);

    hikaku((void *)0x00000, (void *)0x6050000, 0x400, 1, "BOOT to WORK");

    for(;;);
}
/****************************************************************************/
/*  �S�`���l��DMA�J�n                                                       */
/****************************************************************************/
void SDMA_CpuAllStart(void)
{
    DmaCpuComPrm com_prm;                       /* ���ʓ]���p�����[�^        */
    com_prm.dme = DMA_CPU_ENA;                  /* DMAϽ��Ȱ��ِݒ�(����)    */
    com_prm.msk = DMA_CPU_M_DME;                /* �}�X�N�ݒ�(��ײ��èӰ��)  */
    DMA_CpuSetComPrm(&com_prm);                 /* DMA���ʓ]���p�����[�^�ݒ� */
}
/****************************************************************************/
/*  �X�e�[�^�X�擾(�`���l���w��)                                            */
/****************************************************************************/
Uint32 SDMA_CpuResult(Uint32 ch)
{
    DmaCpuComStatus com_status;             /* ���ʃX�e�[�^�X                */
    DmaCpuStatus status;                    /* �X�e�[�^�X                    */
                                            /*********************************/
    DMA_CpuGetComStatus(&com_status);       /* ���ʃX�e�[�^�X�擾            */
    if(com_status.ae == DMA_CPU_ADR_ERR){   /* ���ڽ�װ�����������ꍇ        */
        return(DMA_CPU_FAIL);               /* �ُ�I�������^�[��            */
    }
    status = DMA_CpuGetStatus(ch);          /* �X�e�[�^�X�擾                */
    if(status == DMA_CPU_TE_MV){            /* ���쒆�ł���ꍇ              */
        return(DMA_CPU_BUSY);               /* ���s�������^�[��              */
    }
    return(DMA_CPU_END);                    /* ����I�������^�[��            */
}
/****************************************************************************/
/*  �`���l���A�o�C�g���w��]���֐�                                          */
/*    �i���jDMA�}�X�^�C�l�[�u���͕ێ�                                       */
/****************************************************************************/
void SDMA_CpuCst(Uint32 ch, Uint32 byte ,void *dst, void *src, Uint32 cnt)
{
    DmaCpuComPrm com_prm;                       /* ���ʓ]���p�����[�^        */
    DmaCpuPrm prm;                              /* �]���p�����[�^            */
                                                /*****************************/
    DMA_CpuStop(ch);                            /* DMA�]�����~               */
    com_prm.pr = DMA_CPU_FIX;                   /* �D�揇�ʐݒ�(׳�������)   */
    com_prm.msk = DMA_CPU_M_PR |                /* �}�X�N�ݒ�(��ײ��èӰ��)  */
                  DMA_CPU_M_AE |                /* (�A�h���X�G���[�t���O)    */
                  DMA_CPU_M_NMIF;               /* (NMI�t���O)               */

    DMA_CpuSetComPrm(&com_prm);                 /* DMA���ʓ]���p�����[�^�ݒ� */

    prm.sar = (Uint32)src;                      /* �\�[�X�A�h���X�ݒ�        */
    prm.dar = (Uint32)dst;                      /* �ި�èȰ��ݱ��ڽ�ݒ�      */
    prm.tcr = cnt;                              /* �g�����X�t�@�J�E���g�ݒ�  */
    prm.dm = DMA_CPU_AM_ADD;                    /* �ި�èȰ��ݱ��ڽӰ�ސݒ�  */
    prm.sm = DMA_CPU_AM_ADD;                    /* ������ڽӰ�ސݒ�          */
    switch(byte){
        case DMA_CPU_1:
            prm.ts = DMA_CPU_1;                 /* ��ݽ̧���ސݒ�            */
            break;
        case DMA_CPU_2:
            prm.ts = DMA_CPU_2;                 /* ��ݽ̧���ސݒ�            */
            break;
        case DMA_CPU_4:
            prm.ts = DMA_CPU_4;                 /* ��ݽ̧���ސݒ�            */
            break;
        case DMA_CPU_16:
            prm.ts = DMA_CPU_16;                /* ��ݽ̧���ސݒ�            */
            break;
    }
    prm.ar = DMA_CPU_AUTO;                      /* ���ظ���Ӱ�ސݒ�          */
    prm.ie = DMA_CPU_INT_DIS;                   /* ������ĲȰ��ِݒ�         */
    
    prm.msk = DMA_CPU_M_SAR |                   /* �}�X�N�ݒ�                */
              DMA_CPU_M_DAR |
              DMA_CPU_M_TCR |
              DMA_CPU_M_DM  |
              DMA_CPU_M_SM  |
              DMA_CPU_M_TS  |
              DMA_CPU_M_AR  |
              DMA_CPU_M_IE  |
              DMA_CPU_M_TE;                     /* ��ݽ̧�����ޯẴN���A�w��*/

    DMA_CpuSetPrm(&prm, ch);                    /* DMA�]���p�����[�^�ݒ�     */
    
    DMA_CpuStart(ch);                           /* DMA�]���J�n               */
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