/*	Copyright(c) 1994 SEGA			*/
/*
 *
 *   ウィンドウ機能を使ったサンプル
 *
 */

#include "smp_bios.h"

#include "smp_slcg.h"
#include "smp_slct.h"
#include "smp_wicg.h"
#include "smp_wind.h"


/*
 *    スプライトキャラクタNo
 */
#define SMWI_logocharNo 0x40
#define SMWI_Lrun1CharNo 0x41
#define SMWI_Lrun2CharNo 0x42
#define SMWI_Lrun3CharNo 0x43

/* 
 *    外部参照ワーク
 */
extern SclSysreg Scl_s_reg;
extern SclWinscl Scl_w_reg;
extern SclNorscl Scl_n_reg;



/*
 *    メニューデータ
 */
const Uint8 SMWI_mes1[] = " 1. NORMAL BOX  WINDOW";
const Uint8 SMWI_mes2[] = " 2. NORMAL LINE WINDOW";
const Uint8 SMWI_mes3[] = " 3. SPRITE WINDOW";
const Uint8 SMWI_mes4[] = " 4. 3 WINDOWS SAMPLE ";
const Uint8 SMWI_mes5[] = " 5. < EXIT >";

    enum {
        WISL_INIT,
        WISL_LOOP,
        WISL_NBWINDOW,
        WISL_NLWINDOW,
        WISL_SPWINDOW,
        WISL_MXWINDOW,
        WISL_EXIT
        };

const MENU_INFO SMWI_selectmenu[] = {
          SMWI_mes1, WISL_NBWINDOW,
          SMWI_mes2, WISL_NLWINDOW,
          SMWI_mes3, WISL_SPWINDOW,
          SMWI_mes4, WISL_MXWINDOW,
          SMWI_mes5, WISL_EXIT
          };


/*
 *    SMWI_WindModeで使うenum定義
 */
    enum {
        SMWI_INIT,
        SMWI_LOOP,
        SMWI_NBWINDOW,
        SMWI_NLWINDOW,
        SMWI_SPWINDOW,
        SMWI_MXWINDOW,
        SMWI_EXITMODE
      };


    
    /* 矩形ウィンドウ移動制御の値 */
    enum {
        SMWI_NBW_DOWN,
        SMWI_NBW_RIGHT,
        SMWI_NBW_UP,
        SMWI_NBW_LEFT,
        SMWI_NBW_RIGHTDOWN,
        SMWI_NBW_LEFTUP,
        SMWI_NBW_LEFTDOWN,
        SMWI_NBW_RIGHTUP
      };

    /* 矩形ウィンドウ移動のスピードテーブル */
   const Sint16 SMWI_NBW_spdtbl[8][3] = {
          0x000,  0x200, 0x40,  /* 0 DOWN       */
          0x300,  0x000, 0x40,  /* 1 RIGHT      */
          0x000, -0x200, 0x40,  /* 2 UP         */
         -0x300,  0x000, 0x40,  /* 3 LEFT       */
          0x300,  0x200, 0x40,  /* 4 RIGHT DOWN */
         -0x300, -0x200, 0x40,  /* 5 LEFT UP    */
         -0x300,  0x200, 0x40,  /* 6 LEFT DOWN  */
          0x300, -0x200, 0x40,  /* 7 RIGHT UP   */
      };

    /* 矩形ウィンドウ行動パターンテーブル 1 */
    const Uint8 SMWI_NBW_Movetbl1[] = {
          SMWI_NBW_DOWN,        /* 0 */
          SMWI_NBW_RIGHT,       /* 1 */
          SMWI_NBW_LEFTUP,      /* 2 */
          SMWI_NBW_RIGHT,       /* 3 */
          SMWI_NBW_LEFTDOWN,    /* 4 */
          SMWI_NBW_RIGHT,       /* 5 */
          SMWI_NBW_LEFTUP       /* 6 */
	};

    /* 矩形ウィンドウ行動パターンテーブル 2 */
    const Uint8 SMWI_NBW_Movetbl2[] = {
          SMWI_NBW_UP,          /* 0 */
          SMWI_NBW_LEFT,        /* 1 */
          SMWI_NBW_RIGHTDOWN,   /* 2 */
          SMWI_NBW_LEFT,        /* 3 */
          SMWI_NBW_RIGHTUP,     /* 4 */
          SMWI_NBW_LEFT,        /* 5 */
          SMWI_NBW_RIGHTDOWN    /* 6 */ 
      };


