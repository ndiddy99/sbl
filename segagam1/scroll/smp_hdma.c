/*	Copyright(c) 1994 SEGA			*/
/*
 *
 *      SCU_DMA で VDP2 へ 送る サンプル
 *
 */

#include "smp_bios.h"

#include "smp_sccg.h"
#include "smp_ncg.h"

#include "smp_hdma.h"

/*
 *     外部参照ワーク
 */
extern SclRotscl Scl_r_reg;         
extern SclSysreg Scl_s_reg;
extern SclDataset Scl_d_reg;
extern SclNorscl Scl_n_reg;
extern SclWinscl Scl_w_reg;

extern ROTPARAM SMRK_rotregbufA; 
extern ROTPARAM SMRK_rotregbufB; 
extern Uint32 SMRK_RotregReq;
extern Uint32 SMRK_KeiEnd;
extern Sint16 SMRK_KeisuBuf[0xa000];
extern Sint16 SMRK_FilData;
extern Uint8  SMRK_sqrttbl[0x4000];



/* <<<< 関数プロトタイプ >>>> */
void SMHD_cgset(void);
void SMHD_MapWrite32(Uint32 *wradd, Uint16 *mapdata, 
                                              Uint16 hsize, Uint16 vsize);
void SMHD_VintHook(void);
void SMHD_SCUDMA_LDIR0(Uint32 src, Uint32 dst, Uint32 trcnt);
void SMHD_CPUDmaLdir160(Uint32 srcadd, Uint32 dstadd, Uint32 trcnt);

void SMHD_MainContrl(SMHDMAINACT *ix);
void SMHD_VintMode1(void);
void SMHD_Set1Font4(Uint32 chrno, Uint8 *vramadd, 
                             Uint32 fontcolor, Uint32 backcolor);


/* スクロールVRAMアクセスサイクルパターンテーブル */
const Uint16 SMHD_CYC[] = { 0x0c44, 0x44ff, 
                            0xffff, 0xffff, 
                            0xffff, 0xffff, 
                            0xffff, 0xffff };



/* 定数 */
#define SMRK_KEICNTRL_DAT (0x13) /* 係数テーブルコントロールレジスタへの値 */



/*
 *  メインルーチン
 */
