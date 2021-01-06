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
#include        "sega_spr.h"
#include        "sega_dbg.h"
#include        "sega_per.h"

#include	<machine.h>
#include        "sega_scl.h"
#include	"../../v_blank/v_blank.h"


#define   COLOR_BANK       0x0000

static SprSpCmd  spCmd16_320_240[] = {
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
    /* color    */ RGB16_COLOR(10,20,30),
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
    /* color    */ RGB16_COLOR(31,0,0),
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
    /* color    */ RGB16_COLOR(31,31,0),
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
    /* color    */ RGB16_COLOR(31,0,31),
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
    /* color    */ RGB16_COLOR(0,31,31),
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
    /* color    */ RGB16_COLOR( 0, 0,31),
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

static SprSpCmd  spCmd8_320_240[] = {
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

   {/* [ 2]     */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_SCALESP),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ COLOR_BANK,
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
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ COLOR_BANK,
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
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ COLOR_BANK,
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
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ COLOR_BANK,
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
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ COLOR_BANK,
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
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ COLOR_BANK,
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
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ 16,
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
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ COLOR_BANK,
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
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ 17,
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
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ 18,
    /* charAddr */ 0,
    /* charSize */ 0,
    /* ax, ay   */ 180,  20,
    /* bx, by   */ 240,  10,
    /* cx, cy   */ 220,  70,
    /* dx, dy   */ 170,  40,
    /* grshAddr */ 0,
    /* dummy    */ 0},

   {/* [14]     */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_POLYGON),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ 19,
    /* charAddr */ 0,
    /* charSize */ 0,
    /* ax, ay   */ 180, 120,
    /* bx, by   */ 240, 110,
    /* cx, cy   */ 220, 170,
    /* dx, dy   */ 170, 140,
    /* grshAddr */ 0,
    /* dummy    */ 0},

   {/* [15]     */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_POLYLINE),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ 20,
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
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ 21,
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

