/*	Copyright(c) 1994 SEGA			*/
/*
 *
 *     MODE SELECT MODE
 *
 */

#include "smp_bios.h"

#include "smp_slcg.h"
#include "smp_slct.h"


/*
 *    関数プロトタイプ
 */
void SMSL_ModeSelAct(MDSELACT *ix);
void csrers(Sint8 posi);
void csrset(Sint8 posi);
void SLCT_setmosaic(Uint16 msrate);
void SMSL_LogoAct(MDSELACT *ix);
static void SMSL_cgset(void);

/*
 *    スプライト キャラクタナンバ アサイン
 */
#define SMSL_logocharNo 0

/*
 *    外部参照
 */
extern SclSysreg Scl_s_reg;
extern SclWinscl Scl_w_reg;

/*
 *    メニューデータ
 */
const Uint8 select_title[] = "---- SELECT MODE ----";

const Uint8 message1[] = " 1. SCROLL SAMPLE ";
const Uint8 message2[] = " 2. SPRITE SAMPLE";
const Uint8 message3[] = " 3. WINDOW SAMPLE";
const Uint8 message4[] = " 4. GAME SAMPLE";
const Uint8 message5[] = " 5. 2/14 DEMO";
const Uint8 message6[] = " 6. < EXIT >";


/* <<<< スプライトパターンデータ >>>> */
/*
    枚数, CharNo, HVreverse, Ｈサイズ, Ｖサイズ, Ｈオフセット, Ｖオフセット
 */
const Sint16 SMSL_logo_pat0[] = {
        1, SMSL_logocharNo, 0, 0x0060, 0x0020, -0x30, -0x10,
    };
Sint16 * const SMSL_logo_pattbl[] = {
        SMSL_logo_pat0,
    };


    enum {
        SL_INIT,
        SL_LOOP,
        SL_SCROLL,
        SL_SPSAMPLE,
        SL_WINDOW,
        SL_GAME,
        SL_D214,
        SL_EXIT,
        SL_NEXT
        };

const MENU_INFO menulist[] = {
          message1, SL_SCROLL,
          message2, SL_SPSAMPLE,
          message3, SL_WINDOW,
          message4, SL_GAME,
          message5, SL_D214,
          message6, SL_EXIT,
          };

/*==========================================================================*/

/*
 *
 *  MODE SELECT MAIN
 *
 */
