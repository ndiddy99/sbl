/*	Copyright(c) 1994 SEGA			*/
/*
 *
 * 2ndstepライブラリを使ったサンプルプログラム集
 * 1994/3/4 Y.M
 *
 */


#include "smp_bios.h"




/*
 *     関数プロトタイプ 
 */
static void SMMA_IniSystem(void);
void SMMA_LoadModule(Uint8 *fname);


Uint8 SMMA_MainMode;
Uint8 SMMA_MainLevel;

Uint8 SMMA_fname_logo[] = "LOGO.BIN";
Uint8 SMMA_fname_titl[] = "TITL.BIN";
Uint8 SMMA_fname_wind[] = "WIND.BIN";
Uint8 SMMA_fname_scrl[] = "SCRL.BIN";
Uint8 SMMA_fname_gm2d[] = "GM2D.BIN";
Uint8 SMMA_fname_sprt[] = "SPRT.BIN";
Uint8 SMMA_fname_d214[] = "D214.BIN";

Uint32 SMSC_RoutineNum;

/*===================================================*/
/*      メインループ                                 */
/*      各モードを0x6080000以降にロードし実行する    */
/*===================================================*/
void main (void)
{
    SMMA_IniSystem();	          /* System Initialize                */
    SMV1_SprCmdStart();
    SMV1_SprCmdEnd();             /* SPRITE COMMAND SET               */
    SCL_DisplayFrame();           /* V-BLANK WAIT                     */
    SMMA_MainMode = LOGO_MODE;    /* First exec mode = SLOGO_MODE     */
    SMMA_MainLevel = 0;

    for (;;) {
        switch (SMMA_MainMode) {
            case DUMMY_MODE:
                SMMA_intwait();
                break;

            case LOGO_MODE:
                SMMA_LoadModule(SMMA_fname_logo);
               /* SMLO_SegaLogo(); */
                ((void (*)()) (0x6080000))();
                break;

            case TITLE_MODE:
                SMMA_LoadModule(SMMA_fname_titl);
                /* SMTI_Title(); */
                ((void (*)()) (0x6080000))();
                break;

            case SELECT_MODE:
                SMSL_ModeSel();
                break;

            case SMSC_SELECTMODE:
                SMMA_LoadModule(SMMA_fname_scrl);
                /* SMSC_SelectMode(); */
                SMSC_RoutineNum = 0x00;
                ((void (*)()) (0x6080000))();
                break;

            case SMSC_MODE:
                SMMA_LoadModule(SMMA_fname_scrl);
                /* SMSC_SclMode(); */
			    SMSC_RoutineNum = 0x01;
                ((void (*)()) (0x6080000))();
                break;                

            case SMSC_BACK:
                SMMA_LoadModule(SMMA_fname_scrl);
                /* SMSC_BackScl(); */
			    SMSC_RoutineNum = 0x02;
                ((void (*)()) (0x6080000))();
				break;                

            case SMSC_NORMSCLMODE:
                SMMA_LoadModule(SMMA_fname_scrl);
                /* SMSC_NormalScl(); */
			    SMSC_RoutineNum = 0x03;
                ((void (*)()) (0x6080000))();
                break;                

            case LNSC_MODE:
                SMMA_LoadModule(SMMA_fname_scrl);
                /* SMLN_LnscMode(); */
			    SMSC_RoutineNum = 0x04;
                ((void (*)()) (0x6080000))();
                break;                

            case SMSC_MULTISCLMODE:
                SMMA_LoadModule(SMMA_fname_scrl);
                /* SMSC_MultiScl(); */
			    SMSC_RoutineNum = 0x05;
                ((void (*)()) (0x6080000))();
                break;                

            case SMSC_ROTSCLMODE:
                SMMA_LoadModule(SMMA_fname_scrl);
                /* SMSC_RotateScl(); */
			    SMSC_RoutineNum = 0x06;
                ((void (*)()) (0x6080000))();
                break;                

            case LCLR_MODE:
                SMMA_LoadModule(SMMA_fname_scrl);
                /* SMLC_LclrMode(); */
			    SMSC_RoutineNum = 0x07;
                ((void (*)()) (0x6080000))();
                break;

            case SMRK_MODE:
                SMMA_LoadModule(SMMA_fname_scrl);
                /* SMRK_MainMode(); */
			    SMSC_RoutineNum = 0x08;
                ((void (*)()) (0x6080000))();
                break;                

            case WIND_MODE:
                SMMA_LoadModule(SMMA_fname_wind);
                /* SMWI_WindMode(); */
                ((void (*)()) (0x6080000))();
				break;

            case GAME_MODE:
                SMMA_LoadModule(SMMA_fname_gm2d);
				/* SMG2_GameMode(); */
                ((void (*)()) (0x6080000))();

                break;

            case SPSAMPLE_MODE:
                SMMA_LoadModule(SMMA_fname_sprt);
                /* SMSP_DemoMode(); */
                ((void (*)()) (0x6080000))();
                break;

            case DEMO214_MODE:
                SMMA_LoadModule(SMMA_fname_d214);
                /* SMD2_MainMode(); */
                ((void (*)()) (0x6080000))();
                break;


        }

    }
}


