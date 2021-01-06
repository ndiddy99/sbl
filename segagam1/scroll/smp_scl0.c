/*	Copyright(c) 1994 SEGA			*/

/*
 *
 *     SCROLL SAMPLE
 *
 */

#include "smp_bios.h"

#include "smp_slct.h"
#include "smp_scl0.cg"


/*
 *
 *    アクションワークアサイン
 *
 */
SMTA_DefActWk(SMSCACT,
        SPRITE sprite;
        );

/*
 *    関数プロトタイプ
 */
void SMSC_SelectMode(void);
void SMSC_SclMode(void);
void SMSC_BackScl(void);
void SMSC_NormalScl(void);
void SMLN_LnscMode(void);
void SMSC_MultiScl(void);
void SMSC_RotateScl(void);
void SMLC_LclrMode(void);
void SMRK_MainMode(void);

void SMSC_ModeSelAct(MDSELACT *ix);
void SMSC_normscl(SMSCACT *ix);
void SMSC_Setcg2(Uint8 *);
void SMSC_opaact(SMSCACT *ix);
void SMSC_patchg(SMSCACT *ix);
void SMSC_multiscl(SMSCACT *ix);
void SMSC_IniMultiScr(void);
void SMSC_Ldir8(Sint8 *src, Sint8 *dst, Sint32 len);

/* void SMSC_SetFont8(Uint8 *cgadd); */

/* 
 *    外部参照
 */
extern Uint8 const SMSP_OPA1_cg[];
extern Uint8 const SMSP_OPA2_cg[];
extern Uint8 const SMSP_OPA3_cg[];
extern Uint16 const SMSP_OPA1_cl[];
extern void SMIN_VblankIn(void);
extern void SMIN_VblankOut(void);
extern SclNorscl Scl_n_reg;

extern Uint32 SMSC_RoutineNum;

void (* const SMSC_JumpTbl[])(void) = {
	SMSC_SelectMode,
    SMSC_SclMode,
    SMSC_BackScl,
    SMSC_NormalScl,
    SMLN_LnscMode,
    SMSC_MultiScl,
    SMSC_RotateScl,
    SMLC_LclrMode,
    SMRK_MainMode
};

/*
 * OPAOPA パタンチェンジテーブル
 */
#define OPA1_charno 0x51

static const Sint16 opa_pat0[] = {
        1,OPA1_charno +0,0, 32, 32, -16, -16,
    };
static const Sint16 opa_pat1[] = {
        1,OPA1_charno +1,0, 32, 32, -16, -16,
    };
static const Sint16 opa_pat2[] = {
        1,OPA1_charno +2,0, 32, 32, -16, -16,
    };

static Sint16 * const opa_pat[] = {
        opa_pat0,
        opa_pat1,
        opa_pat2,
        opa_pat1,
    };

static const Sint16 anime_time[] = {
        10,
        10,
        10,
        10,
    };

/*
 *    スクロールVRAMアサイン 
 */
#define SMSC_VRAM_A     0x25e00000
#define SMSC_VRAM_B     0x25e40000
#define SMSC_NBG_PTGEN  (SMSC_VRAM_A + 0x00000)   /* NBG PTGEN  */
#define SMSC_N0_PTN     (SMSC_VRAM_B + 0x00000)   /* NBG0 PTNAM */
#define SMSC_N1_PTN     (SMSC_VRAM_B + 0x02000)   /* NBG1 PTNAM */
#define SMSC_N2_PTN     (SMSC_VRAM_B + 0x04000)   /* NBG2 PTNAM */
#define SMSC_N3_PTN     (SMSC_VRAM_B + 0x06000)   /* NBG3 PTNAM */
/*
 *    ノーマルスクロール定数アサイン
 */
#define SMSC_MAXSPD 3.0     /* NORMAL SCROLL MAX SPEED */
#define SMSC_MLTSPD 2.5     /* MULTI SCROLL BASE SPEED */
#define OPA1_charno 0x51
#define SMSC_ZMMAX 0x100    /* OPAOPA ZOOM MAX RATE */
#define SMSC_ZMMIN 0x0c0    /* OPAOPA ZOOM MIN RATE */

/*
 *    メニューデータ
 */
const Uint8 SMSC_mes1[] = " 1. BITMAP SCROLL";
const Uint8 SMSC_mes2[] = " 2. NORMAL SCROLL";
const Uint8 SMSC_mes3[] = " 3. LINE SCROLL";
const Uint8 SMSC_mes4[] = " 4. MULTI SCROLL";
const Uint8 SMSC_mes5[] = " 5. LINECOLOR SAMPLE";
const Uint8 SMSC_mes6[] = " 6. ROTATE SCROLL";
const Uint8 SMSC_mes7[] = " 7. ROTATE KEISU SAMPLE ";
const Uint8 SMSC_mes9[] = " < EXIT >";

    enum {
        SCSL_INIT,
        SCSL_LOOP,
        SCSL_SCROLL1,
        SCSL_SCROLL2,
        SCSL_SCROLL3,
        SCSL_SCROLL4,
        SCSL_SCROLL5,
        SCSL_SCROLL6,
        SCSL_SCROLL7,
        SCSL_EXIT
        };

