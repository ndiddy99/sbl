/*-----------------------------------------------------------------------------
 *  FILE: dma_cpu0.h
 *
 *      Copyright(c) 1994 SEGA.
 *
 *  PURPOSE:
 *
 *      CPU DMA�ᐅ���w�b�_�t�@�C���B
 *
 *  DESCRIPTION:
 *
 *      CPU DMA�ᐅ���]���@�\�B
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

/* �萔 */
/**** DMA���W�X�^�r�b�g�ʒu **************************************************/
/******** �`���l���R���g���[�����W�X�^ ***************************************/
#define DMA_CHCR_DE     0               /* DMA�C�l�[�u���r�b�g               */
#define DMA_CHCR_TE     1               /* �g�����X�t�@�G���h�t���O�r�b�g    */
#define DMA_CHCR_IE     2               /* �C���^���v�g�C�l�[�u���r�b�g      */
#define DMA_CHCR_TA     3               /* �g�����X�t�@�A�h���X���[�h�r�b�g  */
#define DMA_CHCR_TB     4               /* �g�����X�t�@�o�X���[�h�r�b�g      */
#define DMA_CHCR_DL     5               /* DREQ���x���r�b�g�@�@              */
#define DMA_CHCR_DS     6               /* DREQ�Z���N�g�r�b�g                */
#define DMA_CHCR_AL     7               /* �A�N�m���b�W���x���r�b�g          */
#define DMA_CHCR_AM     8               /* ���د��/��ݽ̧Ӱ���ޯ�            */
#define DMA_CHCR_AR     9               /* �I�[�g���N�G�X�g�r�b�g            */
#define DMA_CHCR_TS     10              /* �g�����X�t�@�T�C�Y�r�b�g          */
#define DMA_CHCR_SM     12              /* �\�[�X�A�h���X���[�h�r�b�g        */
#define DMA_CHCR_DM     14              /* �ި�èȰ��ݱ��ڽӰ���ޯ�          */
/******** DMA�I�y���[�V�������W�X�^ ******************************************/
#define DMA_DMAOR_DME   0               /* DMA�}�X�^�C�l�[�u���r�b�g         */
#define DMA_DMAOR_NMIF  1               /* NMI�t���O�r�b�g                   */
#define DMA_DMAOR_AE    2               /* �A�h���X�G���[�t���O�r�b�g        */
#define DMA_DMAOR_PR    3               /* �v���C�I���e�B���[�h�r�b�g        */

/*****************************************************************************/
/**** DMA�`���l�� ************************************************************/
/*****************************************************************************/
#define DMA_CPU_CH0 0                           /* �`���l��0                 */
#define DMA_CPU_CH1 1                           /* �`���l��1                 */

/*****************************************************************************/
/**** (DmaCpuComPrm)���ʓ]���p�����[�^ ***************************************/
/*****************************************************************************/
/******** (pr)�v���C�I���e�B���[�h  ******************************************/
#define DMA_CPU_FIX (0 << DMA_DMAOR_PR)         /* �D�揇�ʂ͌Œ�            */
#define DMA_CPU_ROR (1 << DMA_DMAOR_PR)         /* �D�揇�ʂ̓��E���h���r��- */
                                                /* �ɂ��                    */
/******** (dme)DMA�}�X�^�C�l�[�u�� *******************************************/
#define DMA_CPU_DIS     (0 << DMA_DMAOR_DME)    /* �S�`���l����DMA�]�����֎~ */
#define DMA_CPU_ENA     (1 << DMA_DMAOR_DME)    /* �S�`���l����DMA�]�������� */
/******** (msk)�}�X�N�r�b�g **************************************************/
#define DMA_CPU_M_PR    (1 << 0)                /* �v���C�I���e�B���[�h�@�@  */
#define DMA_CPU_M_AE    (1 << 1)                /* �A�h���X�G���[�t���O�@�@  */
#define DMA_CPU_M_NMIF  (1 << 2)                /* NMI�t���O           �@�@  */
#define DMA_CPU_M_DME   (1 << 3)                /* DMA�}�X�^�C�l�[�u�� �@�@  */