void SMHD_MainMode(void)
{
    enum {
        INIT,
        MAIN,
        NEXT
      };

    Uint16 i, j, k, swedge;
    Uint32 times;
    SclRotscl *rotreg;
    ROTPARAM *rp_ptr;

    while (SMMA_MainMode == SMHD_MODE) {
        SMV1_SprCmdStart();                /* VDP1 SPRITE START */

        swedge = SMPA_pad_edge1;
        switch (SMMA_MainLevel) {
            case INIT:
                SMV2_TvOff();
                SMMA_initall();

                SMRK_RotregReq = 0; /* スクロール転送要求フラグクリア */

                SMV2_TvOff();
                SMV1_SprCmdEnd();
                SMMA_intwait();
                SMMA_intwait();

                SCL_Vdp2Init();
                SCL_SetFrameInterval(0xffff);   /* Intrpt Mode  */


                /* カラーモードの設定 */
                SCL_SetColRamMode(SCL_CRM15_1024);

                SCL_InitVramConfigTb(&SMV2_sclvram);
                SMV2_sclvram.vramModeA = ON;
                SMV2_sclvram.vramModeB = ON; 
                SMV2_sclvram.vramA0 = SCL_NON;
                SMV2_sclvram.vramA1 = SCL_NON;
                SMV2_sclvram.vramB0 = SCL_RBG0_PN;
                SMV2_sclvram.vramB1 = SCL_RBG0_K;
                SCL_SetVramConfig(&SMV2_sclvram);

                SCL_Open(SCL_NBG0);
                SCL_InitConfigTb(&SMV2_nscrlreg0);
                SMV2_nscrlreg0.dispenbl = ON;        
                SMV2_nscrlreg0.charsize = SCL_CHAR_SIZE_1X1; 
                SMV2_nscrlreg0.pnamesize = SCL_PN1WORD;      
                SMV2_nscrlreg0.platesize = SCL_PL_SIZE_1X1;  
                SMV2_nscrlreg0.bmpsize = SCL_BMP_SIZE_512X256; 
                SMV2_nscrlreg0.coltype = SCL_COL_TYPE_16;     
                SMV2_nscrlreg0.datatype = SCL_CELL;    
                SMV2_nscrlreg0.flip = SCL_PN_12BIT;
                SMV2_nscrlreg0.mapover = SCL_OVER_0;   
                SMV2_nscrlreg0.plate_addr[0] = SMHD_NBG0_PTNAMA;
                SMV2_nscrlreg0.plate_addr[1] = SMHD_NBG0_PTNAMB;
                SMV2_nscrlreg0.plate_addr[2] = SMHD_NBG0_PTNAMC;
                SMV2_nscrlreg0.plate_addr[3] = SMHD_NBG0_PTNAMD;
                SCL_SetConfig(SCL_NBG0, &SMV2_nscrlreg0);
                SCL_SetCycleTable(SMHD_CYC);
                SCL_Close();

                SCL_Open(SCL_NBG1);
                SCL_InitConfigTb(&SMV2_nscrlreg1);
                SCL_SetConfig(SCL_NBG1, &SMV2_nscrlreg1);
                SCL_Close();

                Scl_n_reg.linecolmode = 0x00000000 |
                                         ((SMHD_LCLRTBLADD & 0xfffff) / 2);
                Scl_n_reg.backcolmode = 0x00000000 |
                                         ((SMHD_BCLRTBLADD & 0xfffff) / 2);

                poke_w(SMHD_BCLRTBLADD, 0x0000);

                SCL_Open(SCL_RBG0);
                SCL_InitConfigTb(&SMV2_rscrlreg);
                SMV2_rscrlreg.dispenbl = OFF;        
                SMV2_rscrlreg.charsize = SCL_CHAR_SIZE_1X1; 
                SMV2_rscrlreg.pnamesize = SCL_PN2WORD;      
                SMV2_rscrlreg.platesize = SCL_PL_SIZE_1X1;  
                SMV2_rscrlreg.bmpsize = SCL_BMP_SIZE_512X256; 
                SMV2_rscrlreg.coltype = SCL_COL_TYPE_256;     
                SMV2_rscrlreg.datatype = SCL_CELL;    
                SMV2_nscrlreg0.flip = SCL_PN_12BIT;
                SMV2_rscrlreg.mapover = SCL_OVER_0;   
                for (i = 0; i < 16; i++) {
                     SMV2_rscrlreg.plate_addr[i] = SMHD_RBG0_PTNAM;
                }
                SCL_SetConfig(SCL_RBG0, &SMV2_rscrlreg);
                SCL_InitRotateTable(SMHD_RBG0_RPAR, 1, SCL_RBG0, SCL_NON);
                SCL_Close();

                /* プライオリティの設定 */
                SCL_SetPriority(SCL_SP0, 7);
                SCL_SetPriority(SCL_NBG0, 6);
                SCL_SetPriority(SCL_NBG1, 0);
                SCL_SetPriority(SCL_RBG0, 0);

                SMV2_TvOff();
                SMV1_SprCmdEnd();
                SMMA_intwait();
                SMMA_intwait();

                SCL_Open(SCL_RBG0);
                SCL_SetConfig(SCL_RBG0, &SMV2_rscrlreg);
                SCL_InitRotateTable(SMHD_RBG0_RPAR, 1, SCL_RBG0, SCL_NON);
	        Scl_r_reg.paramode = 0x0; /* 回転パラメータモード３ */
                Scl_r_reg.k_contrl = 0x0000;
                Scl_r_reg.k_offset = (0x40000 / 0x20000) * 0x100
                                   + (0x40000 / 0x20000);
                Scl_s_reg.dispenbl |= 0x1000;
                Scl_s_reg.ramcontrl |= 0x0000; /* 係数指定 */
                SCL_Close();

                  
                /* Int Hook1 */
                SMIN_VintHook1  = &SMHD_VintHook;

                /* CGデータの展開 */
                SMHD_cgset(); 

                /* アクションのセット */
                SMTA_MakeAction(SMHD_MainContrl);
                SMV2_TvOn();               
                SMMA_MainLevel++;

            case MAIN:
                SCL_Open(SCL_RBG0);
                SCL_Close();
                SCL_Open(SCL_NBG0);
                SCL_Close();
                SMTA_ActionLoop();  /* アクション実行   */

                if (swedge & PAD_START) SMMA_MainLevel = NEXT;
                break;

            case NEXT:
                SMMA_MainMode = SMSC_SELECTMODE;
                SMMA_MainLevel = 0;
                break;
	}
 
        SMV1_SprCmdEnd();
        SMMA_intwait();        /* V-BLANK WAIT */

    }; /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

}