const MENU_INFO SMSC_selectmenu[] = {
          SMSC_mes1, SCSL_SCROLL1,
          SMSC_mes2, SCSL_SCROLL2,
          SMSC_mes3, SCSL_SCROLL3,
          SMSC_mes4, SCSL_SCROLL4,
          SMSC_mes5, SCSL_SCROLL5,
          SMSC_mes6, SCSL_SCROLL6,
          SMSC_mes7, SCSL_SCROLL7,
          SMSC_mes9, SCSL_EXIT
          };

/*
 Here we go loop de loo...
*/
void SMSC_JumpTable(void)
{
   (*SMSC_JumpTbl[SMSC_RoutineNum])();
}

/*==========================================================================*/

/*------------------------------------------------------
 *    スクロールサンプルプログラムセレクトモード
 *-----------------------------------------------------*/
void SMSC_SelectMode(void)
{

    enum {
          INIT,
          LOOP,
          NEXT
    };


    while (SMMA_MainMode == SMSC_SELECTMODE) {
        switch (SMMA_MainLevel) {
            case INIT:
                SMV2_TvOff();
                SMMA_initall();

                /* カラーモードの設定 */
                SCL_SetColRamMode(SCL_CRM15_1024);

                /* スプライトフレームバッファのクリア */
                SMV1_SprCmdStart();
                SMV1_SprCmdEnd();
                SMMA_intwait();
                SMMA_intwait();

                /* アスキーCG展開 */
                SMV2_SetFont8((Uint8 *)NBG_PTGEN, 1, 0);
                poke_w(COLOR_RAM + 0, 0x0000);
                poke_w(COLOR_RAM + 2, 0x739c);

                /* モードセレクトアクションセット */
                SMTA_MakeAction(SMSC_ModeSelAct);

                /* 自動ＶＥの強制終了 */
                SCL_AbortAutoVe();

                SMV2_TvOn();
                SMMA_MainLevel++;
                /* NO BREAK */

            case LOOP:    
                /* スプライト表示準備 */
                SMV1_SprCmdStart();

                /* アクション実行 */
                SMTA_ActionLoop();

                /* スプライト表示終了コマンドセット */
                SMV1_SprCmdEnd();

                /* V-BLANKインタラプト待ち*/
                SMMA_intwait();     
                break;
        }
    }
}


/*==========================================================================*/

/*---------------------------------------------
 *     モードセレクトプログラム
 *--------------------------------------------*/
void SMSC_ModeSelAct(MDSELACT *ix)
{

    Uint8 i;
    Uint8 *wrdata8;
    Uint16 *wraddbase, *wradd;
    Uint16 swdata;
    static Sint32 zmrate;
    MENU_INFO *work;

    swdata = SMPA_pad_edge1;

    switch (ix->mode) {
        case SCSL_INIT:
            ix->sel.menuptr = SMSC_selectmenu;
            ix->sel.wcnt[0] = sizeof SMSC_selectmenu
                            / (sizeof (struct menu_info));/*MENU LIST COUNT*/
            ix->sel.wcnt[1] = 0;                 /* CURSOR POINTER  */
            ix->sel.wcnt[2] = 0x10;              /* MOZAIC INITIAL HV COUNT */
            ix->sel.wcnt[3] = 0;                 /* MOZAIC DISPLAY TIMER    */
            ix->sel.wreg[0] = ix->sel.wcnt[0]-1; /* MENU LIST NUMBER */
            ix->sel.mode = 0;                    /* INITIAL */

            /* MODE SELECT TABLE SET */
            SMSL_SlctTblSet(ix);
            ix->mode++;
            break;

        case SCSL_LOOP:
            /* MODE SELECT ACTION */
            SMSL_SlctAct(ix);
            break;

        case SCSL_SCROLL1:
            SMMA_MainMode = SMSC_MODE;
            SMMA_MainLevel = 0;
            break;

        case SCSL_SCROLL2:
            SMMA_MainMode = SMSC_NORMSCLMODE;
	    SMMA_MainLevel = 0;
            break;

        case SCSL_SCROLL3:
            SMMA_MainMode = LNSC_MODE;
	    SMMA_MainLevel = 0;
            break;

        case SCSL_SCROLL4:
            SMMA_MainMode = SMSC_MULTISCLMODE;
	    SMMA_MainLevel = 0;
            break;

        case SCSL_SCROLL5:
            SMMA_MainMode = LCLR_MODE;
	    SMMA_MainLevel = 0;
            break;

        case SCSL_SCROLL6:
            SMMA_MainMode = SMSC_ROTSCLMODE;
	    SMMA_MainLevel = 0;
            break;

        case SCSL_SCROLL7:
            SMMA_MainMode = SMRK_MODE;
	    SMMA_MainLevel = 0;
            break;

        case SCSL_EXIT:
            SMMA_MainMode = SELECT_MODE;
	    SMMA_MainLevel = 0;
            break;
    }
}

