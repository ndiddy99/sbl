/*----------------------------------------------------------------------------
 *  smpspr20.c -- スプライトサンプル 2
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-02-15 Ver.0.80
 *  Updated by H.E on 1994-07-25 Ver.1.00
 *
 *  スプライト表示基本処理のテスト
 *  各種スプライトコマンドの実行。
 *
 *  [キーﾊﾟｯﾄﾞ操作]
 *      A        : (1) 画面＝３２０ｘ２４０ カラー＝ＲＧＢモード
 *               : (2) 画面＝３２０ｘ２４０ カラー＝８ビットモード
 *               : (3) 画面＝６４０ｘ４８０ カラー＝８ビットモード
 *                           （座標値もＸ，Ｙ２倍にする）
 *      B        : 画面＝６４０ｘ２４０ カラー＝８ビットモード
 *      C        : 画面＝６４０ｘ４８０ カラー＝８ビットモード
 *
 *----------------------------------------------------------------------------
 */

#include	<stdio.h>
#include	<stdlib.h>
#define		_SPR2_
#include    "sega_gfs.h"
#include    "sega_spr.h"
#include    "sega_dbg.h"

#include	"machine.h"
#include    "sega_scl.h"
#include	"../../v_blank/v_blank.h"



Uint16 tut160[(160*160*2)+4];


#define OPEN_MAX 5
#define MAX_DIR  10
#define SECT_SIZE 2048


Uint32 work[GFS_WORK_SIZE(OPEN_MAX) / sizeof(Uint32)]; /* library working area */
GfsDirTbl dirtbl;
GfsDirId dirid[MAX_DIR];

#define   COLOR_BANK       0x0000

SprSpCmd  spCmd16_320_240[] = {
   {/* [ 0]     */
    /* control  */ (JUMP_NEXT | FUNC_SCLIP),
    /* link     */ 0,
    /* drawMode */ 0,
    /* color    */ 0,
    /* charAddr */ 0,
    /* charSize */ 0,
    /* ax, ay   */ 0, 0,
    /* bx, by   */ 0, 0,
    /* cx, cy   */ 320-1, 240-1,
    /* dx, dy   */ 0, 0,
    /* grshAddr */ 0,
    /* dummy    */ 0},

   {/* [ 1]     */
    /* control  */ (JUMP_NEXT | FUNC_LCOORD),
    /* link     */ 0,
    /* drawMode */ 0,
    /* color    */ 0,
    /* charAddr */ 0,
    /* charSize */ 0,
    /* ax, ay   */ 0, 0,
    /* bx, by   */ 0, 0,
    /* cx, cy   */ 0, 0,
    /* dx, dy   */ 0, 0,
    /* grshAddr */ 0,
    /* dummy    */ 0},

   {/* [10]     */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_DISTORSP),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_5 | DRAW_GOURAU | COMPO_REP),
    /* color    */ 0,
    /* charAddr */ 0x0080,  /* 1024 byte pos */
    /* charSize */ 0x0108,  /* x*y = 8*8     */
    /* ax, ay   */  90,  90,
    /* bx, by   */  90+50,  90,
    /* cx, cy   */  90+50,  90+50,
    /* dx, dy   */  90,  90+50,
    /* grshAddr */ 0x0100,  /* 2048 byte pos */
    /* dummy    */   0},


   {/* END      */
    /* control  */ CTRL_END,
    /* link     */ 0,
    /* drawMode */ 0,
    /* color    */ 0,
    /* charAddr */ 0,
    /* charSize */ 0,
    /* ax, ay   */ 0,
    /* bx, by   */ 0,
    /* cx, cy   */ 0,
    /* dx, dy   */ 0,
    /* grshAddr */ 0,
    /* dummy    */ 0}
};


static Uint16 gourauTbl[] = { 	/*Define gouraud shading table */
     /*         0      1      2      3     */
     /* 0 */ RGB16_COLOR(0,0,0),    RGB16_COLOR(9,9,9),
             RGB16_COLOR(18,18,18), RGB16_COLOR(27,27,27),
     /* 1 */ RGB16_COLOR(0,0,0),    RGB16_COLOR(9,9,9),
             RGB16_COLOR(18,18,18), RGB16_COLOR(27,27,27),
     /*END*/ 0xffff,0xffff,0xffff,0xffff};

static Uint16  colorData[] = {
/* 0x00 */  0x0000,
/* 0x01 */  0x801f,
/* 0x02 */  0x82b5,
/* 0x03 */  0x83e0,
/* 0x04 */  0xfc00,
/* 0x05 */  0xffe0,
/* 0x06 */  0xfc1f,
/* 0x07 */  0x8000,
/* 0x08 */  0xffff,
/* 0x09 */  RGB16_COLOR(31,31,31),
/* 0x0a */  RGB16_COLOR(31,31,31),
/* 0x0b */  RGB16_COLOR(31,31,31),
/* 0x0c */  RGB16_COLOR(31,31,31),
/* 0x0d */  RGB16_COLOR(31,31,31),
/* 0x0e */  RGB16_COLOR(31,31,31),
/* 0x0f */  RGB16_COLOR(31,31,31),
/* 0x10 */  RGB16_COLOR(10,20,30),
/* 0x11 */  RGB16_COLOR(31, 0, 0),
/* 0x12 */  RGB16_COLOR(31,31, 0),
/* 0x13 */  RGB16_COLOR(31, 0,31),
/* 0x14 */  RGB16_COLOR( 0,31,31),
/* 0x15 */  RGB16_COLOR( 0, 0,31),
/* 0x16 */  RGB16_COLOR( 0,31, 0),
/* 0x17 */  RGB16_COLOR(31,31,31),
/* 0x18 */  RGB16_COLOR(31,31,31),
/* 0x19 */  RGB16_COLOR(31,31,31),
/* 0x1a */  RGB16_COLOR(31,31,31),
/* 0x1b */  RGB16_COLOR(31,31,31),
/* 0x1c */  RGB16_COLOR(31,31,31),
/* 0x1d */  RGB16_COLOR(31,31,31),
/* 0x1e */  RGB16_COLOR(31,31,31),
/* 0x1f */  RGB16_COLOR( 0, 0, 0),
};