/*****************************************************************************/
/**** (DmaCpuPrm)�]���p�����[�^ **********************************************/
/*****************************************************************************/
/******** (dm)�f�B�X�e�B�l�[�V�����A�h���X���[�h�r�b�g ***********************/
/******** (sm)�\�[�X�A�h���X���[�h�r�b�g *************************************/
#define DMA_CPU_AM_NOM  0x0                    /* �Œ�                      */
#define DMA_CPU_AM_ADD  0x1                    /* ����                      */
#define DMA_CPU_AM_SUB  0x2                    /* ����                      */
/******** (ts)�g�����X�t�@�T�C�Y *********************************************/
#define DMA_CPU_1   (0 << DMA_CHCR_TS)          /* �o�C�g�P��                */
#define DMA_CPU_2   (1 << DMA_CHCR_TS)          /* ���[�h(2�o�C�g)�P��       */
#define DMA_CPU_4   (2 << DMA_CHCR_TS)          /* �����O���[�h(4�o�C�g)�P�� */
#define DMA_CPU_16  (3 << DMA_CHCR_TS)          /* 16�o�C�g�]��              */
/******** (ar)�I�[�g���N�G�X�g���[�h *****************************************/
#define DMA_CPU_MOD     (0 << DMA_CHCR_AR)      /* ���W���[�����N�G�X�g      */
#define DMA_CPU_AUTO    (1 << DMA_CHCR_AR)      /* �I�[�g���N�G�X�g          */
/******** (ie)�C���^���v�g�C�l�[�u�� *****************************************/
#define DMA_CPU_INT_ENA (1 << DMA_CHCR_IE)      /* ���荞�ݗv��������        */
#define DMA_CPU_INT_DIS (0 << DMA_CHCR_IE)      /* ���荞�ݗv�����֎~(�����l)*/
/******** (drcr)DMA�v���^�����I�𐧌� ****************************************/
#define DMA_CPU_DREQ    0x0                     /* DREQ(�O�����N�G�X�g)      */
#define DMA_CPU_RXI     0x1                     /* RXI(����SCI�̎�M�f�[�^-  */
                                                /* �t�����荞�ݓ]���v��)     */
#define DMA_CPU_TXI     0x2                     /* TXI(����SCI�̑��M�f�[�^-  */
                                                /* �G���v�e�B���荞�ݓ]���v��*/
/******** (msk)�}�X�N�r�b�g **************************************************/
#define DMA_CPU_M_SAR   (1 << 0)                /* DMA�\�[�X�A�h���X         */
#define DMA_CPU_M_DAR   (1 << 1)                /* DMA�f�B�X�e�B�l�[�V����-  */
                                                /* �A�h���X                  */
#define DMA_CPU_M_TCR   (1 << 2)                /* DMA�g�����X�t�@�J�E���g   */
#define DMA_CPU_M_DM    (1 << 3)                /* DMA�f�B�X�e�B�l�[�V����-  */
                                                /* �A�h���X���[�h�r�b�g      */
#define DMA_CPU_M_SM    (1 << 4)                /* DMA�\�[�X�A�h���X���[�h-  */
                                                /* �r�b�g                    */
#define DMA_CPU_M_TS    (1 << 5)                /* �g�����X�t�@�T�C�Y        */
#define DMA_CPU_M_AR    (1 << 6)                /* �I�[�g���N�G�X�g���[�h    */
#define DMA_CPU_M_IE    (1 << 7)                /* �C���^���v�g�C�l�[�u��    */
#define DMA_CPU_M_DRCR  (1 << 8)                /* DMA�v���^�����I�𐧌�     */
#define DMA_CPU_M_TE    (1 << 9)                /* �g�����X�t�@�G���h�t���O  */

