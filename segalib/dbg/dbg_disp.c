/*----------------------------------------------------------------------------
 *  dbg_disp.c -- DBG ライブラリ Disp モジュール
 *  Copyright(c) 1994 SEGA
 *  Written by H.E on 1994-05-23 Ver.0.90
 *  Updated by H.E on 1994-07-25 Ver.1.00
 *
 *  このライブラリはデバッグサポートライブラリで、以下のルーチンを含む。
 *
 *  DBG_Initial       -  デバッグ環境の初期化
 *  DBG_DisplayOn     -  デバッグ画面の表示ＯＮ
 *  DBG_DisplayOff    -  デバッグ画面の表示ＯＦＦ
 *  DBG_ClearScreen   -  デバッグ画面のクリア
 *  DBG_SetCursol     -  デバッグ画面カーソルセット
 *  DBG_Printf        -  デバッグ画面に文字列表示
 *  DBG_Scanf         -  ソフトウェアキーボードによるキー入力処理
 *
 *----------------------------------------------------------------------------
 */

/*
 * C STANDARD LIBRARY FUNCTIONS/MACROS DEFINES
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include <machine.h>
#include "sega_dbg.h"
#include "sega_mth.h"
#include "sega_scl.h"


#define   SCR_X         40
#define   SCR_Y         28
#define   KEYWIN_X      18
#define   KEYWIN_Y      8
#define   FONT_COUNT    (16+96)

/*
 * EXTERNAL DECLARATIONS
 */

/* 例外処理エントリ */
void DBG_IntrGenOpErr(void);        /*  一般不当命令処理          */
void DBG_IntrSlotOpErr(void);       /*  スロット不当命令処理      */
void DBG_IntrCpuAddrErr(void);      /*  ＣＰＵアドレスエラー処理  */
void DBG_IntrDmaAddrErr(void);      /*  ＤＭＡアドレスエラー処理  */

/* スクロールレジスタバッファ */
extern	SclSysreg	Scl_s_reg;
extern	SclDataset	Scl_d_reg;
extern	SclNorscl	Scl_n_reg;
extern	SclRotscl	Scl_r_reg;
extern	SclWinscl	Scl_w_reg;
int  DBG_DelayCount;

struct WinDef {
    int     width;
    int     height;
    int     ltX;
    int     ltY;
    Uint16  *saveBuf;
};
typedef struct WinDef WinDef;

/*
 * STATIC DECLARATIONS
 */
#include "dbg_font.h"

/* スクロールレジスタバッファセーブエリア */
static	SclSysreg	saveScl_s_reg;
static	SclDataset	saveScl_d_reg;
static	SclNorscl	saveScl_n_reg;
static	SclRotscl	saveScl_r_reg;
static	SclWinscl	saveScl_w_reg;

/* プライオリティレジスタバッファセーブエリア */
static SclOtherPriRegister	saveSclOtherPri;
static SclSpPriNumRegister	saveSclSpPriNum;
static SclBgPriNumRegister	saveSclBgPriNum;
static SclSpColMixRegister	saveSclSpColMix;
static SclBgColMixRegister	saveSclBgColMix;
static SclColOffsetRegister	saveSclColOffset;

static Uint32  sprSW = 0;
static Uint32  sclSW = 0;
static int     befKeyX = 1;
static int     befKeyY = 1;
static Uint8   sprPrty;
static Uint8   sclNbg1Prty;
static Uint8   sclNbg2Prty;
static Uint8   sclNbg3Prty;
static Uint8   sclRbg0Prty;
static Uint32  fontArea[FONT_COUNT*2][8];   /* Font Data Area                */
static Uint16  mapArea[SCR_Y][SCR_X];       /* Map Data Area                 */
static Uint32  colorArea32[4][2];           /* 32 bit Color Data Area        */
static Uint16  colorArea16[4][2];           /* 16 bit Color Data Area        */
static Uint32  colorMode;                   /* Color Data Size               */
static Uint16  *padData;                    /* AP Pad Data Area              */
static SclConfig      scfg;
static Uint32         swapFlag;
static Uint16         *charMap;
static Uint32         *charRam;
static Sint32         xPos;
static Sint32         yPos;
static Sint32         toumeiColor;
static Uint16         keyWinBuf[KEYWIN_Y][KEYWIN_X];
static WinDef         keyWin = {
                         KEYWIN_X,
                         KEYWIN_Y,
                         22,
                         0,
                         (Uint16*)keyWinBuf
                      };

