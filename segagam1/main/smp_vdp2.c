/*	Copyright(c) 1994 SEGA			*/
/*
 *
 * VDP2(SCROLL) CONTROL
 *
 */

#include "smp_bios.h"


/*
 *     外部参照ワーク
 */
SclRotscl Scl_r_reg;         
SclSysreg Scl_s_reg;
SclDataset Scl_d_reg;
SclNorscl Scl_n_reg;
SclWinscl Scl_w_reg;


/* スクロールVRAMアクセスサイクルパターンテーブル */
const Uint16 SMV2_DEFAULT_CYC[] = { 0x0145, 0x45ff, 
                                    0xffff, 0xffff, 
                                    0xffff, 0xffff, 
                                    0xffff, 0xffff };


SclVramConfig SMV2_sclvram;   /* スクロールVRAMの使用目的アサイン    */
SclConfig SMV2_nscrlreg0;     /* NBG0 scroll initial                 */
SclConfig SMV2_nscrlreg1;     /* NBG1 scroll initial                 */
SclConfig SMV2_rscrlreg;      /* RBG0 scroll initial                 */
SclLineparam SMV2_lparam;     /* ラインスクロールテーブルバッファ    */
ROTPARAM SMV2_rotparbuf;      /* 回転パラーメータバッファ            */
Sint32 SMV2_KeisuBuf[0x200];  /* 回転係数バッファ                    */
Sint16 SMV2_LWnd0Buf[0x200];  /* ラインウィンドウ0テーブルバッファ   */
Sint16 SMV2_LWnd1Buf[0x200];  /* ラインウィンドウ1テーブルバッファ   */
Uint32 SMV2_fontoffset;


/*
 *  SCROLL REGISTER INIT
 */
