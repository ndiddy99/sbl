/*	Copyright(c) 1994 SEGA			*/
/*
 *
 * sprite sample
 *
 */

#include "smp_bios.h"
#include "smp_slct.h"
#include "smp_spr.h"
#include "smp_spr2.h"


/* マクロ */

/* プロトタイプ宣言 */
static void SMSP_ModeSelAct(MDSELACT *);        /* セレクト画面 */
static void SMSP_TestAct1(SPACT *);             /* ポリゴン回転 */
static void SMSP_TestAct11(SPACT *);            /* ポリライン回転 */
void SMSP_TestAct12(SPACT *);            /* スプライト回転 */
static void SMSP_TestAct2(SPACT *);             /* 変形スプライト */
static void SMSP_3DAct1(SPACT *);               /* 立方体シェーディング */
static void SMSP_3DAct11(SPACT *);              /* シェーディングタイトル */
static void SMSP_3DAct2(SPACT *);               /* ３Ｄデモ */
static void SMSP_TestAct3(SPACT *);             /* ラインデモ */
static void SMSP_TestAct4(SPACT *);             /* シャドウデモ */
static void SMSP_sdtitle1(SPACT *);             /* シャドウタイトル１ */
static void SMSP_sdtitle2(SPACT *);             /* シャドウタイトル２ */
static void SMSP_shadow1(SPACT *);              /* シャドウスプライト１ */
static void SMSP_shadow2(SPACT *);              /* シャドウスプライト２ */
static void SMSP_TestAct5(SPACT *);             /* スプライトデモ */

/* スプライトデモ用 */
static void SMSP_OPA_ACT(SPACT *);
/* 出現パターン */
static void SMSP_OPA_ACT01(SPACT *);
static void SMSP_OPA_ACT11(SPACT *);
static void SMSP_OPA_ACT21(SPACT *);
static void SMSP_OPA_ACT31(SPACT *);
/* 移動パターン */
static void SMSP_OPA_ACT02(SPACT *);
static void SMSP_OPA_ACT12(SPACT *);
static void SMSP_OPA_ACT22(SPACT *);
static void SMSP_OPA_ACT32(SPACT *);
/* 消滅パターン */
static void SMSP_OPA_ACT03(SPACT *);
static void SMSP_OPA_ACT13(SPACT *);
static void SMSP_OPA_ACT23(SPACT *);
static void SMSP_OPA_ACT33(SPACT *);

/* その他サブルーチン */
void SMSP_Setcg1(Uint8 *);               /* スクロール面のＣＧセット */
void SMSP_spr_cgset(void);               /* スプライトのＣＧセット */
void SMSP_patchg(SPACT *);               /* スプライトのパターンチェンジ */
void SMSP_addxy(SPACT *,int);            /* 変形スプライトの座標変更 */
void SMSP_h_add(Sint16 *,Sint16 *);      /* ラインデモのＨ方向移動 */
void SMSP_v_add(Sint16 *,Sint16 *);      /* ラインデモのＶ方向移動 */
void Move_3DAct1(void);                  /* シェーディングデモの自転 */
void SMSP_SprCmdStart(void);             /* プライオリティありの初期設定 */
void local_coord_center(void);           /* ローカル座標の原点を中央にセット */
void local_coord_rtop(void);             /* ローカル座標の原点を右上にセット */


/* 定数テーブル */
const Uint8 SMSP_mes01[] = " 1. POLYGON TEST";
const Uint8 SMSP_mes02[] = " 2. POLYLINE TEST";
const Uint8 SMSP_mes03[] = " 3. TEXTURE TEST";
const Uint8 SMSP_mes04[] = " 4. HENKEI TEST";
const Uint8 SMSP_mes05[] = " 5. LINE TEST";
const Uint8 SMSP_mes06[] = " 6. SHADOW TEST";
const Uint8 SMSP_mes07[] = " 7. SPRITE TEST";
const Uint8 SMSP_mes08[] = " 8. SHADING TEST";
const Uint8 SMSP_mes09[] = " 9. 3D TEST";
const Uint8 SMSP_mes10[] = "10. < EXIT >";

    enum {
        SL_INIT,
        SL_LOOP,
        SL_DEMO1,
        SL_DEMO2,
        SL_DEMO3,
        SL_DEMO4,
        SL_DEMO5,
        SL_DEMO6,
        SL_DEMO7,
        SL_DEMO8,
        SL_DEMO9,
        SL_DEMO_EXIT
        };

const MENU_INFO spmenu[] = {
          SMSP_mes01, SL_DEMO1,
          SMSP_mes02, SL_DEMO2,
          SMSP_mes03, SL_DEMO3,
          SMSP_mes04, SL_DEMO4,
          SMSP_mes05, SL_DEMO5,
          SMSP_mes06, SL_DEMO6,
          SMSP_mes07, SL_DEMO7,
          SMSP_mes08, SL_DEMO8,
          SMSP_mes09, SL_DEMO9,
          SMSP_mes10, SL_DEMO_EXIT,
          };
/* ３Ｄワーク設定 */

#define DrawPolMax     64          /* 最大表示ポリゴン数 */
#define ObjSurfMax     64          /* ３Ｄオブジェクトの最大面数 */
#define ObjVertMax     64          /* ３Ｄオブジェクトの最大頂点数 */
/* 3D Work Define */
SPR_3DefineWork(work3D, ObjSurfMax, ObjVertMax)
                                   /* ３Ｄワークエリア定義 */

/* 各スプライトサンプルアクション */
void (* const func_list[])(SPACT *) = {
        SMSP_TestAct1,  	/* DUMMY */
        SMSP_TestAct1,		/* polygon demo */
        SMSP_TestAct11,		/* polyline demo */
        SMSP_TestAct12,		/* texture demo */
        SMSP_TestAct2,		/* henkei demo */
        SMSP_TestAct3,		/* line demo */
        SMSP_TestAct4,		/* shadow demo */
        SMSP_TestAct5,		/* texture demo */
        SMSP_3DAct1,		/* shading demo */
        SMSP_3DAct2,		/* 3D demo */
    };

/*===========================================================================*/
/*
 *  SPRITE SAMPLE MAIN
 */
void SMSP_DemoMode(void)
{

    enum {
        SP_SAMPLE_INIT,
        SP_SAMPLE_MAIN,
        SP_SAMPLE_NEXT,
        SP_SAMPLE_END
      };

    SMSP_SelectLevel = 0;

    while (SMMA_MainMode == SPSAMPLE_MODE) {
        SMSP_SprCmdStart();

        switch (SMMA_MainLevel) {
            case SP_SAMPLE_INIT:
                SMV2_TvOff();
                SMMA_initall();
                SCL_SetPriority(SCL_SP0, 7);
                SCL_SetPriority(SCL_SP1, 6);
                SCL_SetPriority(SCL_NBG0, 6);
                SCL_SetPriority(SCL_RBG0, 5);

                SCL_SetColRamMode(SCL_CRM15_2048);
		                                    /* 15bit 2048 color mode */
               SMV2_TvOff(); 
               SMV1_SprCmdEnd();
               SMMA_intwait();
 
                SCL_SetSpriteMode(SCL_TYPE3,SCL_MIX,SCL_SP_WINDOW);
                SCL_SetColMixMode(7,SCL_IF_BEHIND);
                SCL_AbortAutoVe();
                SMV2_SetFont8((Uint8 *)NBG_PTGEN,1,0);  /* VDP2 FONT SET */
                SMV1_SetFont4sp(0, 1, 0);               /* VDP1 FONT SET */
                SMSP_spr_cgset();                   /* VDP1 CG SET */

                poke_w(COLOR_RAM + 0x00, 0x0000);
                poke_w(COLOR_RAM + 0x02, 0x739c);
            SMSP_Setcg1((Uint8 *)(SCR_VRAMA0+0x1000));
            if (SMSP_SelectLevel==0)
	        SMTA_MakeAction(SMSP_ModeSelAct);
            else
                SMTA_MakeAction(func_list[SMSP_SelectLevel]);

                SMV2_TvOn();            
    	        SMMA_MainLevel++;
                break;
            case SP_SAMPLE_MAIN:
                if (SMPA_pad_edge1 & PAD_START)
                    SMMA_MainLevel++ ;
                SMTA_ActionLoop();
                break;
            case SP_SAMPLE_NEXT:
                SMMA_MainMode = SPSAMPLE_MODE;
                SMMA_MainLevel = 0;
                SMSP_SelectLevel = 0; 
                break;
            case SP_SAMPLE_END:
                SMMA_MainMode = SELECT_MODE;
                SMMA_MainLevel = 0;
                break;
        }
        SMV1_SprCmdEnd();
        SMMA_intwait();
    }
}

