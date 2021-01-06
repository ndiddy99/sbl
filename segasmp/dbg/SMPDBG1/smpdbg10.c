/*----------------------------------------------------------------------------
 *  smpdbg10.c -- スプライトサンプル２
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-02-15 Ver.0.80
 *  Updated by H.E on 1994-02-15 Ver.0.80
 *
 *  スプライト表示基本処理のテスト
 *  各種スプライトコマンドの実行。
 *
 *----------------------------------------------------------------------------
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<machine.h>

#define		_SPR2_
#include        "sega_spr.h"
#include        "sega_scl.h"
#include        "sega_dbg.h"
#include        "sega_per.h"

#define   DISP_XSIZE       320
#define   DISP_YSIZE       224

static SprSpCmd  spCmd[] = {
   {/* [ 0]     */
    /* control  */ (JUMP_NEXT | FUNC_SCLIP),
    /* link     */ 0,
    /* drawMode */ 0,
    /* color    */ 0,
    /* charAddr */ 0,
    /* charSize */ 0,
    /* ax, ay   */ 0, 0,
    /* bx, by   */ 0, 0,
    /* cx, cy   */ DISP_XSIZE-1, DISP_YSIZE-1,
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

   {/* [ 2]     */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_SCALESP),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_5 | COMPO_REP),
    /* color    */ 0,
    /* charAddr */ 0x0080,  /* 1024 byte pos */
    /* charSize */ 0x0108,  /* x*y = 8*8     */
    /* ax, ay   */  10, 160,
    /* bx, by   */   0,   0,
    /* cx, cy   */  60, 210,
    /* dx, dy   */   0,   0,
    /* grshAddr */   0,
    /* dummy    */   0},

   {/* [ 3]     */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_LRREV | FUNC_SCALESP),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_5 | COMPO_REP),
    /* color    */ 0,
    /* charAddr */ 0x0080,  /* 1024 byte pos */
    /* charSize */ 0x0108,  /* x*y = 8*8     */
    /* ax, ay   */  70, 160,
    /* bx, by   */   0,   0,
    /* cx, cy   */ 120, 210,
    /* dx, dy   */   0,   0,
    /* grshAddr */   0,
    /* dummy    */   0},

   {/* [ 4]     */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_TBREV | FUNC_SCALESP),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_5 | COMPO_REP),
    /* color    */ 0,
    /* charAddr */ 0x0080,  /* 1024 byte pos */
    /* charSize */ 0x0108,  /* x*y = 8*8     */
    /* ax, ay   */ 130, 160,
    /* bx, by   */   0,   0,
    /* cx, cy   */ 180, 210,
    /* dx, dy   */   0,   0,
    /* grshAddr */   0,
    /* dummy    */   0},

   {/* [ 5]     */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_LRTBREV | FUNC_SCALESP),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_5 | COMPO_REP),
    /* color    */ 0,
    /* charAddr */ 0x0080,  /* 1024 byte pos */
    /* charSize */ 0x0108,  /* x*y = 8*8     */
    /* ax, ay   */ 190, 160,
    /* bx, by   */   0,   0,
    /* cx, cy   */ 240, 210,
    /* dx, dy   */   0,   0,
    /* grshAddr */   0,
    /* dummy    */   0},

   {/* [ 6]     */
    /* control  */ (CTRL_SKIP | JUMP_ASSIGN),
    /* link     */ 10 *4,
    /* drawMode */ 0,
    /* color    */ 0,
    /* charAddr */ 0,
    /* charSize */ 0,
    /* ax, ay   */ 0,
    /* bx, by   */ 0,
    /* cx, cy   */ 0,
    /* dx, dy   */ 0,
    /* grshAddr */ 0,
    /* dummy    */ 0},

   {/* [ 7]     */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_SCALESP),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_5 | COMPO_TRANS),
    /* color    */ 0,
    /* charAddr */ 0x0080,  /* 1024 byte pos */
    /* charSize */ 0x0108,  /* x*y = 8*8     */
    /* ax, ay   */  30, 130,
    /* bx, by   */   0,   0,
    /* cx, cy   */  80, 180,
    /* dx, dy   */   0,   0,
    /* grshAddr */   0,
    /* dummy    */   0},

   {/* [ 8]     */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_SCALESP),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_5 | COMPO_HARF),
    /* color    */ 0,
    /* charAddr */ 0x0080,  /* 1024 byte pos */
    /* charSize */ 0x0108,  /* x*y = 8*8     */
    /* ax, ay   */  90, 130,
    /* bx, by   */   0,   0,
    /* cx, cy   */ 140, 180,
    /* dx, dy   */   0,   0,
    /* grshAddr */   0,
    /* dummy    */   0},

   {/* [ 9]     */
    /* control  */ (JUMP_RETURN | ZOOM_NOPOINT | DIR_NOREV | FUNC_POLYGON),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_0 | COMPO_SHADOW),
    /* color    */ RGB16_COLOR(31,31,0),
    /* charAddr */ 0,
    /* charSize */ 0,
    /* ax, ay   */ 150, 130,
    /* bx, by   */ 200, 130,
    /* cx, cy   */ 200, 180,
    /* dx, dy   */ 150, 180,
    /* grshAddr */ 0,
    /* dummy    */   0},

   {/* [10]     */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_DISTORSP),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_5 | DRAW_GOURAU | COMPO_REP),
    /* color    */ 0,
    /* charAddr */ 0x0080,  /* 1024 byte pos */
    /* charSize */ 0x0108,  /* x*y = 8*8     */
    /* ax, ay   */  20,  20,
    /* bx, by   */  80,  10,
    /* cx, cy   */  60,  70,
    /* dx, dy   */  10,  40,
    /* grshAddr */ 0x0100,  /* 2048 byte pos */
    /* dummy    */   0},

   {/* [11]     */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_POLYGON),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_0 | COMPO_REP),
    /* color    */ RGB16_COLOR(31,31,0),
    /* charAddr */ 0,
    /* charSize */ 0,
    /* ax, ay   */ 100,  20,
    /* bx, by   */ 160,  10,
    /* cx, cy   */ 140,  70,
    /* dx, dy   */  90,  40,
    /* grshAddr */ 0,
    /* dummy    */   0},

   {/* [12]     */
    /* control  */ (CTRL_SKIP | JUMP_CALL),
    /* link     */ 7 *4,
    /* drawMode */ 0,
    /* color    */ 0,
    /* charAddr */ 0,
    /* charSize */ 0,
    /* ax, ay   */ 0,
    /* bx, by   */ 0,
    /* cx, cy   */ 0,
    /* dx, dy   */ 0,
    /* grshAddr */ 0,
    /* dummy    */ 0},

   {/* [13]     */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_POLYGON),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_0 | DRAW_GOURAU | COMPO_REP),
    /* color    */ RGB16_COLOR(15,15,15),
    /* charAddr */ 0,
    /* charSize */ 0,
    /* ax, ay   */ 180,  20,
    /* bx, by   */ 240,  10,
    /* cx, cy   */ 220,  70,
    /* dx, dy   */ 170,  40,
    /* grshAddr */ 0x0100,  /* 2048 byte pos */
    /* dummy    */ 0},

   {/* [14]     */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_POLYGON),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_0 | DRAW_GOURAU | COMPO_REP),
    /* color    */ RGB16_COLOR(15,15,15),
    /* charAddr */ 0,
    /* charSize */ 0,
    /* ax, ay   */ 180, 120,
    /* bx, by   */ 240, 110,
    /* cx, cy   */ 220, 170,
    /* dx, dy   */ 170, 140,
    /* grshAddr */ 0x0101,  /* 2048 byte pos */
    /* dummy    */ 0},

   {/* [15]     */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_POLYLINE),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_0 | COMPO_REP),
    /* color    */ 0x83e0,
    /* charAddr */ 0,
    /* charSize */ 0,
    /* ax, ay   */ 270,  20,
    /* bx, by   */ 320,  10,
    /* cx, cy   */ 310,  70,
    /* dx, dy   */ 260,  40,
    /* grshAddr */ 0,
    /* dummy    */ 0},

   {/* [16]     */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_LINE),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_0 | COMPO_REP),
    /* color    */ 0x801f,
    /* charAddr */ 0,
    /* charSize */ 0,
    /* ax, ay   */ 270,  80,
    /* bx, by   */ 320,  90,
    /* cx, cy   */   0,   0,
    /* dx, dy   */   0,   0,
    /* grshAddr */ 0,
    /* dummy    */ 0},

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

