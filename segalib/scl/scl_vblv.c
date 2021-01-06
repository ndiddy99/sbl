/*----------------------------------------------------------------------------
 *  SCL_vblv.c -- SCL ���C�u���� VDP ���W���[��
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-02-15 Ver.0.80
 *  Updated by K.M on 1994-11-11 Ver.1.00
 *
 *  ���̃��C�u�����͂u�c�o�p�̂u�|�a�k�`�m�j���荞�ݏ������[�`���ŁA
 *  �ȉ��̃��[�`�����܂ށB
 *
 *  SCL_SetFrameInterval  -  �t���[���\���C���^�[�o�����̃Z�b�g
 *  SCL_DisplayFrame      -  �X�v���C�g�A�X�N���[���̕\��
 *  SCL_VblInit           -  �u�|�a�k�`�m�j���荞�ݏ������[�N�̏�����
 *  SCL_VblankStart       -  �u�|�a�k�`�m�j�J�n���荞�ݏ������[�`��
 *  SCL_VblankEnd         -  �u�|�a�k�`�m�j�I�����荞�ݏ������[�`��
 *
 *----------------------------------------------------------------------------
 */

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include <machine.h>
#include "sega_xpt.h"
#include "sega_def.h"
#include "sega_mth.h"
#define SEGA_SCL_PROTO
#include "sega_scl.h"
#include "sega_spr.h"


#if 0
/*
 * STATIC DECLARATIONS
 */
static Uint16  SpFrameChgMode    = AUTO_FRAME_CHG; /* Frame Change Mode      */
static Sint32  SpFrameEraseMode  = 1;       /* Frame Erase  Mode             */
static Sint32  VBInterval        = 1;   /* V-Blank Interval for Frame Change */
static Sint32  VBIntervalCounter = 0;   /* V-Blank Interval Counter          */
static Sint32  ReqDisplayFlag1    = 0;   /* Display Request flag              */
static Sint32  ReqDisplayFlag2    = 0;   /* Display Request flag              */
static Sint32  VBIntrDisableDepth = 0;  /* V-Blank Disable Depth             */
static Sint32  frameChgFlag;
#else
/*
 * GLOBAL DECLARATIONS
 */
Uint16  SpFrameChgMode    = AUTO_FRAME_CHG; /* Frame Change Mode      */
Sint32  SpFrameEraseMode  = 1;       /* Frame Erase  Mode             */
Sint32  VBInterval        = 1;   /* V-Blank Interval for Frame Change */
Sint32  VBIntervalCounter = 0;   /* V-Blank Interval Counter          */
volatile Sint32  ReqDisplayFlag1    = 0;   /* Display Request flag              */
volatile Sint32  ReqDisplayFlag2    = 0;   /* Display Request flag              */
Sint32  VBIntrDisableDepth = 0;  /* V-Blank Disable Depth             */
Sint32  frameChgFlag;
#endif

extern Sint32  SpInitialFlag;
extern void   SCL_ScrollShow(void);


/*****************************************************************************
 *
 * NAME:  SCL_SetFrameInterval()  - Set Frame Interval Count
 *
 * PARAMETERS :
 *
 *     (1) Uint16  count       - <i> �u�|�a�k�`�m�j�C���^�[�o���J�E���g��
 *                                0      = �X�v���C�g�̃t���[���؂�ւ����I�[�g
 *                                         �ɂ��āA�C���^�[�o�����P�ɂ���
 *                                         SCL_DisplayFrame()�ɂ�铯���Ȃ�
 *                                1      = �X�v���C�g�̃t���[���؂�ւ����I�[�g
 *                                         �ɂ��āA�C���^�[�o�����P�ɂ���
 *                                         SCL_DisplayFrame()�ɂ�铯������
 *                                0xffff = �C���^�[�o���𖳌��ɂ��ăt���[���؂�
 *                                         �ւ��v�����ɑ��؂�ւ���
 *                                0xfffe = �C���^�[�o���𖳌��ɂ��ăt���[���؂�
 *                                         �ւ��v�����ɂu�u�����N�C���[�X���s
 *                                         ���đ��؂�ւ���
 *                                ���̑� = �w��C���^�[�o���Ńt���[���؂�ւ�
 *                                         ���s��
 *                                         b15 = 0 : �C���[�X���C�g���s��
 *                                             = 1 : �C���[�X���C�g���s��Ȃ�
 *
 * DESCRIPTION:
 *
 *     �t���[���\���̂u�|�a�k�`�m�j�C���^�[�o���J�E���g����ݒ肷��B
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *
 *****************************************************************************
 */