void SMV2_IniScroll(void)
{
    Uint16 d0;
    SclRotscl *rregptr;
    SclSysreg *sregptr;
    SclNorscl *nregptr;
    SclDataset *dregptr;


    SCL_Vdp2Init();

    SCL_SetDisplayMode(SCL_NON_INTER, SCL_224LINE, SCL_NORMAL_A);

    SCL_SetColRamMode(SCL_CRM15_2048);           /* 15bit color 2048        */


    SCL_InitRotateTable(RBG0_RPAR, 1, SCL_RBG0, SCL_NON);


    SCL_InitVramConfigTb(&SMV2_sclvram); /* VDP2用VRAMの使用目的を設定する */
    SMV2_sclvram.vramModeA = OFF;        /* VRAM-Aを分割モードにしない     */
    SMV2_sclvram.vramModeB = OFF;        /* VRAM-Bを分割モードにしない     */
    SMV2_sclvram.vramA0 = SCL_NON;       /* VRAM-A0に回転面データはおかない*/
    SMV2_sclvram.vramA1 = SCL_NON;       /* VRAM-A1に回転面データはおかない*/
    SMV2_sclvram.vramB0 = SCL_NON;       /* VRAM-B0に回転面データはおかない */
    SMV2_sclvram.vramB1 = SCL_NON;       /* VRAM-B1に回転面データはおかない*/
    SCL_SetVramConfig(&SMV2_sclvram);


    SCL_Open(SCL_NBG0);
    SCL_InitConfigTb(&SMV2_nscrlreg0);           /* NBG0 Scroll Config Set  */
    SMV2_nscrlreg0.dispenbl = ON;                /* 表示 ON 1/OFF 0         */
    SMV2_nscrlreg0.charsize = SCL_CHAR_SIZE_1X1; /*charsiz 0:1X1cel 1:2X2cel*/
    SMV2_nscrlreg0.pnamesize = SCL_PN1WORD;      /*patname 0:2word 1:1word  */
    SMV2_nscrlreg0.platesize = SCL_PL_SIZE_1X1;  /*plane size 0:h1Xv1 page  */
                                                 /*1:h2Xv1page 3:h2Xv2page  */
    SMV2_nscrlreg0.bmpsize = SCL_BMP_SIZE_512X256; /* BIT MAP SIZ           */
    SMV2_nscrlreg0.coltype = SCL_COL_TYPE_256;      /* color mode 0:16 1:256 */
                                                   /* 2:2048 3:32767 4:1677 */
    SMV2_nscrlreg0.datatype = SCL_CELL;            /*scroll 0:CELL 1:BitMap */
    SMV2_nscrlreg0.flip = SCL_PN_12BIT;            /*補助モード10bit 12bit  */
    SMV2_nscrlreg0.mapover = SCL_OVER_0;           /*map over mode 繰り返し */
    SMV2_nscrlreg0.plate_addr[0] = NBG0_PTNAMA;    /*MAPA patnam add        */
    SMV2_nscrlreg0.plate_addr[1] = NBG0_PTNAMA;    /*MAPB patnam add        */
    SMV2_nscrlreg0.plate_addr[2] = NBG0_PTNAMA;    /*MAPC patnam add        */
    SMV2_nscrlreg0.plate_addr[3] = NBG0_PTNAMA;    /*MAPD patnam add        */
    SCL_SetConfig(SCL_NBG0, &SMV2_nscrlreg0);
    SCL_SetCycleTable(SMV2_DEFAULT_CYC);           /* CYCLE PAT SET         */
    SCL_Close();


    SCL_Open(SCL_NBG1);
    SCL_InitConfigTb(&SMV2_nscrlreg1);             /*NBG1 Scroll Config Set */
    SMV2_nscrlreg1.dispenbl = ON;                  
    SMV2_nscrlreg1.charsize = SCL_CHAR_SIZE_1X1;   
    SMV2_nscrlreg1.pnamesize = SCL_PN1WORD;        
    SMV2_nscrlreg1.platesize = SCL_PL_SIZE_1X1;    
    SMV2_nscrlreg1.bmpsize = SCL_BMP_SIZE_512X256; 
    SMV2_nscrlreg1.coltype = SCL_COL_TYPE_256;     
    SMV2_nscrlreg1.datatype = SCL_CELL;            
    SMV2_nscrlreg1.flip = SCL_PN_12BIT;            
    SMV2_nscrlreg1.mapover = SCL_OVER_0;           
    SMV2_nscrlreg1.plate_addr[0] = NBG1_PTNAMA;    
    SMV2_nscrlreg1.plate_addr[1] = NBG1_PTNAMA;    
    SMV2_nscrlreg1.plate_addr[2] = NBG1_PTNAMA;    
    SMV2_nscrlreg1.plate_addr[3] = NBG1_PTNAMA;    
    SCL_SetConfig(SCL_NBG1, &SMV2_nscrlreg1);
    SCL_Close();


    Scl_n_reg.linecolmode = 0x00000000 | ((LCLRTBLADD & 0xfffff) / 2);
    Scl_n_reg.backcolmode = 0x00000000 | ((BCLRTBLADD & 0xfffff) / 2);
    poke_w(BCLRTBLADD, 0x0000);


    SMV2_FilMem32((Uint32 *)NBG_PTGEN,   0x0100 / sizeof (Uint32), 0x0000);
    SMV2_FilMem16((Uint16 *)NBG0_PTNAMA, 0x2000 / sizeof (Uint16), 0x0000);
    SMV2_FilMem16((Uint16 *)NBG1_PTNAMA, 0x2000 / sizeof (Uint16), 0x0000);
    SMV2_FilMem16((Uint16 *)COLOR_RAM,   0x0800 / sizeof (Uint16), 0x0000);




    SCL_InitConfigTb(&SMV2_rscrlreg);    /* RBG0 Scroll Config Set         */
    SCL_SetConfig(SCL_RBG0, &SMV2_rscrlreg);

    SMV2_FilMem32((Uint32 *)RBG0_PTGEN, 
                  0x100 / sizeof (Uint32), 
                  0x00000000);
    SMV2_FilMem32((Uint32 *)RBG0_PTNAM, 
                  0x4000 / sizeof (Uint32),
                  (RBG0_PTGEN & 0xfffff) / 0x20);





}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

 
/*=====================================
 *    アスキーフォント展開(4bit color)
 *    vramadd   : 展開先vramaddress
 *    fontcolor : 文字のカラー
 *    backcolor : 文字以外のカラー
 *====================================*/
