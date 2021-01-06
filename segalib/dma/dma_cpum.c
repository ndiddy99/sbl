/*-----------------------------------------------------------------------------  *  FILE: dma_cpu1.c
 *
 *      Copyright(c) 1994 SEGA.
 *
 *  PURPOSE:
 *
 *      CPU DMA�������B
 *
 *  DESCRIPTION:
 *
 *      CPU DMA�������]���@�\�B
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *          (1) DMA_CpuMemCopy1         -   �f�[�^�]��(�o�C�g�P��)
 *          (2) DMA_CpuMemCopy2         -   �f�[�^�]��(���[�h�P��)
 *          (3) DMA_CpuMemCopy4         -   �f�[�^�]��(�����O���[�h�P��)
 *          (4) DMA_CpuMemCopy16        -   �f�[�^�]��(16�o�C�g�P��)
 *          (5) DMA_CpuResult           -   �f�[�^�]���I���`�F�b�N
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
extern void *dma_cpu_dis_adr;                   /* �߰�ޑΏ��ި�èȰ��ݱ��ڽ */
extern Uint32 dma_cpu_cnt;                      /* �߰�ޑΏ��ި�èȰ��ݶ���  */

/*
 * STATIC FUNCTION PROTOTYPE DECLARATIONS
 */


/******************************************************************************
 *
 * NAME:    DMA_CpuMemCopy1()   -   �f�[�^�]��(�o�C�g�P��)
 *
 * PARAMETERS :
 *      (1) void *dst       <i>   �f�B�X�e�B�l�[�V�����A�h���X.
 *      (2) void *src       <i>   �\�[�X�A�h���X.
 *      (3) Uint32 cnt      <i>   �]����(0�`16777215)
 *
 * DESCRIPTION:
 *      src�A�h���X����dst�A�h���X�փo�C�g�f�[�^��cnt�񐔕��]�����܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      �Ȃ��B
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */

void DMA_CpuMemCopy1(void *dst, void *src, Uint32 cnt)
{
    DmaCpuComPrm com_prm;                       /* ���ʓ]���p�����[�^        */
    DmaCpuPrm prm;                              /* �]���p�����[�^            */
                                                /*****************************/
    DMA_CpuStop(DMA_CPU_CH0);                   /* DMA�]�����~               */

    com_prm.pr = DMA_CPU_FIX;                   /* �D�揇�ʐݒ�(׳�������)   */
    com_prm.dme = DMA_CPU_ENA;                  /* DMAϽ��Ȱ��ِݒ�(����)    */
    com_prm.msk = DMA_CPU_M_PR |                /* �}�X�N�ݒ�(��ײ��èӰ��)  */
                  DMA_CPU_M_AE |                /* (�A�h���X�G���[�t���O)    */
                  DMA_CPU_M_NMIF |              /* (NMI�t���O)               */
                  DMA_CPU_M_DME;                /* (DMA�}�X�^�C�l�[�u��)     */

    DMA_CpuSetComPrm(&com_prm);                 /* DMA���ʓ]���p�����[�^�ݒ� */

    prm.sar = (Uint32)src;                      /* �\�[�X�A�h���X�ݒ�        */
    prm.dar = (Uint32)dst;                      /* �ި�èȰ��ݱ��ڽ�ݒ�      */
    prm.tcr = cnt;                              /* �g�����X�t�@�J�E���g�ݒ�  */
    prm.dm = DMA_CPU_AM_ADD;                    /* �ި�èȰ��ݱ��ڽӰ�ސݒ�  */
    prm.sm = DMA_CPU_AM_ADD;                    /* ������ڽӰ�ސݒ�          */
    prm.ts = DMA_CPU_1;                         /* ��ݽ̧���ސݒ�            */
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

    DMA_CpuSetPrm(&prm, DMA_CPU_CH0);           /* DMA�]���p�����[�^�ݒ�     */
    
    DMA_CpuStart(DMA_CPU_CH0);                  /* DMA�]���J�n               */

    dma_cpu_dis_adr = dst;
    dma_cpu_cnt = cnt;

}

/******************************************************************************
 *
 * NAME:    DMA_CpuMemCopy2()   -   �f�[�^�]��(���[�h�P��)
 *
 * PARAMETERS :
 *      (1) void *dst       <i>   �f�B�X�e�B�l�[�V�����A�h���X.
 *      (2) void *src       <i>   �\�[�X�A�h���X.
 *      (3) Uint32 cnt      <i>   �]����(0�`16777215)
 *
 * DESCRIPTION:
 *      src�A�h���X����dst�A�h���X�փ��[�h�f�[�^��cnt�񐔕��]�����܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      �Ȃ��B
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */

void DMA_CpuMemCopy2(void *dst, void *src, Uint32 cnt)
{
    DmaCpuComPrm com_prm;                       /* ���ʓ]���p�����[�^        */
    DmaCpuPrm prm;                              /* �]���p�����[�^            */
                                                /*****************************/
    DMA_CpuStop(DMA_CPU_CH0);                   /* DMA�]�����~               */

    com_prm.pr = DMA_CPU_FIX;                   /* �D�揇�ʐݒ�(׳�������)   */
    com_prm.dme = DMA_CPU_ENA;                  /* DMAϽ��Ȱ��ِݒ�(����)    */
    com_prm.msk = DMA_CPU_M_PR |                /* �}�X�N�ݒ�(��ײ��èӰ��)  */
                  DMA_CPU_M_AE |                /* (�A�h���X�G���[�t���O)    */
                  DMA_CPU_M_NMIF |              /* (NMI�t���O)               */
                  DMA_CPU_M_DME;                /* (DMA�}�X�^�C�l�[�u��)     */

    DMA_CpuSetComPrm(&com_prm);                 /* DMA���ʓ]���p�����[�^�ݒ� */

    prm.sar = (Uint32)src;                      /* �\�[�X�A�h���X�ݒ�        */
    prm.dar = (Uint32)dst;                      /* �ި�èȰ��ݱ��ڽ�ݒ�      */
    prm.tcr = cnt;                              /* �g�����X�t�@�J�E���g�ݒ�  */
    prm.dm = DMA_CPU_AM_ADD;                    /* �ި�èȰ��ݱ��ڽӰ�ސݒ�  */
    prm.sm = DMA_CPU_AM_ADD;                    /* ������ڽӰ�ސݒ�          */
    prm.ts = DMA_CPU_2;                         /* ��ݽ̧���ސݒ�            */
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

    DMA_CpuSetPrm(&prm, DMA_CPU_CH0);           /* DMA�]���p�����[�^�ݒ�     */
    
    DMA_CpuStart(DMA_CPU_CH0);                  /* DMA�]���J�n               */
    dma_cpu_dis_adr = dst;
    dma_cpu_cnt = cnt * 2;
}

/******************************************************************************
 *
 * NAME:    DMA_CpuMemCopy4()   -   �f�[�^�]��(�����O���[�h�P��)
 *
 * PARAMETERS :
 *      (1) void *dst       <i>   �f�B�X�e�B�l�[�V�����A�h���X.
 *      (2) void *src       <i>   �\�[�X�A�h���X.
 *      (3) Uint32 cnt      <i>   �]����(0�`16777215)
 *
 * DESCRIPTION:
 *      src�A�h���X����dst�A�h���X�փ����O���[�h�f�[�^��cnt�񐔕��]�����܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      �Ȃ��B
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */

void DMA_CpuMemCopy4(void *dst, void *src, Uint32 cnt)
{
    DmaCpuComPrm com_prm;                       /* ���ʓ]���p�����[�^        */
    DmaCpuPrm prm;                              /* �]���p�����[�^            */
                                                /*****************************/
    DMA_CpuStop(DMA_CPU_CH0);                   /* DMA�]�����~               */

    com_prm.pr = DMA_CPU_FIX;                   /* �D�揇�ʐݒ�(׳�������)   */
    com_prm.dme = DMA_CPU_ENA;                  /* DMAϽ��Ȱ��ِݒ�(����)    */
    com_prm.msk = DMA_CPU_M_PR |                /* �}�X�N�ݒ�(��ײ��èӰ��)  */
                  DMA_CPU_M_AE |                /* (�A�h���X�G���[�t���O)    */
                  DMA_CPU_M_NMIF |              /* (NMI�t���O)               */
                  DMA_CPU_M_DME;                /* (DMA�}�X�^�C�l�[�u��)     */

    DMA_CpuSetComPrm(&com_prm);                 /* DMA���ʓ]���p�����[�^�ݒ� */

    prm.sar = (Uint32)src;                      /* �\�[�X�A�h���X�ݒ�        */
    prm.dar = (Uint32)dst;                      /* �ި�èȰ��ݱ��ڽ�ݒ�      */
    prm.tcr = cnt;                              /* �g�����X�t�@�J�E���g�ݒ�  */
    prm.dm = DMA_CPU_AM_ADD;                    /* �ި�èȰ��ݱ��ڽӰ�ސݒ�  */
    prm.sm = DMA_CPU_AM_ADD;                    /* ������ڽӰ�ސݒ�          */
    prm.ts = DMA_CPU_4;                         /* ��ݽ̧���ސݒ�            */
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

    DMA_CpuSetPrm(&prm, DMA_CPU_CH0);           /* DMA�]���p�����[�^�ݒ�     */
    
    DMA_CpuStart(DMA_CPU_CH0);                  /* DMA�]���J�n               */
    dma_cpu_dis_adr = dst;
    dma_cpu_cnt = cnt * 4;
}