/*****************************************************************************/
/**** (DmaCpuComStatus)���ʃX�e�[�^�X ****************************************/
/*****************************************************************************/
/******** (ae)�A�h���X�G���[�t���O *******************************************/
#define DMA_CPU_ADR_ERR     (1 << DMA_DMAOR_AE) /* DMAC�ɂ��A�h���X�G���[- */
                                                /* ����                      */
#define DMA_CPU_ADR_NO_ERR  (0 << DMA_DMAOR_AE) /* DMAC�ɂ��A�h���X�G���[- */
                                                /* �Ȃ�                      */
                                                
/******** (nmif)NMI�t���O ****************************************************/
#define DMA_CPU_NMI_ON  (1 << DMA_DMAOR_NMIF)   /* NMI���荞�ݔ���           */
#define DMA_CPU_NMI_OFF (0 << DMA_DMAOR_NMIF)   /* NMI���荞�݂Ȃ�           */

/*****************************************************************************/
/**** (DmaCpuComStatus)���ʃX�e�[�^�X ****************************************/
/*****************************************************************************/
#define DMA_CPU_TE_MV   (0 << DMA_CHCR_TE)      /* DMA�]�����܂���DMA�]�����f*/
#define DMA_CPU_TE_SP   (1 << DMA_CHCR_TE)      /* DMA�]������I��           */

/*
 * STRUCTURE DECLARATIONS
 */

/*
 * TYPEDEFS
 */
typedef struct {
    Uint32 pr;                                  /* �v���C�I���e�B���[�h      */
    Uint32 dme;                                 /* DMA�}�X�^�C�l�[�u��       */
    Uint32 msk;                                 /* �}�X�N�r�b�g              */
}DmaCpuComPrm;                                  /* ���ʓ]���p�����[�^        */

typedef struct {
    Uint32 sar;                                 /* DMA�\�[�X�A�h���X         */
    Uint32 dar;                                 /* DMA�f�B�X�e�B�l�[�V����-  */
                                                /* �A�h���X                  */
    Uint32 tcr;                                 /* DMA�g�����X�t�@�J�E���g   */
    Uint32 dm;                                  /* �f�B�X�e�B�l�[�V�����A�h- */
                                                /* ���X���[�h�r�b�g          */
    Uint32 sm;                                  /* �\�[�X�A�h���X���[�h�r�b�g*/
    Uint32 ts;                                  /* �g�����X�t�@�T�C�Y        */
    Uint32 ar;                                  /* �I�[�g���N�G�X�g���[�h    */
    Uint32 ie;                                  /* �C���^���v�g�C�l�[�u��    */
    Uint32 drcr;                                /* DMA�v���^�����I�𐧌�     */
    Uint32 msk;                                 /* �}�X�N�r�b�g              */
}DmaCpuPrm;                                     /* �]���p�����[�^            */

typedef struct {
    Uint32 ae;                                  /* �A�h���X�G���[�t���O      */
    Uint32 nmif;                                /* NMI�t���O                 */
}DmaCpuComStatus;                               /* ���ʃX�e�[�^�X            */

typedef Uint32 DmaCpuStatus;                    /* �X�e�[�^�X                */

/*
 * EXTERNAL VARIABLE DECLARATIONS
 */

/*
 * EXTERNAL FUNCTION PROTOTYPE  DECLARATIONS
 */
void DMA_CpuSetComPrm(DmaCpuComPrm *);          /* DMA���ʓ]���p�����[�^�ݒ� */
void DMA_CpuSetPrm(DmaCpuPrm *, Uint32);        /* DMA�]���p�����[�^�ݒ�     */
void DMA_CpuStart(Uint32);                      /* DMA�]���J�n               */
void DMA_CpuStop(Uint32);                       /* DMA�]�����~               */
void DMA_CpuAllStop(void);                      /* DMA�S�`���l���]�����~     */
void DMA_CpuGetComStatus(DmaCpuComStatus *);    /* DMA���ʃX�e�[�^�X�擾     */
DmaCpuStatus DMA_CpuGetStatus(Uint32);          /* DMA�X�e�[�^�X�擾         */

#endif  /* ifndef DMA_CPU0_H */