void SMV2_SetFont4(Uint8 *vramadd, Uint32 fontcolor, Uint32 backcolor)
{
    Uint32 cgsize;
    Uint8 rdata;
    Uint8 *cgptr;
    Uint8 fontdata[4];

    SMV2_fontoffset = (((Uint32)vramadd) & 0x7ffff) / 0x40;

    fontcolor &= 0xf;
    backcolor &= 0xf;
    fontdata[0] = (backcolor << 4) + backcolor;   /* 00 */
    fontdata[1] = (backcolor << 4) + fontcolor;   /* 01 */
    fontdata[2] = (fontcolor << 4) + backcolor;   /* 10 */
    fontdata[3] = (fontcolor << 4) + fontcolor;   /* 11 */

    cgptr = font8cg;
    for (cgsize = 0; cgsize < 64 * 8; cgsize++) {

        rdata = *cgptr;
        cgptr++;

        *vramadd = fontdata[(rdata >> 6) & 3];         
        vramadd++;

        *vramadd = fontdata[(rdata >> 4) & 3];         
        vramadd++;

        *vramadd = fontdata[(rdata >> 2) & 3];         
        vramadd++;

        *vramadd = fontdata[(rdata >> 0) & 3];         
        vramadd++;
    }


}



/*========================================
 *    アスキーフォント展開(8bit color)
 *    vramadd   : 展開先vramaddress
 *    fontcolor : 文字のカラー
 *    backcolor : 文字以外のカラー
 *======================================*/
void SMV2_SetFont8(Uint8 *vramadd, Uint32 fontcolor, Uint32 backcolor)
{
    Uint32 cellcnt, bitcnt;
    Uint8 rdata;
    Uint8 *cgptr;
    Uint8 fontdata[4];

    SMV2_fontoffset = (((Uint32)vramadd) & 0x7ffff) / 0x20;

    fontcolor &= 0xff;
    backcolor &= 0xff;

    cgptr = font8cg;
    for (cellcnt = 0; cellcnt < 64*8; cellcnt++) {
        rdata = *cgptr;
        cgptr++;
        for (bitcnt = 0; bitcnt < 8; bitcnt++) {
            if (((rdata >> (7-bitcnt)) & 1) == 1)
                *vramadd = fontcolor;
            else
                *vramadd = backcolor;

            vramadd++;
        }

    }

}




/*===================================
 *  アスキー文字表示(4bit color )
 *==================================*/
void SMV2_PutAsc4(Uint16 *wradd, Uint8 *mesadd)
{
    Uint8 moji;
    Uint16 wrdata;

    while ((moji = *mesadd++ ) != 0) {
        wrdata = (Uint16)(moji - ' ') + SMV2_fontoffset;
        *wradd++ = wrdata; 
    }

}

/*==================================
 *  アスキー文字表示(8bit color )
 *=================================*/
void SMV2_PutAsc8(Uint16 *wradd, Uint8 *mesadd)
{
    Uint8 moji;
    Uint16 wrdata;

    while ((moji = *mesadd++ ) != 0) {
        wrdata = (Uint16)(moji - ' ') + SMV2_fontoffset;
        wrdata = wrdata << 1;
        wrdata += SMV2_fontoffset;
        *wradd++ = wrdata; 
    }

}




/*============================================
 *  SCROLL BOX WRITE(64セル境界非対応 WORD)
 *===========================================*/
void SMV2_BoxWrite16(Uint16 *wradd, Uint16 *mapdata, 
                                              Uint16 hsize, Uint16 vsize)
{
    Uint16 h, v;
    Uint16 *dst0, *dst1;
    
    dst0 = wradd;
    for (v = 0; v < vsize; v++) {
        dst1 = dst0;
        for (h = 0; h < hsize; h++) {
	     *dst1 = *mapdata;
             dst1++;
             mapdata++;
        }
	dst0 += 0x40;           /* NEXT LINE OFFSET */
    }
}


/*====================================================
 *  SCROLL BOX WRITE (64セル境界非対応 LONG WORD)
 *===================================================*/