/*--------------------------------------------------------------------------*/
/* スプライトＤＥＭＯセレクト */
void SMSP_ModeSelAct(MDSELACT *ix)
{

    Uint8 i;
    Uint8 *wrdata8;
    Uint16 *wraddbase, *wradd;
    Uint16 swdata;
    static Sint32 zmrate;
    MENU_INFO *work;


    swdata = SMPA_pad_edge1;

    switch (ix->mode) {
        case SL_INIT:

            ix->sel.menuptr = spmenu;
            ix->sel.wcnt[0] = sizeof spmenu / (sizeof (struct menu_info));
	                                         /* MENU LIST COUNT */
            ix->sel.wcnt[1] = 0;                 /* CURSOR POINTER  */
            ix->sel.wcnt[2] = 0x10;              /* MOZAIC INITIAL HV COUNT */
            ix->sel.wcnt[3] = 0;                 /* MOZAIC DISPLAY TIMER    */
            ix->sel.wreg[0] = ix->sel.wcnt[0]-1; /* MENU LIST NUMBER */
            ix->sel.mode = 0;                    /* INITIAL */

            SMSL_SlctTblSet(ix);
            ix->mode++;
            break;

        case SL_LOOP:
            SMSL_SlctAct(ix);
            break;

        case SL_DEMO1:
	    SMMA_MainLevel = 0;
	    SMSP_SelectLevel = 1;
            break;

        case SL_DEMO2:
	    SMMA_MainLevel = 0;
	    SMSP_SelectLevel = 2;
            break;

        case SL_DEMO3:
	    SMMA_MainLevel = 0;
	    SMSP_SelectLevel = 3;
            break;

        case SL_DEMO4:
	    SMMA_MainLevel = 0;
	    SMSP_SelectLevel = 4;
            break;

        case SL_DEMO5:
	    SMMA_MainLevel = 0;
	    SMSP_SelectLevel = 5;
            break;

        case SL_DEMO6:
	    SMMA_MainLevel = 0;
	    SMSP_SelectLevel = 6;
            break;

        case SL_DEMO7:
	    SMMA_MainLevel = 0;
	    SMSP_SelectLevel = 7;
            break;

        case SL_DEMO8:
	    SMMA_MainLevel = 0;
	    SMSP_SelectLevel = 8;
            break;

        case SL_DEMO9:
	    SMMA_MainLevel = 0;
	    SMSP_SelectLevel = 9;
            break;

        case SL_DEMO_EXIT:
            SMMA_MainMode = SELECT_MODE;
	    SMMA_MainLevel = 0;
	    SMSP_SelectLevel = 0;
            break;
    }
}

/*---------------------------------------------------------------------------*/
/* POLYGON DATA */

/*  枚数,
 *   color, offs_ax, offs_ay, offs_bx, offs_by,
 *          offs_cx, offs_cy, offs_dx, offs_dy
 */
    const Sint16 poly_pat0[] = {
        1,
        rgb(0, 0,24), -32,-32, 32,-32, 32, 32,-32, 32
    };
    const Sint16 poly_pat1[] = {
        2,
        rgb(24, 0,24),-40,   8,  40,   8,   0, -20,   0, -20,
        rgb(24, 0,24),  0,  36, -24, -36,   0, -20,  24, -36,
    };

    const Sint16 poly_pat2[] = {
        1,
        rgb(24,24, 0),  0,-37, 32, 18, 0, 18,-32, 18
    };

    const Sint16 poly_pat3[] = {
        4,
        rgb(24, 0, 0),  0,  0, -9, 16,  0, 32,  9, 16,
        rgb(24, 0, 0),  0,  0,-16,  9,-32,  0,-16, -9,
        rgb(24, 0, 0),  0,  0, -9,-16,  0,-32,  9,-16,
        rgb(24, 0, 0),  0,  0, 16,  9, 32,  0, 16, -9,
    };

    Sint16 * const poly_pat[] = {
        poly_pat0,
        poly_pat1,
        poly_pat2,
        poly_pat3,
    };
		
/*---------------------------------------------------------------------------*/
/* TEXTURE DATA */
/*
    枚数,
    オフセット, Ｈサイズ, Ｖサイズ, Ｈオフセット, Ｖオフセット
 */
#define OPA1_charno 0x51
#define MU1_charno  0x54
#define DR1_charno  0x57
#define CO1_charno  0x5a

    const Sint16 opa_pat0[] = {
        1,OPA1_charno+0,0, 32, 32, -16, -16,
    };
    const Sint16 opa_pat1[] = {
        1,OPA1_charno+1,0, 32, 32, -16, -16,
    };
    const Sint16 opa_pat2[] = {
        1,OPA1_charno+2,0, 32, 32, -16, -16,
    };

    Sint16 * const opa_pat[] = {
        opa_pat0,
        opa_pat1,
        opa_pat2,
        opa_pat1,
    };

    const Sint16 mu_pat0[] = {
        1,MU1_charno+0,0, 32, 32, -16, -16,
    };
    const Sint16 mu_pat1[] = {
        1,MU1_charno+1,0, 32, 32, -16, -16,
    };
    const Sint16 mu_pat2[] = {
        1,MU1_charno+2,0, 32, 32, -16, -16,
    };

    Sint16  * const mu_pat[] = {
        mu_pat0,
        mu_pat1,
        mu_pat2,
        mu_pat1,
    };

    const Sint16 dr_pat0[] = {
        1,DR1_charno+0,0, 96, 64, -48, -32,
    };
    const Sint16 dr_pat1[] = {
        1,DR1_charno+1,0, 96, 64, -48, -32,
    };
    const Sint16 dr_pat2[] = {
        1,DR1_charno+2,0, 96, 64, -48, -32,
    };

    Sint16 * const dr_pat[] = {
        dr_pat0,
        dr_pat1,
        dr_pat2,
        dr_pat1,
    };

    const Sint16 co_pat0[] = {
        1,CO1_charno+0,0, 32, 32, -16, -16,
    };
    const Sint16 co_pat1[] = {
        1,CO1_charno+1,0, 16, 32, -8, -16,
    };

    Sint16 * const co_pat[] = {
        co_pat0,
        co_pat1,
        co_pat0,
        co_pat1
    };

    Sint16 * const sprrot_pat[] = {
        opa_pat0,
        mu_pat0,
        dr_pat0,
        co_pat0
    };

    const Sint16 anime_time[] = {
        10,
        10,
        10,
        10,
    };

    Sint16 ** const spr_pat[] = {
        opa_pat,
        mu_pat,
        dr_pat,
        co_pat
    };