static Uint16 charImage1[] = { 	/*Define texture image */
/*         0      1      2      3      4      5      6      7   */
/* 0 */ 0x801f,0x801f,0x801f,0x82b5,0x801f,0x801f,0x801f,0x801f,

/* 1 */ 0x83e0,0x83e0,0x82b5,0x82b5,0x82b5,0x83e0,0x83e0,0x83e0,

/* 2 */ 0xfc00,0x82b5,0x82b5,0x82b5,0x82b5,0x82b5,0xfc00,0xfc00,

/* 3 */ 0x82b5,0x82b5,0x82b5,0x82b5,0x82b5,0x82b5,0x82b5,0xfc00,

/* 4 */ 0xffe0,0xffe0,0xffe0,0x82b5,0xffe0,0xffe0,0xffe0,0xffe0,

/* 5 */ 0xfc1f,0xfc1f,0xfc1f,0x82b5,0xfc1f,0x82b5,0x82b5,0xfc1f,

/* 6 */ 0x8000,0x8000,0x8000,0x82b5,0x8000,0x82b5,0x82b5,0x8000,

/* 7 */ 0xffff,0xffff,0xffff,0x82b5,0xffff,0xffff,0xffff,0xffff
};


static Uint16 gourauTbl[] = { 	/*Define gouraud shading table */
     /*         0      1      2      3     */
     /* 0 */ RGB16_COLOR(0,0,0),    RGB16_COLOR(9,9,9),
             RGB16_COLOR(18,18,18), RGB16_COLOR(27,27,27),
     /* 1 */ RGB16_COLOR(0,0,0),    RGB16_COLOR(9,9,9),
             RGB16_COLOR(18,18,18), RGB16_COLOR(27,27,27),
     /*END*/ 0xffff,0xffff,0xffff,0xffff};

