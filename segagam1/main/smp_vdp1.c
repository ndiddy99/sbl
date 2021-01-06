/*	Copyright(c) 1994 SEGA			*/
/*
 *
 * VDP1(SPRITE) CONTROL
 *
 */


#include "smp_bios.h"


/*
 *    SPR2 DEFINE WORK 
 */
SPR_2DefineWork(SMV1_sprwk,SMV1_COMMAND_MAX,SMV1_GROW_MAX,SMV1_LOOKUP_MAX,SMV1_CHAR_MAX, SMV1_PRIO_LEVEL)



/*
 *    関数プロトタイプ
 */
static void put_nsprite_sx(SPRITE *spr);
static void put_zsprite_sx(SPRITE *spr);
static void put_rzsprite_sx(SPRITE *spr);
static void put_line_sx(SPRITE *spr);
static void put_rzpolygon_sx(SPRITE *spr);
static void put_polygon_sx(SPRITE *spr);
static void put_zpolygon_sx(SPRITE *spr);
static void put_rzpolygon_sx(SPRITE *spr);
static void put_rzpolyline_sx(SPRITE *spr);
static void put_4psprite_sx(SPRITE *spr);

/* 
 *    スプライトプットルーチンアドレステーブル
 */ 
void (* const SMV1_SpritePutModeTbl[])(SPRITE *) = {
    put_nsprite_sx,
    put_zsprite_sx,
    put_rzsprite_sx,
    put_line_sx,
    put_polygon_sx,
    put_zpolygon_sx,
    put_rzpolygon_sx,
/*    put_przsprite_sx, */
    put_rzpolyline_sx,
    put_4psprite_sx,
};


/*----------------------------------------------
 *     8*8 Ascii Font Set for 16color SPRITE
 *     topcharno  先頭キャラクタNo
 *     fontcolor  文字カラー
 *     backcolor  背景カラー
 *---------------------------------------------*/
void SMV1_SetFont4sp(Uint8 topcharno, Uint32 fontcolor, Uint32 backcolor)
{
    Uint8 *src, *dst;
    Uint8 rdata,tmp;
    Uint16 i, j, k;
    Uint8 celbuf[0x20];
    Uint32 fontdata[4];


    fontcolor &= 0xf;
    backcolor &= 0xf;
    fontdata[0] = (backcolor << 4) + backcolor;   /* 00 */
    fontdata[1] = (backcolor << 4) + fontcolor;   /* 01 */
    fontdata[2] = (fontcolor << 4) + backcolor;   /* 10 */
    fontdata[3] = (fontcolor << 4) + fontcolor;   /* 11 */


    src = &font8cg[0];

    for (i = 0; i < 0x40; i++) {
        dst = &celbuf[0];
        for (j = 0; j < 8; j++) {
            rdata = *src;
            src++;

            *dst = fontdata[(rdata >> 6) & 3];         
            dst++;

            *dst = fontdata[(rdata >> 4) & 3];         
            dst++;

            *dst = fontdata[(rdata >> 2) & 3];         
            dst++;

            *dst = fontdata[(rdata >> 0) & 3];         
            dst++;
        }
        k = i + topcharno;
        SPR_2SetChar(k, 0, 0, 8, 8, &celbuf[0]);
    }


}


/*----------------------------------
 *    NORMAL SPRITE
 *---------------------------------*/
static void put_nsprite_sx(SPRITE *spr)
{
    Uint16 hvrev, pmode, color, cgadr, cgsize, growno;
    XyInt posi[4];
    Sint16 *pattbl;
    Sint16 i, xposi, yposi, hsize, vsize, hofst, vofst;

    pattbl = (Sint16 *)spr->patbase[spr->patno]; /*パターンテーブルアドレス */
    xposi = *(Sint16 *)(&spr->xposi);
    yposi = *(Sint16 *)(&spr->yposi);

    pmode = spr->pmode;                      /* 表示モード     */
    color = spr->color;

    for (i = *pattbl++; i > 0 ; i--) {           /* 枚数リード     */
        cgadr = *pattbl, pattbl++;               /* CHAR NO        */
        hvrev = *pattbl, pattbl++;               /* HV reverse     */
        hsize = *pattbl, pattbl++;               /* 元絵hsize      */
        vsize = *pattbl, pattbl++;               /* 元絵vsize      */
        hofst = *pattbl, pattbl++;               /* 表示hoffset    */
        vofst = *pattbl, pattbl++;               /* 表示voffset    */
        cgsize = (hsize << 5) + vsize;
        posi[0].x = xposi + hofst;
        posi[0].y = yposi + vofst;
        SPR_2NormSpr(0, hvrev, pmode, color, cgadr, &posi[0], 0xffff);
    }
      
}



