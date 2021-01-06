/*-----------------------------------------------------------------------------
 *  FILE: dma_scu0.c
 *
 *      Copyright(c) 1994 SEGA.
 *
 *  PURPOSE:
 *
 *      SCU DMA�ᐅ���B
 *
 *  DESCRIPTION:
 *
 *      SCU DMA�ᐅ���]���@�\�B
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *          (1) DMA_ScuSetPrm           -   DMA�]���p�����[�^�ݒ�
 *          (2) DMA_ScuStart            -   DMA�]���J�n
 *          (3) DMA_ScuGetStatus        -   DMA�X�e�[�^�X�擾
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
 *      1994-10-12  N.T Ver.1.04
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
#include "sega_int.h"
#include <machine.h>

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include "dma_xpt.h"
#include "dma_scu0.h"

/*
 * GLOBAL DECLARATIONS
 */

/*
 * LOCAL DEFINES/MACROS
 */

/* �萔 */
/**** ���� *******************************************************************/
#define MAX_CHANEL  3                       /* DMA�`���l����                 */
/**** DMA���W�X�^�A�h���X ****************************************************/
/******** �`���l��0 **********************************************************/
#ifndef _DEB
#define REG_DXR     ((volatile Uint32 *)0x25fe0000)  /* �ǂݍ��݃A�h���X              */
#define REG_DXW     ((volatile Uint32 *)0x25fe0004)  /* �����o���A�h���X              */
#define REG_DXC     ((volatile Uint32 *)0x25fe0008)  /* �]���o�C�g���@�@�@            */
#define REG_DXAD    ((volatile Uint32 *)0x25fe000C)  /* ���Z�l���W�X�^�@�@            */
#define REG_DXEN    ((volatile Uint32 *)0x25fe0010)  /* DMA�����W�X�^ �@            */
#define REG_DXMD    ((volatile Uint32 *)0x25fe0014)  /* DMAӰ�ށA���ڽ�X�V�A�N���v���@*/
#else
#define REG_DXR   ((Uint32 *)0x06060000)  /* �ǂݍ��݃A�h���X              */
#define REG_DXW   ((Uint32 *)0x06060004)  /* �����o���A�h���X              */
#define REG_DXC   ((Uint32 *)0x06060008)  /* �]���o�C�g���@�@�@            */
#define REG_DXAD  ((Uint32 *)0x0606000C)  /* ���Z�l���W�X�^�@�@            */
#define REG_DXEN  ((Uint32 *)0x06060010)  /* DMA�����W�X�^ �@            */
#define REG_DXMD  ((Uint32 *)0x06060014)  /* DMAӰ�ށA���ڽ�X�V�A�N���v���@*/
#endif /* _DEB */
/******** �`���l���I�t�Z�b�g *************************************************/
#define OFSET0_REG  0x00000008              /* REG_DXR   �I�t�Z�b�g          */
                                            /* REG_DXW                       */
                                            /* REG_DXC                       */
                                            /* REG_DXAD                      */
                                            /* REG_DXEN                      */
                                            /* REG_DXMD                      */
/******** ���� ***************************************************************/
#ifndef _DEB
#define REG_DSTP    ((volatile Uint32 *)0x25fe0060)  /* DMA������~���W�X�^         �@*/
#else
#define REG_DSTP   ((Uint32 *)0x06060060)  /* DMA������~���W�X�^         �@*/
#endif /* _DEB */

/***** DMA���W�X�^�}�X�N�r�b�g ***********************************************/
/******** ���Z�l���W�X�^ *****************************************************/
#define R_DXAD_DXRAD (1 << DMA_DXAD_DXRAD)      /* �ǂݍ��݃A�h���X���Z�l    */
#define R_DXAD_DXWAD (3 << DMA_DXAD_DXWAD)      /* �������݃A�h���X���Z�l    */
/******** DMA�����W�X�^ ****************************************************/
#define R_DXEN_DXGO  (1 << DMA_DXEN_DXGO)       /* DMA�N���r�b�g             */
#define R_DXEN_DXEN  (1 << DMA_DXEN_DXEN)       /* DMA���r�b�g             */
/******** DMA���[�h�A�A�h���X�X�V�A�N���v�����W�X�^ **************************/
#define R_DXMD_DXMOD ( 1 << DMA_DXMD_DXMOD)     /* ���[�h�r�b�g              */
#define R_DXMD_DXRUP ( 1 << DMA_DXMD_DXRUP)     /* �ǂݍ��݃A�h���X�X�V�r�b�g*/
#define R_DXMD_DXWUP ( 1 << DMA_DXMD_DXWUP)     /* �����o���A�h���X�X�V�r�b�g*/
#define R_DXMD_DXFT  ( 7 << DMA_DXMD_DXFT )     /* �N���v���I���r�b�g        */

