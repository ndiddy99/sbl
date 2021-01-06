/*------------------------------------------------------------------------
 *  FILE: smpscl5.C
 *
 *	Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *		VDP2 Test Program
 *
 *  AUTHOR(S):
 *		K.M
 *
 *  MOD HISTORY:
 *		Written by K.M on 1994-07-29 Ver.1.00
 *		Updated by K.M on 1994-07-29 Ver.1.00
 * 
 *------------------------------------------------------------------------
 */
#include	<stdio.h>
#include	<machine.h>
#include	<sega_scl.h> 

#include	"..\..\v_blank\v_blank.h"

	Uint16		PadData1EW,PadData2EW;
	SclConfig	Rbg0Scfg,Rbg1Scfg;

#define X_SIZE	512
#define Y_SIZE	256

	Uint32		LinePal[240];
	Uint16		LinePalTb[240];

/* �t�H���g�f�[�^�}�b�`���O�e�[�u�� */
static	Uint8	bitpat[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
static	Uint8	Japan=1;	/* ���ȕ\�����[�h�ɂ��� */

void	Print256(Uint8 *vram,Uint8 *str,Uint16 x,Uint16 y,Uint8 col,Uint16 back);
void	Print256S(Uint8 *vram,Uint8 *str,Uint16 x,Uint16 y,Uint8 col,Uint16 back);
void	ClrVram(Uint8 *buff);

extern	Uint8	AsciiFontData[];/* 8x16 ASCII Font �A���t�@�x�b�g���� */
extern	Uint8	KanaFontData[];	/* 8x16 ASCII Font ���ȕ��� */
extern	Uint8	EouFontData[];	/* 8x16 ASCII Font ���[���b�p�����H���� */

/* �X�N���[���p�O���t�B�b�N�f�[�^�̃|�C���^ */
extern	Uint32	BackPalette[];		/* �S���^�C���̃p���b�g   */
extern	Uint8	BackCharPatData[];	/* �S���^�C���̃f�[�^�{�� */

#define	MAP_OFFSET	0x0800
#define	MAP_NUM		16

void main()
{
	SclVramConfig	tp;
	Uint32		i,j;
	Uint16		*wp,BackCol,col;
	Fixed32		r;
	Uint32		Pal32;
	XyInt		xy[2],sp[3];
	Uint32		Color[16];
	Uint8		*Pal32p8,sw,PrSw;
	Uint32		SprColRam;
	Uint32		Surface;

/*	wp = (Uint16 *)&BackPalette[0];
	for(i=0;i<256;i++){
		Pal32p8 = (Uint8 *)&BackPalette[i];
		wp[i] = (RGB16_COLOR(((Uint16)Pal32p8[3]>>3),((Uint16)Pal32p8[2]>>3),((Uint16)Pal32p8[1]>>3)) ) & 0x7fff;
	}*/

	Pal32p8 = (Uint8 *)&Pal32;
	Pal32   = 0x00ffffff;

	/* ���C���J���[�p�p���b�g�쐬 */
	for(i=0;i<240;i++) {
		LinePal[i] = Pal32;
		LinePalTb[i] = i;
		Pal32p8[1] --;
		Pal32p8[2] --;
		Pal32p8[3] --;
	}


	/*******************************************
	*	�e�평��������                     *
	*******************************************/
	SCL_Vdp2Init();
	SCL_SetDisplayMode(SCL_NON_INTER,SCL_224LINE,SCL_NORMAL_A);
	SCL_SetColRamMode(SCL_CRM24_1024);
/*	SCL_SetColRamMode(SCL_CRM15_2048);*/

	ClrVram((Uint8 *)SCL_VDP2_VRAM_A0);
	Print256S((Uint8 *)SCL_VDP2_VRAM_A0,
		(Uint8 *)"VDP2 Test Program",10,20,2,0);
	Print256S((Uint8 *)SCL_VDP2_VRAM_A0,
		(Uint8 *)"Display RBG0 & RBG1",10,40,3,0);
	Print256S((Uint8 *)SCL_VDP2_VRAM_A0,
		(Uint8 *)" RBG0:��Ҳ � Ӽ�(BitMap)",10,60,4,0);
	Print256S((Uint8 *)SCL_VDP2_VRAM_A0,
		(Uint8 *)" RBG1:����� � ���(Cell)",10,80,5,0);
	Print256S((Uint8 *)SCL_VDP2_VRAM_A0,
		(Uint8 *)"S����:����� � ��޽\nX����:X�޸ ����\nY����:���� ����\nZ����:Z�޸ ����\n",20,100,6,0);
	Print256S((Uint8 *)SCL_VDP2_VRAM_A0,
		(Uint8 *)"A����:RBG0 � ����\nB����:RBG1 � ����\nC����:��ײ��è Change",180,100,6,0);

	/*********************************
	*	V_Blank���[�`���̐ݒ�    *
	*********************************/
	SetVblank();
	set_imask(0);
	SCL_SetFrameInterval(1);
	SCL_DisplayFrame();

	/*******************************************
	*	�o�b�N��ʂ̐F�����ɐݒ�           *
	*******************************************/
	BackCol = RGB16_COLOR(0,0,0) & 0x7fff;
	SCL_SetBack(SCL_VDP2_VRAM+0x80000-2,1,&BackCol);

	/****************************************************
	*   ��]���Ұ�ð��ق�VRAM���̂ǂ��ɒu�������w��     *
	****************************************************/
	SCL_InitRotateTable(SCL_VDP2_VRAM_B1+0x10000,2,SCL_RBG0,SCL_RBG1);

	/*******************************************
	*	�u�q�`�l�̎g�p���@�̐ݒ�           *
	*******************************************/
	SCL_InitVramConfigTb(&tp);
	tp.vramModeA  = ON;		/* VRAM A �𕪊����� */
	tp.vramModeB  = ON;		/* VRAM B �𕪊����� */
	tp.vramA0     = SCL_RBG0_CHAR;	/* VRAM A0 ��RBG0��BitMap�ް���u�� */
	tp.vramA1     = SCL_RBG0_K;	/* VRAM A1 �ɉ�]���Ұ��W��ð��ق�u�� */
/*	tp.colram     = SCL_RBG0_K;*/	/* �װRAM�ɉ�]���Ұ��W��ð��ق�u�� */
	SCL_SetVramConfig(&tp);

	/*************************************
	*	��]��0�̐ݒ�                *
	*************************************/
	SCL_InitConfigTb(&Rbg0Scfg);
	    Rbg0Scfg.dispenbl = ON;
	    Rbg0Scfg.coltype  = SCL_COL_TYPE_256;
	    Rbg0Scfg.datatype = SCL_BITMAP;
	    for(i=0;i<MAP_NUM;i++)
		Rbg0Scfg.plate_addr[i] = SCL_VDP2_VRAM_A0;
	SCL_SetConfig(SCL_RBG0, &Rbg0Scfg);

	/*************************************
	*	��]��1�̐ݒ�                *
	*************************************/
	SCL_InitConfigTb(&Rbg1Scfg);
	    Rbg1Scfg.dispenbl      = ON;
	    Rbg1Scfg.charsize      = SCL_CHAR_SIZE_2X2;
	    Rbg1Scfg.pnamesize     = SCL_PN1WORD;
	    Rbg1Scfg.flip          = SCL_PN_12BIT;
	    Rbg1Scfg.platesize     = SCL_PL_SIZE_1X1;
	    Rbg1Scfg.coltype       = SCL_COL_TYPE_256;
	    Rbg1Scfg.datatype      = SCL_CELL;
	    Rbg1Scfg.mapover       = SCL_OVER_0;
	    for(i=0;i<MAP_NUM;i++)
		Rbg1Scfg.plate_addr[i] = SCL_VDP2_VRAM_B1;
	SCL_SetConfig(SCL_RBG1, &Rbg1Scfg);

	/****************************************
	*	�v���C�I���e�B�̐ݒ�            *
	****************************************/
	SCL_SetPriority(SCL_RBG0,4);
	SCL_SetPriority(SCL_RBG1,3);

	/******************************************************
	*	�X�v���C�g�ɓ\��e�N�X�`���[�̍쐬�ƃZ�b�g    *
	******************************************************/
	SCL_AllocColRam(SCL_RBG0,256,OFF);
	    /* Set Color Data into Color RAM */
	    Color[0] = 0x00000000;
	    Color[1] = RGB32_COLOR(  0,  0,  0) & 0x00ffffff;
	    Color[2] = RGB32_COLOR(255,255,255) & 0x00ffffff;
	    Color[3] = RGB32_COLOR(255,  0,  0) & 0x00ffffff;
	    Color[4] = RGB32_COLOR(255,255,  0) & 0x00ffffff;
	    Color[5] = RGB32_COLOR(  0,  0,255) & 0x00ffffff;
	    Color[6] = RGB32_COLOR(  0,255,  0) & 0x00ffffff;
	SCL_SetColRam(SCL_RBG0,0,7,Color);

	/*****************************************
	*	�w�i�̃^�C�����u�q�`�l�ɓ]��     *
	*****************************************/
	/* �J���[�q�`�l�̊m�ۂƃp���b�g�̓]�� */
	SCL_AllocColRam(SCL_RBG1,256,ON);
	SCL_SetColRam(SCL_RBG1,1,256,BackPalette);

	/* �L�����N�^�p�^�[���f�[�^���u�q�`�l�ɓ]�� */
	memcpy(SCL_VDP2_VRAM_B0,BackCharPatData,1024);
	/* �p�^�[���l�[���f�[�^���u�q�`�l�ɓ]�� */
	for(i=0;i<16;i++)	for(j=0;j<16;j++) {
		wp  = (Uint16 *)(SCL_VDP2_VRAM_B1 + i*2*64 + j*2*2);
		*wp = MAP_OFFSET+0;
		wp++;
		*wp = MAP_OFFSET+2;
		wp += 31;
		*wp = MAP_OFFSET+4;
		wp++;
		*wp = MAP_OFFSET+6;
	}

	/***********************************
	*	���C���J���[��ʂ̐ݒ�     *
	***********************************/
	SCL_AllocColRam(SCL_LNCL,256,ON);
	SCL_SetColRam(SCL_LNCL,0,224,LinePal);
	SCL_SetLncl(SCL_VDP2_VRAM_A1+0x10000,224,LinePalTb);
	SCL_EnableLineCol(SCL_RBG1);
	SCL_SetColMixRate(SCL_RBG1,10);

	/**************************************
	*	�X�N���[����ʂ̏����`��      *
	**************************************/
	SCL_Open(SCL_RBG_TB_A);
	  SCL_MoveTo(FIXED(0), FIXED(0), FIXED(0));/* Home Position */
	  SCL_Scale(FIXED(1.0), FIXED(1.0));
	SCL_Open(SCL_RBG_TB_B);
	  SCL_MoveTo(FIXED(0), FIXED(0), FIXED(0));/* Home Position */
	  SCL_Scale(FIXED(1.0), FIXED(1.0));
	SCL_Close();
	SCL_DisplayFrame();

	/**************************************
	*	�p�b�h���g������ʂ̑���      *
	**************************************/
	r = FIXED(2);
	sw = 0;
	PrSw = ON;
	Surface = SCL_RBG_TB_A;
	while(1){
		PadData1EW=PadData1E;	PadData1E=0;
		PadData2EW=PadData2E;	PadData2E=0;

		if(PadData1 || PadData1EW){
			SCL_Open(Surface);
			if(PadData1 & PAD_U)
				SCL_Move( 0, r, 0);
			else if(PadData1 & PAD_D)
				SCL_Move( 0,-r, 0);
			if(PadData1 & PAD_R)
				SCL_Move(-r, 0, 0);
			else if(PadData1 & PAD_L)
				SCL_Move( r, 0, 0);
			if(PadData1 & PAD_RB)
				SCL_Move( 0, 0,-r);
			else if(PadData1 & PAD_LB)
				SCL_Move( 0, 0, r);
			if((PadData1 & PAD_Z))
				SCL_Rotate(0,r,0);
			if((PadData1 & PAD_Y))
				SCL_Rotate(0,0,r);
			if((PadData1 & PAD_X))
				SCL_Rotate(r,0,0);
			if((PadData1 & PAD_S)) {
				SCL_MoveTo(0,0,0);
				SCL_RotateTo(0,0,0,SCL_X_AXIS);
			}
			if((PadData1 & PAD_A))
				Surface = SCL_RBG_TB_A;
			if((PadData1 & PAD_B))
				Surface = SCL_RBG_TB_B;
			if((PadData1EW & PAD_C)) {
				if(PrSw) {
					SCL_SetPriority(SCL_RBG1,5);
					PrSw=OFF;
				}else{
					SCL_SetPriority(SCL_RBG1,3);
					PrSw=ON;
				}
			}
			SCL_Close();
		}
		SCL_DisplayFrame();
	}
}

/*****************************************************************
 * �����R�[�h���當���p�^�[���̐擪�A�h���X��T���ĕԂ�          *
 *****************************************************************/
Uint8 *SerchFont8(Uint8 code)
{
	if(code >= 0x20 && code < 0x80)
	    return((Uint8 *)(AsciiFontData+((Uint32 )(code-0x20))*16));
	else if(code > 0xa0) {
	    if(Japan)	return((Uint8 *)(KanaFontData+((Uint32 )(code-0xa1))*16) );
	    else	return((Uint8 *)(EouFontData+((Uint32 )(code-0xa1))*16) );
	}
	return((Uint8 *)AsciiFontData);
}


/*****************************************************************
 * �e�N�X�`���[�o�b�t�@�ɃA�X�L�[�R�[�h�������P��������          *
 *****************************************************************/
void  Print1Char256_8x16(Uint8 *vram,Uint8 codedat,Uint16 x,Uint16 y,Uint8 col,Uint16 back)
{
	Uint32	wx,wy,wp;
	Uint32	i,j;
	Uint8	*code;

	code = SerchFont8(codedat);

	wx = x;
	wy = y;

	wp = wx	+ wy * X_SIZE;
	if(back < 256) {
		for(i=0;i<16;i++) {
			for(j=0;j<8;j++) {
				if(code[i] & bitpat[j])	vram[wp]= col;
				else			vram[wp]= back;
				wp++;
			}
			wp += (X_SIZE - 8);
		}
	}else{
		for(i=0;i<16;i++) {
			for(j=0;j<8;j++) {
				if(code[i] & bitpat[j])	vram[wp]= col;
				wp++;
			}
			wp += (X_SIZE - 8);
		}
	}
}

/*****************************************************************
 * �e�N�X�`���[�o�b�t�@�ɃA�X�L�[�R�[�h�����������              *
 * �iX_SIZE �� 512�ɕύX����΂u�c�o�Q�̂u�q�`�l�ɂ��g���܂��B�j *
 *****************************************************************/
void  Print256(Uint8 *vram,Uint8 *str,Uint16 x,Uint16 y,Uint8 col,Uint16 back)
{
	Uint32	i;
	Uint16	wx,wy;

	wx = x;wy = y;

	/* �Z���^�����O */
	if(wx > X_SIZE)	wx = X_SIZE/2 - strlen(str)*4;

	i=0;
	while(str[i]) {
		if(str[i] > 0x80 && str[i] < 0xA0) {
			/* �V�t�g�i�h�r�S�p�R�[�h��������X�y�[�X��������B */
			wx += 16;
			i++;
		}else if(str[i]=='\\'){
			i++;
			if(str[i]=='n') {
				wx = x;wy += 18;/* ���s */
			}
		}else if(str[i]==0x0a) {
			wx = x;wy += 18;/* ���s */
		}else{
			Print1Char256_8x16(vram,str[i],wx,wy,col,back);
			wx += 8;
		}
		if(wx+8 >= X_SIZE) {
			wx = x;wy += 18;/* ���s */
		}
		i++;
	}
}

void	ClrVram(Uint8 *buff)
{
	Uint32	i;

	for(i=0;i<(X_SIZE * Y_SIZE);i++)	buff[i]=0x00;
}

void	Print256S(Uint8 *vram,Uint8 *str,Uint16 x,Uint16 y,Uint8 col,Uint16 back)
{
	/* �����̑� */
	Print256(vram,str,x+1,y+1,1,back);
	Print256(vram,str,x+1,y  ,1,300);
	Print256(vram,str,x  ,y+1,1,300);
	/* �����̖{�� */
	Print256(vram,str,x,y,col,300);
}
