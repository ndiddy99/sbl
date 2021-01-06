/*	Copyright(c) 1994 SEGA			*/

#include "smp_bios.h"


SMTA_DefActWk(SMSCACT, );

#if 0
MTH_INIT_CURVE(crvWork, 50);
#endif
static Uint16 pixelColor;

void SMSC_n1scl(SMSCACT *ix);
void SMSC_n1dot(SMSCACT *ix);
void SMSC_drawAct(SMSCACT *ix);

static const Uint16 CycleTb[] = {
	0x4444, 0xffff,
	0xffff, 0xffff,
	0xffff, 0xffff,
	0xffff, 0xffff
};

static Uint32 rnd(Uint32 x) {
    return MTH_GetRand() % x;
}

static void setColor(Uint16 col)
{
    pixelColor = col;
}

static void setPixel(Uint32 x, Uint32 y)
{
    Uint16 *adrs;

    adrs = (Uint16 *)(SCL_VDP2_VRAM_A + y * 1024 + x * 2);
    *adrs = pixelColor;
}

static void drawHorizon(Uint32 x1, Uint32 x2, Uint32 y)
{
    Uint16 *adrs;
    Uint32 i, w;

    if(x1 > x2) {
        w = x1;
        x1 = x2;
        x2 = w;
     }
    adrs = (Uint16 *)(SCL_VDP2_VRAM_A + y * 1024 + x1 * 2);
    for(i = x1; i <= x2; i++) {
        *adrs = pixelColor;
        adrs++;
    }
}

/****************************************************************************
*	’áƒŒƒxƒ‹ü•`‰æ															*
*	void drawLine( x1, y1, x2, y2 ); <- status								*
*		Uint32 x1, y1, x2, y2; <- ŽlŠpÀ•W									*
****************************************************************************/
static void drawLine(Uint32 x1, Uint32 y1, Uint32 x2, Uint32 y2)
{
	Sint32 gosa, tame, hani, dire;
	Sint32 w, x, y;

	if( y1 == y2 ) {
		drawHorizon( x1, x2, y1 );
		return;
	}
	if( abs( x2 - x1 ) < abs( y2 - y1 ) ) {
		if( y2 < y1 ) {
			w = y1;
			y1 = y2;
			y2 = w;
			w = x1;
			x1 = x2;
			x2 = w;
		}
		dire = x1 < x2 ? 1 : -1;
		gosa = abs( x2 - x1 );
		hani = y2 - y1;
		tame = hani / 2;
		x = x1;
		for( y = y1; y <= y2; y++ ) {
			setPixel( x, y );
			tame += gosa;
			if( tame > hani ) {
				x += dire;
				tame -= hani;
			}
		}
	} else {
		if( x2 < x1 ) {
			w = x1;
			x1 = x2;
			x2 = w;
			w = y1;
			y1 = y2;
			y2 = w;
		}
		dire = y1 < y2 ? 1 : -1;
		gosa = abs( y2 - y1 );
		hani = x2 - x1;
		tame = hani / 2;
		y = y1;
		for( x = x1; x <= x2; x++ ) {
			setPixel( x, y );
			tame += gosa;
			if( tame > hani ) {
				y += dire;
				tame -= hani;
			}
		}
	}
}

static void drawConnect(MthXy *p, Uint32 n)
{
    Uint32 i;

    for(i = 1; i < n; i++) {
        drawLine(MTH_FixedToInt(p[i - 1].x),
         MTH_FixedToInt(p[i - 1].y),
         MTH_FixedToInt(p[i].x), MTH_FixedToInt(p[i].y));
    }
}
 
static void drawBox(Uint32 x1, Uint32 y1, Uint32 x2, Uint32 y2)
{
    drawLine(x1, y1, x2, y1);
    drawLine(x1, y2, x2, y2);
    drawLine(x1, y1, x1, y2);
    drawLine(x2, y1, x2, y2);
}
 