/*----------------------------------
 *    ZOOM
 *---------------------------------*/
static void put_zsprite_sx(SPRITE *spr)
{
    Uint16 hvrev, pmode, color, cgadr, cgsize, growno;
    XyInt posi[4];
    Sint16 *pattbl;
    Sint16 i, xposi, yposi, hsize, vsize, hofst, vofst;
    Sint16 hofst0, hofst1, vofst0, vofst1;

    pattbl = (Sint16 *)spr->patbase[spr->patno]; /*パターンテーブルアドレス */
    xposi = *(Sint16 *)(&spr->xposi);
    yposi = *(Sint16 *)(&spr->yposi);


    pmode = spr->pmode;                      /* 表示モード     */
    color = spr->color;
    
    for (i = *pattbl++; i; i--) {                /* 枚数リード     */
        cgadr = *pattbl, pattbl++;               /* Char No        */
        hvrev = *pattbl, pattbl++;               /* HV reverse     */
        hsize = *pattbl, pattbl++;               /* 元絵hsize      */
        vsize = *pattbl, pattbl++;               /* 元絵vsize      */
        hofst = *pattbl, pattbl++;               /* 表示hoffset    */
        vofst = *pattbl, pattbl++;               /* 表示voffset    */
        cgsize = (hsize << 5) + vsize;

        hofst0 = hofst * spr->hzoom / 0x100;
        hofst1 = (hofst + hsize) * spr->hzoom / 0x100;
        vofst0 = vofst * spr->vzoom / 0x100;
        vofst1 = (vofst + vsize) * spr->vzoom / 0x100;

        posi[0].x = xposi + hofst0; 
        posi[0].y = yposi + vofst0;
        posi[1].x = xposi + hofst1;
        posi[1].y = yposi + vofst0;
        posi[2].x = xposi + hofst1;
        posi[2].y = yposi + vofst1;
        posi[3].x = xposi + hofst0;
        posi[3].y = yposi + vofst1;
        SPR_2DistSpr(0, hvrev, pmode, color, cgadr, &posi[0], 0xffff);
    }
      
}

/*-----------------------------------------------
 *    ROTATE & ZOOM
 *-----------------------------------------------*/
static void put_rzsprite_sx(SPRITE *spr)
{
    Uint16 hvrev, pmode, color, cgadr, cgsize, growno;
    Sint16 *pattbl;
    Sint16 i, xposi, yposi, hsize, vsize, hofst, vofst;
    Sint32 rx0c, rx0s, ry0c, ry0s;
    Sint32 rx1c, rx1s, ry1c, ry1s;
    Sint32 hcosz, vcosz, hsinz, vsinz;
	Sint32 sinz,cosz;
	XyInt posi[4];
    pattbl = (Sint16 *)spr->patbase[spr->patno]; /*パターンテーブルアドレス */
    xposi = *(Sint16 *)(&spr->xposi);
    yposi = *(Sint16 *)(&spr->yposi);
	sinz = SMMI_Sinset8(spr->zdirec);
	cosz = SMMI_Cosset8(spr->zdirec);
	hsinz = sinz * spr->hzoom;
	hcosz = cosz * spr->hzoom;
	vsinz = sinz * spr->vzoom;
	vcosz = cosz * spr->vzoom;

    pmode = spr->pmode;                      /* 表示モード     */
    color = spr->color;

    for (i = *pattbl++; i; i--) {                /* 枚数リード     */
        cgadr = *pattbl,pattbl++;                /* CharNo         */
        hvrev = *pattbl,pattbl++;                /* HVreverse      */

        hsize = *pattbl,pattbl++;                /* 元絵hsize      */
        vsize = *pattbl,pattbl++;                /* 元絵vsize      */

        hofst = *pattbl,pattbl++;                /* 表示hoffset    */
        vofst = *pattbl,pattbl++;                /* 表示voffset    */


        cgsize = (hsize << 5) + vsize;

        rx0c = hofst * hcosz;
        ry0c = vofst * vcosz;
        rx0s = hofst * hsinz;
        ry0s = vofst * vsinz;
        rx1c = (hofst + hsize) * hcosz;
        ry1c = (vofst + vsize) * vcosz;
        rx1s = (hofst + hsize) * hsinz;
        ry1s = (vofst + vsize) * vsinz;


        posi[0].x = xposi + (rx0c - ry0s) / 0x10000;
        posi[0].y = yposi + (rx0s + ry0c) / 0x10000;
		
        posi[1].x = xposi + (rx1c - ry0s) / 0x10000;
        posi[1].y = yposi + (rx1s + ry0c) / 0x10000;
		
        posi[2].x = xposi + (rx1c - ry1s) / 0x10000;
        posi[2].y = yposi + (rx1s + ry1c) / 0x10000;
		                                            
        posi[3].x = xposi + (rx0c - ry1s) / 0x10000;
        posi[3].y = yposi + (rx0s + ry1c) / 0x10000;

        SPR_2DistSpr(0, hvrev, pmode, color, cgadr, posi, 0xffff);
    }
      
}