/*----------------------------
SCROLL N0
Pattern Name Table location:A0
Character location: A0
Color Mode: C256
Zoom Mode: 1
----------------------------*/
static Uint16	N0_CYC[]={
	0x0f44,0xffff,
	0xffff,0xffff,
	0xffff,0xffff,
	0xffff,0xffff
};

static Rgb16 fontCol16[4][2]= {
        /* back color          char color          */
	0x00000000,           RGB16_COLOR(31,31,31),
        RGB16_COLOR( 0,10, 0),RGB16_COLOR(31,31,31),
        RGB16_COLOR(10, 0, 0),RGB16_COLOR(31,31,31),
        RGB16_COLOR( 0, 0,10),RGB16_COLOR(31,31,31),
};

static void winSwap(WinDef* win);
static void winPutChar(WinDef* win, int x, int y, int color, char c);
static void winChgCharColor(WinDef* win, int x, int y, int color);
static void winMove(WinDef* win, int dx, int dy);
static void scrollUp(void);
static void writeCtrlChar(char c);
static void writewc(int x, int y, int color, char c);
static void writec(int x, int y, int color, char c);
static void chgc(int x, int y, int color);
static void saveSclReg(void);
static void restoreSclReg(void);
static void swapChar(void);
static void swapMap(void);
static void swapColor(void);
static void delay(int count);


/*****************************************************************************
 *
 * NAME:  DBG_Initial()  - Initialize Debug Environment
 *
 * PARAMETERS :
 *
 *     (1) Uint16        *apPadData    - <i> ＡＰ側定義パッドデータエリア
 *     (2) Uint32        dispScrollNo  - <i> デバッグ表示スクロール面の番号
 *     (3) Rgb16         charColor     - <i> 文字の色
 *     (4) Rgb16         backColor     - <i> バックの色
 *
 * DESCRIPTION:
 *
 *     デバッグ環境の初期化
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
void DBG_Initial (Uint16 *apPadData, Rgb16 charColor, Rgb16 backColor)
{
    Uint32  *wFontArea, *wFontArea2, fontLine, col32;
    Uint8   *wFontData, fontMask, fontChar;
    Uint16  mapId, col16;
    Sint32  i, j, k;
    void    **vector;

    /** BEGIN ***************************************************************/
    padData = apPadData;
    swapFlag = 0;
    xPos = yPos = 0;
    fontCol16[0][0] = backColor;
    fontCol16[0][1] = charColor;
    if(backColor == 0)
        toumeiColor = 1;
    else
        toumeiColor = 0;

    /* Set Font Char Data */
    wFontArea = (Uint32*)fontArea;
    wFontData = fontData;
    for(i=0; i<FONT_COUNT; i++)
       for(j=0; j<8; j++) {
           fontLine = 0;
           fontMask = 0x80;
           fontChar = *wFontData++;
           for(k=0; k<7; k++) {
               if(fontChar & fontMask)   fontLine++;
               fontLine <<= 4;
               fontMask >>= 1;
           }
           if(fontChar & fontMask)   fontLine++;
           *wFontArea++ = fontLine;
       }
    wFontArea  = (Uint32*)fontArea;
    wFontArea2 = (Uint32*)(&fontArea[FONT_COUNT]);
    for(i=0; i<FONT_COUNT; i++)
       for(j=0; j<8; j++)
          *wFontArea2++ = (*wFontArea++) + 0x11111111;

    /* Set Map Data */
    DBG_ClearScreen();

    /* Set Color Data */
    colorMode = SCL_GetColRamMode();
    if(colorMode == SCL_CRM24_1024) {
      for(i=0; i<4; i++)
         for(j=0; j<2; j++) {
            col16 = fontCol16[i][j];
            col32 = ((Uint32)(col16 & 0x7c00) << 9) +
                    ((Uint32)(col16 & 0x03e0) << 6) +
                    ((Uint32)(col16 & 0x001f) << 3);
            colorArea32[i][j] = col32;
         }
    } else {
      for(i=0; i<4; i++)
         for(j=0; j<2; j++)
            colorArea16[i][j] = fontCol16[i][j];
    }
    charMap = (Uint16*)SCL_VDP2_VRAM_A1;
    charRam   = (Uint32*)SCL_VDP2_VRAM_A0;

    /* vector = get_vbr(); */

    vector = (void **)0x6000000;

    vector[4]  = &DBG_IntrGenOpErr;        /*  一般不当命令処理          */
    vector[6]  = &DBG_IntrSlotOpErr;       /*  スロット不当命令処理      */
    vector[9]  = &DBG_IntrCpuAddrErr;      /*  ＣＰＵアドレスエラー処理  */
    vector[10] = &DBG_IntrDmaAddrErr;      /*  ＤＭＡアドレスエラー処理  */
}