void __main(){}

Sint32 main()
{
    Uint8  *VRAM;
    Uint16 *colAddr,i;
	Uint32 rotAngle;
	Fixed32 tCos,tSin,tx,ty;
	SprSpCmd *sp,*vPtr;
	Uint32 offx,offy;
	char myChar;
	GfsHn gfs;
	Sint32 fid;
	Sint32 ret;


    set_imask(0);  /* macro */


    SCL_Vdp2Init();
    SCL_SetPriority(SCL_SP0|SCL_SP1|SCL_SP2|SCL_SP3|
                    SCL_SP4|SCL_SP5|SCL_SP6|SCL_SP7,7);
    SCL_SetSpriteMode(SCL_TYPE1,SCL_MIX,SCL_SP_WINDOW);
    SCL_SetColRamMode(SCL_CRM15_2048);

    SetVblank();

    SPR_Initial(&VRAM);
    SCL_SetFrameInterval(-1);

    SCL_DisplayFrame();
    SCL_DisplayFrame();
    colAddr = (Uint16*)SCL_AllocColRam(SCL_SPR,32,OFF);
    SCL_SetColRam(SCL_SPR,0,32,colorData);

    SCL_SetDisplayMode(SCL_NON_INTER,SCL_240LINE,SCL_NORMAL_A);
    SPR_SetTvMode(SPR_TV_NORMAL,SPR_TV_320X240,OFF);
    SPR_SetEraseData(0x8000,0,0,320-1,240-1);

	GFS_DIRTBL_TYPE(&dirtbl) = GFS_DIR_ID;
	GFS_DIRTBL_NDIR(&dirtbl) = MAX_DIR;
	GFS_DIRTBL_DIRID(&dirtbl) = dirid;

	ret = GFS_Init(OPEN_MAX,work,&dirtbl);
    if (ret <= 2) {
	return 1;
    }

    gfs = GFS_Open(3);
	if (gfs == NULL) return 2;

    GFS_Fread(gfs, ((2*160*160)/SECT_SIZE), (void *)tut160, (2*160*160)+4);
    GFS_Close(gfs);

	for (i = 2; i < ((160*160) +2 ); i++)
	 tut160[i] |= 0x8000; /* set the 15th bit */


    SCL_SetFrameInterval(1);
    memcpy(VRAM,spCmd16_320_240,sizeof(spCmd16_320_240));
    memcpy(VRAM+1024,&tut160[2],2*160*160);
    /* memcpy(VRAM+2048,gourauTbl,sizeof(gourauTbl)); */

	rotAngle = 0;


    for(i=0; i < 360; i++)
    {


	   sp = (SprSpCmd *) 0x5c00040;
	 
	   tCos = cosine(rotAngle);
	   tSin = sine(rotAngle);

	   rotAngle -= (0xb60b60); /* 1/360 of 4 billion */

		sp->control = (JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_DISTORSP);
        sp->link = 0;
		sp->drawMode = (ECDSPD_DISABLE | COLOR_5 | COMPO_REP);
		sp->color = 0;
		sp->charAddr = 0x0080;  /* 1024 byte pos */
        sp->charSize = 0x14a0;  /* x*y = 160*160 */
        sp->grshAddr = 0x6400;  /* 2048 byte pos */

	    offx = 40+(20*i);
	    offx = 160;
	    offy = 120;


	    tx = MTH_FIXED(0-80);
	    ty = tx;
	    sp->ax = offx + MTH_FixedToInt((MTH_Mul(tCos,tx)) - (MTH_Mul(tSin,ty)));
	    sp->ay = offy + MTH_FixedToInt((MTH_Mul(tSin,tx)) + (MTH_Mul(tCos,ty)));


	    tx = MTH_FIXED(160-80);
	    sp->bx = offx + MTH_FixedToInt((MTH_Mul(tCos,tx)) - (MTH_Mul(tSin,ty)));
	    sp->by = offy + MTH_FixedToInt((MTH_Mul(tSin,tx)) + (MTH_Mul(tCos,ty)));


	    ty = tx;
	    sp->cx = offx + MTH_FixedToInt((MTH_Mul(tCos,tx)) - (MTH_Mul(tSin,ty)));
	    sp->cy = offy + MTH_FixedToInt((MTH_Mul(tSin,tx)) + (MTH_Mul(tCos,ty)));


	    tx = MTH_FIXED(0-80);
	    sp->dx = offx + MTH_FixedToInt((MTH_Mul(tCos,tx)) - (MTH_Mul(tSin,ty)));
	    sp->dy = offy + MTH_FixedToInt((MTH_Mul(tSin,tx)) + (MTH_Mul(tCos,ty)));


      SCL_DisplayFrame();


    }

}

/*  end of file */