/*----------------------------------
 *    LINE
 *---------------------------------*/
static void put_line_sx(SPRITE *spr)
{
    Uint16 hvrev, pmode, color, cgadr, cgsize, growno;
    XyInt posi[2];
    Sint16 *lineposi;


    lineposi = (Sint16 *)&spr->posi;
    pmode = spr->pmode;            /* 表示モード     */
    color = spr->color;
    posi[0].x = *lineposi, lineposi++;
    posi[0].y = *lineposi, lineposi++;
    lineposi++;
    posi[1].x = *lineposi, lineposi++;
    posi[1].y = *lineposi, lineposi++;

    SPR_2Line(0, pmode, color, &posi[0], 0xffff);
     
}



/*-----------------------------------------------
 *    Point table polygon set 
 *-----------------------------------------------*/
static void put_polygon_sx(SPRITE *spr)
{
    Uint16 hvrev, pmode, color, cgadr, cgsize, growno;
    XyInt posi[4];
    Sint16 *pattbl;
    Sint16 sinz, cosz, rx, ry;
    Sint16 i, xposi, yposi, hsize, vsize, hofst, vofst;


    pattbl = (Sint16 *)spr->patbase[spr->patno]; /*パターンテーブルアドレス */
    xposi = *(Sint16 *)(&spr->xposi);
    yposi = *(Sint16 *)(&spr->yposi);

    pmode = spr->pmode;                        /* 表示モード     */
	
    for (i = *pattbl++; i; i--) {                /* 枚数リード     */
        color = *pattbl, pattbl++;               /* color data     */

        posi[0].x = xposi + *pattbl;             /* ax offset      */
        pattbl++;
        posi[0].y = yposi + *pattbl;             /* ay offset      */
        pattbl++;

        posi[1].x = xposi + *pattbl;             /* bx offset      */
        pattbl++;
        posi[1].y = yposi + *pattbl;             /* by offset      */
        pattbl++;

        posi[2].x = xposi + *pattbl;             /* cx offset      */
        pattbl++;
        posi[2].y = yposi + *pattbl;             /* cy offset      */
        pattbl++;

        posi[3].x = xposi + *pattbl;             /* dx offset      */
        pattbl++;
        posi[3].y = yposi + *pattbl;             /* dy offset      */
        pattbl++;

        SPR_2Polygon(0, pmode, color, &posi[0], 0xffff);
    }
      
}



/*-----------------------------------------------
 *    Point table polygon set (ZOOM)
 *-----------------------------------------------*/