/*****************************************************************************
 *
 * NAME:  DBG_DisplayOn()  - Debug Display ON
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     デバッグ面の表示をＯＮにする
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
void DBG_DisplayOn(void)
{

    /** BEGIN ***************************************************************/
    if(swapFlag) return;
    swapFlag ^= 1;
    saveSclReg();
    swapChar();
    swapMap();
    swapColor();

    Scl_s_reg.ramcontrl  &= 0xf000;
    Scl_s_reg.dispenbl   &= 0xffcf;
    Scl_s_reg.dispenbl   |= 0x0001;

    /*******************************************
     *	Scroll Configure Set                   *
     *******************************************/
    SCL_InitConfigTb(&scfg);
    scfg.dispenbl = 0x01;
    scfg.charsize = 0x00;	/* 0:1X1cel,1:2X2cel */
    scfg.pnamesize = 0x01;	/* 0:2Word,1:1Word */
    scfg.platesize = 0x03;	/* 0:1X1, 1:2X1, 3:2X2 */
    scfg.bmpsize = 0x00;	/* 無効 */
    scfg.coltype = 0x00;	/* 0:16, 1:256, 2:2048, 3:32k, 4:1M */
    scfg.datatype = 0x00;	/* 0:Cell(1:BitMap)*/
    scfg.flip = SCL_PN_12BIT;
    scfg.plate_addr[0] = SCL_VDP2_VRAM_A1;
    scfg.plate_addr[1] = SCL_VDP2_VRAM_A1;
    scfg.plate_addr[2] = SCL_VDP2_VRAM_A1;
    scfg.plate_addr[3] = SCL_VDP2_VRAM_A1;
    SCL_SetConfig(SCL_NBG0, &scfg);

    /*******************************************
     *	VRAM Access Pattern Set                *
     *******************************************/
    SCL_SetCycleTable(N0_CYC);

    /*******************************************
     *	Set Priority                           *
     *******************************************/
    SCL_SetPriority(SCL_NBG0, 7);

    /*******************************************
     *	Set Normal Scroll Pos                  *
     *******************************************/
    SCL_Open(SCL_NBG0);
       SCL_MoveTo(MTH_FIXED(0), MTH_FIXED(0),0);/* Home Position */
       SCL_Scale(MTH_FIXED(1.0), MTH_FIXED(1.0));
    SCL_Close();
    SCL_DisplayFrame();

}


/*****************************************************************************
 *
 * NAME:  DBG_DisplayOff()  - Debug Display OFF
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     デバッグ面の表示をＯＦＦにする
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
void DBG_DisplayOff(void)
{

    /** BEGIN ***************************************************************/
    if(swapFlag == 0) return;
    swapFlag ^= 1;
    restoreSclReg();
    swapChar();
    swapMap();
    swapColor();
}


/*****************************************************************************
 *
 * NAME:  DBG_ClearScreen()  - Clear Debug Screen
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     デバッグ面全体をスペースクリアする
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
void DBG_ClearScreen(void)
{
    int    x, y;

    /** BEGIN ***************************************************************/
    for(y=0; y<SCR_Y; y++)
        for(x=0; x<SCR_X; x++)
            writec(x, y, 0, ' ');
}


/*****************************************************************************
 *
 * NAME:  DBG_SetCursol()  - Set Current Cursol Position
 *
 * PARAMETERS :
 *
 *     (1) Uint16        x    - <i> Ｘ座標  (0 <= X <= 39)
 *     (2) Uint16        y    - <i> Ｙ座標  (0 <= Y <= 27)
 *
 * DESCRIPTION:
 *
 *     現カーソル位置をセットする
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
void DBG_SetCursol(Uint16 x, Uint16 y)
{
    /** BEGIN ***************************************************************/
    xPos = x;
    yPos = y;
}


/*****************************************************************************
 *
 * NAME:  DBG_Printf()  - Display Char String (printf)
 *
 * PARAMETERS :
 *
 *     (1) const char *control  - <i> printf のフォーマット定義
 *     (2) ...                  - <i> 表示データ
 *
 * DESCRIPTION:
 *
 *     デバッグ画面に printf で文字列を表示する
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
void DBG_Printf(const char *control, ...)
{
    va_list args;
    char buf[256];
    char *wBuf;

    /** BEGIN ***************************************************************/
    va_start(args, control);
    vsprintf(buf, control, args);
    va_end(args);
    wBuf = buf;
    for(; *wBuf != 0; wBuf++)
        writeCtrlChar(*wBuf);
}