/*
 *    走るソニックスプライトパターンテーブル
 */
const Sint16 SMWI_sonic_pat0[] = {
            1,
            SMWI_Lrun1CharNo, SPR_Hrev, 16, 24, -8, -12
        };
const Sint16 SMWI_sonic_pat1[] = {
            1,
            SMWI_Lrun2CharNo, SPR_Hrev, 16, 24, -8, -12
        };
const Sint16 SMWI_sonic_pat2[] = {
            1,
            SMWI_Lrun3CharNo, SPR_Hrev, 16, 24, -8, -12
        };

Sint16 * const SMWI_sonic_pattbl[] = {
            SMWI_sonic_pat0, SMWI_sonic_pat1, SMWI_sonic_pat2
         };


/*
 *    セガロゴスプライトパターンテーブル
 */
/*
    枚数,
    CharNo, HVreverse, Ｈサイズ, Ｖサイズ, Ｈオフセット, Ｖオフセット
 */
const Sint16 SMWI_logo_pat0[] = {
        2,
        SMWI_logocharNo, 0, 0x0060, 0x0020, -0x30, -0x20,
        SMWI_logocharNo, 0, 0x0060, 0x0020, -0x30,     0,
    };

Sint16 * const SMWI_logo_pattbl[] = {
        SMWI_logo_pat0,
    };


/*
 *    関数プロトタイプ
 */
void SMWI_ModeSelAct(MDSELACT *ix);
static void SMWI_cgset(void);
void SMWI_RunSonicAct(WINDACT *ix);
void SMWI_MkLWindTbl(Sint16 *buffbase, Sint16 xposi, Sint16 yposi, 
                  Sint16 rx, Sint16 xdirec);
void SMWI_NBWindAct(NBWINDACT *ix);
void SMWI_NLWindAct(WINDACT *ix);
void SMWI_SPWindAct(WINDACT *ix);
void SMWI_MXWindAct(WINDACT *ix);


/*==========================================================================*/

/*------------------------------------------------------
 *    ウィンドウ機能を使ったサンプルメインプログラム
 *-----------------------------------------------------*/
