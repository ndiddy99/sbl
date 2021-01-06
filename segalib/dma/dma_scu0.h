/*-----------------------------------------------------------------------------
 *  FILE: dma_scu0.h
 *
 *      Copyright(c) 1994 SEGA.
 *
 *  PURPOSE:
 *
 *      SCU DMA�ᐅ���w�b�_�t�@�C���B
 *
 *  DESCRIPTION:
 *
 *      SCU DMA�ᐅ���]���@�\�B
 *
 *  AUTHOR(S)
 *
 *      1994-05-09  N.T Ver.0.90
 *
 *  MOD HISTORY:
 *
 *      1994-10-12  N.T Ver.1.01
 *
 *-----------------------------------------------------------------------------
 */

#ifndef DMA_SCU0_H
#define DMA_SCU0_H

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
#include "sega_xpt.h"

/*
 * GLOBAL DEFINES/MACROS DEFINES
 */

/* �萔 */
/*****************************************************************************/
/**** DMA�`���l�� ************************************************************/
/*****************************************************************************/
#define DMA_SCU_CH0 0x0                         /* �`���l��0                 */
#define DMA_SCU_CH1 0x1                         /* �`���l��1                 */
#define DMA_SCU_CH2 0x2                         /* �`���l��2                 */

/**** DMA���W�X�^�r�b�g�ʒu **************************************************/
/******** ���Z�l���W�X�^ *****************************************************/
#define DMA_DXAD_DXRAD  8                       /* �ǂݍ��݃A�h���X���Z�l    */
#define DMA_DXAD_DXWAD  0                       /* �������݃A�h���X���Z�l    */
/******** DMA�����W�X�^ ****************************************************/
#define DMA_DXEN_DXGO   0                       /* DMA�N���r�b�g             */
#define DMA_DXEN_DXEN   8                       /* DMA���r�b�g             */
/******** DMA���[�h�A�A�h���X�X�V�A�N���v�����W�X�^ **************************/
#define DMA_DXMD_DXMOD  24                      /* ���[�h�r�b�g              */
#define DMA_DXMD_DXRUP  16                      /* �ǂݍ��݃A�h���X�X�V�r�b�g*/
#define DMA_DXMD_DXWUP   8                      /* �����o���A�h���X�X�V�r�b�g*/
#define DMA_DXMD_DXFT    0                      /* �N���v���I���r�b�g        */


/*****************************************************************************/
/**** (DmaScuPrm)�]���p�����[�^ **********************************************/
/*****************************************************************************/
/******** (dxad_r)�ǂݍ��݃A�h���X���Z�l *************************************/
#define DMA_SCU_R0  (0x0 << DMA_DXAD_DXRAD)     /* ���Z���Ȃ�                */
#define DMA_SCU_R4  (0x1 << DMA_DXAD_DXRAD)     /* 4�o�C�g���Z����           */
/******** (dxad_w)�������݃A�h���X���Z�l *************************************/
#define DMA_SCU_W0     (0x0 << DMA_DXAD_DXWAD)  /* ���Z���Ȃ�                */
#define DMA_SCU_W2     (0x1 << DMA_DXAD_DXWAD)  /* 2�o�C�g���Z����           */
#define DMA_SCU_W4     (0x2 << DMA_DXAD_DXWAD)  /* 4�o�C�g���Z����           */
#define DMA_SCU_W8     (0x3 << DMA_DXAD_DXWAD)  /* 8�o�C�g���Z����           */
#define DMA_SCU_W16    (0x4 << DMA_DXAD_DXWAD)  /* 16�o�C�g���Z����          */
#define DMA_SCU_W32    (0x5 << DMA_DXAD_DXWAD)  /* 32�o�C�g���Z����          */
#define DMA_SCU_W64    (0x6 << DMA_DXAD_DXWAD)  /* 64�o�C�g���Z����          */
#define DMA_SCU_W128   (0x7 << DMA_DXAD_DXWAD)  /* 128�o�C�g���Z����         */
/******** (dxmod)���[�h�r�b�g ************************************************/
#define DMA_SCU_DIR    (0x0 << DMA_DXMD_DXMOD)  /* ���ڃ��[�h                */
#define DMA_SCU_IN_DIR (0x1 << DMA_DXMD_DXMOD)  /* �Ԑڃ��[�h                */
/******** (dxrup)�ǂݍ��݃A�h���X�X�V�r�b�g **********************************/
/******** (dxwup)�����o���A�h���X�X�V�r�b�g **********************************/
#define DMA_SCU_KEEP    0x0                     /* �ێ�����                  */
#define DMA_SCU_REN     0x1                     /* �X�V����                  */