/*  枚数,
    Charno, HVreverse, Ｈサイズ, Ｖサイズ, Ｈオフセット, Ｖオフセット */
    static const Sint16 nosh_pat[] = {
            10,
            ('N' - ' ') , 0, 0x0008, 0x0008,   -40,  -4,
            ('O' - ' ') , 0, 0x0008, 0x0008,   -32,  -4,
            (' ' - ' ') , 0, 0x0008, 0x0008,   -24,  -4,
            ('S' - ' ') , 0, 0x0008, 0x0008,   -16,  -4,
            ('H' - ' ') , 0, 0x0008, 0x0008,    -8,  -4,
            ('A' - ' ') , 0, 0x0008, 0x0008,     0,  -4,
            ('D' - ' ') , 0, 0x0008, 0x0008,     8,  -4,
            ('I' - ' ') , 0, 0x0008, 0x0008,    16,  -4,
            ('N' - ' ') , 0, 0x0008, 0x0008,    24,  -4,
            ('G' - ' ') , 0, 0x0008, 0x0008,    32,  -4,
        };

/*  枚数,
    Charno, HVreverse, Ｈサイズ, Ｖサイズ, Ｈオフセット, Ｖオフセット */
    static const Sint16 fltsh_pat[] = {
            12,
            ('F' - ' ') , 0, 0x0008, 0x0008,   -48,  -4,
            ('L' - ' ') , 0, 0x0008, 0x0008,   -40,  -4,
            ('A' - ' ') , 0, 0x0008, 0x0008,   -32,  -4,
            ('T' - ' ') , 0, 0x0008, 0x0008,   -24,  -4,
            (' ' - ' ') , 0, 0x0008, 0x0008,   -16,  -4,
            ('S' - ' ') , 0, 0x0008, 0x0008,    -8,  -4,
            ('H' - ' ') , 0, 0x0008, 0x0008,     0,  -4,
            ('A' - ' ') , 0, 0x0008, 0x0008,     8,  -4,
            ('D' - ' ') , 0, 0x0008, 0x0008,    16,  -4,
            ('I' - ' ') , 0, 0x0008, 0x0008,    24,  -4,
            ('N' - ' ') , 0, 0x0008, 0x0008,    32,  -4,
            ('G' - ' ') , 0, 0x0008, 0x0008,    40,  -4,
        };

/*  枚数,
    Charno, HVreverse, Ｈサイズ, Ｖサイズ, Ｈオフセット, Ｖオフセット */
    static const Sint16 goush_pat[] = {
            15,
            ('G' - ' ') , 0, 0x0008, 0x0008,   -60,  -4,
            ('O' - ' ') , 0, 0x0008, 0x0008,   -52,  -4,
            ('U' - ' ') , 0, 0x0008, 0x0008,   -44,  -4,
            ('R' - ' ') , 0, 0x0008, 0x0008,   -36,  -4,
            ('A' - ' ') , 0, 0x0008, 0x0008,   -28,  -4,
            ('U' - ' ') , 0, 0x0008, 0x0008,   -20,  -4,
            ('D' - ' ') , 0, 0x0008, 0x0008,   -12,  -4,
            (' ' - ' ') , 0, 0x0008, 0x0008,    -4,  -4,
            ('S' - ' ') , 0, 0x0008, 0x0008,     4,  -4,
            ('H' - ' ') , 0, 0x0008, 0x0008,    12,  -4,
            ('A' - ' ') , 0, 0x0008, 0x0008,    20,  -4,
            ('D' - ' ') , 0, 0x0008, 0x0008,    28,  -4,
            ('I' - ' ') , 0, 0x0008, 0x0008,    36,  -4,
            ('N' - ' ') , 0, 0x0008, 0x0008,    44,  -4,
            ('G' - ' ') , 0, 0x0008, 0x0008,    52,  -4,
        };

    Sint16 * const shtitle_pat[] = {
        nosh_pat,
        fltsh_pat,
        goush_pat
    };

/* sprite type 3 : normal shadow polygon data */
    const Sint16 sha_pat0[] = {
        1,
        0x07fe, -48,-48, 48,-48, 48, 48,-48, 48
    };
    Sint16 * const sha_pat[] = {
        sha_pat0,
    };

    static const Sint16 nmlsd_pat[] = {
            13,
            ('N' - ' ') , 0, 0x0008, 0x0008,   -52,  -4,
            ('O' - ' ') , 0, 0x0008, 0x0008,   -44,  -4,
            ('R' - ' ') , 0, 0x0008, 0x0008,   -36,  -4,
            ('M' - ' ') , 0, 0x0008, 0x0008,   -28,  -4,
            ('A' - ' ') , 0, 0x0008, 0x0008,   -20,  -4,
            ('L' - ' ') , 0, 0x0008, 0x0008,   -12,  -4,
            (' ' - ' ') , 0, 0x0008, 0x0008,    -4,  -4,
            ('S' - ' ') , 0, 0x0008, 0x0008,     4,  -4,
            ('H' - ' ') , 0, 0x0008, 0x0008,    12,  -4,
            ('A' - ' ') , 0, 0x0008, 0x0008,    20,  -4,
            ('D' - ' ') , 0, 0x0008, 0x0008,    28,  -4,
            ('O' - ' ') , 0, 0x0008, 0x0008,    36,  -4,
            ('W' - ' ') , 0, 0x0008, 0x0008,    44,  -4,
        };
    static const Sint16 msbsd_pat[] = {
            10,
            ('M' - ' ') , 0, 0x0008, 0x0008,   -40,  -4,
            ('S' - ' ') , 0, 0x0008, 0x0008,   -32,  -4,
            ('B' - ' ') , 0, 0x0008, 0x0008,   -24,  -4,
            (' ' - ' ') , 0, 0x0008, 0x0008,   -16,  -4,
            ('S' - ' ') , 0, 0x0008, 0x0008,    -8,  -4,
            ('H' - ' ') , 0, 0x0008, 0x0008,     0,  -4,
            ('A' - ' ') , 0, 0x0008, 0x0008,     8,  -4,
            ('D' - ' ') , 0, 0x0008, 0x0008,    16,  -4,
            ('O' - ' ') , 0, 0x0008, 0x0008,    24,  -4,
            ('W' - ' ') , 0, 0x0008, 0x0008,    32,  -4,
        };
    static const Sint16 sprsd_pat[] = {
            6,
            ('S' - ' ') , 0, 0x0008, 0x0008,   -24,  -4,
            ('P' - ' ') , 0, 0x0008, 0x0008,   -16,  -4,
            ('R' - ' ') , 0, 0x0008, 0x0008,    -8,  -4,
            ('I' - ' ') , 0, 0x0008, 0x0008,     0,  -4,
            ('T' - ' ') , 0, 0x0008, 0x0008,     8,  -4,
            ('E' - ' ') , 0, 0x0008, 0x0008,    16,  -4,
        };
    Sint16 * const sdtitle_pat[] = {
        nmlsd_pat,
        msbsd_pat,
        sprsd_pat
    };


/*---------------------------------------------------------------------------*/
void SMSP_patchg(SPACT *ix)
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

/*---------------------------------------------------------------------------*/
/*
 *    POLYGON DEMO
 */
