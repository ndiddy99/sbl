/*	Copyright(c) 1994 SEGA			*/
/*
 *
 *      2/14 DEMO   HEADER FILE
 *
 */
 
#ifndef __SMP_D214_H
#define __SMP_D214_H


/*
 *    2/14 DEMO MODE SCROLL VRAM ASSIGN
 */
#define SMD2_NBG_PTGEN (SCR_VRAMA0 + 0x00000) /* NBG PTGEN                   */

#define SMD2_NBG0_PTNAMA (SCR_VRAMA0 + 0x08000)  /* NBG0 PTNAM              */
#define SMD2_NBG0_PTNAMB (SCR_VRAMA0 + 0x0a000)  /* NBG0 PTNAM              */
#define SMD2_NBG0_PTNAMC (SCR_VRAMA0 + 0x0c000)  /* NBG0 PTNAM              */
#define SMD2_NBG0_PTNAMD (SCR_VRAMA0 + 0x0e000)  /* NBG0 PTNAM              */
#define SMD2_BCLRTBLADD (SCR_VRAMA0 + 0x0000)    /* Backcolortable  */
#define SMD2_LCLRTBLADD (SCR_VRAMA0 + 0x10000)   /* ラインカラーテーブル */

#define SMD2_RBG0_KEISU (SCR_VRAMA1 + 0x00000)    /* RBG0 係数           */
#define SMD2_RBG0_RPAR  (SCR_VRAMA1 + 0x10000)    /* RBG0 回転パラメータ */
#define SMD2_RBG0_PTGEN (SCR_VRAMB0 + 0x00000)    /* RBG0 PTGEN          */
#define SMD2_RBG0_PTNAM (SCR_VRAMB1 + 0x00000)    /* RBG0 PTNAM          */



typedef struct {
       Sint32 xposi;	
       Sint32 yposi;	
       Sint32 zposi;
       Sint16 xdirec;
       Sint16 zdirec;
       Sint16 grwk;
       Sint16 dummy;
	} XROT_INFO;

/* ZBUFFER 構造体 */
typedef struct zbufs {
	struct zbufs *next; /* 00 */
	Sint32 zposi;       /* 04 */
	Sint16 control;     /* 08 */
	Sint16 putmode;     /* 10 */
	Sint16 color;       /* 12 */
	Uint16  picadd;      /* 14 */
	Sint16 ga;          /* 18 */
	Sint16 ax;          /* 20 */
	Sint16 ay;          /* 22 */
	Sint16 gb;          /* 24 */
	Sint16 bx;          /* 26 */
	Sint16 by;          /* 28 */
	Sint16 gc;          /* 30 */
	Sint16 cx;          /* 32 */
	Sint16 cy;          /* 34 */
	Sint16 gd;          /* 36 */
	Sint16 dx;          /* 38 */
	Sint16 dy;          /* 40 */
	} ZBUFS;


SMTA_DefActWk(SMD2ACT,
	      SPRITE sprite;
	      Sint32 reg0;
	      Sint32 reg1;
	      Sint32 reg2;
	      Sint32 reg3;
	      Sint32 cnt0;
	      Sint32 cnt1;
	      Sint32 cnt2;
	      Sint32 cnt3;
);


/* FUNCTION PROTOTYPE */
extern void SMD2_MainMode(void);

/* GLOBAL HENSUU */
ROTPARAM SMD2_rotregbuf; 
Uint16 D214FLAG;
Uint16 zkyori;
Uint16 zbufcnt;
Uint16 SMD2_grno;
ZBUFS zbuffer[64];
SMD2ACT *Yukawkmap[8][8];
Uint32 SMD2_RotregReq;
Sint32 SMD2_KeisuBuf[0x100];
Sint16 demoswdata[0x1000];

#endif
 