void SCL_SetFrameInterval(Uint16 count)
{
    /** BEGIN ***************************************************************/
    if(count == 0xffff) {
	SpFrameChgMode = NO_INTER_FRAME_CHG;
        SpFrameEraseMode = 0;          /* Frame Erase Mode Off              */
        SPR_WRITE_REG(SPR_W_FBCR, SPR_FBCR_MANUAL | SpFbcrMode);
                                       /* set frame change to manual mode   */
        VBInterval = count;            /* V-Blank Interval for Frame Change */
    } else
    if(count == 0xfffe) {
	SpFrameChgMode = NO_INTER_VBE_FRAME_CHG;
        SpFrameEraseMode = 0;          /* Frame Erase Mode Off              */
        SPR_WRITE_REG(SPR_W_FBCR, SPR_FBCR_MANUAL | SpFbcrMode);
                                       /* set frame change to manual mode   */
        VBInterval = count;            /* V-Blank Interval for Frame Change */
    } else {
        if(count & 0x8000)
            SpFrameEraseMode = 0;      /* Frame Erase Mode Off              */
        else
            SpFrameEraseMode = 1;      /* Frame Erase Mode On               */
        count &= 0x7fff;
        if(count)
            if(count == 1)
	        SpFrameChgMode = AUTO_FRAME_CHG;
	    else
	        SpFrameChgMode = MANUAL_FRAME_CHG;
        else
	    SpFrameChgMode = AUTO_FRAME_CHG;
        VBInterval = count;            /* V-Blank Interval for Frame Change */
        if(SpFrameChgMode == AUTO_FRAME_CHG)
            SPR_WRITE_REG(SPR_W_FBCR, SPR_FBCR_AUTO | SpFbcrMode);
                                       /* set frame change to auto mode     */
        else
            SPR_WRITE_REG(SPR_W_FBCR, SPR_FBCR_MANUAL | SpFbcrMode);
                                       /* set frame change to manual mode   */
    }
}


/*****************************************************************************
 *
 * NAME:  SCL_DisplayFrame()  - Display Frame
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 *
 * DESCRIPTION:
 *
 *     �t���[���\���v���t���O���I���ɂ��āA�u�|�a�k�`�m�j���荞�݃��[�`��
 *     �ɂ��X�v���C�g����уX�N���[���̃t���[���\��������҂B
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *
 *****************************************************************************
 */
void SCL_DisplayFrame(void)
{

    /** BEGIN ***************************************************************/
    if(VBInterval) {
        ReqDisplayFlag1 = 1;
        while(ReqDisplayFlag1);
        while(ReqDisplayFlag2);
    }
}


/*****************************************************************************
 *
 * NAME:  SCL_VblInit()  - Initial VDP V-Blank Routine
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     �u�c�o �u�|�a�k�`�m�j���荞�݃��[�`���p���[�N�G���A�̏������B
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *
 *****************************************************************************
 */
void SCL_VblInit(void)
{

    /** BEGIN ***************************************************************/
	SpFrameChgMode     = AUTO_FRAME_CHG;
	SpFrameEraseMode   = 1;
	VBInterval         = 0;
	VBIntervalCounter  = 0;
	ReqDisplayFlag1    = 0;
	ReqDisplayFlag2    = 0;
	VBIntrDisableDepth = 0;
	frameChgFlag       = 0;
}


/*****************************************************************************
 *
 * NAME:  SCL_VblankStart()  - V-Blank Start Interrupt Routine for VDP
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 *
 * DESCRIPTION:
 *
 *     �u�|�a���������X�^�[�g���荞�ݏ����Ƃ��Ĉȉ��̏������s��
 *     (1) �X�v���C�g�t���[���o�b�t�@�̃C���[�Y����
 *     (2) �t���[���\���v���t���O���I���łu�|�a���������C���^�[�o����
 *         �ɒB�����ꍇ�A�X�N���[���p�����[�^�̓]���ƃX�v���C�g�̃t���[��
 *         ��؂�ւ���
 *
 *     [�{���[�`�����Q�ƁE�ݒ肷��O���[�o���ϐ�]
 *
 *     (1) IntervalCounter -  �u�|�a�����������荞�ݐ��̃J�E���^�B
 *                            VBL_SetFrameInterval() ����і{���[�`�����N���A�B
 *
 *     (2) Interval        -  �t���[���\���C���^�[�o���u�|�a��������
 *                               ���荞�ݐ��B VBL_SetFrameInterval() ���ݒ�B
 *
 *     (3) SpFrameChgMode  -  �X�v���C�g�t���[���`�F���W���[�h
 *                               VBL_SetFrameInterval() ���ݒ�B
 *
 *     (4) ReqDisplayFlag1 -  �t���[���\���v���t���O�B
 *                               DisplayFrame() ���ݒ�B
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *     ���[�U���u�|�a�����������荞�݃��[�`����o�^���ăt���[���\���؂�ւ�
 *     ���s���ꍇ�A�{���荞�݃��[�`�����Q�l�ɍ쐬���邱��
 *
 *****************************************************************************
 */