/*****************************************************************************
 *
 * NAME:  DBG_GetKeyStr()  - Get Key In String
 *
 * PARAMETERS :
 *
 *     (1) const char *control  - <i> scanf のフォーマット定義
 *     (2) ...                  - <i> 表示データ
 *
 * DESCRIPTION:
 *
 *     ソフトウェアキーボードによりキーを入力し、scanf でフォーマット変換する
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
char *DBG_GetKeyStr(char *keyStr)
{
    int x, y, i;
    char c;
    Uint16 padc, padb, padh;

    /** BEGIN ***************************************************************/
    DBG_DisplayOn();
    /* Key Window Init */
    for(y=0; y<KEYWIN_Y; y++)
        for(x=0; x<KEYWIN_X; x++)
           keyWinBuf[y][x] = 0x0010;
    winSwap(&keyWin);
    winPutChar(&keyWin, 0, 0, 1, 0x00);
    for(x=1; x<(KEYWIN_X - 1); x++)
        winPutChar(&keyWin, x, 0, 1, 0x05);
    winPutChar(&keyWin, KEYWIN_X - 1, 0, 1, 0x01);
    for(y=1; y<(KEYWIN_Y - 1); y++)
        winPutChar(&keyWin, KEYWIN_X - 1, y, 1, 0x04);
    winPutChar(&keyWin, KEYWIN_X - 1, KEYWIN_Y - 1, 1, 0x03);
    for(x=1; x<(KEYWIN_X - 1); x++)
        winPutChar(&keyWin, x, KEYWIN_Y - 1, 1, 0x05);
    winPutChar(&keyWin, 0, KEYWIN_Y - 1, 1, 0x02);
    for(y=1; y<(KEYWIN_Y - 1); y++)
        winPutChar(&keyWin, 0, y, 1, 0x04);
    c = 0x20;
    for(y=1; y<(KEYWIN_Y - 1); y++)
        for(x=1; x<(KEYWIN_X - 1); x++)
            winPutChar(&keyWin, x, y, 1, c++);
    winPutChar(&keyWin, KEYWIN_X - 3, KEYWIN_Y - 2, 1, 0x06);
    winPutChar(&keyWin, KEYWIN_X - 2, KEYWIN_Y - 2, 1, 0x07);

    x = befKeyX;
    y = befKeyY;
    winChgCharColor(&keyWin, x, y, 2);
    /* Key Check */
    padb = 0;
    i = 0;
    writeCtrlChar(0x08); /* Corsol Display */
    for(;;) {
       	padc = *padData;
       	padh = padc & (padb ^ padc);
        padb = padc;
        c = 0;
	if((padc & PER_DGT_B) && (padh & PER_DGT_U)) {
           do {
              winMove(&keyWin, 0, -1);
              delay(5);
              padc = *padData;
           } while(padc == (PER_DGT_B | PER_DGT_U));
	} else
	if((padc & PER_DGT_B) && (padh & PER_DGT_D)) {
           do {
              winMove(&keyWin, 0, 1);
              delay(5);
              padc = *padData;
           } while(padc == (PER_DGT_B | PER_DGT_D));
	} else
	if((padc & PER_DGT_B) && (padh & PER_DGT_L)) {
           do {
              winMove(&keyWin, -1, 0);
              delay(5);
              padc = *padData;
           } while(padc == (PER_DGT_B | PER_DGT_L));
	} else
	if((padc & PER_DGT_B) && (padh & PER_DGT_R)) {
           do {
              winMove(&keyWin, 1, 0);
              delay(5);
              padc = *padData;
           } while(padc == (PER_DGT_B | PER_DGT_R));
	} else
	if(padh & PER_DGT_R) {
           do {
              winChgCharColor(&keyWin, x, y, 1);
              if(x < (KEYWIN_X - 2))
                  x++;
              else {
                  x = 1;
                  if(y < (KEYWIN_Y - 2))
                      y++;
                  else
                      y = 1;
              }
              winChgCharColor(&keyWin, x, y, 2);
              delay(6);
              padc = *padData;
           } while(padc == PER_DGT_R);
	} else
	if(padh & PER_DGT_L) {
           do {
              winChgCharColor(&keyWin, x, y, 1);
              if(x > 1)
                  x--;
              else {
                  x = KEYWIN_X - 2;
                  if(y > 1)
                      y--;
                  else
                      y = KEYWIN_Y - 2;
              }
              winChgCharColor(&keyWin, x, y, 2);
              delay(6);
              padc = *padData;
           } while(padc == PER_DGT_L);
	} else
	if(padh & PER_DGT_D) {
           do {
              winChgCharColor(&keyWin, x, y, 1);
              if(y < (KEYWIN_Y - 2))
                  y++;
              else {
                  y = 1;
                  if(x < (KEYWIN_X - 2))
                      x++;
                  else
                      x = 1;
              }
              winChgCharColor(&keyWin, x, y, 2);
              delay(6);
              padc = *padData;
           } while(padc == PER_DGT_D);
	} else
	if(padh & PER_DGT_U) {
           do {
              winChgCharColor(&keyWin, x, y, 1);
              if(y > 1)
                  y--;
              else {
                  y = KEYWIN_Y - 2;
                  if(x > 1)
                      x--;
                  else
                      x = KEYWIN_X - 2;
              }
              winChgCharColor(&keyWin, x, y, 2);
              delay(6);
              padc = *padData;
           } while(padc == PER_DGT_U);
	} else
	if(padh & PER_DGT_A) {
           c = 16 * (y - 1) + (x - 1) + 0x20;
	} else
	if(padh & PER_DGT_C) {
           c = 0x7e;
	} else
	if(padh & PER_DGT_S) {
           c = 0x7f;
           delay(7);
	} else
	if(padh & PER_DGT_X) {
	   sprSW ^= 1;
           if(sprSW) {
              sprPrty = SCL_GetPriority(SCL_SPR);
              SCL_SetPriority(SCL_SPR, 0);
           } else {
              SCL_SetPriority(SCL_SPR, sprPrty);
           }
	} else
	if(padh & PER_DGT_Y) {
	   sclSW ^= 1;
           if(sclSW) {
              sclNbg1Prty = SCL_GetPriority(SCL_NBG1);
              sclNbg2Prty = SCL_GetPriority(SCL_NBG2);
              sclNbg3Prty = SCL_GetPriority(SCL_NBG3);
              sclRbg0Prty = SCL_GetPriority(SCL_RBG0);
              SCL_SetPriority(SCL_NBG1, 0);
              SCL_SetPriority(SCL_NBG2, 0);
              SCL_SetPriority(SCL_NBG3, 0);
              SCL_SetPriority(SCL_RBG0, 0);
           } else {
              SCL_SetPriority(SCL_NBG1, sclNbg1Prty);
              SCL_SetPriority(SCL_NBG2, sclNbg2Prty);
              SCL_SetPriority(SCL_NBG3, sclNbg3Prty);
              SCL_SetPriority(SCL_RBG0, sclRbg0Prty);
           }
	}
	if(c) {
	   if(c == 0x7f) c = '\n';
	   if(c == 0x7e) c = 0x01;
           winSwap(&keyWin);
           writeCtrlChar(0x01);
           writeCtrlChar(c);
           writeCtrlChar(0x08); /* Corsol Display */
           winSwap(&keyWin);
           if(c == '\n') {
              keyStr[i] = 0;
              break;
           } else
           if(c == 0x01) {
              if(i > 0) --i;
           } else {
              keyStr[i++] = c;
           }
        }
    }

    befKeyX = x;
    befKeyY = y;
    winSwap(&keyWin);
    writeCtrlChar(0x01); /* Corsol Delete */
    return keyStr;
}