static void put_zpolygon_sx(SPRITE *spr)
{
    Uint16 hvrev, pmode, color, cgadr, cgsize, growno;
    XyInt posi[4];
    Sint16 *pattbl;
    Sint16 sinz, cosz, rx, ry;
    Sint16 i, xposi, yposi, hsize, vsize, hofst, vofst;


    pattbl = (Sint16 *)spr->patbase[spr->patno]; /*パターンテーブルアドレス */
    xposi = *(Sint16 *)(&spr->xposi);
    yposi = *(Sint16 *)(&spr->yposi);
	
    pmode = spr->pmode;                        /* 表示モード     */
	
    for (i = *pattbl++; i; i--) {                  /* 枚数リード     */
        color = *pattbl++;                         /* color data     */

        rx = *pattbl++;
        ry = *pattbl++;
        rx = (rx * spr->hzoom) / 0x100;
        ry = (ry * spr->vzoom) / 0x100;
        posi[0].x = xposi + rx;                    /* ax offset      */
        posi[0].y = yposi + ry;                    /* ay offset      */

        rx = *pattbl++;
        ry = *pattbl++;
        rx = (rx * spr->hzoom) / 0x100;
        ry = (ry * spr->vzoom) / 0x100;
        posi[1].x = xposi + rx;                    /* bx offset      */
        posi[1].y = yposi + ry;                    /* by offset      */

        rx = *pattbl++;
        ry = *pattbl++;
        rx = (rx * spr->hzoom) / 0x100;
        ry = (ry * spr->vzoom) / 0x100;
        posi[2].x = xposi + rx;                    /* cx offset      */
        posi[2].y = yposi + ry;                    /* cy offset      */

        rx = *pattbl++;
        ry = *pattbl++;
        rx = (rx * spr->hzoom) / 0x100;
        ry = (ry * spr->vzoom) / 0x100;
        posi[3].x = xposi + rx;                    /* dx offset      */
        posi[3].y = yposi + ry;                    /* dy offset      */

        SPR_2Polygon(0, pmode, color, &posi[0], 0xffff);
    }
      
}



/*-----------------------------------------------
 *    Point table polygon set (ROTATE & ZOOM)
 *-----------------------------------------------*/
static void put_rzpolygon_sx(SPRITE *spr)
{
    Uint16 hvrev, pmode, color, cgadr, cgsize, growno;
    XyInt posi[4];
    Sint16 *pattbl;
    Sint16 sinz, cosz, rx, ry;
    Sint16 i, xposi, yposi, hsize, vsize, hofst, vofst;


    pattbl = (Sint16 *)spr->patbase[spr->patno]; /*パターンテーブルアドレス */
    xposi = *(Sint16 *)(&spr->xposi);
    yposi = *(Sint16 *)(&spr->yposi);
	
    sinz = SMMI_Sinset8(spr->zdirec);
    cosz = SMMI_Cosset8(spr->zdirec);

    pmode = spr->pmode;                        /* 表示モード     */

    for (i = *pattbl++; i; i--) {                  /* 枚数リード     */
        color = *pattbl++;                         /* color data     */

        rx = *pattbl++;
        ry = *pattbl++;
        rx = (rx * spr->hzoom) / 0x100;
        ry = (ry * spr->vzoom) / 0x100;
        posi[0].x = xposi + (rx * cosz - ry * sinz) / 0x100; /* ax offset */
        posi[0].y = yposi + (rx * sinz + ry * cosz) / 0x100; /* ay offset */

        rx = *pattbl++;
        ry = *pattbl++;
        rx = (rx * spr->hzoom) / 0x100;
        ry = (ry * spr->vzoom) / 0x100;
        posi[1].x = xposi + (rx * cosz - ry * sinz) / 0x100; /* bx offset */
        posi[1].y = yposi + (rx * sinz + ry * cosz) / 0x100; /* by offset */

        rx = *pattbl++;
        ry = *pattbl++;
        rx = (rx * spr->hzoom) / 0x100;
        ry = (ry * spr->vzoom) / 0x100;
        posi[2].x = xposi + (rx * cosz - ry * sinz) / 0x100; /* cx offset */
        posi[2].y = yposi + (rx * sinz + ry * cosz) / 0x100; /* cy offset */

        rx = *pattbl++;
        ry = *pattbl++;
        rx = (rx * spr->hzoom) / 0x100;
        ry = (ry * spr->vzoom) / 0x100;
        posi[3].x = xposi + (rx * cosz - ry * sinz) / 0x100; /* dx offset */
        posi[3].y = yposi + (rx * sinz + ry * cosz) / 0x100; /* dy offset */

        SPR_2Polygon(0, pmode, color, &posi[0], 0xffff);
    }
      
}