static void drawBoxFill(Uint32 x1, Uint32 y1, Uint32 x2, Uint32 y2)
{
    Uint32 i, w;

    if(y1 > y2) {
        w = y1;
        y1 = y2;
        y2 = w;
    }
    for(i = y1; i <= y2; i++) {
        drawLine(x1, i, x2, i);
    }
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

void SMSC_SclMode(void)
{
	enum {
		SCL_INIT,
		SCL_MAIN,
		SCL_EXIT
	};

	SclVramConfig tp;
	SclConfig scfg;
	Uint16 BackCol;

	while(SMMA_MainMode == SMSC_MODE) {
		switch(SMMA_MainLevel) {
			case SCL_INIT:
				SMMA_initall();	
/*******************************************
 *	Priority Initialization            *
 *	Scroll Parameters Initialization   *
 *******************************************/
    SCL_Vdp2Init();
	SCL_VblInit();
    SCL_SetDisplayMode(SCL_NON_INTER, SCL_224LINE, SCL_NORMAL_A);
	SCL_SetPriority(SCL_SP0 | SCL_SP1 | SCL_SP2 | SCL_SP3
	 | SCL_SP4 | SCL_SP5 | SCL_SP6 | SCL_SP7, 0);
	setBitmap();

				SCL_SetFrameInterval(1);

/*******************************************
 *	ƒoƒbƒN‰æ–Ê‚ÌF‚ð•‚ÉÝ’è           *
 *******************************************/
    BackCol = 0x0000;
    SCL_SetBack(SCL_VDP2_VRAM+0x80000-2,1,&BackCol);

   /*******************************************
    *	‚u‚q‚`‚l‚ÌŽg—p•û–@‚ÌÝ’è              *
    *******************************************/
    SCL_InitVramConfigTb(&tp);
	tp.vramModeA = OFF;
	tp.vramModeB = OFF;
    SCL_SetVramConfig(&tp);


/*******************************************
 *	Scroll Configure Set               *
 *******************************************/
    SCL_InitConfigTb(&scfg);
	scfg.dispenbl      = ON;
	scfg.bmpsize       = SCL_BMP_SIZE_512X256;
	scfg.coltype       = SCL_COL_TYPE_32K;
	scfg.datatype      = SCL_BITMAP;
	scfg.plate_addr[0] = SCL_VDP2_VRAM_A;
	SCL_SetConfig(SCL_NBG0, &scfg);

/*******************************************
 *	VRAM Access Pattern Set            *
 *******************************************/
    SCL_SetCycleTable(CycleTb);
	
    SCL_SetPriority(SCL_NBG0,7);

	set_imask(0);

    SCL_Open(SCL_NBG0);
	SCL_MoveTo(FIXED(0), FIXED(0),0);/* Home Position */
	SCL_Scale(FIXED(1.0), FIXED(1.0));
    SCL_Close();
				SMTA_MakeAction(SMSC_n1scl);
				SMTA_MakeAction(SMSC_n1dot);
                SMTA_MakeAction(SMSC_drawAct);
				SMMA_MainLevel++;
				break;
			case SCL_MAIN:
				SMMA_intwait();
				SMTA_ActionLoop();
				break;

			case SCL_EXIT:
				SMMA_MainMode = SMSC_SELECTMODE;
				SMMA_MainLevel = 0;
				break;
		}
	}		
}

void SMSC_n1scl(SMSCACT *ix)
{
	enum {
		INIT,
		MAIN
	};

	static Fixed32 mx, my;

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
		if((SMPA_pad_repdata & PAD_L) && (SMPA_pad_repdata & PAD_R)) {
			mx = 0; my = 0;
		}
		if((SMPA_pad_repdata & PAD_START)) {
			SMMA_MainLevel++;
		} 
		SCL_Open(SCL_NBG0);
		SCL_Move(mx, my, 0);
		SCL_Close();
			break;
	}
}

#define IN_P  5
#define OUT_P 30

void SMSC_drawAct(SMSCACT *ix)
{
    enum {
        MAIN
    };
    Uint32 i;
    MthXy p[IN_P];
    MthXy buf[OUT_P];

    if((SMPA_pad_repdata & PAD_A)) {
        setColor(rnd(32768) | 0x8000);
        setPixel(rnd(320), rnd(224));
    }
	if((SMPA_pad_repdata & PAD_B)) {
	    setColor(rnd(32768) | 0x8000);
	    drawLine(rnd(320), rnd(224), rnd(320), rnd(224));
    }
    if((SMPA_pad_repdata & PAD_C)) {
        setColor(rnd(32768) | 0x8000);
        drawBox(rnd(320), rnd(224), rnd(320), rnd(224));
    }
    if((SMPA_pad_repdata & PAD_X)) {
        setColor(rnd(323768) | 0x8000);
        drawBoxFill(rnd(320), rnd(224), rnd(320), rnd(224));
    }
#if 0
    if((SMPA_pad_repdata & PAD_Y)) {
        setColor(rnd(32768) | 0x8000);
        for(i = 0; i < IN_P; i++) {
            p[i].x = MTH_FIXED(rnd(320));
            p[i].y = MTH_FIXED(rnd(224));
        }
        MTH_Curve2(crvWork, p, IN_P, OUT_P, buf);
        drawConnect(buf, st);
    }
#endif 
}

void SMSC_n1dot(SMSCACT *ix)
{
	enum {
		INIT,
		MAIN
	};

	static Uint16 *vadrs;
	static Uint32 i, j;

	switch(ix->level) {
		case INIT:
			vadrs = (Uint16 *)SCL_VDP2_VRAM_A0;
			i = 0;
			ix->level++;
			break;
		case MAIN:
			for(j = 0; j < 8; j++) {
				vadrs[i] = 0xffff;
				i++;
				if(i == 512 * 256) break;
			}
	}
}