/*****************************************************************************
 *
 * NAME:  winSwap()  - Swap Pop Up Window and VRAM Map Area
 *
 * PARAMETERS :
 *
 *     (1) WinDef  *win   - <i> ウィンドウ定義
 *
 * DESCRIPTION:
 *
 *     ポップアップウィンドウとＶＲＡＭマップエリアの内容を交換する
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
static void winSwap(WinDef* win)
{
    int    x, y, i;
    Uint16 w;
    Uint16 *wMapArea;

    /** BEGIN ***************************************************************/
    wMapArea = win->saveBuf;
    for(y=win->ltY; y<(win->ltY + win->height); y++)
        for(x=win->ltX; x<(win->ltX + win->width); x++) {
            i = y*64+x;
            w = charMap[i];
            charMap[i] = *wMapArea;
            *wMapArea++ = w;
        }
}


/*****************************************************************************
 *
 * NAME:  winPutChar()  - Put Char in the Pop Up Window 
 *
 * PARAMETERS :
 *
 *     (1) WinDef  *win       - <i> ウィンドウ定義
 *     (2) int     x, y       - <i> ウィンドウ内文字書き込み座標
 *     (3) int     color      - <i> ０から３のカラー番号
 *     (4) char    c          - <i> 文字データ
 *
 * DESCRIPTION:
 *
 *     ポップアップウィンドウへ文字を書き込む
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
static void winPutChar(WinDef* win, int x, int y, int color, char c)
{
    /** BEGIN ***************************************************************/
    x += win->ltX;
    y += win->ltY;
    writewc(x, y, color, c);
}