/*
 *    CGSET
 */
void SMHD_cgset(void)
{
     Uint16 i, j, k;
     Sint16 direc;
     Uint8 *wkptr;
     Uint16 *dstptrb, *dstptr, *page;
     Uint16 wrdata;
     Uint32 *dst32;
     Uint32 dt;


     /* ASCII CG SCROLL SET */
     SMV2_SetFont4((Uint8 *)SMHD_NBG_PTGEN, 1, 0);

     /* ASCII CG SPRITE SET */
     SMV1_SetFont4sp(0, 1, 0);


     /* ---- ラインカラーの設定 ---- */
     dstptr = (Uint16 *)(COLOR_RAM + 0x400);
     *dstptr++ = (0x00 << 10) + (0x00 << 5) + 0x00; /* 0 */
     *dstptr++ = (0x00 << 10) + (0x00 << 5) + 0x00; /* 1 */
     *dstptr++ = (0x00 << 10) + (0x00 << 5) + 0x00; /* 2 */
     *dstptr++ = (0x00 << 10) + (0x00 << 5) + 0x00; /* 3 */
     *dstptr++ = (0x00 << 10) + (0x00 << 5) + 0x00; /* 4 */
     *dstptr++ = (0x00 << 10) + (0x00 << 5) + 0x00; /* 5 */
     *dstptr++ = (0x00 << 10) + (0x00 << 5) + 0x00; /* 6 */
     *dstptr++ = (0x00 << 10) + (0x00 << 5) + 0x00; /* 7 */
     *dstptr++ = (0x00 << 10) + (0x00 << 5) + 0x00; /* 8 */
     *dstptr++ = (0x01 << 10) + (0x1f << 5) + 0x08; /* 9 */
     *dstptr++ = (0x14 << 10) + (0x00 << 5) + 0x00; /* A */
     *dstptr++ = (0x00 << 10) + (0x1f << 5) + 0x00; /* B */
     *dstptr++ = (0x00 << 10) + (0x00 << 5) + 0x1f; /* C */
     *dstptr++ = (0x14 << 10) + (0x00 << 5) + 0x1f; /* D */
     *dstptr++ = (0x14 << 10) + (0x1f << 5) + 0x00; /* E */
     *dstptr++ = (0x00 << 10) + (0x1f << 5) + 0x1f; /* F */

     dstptr = (Uint16 *)(SMHD_LCLRTBLADD);
     for (i = 0; i < 0x100; i++) {
          direc = (i * 0x80) / 0x100;
          *dstptr = (0x400/2); /*  + (0x1f * SMMI_Cosset8(direc)) / 0x100; */
          dstptr++;
     }

     SCL_Open(SCL_NBG0);
     Scl_n_reg.linecolmode = 0x80000000 | ((SMHD_LCLRTBLADD & 0xfffff) / 2);
     SCL_Close();

     /* カラー演算モードの設定 */
     SCL_SET_CCMD(1);    /* カラー演算そのまま加算イネーブル */
     SCL_SET_CCRTMD(1);  /* 割合を2nd側で指定             */
     SCL_SET_R0CCRT(0x8);
     SCL_SET_LCCCRT(0x1);
     SCL_SET_N0CCEN(0);  /* NBG0カラー演算イネーブル         */
     SCL_SET_R0CCEN(0);  /* 回転面カラー演算イネーブル       */
     SCL_SET_SPCCEN(0);  /* スプライトカラー演算イネーブル   */
     SCL_SET_LCCCEN(0);  /* ラインカラーイネーブル           */
     SCL_SET_R0LCEN(0);  /* ラインカラー                     */
     SCL_SET_SPLCEN(0);  /* ラインカラー                     */
     SCL_SET_EXCCEN(0);  /* 拡張カラー演算                   */

}




/*======================================================
 *  SMHD専用回転SCROLL BOX WRITE(64セル境界非対応 WORD)
 *=====================================================*/
void SMHD_MapWrite32(Uint32 *wradd, Uint16 *mapdata, 
                                              Uint16 hsize, Uint16 vsize)
{
    Uint16 h, v;
    Uint32 *dst0, *dst1;
    Uint32 rdata0, rdata1;
    
    dst0 = wradd;
    for (v = 0; v < vsize; v++) {
        dst1 = dst0;
        for (h = 0; h < hsize; h++) {
             rdata0 = *mapdata;
             mapdata++;
             rdata0 *= 2;
             rdata0 += 0;
 	     *dst1 = rdata0;
             dst1++;
        }
	dst0 += 0x40;           /* NEXT LINE OFFSET */
    }
}