/******************************************************************************
 *
 * NAME:    STOP_COM()  -   DMA�I�����ʏ���
 *
 * PARAMETERS :
 *      (1) Uint32  ch  -   <i> DMA�`���l��.
 *
 * DESCRIPTION:
 *      �w�肳�ꂽDMA�`���l����DMA�I���������s���܂��B
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
#define STOP_COM(ch, vector_num)\
    do{\
        INT_SetScuFunc((vector_num), dma_scu_vector[(ch)]);\
        INT_SetFunc((vector_num), dma_vector[(ch)]);\
        dma_int_status[(ch)] = DMA_SCU_NO_MV;\
    }while(FALSE)

/******************************************************************************
 *
 * NAME:    STOP_COM_CHANEL()   -   DMA�`���l���ʏI�����ʏ���
 *
 * PARAMETERS :
 *      (1) Uint32  ch  -   <i> DMA�`���l��.
 *
 * DESCRIPTION:
 *      �w�肳�ꂽDMA�`���l����DMA�I���������s���܂��B
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
#define STOP_COM_CHANEL(ch, vector_num)\
    do{\
        if(dma_int_status[(ch)] == DMA_SCU_MV){\
            switch((ch)){\
                case DMA_SCU_CH0:\
                    *(vector_num) = INT_SCU_DMA0;\
                    break;\
                case DMA_SCU_CH1:\
                    *(vector_num) = INT_SCU_DMA1;\
                    break;\
                case DMA_SCU_CH2:\
                    *(vector_num) = INT_SCU_DMA2;\
                    break;\
            }\
            STOP_COM((ch), *(vector_num));\
        }\
    }while(FALSE)\

/******************************************************************************
 *
 * NAME:    INTR_FUNC()         -   DMA�`���l���ʏI�����荞�ݏ���
 *
 * PARAMETERS :
 *      ch �`���l��
 *      vector_num �x�N�^�ԍ�
 *
 * DESCRIPTION:
 *      DMA�`���l����DMA�I�����荞�ݏ������s���܂��B
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
#define INTR_FUNC(ch, vector_num)\
    do{\
        Uint32 msk;\
        msk = get_imask();\
        set_imask(15);\
        dma_int_status[(ch)] = DMA_SCU_NO_MV;\
        STOP_COM((ch), (vector_num));           /* DMA�I�����ʏ���          */\
        set_imask(msk);                                     /* ���荞��POP  */\
	}while(FALSE)

/*
 * STATIC DECLARATIONS
 */

static void (*dma_scu_vector[MAX_CHANEL])(void);/* �x�N�^�L���̈�(SCU)       */
static void (*dma_vector[MAX_CHANEL])(void);    /* �x�N�^�L���̈�            */

static Uint8 dma_int_status[MAX_CHANEL];        /* ���荞�݃X�e�[�^�X        */

/*
 * STATIC FUNCTION PROTOTYPE DECLARATIONS
 */

void DMA_ScuIntFunc0(void);
void DMA_ScuIntFunc1(void);
void DMA_ScuIntFunc2(void);

/******************************************************************************
 *
 * NAME:    DMA_ScuSetPrm() -   DMA�]���p�����[�^�ݒ�
 *
 * PARAMETERS :
 *      (1) DmaScuPrm   *prm  - <i>   �]���p�����[�^.
 *      (2) Uint32      ch    - <i>   DMA�`���l��..
 *
 * DESCRIPTION:
 *      �w�肳�ꂽDMA�`���l���֓]���p�����[�^�̒l��ݒ肵�܂��B
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

void DMA_ScuSetPrm(DmaScuPrm *prm, Uint32 ch)
{
    Uint32 work_reg;
    
    if((prm->msk & DMA_SCU_M_DXR) == DMA_SCU_M_DXR){
        DMA_POKE_L((REG_DXR + OFSET0_REG * ch), prm->dxr);
    }

    if((prm->msk & DMA_SCU_M_DXW) == DMA_SCU_M_DXW){
        DMA_POKE_L((REG_DXW + OFSET0_REG * ch), prm->dxw);
    }

    DMA_POKE_L((REG_DXC + OFSET0_REG * ch), prm->dxc);
    
    DMA_POKE_L((REG_DXAD + OFSET0_REG * ch), (prm->dxad_r | prm->dxad_w));
                                                        /* ���Z�l���ڽڼ޽�  */
    DMA_POKE_L((REG_DXMD + OFSET0_REG * ch), (prm->dxmod | 
               (prm->dxrup << DMA_DXMD_DXRUP )|
               (prm->dxwup << DMA_DXMD_DXWUP )| prm->dxft));
                                          /* ���[�h�A�A�h���X�X�V�A�N���v��  */
}

