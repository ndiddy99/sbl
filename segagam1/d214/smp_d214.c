/*	Copyright(c) 1994 SEGA			*/
/*
 *
 *      2/14 DEMO   PROGRAM FILE
 *
 */


#include "smp_bios.h"

#include "smp_dcg0.h"
#include "smp_dcg1.h"
#include "smp_dcg2.h"
#include "smp_dcg3.h"
#include "smp_dcg4.h"
#include "smp_dcg5.h"
#include "smp_dcg6.h"
#include "smp_dswd.h"

#include "smp_d214.h"


/*
 *     外部参照ワーク
 */
extern SclRotscl Scl_r_reg;         
extern SclSysreg Scl_s_reg;
extern SclDataset Scl_d_reg;
extern SclNorscl Scl_n_reg;
extern SclWinscl Scl_w_reg;



/* 定数 */
#define AUTOD 1           /* AUTODEMO? NO..スイッチデータサンプリング  */

#define ZYURE 0x12             /*　揺れの幅         */
#define DZDIR 0x40             /*  角度差           */
#define YUKADIRSPD 0xc         /* 床角度速度        */
#define ZKYORIMAX 0x400        /* ズーム率最大      */
#define ZKYORIMIN 0x100        /* ズーム率最小      */
#define GROWLEVEL 0x12         /* GROW LEVEL        */
#define YUKASIZE 0x0420        /* 床元絵サイズ      */
#define ONPLAYER    1          /*  プレーヤーを出す */
#define ASCRLSPD 0x40000       /* スクロール速度    */
#define NBG0_INI_HSCRL (0x0000)      /* 初期hスクロール量 */
#define NBG0_INI_VSCRL (0xf8*0x10000)   /* 初期vスクロール量 */
#define NBG0_HSPD (0x10000*4)  /* 背景の横スクロールスピード */
#define NBG0_VSPD (0x60000) /* 背景の縦スクロールスピード 0x100000/4 */

#define JOIN_MAXPAT 56 -1   /* JOIN ME MAX PATTERN                */
#define SMM_MAXPAT 40-1     /* モーフィングソニック２ MAX PATTERN */
#define AKIRA_MAXPAT 32-1   /* アキラ MAX PATTERN                 */
#define SGAME_MAXPAT 44-1   /* SGAME MAX PATTERN                  */

/* キーアサイン */
#define XDIRUP      PAD_UP      /* x軸回転角度　+    */
#define XDIRDOWN    PAD_DOWN    /* x軸回転角度  -    */
#define ZDIRUP      PAD_LEFT    /* z軸回転角度  +    */
#define ZDIRDOWN    PAD_RIGHT   /* z軸回転角度  -    */
#define ZKDEC       PAD_C       /* ZKYORI減らす      */
#define ZKINC       PAD_A       /* ZKYORI増やす      */
#define MDCHG       PAD_R        /* 表示形態変化      */
#define MXCHG       PAD_Z       /* MX増減切り替え    */
#define HALFCC      PAD_B       /* カラー演算        */



/*
 *   関数プロトタイプ
 */
void SMD2_cgset(void);
void SMD2_DemoMain(SMD2ACT *ix);
void SMD2_XWaveMain(SMD2ACT *ix);
void SMD2_XWaveObj(SMD2ACT *ix);
void SMD2_XWaveObjS(SMD2ACT *ix);
void SMD2_XWavePlay(SMD2ACT *ix);
void XWaveZposiSet(SMD2ACT *ix);
void SMD2_XWaveKage(SMD2ACT *ix);

Sint16 Xrot(XROT_INFO *wkptr);


void XWaveObj_zbufput(SPRITE *sprite);
void XWaveObj_zbufputOld(SPRITE *sprite);
void XZRotScrlCnt(Sint16 xdirec, Sint16 zdirec);
void SprSet(void);
void SMD2_BoxWrite16(Uint16 *wradd, Uint16 *mapdata, 
                                              Uint16 hsize, Uint16 vsize);

void SMD2_MapWrite32(Uint32 *wradd, Uint16 *mapdata, 
                                              Uint16 hsize, Uint16 vsize);

void SMD2_VintRotRegSet(void);

/* 定数 */

enum {
      yuka_0,
      joina_0, joina_1, joina_2, joina_3, joina_4, joina_5,
      joina_6, joina_7, joina_8, joina_9, joina_A, joina_B,

      joinb_0, joinb_1, joinb_2, joinb_3, joinb_4, joinb_5,
      joinb_6, joinb_7, joinb_8, joinb_9, joinb_A, joinb_B,

      akraa_0, akraa_1, akraa_2, akraa_3, akraa_4, akraa_5,
      akraa_6, akraa_7, akraa_8, akraa_9, akraa_A, akraa_B,

      akrab_0, akrab_1, akrab_2, akrab_3, akrab_4, akrab_5,
      akrab_6, akrab_7, akrab_8, akrab_9, akrab_A, akrab_B,

      smma_0, smma_1, smma_2, smma_3, smma_4, smma_5,
      smma_6, smma_7, smma_8, smma_9, smma_A, smma_B,
      smmb_0, smmb_1, smmb_2, smmb_3, smmb_4, smmb_5,
      smmb_6, smmb_7, smmb_8, smmb_9, smmb_A, smmb_B,

      sgamea_0, sgamea_1, sgamea_2, sgamea_3, sgamea_4, sgamea_5,
      sgamea_6, sgamea_7, sgamea_8, sgamea_9, sgamea_A, sgamea_B,

      sgameb_0, sgameb_1, sgameb_2, sgameb_3, sgameb_4, sgameb_5,
      sgameb_6, sgameb_7, sgameb_8, sgameb_9, sgameb_A, sgameb_B
     };


/* 床スプライトパターンNo */
const Uint8 yuka_ofstbl[] = {
	yuka_0
	};

/*
 * 絵のアニメーション用パターンNoテーブル
 */
/* 箱1用 */
const Uint8 joina_ofstbl[] = {
     joina_0, joina_0,
     joina_0, joina_0,
     joina_0, joina_0,
     joina_0, joina_0,

     joina_0, joina_0,
     joina_0, joina_0,
     joina_0, joina_0,
     joina_0, joina_0,

     joina_0, joina_1,
     joina_2, joina_3,
     joina_4, joina_5,
     joina_6, joina_7,

     joina_8, joina_9,
     joina_A, joina_B,

     joina_B, joina_B,
     joina_B, joina_B,

     joina_B, joina_B,
     joina_B, joina_B,
     joina_B, joina_B,
     joina_B, joina_B,

     joina_B, joina_B,
     joina_B, joina_B,
     joina_B, joina_A,
     joina_9, joina_8,

     joina_7, joina_6,
     joina_5, joina_4,
     joina_3, joina_2,
     joina_1, joina_0       
    };	