void SMWI_WindMode(void)
{

    Uint16 i, x;
    Uint16 *wrptr16;
    SclWinscl *winreg;
    WINDACT *iy; 

    while (SMMA_MainMode == WIND_MODE) {
        switch (SMMA_MainLevel) {

            case SMWI_INIT:    
                SMV2_TvOff();
                SMMA_initall();

                /* カラーモードの設定 */
                SCL_SetColRamMode(SCL_CRM15_1024);

                /* ラインウィンドウバッファをクリア */
                SMV2_FilMem16((Uint16 *)SMV2_LWnd0Buf, 0x200, 0x0000);
                SMV2_FilMem16((Uint16 *)SMV2_LWnd1Buf, 0x200, 0x0000);

                /* アスキーCG展開 */
                SMV2_SetFont8((Uint8 *)NBG_PTGEN, 1, 0);
                poke_w(COLOR_RAM + 0, 0x0000);
                poke_w(COLOR_RAM + 2, 0x739c);

                /* スプライトフレームバッファのクリア */
                SMV1_SprCmdStart();
                SMV1_SprCmdEnd();
                SMMA_intwait();
                SMMA_intwait();

                /* モードセレクトアクションセット */
                SMTA_MakeAction(SMWI_ModeSelAct);

                SMV2_TvOn();
                SMMA_MainLevel++;

            case SMWI_LOOP:    
                SMV1_SprCmdStart();

                /* アクション実行 */
                SMTA_ActionLoop();

                SMV1_SprCmdEnd();

                /* V-BLANKインタラプト待ち*/
                SMMA_intwait();     
                break;


            case SMWI_NBWINDOW:    
            case SMWI_NLWINDOW:    
            case SMWI_SPWINDOW:    
            case SMWI_MXWINDOW:    
                SMV2_TvOff();
                SMMA_initall();

                /* カラーモードの設定 */
                SCL_SetColRamMode(SCL_CRM15_1024);

                /* ラインウィンドウバッファをクリア */
                SMV2_FilMem16((Uint16 *)SMV2_LWnd0Buf, 0x200, 0x0000);
                SMV2_FilMem16((Uint16 *)SMV2_LWnd1Buf, 0x200, 0x0000);

                /* プライオリティの設定 */
                SCL_SetPriority(SCL_SP0, 7);
                SCL_SetPriority(SCL_NBG0, 6);
                SCL_SetPriority(SCL_SP1, 5);
                SCL_SetPriority(SCL_NBG1, 4);
                SCL_SetPriority(SCL_RBG0, 0);


                /* ラインカラーの設定 */
                wrptr16 = (Uint16 *)(COLOR_RAM + 0x7c0);
                for (i = 0; i < 0x20; i++)
                *wrptr16++ = (i << 10) + i;
                wrptr16 = (Uint16 *)(LCLRTBLADD);
                for (i = 0; i < 0x100; i++)
                *wrptr16++ = 0x3e0 + (i / 8); 
                Scl_n_reg.linecolmode = 0x80000000 
                                      | ((LCLRTBLADD & 0xfffff) / 2);
 

                /* カラー演算モードの設定 */
                SCL_SET_CCMD(1);
                SCL_SET_SPCCEN(0);
                SCL_SET_N1CCEN(1);
                SCL_SET_N0CCEN(0);
                SCL_SET_LCCCEN(1);
                SCL_SET_N1LCEN(1);
                SCL_SET_EXCCEN(0);
        
                /* スプライトフレームバッファのクリア */
                SMV1_SprCmdStart();
                SMV1_SprCmdEnd();
                SMMA_intwait();
                SMMA_intwait();

                SMWI_cgset();                     /* CG DATA SET */

                /* 走るソニックのアクションセット*/
                for (i = 0; i < 4; i++) {
                    iy = SMTA_MakeAction(SMWI_RunSonicAct);
                    *(Sint16 *)&iy->sprite.xposi = MTH_GetRand() & 0x00ff;
                    *(Sint16 *)&iy->sprite.yposi = MTH_GetRand() & 0x00ff;
                    iy->sprite.xspd = -0x100 * ((MTH_GetRand() & 3) + 1);
                }

                /* 各モードで使うアクションのセット */
                switch (SMMA_MainLevel) {
                    case SMWI_NBWINDOW:    
                        SMTA_MakeAction(SMWI_NBWindAct);
                        break;
                    case SMWI_NLWINDOW:    
                        SMTA_MakeAction(SMWI_NLWindAct);
                        break;
                    case SMWI_SPWINDOW:
                        SMTA_MakeAction(SMWI_SPWindAct);
                        break;    
                    case SMWI_MXWINDOW:    
                        SMTA_MakeAction(SMWI_MXWindAct);
                        break;
                }

                SMV2_TvOn();
                SMMA_MainLevel = SMWI_LOOP;
                break;

            case SMWI_EXITMODE:
                SMMA_MainMode = SELECT_MODE;    /* セレクトモードへ */
                SMMA_MainLevel = 0;
                break;
        }
    }

}


/*==========================================================================*/

/*---------------------------------------------
 *     ウィンドウ機能サンプルモードセレクト
 *--------------------------------------------*/
void SMWI_ModeSelAct(MDSELACT *ix)
{

    Uint8 i;
    Uint8 *wrdata8;
    Uint16 *wraddbase, *wradd;
    Uint16 swdata;
    MENU_INFO *work;

    swdata = SMPA_pad_edge1;

    switch (ix->mode) {
        case WISL_INIT:

            ix->sel.menuptr = SMWI_selectmenu;
            ix->sel.wcnt[0] = sizeof SMWI_selectmenu
                            / (sizeof (struct menu_info));/*MENU LIST COUNT*/
            ix->sel.wcnt[1] = 0;                 /* CURSOR POINTER  */
            ix->sel.wcnt[2] = 0x10;              /* MOZAIC INITIAL HV COUNT */
            ix->sel.wcnt[3] = 0;                 /* MOZAIC DISPLAY TIMER    */
            ix->sel.wreg[0] = ix->sel.wcnt[0]-1; /* MENU LIST NUMBER */
            ix->sel.mode = 0;                    /* INITIAL */

            SMSL_SlctTblSet(ix);
            ix->mode++;
            break;

        case WISL_LOOP:
            SMSL_SlctAct(ix);
            break;

        case WISL_NBWINDOW:
	    SMMA_MainLevel = SMWI_NBWINDOW;
            break;

        case WISL_NLWINDOW:
	    SMMA_MainLevel = SMWI_NLWINDOW;
            break;

        case WISL_SPWINDOW:
	    SMMA_MainLevel = SMWI_SPWINDOW;
            break;

        case WISL_MXWINDOW:
	    SMMA_MainLevel = SMWI_MXWINDOW;
            break;

        case WISL_EXIT:
	    SMMA_MainLevel = SMWI_EXITMODE;
            break;
    }
}