/******** (dxft)�N���v���I���r�b�g *******************************************/
#define DMA_SCU_F_VBLK_IN  (0x0 << DMA_DXMD_DXFT)/* V-�u�����N-IN            */
#define DMA_SCU_F_VBLK_OUT (0x1 << DMA_DXMD_DXFT)/* V-�u�����N-OUT           */
#define DMA_SCU_F_HBLK_IN  (0x2 << DMA_DXMD_DXFT)/* H-�u�����N-IN            */
#define DMA_SCU_F_TIM0     (0x3 << DMA_DXMD_DXFT)/* �^�C�}0                  */
#define DMA_SCU_F_TIM1     (0x4 << DMA_DXMD_DXFT)/* �^�C�}1                  */
#define DMA_SCU_F_SND      (0x5 << DMA_DXMD_DXFT)/* �T�E���h-Req             */
#define DMA_SCU_F_SPR      (0x6 << DMA_DXMD_DXFT)/* �X�v���C�g�`��           */
#define DMA_SCU_F_DMA      (0x7 << DMA_DXMD_DXFT)/* DMA�N���v���r�b�g�̃Z�b�g*/
/******** (msk)�}�X�N�r�b�g *************************************************/
#define DMA_SCU_M_DXR      (1 << 0)             /* �ǂݍ��݃A�h���X          */
#define DMA_SCU_M_DXW      (1 << 1)             /* �����o���A�h���X          */

/*****************************************************************************/
/**** (DmaScuStatus)�X�e�[�^�X ***********************************************/
/*****************************************************************************/
/******** (dxbkr)DMA���쒆�t���O *********************************************/
/******** (ddmv)DSP��DMA���쒆�t���O *****************************************/
#define DMA_SCU_MV      0x1                     /* ���쒆                    */
#define DMA_SCU_NO_MV   0x0                     /* ���쒆�łȂ�              */

/*
 * STRUCTURE DECLARATIONS
 */

/*
 * TYPEDEFS
 */
typedef struct {
    Uint32 dxr;                                 /* �ǂݍ��݃A�h���X          */
    Uint32 dxw;                                 /* �����o���A�h���X          */
    Uint32 dxc;                                 /* �]���o�C�g��              */
    Uint32 dxad_r;                              /* �ǂݍ��݃A�h���X���Z�l    */
    Uint32 dxad_w;                              /* �������݃A�h���X���Z�l    */
    Uint32 dxmod;                               /* ���[�h�r�b�g              */
    Uint32 dxrup;                               /* �ǂݍ��݃A�h���X�X�V�r�b�g*/
    Uint32 dxwup;                               /* �������݃A�h���X�X�V�r�b�g*/
    Uint32 dxft;                                /* �N���v���I���r�b�g        */
    Uint32 msk;                                 /* �}�X�N�r�b�g              */
}DmaScuPrm;                                     /* �]���p�����[�^            */

typedef struct {
    Uint32 dxmv;                            /* DMA���쒆�t���O               */
}DmaScuStatus;                              /* �X�e�[�^�X                    */

/*
 * EXTERNAL VARIABLE DECLARATIONS
 */

/*
 * EXTERNAL FUNCTION PROTOTYPE  DECLARATIONS
 */
void DMA_ScuSetPrm(DmaScuPrm *, Uint32);    /* DMA�]���p�����[�^             */
void DMA_ScuStart(Uint32);                  /* DMA�]���J�n                   */
void DMA_ScuGetStatus(DmaScuStatus *, Uint32);/* DMA�X�e�[�^�X�擾           */

#endif  /* ifndef DMA_SCU0_H */