/* 箱1用 */
const Uint8 joinb_ofstbl[] = {
     joinb_0, joinb_0,
     joinb_0, joinb_0,
     joinb_0, joinb_0,
     joinb_0, joinb_0,

     joinb_0, joinb_0,
     joinb_0, joinb_0,
     joinb_0, joinb_0,
     joinb_0, joinb_0,

     joinb_0, joinb_1,
     joinb_2, joinb_3,
     joinb_4, joinb_5,
     joinb_6, joinb_7,

     joinb_8, joinb_9,
     joinb_A, joinb_B,

     joinb_B, joinb_B,
     joinb_B, joinb_B,

     joinb_B, joinb_B,
     joinb_B, joinb_B,
     joinb_B, joinb_B,
     joinb_B, joinb_B,

     joinb_B, joinb_B,
     joinb_B, joinb_B,
     joinb_B, joinb_A,
     joinb_9, joinb_8,

     joinb_7, joinb_6,
     joinb_5, joinb_4,
     joinb_3, joinb_2,
     joinb_1, joinb_0       
    };	


/* 箱4用 AKIRA */
const Uint8 akiraa_ofstbl[] = {
     akraa_0, akraa_0,
     akraa_0, akraa_0,

     akraa_0, akraa_1,
     akraa_2, akraa_3,
     akraa_4, akraa_5,
     akraa_6, akraa_7,
     akraa_8, akraa_9,
     akraa_A, akraa_B,

     akraa_B, akraa_B,
     akraa_B, akraa_B,

     akraa_B, akraa_A,
     akraa_9, akraa_8,
     akraa_7, akraa_6,
     akraa_5, akraa_4,
     akraa_3, akraa_2,
     akraa_1, akraa_0
    };	

/* 箱4用 AKIRA */
const Uint8 akirab_ofstbl[] = {
     akrab_0, akrab_0,
     akrab_0, akrab_0,

     akrab_0, akrab_1,
     akrab_2, akrab_3,
     akrab_4, akrab_5,
     akrab_6, akrab_7,
     akrab_8, akrab_9,
     akrab_A, akrab_B,

     akrab_B, akrab_B,
     akrab_B, akrab_B,

     akrab_B, akrab_A,
     akrab_9, akrab_8,
     akrab_7, akrab_6,
     akrab_5, akrab_4,
     akrab_3, akrab_2,
     akrab_1, akrab_0
    };	



/* 箱3用 SONIC MORPHING */
const Uint8 smma_ofstbl[] = {
     smma_0, smma_0,
     smma_0, smma_0,
     smma_0, smma_0,
     smma_0, smma_0,

     smma_0, smma_1,
     smma_2, smma_3,
     smma_4, smma_5,
     smma_6, smma_7,
     smma_8, smma_9,
     smma_A, smma_B,

     smma_B, smma_B,
     smma_B, smma_B,
     smma_B, smma_B,
     smma_B, smma_B,

     smma_B, smma_A,
     smma_9, smma_8,
     smma_7, smma_6,
     smma_5, smma_4,
     smma_3, smma_2,
     smma_1, smma_0	
    };	

/* 箱3用 SONIC MORPHING */
const Uint8 smmb_ofstbl[] = {
     smmb_0, smmb_0,
     smmb_0, smmb_0,
     smmb_0, smmb_0,
     smmb_0, smmb_0,

     smmb_0, smmb_1,
     smmb_2, smmb_3,
     smmb_4, smmb_5,
     smmb_6, smmb_7,
     smmb_8, smmb_9,
     smmb_A, smmb_B,

     smmb_B, smmb_B,
     smmb_B, smmb_B,
     smmb_B, smmb_B,
     smmb_B, smmb_B,

     smmb_B, smmb_A,
     smmb_9, smmb_8,
     smmb_7, smmb_6,
     smmb_5, smmb_4,
     smmb_3, smmb_2,
     smmb_1, smmb_0	
    };	



/* 箱2用 SEGA GAMES */
const Uint8 sgamea_ofstbl[] = {
     sgamea_0, sgamea_1,
     sgamea_2, sgamea_3,
     sgamea_4, sgamea_5,
     sgamea_6, sgamea_7,
     sgamea_8, sgamea_9,
     sgamea_A, sgamea_B,

     sgamea_B, sgamea_B,
     sgamea_B, sgamea_B,
     sgamea_B, sgamea_B,
     sgamea_B, sgamea_B,

     sgamea_B, sgamea_B,
     sgamea_B, sgamea_B,
     sgamea_B, sgamea_B,
     sgamea_B, sgamea_B,

     sgamea_B, sgamea_B,
     sgamea_B, sgamea_B,
     sgamea_B, sgamea_B,
     sgamea_B, sgamea_B,

     sgamea_B, sgamea_B,
     sgamea_B, sgamea_B,
     sgamea_B, sgamea_B,
     sgamea_B, sgamea_B,

    };	

/* 箱2用 SEGA GAMES */
const Uint8 sgameb_ofstbl[] = {
     sgameb_0, sgameb_1,
     sgameb_2, sgameb_3,
     sgameb_4, sgameb_5,
     sgameb_6, sgameb_7,
     sgameb_8, sgameb_9,
     sgameb_A, sgameb_B,

     sgameb_B, sgameb_B,
     sgameb_B, sgameb_B,
     sgameb_B, sgameb_B,
     sgameb_B, sgameb_B,

     sgameb_B, sgameb_B,
     sgameb_B, sgameb_B,
     sgameb_B, sgameb_B,
     sgameb_B, sgameb_B,

     sgameb_B, sgameb_B,
     sgameb_B, sgameb_B,
     sgameb_B, sgameb_B,
     sgameb_B, sgameb_B,

     sgameb_B, sgameb_B,
     sgameb_B, sgameb_B,
     sgameb_B, sgameb_B,
     sgameb_B, sgameb_B,

    };	



/* スクロールVRAMアクセスサイクルパターンテーブル */
const Uint16 SMD2_CYC[] = { 0x044f, 0xffff, 
                            0xffff, 0xffff, 
                            0xffff, 0xffff, 
                            0xffff, 0xffff };

/*---------------------------------------------------------------------------*/
/*      2/14 SAMPLE

/*
 *  メインルーチン
 */