/*==========================================================================*/

/*---------------------------
 *    CG SET
 *--------------------------*/
static void SMWI_cgset(void)
{
    const Uint16 nbg1_map[] = {
        0x1082,0x1084,0x1086,0x1088,
        0x108a,0x108c,0x108e,0x1090,
        0x1092,0x1094,0x1096,0x1098,
        0x109a,0x109c,0x109e,0x10a0,
    };

    Uint16 *dst16, *src16, *temp16;
    Uint16 h,v,i, wk;
    Uint32 *dst32, *temp32, *dst32b;

    /* アスキーフォントの展開 */
    SMV1_SetFont4sp(0, 1, 0);
    SMV2_SetFont8((Uint8 *)NBG_PTGEN, 1, 0);

    /* アスキーフォントカラーのセット */
    poke_w(COLOR_RAM + 0, 0x0000);
    poke_w(COLOR_RAM + 2, 0x739c);

    /* スクロール用CGの展開 */
    SMMI_Ldir8((Uint8 *)scra_info.cgadd, 
               (Uint8 *)(NBG_PTGEN+0x40*0x40), 
               (Uint32)scra_info.cgsize); 

    /* スクロール用カラーの展開 */
    SMMI_Ldir16((Uint16 *)scra_info.clradd, 
                (Uint16 *)(COLOR_RAM+0x200), 
                  (Uint32)scra_info.clrcnt); 


    /* セガロゴスプライトキャラクタの登録 */
    SPR_2SetChar(SMWI_logocharNo, 0, 0, 0x60, 0x20, 
                 (Uint8 *)segalogo_info.cgadd);

    /* セガロゴカラーの展開 */
    SMMI_Ldir16((Uint16 *)segalogo_info.clradd, 
                (Uint16 *)(COLOR_RAM+0x20), 
                segalogo_info.clrcnt);

    /* 走るソニックスプライトキャラクタの登録 */
    SPR_2SetChar(SMWI_Lrun1CharNo, 0, 0, 16, 24, (Uint8 *)LRUN1_info.cgadd);
    SPR_2SetChar(SMWI_Lrun2CharNo, 0, 0, 16, 24, (Uint8 *)LRUN2_info.cgadd);
    SPR_2SetChar(SMWI_Lrun3CharNo, 0, 0, 16, 24, (Uint8 *)LRUN3_info.cgadd);

    /* 走るソニックカラーの展開 */
    SMMI_Ldir16((Uint16 *)LRUN1_info.clradd, 
                (Uint16 *)(COLOR_RAM + 0x40), LRUN1_info.clrcnt);



    /* NBG1に背景マップをセット */
    dst16 = (Uint16 *)NBG1_PTNAMA;
    for (i = 0; i < 1; i++) {
        for (v = 0; v < (64 / 4); v++) {
            temp16 = dst16;
            for (h = 0; h < (64 / 4); h++) {
                SMV2_BoxWrite16(temp16, nbg1_map, 4, 4);
	            temp16 += 0x4;
            }
            dst16 +=  0x40 * 4;
        }
    }


    /* NBG0に背景マップをセット */
    dst16 = (Uint16 *)NBG0_PTNAMA;
    for (i = 0; i < 1; i++) {
        for (v = 0; v < (64 / 4); v++) {
            temp16 = dst16;
            for (h = 0; h < (64 / 4); h++) {
                SMV2_BoxWrite16(temp16, nbg1_map, 4, 4);
	            temp16 += 0x4;
            }
            dst16 +=  0x40 * 4;
        }
    }


}