void SCL_VblankStart(void)
{
    /** BEGIN ***************************************************************/
/*    frameChgFlag = 0;*/
    if(SpFrameChgMode == NO_INTER_FRAME_CHG) {
        if(ReqDisplayFlag1) {
            SCL_ScrollShow();             /* �X�N���[���ւ̃p�����[�^�ݒ�    */
            frameChgFlag = 2;
        }
    } else  if(SpFrameChgMode == NO_INTER_VBE_FRAME_CHG) {
        if(ReqDisplayFlag1) {             /* set V-Blank Erase ON            */
            SPR_WRITE_REG(SPR_W_TVMR, SpTvMode | 0x0008);
            SPR_WRITE_REG(SPR_W_FBCR, SPR_FBCR_MANUAL | SpFbcrMode);
            SCL_ScrollShow();             /* �X�N���[���ւ̃p�����[�^�ݒ�    */
            frameChgFlag = 3;
        }
    } else  if(SpFrameChgMode == AUTO_FRAME_CHG) {
	SCL_ScrollShow();
        ReqDisplayFlag1 = 0;
    }else {
        VBIntervalCounter++;
        if(SpFrameEraseMode == ON) {
            if((VBInterval - 1) <= VBIntervalCounter) {
               SPR_WRITE_REG(SPR_W_FBCR, SPR_FBCR_ERASE | SpFbcrMode);
                                                /* ���t���[���C���[�Y�I��*/
            }
        }
        if(VBInterval <= VBIntervalCounter) {
            if(ReqDisplayFlag1) {
                SCL_ScrollShow();          /* �X�N���[���ւ̃p�����[�^�ݒ�   */
                frameChgFlag = 1;
            }
            VBIntervalCounter = 0;
        }
    }
}


/*****************************************************************************
 *
 * NAME:  SCL_VblankEnd()  - V-Blank End Interrupt Routine for VDP
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 *
 * DESCRIPTION:
 *
 *     �������Ȃ��B
 *
 *
 * POSTCONDITIONS:
 *
 *     No exist.
 *
 * CAVEATS:
 *
 *
 *****************************************************************************
 */
void SCL_VblankEnd(void)
{
	Uint16  tvStat;
    
	/** BEGIN ***************************************************************/
	if(SpDie) {
		tvStat = SPR_SCLREAD_REG(SPR_R_TVSTAT);
		if(tvStat & 2)
			SPR_WRITE_REG(SPR_W_FBCR, 0x000c | SpFbcrMode);
	        else
			SPR_WRITE_REG(SPR_W_FBCR, 0x0008 | SpFbcrMode);
                         /* �X�v���C�g�̃t���[���o�b�t�@�̐؂�ւ� */
			frameChgFlag = 0;
			ReqDisplayFlag1 = 0;
	} else if(frameChgFlag) {
/*		if((frameChgFlag == 2) && (SpInitialFlag != 0))  SPR_WaitDrawEnd();*/
		if((frameChgFlag == 2) && (SpInitialFlag != 0) 
	       	&& ((SPR_READ_REG(SPR_R_EDSR) & 0x0002) == 0) )	ReqDisplayFlag2 = 1;
		else{
			if(frameChgFlag == 3)
				SPR_WRITE_REG(SPR_W_TVMR, SpTvMode);
				/* set V-Blank Erase OFF    */
			else
				SPR_WRITE_REG(SPR_W_FBCR, SPR_FBCR_MANUAL | SpFbcrMode);
	                                  /* �X�v���C�g�̃t���[���o�b�t�@�̐؂�ւ� */
			frameChgFlag = 0;
			ReqDisplayFlag2 = 0;
		}
		ReqDisplayFlag1 = 0;
	}
}

/*  end of file */