/*-----------------------------------------------
 *    Point table polyline set (ROTATE & ZOOM)
 *-----------------------------------------------*/
static void put_rzpolyline_sx(SPRITE *spr)
{
    Uint16 hvrev, pmode, color, cgadr, cgsize, growno;
    XyInt posi[4];
    Sint16 *pattbl;
    Sint16 sinz, cosz, rx, ry;
    Sint16 i, xposi, yposi, hsize, vsize, hofst, vofst;


    pattbl = (Sint16 *)spr->patbase[spr->patno]; /*パターンテーブルアドレス */
    xposi = *(Sint16 *)(&spr->xposi);
    yposi = *(Sint16 *)(&spr->yposi);
	
    sinz = SMMI_Sinset8(spr->zdirec);
    cosz = SMMI_Cosset8(spr->zdirec);


    pmode = spr->pmode;                        /* 表示モード     */

    for (i = *pattbl++; i; i--) {                  /* 枚数リード     */
        color = *pattbl++;                         /* color data     */

        rx = *pattbl++;
        ry = *pattbl++;
        rx = (rx * spr->hzoom) / 0x100;
        ry = (ry * spr->vzoom) / 0x100;
        posi[0].x = xposi + (rx * cosz - ry * sinz) / 0x100; /* ax offset */
        posi[0].y = yposi + (rx * sinz + ry * cosz) / 0x100; /* ay offset */

        rx = *pattbl++;
        ry = *pattbl++;
        rx = (rx * spr->hzoom) / 0x100;
        ry = (ry * spr->vzoom) / 0x100;
        posi[1].x = xposi + (rx * cosz - ry * sinz) / 0x100; /* bx offset */
        posi[1].y = yposi + (rx * sinz + ry * cosz) / 0x100; /* by offset */

        rx = *pattbl++;
        ry = *pattbl++;
        rx = (rx * spr->hzoom) / 0x100;
        ry = (ry * spr->vzoom) / 0x100;
        posi[2].x = xposi + (rx * cosz - ry * sinz) / 0x100; /* cx offset */
        posi[2].y = yposi + (rx * sinz + ry * cosz) / 0x100; /* cy offset */

        rx = *pattbl++;
        ry = *pattbl++;
        rx = (rx * spr->hzoom) / 0x100;
        ry = (ry * spr->vzoom) / 0x100;
        posi[3].x = xposi + (rx * cosz - ry * sinz) / 0x100; /* dx offset */
        posi[3].y = yposi + (rx * sinz + ry * cosz) / 0x100; /* dy offset */

        SPR_2PolyLine(0, pmode, color, &posi[0], 0xffff);
    }
      
}


/*-----------------------------------------------
 *    ROTATE & ZOOM
 *-----------------------------------------------*/
static void put_4psprite_sx(SPRITE *spr)
{
    Uint16 hvrev, pmode, color, cgadr, cgsize, growno;
    XyInt posi[4];
    Sint16 *pattbl;
    Sint16 sinz, cosz, rx, ry;
    Sint16 i, xposi, yposi, hsize, vsize, hofst, vofst;

    pattbl = (Sint16 *)spr->patbase[spr->patno]; /*パターンテーブルアドレス */
    xposi = *(Sint16 *)(&spr->xposi);
    yposi = *(Sint16 *)(&spr->yposi);
    sinz = SMMI_Sinset8(spr->zdirec);
    cosz = SMMI_Cosset8(spr->zdirec);

    pmode = spr->pmode;                      /* 表示モード     */
    color = spr->color;

    for (i = *pattbl++; i; i--) {                /* 枚数リード     */
        cgadr = *pattbl++;                       /* CharNo         */
        hvrev = *pattbl++;                       /* HVreverse      */
        hsize = *pattbl++;                       /* 元絵hsize      */
        vsize = *pattbl++;                       /* 元絵vsize      */
        hofst = *pattbl++;                       /* 表示hoffset    */
        vofst = *pattbl++;                       /* 表示voffset    */
        cgsize = (hsize << 5) + vsize;
        posi[0].x = spr->posi[0][X];
        posi[0].y = spr->posi[0][Y];

        posi[1].x = spr->posi[1][X];
        posi[1].y = spr->posi[1][Y];

        posi[2].x = spr->posi[2][X];
        posi[2].y = spr->posi[2][Y];

        posi[3].x = spr->posi[3][X];
	    posi[3].y = spr->posi[3][Y];

        SPR_2DistSpr(0, hvrev, pmode, color, cgadr, &posi[0], 0xffff);

    }
      
}