/*==========================================================================*/

/*----------------------------------
 *    ノーマル矩形ウィンドウデモ
 *---------------------------------*/
void SMWI_NBWindAct(NBWINDACT *ix)
{

    /* ix->modeに対する値 */
    enum {
      	INIT,
        MAIN,
        NEXT 
    };


    Uint8 work8;
    Sint16 xstart, ystart, xend, yend;

    switch (ix->mode) {
        case INIT:
            /* 矩形ウィンドウ０の初期設定 */
            ix->W0posi[X] = 0x40 * 0x10000;
            ix->W0posi[Y] = 0x30 * 0x10000;
            ix->W0size[X] = 0x80 / 2;
            ix->W0size[Y] = 0x60 / 2;

            /* 矩形ウィンドウ１の初期設定 */
            ix->W1posi[X] = 0x100 * 0x10000;
            ix->W1posi[Y] = 0xb0 * 0x10000;
            ix->W1size[X] = 0x80 / 2;
            ix->W1size[Y] = 0x60 / 2;

            /* 行動パターンインデックス初期化 */
            ix->wcnt[0] = 0;
            ix->wcnt[1] = 0;
            ix->wcnt[2] = 0;
            ix->wcnt[3] = 0;
            ix->mode++;

        case MAIN:
            if (ix->wcnt[2] == 0) {
                work8 = SMWI_NBW_Movetbl1[ix->wcnt[0]];
                ix->W0speed[X] = SMWI_NBW_spdtbl[work8][0];
                ix->W0speed[Y] = SMWI_NBW_spdtbl[work8][1];
                ix->wcnt[2] = SMWI_NBW_spdtbl[work8][2];
                ix->wcnt[0]++;
                if (ix->wcnt[0] > (7 - 1))
                    ix->wcnt[0] = 0;
            }

            if (ix->wcnt[3] == 0) {
                work8 = SMWI_NBW_Movetbl2[ix->wcnt[1]];
                ix->W1speed[X] = SMWI_NBW_spdtbl[work8][0];
                ix->W1speed[Y] = SMWI_NBW_spdtbl[work8][1];
                ix->wcnt[3] = SMWI_NBW_spdtbl[work8][2];
                ix->wcnt[1]++;
                if (ix->wcnt[1] > (7 - 1))
                    ix->wcnt[1] = 0;
            }

            ix->W0posi[X] += (ix->W0speed[X] << 8);
            ix->W0posi[Y] += (ix->W0speed[Y] << 8);
            ix->W1posi[X] += (ix->W1speed[X] << 8);
            ix->W1posi[Y] += (ix->W1speed[Y] << 8);
            ix->wcnt[2]--;
            ix->wcnt[3]--;

            /* 矩形ウィンドウ０のセット */
            xstart = *(Sint16 *)&ix->W0posi[X] - ix->W0size[X];
            xend = *(Sint16 *)&ix->W0posi[X] + ix->W0size[X];
            ystart = *(Sint16 *)&ix->W0posi[Y] - ix->W0size[Y];
            yend = *(Sint16 *)&ix->W0posi[Y] + ix->W0size[Y];
            SCL_SetWindow(SCL_W0, SCL_CC, SCL_NBG0 | SCL_CC, SCL_NON,
                          xstart, ystart, xend, yend);

            /* 矩形ウィンドウ１のセット */
            xstart = *(Sint16 *)&ix->W1posi[X] - ix->W1size[X];
            xend = *(Sint16 *)&ix->W1posi[X] + ix->W1size[X];
            ystart = *(Sint16 *)&ix->W1posi[Y] - ix->W1size[Y];
            yend = *(Sint16 *)&ix->W1posi[Y] + ix->W1size[Y];
            SCL_SetWindow(SCL_W1, SCL_CC, SCL_NBG0 | SCL_CC, SCL_NON,
                          xstart, ystart, xend, yend);


            /* NBG1 の スクロール */
            SCL_Open(SCL_NBG1);
            SCL_Move(MTH_FIXED(1.0), MTH_FIXED(1.0), 0);
            SCL_Close();

            /* パッドチェック */
            if (SMPA_pad_edge1 & PAD_START)
                SMMA_MainLevel = SMWI_INIT;
            break;
    }	    


}



