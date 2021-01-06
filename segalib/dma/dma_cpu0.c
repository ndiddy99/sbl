/*-----------------------------------------------------------------------------
 *  FILE: dma_cpu0.c
 *
 *      Copyright(c) 1994 SEGA.
 *
 *  PURPOSE:
 *
 *      CPU DMA�ᐅ���B
 *
 *  DESCRIPTION:
 *
 *      CPU DMA�ᐅ���]���@�\�B
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *          (1) DMA_CpuSetComPrm        -   DMA���ʓ]���p�����[�^�ݒ�
 *          (2) DMA_CpuSetPrm           -   DMA�]���p�����[�^�ݒ�
 *          (3) DMA_CpuStart            -   DMA�]���J�n
 *          (4) DMA_CpuStop             -   DMA�]�����~
 *          (5) DMA_CpuAllStop          -   DMA�S�`���l���]�����~
 *          (6) DMA_CpuGetComStatus     -   DMA���ʃX�e�[�^�X�擾
 *          (7) DMA_CpuGetStatus        -   DMA�X�e�[�^�X�擾
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

/* �萔 */
/**** DMA���W�X�^�A�h���X ****************************************************/
/******** �`���l��0 **********************************************************/
#define REG_SAR     ((Uint32 *)0xffffff80)  /* �\�[�X�A�h���X���W�X�^        */
#define REG_DAR     ((Uint32 *)0xffffff84)  /* �f�B�X�e�B�l�[�V�����A�h���X- */
                                            /* ���W�X�^                      */
#define REG_TCR     ((Uint32 *)0xffffff88)  /* �g�����X�t�@�J�E���g���W�X�^  */
#define REG_CHCR    ((Uint32 *)0xffffff8C)  /* �`���l���R���g���[�����W�X�^  */
#define REG_DRCR    ((Uint8  *)0xfffffe71)  /* DMA�v���^�����I�𐧌䃌�W�X�^ */
/******** �`���l���I�t�Z�b�g *************************************************/
#define OFSET0_REG  0x00000004              /* REG_SAR   �I�t�Z�b�g          */
                                            /* REG_DAR                       */
                                            /* REG_TCR                       */
                                            /* REG_CHCR                      */
#define OFSET1_REG  0x00000001              /* REG_DRCR  �I�t�Z�b�g          */
/******** ���� ***************************************************************/
#define REG_DMAOR   ((Uint32 *)0xffffffb0)  /* DMA�I�y���[�V�������W�X�^     */

/***** DMA���W�X�^�}�X�N�r�b�g ***********************************************/
/******** �`���l���R���g���[�����W�X�^ ***************************************/
#define R_CHCR_DE    (1 << DMA_CHCR_DE)   /* DMA�C�l�[�u���r�b�g             */
#define R_CHCR_TE    (1 << DMA_CHCR_TE)   /* �g�����X�t�@�G���h�t���O�r�b�g  */
#define R_CHCR_IE    (1 << DMA_CHCR_IE)   /* �C���^���v�g�C�l�[�u���r�b�g    */
#define R_CHCR_TA    (1 << DMA_CHCR_TA)   /* �g�����X�t�@�A�h���X���[�h�r�b�g*/
#define R_CHCR_TB    (1 << DMA_CHCR_TB)   /* �g�����X�t�@�o�X���[�h�r�b�g    */
#define R_CHCR_DL    (1 << DMA_CHCR_DL)   /* DREQ���x���r�b�g�@�@            */
#define R_CHCR_DS    (1 << DMA_CHCR_DS)   /* DREQ�Z���N�g�r�b�g              */
#define R_CHCR_AL    (1 << DMA_CHCR_AL)   /* �A�N�m���b�W���x���r�b�g        */
#define R_CHCR_AM    (1 << DMA_CHCR_AM)   /* ���د��/��ݽ̧Ӱ���ޯ�          */
#define R_CHCR_AR    (1 << DMA_CHCR_AR)   /* �I�[�g���N�G�X�g�r�b�g          */
#define R_CHCR_TS    (3 << DMA_CHCR_TS)   /* �g�����X�t�@�T�C�Y�r�b�g        */
#define R_CHCR_SM    (3 << DMA_CHCR_SM)   /* �\�[�X�A�h���X���[�h�r�b�g      */
#define R_CHCR_DM    (3 << DMA_CHCR_DM)   /* �ި�èȰ��ݱ��ڽӰ���ޯ�        */
/******** DMA�I�y���[�V�������W�X�^ ******************************************/
#define R_DMAOR_DME  (1 << DMA_DMAOR_DME) /* DMA�}�X�^�C�l�[�u���r�b�g       */
#define R_DMAOR_NMIF (1 << DMA_DMAOR_NMIF)/* NMI�t���O�r�b�g                 */
#define R_DMAOR_AE   (1 << DMA_DMAOR_AE)  /* �A�h���X�G���[�t���O�r�b�g      */
#define R_DMAOR_PR   (1 << DMA_DMAOR_PR)  /* �v���C�I���e�B���[�h�r�b�g      */