/************ ノーマルスクロール ************/
/*
 *
 *  NORMAL SCROLL MODE CONTROL
 *
 */
void SMSC_NormalScl(void)
{
    enum {
        SCL_INIT,
        SCL_MAIN,
        SCL_EXIT
    };

    SclVramConfig tp;
    SclConfig scfg;

    while(SMMA_MainMode == SMSC_NORMSCLMODE) {
        /* スプライト表示準備 */
        SMV1_SprCmdStart();

        switch(SMMA_MainLevel) {
            case SCL_INIT:
                SMV2_TvOff();
                SMMA_initall();

                /* カラーモードの設定 */
                SCL_SetColRamMode(SCL_CRM15_1024);

                /* スプライトフレームバッファのクリア */
                SMV1_SprCmdStart();
                SMV1_SprCmdEnd();
                SMMA_intwait();
                SMMA_intwait();

                /* プライオリティの初期化 */
                SCL_SetPriority(SCL_NBG0, 6);
                SCL_SetPriority(SCL_SP0, 7);

                /* NBG0 CG & COLOR セット */
                SMV2_FilMem16((Uint16 *)NBG_PTGEN,
                        (0x40 * 0x40 / sizeof (Uint16)), 0x0000);
                SMSC_Setcg2((Uint8 *)NBG_PTGEN);
                SMMI_Ldir16((Uint16 *)SMSC_B_1_info.clradd,
                        (Uint16 *)COLOR_RAM+0x10,16);

                /* OPAOPA CG & COLOR セット */
                SPR_2SetChar(OPA1_charno+0, 0,0,16,16, (Uint8 *)SMSP_OPA1_cg);
                SPR_2SetChar(OPA1_charno+1, 0,0,16,16, (Uint8 *)SMSP_OPA2_cg);
                SPR_2SetChar(OPA1_charno+2, 0,0,16,16, (Uint8 *)SMSP_OPA3_cg);
                SMMI_Ldir16((Uint16 *)SMSP_OPA1_cl, (Uint16 *)COLOR_RAM,0x10);

                /* スクロールポジションの初期化 */
                SCL_Open(SCL_NBG0);
                SCL_MoveTo(FIXED(0), FIXED(0),0);        /* Home Position */
                SCL_Close();

                /* スクロールアクションセット */
                SMTA_MakeAction(SMSC_normscl);

                /* スプライトアクションセット */
                SMTA_MakeAction(SMSC_opaact);

                SMMA_MainLevel++;
                break;

            case SCL_MAIN:
                /* アクション実行 */
                SMTA_ActionLoop();
                break;

            case SCL_EXIT:
                SMMA_MainMode = SMSC_SELECTMODE;
                SMMA_MainLevel = 0;
                break;
        }
        /* スプライト表示終了コマンドセット */
        SMV1_SprCmdEnd();

        /* V-BLANKインタラプト待ち*/
        SMMA_intwait();
    }
}


/*
 *
 *  NORMAL SCROLL ACTION
 *
 */
