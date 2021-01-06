/*	Copyright(c) 1994 SEGA			*/

#include "smp_bios.h"

SMTA_DefActWk(SMSCACT, );

static void actPad(SMSCACT *ix);
static void actNbg0(SMSCACT *ix);

static const Uint16 CycleTb[] = {
	0xffff, 0xffff,
	0xffff, 0xffff,
	0x044f, 0xffff,
	0xffff, 0xffff
};

static Fixed32 mx, my;

static Uint32 rnd(Uint32 x)
{
	static tane = 1;

	tane = tane * 5 + 3;
	return tane % x;
}

static void setBitmap()
{
	Uint16 *vadrs;
	Uint32 i, j;
	Uint16 dt;
	
	vadrs = (Uint16 *)SCL_VDP2_VRAM_A;
	dt = 0;
	for(j = 0; j < 256; j++) {
		for(i = 0; i < 512; i++) {
			vadrs[j * 512 + i] = dt | 0x8000;
			dt++;
		}
	}
}

static void setNbg0Data(void)
{
	Uint16 *mapram, *chram, *vram;
	Uint32 i;
	const Uint16 chrdat[] = {
		0x0000, 0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6666, 0x7777,
		0x8888, 0x9999, 0xaaaa, 0xbbbb, 0xcccc, 0xdddd, 0xeeee, 0xffff,
		0x0000, 0x0101, 0x0202, 0x0303, 0x0404, 0x0505, 0x0606, 0x0707,
		0x0808, 0x0909, 0x0a0a, 0x0b0b, 0x0c0c, 0x0d0d, 0x0e0e, 0x0f0f
	};

	mapram = (Uint16 *)SCL_VDP2_VRAM_B0 + 0x10000;
	for(i = 0; i < 128 * 1024 / 2; i += 2) {
		mapram[i] = 0;
		mapram[i + 1] = 0;
	}
	mapram[0] = 0; mapram[1] = 0;

	chram = (Uint16 *)SCL_VDP2_VRAM_B0;
	for(i = 0; i < 32; i++) {
		chram[i] = chrdat[i];
	}
	vram = (Uint16 *)SCL_VDP2_VRAM_B0;
	for(i = 0; i < 0x8000; i++) {
/*		vram[i] = ((i % 256) << 8) + (i % 256); */
		vram[i] = rnd(65536);
	}
	vram = (Uint16 *)(SCL_VDP2_VRAM_B0 + 0x10000);
	for(i = 0; i < 0x8000; i += 2) {
		vram[i] = rnd(65536);
		vram[i + 1] = rnd(65536);
	}
}

static void setRbg0Data(void)
{
	Uint16 *vram;
	Uint32 i;

	vram = (Uint16 *)SCL_VDP2_VRAM_A0;
	for(i = 0; i < 512 * 256 / 2; i++) {
		vram[i] = rnd(65536);
	}
}

static void setPalette(void)
{
	static Uint32 col[256];
	Uint32 i;
	Uint16 *pram;

	pram = (Uint16 *)0x25f00000;
	for(i = 0; i < 256; i++) {
		col[i] = rnd(32768);
		pram[i] = rnd(32768);
	}
	SCL_SetColRam(SCL_RBG0, 0, 256, col);
	for(i = 0; i < 2048; i++) {
		pram[i] = rnd(32768);
	}
}