void SMD2_MainMode(void)
{
    enum {
        INIT,
        MAIN,
        NEXT
      };

    Uint16 i, j, k;
    Uint32 times;
    MthXyz mangle, vangle;
    MthXyz mpoint, vpoint;
    XyInt reg_xy;
    SclRotscl *rotreg;


    do {  /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
        SMV1_SprCmdStart();                /* VDP1 SPRITE START */

        switch (SMMA_MainLevel) {
            case INIT:
                SMV2_TvOff();
                SMMA_initall();

                SMD2_RotregReq = 0;

                SMV2_TvOff();
                SMV1_SprCmdEnd();
                SMMA_intwait();
                SMMA_intwait();

                SCL_Vdp2Init();
                SCL_SetFrameInterval(0xffff);   /* Intrpt Mode  */


                /* カラーモードの設定 */
                SCL_SetColRamMode(SCL_CRM15_1024);


                SCL_InitVramConfigTb(&SMV2_sclvram);
                SMV2_sclvram.vramModeA = OFF;
                SMV2_sclvram.vramModeB = ON; 
                SMV2_sclvram.vramA0 = SCL_NON;
                SMV2_sclvram.vramA1 = SCL_NON;
                SMV2_sclvram.vramB0 = SCL_RBG0_CHAR;
                SMV2_sclvram.vramB1 = SCL_RBG0_PN;
                SCL_SetVramConfig(&SMV2_sclvram);

                SCL_Open(SCL_NBG0);
                SCL_InitConfigTb(&SMV2_nscrlreg0);
                SMV2_nscrlreg0.dispenbl = ON;
                SMV2_nscrlreg0.charsize = SCL_CHAR_SIZE_1X1;
                SMV2_nscrlreg0.pnamesize = SCL_PN1WORD;
                SMV2_nscrlreg0.platesize = SCL_PL_SIZE_2X2;
                SMV2_nscrlreg0.bmpsize = SCL_BMP_SIZE_512X256;
                SMV2_nscrlreg0.coltype = SCL_COL_TYPE_256;
                SMV2_nscrlreg0.datatype = SCL_CELL;
                SMV2_nscrlreg0.flip = SCL_PN_12BIT;
                SMV2_nscrlreg0.mapover = SCL_OVER_0;
                SMV2_nscrlreg0.plate_addr[0] = SMD2_NBG0_PTNAMA;
                SMV2_nscrlreg0.plate_addr[1] = SMD2_NBG0_PTNAMB;
                SMV2_nscrlreg0.plate_addr[2] = SMD2_NBG0_PTNAMC;
                SMV2_nscrlreg0.plate_addr[3] = SMD2_NBG0_PTNAMD;
                SCL_SetConfig(SCL_NBG0, &SMV2_nscrlreg0);
                SCL_SetCycleTable(SMD2_CYC);
                SCL_Close();

                SCL_Open(SCL_NBG1);
                SCL_InitConfigTb(&SMV2_nscrlreg1);
                SCL_SetConfig(SCL_NBG1, &SMV2_nscrlreg1);
                SCL_Close();

                Scl_n_reg.linecolmode = 0x00000000;
                Scl_n_reg.backcolmode = 0x00000000 |
                                         ((SMD2_BCLRTBLADD & 0xfffff) / 2);

                SCL_Open(SCL_RBG0);
                SCL_InitConfigTb(&SMV2_rscrlreg);
                SMV2_rscrlreg.dispenbl = ON;        
                SMV2_rscrlreg.charsize = SCL_CHAR_SIZE_1X1; 
                SMV2_rscrlreg.pnamesize = SCL_PN2WORD;      
                SMV2_rscrlreg.platesize = SCL_PL_SIZE_1X1;  
                SMV2_rscrlreg.bmpsize = SCL_BMP_SIZE_512X256; 
                SMV2_rscrlreg.coltype = SCL_COL_TYPE_256;     
                SMV2_rscrlreg.datatype = SCL_CELL;    
                SMV2_nscrlreg0.flip = SCL_PN_12BIT;
                SMV2_rscrlreg.mapover = SCL_OVER_0;   
                for (i = 0; i < 16; i++) {
                     SMV2_rscrlreg.plate_addr[i] = SMD2_RBG0_PTNAM;
                }
                SCL_SetConfig(SCL_RBG0, &SMV2_rscrlreg);
                SCL_InitRotateTable(SMD2_RBG0_RPAR, 1, SCL_RBG0, SCL_NON);
                Scl_r_reg.k_contrl = 0x1;
                SCL_Close();

                /* プライオリティの設定 */
                SCL_SetPriority(SCL_SP0, 7);
                SCL_SetPriority(SCL_NBG0, 5);
                SCL_SetPriority(SCL_NBG1, 0);
                SCL_SetPriority(SCL_RBG0, 6);

                SMV2_TvOff();
                SMV1_SprCmdEnd();
                SMMA_intwait();
                SMMA_intwait();

                SCL_Open(SCL_RBG0);
                SCL_SetConfig(SCL_RBG0, &SMV2_rscrlreg);
                SCL_InitRotateTable(SMD2_RBG0_RPAR, 1, SCL_RBG0, SCL_NON);
                Scl_r_reg.k_contrl = 0x11;
                SCL_Close();

                /* Int Hook1 */
                SMIN_VintHook1  = &SMD2_VintRotRegSet;

                /* CGデータの展開 */
                SMD2_cgset(); 

                /* アクションのセット */
                SMTA_MakeAction(SMD2_DemoMain);
                SMV2_TvOn();               
                SMMA_MainLevel++;

            case MAIN:
                SCL_Open(SCL_RBG0);
                SCL_Close();
                SCL_Open(SCL_NBG0);
                SCL_Close();
                SMD2_grno = 0;      /* グーローNo初期化 */
                SMTA_ActionLoop();  /* アクション実行   */
                SMD2_RotregReq = 1; /* 回転スクロールパラメータ転送要求 */
                break;

            case NEXT:
                SMMA_MainMode = SELECT_MODE;
                SMMA_MainLevel = 0;
                break;
	}
 
        SMV1_SprCmdEnd();
        SMMA_intwait();        /* V-BLANK WAIT */

    } while (SMMA_MainMode == DEMO214_MODE);

}


 
/*
 *    デモメインコントロールアクション
 */
void SMD2_DemoMain(SMD2ACT *ix)
{
    enum {
          INIT,
          MAIN,
          NEXT
          };

    Uint16 swdata;
    SMD2ACT *iy;
	
    swdata = SMPA_pad_edge1;

    switch (ix->mode) {
        case INIT:
            /* SMD2_MakeFadeIn(12); */
            ix->subtbl = SMTA_MakeAction(SMD2_XWaveMain);
            ix->cnt0 = 0x10;
            ix->mode++;
            break;
        case MAIN:
            if (swdata & PAD_X) ix->cnt0++;
            if (swdata & PAD_Y) ix->cnt0--;
            ix->cnt0 &= 0x1f;
            SCL_SET_R0CCRT(ix->cnt0);

            
            /* if (fadeflag) break; */
            iy = ix->subtbl;
            if ((swdata & PAD_START) || (iy->reg3 == -1)) {
                /* SMD2_MakeFadeOut(12); */
                ix->mode++;
            }
            break;
        case NEXT:
            /* if (fadeflag) break; */
            SMMA_MainLevel++;
	}
}



/*
 *    CGSET
 */