static void SMSP_TestAct1(SPACT *ix)
{
    enum {
        INIT,
        MAIN,
        NEXT
        };

    Sint16 hvzoom,diradd;

    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_rzpolygon;
            ix->sprite.pmode = SPM_ecd | SPM_sdd;
            ix->sprite.patbase = poly_pat;
            ix->sprite.patno = 0;
            ix->sprite.patchgcnt = 0;
            ix->sprite.hzoom = 0x100;
            ix->sprite.vzoom = 0x100;
            ix->sprite.zdirec = 0x000;
	    ix->sprite.xposi = 0x0a0*0x10000;
	    ix->sprite.yposi = 0x070*0x10000;
            ix->wreg[0] = 0;

            SMV2_TvOn();
            ix->mode++;
        case MAIN:
            hvzoom = ix->sprite.hzoom;
            diradd = ix->wreg[0];

            if (SMPA_pad_data1 & PAD_UP) {
                hvzoom -= 4;
                if (hvzoom < 0x000) hvzoom = 0x000;
            }
            if (SMPA_pad_data1 & PAD_DOWN) {
                hvzoom += 4;
                if (hvzoom > 0x400) hvzoom = 0x400;
            }
            if (SMPA_pad_data1 & PAD_LEFT) {
                diradd -= 1;
                if (diradd < -0x010) diradd = -0x010;
            }
            if (SMPA_pad_data1 & PAD_RIGHT) {
                diradd += 1;
                if (diradd > 0x010) diradd = 0x010;
            }
            if (SMPA_pad_edge1 & PAD_B) {
                hvzoom = 0x100;
                diradd = 0x000;
                ix->sprite.zdirec = 0;
            }
            ix->sprite.hzoom = hvzoom;
            ix->sprite.vzoom = hvzoom;
            ix->wreg[0] = diradd;
            ix->sprite.zdirec += diradd;

            if (SMPA_pad_edge1 & PAD_C) {
                ix->sprite.patno += 1;
                ix->sprite.patno &= 3;
            }

            SMV1_PutSprite(&ix->sprite);
            break;
    }
}

/*---------------------------------------------------------------------------*/
/*
 *    POLYLINE DEMO
 */
static void SMSP_TestAct11(SPACT *ix)
{
    enum {
        INIT,
        MAIN,
        NEXT
        };

    Sint16 hvzoom,diradd;

    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_rzpolyline;
            ix->sprite.pmode = SPM_ecd | SPM_sdd;
            ix->sprite.patbase = poly_pat;
            ix->sprite.patno = 0;
            ix->sprite.patchgcnt = 0;
            ix->sprite.hzoom = 0x100;
            ix->sprite.vzoom = 0x100;
            ix->sprite.zdirec = 0x000;
	        ix->sprite.xposi = 0x0a0*0x10000;
	        ix->sprite.yposi = 0x070*0x10000;
            ix->wreg[0] = 0;

            SMV2_TvOn();
            ix->mode++;
        case MAIN:
            hvzoom = ix->sprite.hzoom;
            diradd = ix->wreg[0];

            if (SMPA_pad_data1 & PAD_UP) {
                hvzoom -= 4;
                if (hvzoom < 0x000) hvzoom = 0x000;
            }
            if (SMPA_pad_data1 & PAD_DOWN) {
                hvzoom += 4;
                if (hvzoom > 0x400) hvzoom = 0x400;
            }
            if (SMPA_pad_data1 & PAD_LEFT) {
                diradd -= 1;
                if (diradd < -0x010) diradd = -0x010;
            }
            if (SMPA_pad_data1 & PAD_RIGHT) {
                diradd += 1;
                if (diradd > 0x010) diradd = 0x010;
            }
            if (SMPA_pad_edge1 & PAD_B) {
                hvzoom = 0x100;
                diradd = 0x000;
                ix->sprite.zdirec = 0;
            }
            ix->sprite.hzoom = hvzoom;
            ix->sprite.vzoom = hvzoom;
            ix->wreg[0] = diradd;
            ix->sprite.zdirec += diradd;

            if (SMPA_pad_edge1 & PAD_C) {
                ix->sprite.patno += 1;
                ix->sprite.patno &= 3;
            }

            SMV1_PutSprite(&ix->sprite);
            break;
    }
}


/*---------------------------------------------------------------------------*/
/*
 *    TEXTURE DEMO
 */
void SMSP_TestAct12(SPACT *ix)
{
    enum {
        INIT,
        MAIN,
        NEXT
        };

    Sint16 hvzoom,diradd;

    switch (ix->mode) {
        case INIT:

            ix->sprite.putmode = PMD_sprx_rotzoom;
            ix->sprite.pmode = SPM_ecd;
            ix->sprite.patbase = spr_pat[0];
            ix->sprite.patno = 0;
            ix->sprite.patchgcnt = anime_time[0];
            ix->sprite.color = 0x0010;
            ix->sprite.hzoom = 0x100;
            ix->sprite.vzoom = 0x100;
            ix->sprite.zdirec = 0x000;
	        ix->sprite.xposi = 0x0a0*0x10000;
	        ix->sprite.yposi = 0x070*0x10000;
            ix->wreg[0] = 0;           /* diradd save */
            ix->wreg[1] = 0;           /* patern table save */

            SMV2_TvOn();
            ix->mode++;
        case MAIN:
            hvzoom = ix->sprite.hzoom;
            diradd = ix->wreg[0];

            if (SMPA_pad_data1 & PAD_UP) {
                hvzoom -= 4;
                if (hvzoom < 0x000) hvzoom = 0x000;
            }
            if (SMPA_pad_data1 & PAD_DOWN) {
                hvzoom += 4;
                if (hvzoom > 0x400) hvzoom = 0x400;
            }
            if (SMPA_pad_data1 & PAD_LEFT) {
                diradd -= 1;
                if (diradd < -0x010) diradd = -0x010;
            }
            if (SMPA_pad_data1 & PAD_RIGHT) {
                diradd += 1;
                if (diradd > 0x010) diradd = 0x010;
            }
            if (SMPA_pad_edge1 & PAD_B) {
                hvzoom = 0x100;
                diradd = 0x000;
                ix->sprite.zdirec = 0;
            }
            ix->sprite.hzoom = hvzoom;
            ix->sprite.vzoom = hvzoom;
            ix->wreg[0] = diradd;
            ix->sprite.zdirec += diradd;

            if (SMPA_pad_edge1 & PAD_C) {
		        ix->wreg[1] += 1;
		        ix->wreg[1] &= 3;
                ix->sprite.patbase = spr_pat[ix->wreg[1]];
            }

	        SMSP_patchg(ix);
            SMV1_PutSprite(&ix->sprite);
            break;
    }
}

/*---------------------------------------------------------------------------*/
/*
 *    HENKEI DEMO
 */
void SMSP_addxy(SPACT *ix,int i)
{
	if ((SMPA_pad_data1 & PAD_LEFT) != NO) {
	    ix->sprite.posi[i][H]-=4;
	}
	if ((SMPA_pad_data1 & PAD_RIGHT) != NO) {
	    ix->sprite.posi[i][H]+=4;
	}
	if ((SMPA_pad_data1 & PAD_UP) != NO) {
	    ix->sprite.posi[i][V]-=4;
	}
	if ((SMPA_pad_data1 & PAD_DOWN) != NO) {
	    ix->sprite.posi[i][V]+=4;
	}

}

static void SMSP_TestAct2(SPACT *ix)
{
    enum {
        INIT,
        MAIN,
        NEXT
        };

/* ４点ＸＹ座標初期設定 */
    Sint16 init_posi[4][2] = {
        0x0a0-24,0x070-16,
	0x0a0+24,0x070-16,
	0x0a0+24,0x070+16,
	0x0a0-24,0x070+16
    };
    Uint8 i;

    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_4pset;
	        for (i=0;i<4;i++){
                ix->sprite.posi[i][H] = init_posi[i][H];
                ix->sprite.posi[i][V] = init_posi[i][V];
            }
	        ix->sprite.pmode = SPM_ecd;
            ix->sprite.patbase = dr_pat;
            ix->sprite.patno = 0;
            ix->sprite.patchgcnt = anime_time[0];
            ix->sprite.color = 0x0010;
            ix->sprite.hzoom = 0x100;
            ix->sprite.vzoom = 0x100;
            ix->sprite.zdirec = 0x000;
	        ix->sprite.xposi = 0x0a0*0x10000;
	        ix->sprite.yposi = 0x070*0x10000;
            ix->wreg[0] = 0;           /* diradd save */
            ix->wreg[1] = 0;           /* patern table save */

            SMV2_TvOn();
            ix->mode++;
        case MAIN:
	        if (SMPA_pad_data1 & PAD_A) SMSP_addxy(ix,1);
            else
	        if (SMPA_pad_data1 & PAD_B) SMSP_addxy(ix,2);
            else
	        if (SMPA_pad_data1 & PAD_C) SMSP_addxy(ix,3);
            else {
	            SMSP_addxy(ix,0);
	            SMSP_addxy(ix,1);
	            SMSP_addxy(ix,2);
	            SMSP_addxy(ix,3);
	        }

	    SMSP_patchg(ix);
        SMV1_PutSprite(&ix->sprite);
        break;
    }
}