/***************************************/
/*   Entry Point		*/
/***************************************/
void SMSC_BackScl(void)
{
	enum {
		SCL_INIT,
		SCL_MAIN,
		SCL_EXIT
	};

	SclVramConfig tp;
	SclConfig scfgRbg0, scfgNbg0, scfgNbg1;
	Uint16 BackCol;

	while(SMMA_MainMode == SMSC_BACK) {
		switch(SMMA_MainLevel) {
			case SCL_INIT:
				SMMA_initall();	
/*******************************************
 *	Priority Initialization            *
 *	Scroll Parameters Initialization   *
 *******************************************/
				SCL_VblInit();
    			SCL_Vdp2Init();
			    SCL_SetDisplayMode(SCL_NON_INTER, SCL_224LINE, SCL_NORMAL_A);
				SCL_SetPriority(SCL_SP0 | SCL_SP1 | SCL_SP2 | SCL_SP3
				 | SCL_SP4 | SCL_SP5 | SCL_SP6 | SCL_SP7, 0);

/*******************************************
 *	ƒoƒbƒN‰æ–Ê‚ÌF‚ð•‚ÉÝ’è           *
 *******************************************/
			    BackCol = 0x03e0;
			    SCL_SetBack(SCL_VDP2_VRAM+0x80000-2,1,&BackCol);

/*******************************************
*	‚u‚q‚`‚l
****************************************/
			    SCL_InitVramConfigTb(&tp);
				tp.vramModeA = ON;
				tp.vramModeB = ON;
			    SCL_SetVramConfig(&tp);


/*******************************************
 *	Scroll Configure Set RBG0              *
 *******************************************/
#if 0
			SCL_InitRotateTable(SCL_VDP2_VRAM_A1 + 0x10000, 1,
			 SCL_RBG0, SCL_NON);
			 
			SCL_InitConfigTb(&scfgRbg0);
			scfgRbg0.dispenbl      = OFF;
			scfgRbg0.bmpsize       = SCL_BMP_SIZE_512X256;
			scfgRbg0.coltype       = SCL_COL_TYPE_256;
			scfgRbg0.datatype      = SCL_BITMAP;
			scfgRbg0.mapover       = SCL_OVER_2;
			scfgRbg0.plate_addr[0] = SCL_VDP2_VRAM_A0;
			SCL_SetConfig(SCL_RBG0, &scfgRbg0);
			
			SCL_SetPriority(SCL_RBG0, 2);

			setRbg0Data();
#endif
/*******************************************
 *	Scroll Configure Set NBG0              *
 *******************************************/
			SCL_InitConfigTb(&scfgNbg0);
			scfgNbg0.dispenbl      = ON;
			scfgNbg0.charsize      = SCL_CHAR_SIZE_1X1;
			scfgNbg0.pnamesize     = SCL_PN2WORD;
			scfgNbg0.flip          = SCL_PN_12BIT;
			scfgNbg0.platesize     = SCL_PL_SIZE_2X2;
			scfgNbg0.coltype       = SCL_COL_TYPE_256;
			scfgNbg0.datatype      = SCL_CELL;
			scfgNbg0.mapover       = SCL_OVER_2;
			scfgNbg0.plate_addr[0] = SCL_VDP2_VRAM_B0 + 0x10000;
			scfgNbg0.plate_addr[1] = SCL_VDP2_VRAM_B0 + 0x14000;
			scfgNbg0.plate_addr[2] = SCL_VDP2_VRAM_B0 + 0x18000;
			scfgNbg0.plate_addr[3] = SCL_VDP2_VRAM_B0 + 0x1c000;
			SCL_SetConfig(SCL_NBG0, &scfgNbg0);

			SCL_SetPriority(SCL_NBG0, 7);

			setNbg0Data(); 

/*******************************************
 *	Scroll Configure Set NBG1              *
 *******************************************/
#if 0
			SCL_InitConfigTb(&scfgNbg1);
			scfgNbg1.dispenbl      = OFF;
			scfgNbg1.charsize      = SCL_CHAR_SIZE_1X1;
			scfgNbg1.pnamesize     = SCL_PN2WORD;
			scfgNbg1.platesize     = SCL_PL_SIZE_2X1;
			scfgNbg1.coltype       = SCL_COL_TYPE_256;
			scfgNbg1.datatype      = SCL_CELL;
			scfgNbg1.mapover       = SCL_OVER_2;
			scfgNbg1.plate_addr[0] = SCL_VDP2_VRAM_B1 + 0x10000;
			scfgNbg1.plate_addr[1] = SCL_VDP2_VRAM_B1 + 0x14000;
			scfgNbg1.plate_addr[2] = SCL_VDP2_VRAM_B1 + 0x18000;
			scfgNbg1.plate_addr[3] = SCL_VDP2_VRAM_B1 + 0x1c000;
			SCL_SetConfig(SCL_NBG1, &scfgNbg1);

			SCL_SetPriority(SCL_NBG1, 1);
#endif

/*******************************************
 *	VRAM Access Pattern Set            *
 *******************************************/
			    SCL_SetCycleTable(CycleTb);

			    SCL_SetColRamMode(SCL_CRM15_2048);
			    SCL_AllocColRam(SCL_RBG0 | SCL_NBG0 | SCL_NBG1, 
				 256, OFF);
			    setPalette();
	
				set_imask(0);

			    SCL_Open(SCL_NBG0);
				SCL_MoveTo(FIXED(0), FIXED(0),0);/* Home Position */
				SCL_Scale(FIXED(1.0), FIXED(1.0));
			    SCL_Close();
/*
			    SCL_Open(SCL_RBG_TB_A);
				SCL_MoveTo(FIXED(0), FIXED(0), FIXED(0));
				SCL_Scale(FIXED(1), FIXED(1));
				SCL_SetRotateCenter(160, 112, 0);
			    SCL_Close();
*/
				SMTA_MakeAction(actPad);
				SMTA_MakeAction(actNbg0); 
				SCL_DisplayFrame();

				SMMA_MainLevel++;
				break;
			case SCL_MAIN:
/*				SMMA_intwait(); */
				SMTA_ActionLoop();
				SCL_DisplayFrame();
				break;

			case SCL_EXIT:
				SMMA_MainMode = SMSC_SELECTMODE;
				SMMA_MainLevel = 0;
				break;
		}
	}		
}

/******************************************************************/
/*	Pad Control	*/
/******************************************************************/
static void actPad(SMSCACT *ix)
{
	enum {
		INIT,
		MAIN
	};
	
	switch(ix->level) {
		case INIT:
			mx = FIXED(0);
			my = FIXED(0);
			ix->level++;
			break;
		case MAIN:
			if((SMPA_pad_repdata & PAD_LEFT))
		 		mx = mx + FIXED(0.1);
			if((SMPA_pad_repdata & PAD_RIGHT))
				mx = mx - FIXED(0.1);
			if((SMPA_pad_repdata & PAD_UP))
				my = my + FIXED(0.1);
			if((SMPA_pad_repdata & PAD_DOWN))
			 	my = my - FIXED(0.1);
			if((SMPA_pad_repdata & PAD_START)) {
				mx = 0; my = 0;
			}
			if((SMPA_pad_repdata & PAD_START) &&
			 (SMPA_pad_repdata & PAD_B)) {
				SMMA_MainLevel++;
			} 
			break;
	}
}


/******************************************************************/
/*	‚m‚a‚f‚OƒAƒNƒVƒ‡ƒ“	*/
/******************************************************************/
static void actNbg0(SMSCACT *ix)
{
	enum {
		INIT,
		MAIN
	};
	
	switch(ix->level) {
		case INIT:
			ix->level++;
			break;
		case MAIN:
			SCL_Open(SCL_NBG0);
			SCL_Move(mx, my, 0);
			SCL_Close();
			break;
	}
}