/*****************************************************************************
 *
 * NAME:  winChgCharColor()  - Change Char Color in the Pop Up Window 
 *
 * PARAMETERS :
 *
 *     (1) WinDef  *win       - <i> ウィンドウ定義
 *     (2) int     x, y       - <i> ウィンドウ内文字書き込み座標
 *     (3) int     color      - <i> ０から３のカラー番号
 *
 * DESCRIPTION:
 *
 *     ポップアップウィンドウ内の文字カラーを変更する
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
static void winChgCharColor(WinDef* win, int x, int y, int color)
{
    /** BEGIN ***************************************************************/
    x += win->ltX;
    y += win->ltY;
    chgc(x, y, color);
}


/*****************************************************************************
 *
 * NAME:  winMove()  - Move Pop Up Window 
 *
 * PARAMETERS :
 *
 *     (1) WinDef  *win       - <i> ウィンドウ定義
 *     (2) int     dx, dy     - <i> ウィンドウ移動量
 *
 * DESCRIPTION:
 *
 *     ポップアップウィンドウを左右上下に移動する
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
static void winMove(WinDef* win, int dx, int dy)
{
    int x, y;

    /** BEGIN ***************************************************************/
    winSwap(win);
    x = win->ltX + dx;
    y = win->ltY + dy;
    if(x < 0) x = 0;
    if(x > SCR_X - win->width) x = SCR_X - win->width;
    if(y < 0) y = 0;
    if(y > SCR_Y - win->height) y = SCR_Y - win->height;
    win->ltX = x;
    win->ltY = y;
    winSwap(win);
}


/*****************************************************************************
 *
 * NAME:  writeCtrlChar()  - Put Control Char 
 *
 * PARAMETERS :
 *
 *     (1) char  char       - <i> 文字データ
 *
 * DESCRIPTION:
 *
 *     現在のカーソル位置に文字のコントロールコードを意識してｐｕｔし、
 *     カーソル位置を更新する
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
static void writeCtrlChar(char c)
{
    /** BEGIN ***************************************************************/
    switch(c) {
        case '\n':
            xPos = 0;
            if(yPos >= (SCR_Y - 1))
                scrollUp();
            else
               yPos++;
            break;

        case '\r':
            xPos = 0;
            break;

        case 0x01: /* back space */
            if(xPos > 0) {
                xPos--;
            } else {
                if(yPos > 0) yPos--;
                xPos = SCR_X - 1;
            }
            writec(xPos, yPos, 0, ' ');
            break;

        default:
            writec(xPos, yPos, 0, c);
            if(xPos >= (SCR_X - 1)) {
               xPos = 0;
               if(yPos >= (SCR_Y - 1))
                   scrollUp();
               else
                  yPos++;
            } else
               xPos++;
            break;
    }
}


/*****************************************************************************
 *
 * NAME:  writewc()  - Put 1 Char to VRAM or Save Area for Pop Up Window
 *
 * PARAMETERS :
 *
 *     (1) int   x, y           - <i> 文字書き込み座標
 *     (2) int   color          - <i> ０から３のカラー番号
 *     (3) char  c              - <i> 文字データ
 *
 * DESCRIPTION:
 *
 *     指定文字のマップデータを表示ＯＮの場合はＶＲＡＭに書き込み、表示ＯＦＦ
 *     の場合はマップセーブエリアに書き込む
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
static void writewc(int x, int y, int color, char c)
{
    Uint16  mapId;
    int     pos;

    /** BEGIN ***************************************************************/
    if(c >= 0x20) c -= 0x10;
    mapId = (color << 12) + c + FONT_COUNT;
    if(swapFlag) {
        pos = y*64+x;
        charMap[pos] = mapId;
    } else
        mapArea[y][x] = mapId;
}


