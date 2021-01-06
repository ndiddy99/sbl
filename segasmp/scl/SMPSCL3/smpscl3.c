/*------------------------------------------------------------------------
 *  FILE:	SmpScl3.c
 *
 *      Copyright (c) by SEGA Enterprises Ltd. 1994. All rights reserved. 
 *
 *  PURPOSE:
 *		VDPII Test Program
 *
 *  AUTHOR(S):
 *		K.M
 *
 *------------------------------------------------------------------------
 */
#include	<machine.h>
#include	<sega_xpt.h>
#include	<sega_def.h> 
#include	<sega_mth.h> 
#include	<sega_scl.h> 
#include	"botan.h"

void SetAutoLinePara(Uint16 SclNum);
void ClrLinePara(Uint16 SclNum);

/********************************
 * ���������ð���               *
 ********************************/
Uint16	CycleTb[]={
	0x01ff,0x4455,	/* VRAM A (VRAM A0)   1:NBG1�����Ȱ�(ϯ��)ذ�� */
			/*                    0:NBG0�����Ȱ�(ϯ��)ذ�� */
			/*                    5:NBG1��׸������(��)ذ�� */
			/*                    4:NBG0��׸������(��)ذ�� */
			/*                    f:�������Ȃ�             */
	0xffff,0xffff,  /*        (VRAM A1)   �����i�������Ă��Ȃ��j   */
	0xffff,0xffff,  /* VRAM B (VRAM B0)   ���g�p                   */
	0xffff,0xffff   /*        (VRAM B1)   �����i�������Ă��Ȃ��j   */
};