/*
 * STATIC DECLARATIONS
 */

/*
 * STATIC FUNCTION PROTOTYPE DECLARATIONS
 */


/******************************************************************************
 *
 * NAME:    DMA_CpuSetComPrm()  -   DMA���ʓ]���p�����[�^�ݒ�
 *
 * PARAMETERS :
 *      (1) DmaCpuComPrm *com_prm   <i>   ���ʓ]���p�����[�^.
 *
 * DESCRIPTION:
 *      �w�肳�ꂽ���ʓ]���p�����[�^��ݒ肵�܂��B
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

void DMA_CpuSetComPrm(DmaCpuComPrm *com_prm)
{
    Uint32 work_reg;
    work_reg = DMA_PEEK_L(REG_DMAOR);
                                    /* DMA�I�y���[�V�������W�X�^�̓ǂݍ���   */

    if((com_prm->msk & DMA_CPU_M_PR) == DMA_CPU_M_PR){
        work_reg &= ~R_DMAOR_PR;
        work_reg |= com_prm->pr;
                                            /* �v���C�I���e�B���[�h�Z�b�g    */
    }
    if((com_prm->msk & DMA_CPU_M_DME) == DMA_CPU_M_DME){
        work_reg &= ~R_DMAOR_DME;
        work_reg |= com_prm->dme;
                                            /* DMA�}�X�^�C�l�[�u���Z�b�g     */
    }
    if((com_prm->msk & DMA_CPU_M_AE) == DMA_CPU_M_AE){
        
        work_reg &= ~R_DMAOR_AE;            /* �A�h���X�G���[�r�b�g�̃N���A  */
    }
    if((com_prm->msk & DMA_CPU_M_NMIF) == DMA_CPU_M_NMIF){
        work_reg &= ~R_DMAOR_NMIF;              /* NMI�G���[�r�b�g�̃N���A   */
    }
    
    DMA_POKE_L(REG_DMAOR, work_reg);
                                   /* DMA�I�y���[�V�������W�X�^�̏�������    */
}