void SMV2_BoxWrite32(Uint32 *wradd, Uint32 *mapdata,
                                              Uint16 hsize, Uint16 vsize)
{
    Uint16 h, v;
    Uint32 *dst0, *dst1;
    
    dst0 = wradd;
    for (v = 0; v < vsize; v++) {
        dst1 = dst0;
        for (h = 0; h < hsize; h++) {
	     *dst1 = *mapdata;
             dst1++;
             mapdata++;
        }
	dst0 += 0x40;           /* NEXT LINE OFFSET */
    }
}
    

/*==================================
 *  SCROLL BOX FILL WRITE (WORD)
 *=================================*/
void SMV2_BoxFill16(Uint16 *wradd, Uint16 wrdata, Uint16 hsize, Uint16 vsize)
{
    Uint16 h, v;
    Uint16 *dst0, *dst1;
    
    dst0 = wradd;
    for (v = 0; v < vsize; v++) {
        dst1 = dst0;
        for (h = 0; h < hsize; h++) {
	     *dst1 = wrdata;
             dst1++;
        }
	dst0 += 0x40;           /* NEXT LINE OFFSET */
    }
}


/*==================================
 *  SCROLL BOX FILL WRITE (WORD)
 *=================================*/
void SMV2_BoxFil16(Uint16 *wradd, Uint16 wrdata, Uint16 hsize, Uint16 vsize)
{
    Uint16 h, v;
    Uint16 *dst0, *dst1;
    
    dst0 = wradd;
    for (v = 0; v < vsize; v++) {
        dst1 = dst0;
        for (h = 0; h < hsize; h++) {
	     *dst1 = wrdata;
             dst1++;
        }
	dst0 += 0x40;           /* NEXT LINE OFFSET */
    }
}


/*======================================
 *  SCROLL BOX FILL WRITE (LONG WORD)
 *=====================================*/
void SMV2_BoxFil32(Uint32 *wradd, Uint32 wrdata, Uint16 hsize, Uint16 vsize)
{
    Uint16 h, v;
    Uint32 *dst0, *dst1;
    
    dst0 = wradd;
    for (v = 0; v < vsize; v++) {
        dst1 = dst0;
        for (h = 0; h < hsize; h++) {
	     *dst1 = wrdata;
             dst1++;
        }
	dst0 += 0x40;           /* NEXT LINE OFFSET */
    }
}



/*========================
 *  FILL MEMORY (LONG)
 *=======================*/    
void SMV2_FilMem32(Uint32 *memadd, Uint32 len, Uint32 fildata)
{
    while (len--) {
        *memadd++ = fildata;
    }

#if 0  /* <<<<<<<<<<<< */
    do {
        *memadd++ = fildata;
        len--;
    } while (len != 0);
#endif /* <<<<<<<<<<<< */

}


/*=========================
 *  FILL MEMORY (WORD)
 *========================*/    
void SMV2_FilMem16(Uint16 *memadd, Uint32 len, Uint16 fildata)
{

    while (len--) {
        *memadd++ = fildata;
    }

#if 0  /* <<<<<<<<< */
    do {
        *memadd++ = fildata;
        len--;
    } while (len != 0);
#endif /* <<<<<<<<< */


}


/*============
 *  TV On
 *===========*/
void SMV2_TvOn(void)
{
    SclSysreg *sysreg;
    Uint16 d0;


    d0 = *(Uint16 *)(SCRL_TVMOD);
    d0 &= 0x7fff;
    d0 |= 0x8000;                        /* バック画面表示 */
    *(Uint16 *)(SCRL_TVMOD) = d0;

    Scl_s_reg.tvmode &= 0x7fff;
    Scl_s_reg.tvmode |= 0x8000;


}

/*==============
 *   Tv Off
 *=============*/
void SMV2_TvOff(void)
{
    SclSysreg *sysreg;
    Uint16 d0;

    d0 = *(Uint16 *)(SCRL_TVMOD);
    d0 &= 0x7eff;
    *(Uint16 *)(SCRL_TVMOD) = d0;

    Scl_s_reg.tvmode &= 0x7fff;


}