/*****************************************************************************
 *
 * NAME:  writec()  - Put 1 Char to VRAM or Save Area 
 *
 * PARAMETERS :
 *
 *     (1) int   x, y           - <i> 文字書き込み座標
 *     (2) int   color          - <i> ０から３のカラー番号
 *     (3) char  c              - <i> 文字データ
 *
 * DESCRIPTION:
 *
 *     指定文字のマップデータを表示ＯＮの場合はＶＲＡＭに書き込み、表示ＯＦＦ
 *     の場合はマップセーブエリアに書き込む
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
static void writec(int x, int y, int color, char c)
{
    Uint16  mapId;
    int     pos;

    /** BEGIN ***************************************************************/
    if(c >= 0x20) c -= 0x10;
    mapId = (color << 12) + c;
    if(swapFlag) {
        pos = y*64+x;
        charMap[pos] = mapId;
    } else
        mapArea[y][x] = mapId;
}


/*****************************************************************************
 *
 * NAME:  chgc()  - Change Char Color to VRAM or Save Area 
 *
 * PARAMETERS :
 *
 *     (1) int   x, y           - <i> 文字書き込み座標
 *     (2) int   color          - <i> ０から３のカラー番号
 *
 * DESCRIPTION:
 *
 *     指定文字のカラーを変更する
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
static void chgc(int x, int y, int color)
{
    Uint16  mapId;
    int     pos;

    /** BEGIN ***************************************************************/
    color <<= 12;
    if(swapFlag) {
        pos = y*64+x;
        mapId = charMap[pos] & 0x0fff;
        charMap[pos] = mapId | color;
    } else {
        mapId = mapArea[y][x] & 0x0fff;
        mapArea[y][x] = mapId | color;
    }
}


/*****************************************************************************
 *
 * NAME:  scrollUp()  - Scroll Up 1 Line  VRAM or Save Area 
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     表示ＯＮの場合はＶＲＡＭのマップエリアをスクロールし、表示ＯＦＦ
 *     の場合はマップセーブエリアをスクロールする
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
static void scrollUp(void)
{
    Uint16  mapId;
    int     x, y;

    /** BEGIN ***************************************************************/
    if(swapFlag) {
        for(y=1; y<SCR_Y; y++)
            for(x=0; x<SCR_X; x++)
                charMap[(y-1)*64+x] = charMap[y*64+x];
        for(x=0; x<SCR_X; x++)
            charMap[(SCR_Y-1)*64+x] = 0x0010;
    } else {
        for(y=1; y<SCR_Y; y++)
            for(x=0; x<SCR_X; x++)
                mapArea[y-1][x] = mapArea[y][x];
        for(x=0; x<SCR_X; x++)
            mapArea[SCR_Y-1][x] = 0x0010;
    }
}


/*****************************************************************************
 *
 * NAME:  saveSclReg()  - Save Current Scroll & Priority Register
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     現スクロール、プライオリティレジスタ値をセーブする
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
static void saveSclReg(void)
{

    /** BEGIN ***************************************************************/
    /* スクロールレジスタバッファセーブ */
    memcpy(&saveScl_s_reg, &Scl_s_reg, sizeof(SclSysreg)); 
    memcpy(&saveScl_d_reg, &Scl_d_reg, sizeof(SclDataset)); 
    memcpy(&saveScl_n_reg, &Scl_n_reg, sizeof(SclNorscl)); 
    memcpy(&saveScl_r_reg, &Scl_r_reg, sizeof(SclRotscl)); 
    memcpy(&saveScl_w_reg, &Scl_w_reg, sizeof(SclWinscl)); 

    /* プライオリティレジスタバッファセーブ */
    memcpy(&saveSclOtherPri,  &SclOtherPri, sizeof(SclOtherPriRegister));
    memcpy(&saveSclSpPriNum,  &SclSpPriNum, sizeof(SclSpPriNumRegister));
    memcpy(&saveSclBgPriNum,  &SclBgPriNum, sizeof(SclBgPriNumRegister));
    memcpy(&saveSclSpColMix,  &SclSpColMix, sizeof(SclSpColMixRegister));
    memcpy(&saveSclBgColMix,  &SclBgColMix, sizeof(SclBgColMixRegister));
    memcpy(&saveSclColOffset, &SclColOffset,sizeof(SclColOffsetRegister));
}