/*==========================================================================*/

/*------------------------------------
 *    ノーマルラインウィンドウデモ
 *-----------------------------------*/
void SMWI_NLWindAct(WINDACT *ix)
{

    enum {
      	INIT,
        MAIN,
        NEXT 
    };

    Sint16 yu, yd;
    Uint32 tbsize;
    SclLineWindowTb *tbladd;

    switch (ix->mode) {
        case INIT:
            ix->wcnt[0] = 0;
            ix->wcnt[1] = 0;
            ix->wcnt[2] = 0;

            ix->mode++;

        case MAIN:
            ix->wcnt[3] = (0x70 * SMMI_Sinset8(ix->wcnt[2])) / 0x100;
            ix->wcnt[2] = (ix->wcnt[2] + 1) & 0x7f;;
  
            /* ラインウィンドウテーブル作成 */
            ix->wcnt[0] -= 8;
            SMWI_MkLWindTbl(SMV2_LWnd0Buf, 0x50, 0x70, 
                               ix->wcnt[3], ix->wcnt[0]);

            ix->wcnt[1] += 2;
            SMWI_MkLWindTbl(SMV2_LWnd1Buf, 0xd0, 0x70, 
                               0x60, ix->wcnt[1]);


            yu = 0x70 - ix->wcnt[3];
            yd = 0x70 + ix->wcnt[3];
            tbsize = yd - yu;
            tbladd = (SclLineWindowTb *)SMV2_LWnd0Buf + yu;
            /* ノーマルラインウィンドウ０の設定 */
            SCL_SetLineWindow(SCL_W0, SCL_CC, SCL_NBG0 | SCL_CC, SCL_NON, 
                              LWND0TBLADD, yu, 
                              tbsize, tbladd);

            tbsize = ((0x70 + 0x60) - (0x70 - 0x60));
            tbladd = (SclLineWindowTb *)SMV2_LWnd1Buf + (0x70 - 0x60);
            /* ノーマルラインウィンドウ１の設定 */
            SCL_SetLineWindow(SCL_W1, SCL_CC, SCL_NBG0 | SCL_CC, SCL_NON, 
                              LWND1TBLADD, (0x70-0x60), 
                              tbsize, tbladd);

            /* NBG1 のスクロール */
            SCL_Open(SCL_NBG1);
            SCL_Move(MTH_FIXED(0), MTH_FIXED(-1.0), 0);
            SCL_Close();

            if (SMPA_pad_edge1 & PAD_START)
                SMMA_MainLevel = SMWI_INIT;
            break;
    }	    
}



/*-------------------------------------------------------------------
 *  ラインウィンドウ用円形ラインテーブル作成
 *-------------------------------------------------------------------
 *  IN: buffbase   :ラインテーブル格納バッファアドレス (0x200 bytes)
 *      xposi.yposi:円の中心座標(Xは２倍にして入れること）
 *      rx         :円の半径
 *        
 *  NOTES:アルゴリズムの手抜きにより大きな円には対応せず、
 *        大きさに関係なく速度は一定で非常に遅いです。
 *-------------------------------------------------------------------
 */
void SMWI_MkLWindTbl(Sint16 *buffbase, Sint16 xposi, Sint16 yposi, 
                  Sint16 rx, Sint16 xdirec)
{
    Uint16 direc;
    Sint16 i;
    Sint32 xofst, yofst;
    Sint16 xl, xr;
    Sint16 yu, yd;
    Sint16 xposib;

    Sint16 *srcw, *dstw;

    for (direc = 0x100; direc > 0x0; direc--) {
        xofst = ((Sint32)rx * SMMI_Cosset(direc)) / 0x4000;
        yofst = ((Sint32)rx * SMMI_Sinset(direc)) / 0x4000;

        xposib = xposi + (0x20 * SMMI_Sinset8(xdirec) / 0x100);
        xdirec++;

        xl = (Sint16)(((Sint32)xposib - xofst) * 1); /* 2 */
        xr = (Sint16)(((Sint32)xposib + xofst) * 1); /* 2 */
        yu = (Sint16)((Sint32)yposi - yofst);
        yd = (Sint16)((Sint32)yposi + yofst);
        
        if (xl < 0) xl = 0;
        if (xr < 0) xr = 0;
        if (xl > 320 * 2) xl = 320 * 1; /* 2 */
        if (xr > 320 * 2) xr = 320 * 1; /* 2 */

        if ((yu >= 0) && (yu < 0x100)) {
            srcw = buffbase + (yu * 2);
            *srcw++ = xl;
            *srcw = xr;
        }
        if ((yd >= 0) && (yd < 0x100)) {
            srcw = buffbase + (yd * 2);
            *srcw++ = xl;
            *srcw++ = xr;
        }

    }


}