static SprSpCmd  spCmd8_640_480[] = {
   {/* [ 0]     */
    /* control  */ (JUMP_NEXT | FUNC_SCLIP),
    /* link     */ 0,
    /* drawMode */ 0,
    /* color    */ 0,
    /* charAddr */ 0,
    /* charSize */ 0,
    /* ax, ay   */ 0, 0,
    /* bx, by   */ 0, 0,
    /* cx, cy   */ 640-1, 480-1,
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
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ COLOR_BANK,
    /* charAddr */ 0x0080,  /* 1024 byte pos */
    /* charSize */ 0x0108,  /* x*y = 8*8     */
    /* ax, ay   */  20, 320,
    /* bx, by   */   0,   0,
    /* cx, cy   */ 120, 420,
    /* dx, dy   */   0,   0,
    /* grshAddr */   0,
    /* dummy    */   0},

   {/* [ 3]     */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_LRREV | FUNC_SCALESP),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ COLOR_BANK,
    /* charAddr */ 0x0080,  /* 1024 byte pos */
    /* charSize */ 0x0108,  /* x*y = 8*8     */
    /* ax, ay   */ 140, 320,
    /* bx, by   */   0,   0,
    /* cx, cy   */ 240, 420,
    /* dx, dy   */   0,   0,
    /* grshAddr */   0,
    /* dummy    */   0},

   {/* [ 4]     */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_TBREV | FUNC_SCALESP),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ COLOR_BANK,
    /* charAddr */ 0x0080,  /* 1024 byte pos */
    /* charSize */ 0x0108,  /* x*y = 8*8     */
    /* ax, ay   */ 260, 320,
    /* bx, by   */   0,   0,
    /* cx, cy   */ 360, 420,
    /* dx, dy   */   0,   0,
    /* grshAddr */   0,
    /* dummy    */   0},

   {/* [ 5]     */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_LRTBREV | FUNC_SCALESP),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ COLOR_BANK,
    /* charAddr */ 0x0080,  /* 1024 byte pos */
    /* charSize */ 0x0108,  /* x*y = 8*8     */
    /* ax, ay   */ 380, 320,
    /* bx, by   */   0,   0,
    /* cx, cy   */ 480, 420,
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
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ COLOR_BANK,
    /* charAddr */ 0x0080,  /* 1024 byte pos */
    /* charSize */ 0x0108,  /* x*y = 8*8     */
    /* ax, ay   */  60, 260,
    /* bx, by   */   0,   0,
    /* cx, cy   */ 160, 360,
    /* dx, dy   */   0,   0,
    /* grshAddr */   0,
    /* dummy    */   0},

   {/* [ 8]     */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_SCALESP),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ COLOR_BANK,
    /* charAddr */ 0x0080,  /* 1024 byte pos */
    /* charSize */ 0x0108,  /* x*y = 8*8     */
    /* ax, ay   */ 180, 260,
    /* bx, by   */   0,   0,
    /* cx, cy   */ 280, 360,
    /* dx, dy   */   0,   0,
    /* grshAddr */   0,
    /* dummy    */   0},

   {/* [ 9]     */
    /* control  */ (JUMP_RETURN | ZOOM_NOPOINT | DIR_NOREV | FUNC_POLYGON),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ 16,
    /* charAddr */ 0,
    /* charSize */ 0,
    /* ax, ay   */ 300, 260,
    /* bx, by   */ 400, 260,
    /* cx, cy   */ 400, 360,
    /* dx, dy   */ 300, 360,
    /* grshAddr */ 0,
    /* dummy    */   0},

   {/* [10]     */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_DISTORSP),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ COLOR_BANK,
    /* charAddr */ 0x0080,  /* 1024 byte pos */
    /* charSize */ 0x0108,  /* x*y = 8*8     */
    /* ax, ay   */  40,  40,
    /* bx, by   */ 160,  20,
    /* cx, cy   */ 120, 140,
    /* dx, dy   */  20,  80,
    /* grshAddr */ 0x0100,  /* 2048 byte pos */
    /* dummy    */   0},

   {/* [11]     */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_POLYGON),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ 17,
    /* charAddr */ 0,
    /* charSize */ 0,
    /* ax, ay   */ 200,  40,
    /* bx, by   */ 320,  20,
    /* cx, cy   */ 280, 140,
    /* dx, dy   */ 180,  80,
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
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ 18,
    /* charAddr */ 0,
    /* charSize */ 0,
    /* ax, ay   */ 360,  40,
    /* bx, by   */ 480,  20,
    /* cx, cy   */ 440, 140,
    /* dx, dy   */ 340,  80,
    /* grshAddr */ 0,
    /* dummy    */ 0},

   {/* [14]     */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_POLYGON),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ 19,
    /* charAddr */ 0,
    /* charSize */ 0,
    /* ax, ay   */ 360, 240,
    /* bx, by   */ 480, 220,
    /* cx, cy   */ 440, 340,
    /* dx, dy   */ 340, 280,
    /* grshAddr */ 0,
    /* dummy    */ 0},

   {/* [15]     */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_POLYLINE),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ 20,
    /* charAddr */ 0,
    /* charSize */ 0,
    /* ax, ay   */ 540,  40,
    /* bx, by   */ 640,  20,
    /* cx, cy   */ 620, 140,
    /* dx, dy   */ 520,  80,
    /* grshAddr */ 0,
    /* dummy    */ 0},

   {/* [16]     */
    /* control  */ (JUMP_NEXT | ZOOM_NOPOINT | DIR_NOREV | FUNC_LINE),
    /* link     */ 0,
    /* drawMode */ (ECDSPD_DISABLE | COLOR_4 | COMPO_REP),
    /* color    */ 21,
    /* charAddr */ 0,
    /* charSize */ 0,
    /* ax, ay   */ 540, 160,
    /* bx, by   */ 640, 180,
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

static Uint8 charImage8[] = { 	/*Define texture image */
/*        0    1    2    3    4    5    6    7   */
/* 0 */ 0x01,0x01,0x01,0x02,0x01,0x01,0x01,0x01,
/* 1 */ 0x03,0x03,0x02,0x02,0x02,0x03,0x03,0x03,
/* 2 */ 0x04,0x02,0x02,0x02,0x02,0x02,0x04,0x04,
/* 3 */ 0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x04,
/* 4 */ 0x05,0x05,0x05,0x02,0x05,0x05,0x05,0x05,
/* 5 */ 0x06,0x06,0x06,0x02,0x06,0x02,0x02,0x06,
/* 6 */ 0x07,0x07,0x07,0x02,0x07,0x02,0x02,0x07,
/* 7 */ 0x08,0x08,0x08,0x02,0x08,0x08,0x08,0x08};

static Uint16 charImage16[] = { 	/*Define texture image */
/*         0      1      2      3      4      5      6      7   */
/* 0 */ 0x801f,0x801f,0x801f,0x82b5,0x801f,0x801f,0x801f,0x801f,
/* 1 */ 0x83e0,0x83e0,0x82b5,0x82b5,0x82b5,0x83e0,0x83e0,0x83e0,
/* 2 */ 0xfc00,0x82b5,0x82b5,0x82b5,0x82b5,0x82b5,0xfc00,0xfc00,
/* 3 */ 0x82b5,0x82b5,0x82b5,0x82b5,0x82b5,0x82b5,0x82b5,0xfc00,
/* 4 */ 0xffe0,0xffe0,0xffe0,0x82b5,0xffe0,0xffe0,0xffe0,0xffe0,
/* 5 */ 0xfc1f,0xfc1f,0xfc1f,0x82b5,0xfc1f,0x82b5,0x82b5,0xfc1f,
/* 6 */ 0x8000,0x8000,0x8000,0x82b5,0x8000,0x82b5,0x82b5,0x8000,
/* 7 */ 0xfffe,0xfffe,0xfffe,0x82b5,0xffff,0xffff,0xffff,0xffff
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


void main()
{
    Uint8  *VRAM;
    Uint16 *colAddr;
    int    asw;
    
    set_imask(0);

    SCL_Vdp2Init();
    SCL_SetPriority(SCL_SP0|SCL_SP1|SCL_SP2|SCL_SP3|
                    SCL_SP4|SCL_SP5|SCL_SP6|SCL_SP7,7);
    SCL_SetSpriteMode(SCL_TYPE1,SCL_MIX,SCL_SP_WINDOW);
    SCL_SetColRamMode(SCL_CRM15_2048);

    SetVblank();
#if 0
    DBG_Initial(&PAD_info_play1,RGB16_COLOR(31,31,31),0);
    DBG_DisplayOn();
    DBG_Printf("colAddr=%08lxH\n",colAddr);
#endif
    SPR_Initial(&VRAM);
    SCL_SetFrameInterval(-1); 

    SCL_DisplayFrame();
    SCL_DisplayFrame();
    colAddr = (Uint16*)SCL_AllocColRam(SCL_SPR,32,OFF);
    SCL_SetColRam(SCL_SPR,0,32,colorData);

    SCL_SetDisplayMode(SCL_NON_INTER,SCL_240LINE,SCL_NORMAL_A);
    SPR_SetTvMode(SPR_TV_NORMAL,SPR_TV_320X240,OFF);
    SPR_SetEraseData(0x8000,0,0,320-1,240-1);

    SCL_SetFrameInterval(0); 
    memcpy(VRAM,spCmd16_320_240,sizeof(spCmd16_320_240));
    memcpy(VRAM+1024,charImage16,16*8);
    memcpy(VRAM+2048,gourauTbl,sizeof(gourauTbl));
    asw = 0;
    for(;;) {
        switch(PadData1) {
	    case PAD_A:
                 ++asw;
                 if(asw > 2) asw = 0;
                 switch(asw) {
                     case 0 :
                         memcpy(VRAM,spCmd16_320_240,sizeof(spCmd16_320_240));
                         memcpy(VRAM+1024,charImage16,16*8);
                         SCL_Vdp2Init();
                         SCL_SetPriority(SCL_SP0|SCL_SP1|SCL_SP2|SCL_SP3|
                                         SCL_SP4|SCL_SP5|SCL_SP6|SCL_SP7,7);
                         SCL_SetSpriteMode(SCL_TYPE1,SCL_MIX,SCL_SP_WINDOW);
                         SCL_SetColRamMode(SCL_CRM15_2048);
                         SCL_SetFrameInterval(0); 
                         SCL_SetDisplayMode(SCL_NON_INTER,SCL_240LINE,
                                                                SCL_NORMAL_A);
                         SPR_SetTvMode(SPR_TV_NORMAL,SPR_TV_320X240,OFF);
                         SPR_SetEraseData(0x8000,0,0,320-1,240-1);
                         break;

                     case 1 :
                         memcpy(VRAM,spCmd8_320_240,sizeof(spCmd8_320_240));
                         memcpy(VRAM+1024,charImage8,8*8);
                         SCL_Vdp2Init();
                         SCL_SetPriority(SCL_SP0|SCL_SP1|SCL_SP2|SCL_SP3|
                                         SCL_SP4|SCL_SP5|SCL_SP6|SCL_SP7,7);
                         SCL_SetSpriteMode(SCL_TYPE8,SCL_PALETTE,
                                                            SCL_SP_WINDOW);
                         SCL_SetColRamMode(SCL_CRM15_2048);
                         SCL_SetFrameInterval(0); 
                         SCL_SetDisplayMode(SCL_NON_INTER,SCL_240LINE,
                                                                SCL_NORMAL_A);
                         SPR_SetTvMode(SPR_TV_NORMAL,SPR_TV_320X240,OFF);
                         SPR_SetEraseData(0x1f1f,0,0,320-1,240-1);
                         break;

                     case 2 :
                         memcpy(VRAM,spCmd8_640_480,sizeof(spCmd8_640_480));
                         memcpy(VRAM+1024,charImage8,8*8);
                         SCL_Vdp2Init();
                         SCL_SetPriority(SCL_SP0|SCL_SP1|SCL_SP2|SCL_SP3|
                                         SCL_SP4|SCL_SP5|SCL_SP6|SCL_SP7,7);
                         SCL_SetSpriteMode(SCL_TYPE8,SCL_PALETTE,
                                                            SCL_SP_WINDOW);
                         SCL_SetColRamMode(SCL_CRM15_2048);
                         SCL_SetFrameInterval(0); 
                         SCL_SetDisplayMode(SCL_DOUBLE_INTER,SCL_240LINE,
                                                             SCL_HIRESO_A);
                         SPR_SetTvMode(SPR_TV_HIRESO,SPR_TV_640X240,ON);
                         SPR_SetEraseData(0x1f1f,0,0,640-1,480-1);
                         break;
                 }
                 break;

	    case PAD_B:
                 memcpy(VRAM,spCmd8_320_240,sizeof(spCmd8_320_240));
                 memcpy(VRAM+1024,charImage8,8*8);
                 SCL_Vdp2Init();
                 SCL_SetPriority(SCL_SP0|SCL_SP1|SCL_SP2|SCL_SP3|
                    SCL_SP4|SCL_SP5|SCL_SP6|SCL_SP7,7);
                 SCL_SetSpriteMode(SCL_TYPE8,SCL_PALETTE,SCL_SP_WINDOW);
                 SCL_SetColRamMode(SCL_CRM15_2048);
                 SCL_SetFrameInterval(0); 
                 SCL_SetDisplayMode(SCL_NON_INTER,SCL_240LINE,SCL_HIRESO_A);
                 SPR_SetTvMode(SPR_TV_HIRESO,SPR_TV_640X240,OFF);
                 SPR_SetEraseData(0x1f1f,0,0,640-1,240-1);
                 break;

	    case PAD_C:
                 memcpy(VRAM,spCmd8_320_240,sizeof(spCmd8_320_240));
                 memcpy(VRAM+1024,charImage8,8*8);
                 SCL_Vdp2Init();
                 SCL_SetPriority(SCL_SP0|SCL_SP1|SCL_SP2|SCL_SP3|
                    SCL_SP4|SCL_SP5|SCL_SP6|SCL_SP7,7);
                 SCL_SetSpriteMode(SCL_TYPE8,SCL_PALETTE,SCL_SP_WINDOW);
                 SCL_SetColRamMode(SCL_CRM15_2048);
                 SCL_SetFrameInterval(0); 
                 SCL_SetDisplayMode(SCL_DOUBLE_INTER,SCL_240LINE,SCL_HIRESO_A);
                 SPR_SetTvMode(SPR_TV_HIRESO,SPR_TV_640X240,ON);
                 SPR_SetEraseData(0x1f1f,0,0,640-1,480-1);
                 break;
        }
        SCL_DisplayFrame();
    }
}

/*  end of file */