/*****************************************************************************
 *
 * NAME:  restoreSclReg()  - Restore Current Scroll & Priority Register
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     セーブした現スクロール、プライオリティレジスタ値を戻す
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
static void restoreSclReg(void)
{

    /** BEGIN ***************************************************************/
    /* スクロールレジスタバッファリストア */
    memcpy(&Scl_s_reg, &saveScl_s_reg, sizeof(SclSysreg)); 
    memcpy(&Scl_d_reg, &saveScl_d_reg, sizeof(SclDataset)); 
    memcpy(&Scl_n_reg, &saveScl_n_reg, sizeof(SclNorscl)); 
    memcpy(&Scl_r_reg, &saveScl_r_reg, sizeof(SclRotscl)); 
    memcpy(&Scl_w_reg, &saveScl_w_reg, sizeof(SclWinscl)); 

    /* プライオリティレジスタバッファリストア */
    memcpy(&SclOtherPri,  &saveSclOtherPri, sizeof(SclOtherPriRegister));
    memcpy(&SclSpPriNum,  &saveSclSpPriNum, sizeof(SclSpPriNumRegister));
    memcpy(&SclBgPriNum,  &saveSclBgPriNum, sizeof(SclBgPriNumRegister));
    memcpy(&SclSpColMix,  &saveSclSpColMix, sizeof(SclSpColMixRegister));
    memcpy(&SclBgColMix,  &saveSclBgColMix, sizeof(SclBgColMixRegister));
    memcpy(&SclColOffset, &saveSclColOffset,sizeof(SclColOffsetRegister));
    SclPriBuffDirty.SclOtherPri  = 1;
    SclPriBuffDirty.SclSpPriNum  = 1;
    SclPriBuffDirty.SclBgPriNum  = 1;
    SclPriBuffDirty.SclSpColMix  = 1;
    SclPriBuffDirty.SclBgColMix  = 1;
    SclPriBuffDirty.SclColOffset = 1;
}


/*****************************************************************************
 *
 * NAME:  swapChar()  - Swap Font Char Data
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     ワークエリアとＶＲＡＭの文字キャラクタデータを取り替える
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
static void swapChar(void)
{
    Uint32 *wFontArea, *wCharRam, w;
    int    i, j;
    /** BEGIN ***************************************************************/
    wFontArea = (Uint32*)fontArea;
    wCharRam  = charRam;
    for(i=0; i<FONT_COUNT*2; i++)
        for(j=0; j<8; j++) {
            w = *wCharRam;
            *wCharRam++ = *wFontArea;
            *wFontArea++ = w;
        }
}


/*****************************************************************************
 *
 * NAME:  swapMap()  - Swap Font Map Data
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     ワークエリアとＶＲＡＭの文字マップデータを取り替える
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
static void swapMap(void)
{
    Uint16 *wMapArea, w;
    int    i, j;
    /** BEGIN ***************************************************************/
    wMapArea = (Uint16*)mapArea;
    for(i=0;i<SCR_Y;i++)
        for(j=0; j<SCR_X; j++) {
            w = charMap[i*64+j];
            charMap[i*64+j] = *wMapArea;
            *wMapArea++ = w;
        }
}


/*****************************************************************************
 *
 * NAME:  swapColor()  - Swap Color Palette Data
 *
 * PARAMETERS :
 *
 *     No exist.
 *
 * DESCRIPTION:
 *
 *     ワークエリアとカラーＲＡＭのカラーデータを取り替える
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
static void swapColor(void)
{
    Uint32 *colRam32;
    Uint16 *colRam16;
    int    i, j;

    /** BEGIN ***************************************************************/
    if(swapFlag) {
        if(toumeiColor)
            colRam32 = (Uint32*)SCL_AllocColRam(SCL_NBG0,64,OFF);
        else
            colRam32 = (Uint32*)SCL_AllocColRam(SCL_NBG0,64,ON);
        if(colorMode == SCL_CRM24_1024) {
            colRam32[0] = colorArea32[0][0];
            colRam32[1] = colorArea32[0][1];
            for(i=1; i<4; i++)
               for(j=1; j<3; j++) 
                  colRam32[16*i + j] = colorArea32[i][j-1];
        } else {
            colRam16 = (Uint16*)colRam32;
            colRam16[0] = colorArea16[0][0];
            colRam16[1] = colorArea16[0][1];
            for(i=1; i<4; i++)
               for(j=1; j<3; j++) 
                  colRam16[16*i + j] = colorArea16[i][j-1];
        }
    } else   {
    }
}

static void delay(int count)
{
   int i, j, k;
   
   for(i=0; i<count; i++)
      for(j=0; j<1000; j++)
         for(k=0; k<100; k++)  DBG_DelayCount++;
}

/*  end of file */