void SMSC_normscl(SMSCACT *ix)
{
    enum {
        INIT,
        MAIN
    };

    static Fixed32 mx, my;
    Uint16 swdata;

    swdata = SMPA_pad_data1;
    switch(ix->level) {
        case INIT:
            /* NBG0 マップセット(64*64cell) */
            SMV2_BoxWrite16((Uint16 *)(NBG0_PTNAMA + 00 * 2 + 00 * 0x80),
                SMSC_B_1_info.mapadd,32,32);
            SMV2_BoxWrite16((Uint16 *)(NBG0_PTNAMA + 32 * 2 + 00 * 0x80),
                SMSC_B_1_info.mapadd,32,32);
            SMV2_BoxWrite16((Uint16 *)(NBG0_PTNAMA + 00 * 2 + 32 * 0x80),
                SMSC_B_1_info.mapadd,32,32);
            SMV2_BoxWrite16((Uint16 *)(NBG0_PTNAMA + 32 * 2 + 32 * 0x80),
                SMSC_B_1_info.mapadd,32,32);
            mx = FIXED(0);
            my = FIXED(0);

            SMV2_TvOn();
            ix->level++;
            /* NO BREAK */

        case MAIN:
            if((swdata & PAD_C)) {
                if((swdata & PAD_LEFT)) {
                    if (mx > FIXED(-SMSC_MAXSPD*2)) {
                        mx = mx - FIXED(0.1);
                    }
                    else
                        mx = FIXED(-SMSC_MAXSPD*2);
                }
                if((swdata & PAD_RIGHT)) {
                    if (mx < FIXED(SMSC_MAXSPD*2)) {
                        mx = mx + FIXED(0.1);
                    }
                    else
                        mx = FIXED(SMSC_MAXSPD*2);
                }
                if((swdata & PAD_UP)) {
                    if (my > FIXED(-SMSC_MAXSPD*2)) {
                        my = my - FIXED(0.1);
                    }
                    else
                        my = FIXED(-SMSC_MAXSPD*2);
                }
                if((swdata & PAD_DOWN)) {
                    if (my < FIXED(SMSC_MAXSPD*2)) {
                         my = my + FIXED(0.1);
                    }
                    else
                        my = FIXED(SMSC_MAXSPD*2);
                }
            }
            else {
                if((swdata & PAD_LEFT)) {
                    if (mx > FIXED(-SMSC_MAXSPD)) {
                        mx = mx - FIXED(0.1);
                    }
                    else if (mx < FIXED(-SMSC_MAXSPD)) {
                        mx = mx + FIXED(0.1);
                    }
                    else
                        mx = FIXED(-SMSC_MAXSPD);
                }
                if((swdata & PAD_RIGHT)) {
                    if (mx < FIXED(SMSC_MAXSPD)) {
                        mx = mx + FIXED(0.1);
                    }
                    else if (mx > FIXED(SMSC_MAXSPD)) {
                        mx = mx - FIXED(0.1);
                    }
                    else
                        mx = FIXED(SMSC_MAXSPD);
                }
                if((swdata & PAD_UP)) {
                    if (my > FIXED(-SMSC_MAXSPD)) {
                        my = my - FIXED(0.1);
                    }
                    else if (my < FIXED(-SMSC_MAXSPD)) {
                        my = my + FIXED(0.1);
                    }
                    else
                        my = FIXED(-SMSC_MAXSPD);
                }
                if((swdata & PAD_DOWN)) {
                    if (my < FIXED(SMSC_MAXSPD)) {
                         my = my + FIXED(0.1);
                    }
                    else if (my > FIXED(SMSC_MAXSPD)) {
                         my = my - FIXED(0.1);
                    }
                    else
                        my = FIXED(SMSC_MAXSPD);
                }
            }

            if((swdata & PAD_B)) {
                mx = 0; my = 0;
            }
            if((swdata & PAD_START)) {
                SMMA_MainLevel++;
            } 
            SCL_Open(SCL_NBG0);
            SCL_Move(mx, my, 0);
            SCL_Close();
            break;
    }
}


/*
 *
 *  SCROLL CG SET
 *
 */
void SMSC_Setcg2(Uint8 *cgadd)
{
     SMMI_Ldir8((Uint8 *)SMSC_B_1_info.cgadd, (Uint8 *)cgadd,
         (Uint32)SMSC_B_1_info.cgsize);
}


/*
 *
 *  SPRITE ACTION
 *
 */
void SMSC_opaact(SMSCACT *ix)
{
    enum {
        INIT,
        MAIN
    };

    Uint16 swdata;
    static Sint16 vrate;

    swdata = SMPA_pad_data1;
    switch(ix->level) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_rotzoom;
            ix->sprite.pmode = SPM_ecd;
            ix->sprite.patbase = opa_pat;
            ix->sprite.color = 0;
            ix->sprite.patno = 0;
            ix->sprite.patchgcnt = anime_time[0];
            ix->sprite.hzoom = 0x100;
            ix->sprite.vzoom = SMSC_ZMMAX;
            ix->sprite.zdirec = 0x000;
	    ix->sprite.xposi = 0x0a0*0x10000;
	    ix->sprite.yposi = 0x070*0x10000;
            ix->level++;
            vrate = SMSC_ZMMAX;
            /* NO BREAK */

        case MAIN:
            SMSC_patchg(ix);
            if((swdata & PAD_LEFT)) {
                ix->sprite.hzoom = 0x100;
            }
            if((swdata & PAD_RIGHT)) {
                ix->sprite.hzoom = -0x100;
            }

            if(((swdata & PAD_C) && (swdata & PAD_LEFT)) ||
                    ((swdata & PAD_C) && (swdata & PAD_RIGHT))) {
                if ((vrate > SMSC_ZMMIN))
                     vrate -= 4;
                else
                     vrate = SMSC_ZMMIN;
            }
            else {
                if ((vrate < SMSC_ZMMAX))
                     vrate += 4;
                else
                     vrate = SMSC_ZMMAX;
            }
            ix->sprite.vzoom = vrate;

            SMV1_PutSprite(&ix->sprite);
            break;
    }
}


/*
 *
 *  PATTERN CHANGE CONTROL
 *
 */
void SMSC_patchg(SMSCACT *ix)
{
    SPRITE *spr;
    spr = &ix->sprite;
    if (--(spr->patchgcnt)==0) {
        if (++(spr->patno) == 4) {
            spr->patno = 0; 
        }
        spr->patchgcnt = anime_time[spr->patno];
    }
}