void SMD2_cgset(void)
{
     Uint16 i, j, k;
     Sint16 direc;
     Uint8 *wkptr;
     Uint16 *dstptrb, *dstptr, *page;
     Uint16 wrdata;
     Uint32 *dst32;
     Uint32 dt;

    /*----- NBG0 SETUP -----*/
    SMMI_Ldir8((Uint8 *)SUNSET_info.cgadd, (Uint8 *)SMD2_NBG_PTGEN+0x40,
                                            (Uint32)SUNSET_info.cgsize); 
    SMMI_Ldir16((Uint16 *)SUNSET_info.clradd, (Uint16 *)(COLOR_RAM + 0x000),
                                            (Uint32)SUNSET_info.clrcnt); 

    SMV2_FilMem16((Uint16 *)SMD2_NBG0_PTNAMA, 64*64, 0x0002);	
    SMV2_FilMem16((Uint16 *)SMD2_NBG0_PTNAMB, 64*64, 0x0002);	
    SMV2_FilMem16((Uint16 *)SMD2_NBG0_PTNAMC, 64*64, 0x0002);	
    SMV2_FilMem16((Uint16 *)SMD2_NBG0_PTNAMD, 64*64, 0x0002);	


    dstptr = (Uint16 *)(SMD2_NBG0_PTNAMC + 0x80*32);
    SMD2_BoxWrite16(dstptr , a_1_map, 32, 32);
    dstptr += 0x20;
    SMD2_BoxWrite16(dstptr , a_2_map, 32, 32);

    dstptr = (Uint16 *)(SMD2_NBG0_PTNAMD + 0x80*32);
    SMD2_BoxWrite16(dstptr , a_1_map, 32, 32);
    dstptr += 0x20;
    SMD2_BoxWrite16(dstptr , a_2_map, 32, 32);


    /*----- SCROLL A SETUP -----*/
    SMMI_Ldir8((Uint8 *)DE01_info.cgadd, (Uint8 *)(SMD2_RBG0_PTGEN + 0x40),
                                                   (Uint32)DE01_info.cgsize); 
    SMMI_Ldir16((Uint16 *)DE01_info.clradd, (Uint16 *)(COLOR_RAM + 0x200),
                                                    (Uint32)DE01_info.clrcnt); 


    /*---- 雲の配置 ----*/
    SMV2_FilMem32((Uint32 *)SMD2_RBG0_PTNAM, 64*64, 0x0000);
    dst32 = (Uint32 *)SMD2_RBG0_PTNAM;
    SMD2_MapWrite32(dst32, K4_info.mapadd, K4_info.hsize, K4_info.vsize);
    dst32 = (Uint32 *)SMD2_RBG0_PTNAM + 32;
    SMD2_MapWrite32(dst32, K4_info.mapadd, K4_info.hsize, K4_info.vsize);
    dst32 = (Uint32 *)SMD2_RBG0_PTNAM + 64 * 32;
    SMD2_MapWrite32(dst32, K4_info.mapadd, K4_info.hsize, K4_info.vsize);
    dst32 = (Uint32 *)SMD2_RBG0_PTNAM + 64 * 32 + 32;
    SMD2_MapWrite32(dst32, K4_info.mapadd, K4_info.hsize, K4_info.vsize);

    zkyori = ZKYORIMIN;     /* 透視変換距離 */
    D214FLAG = 0;           /* POLYLINE->POLYGON->TEXTURE MODE */

    /*----- SPRITE CG SETUP -----*/
    wkptr = (Uint8 *)YUKA_info.cgadd;
    SPR_2SetChar(yuka_0,COLOR_5,0,0x20,0x20,wkptr);

    wkptr = (Uint8 *)JOIN_info.cgadd;
    for (i = 0; i < 24; i++) {
         j = joina_0 + i;
         SPR_2SetChar(j,COLOR_5,0,0x20,0x40,wkptr);
         wkptr += 0x20*0x40*2;
    }         

    wkptr = (Uint8 *)AKIRA_info.cgadd;
    for (i = 0; i < 24; i++) {
         j = akraa_0 + i;
         SPR_2SetChar(j,COLOR_5,0,0x20,0x40,wkptr);
         wkptr += 0x20*0x40*2;
    }         

    wkptr = (Uint8 *)SMM_info.cgadd;
    for (i = 0; i < 24; i++) {
         j = smma_0 + i;
         SPR_2SetChar(j,COLOR_5,0,0x20,0x40,wkptr);
         wkptr += 0x20*0x40*2;
    }         

    wkptr = (Uint8 *)SGAME_info.cgadd;
    for (i = 0; i < 24; i++) {
         j = sgamea_0 + i;
         SPR_2SetChar(j,COLOR_5,0,0x20,0x40,wkptr);
         wkptr += 0x20*0x40*2;
    }         


    /* ---- ラインカラーの設定 ---- */
    dstptr = (Uint16 *)(COLOR_RAM + 0x400);
    for (dt = 0; dt < 0x20; dt++) {
         *dstptr = (dt << 10) + (dt << 5) + dt; /* 白のグラデテーブル作成 */
         dstptr++;
    }

    dstptr = (Uint16 *)(SMD2_LCLRTBLADD);
    for (i = 0; i < 0x100; i++) {
         direc = (i * 0x40) / 0x100;
         *dstptr = (0x400/2); /*  + (0x10 * SMMI_Cosset8(direc)) / 0x100; */
         dstptr++;
    }

    SCL_Open(SCL_NBG0);
    Scl_n_reg.linecolmode = 0x80000000 | ((SMD2_LCLRTBLADD & 0xfffff) / 2);
    SCL_Close();

    /* カラー演算モードの設定 */
    SCL_SET_CCMD(1);    /* カラー演算そのまま加算イネーブル */
    SCL_SET_CCRTMD(1);  /* 割合を2nd側で指定             */
    SCL_SET_R0CCRT(0x8);
    SCL_SET_LCCCRT(0x1);
    SCL_SET_N0CCEN(0);  /* NBG0カラー演算イネーブル         */
    SCL_SET_R0CCEN(1);  /* 回転面カラー演算イネーブル       */
    SCL_SET_SPCCEN(0);  /* スプライトカラー演算イネーブル   */
    SCL_SET_LCCCEN(1);  /* ラインカラーイネーブル           */
    SCL_SET_R0LCEN(1);  /* ラインカラー                     */
    SCL_SET_SPLCEN(0);  /* ラインカラー                     */
    SCL_SET_EXCCEN(0);  /* 拡張カラー演算                   */




}

/*======================================================
 *  D214専用SCROLL BOX WRITE(64セル境界非対応 WORD)
 *=====================================================*/
void SMD2_BoxWrite16(Uint16 *wradd, Uint16 *mapdata, 
                                              Uint16 hsize, Uint16 vsize)
{
    Uint16 h, v;
    Uint16 *dst0, *dst1;
    Uint16 rdata0, rdata1;
    
    dst0 = wradd;
    for (v = 0; v < vsize; v++) {
        dst1 = dst0;
        for (h = 0; h < hsize; h++) {
             rdata0 = (*mapdata) & 0x0fff;
             rdata1 = (*mapdata) & 0xf000;
             rdata0 = rdata1 + (rdata0 * 2);
 	     *dst1 = rdata0;
             dst1++;
             mapdata++;
        }
	dst0 += 0x40;           /* NEXT LINE OFFSET */
    }
}

/*======================================================
 *  D214専用回転SCROLL BOX WRITE(64セル境界非対応 WORD)
 *=====================================================*/
void SMD2_MapWrite32(Uint32 *wradd, Uint16 *mapdata, 
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
             rdata0 += (0x100000 + 0x2000);
 	     *dst1 = rdata0;
             dst1++;
        }
	dst0 += 0x40;           /* NEXT LINE OFFSET */
    }
}





/* ========================================================
 *  XWAVE MAIN
 * ======================================================*/