/******************************************************************************
 *
 * NAME:    DMA_CpuMemCopy16()  -   �f�[�^�]��(16�o�C�g�P��)
 *
 * PARAMETERS :
 *      (1) void *dst       <i>   �f�B�X�e�B�l�[�V�����A�h���X.
 *      (2) void *src       <i>   �\�[�X�A�h���X.
 *      (3) Uint32 cnt      <i>   �]����(0�`16777215)
 *
 * DESCRIPTION:
 *      src�A�h���X����dst�A�h���X��16�o�C�g�f�[�^��cnt�񐔕��]�����܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      �Ȃ��B
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */

void DMA_CpuMemCopy16(void *dst, void *src, Uint32 cnt)
{
    DmaCpuComPrm com_prm;                       /* ���ʓ]���p�����[�^        */
    DmaCpuPrm prm;                              /* �]���p�����[�^            */
                                                /*****************************/
    DMA_CpuStop(DMA_CPU_CH0);                   /* DMA�]�����~               */

    com_prm.pr = DMA_CPU_FIX;                   /* �D�揇�ʐݒ�(׳�������)   */
    com_prm.dme = DMA_CPU_ENA;                  /* DMAϽ��Ȱ��ِݒ�(����)    */
    com_prm.msk = DMA_CPU_M_PR |                /* �}�X�N�ݒ�(��ײ��èӰ��)  */
                  DMA_CPU_M_AE |                /* (�A�h���X�G���[�t���O)    */
                  DMA_CPU_M_NMIF |              /* (NMI�t���O)               */
                  DMA_CPU_M_DME;                /* (DMA�}�X�^�C�l�[�u��)     */

    DMA_CpuSetComPrm(&com_prm);                 /* DMA���ʓ]���p�����[�^�ݒ� */

    prm.sar = (Uint32)src;                      /* �\�[�X�A�h���X�ݒ�        */
    prm.dar = (Uint32)dst;                      /* �ި�èȰ��ݱ��ڽ�ݒ�      */
    prm.tcr = cnt;                              /* �g�����X�t�@�J�E���g�ݒ�  */
    prm.dm = DMA_CPU_AM_ADD;                    /* �ި�èȰ��ݱ��ڽӰ�ސݒ�  */
    prm.sm = DMA_CPU_AM_ADD;                    /* ������ڽӰ�ސݒ�          */
    prm.ts = DMA_CPU_16;                        /* ��ݽ̧���ސݒ�            */
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

    DMA_CpuSetPrm(&prm, DMA_CPU_CH0);           /* DMA�]���p�����[�^�ݒ�     */
    
    DMA_CpuStart(DMA_CPU_CH0);                  /* DMA�]���J�n               */
    dma_cpu_dis_adr = dst;
    dma_cpu_cnt = cnt * 4;
}

/******************************************************************************
 *
 * NAME:    DMA_CpuResult()     -   �f�[�^�]���I���`�F�b�N
 *
 * PARAMETERS :
 *      (2) void *src       <i>   �\�[�X�A�h���X.
 *      (3) Uint32 cnt      <i>   �]����(0�`16777215)
 *
 * DESCRIPTION:
 *      src�A�h���X����dst�A�h���X��16�o�C�g�f�[�^��cnt�񐔕��]�����܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      Uint32 DMA_CpuMemCopy1,2,4,16()�̌���.
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */

Uint32 DMA_CpuResult(void)
{
    DmaCpuComStatus com_status;             /* ���ʃX�e�[�^�X                */
    DmaCpuStatus status;                    /* �X�e�[�^�X                    */
                                            /*********************************/
    DMA_CpuGetComStatus(&com_status);       /* ���ʃX�e�[�^�X�擾            */
    if(com_status.ae == DMA_CPU_ADR_ERR){   /* ���ڽ�װ�����������ꍇ        */
        return(DMA_CPU_FAIL);               /* �ُ�I�������^�[��            */
    }
    status = DMA_CpuGetStatus(DMA_CPU_CH0); /* �X�e�[�^�X�擾                */
    if(status == DMA_CPU_TE_MV){            /* ���쒆�ł���ꍇ              */
        return(DMA_CPU_BUSY);               /* ���s�������^�[��              */
    }
    CSH_Purge(dma_cpu_dis_adr, dma_cpu_cnt);
    return(DMA_CPU_END);                    /* ����I�������^�[��            */
}