/*==========================================================================*/
/*    スプライトコマンド転送                                                */
/*==========================================================================*/

/*-------------------------------
 *    SPRITE COMMAND START SET
 *-------------------------------*/
void SMV1_SprCmdStart(void)
{
     XyInt reg_xy;
     XyInt pos_xy[4];
     Uint16 pmode, color, growno;

     SPR_2OpenCommand(SPR_2DRAW_PRTY_ON);    /* SPRITE COMMAND WRITE SETUP */
       
     reg_xy.x = WIDTH_H -1;
     reg_xy.y = WIDTH_V -1;
     SPR_2SysClip(SPR_2MOST_FAR, &reg_xy); /* SYSTEM CLIPPING REG SET    */
     

     reg_xy.x = 0;
     reg_xy.y = 0;
     SPR_2LocalCoord(SPR_2MOST_FAR, &reg_xy); /* SPRITE LOCAL POINT SET */

#if 1 /* <<<<<<<<<<<<<< */
     pmode = SPM_ecd + SPM_sdd;              /* FB ERASE                   */
     color = 0x0000;
     growno = 0xffff;                        
     pos_xy[0].x = 0;
     pos_xy[0].y = 0;
     pos_xy[1].x = WIDTH_H - 1;
     pos_xy[1].y = 0;
     pos_xy[2].x = WIDTH_H - 1;
     pos_xy[2].y = WIDTH_V - 1;
     pos_xy[3].x = 0;
     pos_xy[3].y = WIDTH_V - 1;
     SPR_2Polygon(SPR_2MOST_FAR, pmode, color, &pos_xy[0], growno);
#endif



}


/*-----------------------------------
 *    SPRITE END COMMAND TRANSFER
 *-----------------------------------*/
void SMV1_SprCmdEnd(void)
{
    SPR_2CloseCommand();
}


/*-----------------------
 *    Init SPRITE I/O
 *-----------------------*/
void SMV1_IniSprite(void)
{
     XyInt reg_xy;
     XyInt pos_xy[4];
     Uint16 pmode, color, growno;

     SPR_2Initial(&SMV1_sprwk);
     SPR_2SetTvMode(SPR_TV_NORMAL, SPR_TV_320X224, 0);
     SPR_2FrameEraseData(0x0000);
     SPR_2OpenCommand(SPR_2DRAW_PRTY_ON);   /* SPRITE COMMAND WRITE SETUP */
       
     reg_xy.x = WIDTH_H -1;
     reg_xy.y = WIDTH_V -1;
     SPR_2SysClip(SPR_2MOST_FAR, &reg_xy);  /* SYSTEM CLIPPING REG SET    */
     
     reg_xy.x = 0;
     reg_xy.y = 0;
     SPR_2LocalCoord(SPR_2MOST_FAR, &reg_xy);  /* SPRITE LOCAL POINT SET */


#if 1 /* <<<<<<<<<<<<<< */
     pmode = SPM_ecd + SPM_sdd;             /* FB ERASE                   */
     color = 0x0000;
     growno = 0xffff;                       /* not use grow */
     pos_xy[0].x = 0;
     pos_xy[0].y = 0;
     pos_xy[1].x = WIDTH_H - 1;
     pos_xy[1].y = 0;
     pos_xy[2].x = WIDTH_H - 1;
     pos_xy[2].y = WIDTH_V - 1;
     pos_xy[3].x = 0;
     pos_xy[3].y = WIDTH_V - 1;
     SPR_2Polygon(SPR_2MOST_FAR, pmode, color, &pos_xy[0], growno);
#endif


}