/*---------------------------------------------------------------------------*/
/*
 *    LINE DEMO
 */
static void SMSP_TestAct3(SPACT *ix)
{
#define LINE_WAIT  1
#define LINE_NUM  30

    enum {
        INIT,
	WAIT1,
        SET,
	WAIT2,
        NEXT
        };

    SPACT *iy;
    Sint16 color;
    Uint8 i;

    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_line;
            ix->sprite.pmode = SPM_ecd | SPM_sdd;
            ix->sprite.color = rgb(0,0,31);
	    ix->sprite.posi[0][H] = 0x080;            /* posi */
	    ix->sprite.posi[0][V] = 0x070;
	    ix->sprite.posi[1][H] = 0x0c0;
	    ix->sprite.posi[1][V] = 0x070;
	    ix->sprite.posi[2][H] = -2;              /* speed */
	    ix->sprite.posi[2][V] = -1;
	    ix->sprite.posi[3][H] =  1;
	    ix->sprite.posi[3][V] =  2;
	    SMV2_TvOn();
	    ix->wreg[0] = LINE_WAIT;                 /* delete timer */
            ix->mode++;
        case WAIT1:
           if (--(ix->wreg[0])==0)
                ix->mode++;
            SMV1_PutSprite(&ix->sprite);
	    break;
        case SET:
/* 次のラインをセット */
            iy = (SPACT *)SMTA_MakeAction(SMSP_TestAct3);
            iy->sprite.putmode = PMD_sprx_line;
            iy->sprite.pmode = SPM_ecd | SPM_sdd;
            iy->sprite.color = 0xbdef;

	    color = ix->sprite.color;
	    if (((color & 0x001f) ==0) && ((color & 0x7c00) != 0)){
	        color = color-0x0400+0x0020;
	    }else if (((color & 0x7c00) ==0) && ((color & 0x03e0) != 0)){
	        color = color-0x0020+0x0001;
	    }else if (((color & 0x03e0) ==0) && ((color & 0x001f) != 0)){
	        color = color-0x0001+0x0400;
	    }
	    iy->sprite.color = color;
	    for (i=0;i<4;i++){
	        iy->sprite.posi[i][H] = ix->sprite.posi[i][H];
	        iy->sprite.posi[i][V] = ix->sprite.posi[i][V];
	    }
	    SMSP_h_add(&(iy->sprite.posi[0][H]),&(iy->sprite.posi[2][H]));
	    SMSP_v_add(&(iy->sprite.posi[0][V]),&(iy->sprite.posi[2][V]));
	    SMSP_h_add(&(iy->sprite.posi[1][H]),&(iy->sprite.posi[3][H]));
	    SMSP_v_add(&(iy->sprite.posi[1][V]),&(iy->sprite.posi[3][V]));
	    iy->wreg[0] = LINE_WAIT;
            iy->mode = WAIT1;

	    ix->wreg[0] = LINE_WAIT*LINE_NUM;
            ix->mode++;

            SMV1_PutSprite(&ix->sprite);
            break;
        case WAIT2:
           if (--(ix->wreg[0])==0)
                ix->actid = 0;				/* kill act */
            SMV1_PutSprite(&ix->sprite);
	    break;
    }
}

/*---------------------------------------------------------------------------*/
void SMSP_h_add(Sint16 *posi,Sint16 *spd)
{
    *posi += *spd;
    if ((*posi) < 0) {
	*posi = -(*posi);
	*spd = -(*spd);
    }
    if ((*posi) > 320) {
	*posi = 320*2-(*posi);
	*spd = -(*spd);
    }
}
void SMSP_v_add(Sint16 *posi,Sint16 *spd)
{
    *posi += *spd;
    if ((*posi) < 0) {
	*posi = -(*posi);
	*spd = -(*spd);
    }
    if ((*posi) > 224) {
        *posi = 224*2-(*posi);
        *spd = -(*spd);
    }
}
/*---------------------------------------------------------------------------*/
/*
 *     SHADOW DEMO
 */

static void SMSP_TestAct4(SPACT *ix)
{
    SPACT *iy;

    enum {
        INIT,
        MAIN,
        NEXT
        };

    switch (ix->mode) {
        case INIT:
            SCL_SetSpriteMode(SCL_TYPE3,SCL_PALETTE,SCL_MSB_SHADOW);
            SMMI_Ldir16((Uint16 *)SMSP_testa_info.clradd,
	                (Uint16 *)COLOR_RAM,16);
            SMV2_BoxWrite16((Uint16 *)NBG0_PTNAMA,
	                  SMSP_testa_info.mapadd,40,28);

            ix->sprite.putmode = PMD_sprx_rotzoom;
            ix->sprite.pmode = SPM_ecd;
            ix->sprite.patbase = opa_pat;
            ix->sprite.color = 0x2810;
            ix->sprite.patno = 0;
            ix->sprite.patchgcnt = anime_time[0];
            ix->sprite.hzoom = 0x100;
            ix->sprite.vzoom = 0x100;
            ix->sprite.zdirec = 0x000;
	    ix->sprite.xposi = 0x050*0x10000;
	    ix->sprite.yposi = 0x0a0*0x10000;
	    ix->wcnt[0] = 0;                   /* シャドウポリゴン用 */
	    ix->wcnt[1] = 0;                   /* 敵スプライト用　*/
            iy = (SPACT *)SMTA_MakeAction(SMSP_shadow1);
	    iy->chlnk = ix;
            iy = (SPACT *)SMTA_MakeAction(SMSP_shadow2);
	    iy->chlnk = ix;
            iy = (SPACT *)SMTA_MakeAction(SMSP_sdtitle1);
	    iy->chlnk = ix;
            iy = (SPACT *)SMTA_MakeAction(SMSP_sdtitle2);
	    iy->chlnk = ix;

            SMV2_TvOn();

            ix->mode++;
        case MAIN:
            SMSP_patchg(ix);
            if (SMPA_pad_data1 & PAD_UP) {
                ix->sprite.yposi -= 2*0x10000;
            }
            if (SMPA_pad_data1 & PAD_DOWN) {
                ix->sprite.yposi += 2*0x10000;
            }
            if (SMPA_pad_data1 & PAD_LEFT) {
                ix->sprite.hzoom = 0x100;
                ix->sprite.xposi -= 2*0x10000;
            }
            if (SMPA_pad_data1 & PAD_RIGHT) {
                ix->sprite.hzoom = -0x100;
                ix->sprite.xposi += 2*0x10000;
            }
            if (SMPA_pad_edge1 & PAD_A) {
                SCL_SetAutoColMix(SCL_SP1,5,5*32,31,0);  /* fade in opa */
            }
            if (SMPA_pad_edge1 & PAD_C) {
                SCL_SetAutoColMix(SCL_SP1,5,5*32,0,31);  /* fade out opa */
            }
            if (SMPA_pad_edge1 & PAD_X) {
                SCL_SetAutoColMix(SCL_SP0,5,5*32,31,0);  /* fade in enemy */
            }
            if (SMPA_pad_edge1 & PAD_Z) {
                SCL_SetAutoColMix(SCL_SP0,5,5*32,0,31);  /* fade out enemy */
            }
            if (SMPA_pad_edge1 & PAD_B) {
                ix->wcnt[0] ^= 1;                        /* polygon shade */
            }
            if (SMPA_pad_edge1 & PAD_Y) {
                ix->wcnt[1] ^= 1;                        /* enemy shade */
            }
            poke_w(SCRL_SPRCNT + 0x02, 0x0101);          /* shadow on */
            SMV1_PutSprite(&ix->sprite);
            break;
    }
}