/******************************************************************************
 *
 * NAME:    DMA_CpuSetPrm() -   DMA�]���p�����[�^�ݒ�
 *
 * PARAMETERS :
 *      (1) DmaCpuPrm *prm      <i>   �]���p�����[�^.
 *      (2) Uint32 ch           <i>   DMA�`���l��.
 *
 * DESCRIPTION:
 *      �w�肳�ꂽDMA�`���l���֓]���p�����[�^��ݒ肵�܂��B
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

void DMA_CpuSetPrm(DmaCpuPrm *prm, Uint32 ch)
{
    Uint32 work_reg;

    if((prm->msk & DMA_CPU_M_SAR) == DMA_CPU_M_SAR){
        DMA_POKE_L((REG_SAR + OFSET0_REG * ch), prm->sar);
                                                /* �\�[�X�A�h���X�Z�b�g      */
    }
    if((prm->msk & DMA_CPU_M_DAR) == DMA_CPU_M_DAR){
        DMA_POKE_L((REG_DAR + OFSET0_REG * ch), prm->dar);
                                       /* �f�B�X�e�B�l�[�V�����A�h���X�Z�b�g */
    }
    if((prm->msk & DMA_CPU_M_TCR) == DMA_CPU_M_TCR){
        DMA_POKE_L((REG_TCR + OFSET0_REG * ch), prm->tcr);
                                       /* �g�����X�t�@�J�E���g�Z�b�g         */
    }


    work_reg = DMA_PEEK_L(REG_CHCR + OFSET0_REG * ch);
                                /* �`���l���R���g���[�����W�X�^�̓ǂݍ���    */
    
    if((prm->msk & DMA_CPU_M_DM) == DMA_CPU_M_DM){
        work_reg &= ~R_CHCR_DM;
        work_reg |= prm->dm << DMA_CHCR_DM;
                           /* �f�B�X�e�B�l�[�V�����A�h���X���[�h�r�b�g�Z�b�g */
    }
    if((prm->msk & DMA_CPU_M_SM) == DMA_CPU_M_SM){
        work_reg &= ~R_CHCR_SM;
        work_reg |= prm->sm << DMA_CHCR_SM;
                                        /* �\�[�X�A�h���X���[�h�r�b�g�Z�b�g  */
    }
    if((prm->msk & DMA_CPU_M_TS) == DMA_CPU_M_TS){
        work_reg &= ~R_CHCR_TS;
        work_reg |= prm->ts;                    /* �g�����X�t�@�T�C�Y�Z�b�g  */
    }
    if((prm->msk & DMA_CPU_M_AR) == DMA_CPU_M_AR){
        work_reg &= ~R_CHCR_AR;
        work_reg |= prm->ar;                /* �I�[�g���N�G�X�g���[�h�Z�b�g  */
    }
    work_reg &= ~R_CHCR_TB;                 /* �g�����X�t�@�o�X���[�h�Z�b�g  */
                                            /* (�T�C�N���X�`�[�����[�h�Œ�)  */
    work_reg &= ~R_CHCR_TA;                 /* ��ݽ̧���ڽӰ���ޯľ�ā@�@�@�@*/
                                            /* (�f���A���A�h���X���[�h�Œ�)  */
    if((prm->msk & DMA_CPU_M_IE) == DMA_CPU_M_IE){
        work_reg &= ~R_CHCR_IE;             
        work_reg |= prm->ie;                /* �C���^���v�g�C�l�[�u���Z�b�g  */
    }
    if((prm->msk & DMA_CPU_M_TE) == DMA_CPU_M_TE){
        work_reg &= ~R_CHCR_TE;             /* ��ݽ̧�����׸ނ̸ر           */
    }
    DMA_POKE_L((REG_CHCR + OFSET0_REG * ch), work_reg);
                                /* �`���l���R���g���[�����W�X�^�̏�������    */

    if((prm->msk & DMA_CPU_M_DRCR) == DMA_CPU_M_DRCR){
        DMA_POKE_B((REG_DRCR + OFSET1_REG * ch), prm->drcr);
                                /* DMA�v���^�����I�𐧌䃌�W�X�^�̏�������   */
    }
}

/******************************************************************************
 *
 * NAME:    DMA_CpuStart()  -   DMA�]���J�n
 *
 * PARAMETERS :
 *      (1) Uint32 ch           <i>   DMA�`���l��.
 *
 * DESCRIPTION:
 *      �w�肳�ꂽDMA�`���l����DMA�]�����J�n���܂��B
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

void DMA_CpuStart(Uint32 ch)
{
    Uint32 work_reg;

    work_reg = DMA_PEEK_L(REG_CHCR + OFSET0_REG * ch);
                                /* �`���l���R���g���[�����W�X�^�̓ǂݍ���    */
    work_reg |= R_CHCR_DE;      /* DMA�C�l�[�u���r�b�g������                 */
    DMA_POKE_L((REG_CHCR + OFSET0_REG * ch), work_reg);
                                /* �`���l���R���g���[�����W�X�^�̏�������    */
}