void SMD2_XWaveMain(SMD2ACT *ix)
{
    enum {
          INIT,
          MAIN,
          NEXT
    };

    ROTPARAM *aregptr;
    SclNorscl *bregptr;
    Uint16 swdata, swedge;
    SMD2ACT *iy;
    Uint16 *swptr;
    Sint32 nbg0_dytmp;

    zbufcnt = 0;
    zbuffer[0].next = NULL;                    /* ZBUFFER INIT          */

    aregptr = &SMD2_rotregbuf;                  /* SCROLL_A POINTER      */
    bregptr = &Scl_n_reg;                      /* SCROLL_B POINTER      */

    switch (ix->mode) {
        case INIT:
            ix->subtbl = SMTA_MakeAction(SMD2_XWaveObj);
            ix->level = 0;                          /* 半透明フラグ          */
            ix->reg0 = 0;                           /* 初期角度              */
            ix->reg2 = NO;                          /* AUTO->MANUAL FLAG     */
            ix->reg3 = 0;                           /* モード終了フラグ      */
            ix->cnt0 = 0;                           /* スイッチデータセーブ  */
            ix->cnt1 = DEMOSW_info.cgsize;          /* スイッチデータ数      */
            ix->cnt2 = 0;                           /*                       */
            ix->cnt3 = 0;                           /*                       */
            ix->sprite.patchgtim = 0;
            ix->msttbl = (void *)DEMOSW_info.cgadd;

            aregptr->xst = 0x0000;
            aregptr->yst = 0x0000;
            aregptr->zst = 0;
            aregptr->dxst = 0;
            aregptr->dyst = 0x10000;
            aregptr->dx = 0x10000;
            aregptr->dy = 0;
            aregptr->kx = 0x10000;
            aregptr->ky = 0x10000;
            aregptr->mx = 0;               /* A初期xスクロール量    */
            aregptr->my = 0x7200000;       /* A初期yスクロール量    */
            bregptr->n0_move_x = NBG0_INI_HSCRL;  /* B初期xスクロール量    */
            bregptr->n0_move_y = NBG0_INI_VSCRL;  /* B初期yスクロール量    */
            ix->mode++;
			
        case MAIN:

#if AUTOD /*================================================================*/ 
            if (SMPA_pad_data1 & (PAD_UP | PAD_DOWN | PAD_LEFT | PAD_RIGHT))
                ix->reg2 = YES;

                if (ix->reg2 == NO) {               /* AUTOモードなら       */
                    swptr = (Uint16 *)ix->msttbl;
                    swdata = *swptr;
                    ix->sprite.patchgtim++;
                    if (ix->sprite.patchgtim > 3) {
                        ix->cnt1--;
                        swptr++;
                        ix->sprite.patchgtim = 0;
                    }
                    ix->msttbl = (void *)swptr;
                      
                    if (ix->cnt1 == 0) {
                        ix->cnt1 = DEMOSW_info.cgsize;
                        ix->msttbl = (void *)DEMOSW_info.cgadd;
                    }
                } else {
                    swdata = SMPA_pad_data1;      /* MANUALモードなら      */
                }
			
            swedge = SMPA_pad_edge1;
#else  /*===================================================================*/
            swdata = SMPA_pad_data1;             /* SWサンプリングモード   */
            swedge = SMPA_pad_edge1;
            if (ix->cnt0 < 0x800)                 /* デモスイッチデータ保存 */
                demoswdata[ix->cnt0++] = swdata;
#endif /*===================================================================*/

            iy = ix->subtbl;                      /* 床代表ワークアドレス  */
			
            if (swdata & XDIRUP) {                /* xdirecと背景縦の操    */
                bregptr->n0_move_y -= NBG0_VSPD;
                ix->reg0 += (0x10000);
                if (ix->reg0 > 0x800000) {
                    ix->reg0 = 0x800000;
                    bregptr->n0_move_y += NBG0_VSPD; /* 背景縦スクロール */
                }
            }
            if (swdata & XDIRDOWN) {
                bregptr->n0_move_y += NBG0_VSPD;
                ix->reg0 -= (0x10000);
                if (ix->reg0 < 0) {
                    ix->reg0 = 0;
                    bregptr->n0_move_y -= NBG0_VSPD; /* 背景縦スクロール */
                }
            }


            iy->sprite.xdirec = *(Sint16 *)&ix->reg0;
			
            if (swdata & ZDIRUP) {               /* zdirec操作              */
                iy->sprite.zdirec += 0x1;
                bregptr->n0_move_x += NBG0_HSPD; /* 背景横スクロール */
            }
            if (swdata & ZDIRDOWN) {             /* zdirec操作              */
                iy->sprite.zdirec -= 0x1;
                bregptr->n0_move_x -= NBG0_HSPD; /* 背景横スクロール */
            }
				
			
            aregptr->mx -= ASCRLSPD;            /* スクロールスピード      */
						
	
            if (swdata & ZKINC) {               /* 背景ズーム操作          */
                zkyori += (0x10/4);
            }
            if (swdata & ZKDEC) {
                zkyori -= (0x10/4);
            }
            if (zkyori > ZKYORIMAX)
                zkyori = ZKYORIMAX;
            if (zkyori < ZKYORIMIN)
                zkyori = ZKYORIMIN;
 
            /* ==== 回転スクロールコントロール ==== */
            XZRotScrlCnt(-iy->sprite.xdirec, iy->sprite.zdirec);


			
            if (swedge & MDCHG) {         /* TEXTURE->POLYGON->POLYLINE */
                D214FLAG++;
                if (D214FLAG > 2)
                    D214FLAG = 0;
            }
            break;
     }

}
	

/*
 *    2/14 WAVE OBJECT MASTER
 */