/*=================================
 *   VBlankIn関数内から呼ばれる
 *===============================*/ 
void SMHD_VintHook(void)
{

    poke_w(SMHD_BCLRTBLADD, 0x7000);
    poke_w(SMHD_BCLRTBLADD, 0x0000);

}

/*=================================
 *   VBlankIn関数内からMode1
 *===============================*/ 
void SMHD_VintMode1(void)
{
    Uint32 *wrptr32;
    Uint32 i;

    poke_w(SMHD_BCLRTBLADD, 0x7000);

    wrptr32 = (Uint32 *)(SCU_DMA0REG + SCUDMA_SRCADD);
    *wrptr32 = (Uint32)&SMHD_CellBuf[0];

    wrptr32 = (Uint32 *)(SCU_DMA0REG + SCUDMA_DSTADD);
    *wrptr32 = (Uint32)SMHD_NBG_PTGEN;

    wrptr32 = (Uint32 *)(SCU_DMA0REG + SCUDMA_SIZE);
    *wrptr32 = 0x20;

    wrptr32 = (Uint32 *)(SCU_DMA0REG + SCUDMA_INC);
    *wrptr32 = (1 << 8) + 1; /* 4byte add mode  */

    wrptr32 = (Uint32 *)(SCU_DMA0REG + SCUDMA_MODE);
    *wrptr32 = (0 << 24) + (0 << 16) + (1 << 8) + (7); /* MODE */

    wrptr32 = (Uint32 *)(SCU_DMA0REG + SCUDMA_ENBL);
    *wrptr32 = (1 << 8) + (1);  /* DMA START */


    wrptr32 = (Uint32 *)(SCU_DMASTS); 
    while (((*wrptr32) & ((1 << 16) + (1 << 5) + (1 << 4))) != 0) {
    }

 
    poke_w(SMHD_BCLRTBLADD, 0x0000);

}


/*==================================
 *     メインコントロール
 *=================================*/
void SMHD_MainContrl(SMHDMAINACT *ix)
{

    enum {
        INIT,
        MODE1_I, MODE1_M,
        NEXT
	};

    Uint16 swdata, swedge;
    Uint32 i;
    Uint16 h, v;
    Uint16 *dst0, *dst1;
    Uint16 wdata16;
    Uint32 chrno;




    swdata = SMPA_pad_data1;
    swedge = SMPA_pad_edge1;

    switch (ix->mode) {
        case INIT:
            ix->mode++;
            break;

        case MODE1_I:
            dst1 = (Uint16 *)SMHD_NBG0_PTNAMA;
            wdata16 = 0x01;
            for (v = 0; v < 32; v++) {
                dst0 = dst1;
                dst1 += 64;
                for (h = 0; h < 40; h++) {
                    *dst0 = wdata16;
                    wdata16++;
                    dst0++;
                }
             }

            
            
            /* Int Hook1 */
            SMIN_VintHook1  = &SMHD_VintMode1;
            poke_w(COLOR_RAM + 0x0, rgb(0,0,0));
            poke_w(COLOR_RAM + 0x2, rgb(30,30,30));

            ix->wcnt[0] = '0';
            ix->wcnt[1] = 0;
            ix->mode++;
        case MODE1_M:
            chrno = ix->wcnt[0] - ' ';
            SMHD_Set1Font4(chrno, SMHD_CellBuf, 1, 0);

            ix->wcnt[1]++;
            if (ix->wcnt[1] > 0x40) {
                ix->wcnt[1] = 0;
                ix->wcnt[0]++;
                if (ix->wcnt[0] > '9') ix->wcnt[0] = '0';
            }
            break;
    }

}
    



/*================================================
 *    アスキーフォント１文字展開4 bit color
 *    chrno     : 展開するキャラクタNo
 *    vramadd   : 展開先vramaddress
 *    fontcolor : 文字のカラー
 *    backcolor : 文字以外のカラー
 *===============================================*/