static void SMSP_shadow1(SPACT *ix)
{
    enum {
        INIT,
        MAIN,
        NEXT
    };
    SPACT *iy;

    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_rzpolygon;
            ix->sprite.pmode = SPM_ecd | SPM_sdd;
            ix->sprite.patbase = sha_pat;
            ix->sprite.patno = 0;
            ix->sprite.patchgcnt = 0;
            ix->sprite.hzoom = 0x100;
            ix->sprite.vzoom = 0x100;
            ix->sprite.zdirec = 0x000;
	    ix->sprite.xposi = 0x0a0*0x10000;
	    ix->sprite.yposi = 0x070*0x10000;

            ix->mode++;
        case MAIN:
            ix->sprite.zdirec += 2;
            iy = ix->chlnk;
	    if (iy->wcnt[0]==0) {
                ix->sprite.pmode = SPM_ecd | SPM_sdd;    /* flat shadow */
            } else {
                ix->sprite.pmode = SPM_ecd | SPM_sdd | SPM_mon; /* MSBshadow */
            }
	    SMV1_PutSprite(&ix->sprite);
            break;
        case NEXT:
            break;
    }
}

static void SMSP_shadow2(SPACT *ix)
{
    enum {
        INIT,
        MAIN,
        NEXT
    };

    SPACT *iy;

    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_rotzoom;
            ix->sprite.pmode = SPM_ecd;
            ix->sprite.patbase = dr_pat;
            ix->sprite.color = 0x0010;
            ix->sprite.patno = 0;
            ix->sprite.patchgcnt = anime_time[0];
            ix->sprite.hzoom = 0x100;
            ix->sprite.vzoom = 0x100;
            ix->sprite.zdirec = 0x000;
	    ix->sprite.xposi = 0x0d0*0x10000;
	    ix->sprite.yposi = 0x0a0*0x10000;

            SMV2_TvOn();
            ix->mode++;
        case MAIN:
	    SMSP_patchg(ix);
            iy = ix->chlnk;
	    if (iy->wcnt[1]==0) {
                ix->sprite.pmode = SPM_ecd;                 /* normal sprite */
            } else {
                ix->sprite.pmode = SPM_ecd | SPM_mon;       /* MSB shadow */
            }
            SMV1_PutSprite(&ix->sprite);
            break;
        case NEXT:
            break;
    }
}

/*---------------------------------------------------------------------------*/
/*
 *    SHADOW TITLE
 */
static void SMSP_sdtitle1(SPACT *ix)
{
    enum {
        INIT,
        MAIN,
        NEXT
        };

    SPACT *iy;

    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_nrm;
            ix->sprite.pmode = SPM_ecd;
            ix->sprite.patbase = sdtitle_pat;
            ix->sprite.patno = 0;
            ix->sprite.patchgcnt = 0;
            ix->sprite.color = 0x0010;
	    ix->sprite.xposi = 0x0a0*0x10000;
	    ix->sprite.yposi = 0x030*0x10000;
	    ix->mode++;
        case MAIN:
            iy = ix->chlnk;
	    if (iy->wcnt[0]==0) {
                ix->sprite.patno = 0;    /* flat shadow */
            } else {
                ix->sprite.patno = 1;    /* MSB shadow */
            }
                SMV1_PutSprite(&ix->sprite);
	    break;
    }
}

static void SMSP_sdtitle2(SPACT *ix)
{
    enum {
        INIT,
        MAIN,
        NEXT
        };
    SPACT *iy;

    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_nrm;
            ix->sprite.pmode = SPM_ecd;
            ix->sprite.patbase = sdtitle_pat;
            ix->sprite.patno = 0;
            ix->sprite.patchgcnt = 0;
            ix->sprite.color = 0x0010;
	    ix->sprite.xposi = 0x0d0*0x10000;
	    ix->sprite.yposi = 0x0c0*0x10000;
	    ix->mode++;
        case MAIN:
            iy = ix->chlnk;
	    if (iy->wcnt[1]==0) {
                ix->sprite.patno = 2;    /* sprite */
            } else {
                ix->sprite.patno = 1;    /* MSB shadow */
            }
                SMV1_PutSprite(&ix->sprite);
	    break;
    }
}

/*---------------------------------------------------------------------------*/
/*
 *    TEXTURE TEST
 */

static void SMSP_TestAct5(SPACT *ix)
{

    enum {
        INIT,
        MAIN,
        SET_ACT
        };

    SPRITE *spr;

    switch (ix->mode) {

        case INIT:
            SMV2_TvOn();   
            ix->sprite.patchgcnt = 1;
            ix->mode++;
        case MAIN:
            spr = &ix->sprite;
            if (--(spr->patchgcnt)==0) {
                spr->patchgcnt = 60;             /* set timer */
	        ix->mode++;
	    }
	    break;
	case SET_ACT:
            SMTA_MakeAction(SMSP_OPA_ACT);
	    ix->mode = MAIN;
	    break;
    }
}

static void SMSP_OPA_ACT(SPACT *ix)
{

void (* const opa_start[])(SPACT *) = {
        SMSP_OPA_ACT01,
        SMSP_OPA_ACT11,
        SMSP_OPA_ACT21,
        SMSP_OPA_ACT31,
    };

void (* const opa_move[])(SPACT *) = {
        SMSP_OPA_ACT02,
        SMSP_OPA_ACT12,
        SMSP_OPA_ACT22,
        SMSP_OPA_ACT32,
    };

void (* const opa_end[])(SPACT *) = {
        SMSP_OPA_ACT03,
        SMSP_OPA_ACT13,
        SMSP_OPA_ACT23,
        SMSP_OPA_ACT33,
    };

    enum {
        INIT,
        MAIN_1,
        MAIN_2,
        MAIN_3,
        NEXT
    };

    switch (ix->mode) {
        Sint32 tmp;
	Uint8 inttmp;

        case INIT:
            ix->sprite.putmode = PMD_sprx_rotzoom;
            ix->sprite.pmode = SPM_ecd;
	    tmp = MTH_GetRand();
	    tmp /= 16;
            ix->sprite.patbase = spr_pat[tmp & 3];
	    tmp /= 16;
            ix->wcnt[0] = tmp & 3;
	    tmp /= 16;
            ix->wcnt[1] = tmp & 3;
	    tmp /= 16;
            ix->wcnt[2] = tmp & 3;
            ix->sprite.color = 0x0010;
            ix->sprite.patno = 0;
            ix->sprite.patchgcnt = anime_time[0];
            ix->sprite.hzoom = 0x100;
            ix->sprite.vzoom = 0x100;
            ix->sprite.zdirec = 0x000;

            tmp = MTH_GetRand();
	    ix->sprite.xposi = (tmp & 0x0ff0000)+0x020*0x10000;
            tmp = MTH_GetRand();
	    ix->sprite.yposi = (tmp & 0x07f0000)+0x030*0x10000;
            ix->sprite.xspd = 0;
            ix->sprite.yspd = 0;
            ix->wreg[0] = 30;
            ix->mode++;
        case MAIN_1:
            if (--(ix->wreg[0])==0) {
                ix->wreg[0] = 130;
	        ix->mode++;
            }else {
                opa_start[ix->wcnt[0]](ix);
            }
            SMV1_PutSprite(&ix->sprite);
            break;
        case MAIN_2:
            if (--(ix->wreg[0])==0){
	        ix->wreg[0] = 30;
                ix->mode++;
            } else {
                opa_move[ix->wcnt[1]](ix);
                SMSP_patchg(ix);
            }
            SMV1_PutSprite(&ix->sprite);
            break;
        case MAIN_3:
           if (--(ix->wreg[0])==0)
                ix->actid = 0;				/* kill act */
                opa_end[ix->wcnt[2]](ix);
            SMV1_PutSprite(&ix->sprite);
            break;
    }
}