void SMD2_XWaveObj(SMD2ACT *ix)
{
    enum {
         INIT,
         MAIN,
         NEXT
         };

    Uint8 i, j;
    Sint32 d0, d1, d2, d3;
    Sint32 sinx, cosx;
    SMD2ACT *iym, *iys, *iyj, *iymap;


    switch (ix->mode) {
        case INIT:
            ix->sprite.linktbl = &yuka_ofstbl[0];  /*  PatterNo Table ADD */
            ix->sprite.patno = 0;                  /*  パターンNo         */
            ix->sprite.pmode = 0x08ac;           /*  PUT MODE           */
            ix->sprite.sprhsize = 0x20;      /*  元絵サイズ h 0x20dot      */
            ix->sprite.sprvsize = 0x20;      /*            v 0x20 dot      */

            ix->sprite.color = rgb15(0x1c, 0x10, 0x14);
		
            ix->sprite.xposi = 0xffb00000;
            ix->sprite.yposi = 0xffb00000;
            ix->sprite.zposi = 0x00000000;
            ix->sprite.xdirec = 0x0000;
            ix->sprite.zdirec = 0x0000;
			
            ix->cnt0 = 0;
            ix->reg0 = 0;
            ix->reg1 = 0;


            /* じゅうたんのアクションセット */	
            for (j = 0; j < 8; j++) {
                for (i = 0; i < 8; i++)
                    Yukawkmap[i][j] = NULL;
            }

            iym = ix;
            iyj = ix;
            for (j = 0; j < 6; j++) {
                for (i = 1; i < 6; i++) {
                    iys = SMTA_MakeAction(SMD2_XWaveObjS);
                    iys->msttbl = iym;
                    iys->subtbl = NULL;
                    iys->reg0 = i;
                    iys->reg1 = j;
                    iym = iys;
                    Yukawkmap[i][j] = iys;
                }
                if (j != 5) {
                    iys = SMTA_MakeAction(SMD2_XWaveObjS);
                    iys->msttbl = NULL;
                    iys->subtbl = iyj;
                    iys->reg0 = 0;
                    iys->reg1 = j + 1;
                    iyj = iys;
                    iym = iys;
                    Yukawkmap[0][j] = iys;
                }
            }
			

#if ONPLAYER	/* ========================================================*/

/*------------------　床の上に乗る板のタスクセット ---------------------- */

            iym = SMTA_MakeAction(SMD2_XWavePlay);
            iym->msttbl = Yukawkmap[2][2];   /* 0-A */
            iym->subtbl = Yukawkmap[2][3];
            iym->sprite.linktbl = &joina_ofstbl[0]; /*  PatternNoTableAdd  */
            iym->sprite.patchgcnt = JOIN_MAXPAT;     /*  MaxPatternNo       */

            iym = SMTA_MakeAction(SMD2_XWavePlay);
            iym->msttbl = Yukawkmap[2][3];   /* 0-B */
            iym->subtbl = Yukawkmap[2][4];
            iym->sprite.linktbl = &joinb_ofstbl[0]; 
            iym->sprite.patchgcnt = JOIN_MAXPAT;

			
            iym = SMTA_MakeAction(SMD2_XWavePlay);
            iym->msttbl = Yukawkmap[4][2];   /* 1-A */
            iym->subtbl = Yukawkmap[4][3];
            iym->sprite.linktbl = &smma_ofstbl[0];
            iym->sprite.patchgcnt = SMM_MAXPAT;

            iym = SMTA_MakeAction(SMD2_XWavePlay);
            iym->msttbl = Yukawkmap[4][3];   /* 1-B */
            iym->subtbl = Yukawkmap[4][4];
            iym->sprite.linktbl = &smmb_ofstbl[0];
            iym->sprite.patchgcnt = SMM_MAXPAT; 

			
            iym = SMTA_MakeAction(SMD2_XWavePlay);
            iym->msttbl = Yukawkmap[2][2];   /* 2-A */
            iym->subtbl = Yukawkmap[3][2];
            iym->sprite.linktbl = &akiraa_ofstbl[0];
            iym->sprite.patchgcnt = AKIRA_MAXPAT;

            iym = SMTA_MakeAction(SMD2_XWavePlay);
            iym->msttbl = Yukawkmap[3][2];   /* 2-B */
            iym->subtbl = Yukawkmap[4][2];
            iym->sprite.linktbl = &akirab_ofstbl[0];
            iym->sprite.patchgcnt = AKIRA_MAXPAT;

			
            iym = SMTA_MakeAction(SMD2_XWavePlay);
            iym->msttbl = Yukawkmap[2][4];   /* 3-A */
            iym->subtbl = Yukawkmap[3][4];
            iym->sprite.linktbl = &sgamea_ofstbl[0];
            iym->sprite.patchgcnt = SGAME_MAXPAT;

            iym = SMTA_MakeAction(SMD2_XWavePlay);
            iym->msttbl = Yukawkmap[3][4];   /* 3-B */
            iym->subtbl = Yukawkmap[4][4];
            iym->sprite.linktbl = &sgameb_ofstbl[0];
            iym->sprite.patchgcnt = SGAME_MAXPAT;
			
#endif /* endif ONPLAYER ================================================= */


            /* じゅうたんの影アクションセット */	
            for (j = 0; j < 8; j++) {
                for (i = 0; i < 8; i++) {
                    iym = SMTA_MakeAction(SMD2_XWaveKage);
                    iym->msttbl = Yukawkmap[i][j];
                }
            }

            ix->mode++;

        case MAIN:
			
            ix->cnt0 += YUKADIRSPD;                  /* yukaのゆれスピード */
            XWaveZposiSet(ix);                       /* ZPOSI　揺れ計算    */
		
            ix->sprite.posi[0][X] = *(Sint16 *)&ix->sprite.xposi + (-0x10);
            ix->sprite.posi[0][Y] = *(Sint16 *)&ix->sprite.yposi + (-0x10);
			
            ix->sprite.posi[1][X] = *(Sint16 *)&ix->sprite.xposi + ( 0x10);
            ix->sprite.posi[1][Y] = *(Sint16 *)&ix->sprite.yposi + (-0x10);
			
            ix->sprite.posi[2][X] = *(Sint16 *)&ix->sprite.xposi + ( 0x10);
            ix->sprite.posi[2][Y] = *(Sint16 *)&ix->sprite.yposi + ( 0x10);
			
            ix->sprite.posi[3][X] = *(Sint16 *)&ix->sprite.xposi + (-0x10);
            ix->sprite.posi[3][Y] = *(Sint16 *)&ix->sprite.yposi + ( 0x10);
			
            break;
    }
	
    XWaveObj_zbufput(&ix->sprite);

}




/*
 *    2/14 WAVE SPRITE SLAVE
 */
void SMD2_XWaveObjS(SMD2ACT *ix)
{
    enum {
          INIT,
          MAIN,
          NEXT
	};

    Sint16 xofs, yofs;
    Sint32 d0, d1, d2, d3;
    SMD2ACT *iy;
	
    switch (ix->mode) {
        case INIT:
            ix->sprite.linktbl = &yuka_ofstbl[0];/*  PatternNoTableAdd */
            ix->sprite.patno = 0;                /*  パターンNo        */
            ix->sprite.pmode = 0x08ac;         /*  PUT MODE          */
            ix->sprite.sprhsize = 0x20;       /*  元絵サイズ        */
            ix->sprite.sprvsize = 0x20;       /*  元絵サイズ        */
			
            if ((ix->reg0 + ix->reg1) & 1)
                ix->sprite.color = rgb15(0x0e, 0x08, 0x0e);
            else 
                ix->sprite.color = rgb15(0x1c, 0x10, 0x14);
			
            ix->mode++;

        case MAIN:
			
            if (ix->msttbl != NULL) {
                iy = ix->msttbl;
                ix->sprite.posi[0][X] = iy->sprite.posi[1][X];
                ix->sprite.posi[0][Y] = iy->sprite.posi[1][Y];
                ix->cnt0 = iy->cnt1;
            } else {
                iy = ix->subtbl;
                ix->sprite.posi[0][X] = iy->sprite.posi[3][X];
                ix->sprite.posi[0][Y] = iy->sprite.posi[3][Y];
                ix->cnt0 = iy->cnt1;
            }
			
            ix->sprite.xdirec = iy->sprite.xdirec;         /* XDIREC   */
            ix->sprite.zdirec = iy->sprite.zdirec;         /* YDIREC   */
			
            ix->sprite.posi[1][X] = ix->sprite.posi[0][X] + 0x20;
            ix->sprite.posi[1][Y] = ix->sprite.posi[0][Y];
            ix->sprite.posi[2][X] = ix->sprite.posi[0][X] + 0x20;
            ix->sprite.posi[2][Y] = ix->sprite.posi[0][Y] + 0x20;
            ix->sprite.posi[3][X] = ix->sprite.posi[0][X];
            ix->sprite.posi[3][Y] = ix->sprite.posi[0][Y] + 0x20;
			
            XWaveZposiSet(ix);          /* ZPOSI　揺れ計算 */
            break;
    }
	
    XWaveObj_zbufput(&ix->sprite);
	

}