/*==========================================================================*/

/*--------------------------------
 *    スプライトウィンドウデモ
 *-------------------------------*/
void SMWI_SPWindAct(WINDACT *ix)
{

    enum {
      	INIT,
        MAIN,
        NEXT 
    };
    Uint16 i;
    WINDACT *iy;

    switch (ix->mode) {
        case INIT:
            /* 走るウィンドウソニックのアクションセット*/
            for (i = 0; i < 4; i++) {
                 iy = SMTA_MakeAction(SMWI_RunSonicAct);
                    *(Sint16 *)&iy->sprite.yposi = MTH_GetRand() & 0xff;
                    *(Sint16 *)&iy->sprite.xposi = MTH_GetRand() & 0xff;
                    iy->sprite.xspd = -0x80 * ((MTH_GetRand() & 3) + 1);
                    iy->sprite.yspd = 0;
                    iy->sprite.putmode = PMD_sprx_rotzoom;
                    iy->sprite.patbase = SMWI_sonic_pattbl;
                    iy->sprite.patno = 0;
                    iy->sprite.hzoom = 0x400;
                    iy->sprite.vzoom = 0x400;
                    iy->sprite.patchgcnt = 0;
                    iy->sprite.pmode = 0x8000;
                    iy->sprite.color = 0x0020;
                    iy->mode++;
            }
            ix->mode++;

        case MAIN:

            /* スプライトデータの設定 */
            SCL_SetSpriteMode(SCL_TYPE7, SCL_PALETTE, SCL_SP_WINDOW);

            /* スプライトウィンドウの設定 */
            SCL_SetSpriteWindow(SCL_NON, SCL_NBG0, SCL_NON);   
            SCL_SET_SPWINEN(1);

            /* NBG1 スクロールセット */
            SCL_Open(SCL_NBG1);
            SCL_Move(MTH_FIXED(1.0), MTH_FIXED(-1.0), 0);
            SCL_Close();

            /* パッドチェック*/
            if (SMPA_pad_edge1 & PAD_START)
                SMMA_MainLevel = SMWI_INIT;
            break;
                
    }	    
}




/*==========================================================================*/

/*-------------------------------------------
 *    矩形、ライン、スプライトウィンドウ
 *------------------------------------------*/