/************ 多重スクロール ************/
/*
 *
 *  MULTI SCROLL MODE CONTROL
 *
 */
void SMSC_MultiScl(void)
{
    enum {
        SCL_INIT,
        SCL_MAIN,
        SCL_EXIT
    };

    SclVramConfig tp;
    SclConfig scfg;

    while(SMMA_MainMode == SMSC_MULTISCLMODE) {
        switch(SMMA_MainLevel) {
            case SCL_INIT:
                SMV2_TvOff();
                SMMA_initall();
                SMSC_IniMultiScr();
                SCL_SetFrameInterval(0xffff); /* HI-DOUKI mode    */

                /* スプライトフレームバッファのクリア */
                SMV1_SprCmdStart();
                SMV1_SprCmdEnd();
                SMMA_intwait();
                SMMA_intwait();

                /* プライオリティの初期化 */
                SCL_SetPriority(SCL_NBG0, 6);
                SCL_SetPriority(SCL_NBG1, 5);
                SCL_SetPriority(SCL_NBG2, 4);
                SCL_SetPriority(SCL_NBG3, 3);
                SCL_SetPriority(SCL_SP0, 0);

                /* ＶＲＡＭの使用方法の設定 */
                SCL_InitVramConfigTb(&tp);
                tp.vramModeA = OFF;
                tp.vramModeB = OFF;
                SCL_SetVramConfig(&tp);

                /* ASCII CGとカラーの転送 */
                SMV2_SetFont4((Uint8 *)SMSC_NBG_PTGEN, 1, 0);
                poke_w(COLOR_RAM + 0x00, 0x7000);

                poke_w(COLOR_RAM + 0x02, rgb(31,31,31));
                poke_w(COLOR_RAM + 0x22, rgb(28,28, 0));
                poke_w(COLOR_RAM + 0x42, rgb( 0,28,28));
                poke_w(COLOR_RAM + 0x62, rgb(28, 0,28));
#if 0
                poke_w(COLOR_RAM + 0x02, 0x6318);
                poke_w(COLOR_RAM + 0x22, 0x4210);
                poke_w(COLOR_RAM + 0x42, 0x318c);
                poke_w(COLOR_RAM + 0x62, 0x1084);
#endif

                /* スクロールポジションの初期化 */
                SCL_Open(SCL_NBG0);
                SCL_MoveTo(FIXED(0), FIXED(0),0);/* Home Position */
                SCL_Close();
                SCL_Open(SCL_NBG1);
                SCL_MoveTo(FIXED(0), FIXED(0),0);/* Home Position */
                SCL_Close();
                SCL_Open(SCL_NBG2);
                SCL_MoveTo(FIXED(0), FIXED(0),0);/* Home Position */
                SCL_Close();
                SCL_Open(SCL_NBG3);
                SCL_MoveTo(FIXED(0), FIXED(0),0);/* Home Position */
                SCL_Close();

                /* CG DATA FILL */
                SMV2_FilMem32((Uint32 *)SMSC_N0_PTN, 32*16,
                        (Uint32)('A'-' '));
                SMV2_FilMem32((Uint32 *)SMSC_N1_PTN, 32*16,
                        (Uint32)(('B'-' ') + 0x1000));
                SMV2_FilMem32((Uint32 *)SMSC_N2_PTN, 32*32,
                        (Uint32)(('C'-' ') + 0x2000));
                SMV2_FilMem32((Uint32 *)SMSC_N3_PTN, 32*64,
                        (Uint32)(('D'-' ') + 0x3000));

                /* 多重スクロールアクションセット */
                SMTA_MakeAction(SMSC_multiscl);

                SMV2_TvOn();
                SMMA_MainLevel++;
                /* NO BREAK */

            case SCL_MAIN:
                /* アクション実行 */
                SMTA_ActionLoop();

                /* V-BLANKインタラプト待ち*/
                SMMA_intwait();
                break;

            case SCL_EXIT:
                SMMA_MainMode = SMSC_SELECTMODE;
                SMMA_MainLevel = 0;
                break;
        }
    }
}


/*
 *
 *  MULTI SCROLL ACTION
 *
 */