/* ========================================
 *      床のZPOSI(ゆれ)計算
 + ======================================*/
void XWaveZposiSet(SMD2ACT *ix)
{
    Sint32 d0, d1, d2, d3;
	
    ix->cnt1 = ix->cnt0 - DZDIR;
    ix->cnt2 = ix->cnt1 - DZDIR;
    ix->cnt3 = ix->cnt2 - DZDIR;
			
    d0 = ZYURE * SMMI_Cosset(ix->cnt0) * 4;
    d1 = ZYURE * SMMI_Cosset(ix->cnt1) * 4;
    d2 = ZYURE * SMMI_Cosset(ix->cnt2) * 4;
    d3 = ZYURE * SMMI_Cosset(ix->cnt3) * 4;

    ix->sprite.posi[0][Z] = *(Sint16 *)&d0;
    ix->sprite.posi[1][Z] = *(Sint16 *)&d1;
    ix->sprite.posi[2][Z] = *(Sint16 *)&d2;
    ix->sprite.posi[3][Z] = *(Sint16 *)&d1;
}



/*
 *    2/14 Player 箱の側面
 */
void SMD2_XWavePlay(SMD2ACT *ix)
{
    enum {
          INIT,
          MAIN,
          NEXT
    };

    Sint8 patno;
    Sint32 d0, d1, d2, d3;
    SMD2ACT *iy;
	

    switch (ix->mode) {
        case INIT:
            ix->sprite.patno = 0;                    /*  パターンNo    */
            ix->sprite.pmode = 0x00a8;             /*  PUT MODE       */
            ix->sprite.sprhsize = 0x20; /*  元絵サイズ     */
            ix->sprite.sprvsize = 0x40; /*  元絵サイズ     */
            ix->sprite.color = rgb15(0x08, 0x18, 0x06);
            ix->cnt0 = 0;   /* pattern change counter */
            ix->mode++;
          
        case MAIN:
            iy = ix->msttbl;
            ix->sprite.posi[3][X] = iy->sprite.posi[0][X];
            ix->sprite.posi[3][Y] = iy->sprite.posi[0][Y];
            ix->sprite.posi[3][Z] = iy->sprite.posi[0][Z] - 0x08;
			
            iy = ix->subtbl;
            ix->sprite.posi[2][X] = iy->sprite.posi[0][X];
            ix->sprite.posi[2][Y] = iy->sprite.posi[0][Y];
            ix->sprite.posi[2][Z] = iy->sprite.posi[0][Z] - 0x08;
			
            ix->sprite.posi[0][X] = ix->sprite.posi[3][X];
            ix->sprite.posi[0][Y] = ix->sprite.posi[3][Y];
            ix->sprite.posi[0][Z] = ix->sprite.posi[3][Z] - 0x48;
			
            ix->sprite.posi[1][X] = ix->sprite.posi[2][X];
            ix->sprite.posi[1][Y] = ix->sprite.posi[2][Y];
            ix->sprite.posi[1][Z] = ix->sprite.posi[2][Z] - 0x48;
			
            ix->sprite.xdirec = iy->sprite.xdirec; /* XDIREC   */
            ix->sprite.zdirec = iy->sprite.zdirec; /* YDIREC   */

            ix->cnt0++;
            if (ix->cnt0 > 3) {     /* PatChgTiming */
                ix->sprite.patno++;
                ix->cnt0 = 0;
            }
            if (ix->sprite.patno > ix->sprite.patchgcnt)
                ix->sprite.patno = 0;
            break;
    }
	
    XWaveObj_zbufput(&ix->sprite);
	

}






/*
 *    ゆかの影
 */
void SMD2_XWaveKage(SMD2ACT *ix)
{
    enum {
         INIT,
         MAIN,
         NEXT
         };

    Uint8 i, j;
    Sint32 d0, d1, d2, d3;
    Sint32 sinx, cosx;
    SMD2ACT *iym, *iys, *iyj, *iymap;


    switch (ix->mode) {
        case INIT:
            ix->sprite.linktbl = &yuka_ofstbl[0];  /*  PatterNo Table ADD */
            ix->sprite.patno = 0;                  /*  パターンNo         */
            ix->sprite.pmode = 0x08ad;           /*  SHADOW PUT MODE      */
            ix->sprite.sprhsize = 0x20;      /*  元絵サイズ h 0x20dot      */
            ix->sprite.sprvsize = 0x20;      /*            v 0x20 dot      */

            ix->sprite.color = 0;
	
            ix->sprite.zposi = 0x00000000;
	
            ix->mode++;

        case MAIN:
            iym = ix->msttbl;           			
            for (i = 0; i < 4; i++) {
                ix->sprite.posi[i][X] = iym->sprite.posi[i][X];
                ix->sprite.posi[i][Y] = iym->sprite.posi[i][Y];
                ix->sprite.posi[i][Z] = iym->sprite.posi[i][Z];
                ix->sprite.xdirec = iym->sprite.xdirec;
                ix->sprite.zdirec = iym->sprite.zdirec;
            }
    }
	
/*    XWaveObj_zbufput(&ix->sprite); */

}





/*----------------------------------------------
 *    4点指定 POLYGON(3D OBJECT ZBUFFER PUT)
 *---------------------------------------------*/