/*================================================*/
/*   GFSライブラリを使った簡単なファイルロード    */
/*   毎回初期化するので無駄かもしれません。       */
/*================================================*/
void SMMA_LoadModule(Uint8 *fname)
{
    Sint32 result;

      result = SMCD_InitFileSys(); /* <<<< 必要ないでしょう */
      if (result <= 2) {
         SMMA_MainMode= DUMMY_MODE;
         return;
      }

      result = SMCD_FileLoad(fname, (Uint8 *)0x6080000);
      while (result <= 0) {
      }
     
}



/*
 *  V-IntWait
 */
void SMMA_intwait(void)
{
    SMIN_vintflag = 1;             /* vint ready flag set   */
    SCL_DisplayFrame();            /* V-BLANK WAIT          */
    SMIN_vintflag = 0;             /* vint ready flag set   */
}



/*
 *  System initialize
 */
static void SMMA_IniSystem(void)
{
    di();
    SMV2_TvOff();    
    INT_ChgMsk(INT_MSK_NULL, INT_MSK_VBLK_IN | INT_MSK_VBLK_OUT | INT_MSK_SPR);
    SCL_VblInit();                 /* V-BLANK割り込みライブラリワーク初期化 */
    INT_SetScuFunc(INT_SCU_VBLK_IN , SMIN_VblankIn);  /* VINT-IN  関数登録 */
    INT_SetScuFunc(INT_SCU_VBLK_OUT, SMIN_VblankOut); /* VINT-OUT 関数登録 */
    INT_SetScuFunc(INT_SCU_SPR     , SMIN_SpriteEnd); /* 描画終了 関数登録  */
    SMIN_VintHook1 = NULL;         /* インタラプトフック 初期化 */
    SMV2_IniScroll();
    SMV1_IniSprite();
    SMPA_SwInit();
    SMTA_ActWkInit();
    MTH_InitialRand(0xaa55aa55);
    SCL_SetFrameInterval(0xffff); /* HI-DOUKI mode                       */
    INT_ChgMsk(INT_MSK_VBLK_IN | INT_MSK_VBLK_OUT | INT_MSK_SPR ,INT_MSK_NULL);
    SMCD_InitFileSys(); /* CD FILE SYSTEM INIT */
    SMSN_IniSound();    /* サウンドシステムの初期化 */
    ei();
}




/*
 *  Initial ALL
 */
void SMMA_initall(void)
{
    di();
    INT_ChgMsk(INT_MSK_NULL, INT_MSK_VBLK_IN | INT_MSK_VBLK_OUT | INT_MSK_SPR);
    SCL_VblInit();                 /* V-BLANK割り込みライブラリワーク初期化 */
    INT_SetScuFunc(INT_SCU_VBLK_IN , SMIN_VblankIn);  /* VINT-IN  関数登録  */
    INT_SetScuFunc(INT_SCU_VBLK_OUT, SMIN_VblankOut); /* VINT-OUT 関数登録  */
    INT_SetScuFunc(INT_SCU_SPR     , SMIN_SpriteEnd); /* 描画終了 関数登録  */
    SMIN_VintHook1 = NULL;         /* インタラプトフック 初期化 */
    SMV2_IniScroll();
    SMV1_IniSprite();
    SPR_2ClrAllChar();
    SMPA_SwInit();
    SMTA_ActWkInit();
    MTH_InitialRand(0xaa55aa55);
    SCL_SetFrameInterval(0xffff); /* HI-DOUKI mode                       */
    INT_ChgMsk(INT_MSK_VBLK_IN | INT_MSK_VBLK_OUT | INT_MSK_SPR ,INT_MSK_NULL);
    ei();
}