void SMSC_multiscl(SMSCACT *ix)
{
    enum {
        INIT,
        MAIN
    };

    static Fixed32 mx0, my0, mx1, my1, mx2, my2, mx3, my3;
    Uint16 swdata;
    Uint16 wrdata;

    swdata = SMPA_pad_data1;
    switch(ix->level) {
        case INIT:
            mx0 = mx1 = mx2 = mx3 = FIXED(0);
            my0 = my1 = my2 = my3 = FIXED(0);
            ix->level++;
            /* NO BREAK */

        case MAIN:
            if((swdata & PAD_LEFT)) {
                mx0 = FIXED(SMSC_MLTSPD);
                my1 = FIXED(-(SMSC_MLTSPD - 0.5));
                mx2 = FIXED(-(SMSC_MLTSPD - 1.0));
                my3 = FIXED(SMSC_MLTSPD - 1.5);
                if((swdata & PAD_UP)) {
                    my0 = FIXED(SMSC_MLTSPD);
                    mx1 = FIXED(SMSC_MLTSPD - 0.5);
                    my2 = FIXED(-(SMSC_MLTSPD - 1.0));
                    mx3 = FIXED(-(SMSC_MLTSPD - 1.5));
                }
                else if((swdata & PAD_DOWN)) {
                    my0 = FIXED(-SMSC_MLTSPD);
                    mx1 = FIXED(-(SMSC_MLTSPD - 0.5));
                    my2 = FIXED((SMSC_MLTSPD - 1.0));
                    mx3 = FIXED((SMSC_MLTSPD - 1.5));
                }
                else {
                    my0 = 0;
                    mx1 = 0;
                    my2 = 0;
                    mx3 = 0;
                }
            }
            if((swdata & PAD_RIGHT)) {
                mx0 = FIXED(-SMSC_MLTSPD);
                my1 = FIXED((SMSC_MLTSPD - 0.5));
                mx2 = FIXED((SMSC_MLTSPD - 1.0));
                my3 = FIXED(-(SMSC_MLTSPD - 1.5));
                if((swdata & PAD_UP)) {
                    my0 = FIXED(SMSC_MLTSPD);
                    mx1 = FIXED(SMSC_MLTSPD - 0.5);
                    my2 = FIXED(-(SMSC_MLTSPD - 1.0));
                    mx3 = FIXED(-(SMSC_MLTSPD - 1.5));
                }
                else if((swdata & PAD_DOWN)) {
                    my0 = FIXED(-SMSC_MLTSPD);
                    mx1 = FIXED(-(SMSC_MLTSPD - 0.5));
                    my2 = FIXED((SMSC_MLTSPD - 1.0));
                    mx3 = FIXED((SMSC_MLTSPD - 1.5));
                }
                else {
                    my0 = 0;
                    mx1 = 0;
                    my2 = 0;
                    mx3 = 0;
                }
            }
            if((swdata & PAD_UP)) {
                my0 = FIXED(SMSC_MLTSPD);
                mx1 = FIXED(SMSC_MLTSPD - 0.5);
                my2 = FIXED(-(SMSC_MLTSPD - 1.0));
                mx3 = FIXED(-(SMSC_MLTSPD - 1.5));
                if((swdata & PAD_LEFT)) {
                    mx0 = FIXED(SMSC_MLTSPD);
                    my1 = FIXED(-(SMSC_MLTSPD - 0.5));
                    mx2 = FIXED(-(SMSC_MLTSPD - 1.0));
                    my3 = FIXED(SMSC_MLTSPD - 1.5);
                }
                else if((swdata & PAD_RIGHT)) {
                    mx0 = FIXED(-SMSC_MLTSPD);
                    my1 = FIXED((SMSC_MLTSPD - 0.5));
                    mx2 = FIXED((SMSC_MLTSPD - 1.0));
                    my3 = FIXED(-(SMSC_MLTSPD - 1.5));
                }
                else {
                    mx0 = 0;
                    my1 = 0;
                    mx2 = 0;
                    my3 = 0;
                }
            }
            if((swdata & PAD_DOWN)) {
                my0 = FIXED(-SMSC_MLTSPD);
                mx1 = FIXED(-(SMSC_MLTSPD - 0.5));
                my2 = FIXED((SMSC_MLTSPD - 1.0));
                mx3 = FIXED((SMSC_MLTSPD - 1.5));
                if((swdata & PAD_LEFT)) {
                    mx0 = FIXED(SMSC_MLTSPD);
                    my1 = FIXED(-(SMSC_MLTSPD - 0.5));
                    mx2 = FIXED(-(SMSC_MLTSPD - 1.0));
                    my3 = FIXED(SMSC_MLTSPD - 1.5);
                }
                else if((swdata & PAD_RIGHT)) {
                    mx0 = FIXED(-SMSC_MLTSPD);
                    my1 = FIXED((SMSC_MLTSPD - 0.5));
                    mx2 = FIXED((SMSC_MLTSPD - 1.0));
                    my3 = FIXED(-(SMSC_MLTSPD - 1.5));
                }
                else {
                    mx0 = 0;
                    my1 = 0;
                    mx2 = 0;
                    my3 = 0;
                }
            }
            if((swdata & PAD_A)) {
                mx1 = my1 = 0;
                mx2 = my2 = 0;
                mx3 = my3 = 0;
            }
            if((swdata & PAD_B)) {
                mx0 = my0 = 0;
                mx2 = my2 = 0;
                mx3 = my3 = 0;
            }
            if((swdata & PAD_C)) {
                mx0 = my0 = 0;
                mx1 = my1 = 0;
                mx3 = my3 = 0;
            }
            if((swdata & PAD_Z)) {
                mx0 = my0 = 0;
                mx1 = my1 = 0;
                mx2 = my2 = 0;
            }

            if((swdata & PAD_Y)) {
                mx0 = my0 = 0;
                mx1 = my1 = 0;
                mx2 = my2 = 0;
                mx3 = my3 = 0;
            }
            if((swdata & PAD_START)) {
                SMMA_MainLevel++;
            } 
            SCL_Open(SCL_NBG0);
            SCL_Move(mx0, my0, 0);
            SCL_Close();
            SCL_Open(SCL_NBG1);
            SCL_Move(mx1, my1, 0);
            SCL_Close();
            SCL_Open(SCL_NBG2);
            SCL_Move(mx2, my2, 0);
            SCL_Close();
            SCL_Open(SCL_NBG3);
            SCL_Move(mx3, my3, 0);
            SCL_Close();
            break;
    }
}