extern volatile Uint16	PadData1;

void main()
{
    Uint8  *VRAM;
    int    i, j, l;
    char   keyStr[64], name[64];
    Uint32    val;

    SCL_Vdp2Init();
    SetVblank();
    set_imask(0);

    SCL_SetPriority(SCL_SP0|SCL_SP1|SCL_SP2|SCL_SP3|
                    SCL_SP4|SCL_SP5|SCL_SP6|SCL_SP7,6);
    SCL_SetSpriteMode(SCL_TYPE1,SCL_MIX,SCL_SP_WINDOW);

    SPR_Initial(&VRAM);

    SPR_SetEraseData(RGB16_COLOR(0,0,0),0,0,DISP_XSIZE-1,DISP_YSIZE-1);
    SCL_SetFrameInterval(1); 
    memcpy(VRAM,spCmd,sizeof(spCmd));
    memcpy(VRAM+1024,charImage1,sizeof(charImage1));
    memcpy(VRAM+2048,gourauTbl,sizeof(gourauTbl));
    DBG_Initial(&PadData1,RGB16_COLOR(31,31,31),0);
    l = 0;
    for(;;) {
        DBG_DisplayOn();
        DBG_Printf("Key In ? (name  val)\n");
        sscanf(DBG_GetKeyStr(keyStr),"%s %d", name, &val);
        for(j=0; j<200; j++) {
            SCL_DisplayFrame();
            for(i=0; i<10; i++) {
                /* DBG_SetCursol(10,5); */
                DBG_Printf("* name[%s] val[%d] l=%d\n", name, val, l++);
            }
        }
        DBG_DisplayOff();
        for(j=0; j<200; j++) {
            SCL_DisplayFrame();
            for(i=0; i<10; i++) {
                /* DBG_SetCursol(10,5); */
                DBG_Printf("* name[%s] val[%d] l=%d\n", name, val, l++);
            }
        }
        if(val == 98)
            DBG_Monitor();
        if(val == 99)
            *(Uint32*)0x6000001 = val; /* cause program error */
    }
}

/*  end of file */