/******************************************************************************
 *
 * NAME:    DMA_CpuStop()   -   DMA�]�����~
 *
 * PARAMETERS :
 *      (1) Uint32 ch           <i>   DMA�`���l��.
 *
 * DESCRIPTION:
 *      �w�肳�ꂽDMA�`���l����DMA�]���𒆎~���܂��B
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

void DMA_CpuStop(Uint32 ch)
{
    Uint32 work_reg;

    work_reg = DMA_PEEK_L(REG_CHCR + OFSET0_REG * ch);
                                /* �`���l���R���g���[�����W�X�^�̓ǂݍ���    */
    work_reg &= ~R_CHCR_DE;     /* DMA�C�l�[�u���r�b�g���֎~                 */
    DMA_POKE_L((REG_CHCR + OFSET0_REG * ch), work_reg);
                                /* �`���l���R���g���[�����W�X�^�̏�������    */
}

/******************************************************************************
 *
 * NAME:    DMA_CpuAllStop()    -   DMA�S�`���l���]�����~
 *
 * PARAMETERS :
 *      �Ȃ��B
 *
 * DESCRIPTION:
 *      �S�`���l����DMA�]���𒆎~���܂��B
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

void DMA_CpuAllStop(void)
{
    Uint32 work_reg;
    work_reg = DMA_PEEK_L(REG_DMAOR);
                                    /* DMA�I�y���[�V�������W�X�^�̓ǂݍ���   */
    work_reg &= ~R_DMAOR_DME;       /* DMA�}�X�^�C�l�[�u���r�b�g���֎~       */
    DMA_POKE_L(REG_DMAOR, work_reg);
                                   /* DMA�I�y���[�V�������W�X�^�̏�������    */
}

/******************************************************************************
 *
 * NAME:    DMA_CpuGetComStatus()   -   DMA���ʃX�e�[�^�X�擾
 *
 * PARAMETERS :
 *      (1) DmaCpuComStatus *status     <o>   ���ʃX�e�[�^�X�|�C���^.
 *
 * DESCRIPTION:
 *      ���ʃX�e�[�^�X���擾���܂��B
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

void DMA_CpuGetComStatus(DmaCpuComStatus *status)
{
    Uint32 work_reg;
    
    work_reg = DMA_PEEK_L(REG_DMAOR);
                                    /* DMA�I�y���[�V�������W�X�^�̓ǂݍ���   */
    status->ae = work_reg & R_DMAOR_AE; /* ���ڽӰ�޴װ�׸ނ̎擾            */
    status->nmif = work_reg & R_DMAOR_NMIF; /* NMI�t���O�̎擾               */
}

/******************************************************************************
 *
 * NAME:    DMA_CpuGetStatus()  -   DMA�X�e�[�^�X�擾
 *
 * PARAMETERS :
 *      (1) Uint32 ch       <o>   DMA�`���l��.
 *
 * DESCRIPTION:
 *      �X�e�[�^�X���擾���܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      (1) DmaCpuStatus    <o>   �X�e�[�^�X�|�C���^.
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */

DmaCpuStatus DMA_CpuGetStatus(Uint32 ch)
{
    Uint32 work_reg;
    
    work_reg = DMA_PEEK_L(REG_CHCR + OFSET0_REG * ch);
                                        /* DMA���ٺ��۰�ڼ޽��̓ǂݍ���      */
    return(work_reg & R_CHCR_TE);       /* ��ݽ̧�����׸ނ̎擾              */
}