void SMHD_Set1Font4(Uint32 chrno, Uint8 *vramadd, 
                             Uint32 fontcolor, Uint32 backcolor)
{
    Uint32 cellcnt, bitcnt;
    Uint8 rdata;
    Uint8 *cgptr;
    Uint8 fontdata[4];

    fontcolor &= 0xf;
    backcolor &= 0xf;
    fontdata[0] = (backcolor << 4) + backcolor;   /* 00 */
    fontdata[1] = (backcolor << 4) + fontcolor;   /* 01 */
    fontdata[2] = (fontcolor << 4) + backcolor;   /* 10 */
    fontdata[3] = (fontcolor << 4) + fontcolor;   /* 11 */

    cgptr = font8cg + (chrno * 8);
    for (cellcnt = 0; cellcnt < 8; cellcnt++) {

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



#if 0
#define SCU_DMA0REG 0x25fe0000      /* LEVEL 0 DMA セットレジスタ       */
#define SCU_DMA1REG 0x25fe0020      /* LEVEL 1 DMA セットレジスタ       */
#define SCU_DMA2REG 0x25fe0040      /* LEVEL 2 DMA セットレジスタ       */
#define SCUDMA_SRCADD 0x0000        /* ソースアドレス                   */
#define SCUDMA_DSTADD 0x0004        /* ディストネーションアドレス       */
#define SCUDMA_SIZE   0x0008        /* 転送サイズ                       */
#define SCUDMA_INC    0x000c        /* 終了コード、加算値レジスタ       */
#define SCUDMA_ENBL   0x0010        /* DMA許可レジスタ                  */
#define SCUDMA_MODE   0x0014        /* DMAモードレジスタ                */

#define SCU_DMASET  0x25fe0060      /* DMAセットレジスタ                */
#define SCU_DMASTS  0x25fe0070      /* DMAステータスレジスタ            */
#define SCUDMA_STSRC 0x0000         /* 中断時のソースアドレス           */
#define SCUDMA_STDST 0x0004         /* 中断時のディストネーションアドレス */
#define SCUDMA_STSIZ 0x0008         /* 中断時のコントロールサイズ         */
#define SCUDMA_CNTRL 0x000c         /* アクセス,中断,待機,動作レジスタ    */

#endif

/*
 *   SCU_DMA
 */
void SMHD_SCUDMA_LDIR0(Uint32 src, Uint32 dst, Uint32 trcnt)
{
    Uint32 *wrptr32;

    wrptr32 = (Uint32 *)(SCU_DMA0REG + SCUDMA_SRCADD);
    *wrptr32 = src;

    wrptr32 = (Uint32 *)(SCU_DMA0REG + SCUDMA_DSTADD);
    *wrptr32 = dst;

    wrptr32 = (Uint32 *)(SCU_DMA0REG + SCUDMA_SIZE);
    *wrptr32 = trcnt;

    wrptr32 = (Uint32 *)(SCU_DMA0REG + SCUDMA_INC);
    *wrptr32 = (1 << 8) + 1; /* 4byte add mode  */

    wrptr32 = (Uint32 *)(SCU_DMA0REG + SCUDMA_MODE);
    *wrptr32 = (0 << 24) + (0 << 16) + (1 << 8) + (7); /* MODE */

    wrptr32 = (Uint32 *)(SCU_DMA0REG + SCUDMA_ENBL);
    *wrptr32 = (1 << 8) + (1);  /* DMA START */

}






/*===========================================================================
 *    メモリ→メモリ間１６バイトモードCPUDMA転送 (CH#0 サイクルスチールモード)
 *    注意！！: SDRAM以外での動作は知らない
 *              必ず１６バイト境界のアドレスセットすること
 *    srcadd : 転送元アドレス
 *    dstadd : 転送先アドレス
 *    trcnt  : 転送サイズ(バイトサイズ)
 *==========================================================================*/
void SMHD_CPUDmaLdir160(Uint32 srcadd, Uint32 dstadd, Uint32 trcnt)
{
    Uint32 d0, d1;

    d0 = *(Uint32 *)CDMA_OPR;
    d0 &= 0x1;
    d0 |= 0x1;
    *(Uint32 *)CDMA_OPR = d0;

    *(Uint32 *)CDMA0_SRC = srcadd;
    *(Uint32 *)CDMA0_DST = dstadd;
    *(Uint32 *)CDMA0_LEN = (trcnt+3)>>2;
    d0 = *(Uint32 *)CDMA0_CNT;
    *(Uint32 *)CDMA0_CNT = 0x5e01;        /* src++ dst++ NoINT */
	
    do {
        d0 = *(Uint32 *)CDMA0_CNT;
    } while ((d0 & 2) == 0);
	
}