/*-------------------------------------------*/
static void SMSP_OPA_ACT01(SPACT *ix)
{
    SPRITE *spr;
    spr = &ix->sprite;
    spr->hzoom = 0x100 / (ix->wreg[0]);
    spr->vzoom = 0x100 * (ix->wreg[0]);
}

static void SMSP_OPA_ACT02(SPACT *ix)
{
    SPRITE *spr;
    spr = &ix->sprite;
    spr->xposi += 0x002*0x10000;
}

static void SMSP_OPA_ACT03(SPACT *ix)
{
    SPRITE *spr;
    spr = &ix->sprite;
    spr->hzoom = 0x100 / (30-ix->wreg[0]);
    spr->vzoom = 0x100 * (30-ix->wreg[0]);
}

/*-------------------------------------------*/
static void SMSP_OPA_ACT11(SPACT *ix)
{
    SPRITE *spr;
    spr = &ix->sprite;
    spr->zdirec = (ix->wreg[0]-1) * 12;
    spr->hzoom = 0x080 + 0x080 / (ix->wreg[0]);
    spr->vzoom = 0x080 + 0x080 / (ix->wreg[0]);
}

static void SMSP_OPA_ACT12(SPACT *ix)
{
    SPRITE *spr;
    spr = &ix->sprite;
    spr->yspd += 0x040;
    if (spr->yspd > 0x800) spr->yspd = 0x800;
    spr->yposi += spr->yspd*0x100;
    if (spr->yposi > 0xc0*0x10000) {
        spr->yposi = 0xc0*0x10000;
        spr->yspd = -spr->yspd;
    }
}

static void SMSP_OPA_ACT13(SPACT *ix)
{
    SPRITE *spr;
    spr = &ix->sprite;
    spr->zdirec = (30-ix->wreg[0]) * 12;
    spr->hzoom = 0x080 + 0x080 / (30-ix->wreg[0]);
    spr->vzoom = 0x080 + 0x080 / (30-ix->wreg[0]);
}
/*-------------------------------------------*/
static void SMSP_OPA_ACT21(SPACT *ix)
{
    SPRITE *spr;
    spr = &ix->sprite;
    spr->zdirec = (ix->wreg[0]-1) * 12;
    spr->hzoom = 0x080 + 0x080 * (ix->wreg[0]);
    spr->vzoom = 0x080 + 0x080 / (ix->wreg[0]);
}

static void SMSP_OPA_ACT22(SPACT *ix)
{
    SPRITE *spr;
    spr = &ix->sprite;
    spr->zdirec += 0x010;
}

static void SMSP_OPA_ACT23(SPACT *ix)
{
    SPRITE *spr;
    spr = &ix->sprite;
    spr->zdirec = (30-ix->wreg[0]) * 12;
    spr->hzoom = 0x080 + 0x080 / (30-ix->wreg[0]);
    spr->vzoom = 0x080 + 0x080 * (30-ix->wreg[0]);
}
/*-------------------------------------------*/
static void SMSP_OPA_ACT31(SPACT *ix)
{
    SPRITE *spr;
    spr = &ix->sprite;
    spr->hzoom = 0x100 * (ix->wreg[0]);
    spr->vzoom = 0x100 / (ix->wreg[0]);
    SMSP_patchg(ix);
}

static void SMSP_OPA_ACT32(SPACT *ix)
{
    SPRITE *spr;
    spr = &ix->sprite;
    spr->xposi -= 0x002*0x10000;
    SMSP_patchg(ix);
}

static void SMSP_OPA_ACT33(SPACT *ix)
{
    SPRITE *spr;
    spr = &ix->sprite;
    spr->hzoom = 0x100 * (30-ix->wreg[0]);
    spr->vzoom = 0x100 / (30-ix->wreg[0]);
}

/*-------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*
 *    SHADING DEMO
 */
static void SMSP_3DAct1(SPACT *ix)
{

    MthXyz rot_r = {MTH_FIXED(0),MTH_FIXED(2),MTH_FIXED(0)};
    MthXyz rot_l = {MTH_FIXED(0),MTH_FIXED(-2),MTH_FIXED(0)};

    enum {
         INIT,
         MAIN,
         TURN_LEFT,
	 TURN_RIGHT
         };

    SPACT *iy;

    switch (ix->mode) {
        case INIT:
            SPR_3Initial(&work3D);
            SPR_3SetTexture((SprTexture *)&SMSP_Texture);
					/* ３Ｄ用テクスチャデータのセット */
            SPR_3SetClipLevel(3,MTH_FIXED(-16.0),MTH_FIXED(-4.0));
	    SPR_3SetPixelCount(256,256);

            SPR_3SetLight(1,&SMSP_LightXYZ);
            SPR_3SetView(1,&SMSP_ViewPoint,
                                &SMSP_ViewAngle,ROT_SEQ_ZYX, 0);
            SPR_3MoveCluster(&SMSP_rootclu,1,&SMSP_null_xyz,&SMSP_init_point);

            iy = (SPACT *)SMTA_MakeAction(SMSP_3DAct11);
	    ix->chlnk = iy;
            SMV2_TvOn();
            ix->mode++;
        case MAIN:

            if (SMPA_pad_data1 & PAD_LEFT) {
		ix->wcnt[0] = 60;
                ix->mode = TURN_LEFT;
		iy = ix->chlnk;
                iy->wreg[0] = 1;                     /* title no disp */
            }
            if (SMPA_pad_data1 & PAD_RIGHT) {
		ix->wcnt[0] = 60;
                ix->mode = TURN_RIGHT;
		iy = ix->chlnk;
                iy->wreg[0] = 1;                     /* title no disp */
            }
	    Move_3DAct1();
            break;

        case TURN_LEFT:
	    if (--ix->wcnt[0]==0) {
		ix->mode = MAIN;
		iy = ix->chlnk;
                iy->wreg[0] = 0;                     /* title disp */
		if (--iy->sprite.patno==0xff) {
		    iy->sprite.patno = 2;
		}
            }
            SPR_3MoveCluster(&SMSP_rootclu,0,&rot_l,&SMSP_null_xyz);
	    Move_3DAct1();
            break;

        case TURN_RIGHT:
	    if (--ix->wcnt[0]==0) {
		ix->mode = MAIN;
		iy = ix->chlnk;
                iy->wreg[0] = 0;                     /* title disp */
		if (++iy->sprite.patno==3) {
		    iy->sprite.patno = 0;
		}
	    }
            SPR_3MoveCluster(&SMSP_rootclu,0,&rot_r,&SMSP_null_xyz);
	    Move_3DAct1();
            break;
    }
}

void Move_3DAct1()
{
    SPR_3MoveCluster(&SMSP_test1clu,0,&SMSP_rot_angle,&SMSP_null_xyz);
    SPR_3MoveCluster(&SMSP_test2clu,0,&SMSP_rot_angle,&SMSP_null_xyz);
    SPR_3MoveCluster(&SMSP_test3clu,0,&SMSP_rot_angle,&SMSP_null_xyz);

    local_coord_center();
    SPR_3DrawModel(&SMSP_rootclu);
    SPR_3Flush();
}
/*---------------------------------------------------------------------------*/
/*
 *    SHADING TITLE
 */
