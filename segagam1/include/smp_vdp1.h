/*	Copyright(c) 1994 SEGA			*/
/*
 *
 * VDP1(SPRITE) CONTROL
 *
 */
 
#ifndef	__SMP_VDP1_H
#define	__SMP_VDP1_H

/*
 *    定数 
 */
#define	SMV1_COMMAND_MAX    0x400    /* 最大コマンド数                   */
#define SMV1_GROW_MAX        0x80    /* 最大グーローテーブル数           */
#define SMV1_LOOKUP_MAX      0x02    /* 最大ルックアップカラーテーブル数 */
#define SMV1_CHAR_MAX       0x200    /* 最大キャラクタ定義数             */
#define SMV1_PRIO_LEVEL     0x200    /* プライオリティレベル数           */


/*
 *    SPRITE 構造体用 PUTMODE NO
 */
enum {
    PMD_sprx_nrm,           /* NORMAL SPRITE		    */
    PMD_sprx_zoom,          /* ZOOM SPRITE		        */
    PMD_sprx_rotzoom,	    /* ROTATE & ZOOM SPRITE	    */
    PMD_sprx_line,          /* LINE                     */
    PMD_sprx_npolygon,      /* NORMAL POLYGON           */
    PMD_sprx_zpolygon,      /* ZOOM POLYGON             */
    PMD_sprx_rzpolygon,     /* ROTATE & ZOOM POLYGON    */
    PMD_sprx_rzpolyline,    /* ROTATE & ZOOM POLYLINE   */
    PMD_sprx_4pset          /* ROTATE & ZOOM POLYLINE   */
};


/*
 *    SPRITE 構造体　
 */
typedef struct SPRITE {
    Uint8 putmode;          /* SPRITE PUT TYPE         1 00 */
    Uint8 patno;            /* SPR pattern No          1 01 */
    Uint8 patchgcnt;        /* patchg counter          1 02 */
    Uint8 patchgtim;        /* patchg timer            1 03 */
    Uint8 sprhsize;         /* SPRITE h_SIZE           1 04 */
    Uint8 sprvsize;         /* SPRITE v_SIZE           1 05 */
    Sint16 pmode;           /* PMODE                   2 06 */
    Sint16 color;           /* COLOR DATA              2 08 */
    Sint16 sproffset;       /* 元絵vramオフセット      2 10 */
    Sint16 **patbase;       /* PATTERN TABLE ADDRESS   4 12 */
    Sint32 xposi;           /* XPOSI                   4 16 */
    Sint32 yposi;           /* YPOSI                   4 20 */
    Sint32 zposi;           /* ZPOSI                   4 24 */
    Sint16 posi[4][3];      /* SPRITE XYZ OFFSET      24 28 */
    Sint16 xspd;            /* xスピード               2 52 */
    Sint16 yspd;            /* yスピード               2 54 */
    Sint16 zspd;            /* zスピード               2 56 */
    Sint16 xacc;            /* x加速度                 2 58 */
    Sint16 yacc;            /* y加速度                 2 60 */    
    Sint16 zacc;            /* z加速度                 2 62 */
    Sint16 xdirec;          /* x角度                   2 64 */
    Sint16 ydirec;          /* y角度                   2 66 */
    Sint16 zdirec;          /* z角度                   2 68 */
    Sint16 vzoom;           /* 縦拡大係数  $00_00      2 70 */
    Sint16 hzoom;           /* 横拡大係数  $00_00      2 72 */
    Sint16 sprprio;         /* プライオリティ低0..7高  2 74 */
    Uint8 *linktbl;        /* priority linkptr        4 76 */
                            /* TOTAL         80(0x50) bytes */            
} SPRITE;



/*
 *    スプライト表示リクエストマクロ 
 */
#define SMV1_PutSprite(_sprite_) \
    (*SMV1_SpritePutModeTbl[(_sprite_)->putmode])(_sprite_)
 


/*
 *    関数プロトタイプ 
 */
extern void SMV1_IniSprite(void);
extern void SMV1_SprCmdStart(void);
extern void SMV1_SprCmdEnd(void);
extern void (* const SMV1_SpritePutModeTbl[])(SPRITE *);

extern void SMV1_SetFont4sp(Uint8 topcharno, Uint32 fontcolor,
                                                   Uint32 backcolor);



#endif  /* endif __SMP_VDP1_H */

 