/******************************************************************************
 *
 * NAME:    DMA_ScuStart()  -   DMA�]���J�n
 *
 * PARAMETERS :
 *      (1) Uint32  ch  -   <i> DMA�`���l��.
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

void DMA_ScuStart(Uint32 ch)
{
    Uint32 msk;
    Uint32 now_vector_num;
    void (*intr_func)(void);

    msk = get_imask();
    set_imask(15);
    /* ���荞�݃x�N�^�o�^ (start)*********************************************/
    switch(ch){
        case DMA_SCU_CH0:
            now_vector_num = INT_SCU_DMA0;
            intr_func = DMA_ScuIntFunc0;
            break;
        case DMA_SCU_CH1:
            now_vector_num = INT_SCU_DMA1;
            intr_func = DMA_ScuIntFunc1;
            break;
        case DMA_SCU_CH2:
            now_vector_num = INT_SCU_DMA2;
            intr_func = DMA_ScuIntFunc2;
            break;
    }
    dma_scu_vector[ch] = INT_GetScuFunc(now_vector_num);
                                                        /* �x�N�^�Ҕ�SCU     */
    dma_vector[ch] = INT_GetFunc(now_vector_num);
                                                        /* �x�N�^�Ҕ�        */
    dma_int_status[ch] = DMA_SCU_MV;
    INT_SetScuFunc(now_vector_num, intr_func);        /* �x�N�^�o�^        */
    INT_SetFunc(now_vector_num, NULL);          /* SCU���荞�݊֐��̍ēo�^   */

    /* ���荞�݃x�N�^�o�^ (end)***********************************************/

    DMA_POKE_L((REG_DXEN + OFSET0_REG * ch), (R_DXEN_DXGO | R_DXEN_DXEN));
                                                /* DMA����ڼ޽�              */

    set_imask(msk);                                         /* ���荞��POP   */
}
/******************************************************************************
 *
 * NAME:    DMA_ScuGetStatus() -   DMA�X�e�[�^�X�擾
 *
 * PARAMETERS :
 *      (1) Uint32  ch              -   <i> DMA�`���l��.
 *      (2) DmaScuStatus *status    -   <o> �X�e�[�^�X�|�C���^.
 *
 * DESCRIPTION:
 *      �w�肳�ꂽDMA�`���l���̃X�e�[�^�X���擾���܂��B
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

void DMA_ScuGetStatus(DmaScuStatus *status, Uint32 ch)
{
    Uint32 msk;
    msk = get_imask();
    set_imask(15);

    status->dxmv = (Uint32)dma_int_status[ch];

    set_imask(msk);                                         /* ���荞��POP   */
}

/******************************************************************************
 *
 * NAME:    DMA_ScuIntFunc0()     -   DMA0���荞�ݏ���
 *
 * PARAMETERS :
 *
 * DESCRIPTION:
 *      DMA0�̏I�����荞�݃��[�`���ł��B
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

void DMA_ScuIntFunc0(void){
    INTR_FUNC(DMA_SCU_CH0, INT_SCU_DMA0);   /* DMA�`���l���ʏI�����荞�ݏ��� */
}

/******************************************************************************
 *
 * NAME:    DMA_ScuIntFunc1()     -   DMA1���荞�ݏ���
 *
 * PARAMETERS :
 *
 * DESCRIPTION:
 *      DMA1�̏I�����荞�݃��[�`���ł��B
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

void DMA_ScuIntFunc1(void){
    INTR_FUNC(DMA_SCU_CH1, INT_SCU_DMA1);   /* DMA�`���l���ʏI�����荞�ݏ��� */
}

/******************************************************************************
 *
 * NAME:    DMA_ScuIntFunc2()     -   DMA2���荞�ݏ���
 *
 * PARAMETERS :
 *
 * DESCRIPTION:
 *      DMA2�̏I�����荞�݃��[�`���ł��B
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

void DMA_ScuIntFunc2(void){
    INTR_FUNC(DMA_SCU_CH2, INT_SCU_DMA2);   /* DMA�`���l���ʏI�����荞�ݏ��� */
}