void SMWI_MXWindAct(WINDACT *ix)
{

    enum {
      	INIT,
        MAIN,
        NEXT 
    };

    Uint16 swedge;
    Uint32 tbsize;
    SclLineWindowTb *tbladd;

    swedge = SMPA_pad_edge1;

    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_rotzoom; 
            ix->sprite.pmode = 0x000;
            ix->sprite.patbase = SMWI_logo_pattbl;
            ix->sprite.patno = 0;
            ix->sprite.xposi = 0xa0 * 0x10000;
            ix->sprite.yposi = 0x70 * 0x10000;
            ix->sprite.color = 0x0010;
            ix->sprite.hzoom = 0x200;
            ix->sprite.vzoom = 0x200;
            ix->sprite.zdirec = 0;
            ix->sprite.patchgcnt = 0;
            ix->wcnt[0] = 0;
            ix->wcnt[1] = 0;
            
            ix->mode++;

        case MAIN:
            /* スプライトを回転させる*/
            ix->sprite.zdirec--;

            /* ラインウィンドウテーブル作成 */
            ix->wcnt[1] += 2;
            SMWI_MkLWindTbl(SMV2_LWnd1Buf, 0xe0, 0x60, 0x40, ix->wcnt[1]);


            /* スプライトウィンドウの設定     */
            if (ix->wcnt[0] == 1)
                SCL_SetSpriteWindow(SCL_CC, SCL_NBG0 | SCL_CC, SCL_NON);   
            else
                SCL_SET_SPWINEN(0);

            /* ノーマル矩形ウィンドウの設定   */
            SCL_SetWindow(SCL_W0, SCL_CC, SCL_NBG0 | SCL_CC, SCL_NON,
                          0, 0x80, 0x80, 0xe0);

            /* ノーマルラインウィンドウの設定 */
            tbsize = ((0x60+0x40) - (0x60 - 0x40));
            tbladd = (SclLineWindowTb *)SMV2_LWnd1Buf + (0x60-0x40);
            SCL_SetLineWindow(SCL_W1, SCL_CC, SCL_NBG0 | SCL_CC, SCL_NON, 
                              LWND1TBLADD, 0x20, 
                              tbsize, tbladd);

            /* NBG1 スクロールセット */
            SCL_Open(SCL_NBG1);
            SCL_Move(MTH_FIXED(1.0), MTH_FIXED(-1.0), 0);
            SCL_Close();

            ix->sprite.patchgcnt++;
            if (ix->sprite.patchgcnt > 60 * 1) {
                ix->wcnt[0]++;
                ix->wcnt[0] %= 3;
                ix->sprite.patchgcnt = 0;
            }
    
            switch (ix->wcnt[0]) {
                case 0:
                    ix->sprite.pmode = 0x0000;
                    break;
                case 1:
                    /* スプライトMSBをスプライトウィンドウに */
                    SCL_SetSpriteMode(SCL_TYPE7, SCL_PALETTE, 
                                                 SCL_SP_WINDOW);
                    ix->sprite.pmode = 0x8000;
                    break;
                case 2:
                    /* スプライトMSBをMSBシャドウに*/
                    SCL_SetSpriteMode(SCL_TYPE7, SCL_PALETTE, 
                                                 SCL_MSB_SHADOW); 
                    ix->sprite.pmode = 0x8000;
                    /* シャドウをスクロールにでるようにする */
                    SCL_SetShadowBit(SCL_NBG0 | SCL_NBG1);
                    SCL_SET_TPSDSL(1);
            }


            if (swedge & PAD_START)
                SMMA_MainLevel = SMWI_INIT;
            break;
    }	    

    SMV1_PutSprite(&ix->sprite);
}



/*==========================================================================*/

/*---------------------------
 *    走るソニック
 *---------------------------*/
void SMWI_RunSonicAct(WINDACT *ix)
{


    enum {
      	INIT,
        MAIN,
        NEXT 
    };

    Uint16 swdata;

    swdata = SMPA_pad_data1;

    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_rotzoom;
            ix->sprite.patbase = SMWI_sonic_pattbl;
            ix->sprite.patno = 0;
            ix->sprite.hzoom = 0x200;
            ix->sprite.vzoom = 0x200;
            ix->sprite.yspd = 0x0000;
            ix->sprite.patchgcnt = 0;
            ix->sprite.pmode = 0;
            ix->sprite.color = 0x1020;
            ix->sprite.zdirec = 0;
            ix->mode++;
        case MAIN:

            ix->sprite.xposi += (ix->sprite.xspd << 8);
            ix->sprite.yposi += (ix->sprite.yspd << 8);

            if (*(Sint16 *)&ix->sprite.xposi > 360)
                *(Sint16 *)&ix->sprite.xposi = -20;
            if (*(Sint16 *)&ix->sprite.xposi < -20)
                *(Sint16 *)&ix->sprite.xposi = 360;

            if (*(Sint16 *)&ix->sprite.yposi > 240)
                *(Sint16 *)&ix->sprite.yposi = -20;
            if (*(Sint16 *)&ix->sprite.yposi < -20)
                *(Sint16 *)&ix->sprite.yposi = 240;

            ix->sprite.patchgcnt++;
            if ((ix->sprite.patchgcnt & 3) == 0) {
                ix->sprite.patno++;
                ix->sprite.patno %= 3;
            }
            break;
    }	    
    SMV1_PutSprite(&ix->sprite);
}