static void SMSP_3DAct11(SPACT *ix)
{
    enum {
        INIT,
        MAIN,
        NEXT
        };

    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_nrm;
            ix->sprite.pmode = SPM_ecd;
            ix->sprite.patbase = shtitle_pat;
            ix->sprite.patno = 0;
            ix->sprite.patchgcnt = 0;
            ix->sprite.color = 0x0010;
	    ix->sprite.xposi = 0x000*0x10000;
	    ix->sprite.yposi = 0x050*0x10000;
            ix->wreg[0] = 0;                        /* disp flag */
	    ix->mode++;
        case MAIN:
	    if (ix->wreg[0]==0) {
                SMV1_PutSprite(&ix->sprite);
            }
	    break;
    }
}

/*---------------------------------------------------------------------------*/
/*
 *    3D DEMO
 */
static void SMSP_3DAct2(SPACT *ix)
{

#define Z_MOVE_MAX 9.5

    enum {
         INIT,
         MAIN,
         NEXT
         };

    static MthXyz rot;
    static MthXyz move;

    switch (ix->mode) {
        case INIT:
            SPR_3Initial(&work3D);
            SPR_3SetTexture((SprTexture *)&SMSP_Texture);
					/* ３Ｄ用テクスチャデータのセット */

            SPR_3SetClipLevel(3,MTH_FIXED(-20),MTH_FIXED(-0.1));
	    SPR_3SetPixelCount(256,256);
            SPR_3SetLight(1,&SMSP_LightXYZ);
            SPR_3SetView(1,&SMSP_ViewPoint,
                          &SMSP_ViewAngle,ROT_SEQ_ZYX, 0);
            SPR_3MoveCluster(&SMSP_root_clu2,
	                     1,&SMSP_null_xyz,&SMSP_init_point);
            rot = SMSP_null_xyz;
            move = SMSP_null_xyz;

            SMV2_TvOn();
            ix->mode++;
        case MAIN:
            local_coord_center();

            if (SMPA_pad_data1 & PAD_UP) {
                rot.x -= MTH_FIXED(2.0);
                if (rot.x < MTH_FIXED(-180))
		    rot.x += MTH_FIXED(360);
            }
            if (SMPA_pad_data1 & PAD_DOWN) {
                rot.x += MTH_FIXED(2.0);
                if (rot.x > MTH_FIXED(180))
		    rot.x -= MTH_FIXED(360);
            }
            if (SMPA_pad_data1 & PAD_LEFT) {
                rot.z -= MTH_FIXED(2.0);
                if (rot.z < MTH_FIXED(-180))
		    rot.z += MTH_FIXED(360);
            }
            if (SMPA_pad_data1 & PAD_RIGHT) {
                rot.z += MTH_FIXED(2.0);
                if (rot.z > MTH_FIXED(180))
		    rot.z -= MTH_FIXED(360);
            }
            if (SMPA_pad_data1 & PAD_A) {
                move.z -= MTH_FIXED(0.1);
                if (move.z < MTH_FIXED(-Z_MOVE_MAX))
		    move.z = MTH_FIXED(-Z_MOVE_MAX);
	    }
            if (SMPA_pad_data1 & PAD_C) {
                move.z += MTH_FIXED(0.1);
                if (move.z > MTH_FIXED(Z_MOVE_MAX))
		    move.z = MTH_FIXED(Z_MOVE_MAX);
            }
            SPR_3MoveCluster(&SMSP_root_clu2,1,&rot,&move);

            if (SMPA_pad_edge1 & PAD_B) {
                rot = SMSP_null_xyz;
                move = SMSP_null_xyz;
	    };

            SPR_3DrawModel(&SMSP_root_clu2);
            SPR_3Flush();
            break;
    }
}
/*---------------------------------------------------------------------------*/

/*-------------------------------
 *    SPRITE COMMAND START SET
 *-------------------------------*/
void SMSP_SprCmdStart(void)
{
     XyInt reg_xy;
     XyInt pos_xy[4];
     Uint16 pmode, color, growno;

     SPR_2OpenCommand(SPR_2DRAW_PRTY_ON);     /* SPRITE COMMAND WRITE SETUP */
       
     reg_xy.x = WIDTH_H -1;
     reg_xy.y = WIDTH_V -1;
     SPR_2SysClip(SPR_2MOST_FAR, &reg_xy);   /* SYSTEM CLIPPING REG SET */

     reg_xy.x = 0;
     reg_xy.y = 0;
     SPR_2LocalCoord(SPR_2MOST_FAR, &reg_xy);   /* SPRITE LOCAL POINT SET */

     
     pmode = SPM_ecd + SPM_sdd;                /* FB ERASE        */
     color = 0x0000;
     growno = 0xffff;                          /* not use grow */
     pos_xy[0].x = 0;
     pos_xy[0].y = 0;
     pos_xy[1].x = WIDTH_H - 1;
     pos_xy[1].y = 0;
     pos_xy[2].x = WIDTH_H - 1;
     pos_xy[2].y = WIDTH_V - 1;
     pos_xy[3].x = 0;
     pos_xy[3].y = WIDTH_V - 1;
     SPR_2Polygon(SPR_2MOST_FAR, pmode, color, &pos_xy[0], growno);


}

void local_coord_center(void)
{
     XyInt reg_xy;
     
     reg_xy.x = WIDTH_H/2 ;
     reg_xy.y = WIDTH_V/2 ;
     SPR_2LocalCoord(SPR_2MOST_FAR, &reg_xy);   /* SPRITE LOCAL POINT SET */
}

void local_coord_rtop(void)
{
     XyInt reg_xy;

     reg_xy.x = 0;
     reg_xy.y = 0;
     SPR_2LocalCoord(SPR_2MOST_FAR, &reg_xy);  /* SPRITE LOCAL POINT SET */
}

/*--------------------------------------------------------------------------*/

/* scroll cg set */

void SMSP_Setcg1(Uint8 *cgadd)
{
     SMMI_Ldir8((Uint8 *)SMSP_testa_info.cgadd, (Uint8 *)cgadd,
                (Uint32)SMSP_testa_info.cgsize);
}

/* sprite cg set */
void SMSP_spr_cgset(void)
{
    SPR_2SetChar(OPA1_charno+0, 0, 0, 16,16, (Uint8 *)SMSP_OPA1_cg);
    SPR_2SetChar(OPA1_charno+1, 0, 0, 16,16, (Uint8 *)SMSP_OPA2_cg);
    SPR_2SetChar(OPA1_charno+2, 0, 0, 16,16, (Uint8 *)SMSP_OPA3_cg);
    SPR_2SetChar(MU1_charno+0, 0, 0, 16,16, (Uint8 *)SMSP_MU1_cg);
    SPR_2SetChar(MU1_charno+1, 0, 0, 16,16, (Uint8 *)SMSP_MU2_cg);
    SPR_2SetChar(MU1_charno+2, 0, 0, 16,16, (Uint8 *)SMSP_MU3_cg);
    SPR_2SetChar(DR1_charno+0, 0, 0, 48,32, (Uint8 *)SMSP_DR1_cg);
    SPR_2SetChar(DR1_charno+1, 0, 0, 48,32, (Uint8 *)SMSP_DR2_cg);
    SPR_2SetChar(DR1_charno+2, 0, 0, 48,32, (Uint8 *)SMSP_DR3_cg);
    SPR_2SetChar(CO1_charno+0, 0, 0, 16,16, (Uint8 *)SMSP_COIN5_cg);
    SPR_2SetChar(CO1_charno+1, 0, 0, 8,16, (Uint8 *)SMSP_COIN55_cg);
    SMMI_Ldir16((Uint16 *)SMSP_OPA1_cl, (Uint16 *)(COLOR_RAM+0x20),0x10);
}