void main()
{
    SclConfig	scfg;
    Uint16	*prnum_addr;
    Uint32	sclnum,Sflag;
    Uint32	sclnum2;
    Uint32	StartAddr2;
    Fixed32	size;
    Uint8	Key;
    Uint8	sw;

    sclnum = SCL_NBG1;
    sclnum2 = SCL_NBG0;

   /*******************************************
    *	�u�c�o�Q���C�u�����̏�����            *
    *******************************************/
    SCL_Vdp2Init();
    SCL_SetFrameInterval(1);

   /*******************************************
    *	�J���[�q�`�l���[�h��1600���F�ɐݒ�    *
    *******************************************/
    SCL_SetColRamMode(SCL_CRM24_1024);

   /*******************************
    *	V_Blank���[�`���̓o�^     *
    *******************************/
    SetVblank();
   /*******************************************
    *	���荞�݃}�X�N������                  *
    *******************************************/
    set_imask(0);
    /*******************************************
    *	�J���[���[�h�ݒ�ׂ̈u�|�h�m�s�҂�     *
    ********************************************/
    SCL_DisplayFrame();

   /*******************************************
    *	�X�N���[���f�[�^�̃Z�b�g              *
    *******************************************/
    FirstData(sclnum);
    StartAddr2 = SecondData(sclnum2);

   /*******************************************
    *	��۰ٺ�̨��ڰ��݂̐ݒ�                *
    *******************************************/
    SCL_InitConfigTb(&scfg);
    scfg.dispenbl      = ON;
    scfg.charsize      = SCL_CHAR_SIZE_1X1;
    scfg.pnamesize     = SCL_PN2WORD;
    scfg.platesize     = SCL_PL_SIZE_1X1;
    scfg.coltype       = SCL_COL_TYPE_256;
    scfg.datatype      = SCL_CELL;
    scfg.plate_addr[0] = SCL_VDP2_VRAM_A1;
    scfg.plate_addr[1] = SCL_VDP2_VRAM_A1;
    scfg.plate_addr[2] = SCL_VDP2_VRAM_A1;
    scfg.plate_addr[3] = SCL_VDP2_VRAM_A1;
    SCL_SetConfig(sclnum, &scfg);

    scfg.plate_addr[0] = StartAddr2;
    scfg.plate_addr[1] = StartAddr2;
    scfg.plate_addr[2] = StartAddr2;
    scfg.plate_addr[3] = StartAddr2;
    SCL_SetConfig(sclnum2, &scfg);

   /*******************************************
    *	�T�C�N���p�^�[���̐ݒ�                *
    *******************************************/
    SCL_SetCycleTable(CycleTb);

   /*******************************************
    *	��ײ��è��ݒ�(0�`7)                  *
    *******************************************/
    SCL_SetPriority(sclnum2,7);
    SCL_SetPriority(sclnum,6);
    SCL_SetPriority(SCL_SPR,0);

    SetAutoLinePara(sclnum);

    SCL_Open(sclnum2);
	SCL_MoveTo(FIXED(0), FIXED(50),0);/* Home Position */
	SCL_Scale(FIXED(1.0), FIXED(1.0));
    SCL_Close();

    SCL_Open(sclnum);
	SCL_MoveTo(FIXED(0), FIXED(0),0);/* Home Position */
	SCL_Scale(FIXED(1.0), FIXED(1.0));
    SCL_Close();
    SCL_DisplayFrame();

    Key=1;
    sw=1;
    Sflag = 0;
    while(1) {
	switch(Key) {
	    case UP_LEFT:
		SCL_Open(sclnum);
		    SCL_Move(FIXED(2), FIXED(2),0);
		SCL_Close();
		SCL_DisplayFrame();
		if(sw==0) Key = 8;
		break;
	    case UP_RIGHT:
		SCL_Open(sclnum);
		    SCL_Move(-FIXED(2), FIXED(2), 0);
		SCL_Close();
		SCL_DisplayFrame();
		if(sw==0) Key = 1;
		break;
	    case UP_BUTTON:
		SCL_Open(sclnum);
		    SCL_Move(0, FIXED(2), 0);
		SCL_Close();
		SCL_DisplayFrame();
		if(sw==0) Key = 9;
		break;
	    case DOWN_LEFT:
		SCL_Open(sclnum);
		    SCL_Move(FIXED(2), -FIXED(2), 0);
		SCL_Close();
		SCL_DisplayFrame();
		if(sw==0) Key = 2;
		break;
	    case DOWN_RIGHT:
		SCL_Open(sclnum);
		    SCL_Move(-FIXED(2), -FIXED(2), 0);
		SCL_Close();
		SCL_DisplayFrame();
		if(sw==0) Key = 4;
		break;
	    case DOWN_BUTTON:
		SCL_Open(sclnum);
		    SCL_Move(0, -FIXED(2), 0);
		SCL_Close();
		SCL_DisplayFrame();
		if(sw==0) Key = 3;
		break;
	    case LEFT_BUTTON:
		SCL_Open(sclnum);
		    SCL_Move(FIXED(2), 0, 0);
		SCL_Close();
		SCL_DisplayFrame();
		if(sw==0) Key = 6;
		break;
	    case RIGHT_BUTTON:
		SCL_Open(sclnum);
		    SCL_Move(-FIXED(2), 0, 0);
		SCL_Close();
		SCL_DisplayFrame();
		if(sw==0) Key = 7;
		break;
	}
	sw++;
    }
}


/********************************************************************
 * ���C���p�����[�^�e�[�u���̃_�~�[���쐬����                       *
 ********************************************************************/
#define X_SIZE		320
#define Y_SIZE		224
#define Y_SIZE_H	112

void SetAutoLinePara(Uint16 SclNum)
{
    Fixed32 count;
    SclLineparam lp;

    SCL_InitLineParamTb(&lp);

    lp.delta_enbl=ON;
    lp.v_enbl=ON;
    lp.h_enbl=ON;
    lp.line_addr=SCL_VDP2_VRAM_B0;
    lp.interval=0;

    for(count=0;count<Y_SIZE;count++)
    {
    	lp.line_tbl[count].h = FIXED(count)/2;
	lp.line_tbl[count].v = FIXED(count)*2;
	lp.line_tbl[count].dh
	= DIV_FIXED((FIXED(X_SIZE)-(2* lp.line_tbl[count].h)),FIXED(X_SIZE));
    }

    SCL_Open(SclNum);
	SCL_SetLineParam(&lp);
    SCL_Close();
    SCL_DisplayFrame();
}

/********************************************************************
 * ���C���p�����[�^�e�[�u���𖳌��ɂ���                             *
 ********************************************************************/
void ClrLinePara(Uint16 SclNum)
{
    SclLineparam lp;

    SCL_InitLineParamTb(&lp);

    SCL_Open(SclNum);
	SCL_SetLineParam(&lp);
    SCL_Close();
    SCL_DisplayFrame();
}