/*
 *
 *  SCROLL REGISTER INIT
 *
 */
void SMSC_IniMultiScr(void)
{
    /* SCROLL VRAM ACCESS CYCLE PATTERN TABLE */
    const Uint16 MULTI_CYC[] = {
        0x4567, 0xffff,    /* VRAM A(A0)     */
        0xffff, 0xffff,    /* VRAM A1 未使用 */
        0x0123, 0xffff,    /* VRAM B(B0)     */
        0xffff, 0xffff     /* VRAM B1 未使用 */
    };

    SclConfig scfg0, scfg1, scfg2, scfg3;

    SCL_Vdp2Init();
    SCL_SetDisplayMode(SCL_NON_INTER, SCL_224LINE, SCL_NORMAL_A);
    SCL_SetColRamMode(SCL_CRM15_2048);    /* 15bit color 2048               */

    /* NBG0 イニシャル */
    SCL_Open(SCL_NBG0);
    SCL_InitConfigTb(&scfg0);             /* NBG0 Scroll Config Set         */
    scfg0.dispenbl = ON;                  /* 表示 ON 1/OFF 0                */
    scfg0.charsize = SCL_CHAR_SIZE_1X1;   /*charsiz 0:1X1cel 1:2X2cel*/
    scfg0.pnamesize = SCL_PN1WORD;        /*patname 0:2word 1:1word  */
    scfg0.platesize = SCL_PL_SIZE_1X1;    /*plane size 0:h1Xv1 page  */
                                          /*1:h2Xv1page 3:h2Xv2page  */
    scfg0.bmpsize = SCL_BMP_SIZE_512X256; /* BIT MAP SIZ           */
    scfg0.coltype = SCL_COL_TYPE_16;      /* color mode 0:16 1:256 */
    scfg0.datatype = SCL_CELL;            /*scroll 0:CELL 1:BitMap */
    scfg0.flip = SCL_PN_12BIT;            /*補助モード10bit 12bit  */
    scfg0.mapover = SCL_OVER_0;           /*map over mode 繰り返し */
    scfg0.plate_addr[0] = SMSC_N0_PTN;    /*MAPA patnam add        */
    scfg0.plate_addr[1] = SMSC_N0_PTN;    /*MAPB patnam add        */
    scfg0.plate_addr[2] = SMSC_N0_PTN;    /*MAPC patnam add        */
    scfg0.plate_addr[3] = SMSC_N0_PTN;    /*MAPD patnam add        */
    SCL_SetConfig(SCL_NBG0, &scfg0);
    SCL_SetCycleTable(MULTI_CYC);    /* VRAM アクセスサイクルパターンセット */
    SCL_Close();

    /* NBG1 イニシャル */
    SCL_Open(SCL_NBG1);
    SCL_InitConfigTb(&scfg1);             /*NBG1 Scroll Config Set */
    scfg1.dispenbl = ON;                  
    scfg1.charsize = SCL_CHAR_SIZE_1X1;   
    scfg1.pnamesize = SCL_PN1WORD;        
    scfg1.platesize = SCL_PL_SIZE_1X1;    
    scfg1.bmpsize = SCL_BMP_SIZE_512X256; 
    scfg1.coltype = SCL_COL_TYPE_16;     
    scfg1.datatype = SCL_CELL;            
    scfg1.flip = SCL_PN_12BIT;            
    scfg1.mapover = SCL_OVER_0;           
    scfg1.plate_addr[0] = SMSC_N1_PTN;    
    scfg1.plate_addr[1] = SMSC_N1_PTN;    
    scfg1.plate_addr[2] = SMSC_N1_PTN;    
    scfg1.plate_addr[3] = SMSC_N1_PTN;    
    SCL_SetConfig(SCL_NBG1, &scfg1);
    SCL_Close();

    /* NBG2 イニシャル */
    SCL_Open(SCL_NBG2);
    SCL_InitConfigTb(&scfg2);             /*NBG2 Scroll Config Set */
    scfg2.dispenbl = ON;                  
    scfg2.charsize = SCL_CHAR_SIZE_1X1;   
    scfg2.pnamesize = SCL_PN1WORD;        
    scfg2.platesize = SCL_PL_SIZE_1X1;    
    scfg2.bmpsize = SCL_BMP_SIZE_512X256; 
    scfg2.coltype = SCL_COL_TYPE_16;     
    scfg2.datatype = SCL_CELL;            
    scfg2.flip = SCL_PN_12BIT;            
    scfg2.mapover = SCL_OVER_0;           
    scfg2.plate_addr[0] = SMSC_N2_PTN;    
    scfg2.plate_addr[1] = SMSC_N2_PTN;    
    scfg2.plate_addr[2] = SMSC_N2_PTN;    
    scfg2.plate_addr[3] = SMSC_N2_PTN;    
    SCL_SetConfig(SCL_NBG2, &scfg2);
    SCL_Close();

    /* NBG3 イニシャル */
    SCL_Open(SCL_NBG3);
    SCL_InitConfigTb(&scfg3);             /*NBG3 Scroll Config Set */
    scfg3.dispenbl = ON;                  
    scfg3.charsize = SCL_CHAR_SIZE_1X1;   
    scfg3.pnamesize = SCL_PN1WORD;        
    scfg3.platesize = SCL_PL_SIZE_1X1;    
    scfg3.bmpsize = SCL_BMP_SIZE_512X256; 
    scfg3.coltype = SCL_COL_TYPE_16;     
    scfg3.datatype = SCL_CELL;            
    scfg3.flip = SCL_PN_12BIT;            
    scfg3.mapover = SCL_OVER_0;           
    scfg3.plate_addr[0] = SMSC_N3_PTN;    
    scfg3.plate_addr[1] = SMSC_N3_PTN;    
    scfg3.plate_addr[2] = SMSC_N3_PTN;    
    scfg3.plate_addr[3] = SMSC_N3_PTN;    
    SCL_SetConfig(SCL_NBG3, &scfg3);
    SCL_Close();

    Scl_n_reg.linecolmode = 0x00000000 | ((LCLRTBLADD & 0xfffff) / 2);
    Scl_n_reg.backcolmode = 0x00000000 | ((BCLRTBLADD & 0xfffff) / 2);
    poke_w(BCLRTBLADD, 0x0000);

    SMV2_FilMem32((Uint32 *)SMSC_NBG_PTGEN,0x100 / sizeof (Uint32),0x00000000);
    SMV2_FilMem16((Uint16 *)SMSC_N0_PTN, 0x2000 / sizeof (Uint16), 0x2000);
    SMV2_FilMem16((Uint16 *)SMSC_N1_PTN, 0x2000 / sizeof (Uint16), 0x2000);
    SMV2_FilMem16((Uint16 *)SMSC_N2_PTN, 0x2000 / sizeof (Uint16), 0x2000);
    SMV2_FilMem16((Uint16 *)SMSC_N3_PTN, 0x2000 / sizeof (Uint16), 0x2000);
    SMV2_FilMem16((Uint16 *)COLOR_RAM, 0x800 / sizeof (Uint16), 0x0000);
}



