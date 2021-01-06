/*------------------------------------------------------------------------
 *  FILE:	SmpScl1.c
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
#include    <string.h>
#include	<machine.h>
#include	<sega_scl.h> 

#include	"..\..\v_blank\v_blank.h"

#include	"..\graphic\cosmo2u.cha"
#include	"..\graphic\cosmo2u.col"
#include	"..\graphic\cosmo2u.map"

/*----------------------------
SCROLL NBG0 Cycle Table
Pattern Name Table location:B0
Character location: B1
Color Mode: C256
Zoom Mode: 1
----------------------------*/
Uint16	CycleTb[]={
	0xffff,0xffff,
	0xffff,0xffff,
	0x0f44,0xffff,
	0xffff,0xffff
};


void main()
{
	SclConfig	scfg;
	SclRgb		start,end;
	Uint16  	BackCol,i,PadData1EW;
	Uint8   	*VramWorkP;

	/*******************************************
	*	ＶＤＰ２ライブラリの初期化         *
	*******************************************/
	SCL_Vdp2Init();
	SCL_SetColRamMode(SCL_CRM24_1024);

	/**********************************
	*	V_Blankルーチンの登録     *
	**********************************/
	SetVblank();
	set_imask(0);

	/***********************************************
	*	カラーモード設定の為Ｖ－ＩＮＴ待ち     *
	***********************************************/
	SCL_SetFrameInterval(1);
	SCL_DisplayFrame();
/*	SCL_SetFrameInterval(0);*/

	/*******************************************
	*	バック画面の色を黒に設定           *
	*******************************************/
	BackCol = 0x0000;
	SCL_SetBack(SCL_VDP2_VRAM+0x80000-2,1,&BackCol);

	/******************************************************
	*	スクロールのｷｬﾗｸﾀﾊﾟﾀｰﾝの設定(VRAM B1に置く)   *
	******************************************************/
/*	VramWorkP = (Uint8 *)SCL_VDP2_VRAM_A0;*/
	VramWorkP = (Uint8 *)SCL_VDP2_VRAM_B1;
	memcpy(VramWorkP, cosmo2u_char, sizeof(cosmo2u_char));

	/*******************************************
	*	スクロールのﾊﾟﾀｰﾝﾈｰﾑﾃｰﾌﾞﾙの設定    *
	*******************************************/
	/* データが40x28cel(320x224ﾄﾞｯﾄ)なのでそのまま転送できない */
/*	VramWorkP = (Uint8 *)SCL_VDP2_VRAM_A1;*/
	VramWorkP = (Uint8 *)SCL_VDP2_VRAM_B0;
	for(i=0;i<28;i++) {
		memcpy(VramWorkP, &cosmo2u_map[i*40],40*2);
		VramWorkP += 64*2;
	}

	/*******************************************
	*	パレットデータの設定               *
	*******************************************/
	SCL_AllocColRam(SCL_NBG0,256,OFF);
	SCL_SetColRam(SCL_NBG0,0,sizeof(cosmo2u_col),(void *)cosmo2u_col);

	/*******************************************
	*	Scroll Configure Set               *
	*******************************************/
	SCL_InitConfigTb(&scfg);
		scfg.dispenbl      = ON;
		scfg.charsize      = SCL_CHAR_SIZE_1X1;
		scfg.pnamesize     = SCL_PN1WORD;
		scfg.flip          = SCL_PN_12BIT;
		scfg.platesize     = SCL_PL_SIZE_2X2;
		scfg.coltype       = SCL_COL_TYPE_256;
		scfg.datatype      = SCL_CELL;
		scfg.patnamecontrl = 0x000c;/* VRAM B1 のオフセット */
/*		for(i=0;i<4;i++)   scfg.plate_addr[i] = SCL_VDP2_VRAM_A1;*/
		for(i=0;i<4;i++)   scfg.plate_addr[i] = SCL_VDP2_VRAM_B0;
	SCL_SetConfig(SCL_NBG0, &scfg);

	/*******************************************
	*	VRAM Access Pattern Set            *
	*******************************************/
	SCL_SetCycleTable(CycleTb);

	SCL_SetPriority(SCL_NBG0,7);
	SCL_SetPriority(SCL_SPR,0);	/* スプライトは表示しない */

	SCL_Open(SCL_NBG0);
		SCL_MoveTo(FIXED(0), FIXED(0),0);/* Home Position */
		SCL_Scale(FIXED(1.0), FIXED(1.0));
	SCL_Close();

	start.red = start.green = start.blue = 0;
	end.red = end.green = end.blue = - 255;
	while(1) {
		PadData1EW = PadData1E;
		PadData1E = 0;
		if(PadData1EW & PAD_U){
			SCL_SetColOffset(SCL_OFFSET_A,SCL_NBG0,start.red,start.green,start.blue);
			SCL_SetAutoColOffset(SCL_OFFSET_A,1,10,&start,&end);
		}
		if(PadData1EW & PAD_D){
			SCL_SetColOffset(SCL_OFFSET_A,SCL_NBG0,end.red,end.green,end.blue);
			SCL_SetAutoColOffset(SCL_OFFSET_A,1,10,&end,&start);
		}
		if((PadData1 & PAD_S)) 	SCL_SetMosaic(SCL_NBG0, 0, 0);
		if((PadData1 & PAD_LB))	SCL_SetMosaic(SCL_NBG0, 2, 2);
		if((PadData1 & PAD_RB))	SCL_SetMosaic(SCL_NBG0, 4, 4);
		if((PadData1 & PAD_X)) 	SCL_SetMosaic(SCL_NBG0, 6, 6);
		if((PadData1 & PAD_Y)) 	SCL_SetMosaic(SCL_NBG0, 8, 8);
		if((PadData1 & PAD_Z)) 	SCL_SetMosaic(SCL_NBG0,10,10);
		if((PadData1 & PAD_A)) 	SCL_SetMosaic(SCL_NBG0,12,12);
		if((PadData1 & PAD_B)) 	SCL_SetMosaic(SCL_NBG0,14,14);
		if((PadData1 & PAD_C)) 	SCL_SetMosaic(SCL_NBG0,16,16);
		SCL_DisplayFrame();
	}
}