void XWaveObj_zbufput(SPRITE *sprite)
{
    Sint32 zposi;
    Uint16 hvrev, pmode, color, cgadr, cgsize, growno;
    XyInt posi[4];
    Uint8 *pattbl;
    Sint16 i, grtmp;
    SprGourTbl grwk;
    XROT_INFO posibuf[4];
	

    for (i = 0; i < 4; i++) {
        posibuf[i].xposi = sprite->posi[i][X];          /* POSI ABCD */
        posibuf[i].yposi = sprite->posi[i][Y];
        posibuf[i].zposi = sprite->posi[i][Z];
        posibuf[i].xdirec = sprite->xdirec;
        posibuf[i].zdirec = sprite->zdirec;
        posibuf[i].grwk = Xrot(&posibuf[i]);
    }

    zposi = (posibuf[0].zposi + posibuf[1].zposi) >> 1; /* 代表点は平均     */
    zposi = -zposi;
    zposi += 0x100;
    
    color = sprite->color;                              /* カラーデータ     */
	
    switch (D214FLAG) {
        case 0:
            pmode = sprite->pmode;                      /* PUTMODE        */
            break;
        case 1:
            pmode = 0x08c4;
            break;
        case 2:
            pmode = 0x00c0;
            break;
    }


    /* ポジションとグーロー値セット */
    for (i = 0; i < 4; i++) {	
         grtmp = posibuf[i].grwk;
         grwk.entry[i] = (grtmp << 10) + (grtmp << 5) + (grtmp);
         posi[i].x = (Sint16)posibuf[i].xposi;	
         posi[i].y = (Sint16)posibuf[i].yposi;
    }

    pattbl = sprite->linktbl;     /* パターンテーブルアドレス   */  
    pattbl += sprite->patno;      /* パターンNo +で元絵アドレス */
    cgadr = (Uint16)*pattbl;

    SPR_2SetGourTbl(SMD2_grno, &grwk);

    switch (D214FLAG) {
        case 0:
            SPR_2DistSpr(zposi,0, pmode, color, cgadr, &posi[0], SMD2_grno);
            break;
        case 1:
            SPR_2Polygon(zposi, pmode, color, &posi[0], SMD2_grno);
            break;
        case 2:
            SPR_2PolyLine(zposi, pmode, color, &posi[0], SMD2_grno);
            break;
    }
    SMD2_grno++;

}



    

/*
 *    PITCHING-BANK (X-Z ROTATE)
 *    Return: GROW COLOR RATE
 */
Sint16 Xrot(XROT_INFO *ptr)
{
    Sint32 sin, cos;
    Sint32 r0, r1, r2, r3, grow;

    grow = ptr->zposi;
    grow = -(grow - ZYURE);          /* 揺れ幅の半分    */
    grow = (grow * GROWLEVEL) / (ZYURE * 2);/* (zposi*頂上での明るさ)/揺れ幅 */
	
    sin = SMMI_Sinset8(ptr->zdirec);
    cos = SMMI_Cosset8(ptr->zdirec);
	
    r0 = ptr->xposi * cos;
    r1 = ptr->yposi * sin;
    r2 = ptr->xposi * sin;
    r3 = ptr->yposi * cos;
	
    ptr->xposi = (r0 + r1) >> 8;
    ptr->yposi = (r3 - r2) >> 8;
	
    sin = SMMI_Sinset8(ptr->xdirec);
    cos = SMMI_Cosset8(ptr->xdirec);
	
    r0 = ptr->yposi * cos;
    r1 = ptr->zposi * sin;
    r2 = ptr->yposi * sin;
    r3 = ptr->zposi * cos;
	
    ptr->yposi = (r0 + r1) >> 8;
    ptr->zposi = (r3 - r2) >> 8;

    ptr->xposi = (ptr->xposi * 0x100) / (ptr->zposi + zkyori) + 0xb0; 
    ptr->yposi = (ptr->yposi * 0x100) / (ptr->zposi + zkyori) + 0x70;

    return (Sint16)grow;
	
	
}






/*
 *  X-Z ROTATE SCROLL CONTROL
 */
void XZRotScrlCnt(Sint16 xdirec, Sint16 zdirec)
{
    ROTPARAM *rregptr;
    Sint32 *keisuptr;
    Sint32 sx, sy, mz, dt0, dt1, dt2, cosx, sinx, cosz, sinz;
    Sint32 mtr_g, mtr_h, mtr_i;

    rregptr = &SMD2_rotregbuf;
	
    cosx = (Sint32)(SMMI_Cosset8(xdirec));
    sinx = (Sint32)(SMMI_Sinset8(xdirec));
    cosz = (Sint32)(SMMI_Cosset8(zdirec));
    sinz = (Sint32)(SMMI_Sinset8(zdirec));

    rregptr->mtr_a = cosz << 5;
    rregptr->mtr_b = -(SMMI_Sinset8(zdirec + xdirec) +
                                       SMMI_Sinset8(zdirec - xdirec)) << 4;
    rregptr->mtr_c =  (SMMI_Cosset8(zdirec - xdirec) - 
                                       SMMI_Cosset8(zdirec + xdirec)) << 4;
    rregptr->mtr_d = sinz << 5;
    rregptr->mtr_e =  (SMMI_Cosset8(zdirec + xdirec) + 
                                       SMMI_Cosset8(zdirec - xdirec)) << 4;
    rregptr->mtr_f = -(SMMI_Sinset8(zdirec + xdirec) - 
                                       SMMI_Sinset8(zdirec - xdirec)) << 4;
    mtr_g = 0;
    mtr_h = sinx;
    mtr_i = cosx;

    rregptr->px = 0xb0;
    rregptr->py = 0x70;
    rregptr->pz = 0x100;
	
    rregptr->cx = 0xb0;
    rregptr->cy = 0x70;
    rregptr->cz = 0;
	
    rregptr->kast = ((SMD2_RBG0_KEISU & 0x7ffff) / 4) * 0x10000;
    rregptr->dkasy = 0x00010000;
    rregptr->dkasx = 0x00000000;

    mz = zkyori;
    dt2 = (mz << 12) + 0x40000;      /* 地平線消す目安 0x60000-0x140000 */


    keisuptr = &SMD2_KeisuBuf[0];
    dt0  = (mtr_g * (rregptr->cx - rregptr->px) >> 8);
    dt0 += (mtr_h * (rregptr->cy - rregptr->py) >> 8);
    dt0 += (mtr_i * (rregptr->cz - rregptr->pz) >> 8) - rregptr->cz - mz;
     


    sx = 0;
    for (sy = 0; sy < 0x100 ; sy++) {

        dt1  = (mtr_i * ( 0 - rregptr->pz) >> 8);                /* 0は sz */
        dt1 += (mtr_g * (sx - rregptr->px) >> 8);
        dt1 += (mtr_h * (sy - rregptr->py) >> 8);
        dt1  = ((dt0 * 0x10000) / dt1);
			 
        if (dt1 <= 0) {
            dt1 = 0x80000000;
        } else {
            if (dt1 > dt2)
                dt1 = 0x80000000;
            else {
                dt1 &= 0x00ffffff;
                dt1 = dt1 + ((dt1 << 6) & 0xff000000);
            
            }
        }							
        *keisuptr++ = dt1;
    }



}





/*
 *   Vintrpt Trans
 */ 
void SMD2_VintRotRegSet(void)
{
    if (SMD2_RotregReq) {
#if 1
        SMMI_CPUDmaLdirW0((Uint32)&SMD2_rotregbuf,
                                 (Uint32)SMD2_RBG0_RPAR,0x60);
        SMMI_CPUDmaLdirW0((Uint32)&SMD2_KeisuBuf[0],
                                 (Uint32)SMD2_RBG0_KEISU,0x400);

#else
        SMMI_Ldir16((Uint16 *)&SMD2_rotregbuf, 
                            (Uint16 *)SMD2_RBG0_RPAR, (0x60/2));
        SMMI_Ldir32((Uint32 *)&SMD2_KeisuBuf[0], 
                            (Uint32 *)SMD2_RBG0_KEISU, 0x100);

#endif

        SMD2_RotregReq = 0;
    }



}