/*
 *
 *  BYTE LDIR
 *
 */
void SMSC_Ldir8(Sint8 *src, Sint8 *dst, Sint32 len)
{
    Sint8 tmp1, tmp2;

    while (len--) {
        tmp1 = *src++;
        tmp2 = *src++;
        tmp1 <<= 4;
        tmp1 += tmp2;
        *dst++ = tmp1;
        VdpWait();
        VdpWait();
        VdpWait();
        VdpWait();
        VdpWait();
        VdpWait();
        VdpWait();
        VdpWait();
    }
}


/*
 *
 *  INITIAL ALL
 *
 */
void SMSC_initall(void)
{
    di();
    INT_ChgMsk(INT_MSK_NULL, INT_MSK_VBLK_IN | INT_MSK_VBLK_OUT);
    SCL_VblInit();                 /* V-BLANK割り込みライブラリワーク初期化 */
    SCL_SetFrameInterval(0xffff); /* HI-DOUKI mode                       */
    INT_SetScuFunc(INT_SCU_VBLK_IN , SMIN_VblankIn);  /* VINT-IN  関数登録 */
    INT_SetScuFunc(INT_SCU_VBLK_OUT, SMIN_VblankOut); /* VINT-OUT 関数登録 */
    SMSC_IniMultiScr();
    SMV1_IniSprite();
    SPR_2ClrAllChar();
    SMPA_SwInit();
    SMTA_ActWkInit();
    MTH_InitialRand(0x36b53b67);
    INT_ChgMsk(INT_MSK_VBLK_IN | INT_MSK_VBLK_OUT, INT_MSK_NULL);
    ei();
}