void SMSL_ModeSel(void)
{
    enum {
        INIT,
        MAIN,
        NEXT
      };


    while (SMMA_MainMode == SELECT_MODE) {
        /* スプライト表示準備 */
        SMV1_SprCmdStart();

        switch (SMMA_MainLevel) {
            case INIT:
                SMV2_TvOff();
                SMMA_initall();

                /* プライオリティのセット */
                SCL_SetPriority(SCL_SP0, 4);
                SCL_SetPriority(SCL_NBG0, 6);
                SCL_SetPriority(SCL_NBG1, 5);

                /* カラーモードの設定 */
                SCL_SetColRamMode(SCL_CRM15_1024);  /* 15bit 2048 color mode */

                /* スプライトフレームバッファのクリア */
                SMV2_TvOff();
                SMV1_SprCmdStart();
                SMV1_SprCmdEnd();
                SMMA_intwait();
                SMMA_intwait();

                SMV2_FilMem16((Uint16 *)SCR_VRAMA1,
                        (0x40 * 0x40 / sizeof (Uint16)), 0x0000);

                /* モードセレクトアクションセット */
                SMTA_MakeAction(SMSL_ModeSelAct);

                /* セガロゴアクションセット */
                SMTA_MakeAction(SMSL_LogoAct);

                SMSL_cgset();
                SMV2_PutAsc8((Uint16 *)(NBG0_PTNAMA + 10 * 2 + 2 * 0x80),
                        select_title);

                SCL_SET_EXCCEN(1);              /* 拡張カラー演算イネーブル */
                SCL_SET_CCMD(1);                /* カラー演算モード */

                /* カラー演算イネーブル N1, SPRITE */ 
                SCL_SET_SPCCEN(1);
                SCL_SET_N1CCEN(1);

                /* カラーセット */
                poke_w(COLOR_RAM + 0, 0x7000);
                poke_w(COLOR_RAM + 2, 0x739c);

                SMV2_TvOn();
                SMMA_MainLevel++;
                /* NO BREAK */

            case MAIN:
                /* アクション実行 */
                SMTA_ActionLoop();
                break;

            case NEXT:
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
 *  MODE SELECT CONTROL ACTION
 *
 */
void SMSL_ModeSelAct(MDSELACT *ix)
{

    Uint8 i;
    Uint8 *wrdata8;
    Uint16 *wraddbase, *wradd;
    Uint16 swdata;
    MENU_INFO *work;

    swdata = SMPA_pad_edge1;

    switch (ix->mode) {
        case SL_INIT:
            ix->sel.menuptr = menulist;
            ix->sel.wcnt[0] = sizeof menulist / (sizeof (struct menu_info));
                                                         /* MENU LIST COUNT */
            ix->sel.wcnt[1] = 0;                         /* CURSOR POINTER  */
            ix->sel.wcnt[2] = 0x10;              /* MOZAIC INITIAL HV COUNT */
            ix->sel.wcnt[3] = 0;                 /* MOZAIC DISPLAY TIMER    */
            ix->sel.wreg[0] = ix->sel.wcnt[0] - 1;      /* MENU LIST NUMBER */
            ix->sel.mode = 0;                           /* INITIAL */

            /* MODE SELECT TABLE SET */
            SMSL_SlctTblSet(ix);
            ix->mode++;
            break;

        case SL_LOOP:
            /* MODE SELECT ACTION */
            SMSL_SlctAct(ix);
            break;

        case SL_SCROLL:
            SMV2_TvOff();
            SMMA_MainMode = SMSC_SELECTMODE;
            SMMA_MainLevel = 0;
            break;

        case SL_SPSAMPLE:
            SMV2_TvOff();
            SMMA_MainMode = SPSAMPLE_MODE;
            SMMA_MainLevel = 0;
            break;

        case SL_WINDOW:
            SMV2_TvOff();
            SMMA_MainMode = WIND_MODE;
            SMMA_MainLevel = 0;
            break;

        case SL_GAME:
            SMV2_TvOff();
            SMMA_MainMode = GAME_MODE;
            SMMA_MainLevel = 0;
            break;

        case SL_D214:
            SMV2_TvOff();
            SMMA_MainMode = DEMO214_MODE;
            SMMA_MainLevel = 0;
            break;

        case SL_EXIT:
            SMV2_TvOff();
            SMMA_MainMode = LOGO_MODE;
            SMMA_MainLevel = 0;
            break;

        case SL_NEXT:
            csrers(ix->sel.wcnt[1]);
            ix->mode = SL_INIT;
            break;
    }
}


/*
 *
 *  CURSOR ERASE
 *
 */
void csrers(Sint8 posi)
{
    Uint16 *wradd;

    wradd = (Uint16 *)(NBG0_PTNAMA + 3 * 2 + 4 * 0x80);    /* HEAD POSITION */
    wradd += (posi * 0x80);
    *wradd = 0x00;
}


/*
 *
 *  CURSOR WRITE
 *
 */
void csrset(Sint8 posi)
{
    Uint16 *wradd;

    wradd = (Uint16 *)(NBG0_PTNAMA + 3 * 2 + 4 * 0x80);    /* HEAD POSITION */
    wradd += (posi * 0x80);
    *wradd = 0x1e * 2;
}


/*
 *
 *  MOZAIC REGISTER SET
 *
 */
void SLCT_setmosaic(Uint16 msrate)
{
    msrate = msrate & 0xf;
    SCL_SetMosaic(SCL_NBG0 | SCL_NBG1, msrate, msrate);
}


/*
 *
 *  MODE SELECT TABLE SET
 *
 */
void SMSL_SlctTblSet(MDSELACT *ix)
{
    Uint8 i;
    Uint8 *wrdata8;
    Uint16 *wraddbase, *wradd;
    MENU_INFO *work;

    wraddbase = (Uint16 *)(NBG0_PTNAMA + 4 * 2 + 4 * 0x80);
                                             /* BASE + HPOSI*2 + VPOSI*0x80 */
    work = ix->sel.menuptr;
    for (i = ix->sel.wcnt[0]; i; i--) {
        wradd = wraddbase;
        wrdata8 = work->msgadd;
        SMV2_PutAsc8(wradd, wrdata8);
        work++;
        wraddbase += 0x80;
    }
    csrset(ix->sel.wcnt[1]);
    SLCT_setmosaic(ix->sel.wcnt[2] / 4);        /* MOSAIC SET */
}


/*
 *
 *    MODE SELECT ACTION
 *
 */
void SMSL_SlctAct(MDSELACT *ix)
{

    enum {
        ZOOMIN,
        SELECT,
        ZOOMOUT,
        NEXT
        };

    Uint16 swdata;
    MENU_INFO *work;

    swdata = SMPA_pad_edge1;

    switch (ix->sel.mode) {
        case ZOOMIN:
            if (--ix->sel.wcnt[2] == 0) {
                ix->sel.mode++;
            }
            SLCT_setmosaic(ix->sel.wcnt[2] / 4);           /* MOSAIC SET */
            break;
 
       case SELECT:
            if (swdata & PAD_C) {
                work = ix->sel.menuptr + ix->sel.wcnt[1];
                ix->sel.mode++;
            }
            if (swdata & PAD_DOWN) {
                csrers(ix->sel.wcnt[1]);
                ix->sel.wcnt[1]++;
                if (ix->sel.wcnt[1] > ix->sel.wreg[0])
                    ix->sel.wcnt[1] = 0;
                csrset(ix->sel.wcnt[1]);
            }
            if (swdata & PAD_UP) {
                csrers(ix->sel.wcnt[1]);
                ix->sel.wcnt[1]--;
                if (ix->sel.wcnt[1] < 0)
                    ix->sel.wcnt[1] = ix->sel.wreg[0];
                csrset(ix->sel.wcnt[1]);
            }
            break;

        case ZOOMOUT:
            work = ix->sel.menuptr + ix->sel.wcnt[1];
            if (++ix->sel.wcnt[2] > 0xe) {

                /* 自動ＶＥの強制終了 */
                SCL_AbortAutoVe();

                ix->mode = work->execmode;      /* 飛び先のモード */
                ix->sel.mode++;
            }
            SLCT_setmosaic(ix->sel.wcnt[2]);       /* MOSAIC SET */

                        

            break;

        case NEXT:
            break;
    }
}


/*
 *
 *  SEGA LOGO ACTION
 *
 */
void SMSL_LogoAct(MDSELACT *ix)
{

    enum {
        INIT,
        MOVE_IN,
        MOVE_OUT,
        NEXT
	};

    enum {
        MODE_1,
        MODE_2,
        MODE_3,
        MODE_4,
        MODE_5,
        MODE_6,
        MODE_7,
        MODE_8,
        MODE_9,
        MODE_NEXT
	};

    SclWinscl *winreg;

    winreg = &Scl_w_reg;

    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_rotzoom;
            ix->sprite.pmode = 0;
            ix->sprite.patbase = SMSL_logo_pattbl;
            ix->sprite.sproffset = (0x14000 >> 3);
            ix->sprite.patno = 0;
            ix->sprite.xposi = 0xa0 * 0x10000;
            ix->sprite.yposi = 0x70 * 0x10000;
            ix->sprite.color = 0x10;
            ix->sprite.hzoom = 0x300;
            ix->sprite.vzoom = 0x300;
            ix->sprite.zdirec = 0;
            ix->wcnt[0] = 0;                /* COUNTER */
            ix->wcnt[1] = 0;                /* COUNTER */
            ix->wcnt[2] = 0;                /* MODE COUNTER */

            /* 自動カラー演算をセット */
            SCL_SetAutoColMix(SCL_NBG1, 5, 0x80, 0x01, 0x1c);

            ix->mode++;
            /* NO BREAK */

        case MOVE_IN:
            if (++ix->wcnt[0] > 0x200) {
                /* 自動カラー演算をセット */
                SCL_SetAutoColMix(SCL_NBG1, 5, 0x80, 0x1c, 0x04);

                ix->wcnt[0] = 0;
                ix->mode++;
            }
            ix->sprite.zdirec++;
            ix->sprite.zdirec &= 0xff;
            break;

        case MOVE_OUT:
            if (++ix->wcnt[0] > 0x200) {
                /* 自動カラー演算をセット */
                SCL_SetAutoColMix(SCL_NBG1, 5, 0x80, 0x04, 0x1c);

                ix->wcnt[0] = 0;
                ix->mode--;
            }
            ix->sprite.zdirec++;
            ix->sprite.zdirec &= 0xff;
            break;
    }

    switch (ix->wcnt[2]) {
        case MODE_1:
            if (++ix->wcnt[1] > 0x80) {
                ix->wcnt[1] = 0;
                ix->wcnt[2]++;
            }
            break;

        case MODE_2:
            if (++ix->wcnt[1] > 0x160) {
                ix->wcnt[1] = 0;
                ix->wcnt[2]++;
            }
            ix->sprite.hzoom -= 2;
            ix->sprite.vzoom -= 2;
            break;

        case MODE_3:
            if (++ix->wcnt[1] > 0x60) {
                ix->wcnt[1] = 0;
                ix->wcnt[2]++;
            }
            break;

        case MODE_4:
            if (++ix->wcnt[1] > 0x18) {
                ix->wcnt[1] = 0;
                ix->wcnt[2]++;
            }
            ix->sprite.hzoom += 0x20;
            ix->sprite.vzoom += 0x20;
            break;

        case MODE_5:
            if (++ix->wcnt[1] > 0x8) {
                ix->wcnt[1] = 0;
                ix->wcnt[2]++;
            }
            ix->sprite.hzoom -= 0x10;
            ix->sprite.vzoom -= 0x10;
            break;

        case MODE_6:
            if (++ix->wcnt[1] > 0x6) {
                ix->wcnt[1] = 0;
                ix->wcnt[2]++;
            }
            ix->sprite.hzoom += 0x10;
            ix->sprite.vzoom += 0x10;
            break;

        case MODE_7:
            if (++ix->wcnt[1] > 0x4) {
                ix->wcnt[1] = 0;
                ix->wcnt[2]++;
            }
            ix->sprite.hzoom -= 0xc;
            ix->sprite.vzoom -= 0xc;
            break;

        case MODE_8:
            if (++ix->wcnt[1] > 0x2) {
                ix->wcnt[1] = 0;
                ix->wcnt[2]++;
            }
            ix->sprite.hzoom += 0x8;
            ix->sprite.vzoom += 0x8;
            break;

        case MODE_9:
            ix->sprite.hzoom = 0x300;
            ix->sprite.vzoom = 0x300;
            ix->wcnt[1] = 0;
            ix->wcnt[2] = MODE_1;
            break;
    }
    SMV1_PutSprite(&ix->sprite);
}


/*
 *
 *    ASCII & SCROLL_A CG SET
 *
 */
static void SMSL_cgset(void)
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

    SMV2_SetFont8((Uint8 *)NBG_PTGEN, 1, 0);
    SMMI_Ldir8((Uint8 *)scra_info.cgadd, (Uint8 *)(NBG_PTGEN+0x40*0x40),
            (Uint32)scra_info.cgsize); 

    SMMI_Ldir16((Uint16 *)scra_info.clradd, (Uint16 *)(COLOR_RAM+0x200),
            (Uint32)scra_info.clrcnt); 

    SPR_2SetChar(SMSL_logocharNo,0,0,0x60, 0x20, (Uint8 *)segalogo_info.cgadd);
    SMMI_Ldir16((Uint16 *)segalogo_info.clradd, (Uint16 *)(COLOR_RAM+0x20),
            segalogo_info.clrcnt);

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
}


