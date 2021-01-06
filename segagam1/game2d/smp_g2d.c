/* Copyright(c) 1994 SEGA			*/
/*
 *
 *    segalibを使った簡単なシューティングゲーム
 *
 */

#include "smp_bios.h"

#include "smp_g2cg.h"
#include "smp_g2d.h"


Sint32 SMG2_player_xposi;
Sint32 SMG2_player_yposi;
Sint32 SMG2_player_xspd;
Sint32 SMG2_player_yspd;

Sint32 SMG2_score;
Sint32 SMG2_hiscore;
Sint8 SMG2_player_zanki;
Uint8 SMG2_stageno;
Uint8 SMG2_gamestatus;
Uint8 SMG2_pauseflag;

Uint32 SMG2_mainloop_cnt;
Sint16 SMG2_starlength;

Uint16 SMG2_objcount;
SPRITE *SMG2_objbuff[0x100];

Sint16 SMG2_scorepat[64];

/*
 *    関数プロトタイプ
 */
static void SMG2_stagesetup(void);          /* 各ラウンドごとの初期化  */

static void SMG2_EnemySetAct(ENSETACT *ix); /* エネミー発生制御        */


static void SMG2_PlayerAct(GAMEACT *ix);    /* プレーヤーアクション    */
static void SMG2_PlMainShot(GAMEACT *ix);   /* プレーヤーショットアクション */

static void SMG2_BackStar(GAMEACT *ix);     /* 背景を流れる星アクション     */

static GAMEACT *SMG2_player_hitcheck(GAMEACT *ix);

static void SMG2_ScoreDispAct(GAMEACT *ix);  /* SCORE,HISCORE,PLAYER DISP */
static void SMG2_ScoreNumAct(GAMEACT *ix);


static void SMG2_ObjTrans(void);

void SMG2_Stage2scrollAct(STG2SCLACT *ix);


/* 外部参照 */
extern void SMG2_Enemy1Set(GAMEACT *ix);     /* Enemy 1 Set Control */
extern void SMG2_Enemy1Act(GAMEACT *ix);     /* Enemy 1 action      */
extern void SMG2_Enemy2Act(GAMEACT *ix);     /* Enemy 2 action      */
extern void SMG2_Enemy3Set(GAMEACT *ix);     /* Enemy 3 Set Control */
extern void SMG2_Enemy3Act(GAMEACT *ix);     /* Enemy 3 action      */
extern void SMG2_Enemy4Set(GAMEACT *ix);     /* Enemy 4 Set Control */
extern void SMG2_Enemy4Act(GAMEACT *ix);     /* Enemy 4 action      */

/* 定数テーブル */


/* ROUND 1 エネミー発生テーブル */
const ENSET_DATA rd1_tbl[] = {
           1, 0, (void *)SMG2_StageStart, 
                     SMG2_ETCWK_TOP,SMG2_ETCWK_CNT, 0x160, 0x20,  /*  0 */

    60 *   7, 0, (void *)SMG2_Enemy1Set, 
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x20,   /*  1 */

    60 *   2, 0, (void *)SMG2_Enemy1Set, 
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0xc0,   /*  2 */

    60 *   2, 0, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x20,   /*  3 */

    60 *   2, 0, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0xc0,   /*  4 */

    60 *   2, 2, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x20,   /*  5 */

    60 *   2, 0, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x50,   /*  6 */

    60 *   0, 0, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x90,   /*  7 */

    60 *   2, 0, (void *)SMG2_Enemy3Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x30,   /*  8 */

    60 *   3, 1, (void *)SMG2_Enemy3Set, 
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x30,   /*  9 */

    60 *   3, 0, (void *)SMG2_Enemy3Set, 
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0xa0,   /* 10 */

    60 *   2, 0, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x20,   /* 11 */

    60 *   2, 0, (void *)SMG2_Enemy4Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x120, 0xf0,   /* 12 */

    60 *   2, 0, (void *)SMG2_Enemy3Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x40,   /* 13 */

    60 *   2, 0, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0xc0,   /* 14 */

    60 *   2, 2, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x20,   /* 15 */

    60 *   2, 0, (void *)SMG2_Enemy3Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0xa0,   /* 16 */

    60 *   2, 0, (void *)SMG2_Enemy4Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x120, -0x10,  /* 17 */

    60 *   2, 0, (void *)SMG2_Enemy4Set, 
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x100, 0xf0,   /* 18 */

     1 *   2, 0, (void *)SMG2_Enemy3Set, 
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x70,   /* 19 */

    60 *   3, 1, (void *)SMG2_Enemy1Set, 
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x30,   /* 20 */

    60 *   2, 0, (void *)SMG2_Enemy4Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x100, -0x10,  /* 21 */

    60 *   3, 2, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x30,   /* 22 */

    60 *   2, 0, (void *)SMG2_Enemy3Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x60,   /* 23 */

    60 *   3, 1, (void *)SMG2_Enemy1Set, 
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x60,   /* 24 */

    60 *   2, 0, (void *)SMG2_Enemy3Set,
                    SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x40,    /* 25 */

    60 *   2, 0, (void *)SMG2_Enemy3Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0xa0,   /* 26 */

    60 *   2, 0, (void *)SMG2_Enemy2Act,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x70, /* 27  BOSS */
      0xffff, 0, (void *)NULL, 
                     0, 0, 0, 0,                                /* 28  END  */
};


/* ROUND 2 エネミー発生テーブル */
const ENSET_DATA rd2_tbl[] = {
           1, 0, (void *)SMG2_StageStart, 
                     SMG2_ETCWK_TOP,SMG2_ETCWK_CNT, 0x160, 0x20,  /*  0 */


    60 *   7, 0, (void *)SMG2_Enemy1Set, 
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x20,   /*  1 */

    60 *   0, 0, (void *)SMG2_Enemy1Set, 
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0xc0,   /*  2 */


    60 *   1, 0, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x40,   /*  3 */

    60 *   0, 0, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0xa0,   /*  4 */


    60 *   1, 0, (void *)SMG2_Enemy1Set, 
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x60,   /*  5 */

    60 *   0, 0, (void *)SMG2_Enemy1Set, 
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x80,   /*  6 */


    60 *   1, 0, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0xa0,   /*  7 */

    60 *   0, 0, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x40,   /*  8 */


    60 *   1, 0, (void *)SMG2_Enemy1Set, 
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x20,   /*  1 */

    60 *   0, 0, (void *)SMG2_Enemy1Set, 
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0xc0,   /*  2 */


    60 *   1, 0, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x40,   /*  3 */

    60 *   0, 0, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0xa0,   /*  4 */


    60 *   3, 2, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x20,   /*  9 */
    60 *   1, 0, (void *)SMG2_Enemy4Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x100, -0x10,  /* 21 */
    60 *   0, 0, (void *)SMG2_Enemy4Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x100, 0xf0,  /* 21 */

    60 *   1, 2, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x20,   /*  9 */
    60 *   1, 0, (void *)SMG2_Enemy4Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0xa0, -0x10,  /* 21 */
    60 *   0, 0, (void *)SMG2_Enemy4Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0xa0, 0xf0,  /* 21 */

    60 *   1, 2, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x20,   /*  9 */
    60 *   1, 0, (void *)SMG2_Enemy4Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x120, -0xf0,  /* 21 */
    60 *   0, 0, (void *)SMG2_Enemy4Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x120, -0x10,  /* 21 */

    60 *   3, 0, (void *)SMG2_Enemy3Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0xa0,   /* 16 */

    60 *   2, 0, (void *)SMG2_Enemy3Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x30,   /* 16 */

    60 *   2, 0, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x20,   /*  7 */

    60 *   0, 0, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0xc0,   /*  8 */

    60 *   3, 1, (void *)SMG2_Enemy1Set, 
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x30,   /* 20 */
    60 *   2, 0, (void *)SMG2_Enemy3Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x30,   /* 25 */

    60 *   3, 1, (void *)SMG2_Enemy1Set, 
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x30,   /* 20 */
    60 *   2, 0, (void *)SMG2_Enemy3Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0xa0,   /* 25 */

    60 *   4, 0, (void *)SMG2_Enemy2Act,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x70, /* 27  BOSS */
      0xffff, 0, (void *)NULL, 
                     0, 0, 0, 0,                                /* 28  END  */
};




/* ROUND 3 エネミー発生テーブル */
const ENSET_DATA rd3_tbl[] = {
           1, 0, (void *)SMG2_StageStart, 
                     SMG2_ETCWK_TOP,SMG2_ETCWK_CNT, 0x160, 0x20,  /*  0 */

    60 *   7, 0, (void *)SMG2_Enemy1Set, 
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x20,   /*  1 */

    60 *   2, 0, (void *)SMG2_Enemy1Set, 
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0xc0,   /*  2 */

    60 *   2, 0, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x20,   /*  3 */

    60 *   2, 0, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0xc0,   /*  4 */

    60 *   2, 2, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x20,   /*  5 */

    60 *   2, 0, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x50,   /*  6 */

    60 *   0, 0, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x90,   /*  7 */

    60 *   2, 0, (void *)SMG2_Enemy3Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x30,   /*  8 */

    60 *   3, 1, (void *)SMG2_Enemy3Set, 
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x30,   /*  9 */

    60 *   3, 0, (void *)SMG2_Enemy3Set, 
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0xa0,   /* 10 */

    60 *   2, 0, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x20,   /* 11 */

    60 *   2, 0, (void *)SMG2_Enemy4Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x120, 0xf0,   /* 12 */

    60 *   2, 0, (void *)SMG2_Enemy3Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x40,   /* 13 */

    60 *   2, 0, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0xc0,   /* 14 */

    60 *   2, 2, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x20,   /* 15 */

    60 *   2, 0, (void *)SMG2_Enemy3Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0xa0,   /* 16 */

    60 *   2, 0, (void *)SMG2_Enemy4Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x120, -0x10,  /* 17 */

    60 *   2, 0, (void *)SMG2_Enemy4Set, 
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x100, 0xf0,   /* 18 */

     1 *   2, 0, (void *)SMG2_Enemy3Set, 
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x70,   /* 19 */

    60 *   3, 1, (void *)SMG2_Enemy1Set, 
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x30,   /* 20 */

    60 *   2, 0, (void *)SMG2_Enemy4Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x100, -0x10,  /* 21 */

    60 *   3, 2, (void *)SMG2_Enemy1Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x30,   /* 22 */

    60 *   2, 0, (void *)SMG2_Enemy3Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x60,   /* 23 */

    60 *   3, 1, (void *)SMG2_Enemy1Set, 
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x60,   /* 24 */

    60 *   2, 0, (void *)SMG2_Enemy3Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x40,   /* 25 */

    60 *   2, 0, (void *)SMG2_Enemy3Set,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0xa0,   /* 26 */

    60 *   2, 0, (void *)SMG2_Enemy2Act,
                     SMG2_ENWK_TOP, SMG2_ENWK_CNT, 0x160, 0x70, /* 27  BOSS */
      0xffff, 0, (void *)NULL, 
                     0, 0, 0, 0,                                /* 28  END  */
};



/*
 *   エネミー発生テーブルアドレス
 */
ENSET_DATA * const enset_rdtbl[] = {
    rd1_tbl,                        /* DUMMY */
    rd1_tbl, rd2_tbl, rd3_tbl
  };


/*
 *     コリジョンテーブル
 */
const SMCO_COLLTBL SMCO_colltbl[] = {
/*     xl,    yl,    xr,    yr  */
        0,     0,     0,     0,            /* 00 */
     -0x1,  -0x1,   0x1,   0x1,            /* 01 Enemy Shot */
     -0x2,  -0x2,   0x2,   0x2,            /* 02 */
     -0x4,  -0x4,   0x4,   0x4,            /* 03 */
     -0x8,  -0x8,   0x8,   0x8,            /* 04 Item etc... */
       -4, - 0xc,     4,   0xc,            /* 05 PLAYER MAIN SHOT */
       -8,    -4,     8,     4,            /* 06 PLAYER */
      -24,   -24,    24,    24,            /* 07 Enemy 2 */
};





/*==========================================================================*/
/*--------------------------------------------------------------
 *  ゲームメインプログラム
 *--------------------------------------------------------------
 *  ここでゲームの進行を管理する
 *  
 *  
 *--------------------------------------------------------------
 */
void SMG2_GameMode(void)
{
    enum {
        G2D_INIT,
        G2D_SETUP,
        G2D_MAIN,
        G2D_EXIT
      };

    Uint16 vcnt, i, j;
    GAMEACT *iy;

    while (SMMA_MainMode == GAME_MODE) {
        SMV1_SprCmdStart();

        switch (SMMA_MainLevel) {

            case G2D_INIT:
                SMG2_score = 0;                  /* SCORE            */
                SMG2_hiscore = 10000;            /* HISCORE          */
                SMG2_player_zanki = 5;           /* PLAYER LEFT      */
                SMG2_starlength = 0;             /* STAR LENGTH WORK */
                SMG2_stageno = 1;                /* STAGE NO         */ 
                SMMA_MainLevel++;

            case G2D_SETUP:
                SMV2_TvOff();
                SMMA_initall();

                SCL_SetPriority(SCL_SP0, 7);
                SCL_SetPriority(SCL_NBG0, 6);
                SCL_SetPriority(SCL_NBG1, 5);
                SCL_SetPriority(SCL_RBG0, 0);

                SCL_SetColRamMode(SCL_CRM15_2048); /* 15bit 2048 color mode */

                SMV2_TvOff();
                SMV1_SprCmdStart();
                SMV1_SprCmdEnd();
                SMMA_intwait();
                SMMA_intwait();

                SMG2_stagesetup();                 /* ステージごとの初期化  */

                SMTA_MakeAction(SMG2_EnemySetAct); /* enemyset action set   */
                SMTA_SetAction(SMG2_PlayerAct, 
                               SMG2_PLWK_TOP);     /* player action */
                SMTA_MakeActionX(SMG2_ScoreDispAct, 
                                 SMG2_ETCWK_TOP, SMG2_ETCWK_CNT); /* SCORE */
                SMTA_MakeActionX(SMG2_ScoreNumAct, 
                                 SMG2_ETCWK_TOP, SMG2_ETCWK_CNT); /* SCORE */


                SMG2_gamestatus = 0;               /* GAME STATUS           */
                SMG2_mainloop_cnt = 0;             /* MainLoop COUNT        */
                SMG2_pauseflag = 0;                /* PAUSE FLAG            */
                SMG2_objcount = 0;
                SMV2_TvOn();
                SMMA_MainLevel++;
 
            case G2D_MAIN:
                if (SMG2_pauseflag == 0) {
                    SMG2_objcount = 0;
                    SMTA_ActionLoop();
                    SMG2_mainloop_cnt++;
                }
                SMG2_ObjTrans();
 
                /* ポーズのチェック */
                if (SMPA_pad_edge1 & PAD_START) 
                    SMG2_pauseflag ^= 1;

                /* ゲーム強制中断のチェック */
                if (SMG2_pauseflag != 0) {
                    if (SMPA_pad_edge1 & PAD_B)
                        SMMA_MainLevel = G2D_EXIT;
                }

                /* ゲームオーバーのチェック */
                if (SMG2_gamestatus & SMG2_gmsts_GAMEOVER) {
                    SMMA_MainLevel = G2D_EXIT;
                    break;
                }

                /* ステージクリアのチェック*/
                if (SMG2_gamestatus & SMG2_gmsts_NEXTSTAGE) {
                    SMG2_stageno++;
                    if (SMG2_stageno > 3) 
                        SMMA_MainLevel = G2D_EXIT;
                    else
                        SMMA_MainLevel = G2D_SETUP;
                }
                break;

            case G2D_EXIT:
                SMMA_MainMode = SELECT_MODE;
                SMMA_MainLevel = 0;
                break;
	}
        SMV1_SprCmdEnd();
        SMMA_intwait();                  /* V-BLANK WAIT         */

    }

}


/*==========================================================================*/
/*--------------------------------------------------------------
 *  スプライト表示要求
 *--------------------------------------------------------------
 *  ポーズ時でもスプライト表示をするため、一度表示バッファに
 *  表示したいスプライトをセットする
 *  
 *--------------------------------------------------------------
 */
void SMG2_PutObject(SPRITE *spr)
{
    if (SMG2_objcount < 0x100) {
        SMG2_objbuff[SMG2_objcount] = spr;
        SMG2_objcount++;
    }
}

/*--------------------------------------------------------------
 *  スプライト転送
 *--------------------------------------------------------------
 *  表示バッファにあるスプライトを実際に描画する
 *  
 *  
 *--------------------------------------------------------------
 */
void SMG2_ObjTrans(void)
{
    Uint16 i;

    for (i = 0; i < SMG2_objcount; i++) {
        SMV1_PutSprite(SMG2_objbuff[i]);
    }
}



/*==========================================================================*/
/*--------------------------------------------------------------
 *  各ステージごとの初期化
 *--------------------------------------------------------------
 *  各ステージごとのCGのセットなどを行なう
 *  
 *  
 *--------------------------------------------------------------
 */
void SMG2_stagesetup(void)
{
    const Uint16 nbg1_map[] = {
        0x1082,0x1084,0x1086,0x1088,
        0x108a,0x108c,0x108e,0x1090,
        0x1092,0x1094,0x1096,0x1098,
        0x109a,0x109c,0x109e,0x10a0,
    };

    GAMEACT *iy;
    Uint16 *dst16, *src16, *temp16;
    Uint16 h,v,i, wk;
    Uint32 *dst32, *temp32, *dst32b;


    /* アスキーフォントセット */    
    SMV2_SetFont8((Uint8 *)NBG_PTGEN,1,0);

    /* スクロール面のクリア */
    SMV2_FilMem16((Uint16 *)NBG0_PTNAMA, 
                  (0x40 * 0x40 / sizeof (Uint16)), 0x0000);
    SMV2_FilMem16((Uint16 *)NBG1_PTNAMA, 
                  (0x40 * 0x40 / sizeof (Uint16)), 0x0000);

    /* スプライトアスキーフォントセット */
    SMV1_SetFont4sp(0,1,0);
    poke_w(COLOR_RAM + 0x00, 0x0000);
    poke_w(COLOR_RAM + 0x02, 0x739c);
    poke_w(COLOR_RAM + 0x20, 0x7000);
    poke_w(COLOR_RAM + 0x22, 0x739c);


    /* プレーヤーキャラクタのセット */
    SPR_2SetChar(SMG2_PlayCharNo, 0, 0, 24, 16, (Uint8 *)JAGER_info.cgadd);
    SMMI_Ldir16((Uint16 *)JAGER_info.clradd, 
                (Uint16 *)(COLOR_RAM + 0x40), JAGER_info.clrcnt);

    /* ボスキャラクタのセット */
    SPR_2SetChar(SMG2_BossCharNo, 0, 0, 64, 64, (Uint8 *)BOSS_info.cgadd);
    SMMI_Ldir16((Uint16 *)BOSS_info.clradd, 
                (Uint16 *)(COLOR_RAM + 0x60), BOSS_info.clrcnt);

    /* ザコエネミーのセット */
    SPR_2SetChar(SMG2_Lrun1CharNo, 0, 0, 16, 24, (Uint8 *)LRUN1_info.cgadd);
    SPR_2SetChar(SMG2_Lrun2CharNo, 0, 0, 16, 24, (Uint8 *)LRUN2_info.cgadd);
    SPR_2SetChar(SMG2_Lrun3CharNo, 0, 0, 16, 24, (Uint8 *)LRUN3_info.cgadd);
    SPR_2SetChar(SMG2_Lrun4CharNo, 0, 0, 16, 24, (Uint8 *)LRUN4_info.cgadd);
    SPR_2SetChar(SMG2_Lrun5CharNo, 0, 0, 16, 24, (Uint8 *)LRUN5_info.cgadd);
    SMMI_Ldir16((Uint16 *)LRUN1_info.clradd, 
                (Uint16 *)(COLOR_RAM + 0xa0), LRUN1_info.clrcnt);

    /* 爆発パターンのセット */
    SPR_2SetChar(SMG2_Kemu0CharNo, 0, 0, 16, 16, (Uint8 *)KEMU0_info.cgadd);
    SPR_2SetChar(SMG2_Kemu1CharNo, 0, 0, 16, 16, (Uint8 *)KEMU1_info.cgadd);
    SPR_2SetChar(SMG2_Kemu2CharNo, 0, 0, 16, 16, (Uint8 *)KEMU2_info.cgadd);
    SPR_2SetChar(SMG2_Kemu3CharNo, 0, 0, 16, 16, (Uint8 *)KEMU3_info.cgadd);
    SPR_2SetChar(SMG2_Kemu4CharNo, 0, 0,  8,  8, (Uint8 *)KEMU4_info.cgadd);

    /* アイテムキャラクタのセット */
    SPR_2SetChar(SMG2_Item0CharNo, 0, 0, 16, 16, (Uint8 *)ITEM1_info.cgadd);
    SPR_2SetChar(SMG2_Item1CharNo, 0, 0, 16, 16, (Uint8 *)ITEM2_info.cgadd);


    /* 背景を流れる星のアクションセット */
    for (i = 0; i < 16; i++) { 
        iy = SMTA_MakeActionX(SMG2_BackStar, SMG2_BSTAR_TOP, SMG2_BSTAR_CNT);
        iy->wcnt[0] = i & 3;
    }

    /* NBG0表示画面クリア */
    SMV2_FilMem16((Uint16 *)NBG0_PTNAMA, 
                  (0x40 * 0x40 / sizeof (Uint16)),
                   0x0000);

    /* ステージ２なら */
    if (SMG2_stageno == 2) {

        /* ラインスクロールパラメータ初期化 */
        SCL_Open(SCL_NBG0); 
        SCL_InitLineParamTb(&SMV2_lparam);
        SCL_Close();        

        /* スクロール用CGの転送 */
        SMMI_Ldir8((Uint8 *)scra_info.cgadd, 
                   (Uint8 *)(NBG_PTGEN+0x40*0x40), 
                   (Uint32)scra_info.cgsize); 

        /* スクロール用カラーの転送 */
        SMMI_Ldir16((Uint16 *)scra_info.clradd, 
                    (Uint16 *)(COLOR_RAM+0x200), 
                    (Uint32)scra_info.clrcnt); 

        /* スクロールマップのセット */
        dst16 = (Uint16 *)NBG0_PTNAMA;
        for (v = 0; v < (64 / 4); v++) {
             temp16 = dst16;
             for (h = 0; h < (64 / 4); h++) {
                  SMV2_BoxWrite16(temp16, nbg1_map, 4, 4);
                  temp16 += 0x4;
             }
             dst16 +=  (0x40 * 4);  /* 次のラインへ */
        }
        /* スクロールアクションセット */
        SMTA_MakeAction(SMG2_Stage2scrollAct);
    }
}

/*==========================================================================*/
/*--------------------------------------------------------------
 *  エネミーフレームアウトチェック
 *--------------------------------------------------------------
 *  エネミーが表示領域より外側にいるかどうかチェックする
 *  
 *  
 *--------------------------------------------------------------
 */
Uint8 SMG2_EnemyFrameOutChk(Sint16 xposi, Sint16 yposi)
{
     if ((yposi > -0x20) && (yposi < 0x100) && 
         (xposi > -0x20) && (xposi < 0x150))
        return 0;   /* フレームアウトしていない */
     else
        return 1;   /* フレームアウトした       */
}
        



/*==========================================================================*/
/*--------------------------------------------------------------
 *  エネミー発生コントール
 *--------------------------------------------------------------
 *  発生テーブルに書かれたタイマーから指定のエネミーを
 *  発生させます。
 *  
 *--------------------------------------------------------------
 */
static void SMG2_EnemySetAct(ENSETACT *ix)
{
    enum {
         INIT,
         MAIN,
         NEXT
         };

    Uint16 timer, wkstart, wkcnt;
    void *execadd;    
    GAMEACT *iy, *iys;
    ENSET_DATA *ensetptr;
	
    switch (ix->mode) {
        case INIT:
            /* エネミーセットタイマーカウントクリア */
            ix->wcnt[0] = 0;

            /* エネミー発生テーブルアドレスセット */
            ix->setptr = enset_rdtbl[SMG2_stageno];
            ix->mode++;

        case MAIN:
            /* エネミー発生禁止フラグが立っていたら終り */
            if (SMG2_gamestatus & SMG2_gmsts_ENSETOFF)
                break;

            /* タイマーカウントアップ */
            ix->wcnt[0]++;

            ensetptr = ix->setptr;
            timer = ensetptr->timer;       /* 出現タイマーリード   */

            if (timer == 0xffff) 
                SMTA_KillAction(ix);       /* テーブルの最後？     */

            /* タイマーカウントがテーブルのタイマーを越えたら */
            if (ix->wcnt[0] > timer) {
                wkstart = ensetptr->wkstart;
                wkcnt = ensetptr->srchcnt;
                execadd = ensetptr->execadd;
                iy = SMTA_MakeActionX(execadd, wkstart, wkcnt);
                *(Sint16 *)&iy->sprite.xposi = ensetptr->xposi;
                *(Sint16 *)&iy->sprite.yposi = ensetptr->yposi;
                iy->level = ensetptr->level;

                ix->setptr++;
                ix->wcnt[0] = 0;                
            }
            break;
    }
}







/*==========================================================================*/
/*
 *    'READY'表示アクション
 */
void SMG2_StageStart(GAMEACT *ix)
{

/*  枚数,
    Charno, HVreverse, Ｈサイズ, Ｖサイズ, Ｈオフセット, Ｖオフセット */
    static const Sint16 stage1_pat[] = {
            13,
            ('S' - ' ') , 0, 0x0008, 0x0008,   -52,  -4,
            ('T' - ' ') , 0, 0x0008, 0x0008,   -44,  -4,
            ('A' - ' ') , 0, 0x0008, 0x0008,   -36,  -4,
            ('G' - ' ') , 0, 0x0008, 0x0008,   -28,  -4,
            ('E' - ' ') , 0, 0x0008, 0x0008,   -20,  -4,
            (' ' - ' ') , 0, 0x0008, 0x0008,   -12,  -4,
            ('1' - ' ') , 0, 0x0008, 0x0008,    -4,  -4,
            (' ' - ' ') , 0, 0x0008, 0x0008,     4,  -4,
            ('S' - ' ') , 0, 0x0008, 0x0008,    12,  -4,
            ('T' - ' ') , 0, 0x0008, 0x0008,    20,  -4,
            ('A' - ' ') , 0, 0x0008, 0x0008,    28,  -4,
            ('R' - ' ') , 0, 0x0008, 0x0008,    36,  -4,
            ('T' - ' ') , 0, 0x0008, 0x0008,    44,  -4,
        };

    static const Sint16 stage2_pat[] = {
            13,
            ('S' - ' ') , 0, 0x0008, 0x0008,   -52,  -4,
            ('T' - ' ') , 0, 0x0008, 0x0008,   -44,  -4,
            ('A' - ' ') , 0, 0x0008, 0x0008,   -36,  -4,
            ('G' - ' ') , 0, 0x0008, 0x0008,   -28,  -4,
            ('E' - ' ') , 0, 0x0008, 0x0008,   -20,  -4,
            (' ' - ' ') , 0, 0x0008, 0x0008,   -12,  -4,
            ('2' - ' ') , 0, 0x0008, 0x0008,    -4,  -4,
            (' ' - ' ') , 0, 0x0008, 0x0008,     4,  -4,
            ('S' - ' ') , 0, 0x0008, 0x0008,    12,  -4,
            ('T' - ' ') , 0, 0x0008, 0x0008,    20,  -4,
            ('A' - ' ') , 0, 0x0008, 0x0008,    28,  -4,
            ('R' - ' ') , 0, 0x0008, 0x0008,    36,  -4,
            ('T' - ' ') , 0, 0x0008, 0x0008,    44,  -4,
        };

    static const Sint16 stage3_pat[] = {
            13,
            ('S' - ' ') , 0, 0x0008, 0x0008,   -52,  -4,
            ('T' - ' ') , 0, 0x0008, 0x0008,   -44,  -4,
            ('A' - ' ') , 0, 0x0008, 0x0008,   -36,  -4,
            ('G' - ' ') , 0, 0x0008, 0x0008,   -28,  -4,
            ('E' - ' ') , 0, 0x0008, 0x0008,   -20,  -4,
            (' ' - ' ') , 0, 0x0008, 0x0008,   -12,  -4,
            ('3' - ' ') , 0, 0x0008, 0x0008,    -4,  -4,
            (' ' - ' ') , 0, 0x0008, 0x0008,     4,  -4,
            ('S' - ' ') , 0, 0x0008, 0x0008,    12,  -4,
            ('T' - ' ') , 0, 0x0008, 0x0008,    20,  -4,
            ('A' - ' ') , 0, 0x0008, 0x0008,    28,  -4,
            ('R' - ' ') , 0, 0x0008, 0x0008,    36,  -4,
            ('T' - ' ') , 0, 0x0008, 0x0008,    44,  -4,
        };



/*  枚数,
    Charno, HVreverse, Ｈサイズ, Ｖサイズ, Ｈオフセット, Ｖオフセット */
    static const Sint16 stgclr_pat[] = {
            11,
            ('S' - ' ') , 0, 0x0008, 0x0008,   -44,  -4,
            ('T' - ' ') , 0, 0x0008, 0x0008,   -36,  -4,
            ('A' - ' ') , 0, 0x0008, 0x0008,   -28,  -4,
            ('G' - ' ') , 0, 0x0008, 0x0008,   -20,  -4,
            ('E' - ' ') , 0, 0x0008, 0x0008,   -12,  -4,
            (' ' - ' ') , 0, 0x0008, 0x0008,    -4,  -4,
            ('C' - ' ') , 0, 0x0008, 0x0008,     4,  -4,
            ('L' - ' ') , 0, 0x0008, 0x0008,    12,  -4,
            ('E' - ' ') , 0, 0x0008, 0x0008,    20,  -4,
            ('A' - ' ') , 0, 0x0008, 0x0008,    28,  -4,
            ('R' - ' ') , 0, 0x0008, 0x0008,    36,  -4,
        };

/*  枚数,
    Charno, HVreverse, Ｈサイズ, Ｖサイズ, Ｈオフセット, Ｖオフセット */
    static const Sint16 gmover_pat[] = {
            9,
            ('G' - ' ') , 0, 0x0008, 0x0008,   -36,  -4,
            ('A' - ' ') , 0, 0x0008, 0x0008,   -28,  -4,
            ('M' - ' ') , 0, 0x0008, 0x0008,   -20,  -4,
            ('E' - ' ') , 0, 0x0008, 0x0008,   -12,  -4,
            (' ' - ' ') , 0, 0x0008, 0x0008,    -4,  -4,
            ('O' - ' ') , 0, 0x0008, 0x0008,     4,  -4,
            ('V' - ' ') , 0, 0x0008, 0x0008,    12,  -4,
            ('E' - ' ') , 0, 0x0008, 0x0008,    20,  -4,
            ('R' - ' ') , 0, 0x0008, 0x0008,    28,  -4,
        };


    static Sint16 * const stage_pat[] = {
            stage1_pat,      /* 0 */
            stage2_pat,      /* 1 */
            stage3_pat,      /* 2 */
            stgclr_pat,      /* 3 */
            gmover_pat,      /* 4 */   
        };
		
    enum {
         INIT,
         READY_MOVE1, READY_MOVE2, READY_MOVE3, READY_MOVE4,
         STGCLR_MOVE1, STGCLR_MOVE2,
         GOVER_MOVE1, GOVER_MOVE2
         };


    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_rotzoom;
            ix->sprite.pmode = SPM_ecd;
            ix->sprite.patbase = stage_pat;
            ix->sprite.hzoom = 0x0;
            ix->sprite.vzoom = 0x0;
            ix->sprite.xposi = 0xa0 * 0x10000;
            ix->sprite.yposi = 0x70 * 0x10000;
            ix->sprite.color = 0x10;
            ix->sprite.zdirec = 0;

            ix->wcnt[0] = 0;
            ix->wcnt[1] = 0;
            ix->wcnt[2] = 0;
            ix->wcnt[3] = 0;

            if (ix->level == 0) 
                ix->sprite.patno = SMG2_stageno - 1;
            else if (ix->level == 1) 
                ix->sprite.patno = 3;
            else if (ix->level == 2)
                ix->sprite.patno = 4;


            switch (ix->level) {
                case 0:
                    SMG2_starlength = 0x100;
                    ix->mode = READY_MOVE1;
                    break;
                case 1:
                    SMG2_starlength = 0x0;
                    ix->mode = STGCLR_MOVE1;
                    break;
                case 2:
                    ix->mode = GOVER_MOVE1;
                    break;
            }
            break;

        case READY_MOVE1:
            ix->sprite.vzoom += 0x20;
            ix->sprite.hzoom += 0x20;
            if (ix->sprite.vzoom > 0x300)
                ix->mode++;
            SMG2_PutObject(&ix->sprite);
            break;
        case READY_MOVE2:
            ix->wcnt[0] += 0x10;

            if ((ix->wcnt[0] & 0x100) == 0) 
                SMG2_PutObject(&ix->sprite);

            ix->wcnt[1]++;
            if (ix->wcnt[1] > 60 * 3) ix->mode++;


            break;
        case READY_MOVE3:
            ix->sprite.vzoom -= 0x80;
            ix->sprite.hzoom -= 0x80;
            if (ix->sprite.vzoom < 0x80) {
                ix->sprite.hzoom = 0;
                ix->sprite.vzoom = 0;
                ix->mode++;
            }
            SMG2_PutObject(&ix->sprite);
            break;
        case READY_MOVE4:
            SMG2_starlength -= 2;
            if (SMG2_starlength < 0x1) {
                SMG2_starlength = 0;
                SMTA_KillAction(ix);
            }
            break;

        case STGCLR_MOVE1:
            if (ix->wcnt[0]++ < 60 * 2) break;

            ix->sprite.vzoom += 0x20;
            ix->sprite.hzoom += 0x20;
            ix->sprite.zdirec += 4;
            if (ix->sprite.vzoom > 0x300) {
                ix->mode++;
                ix->wcnt[0] = 4;
            }
            SMG2_PutObject(&ix->sprite);
            break;

        case STGCLR_MOVE2:
            ix->sprite.zdirec += ix->wcnt[0];
            if ((ix->sprite.zdirec & 0xff) == 0) ix->wcnt[0] = 0;
            if (SMG2_starlength < 0x100) SMG2_starlength += 2;
            SMG2_PutObject(&ix->sprite);
            if (ix->wcnt[1]++ > 60 * 6) {
                SMTA_KillAction(ix);
                SMG2_gamestatus |= SMG2_gmsts_NEXTSTAGE;
            }
            break;



        case GOVER_MOVE1:
            if (ix->wcnt[0]++ < 60 * 2) break;

            ix->sprite.vzoom += 0x10;
            ix->sprite.hzoom += 0x10;
            ix->sprite.zdirec++;
            if (ix->sprite.vzoom > 0x400) {
                ix->mode++;
                ix->wcnt[0] = 1;
            }
            SMG2_PutObject(&ix->sprite);
            break;

        case GOVER_MOVE2:
            ix->sprite.zdirec += ix->wcnt[0];
            if ((ix->sprite.zdirec & 0xff) == 0) ix->wcnt[0] = 0;
            SMG2_PutObject(&ix->sprite);
            if (ix->wcnt[1]++ > 60 * 6) {
                SMTA_KillAction(ix);
                SMG2_gamestatus |= SMG2_gmsts_GAMEOVER;
            }
            break;
    }

}






/*==========================================================================*/
/*--------------------------------------------------------------
 *  プレーヤーコントロール
 *--------------------------------------------------------------
 *  パッドの内容をみて移動、攻撃をする
 *  
 *  
 *--------------------------------------------------------------
 */
static void SMG2_PlayerAct(GAMEACT *ix)
{

/*  枚数,
    Charno, HVreverse, Ｈサイズ, Ｖサイズ, Ｈオフセット, Ｖオフセット */
    static Sint16 player_pat0[] = {
            1,
            SMG2_PlayCharNo, SPR_Hrev, 24, 16, -12, -8
        };

/*  枚数,
    CharNo, HVreverse, Ｈサイズ, Ｖサイズ, Ｈオフセット, Ｖオフセット */
    static const Sint16 bokan_pat0[] = {
            5,
            ('B' - ' '), 0, 0x0008, 0x0008,   -20,    -4,
            ('O' - ' '), 0, 0x0008, 0x0008,   -12,    -4,
            ('M' - ' '), 0, 0x0008, 0x0008,    -4,    -4,
            ('B' - ' '), 0, 0x0008, 0x0008,     4,    -4,
            ('!' - ' '), 0, 0x0008, 0x0008,    12,    -4,
        };

    static Sint16 *player_pat[] = {
                                    player_pat0, bokan_pat0 };
		
    enum {
         INIT,
         MOVE,
         YARARE_1,
         YARARE_2
         };

    Uint16 swdata, dx, dy;
    GAMEACT *iy;
	
    swdata = SMPA_pad_data1;
	
    switch (ix->mode) {
        case INIT:
            /* プレーヤーが死んでいるフラグのクリア*/
            SMG2_gamestatus &= (SMG2_gmsts_GAMEOVER
                              + SMG2_gmsts_NEXTSTAGE 
                              + SMG2_gmsts_ENSETOFF);
          
            ix->sprite.putmode = PMD_sprx_zoom;
            ix->sprite.pmode = SPM_ecd;
            ix->sprite.patbase = player_pat;
            ix->sprite.patno = 0;
            ix->sprite.xposi = -0x20 * 0x10000;
            ix->sprite.yposi = 0x70 * 0x10000;
            ix->sprite.color = SMG2_PlayPalNo;    /* COLOR_RAM 0x40~0x60 */
            ix->sprite.vzoom = 0x100;
            ix->sprite.hzoom = 0x100;
            
            ix->colino = 6;
            ix->coliatr = SMCO_atr_PLAYER;
            ix->coliflg = 0;

            ix->wcnt[0] = 0;                     /* メインショットカウンタ */

            ix->wcnt[1] = 60 * 2;                /* 無敵タイマー           */
            ix->wreg[0] = 0;                     /* 前回のスイッチデータ   */
            ix->mode++;


        case MOVE:

            /* 無敵中かチェックして当たり判定を行なう*/
            if (ix->wcnt[1] != 0)                /* 無敵中か   */
                ix->wcnt[1]--;
            else
                SMG2_player_hitcheck(ix);        /* 当たり判定 */

            /* 敵、敵の弾に当たっていたら爆発 */
            if ((ix->coliflg & (SMCO_dmg_ENEMY + SMCO_dmg_ENSHOT)) != 0) {
                SMG2_player_zanki--;
                SMG2_gamestatus |= SMG2_gmsts_PLDEAD; /* PLDEAD FLAG SET */
                if (SMG2_player_zanki < 0) 
                    SMG2_gamestatus |= SMG2_gmsts_ENSETOFF;
                ix->mode = YARARE_1;
                break;
            }               

            /* パッドの内容によって移動量を決定する */
            switch (swdata & (PAD_RIGHT + PAD_LEFT + PAD_DOWN + PAD_UP)) {
                case 0x00:
                    ix->sprite.xspd = 0;
                    ix->sprite.yspd = 0;
                    break;
                case PAD_UP:
                    if (ix->wreg[0] == PAD_UP)
                        ix->sprite.yspd = -SMG2_PL_YSPD;
                    else
                        ix->sprite.yspd = -SMG2_PL_BSPD;              
                    break;
                case PAD_DOWN:
                    if (ix->wreg[0] == PAD_DOWN)
                        ix->sprite.yspd = SMG2_PL_YSPD;
                    else
                        ix->sprite.yspd = SMG2_PL_BSPD;
                    break;
                case PAD_LEFT:
                    if (ix->wreg[0] == PAD_LEFT)
                        ix->sprite.xspd = -SMG2_PL_XSPD;
                    else
                        ix->sprite.xspd = -SMG2_PL_BSPD;
                    break;
                case PAD_RIGHT:
                    if (ix->wreg[0] == PAD_RIGHT)
                        ix->sprite.xspd = SMG2_PL_XSPD;
                    else
                        ix->sprite.xspd = SMG2_PL_BSPD;
                    break;
                case (PAD_UP + PAD_LEFT):
                    if (ix->wreg[0] == (PAD_UP+PAD_LEFT)) {
                        ix->sprite.xspd = -SMG2_PL_NXSPD;
                        ix->sprite.yspd = -SMG2_PL_NYSPD;
                    } else {
                        ix->sprite.xspd = -SMG2_PL_NBSPD;
                        ix->sprite.yspd = -SMG2_PL_NBSPD;
                    }
                    break;
                case (PAD_UP + PAD_RIGHT):
                    if (ix->wreg[0] == (PAD_UP+PAD_RIGHT)) {
                        ix->sprite.xspd = SMG2_PL_NXSPD;
                        ix->sprite.yspd = -SMG2_PL_NYSPD;
                    } else {
                        ix->sprite.xspd = SMG2_PL_NBSPD;
                        ix->sprite.yspd = -SMG2_PL_NBSPD;
                    }
                    break;
                case (PAD_DOWN + PAD_LEFT):
                    if (ix->wreg[0] == (PAD_DOWN+PAD_LEFT)) {
                        ix->sprite.xspd = -SMG2_PL_NXSPD;
                        ix->sprite.yspd = SMG2_PL_NYSPD;
                    } else {
                        ix->sprite.xspd = -SMG2_PL_NBSPD;
                        ix->sprite.yspd = SMG2_PL_NBSPD;
                    }
                    break;
                case (PAD_DOWN + PAD_RIGHT):
                    if (ix->wreg[0] == (PAD_DOWN+PAD_RIGHT)) {
                        ix->sprite.xspd = SMG2_PL_NXSPD;
                        ix->sprite.yspd = SMG2_PL_NYSPD;
                    } else {
                        ix->sprite.xspd = SMG2_PL_NBSPD;
                        ix->sprite.yspd = SMG2_PL_NBSPD;
                    }
                    break;
            }

            /* パッドの内容を保時 */
            ix->wreg[0] = swdata & (PAD_RIGHT + PAD_LEFT + PAD_DOWN + PAD_UP);

 
            ix->sprite.xposi += (ix->sprite.xspd << 8);
            ix->sprite.yposi += (ix->sprite.yspd << 8);

            if (*(Sint16 *)&ix->sprite.xposi < SMG2_PL_LLIMIT) 
                *(Sint16 *)&ix->sprite.xposi = SMG2_PL_LLIMIT;
            if (*(Sint16 *)&ix->sprite.xposi > SMG2_PL_RLIMIT)
                *(Sint16 *)&ix->sprite.xposi = SMG2_PL_RLIMIT;
            if (*(Sint16 *)&ix->sprite.yposi < SMG2_PL_ULIMIT)
                *(Sint16 *)&ix->sprite.yposi = SMG2_PL_ULIMIT;
            if (*(Sint16 *)&ix->sprite.yposi > SMG2_PL_DLIMIT)
                *(Sint16 *)&ix->sprite.yposi = SMG2_PL_DLIMIT;


            if (swdata & PAD_B) {                   /* MAIN SHOT */
                if (ix->wcnt[0] == 0) {
                    ix->wcnt[0] = 3;
                    iy = SMTA_MakeActionX(SMG2_PlMainShot, 25, 4);
                    iy->sprite.xposi = ix->sprite.xposi;
                    iy->sprite.yposi = ix->sprite.yposi;
                } else 
                    ix->wcnt[0]--;

            }

            
            if (ix->wcnt[1] == 0)
                SMG2_PutObject(&ix->sprite);                        
            else                                     /* 無敵中は点滅する */
                if (ix->wcnt[1] & 1) SMG2_PutObject(&ix->sprite);   


            /* プレーヤーの座標、速度を保時 */
            SMG2_player_xposi = ix->sprite.xposi;
            SMG2_player_yposi = ix->sprite.yposi;
            SMG2_player_xspd = ix->sprite.xspd;
            SMG2_player_yspd = ix->sprite.yspd;

            SMCO_PutColBox(*(Sint16 *)&ix->sprite.xposi,
                           *(Sint16 *)&ix->sprite.yposi, ix->colino);
            break;			

          
        case YARARE_1:        /* プレーヤーやられ処理 */
            ix->sprite.patno = 1;
            ix->sprite.xspd = 0;
            ix->sprite.yspd = -0x100;
            ix->sprite.hzoom = 0x80;
            ix->sprite.vzoom = 0x80;
            ix->colino = 0;
            ix->wcnt[0] = 20;
            ix->mode++;
        case YARARE_2:
            ix->sprite.hzoom += 0x40;
            ix->sprite.vzoom += 0x40;

            ix->wcnt[0]--;
            if (ix->wcnt[0] < 0) {
                if (SMG2_player_zanki < 0) {
                    SMTA_KillAction(ix);
                    iy = SMTA_MakeActionX(SMG2_StageStart, 
                                          SMG2_ETCWK_TOP, SMG2_ETCWK_CNT);
                    iy->level = 2;  /* ゲームオーバー CALL */
                } else
                  ix->mode = INIT;
	    }
            SMG2_PutObject(&ix->sprite);
            break;			
    }

}




/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------
 *  プレーヤーショットコントロール
 *--------------------------------------------------------------
 *  プレーヤーの撃った弾の処理
 *  
 *  
 *--------------------------------------------------------------
 */
static void SMG2_PlMainShot(GAMEACT *ix)
{


    static const Sint16 sht_pat0[] = {
            2,
            rgb15(28, 20, 31),  -8, -8,  8, -8,  8, -6, -8, -6,
            rgb15(28, 20, 31),  -8,  6,  8,  6,  8,  8, -8,  8,
        };

    static Sint16 *shot_pat[] = {
            sht_pat0,
        };
		
    enum {
        INIT,
        MAIN
        };

    Sint16 dx, dy, d0, d1, d2, d3;
    GAMEACT *iy, *iys;

    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_npolygon;
            ix->sprite.pmode = SPM_ecd + SPM_sdd;
            ix->sprite.patbase = shot_pat;
            ix->sprite.patno = 0;
            ix->sprite.xspd =  0x700;
            ix->sprite.yspd =  0;
            ix->sprite.xacc = 0x20;
            ix->sprite.yacc = 0;

            ix->colino = 5;
            ix->coliatr = SMCO_atr_PLSHOT;
            ix->coliflg = 0;
            ix->atp = 1;

            ix->mode++;
        case MAIN:
            SMG2_player_hitcheck(ix);
            if (ix->coliflg & SMCO_dmg_ENEMY) {
                SMTA_KillAction(ix);
                break;
            }

            ix->sprite.xspd += ix->sprite.xacc;
            ix->sprite.yspd += ix->sprite.yacc;
            if (ix->sprite.xspd > 0xa00) 
                ix->sprite.xspd = 0xa00;

            ix->sprite.xposi += (ix->sprite.xspd << 8);
            ix->sprite.yposi += (ix->sprite.yspd << 8);

            if (*(Sint16 *)&ix->sprite.xposi > 0x140) {
                SMTA_KillAction(ix);
                break;
            }

            SMG2_PutObject(&ix->sprite);

            SMCO_PutColBox(*(Sint16 *)&ix->sprite.xposi, 
                           *(Sint16 *)&ix->sprite.yposi, ix->colino);
            break;			
    }
	
}



/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------
 *  背景をながれる星のプログラム
 *--------------------------------------------------------------
 *  
 *  
 *  
 *--------------------------------------------------------------
 */
static void SMG2_BackStar(GAMEACT *ix)
{
    enum {
        INIT,
        MAIN
        };

    GAMEACT *iy;
    Sint16 d0, d1;
	
    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_line;
            ix->sprite.pmode = SPM_ecd+SPM_sdd;
            ix->sprite.color = rgb(15, 15, 15) | MTH_GetRand();
			
            switch (ix->wcnt[0]) {         /* BASE SPEED */
                case 0:
                    ix->wreg[0] = 0x80;
                    break;
                case 1:
                    ix->wreg[0] = 0x100;
                    break;
                case 2:
                    ix->wreg[0] = 0x180;
                    break;
                case 3:
                    ix->wreg[0] = 0x40;
                    break;
            }
			
            ix->sprite.xposi = MTH_GetRand() & 0x1ffffff;
            if (ix->sprite.xposi > 0x1600000) ix->sprite.xposi -= 0xb00000;

            ix->sprite.yposi = MTH_GetRand() & 0xffffff;

            ix->wcnt[0] = SMG2_starlength;            /* line length */
            ix->mode++;

        case MAIN:
            ix->wcnt[0] = SMG2_starlength;
            ix->sprite.xspd = -(ix->wreg[0] + (ix->wcnt[0] * 0x10));

            ix->sprite.xposi += (ix->sprite.xspd << 8);
            if ((*(Sint16 *)&ix->sprite.xposi + ix->wcnt[0]) < 0x0)
                *(Sint16 *)&ix->sprite.xposi = 0x180;
			
            ix->sprite.posi[0][X] = *(Sint16 *)&ix->sprite.xposi;	
            ix->sprite.posi[0][Y] = *(Sint16 *)&ix->sprite.yposi;	
            ix->sprite.posi[1][X] = (*(Sint16 *)&ix->sprite.xposi) 
                                    + ix->wcnt[0];	
            ix->sprite.posi[1][Y] = *(Sint16 *)&ix->sprite.yposi;
            SMG2_PutObject(&ix->sprite);
            break;			
    }
	
}






/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------
 *  エネミーコリジョンチェック
 *--------------------------------------------------------------
 *  プレーヤー本体、プレーヤーの弾と敵と敵の弾、アイテムとの
 *  矩形の当たり判定をとる。
 *  
 *--------------------------------------------------------------
 */
static GAMEACT *SMG2_player_hitcheck(GAMEACT *ix)
{
    GAMEACT *iy;
    Uint16 i;
    Sint16 mxl, myl, mxh, myh, exl, eyl, exh, eyh;
    Uint8 reg, hitflag;

    mxl = *(Sint16 *)&ix->sprite.xposi + SMCO_colltbl[ix->colino].cxl;
    mxh = *(Sint16 *)&ix->sprite.xposi + SMCO_colltbl[ix->colino].cxh;
    myl = *(Sint16 *)&ix->sprite.yposi + SMCO_colltbl[ix->colino].cyl;
    myh = *(Sint16 *)&ix->sprite.yposi + SMCO_colltbl[ix->colino].cyh;

    hitflag = 0;
    ix->coliflg = 0x0; 


    
    for (i = SMG2_ENWK_TOP;
         i < (SMG2_ENWK_TOP + SMG2_ENWK_CNT + SMG2_ENTM_CNT + SMG2_ITWK_CNT);
         i++) {

        iy = (GAMEACT *)&SMTA_ActionBuf[i];

        if (iy->actid == 0) 
            continue;
        if (iy->colino == 0) 
            continue;

        exl = *(Sint16 *)&iy->sprite.xposi + SMCO_colltbl[iy->colino].cxl;
        exh = *(Sint16 *)&iy->sprite.xposi + SMCO_colltbl[iy->colino].cxh;
        eyl = *(Sint16 *)&iy->sprite.yposi + SMCO_colltbl[iy->colino].cyl;
        eyh = *(Sint16 *)&iy->sprite.yposi + SMCO_colltbl[iy->colino].cyh;


        if (exh < mxl) 
            continue;
        if (mxh < exl) 
            continue;
        if (eyh < myl) 
            continue;
        if (myh < eyl) 
            continue;

        /* HIT!! */
        hitflag = 1;

        reg = iy->coliatr;    /* ENEMY'S collision attribute flag copy */
        ix->coliflg |= reg;

        reg = ix->coliatr;    /* PLAYER'S collision attribute flag copy */
        iy->coliflg |= reg;

        iy->hp -= ix->atp;    /* HIT POINT AUTO DECREMENT */
    }

    if (hitflag)
        return iy;
    else
        return NULL;
}



/*==========================================================================*/
/*--------------------------------------------------------------
 *  エネミーやられ処理
 *--------------------------------------------------------------
 *  エネミーl爆発処理を行なう
 *  
 *  
 *--------------------------------------------------------------
 */
void SMG2_EnemyBokan(GAMEACT *ix)
{

/*  枚数,
    CharNo, HVreverse, Ｈサイズ, Ｖサイズ, Ｈオフセット, Ｖオフセット */
    static const Sint16 bokan_pat0[] = {
            1,
            SMG2_Kemu0CharNo, 0, 16, 16, -8, -8
        };
    static const Sint16 bokan_pat1[] = {
            1,
            SMG2_Kemu1CharNo, 0, 16, 16, -8, -8
        };
    static const Sint16 bokan_pat2[] = {
            1,
            SMG2_Kemu2CharNo, 0, 16, 16, -8, -8
        };
    static const Sint16 bokan_pat3[] = {
            1,
            SMG2_Kemu3CharNo, 0, 16, 16, -8, -8
        };
    static const Sint16 bokan_pat4[] = {
            1,
            SMG2_Kemu4CharNo, 0,  8,  8, -4, -4
        };


    static Sint16 *bokan_pat[] = {
            bokan_pat0, bokan_pat1, bokan_pat2, bokan_pat3, bokan_pat4,
          
        };
		
    enum {
        INIT,
        MAIN,
        NEXT
        };

    Uint8 foutflag;
    Sint16 dx, dy, d0, d1, d2, d3;
    GAMEACT *iy, *iys;

    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_zoom;
            ix->sprite.pmode = SPM_ecd;
            ix->sprite.patbase = bokan_pat;
            ix->sprite.patno = 0;
            ix->sprite.color = SMG2_KemuPalNo;
            if (ix->level == 1) {
                ix->sprite.hzoom = 0x400;        /* BOSS BOKAN FLAG */
                ix->sprite.vzoom = 0x400;
            } else {
                ix->sprite.hzoom = 0x100;        /* ZAKO BOKAN FLAG */
                ix->sprite.vzoom = 0x100;
            }    
            ix->colino = 0;
            ix->wcnt[0] = 0;
            ix->mode++;
        case MAIN:
            ix->sprite.xposi += (ix->sprite.xspd << 8);
            ix->sprite.yposi += (ix->sprite.yspd << 8);
            ix->sprite.hzoom += ix->wcnt[1];
            ix->sprite.vzoom += ix->wcnt[1];

            ix->wcnt[0]++;
            if (ix->wcnt[0] > 3) {
                ix->sprite.patno++;
                ix->wcnt[0]= 0;
            }

            foutflag = SMG2_EnemyFrameOutChk(*(Sint16 *)&ix->sprite.xposi, 
                                             *(Sint16 *)&ix->sprite.yposi);
            if ((foutflag == 1) || (ix->sprite.patno > 4)) {
                if (ix->level != 1) {  /* ボスの爆発か？ */
                    SMTA_KillAction(ix);
                    break;
                } else {
                    ix->mode++; /*ボスの爆発ならステージクリアチェックをする*/
                    break;
                }
            }
            SMG2_PutObject(&ix->sprite);
            break;
        case NEXT:
            if ((SMG2_gamestatus & SMG2_gmsts_PLDEAD) == 0) {
                SMG2_gamestatus |= SMG2_gmsts_ENSETOFF;
                iy = SMTA_MakeActionX(SMG2_StageStart, 
                                      SMG2_ETCWK_TOP, SMG2_ETCWK_CNT);
                iy->level = 1; /* STAGE CLEAR CALL */
                SMTA_KillAction(ix);
            }
            break;
    }

	
}




/*==========================================================================*/
/*--------------------------------------------------------------
 *  当たり判定表示
 *--------------------------------------------------------------
 *  デバッグ用に当たり判定をポリラインで矩形表示する
 *  ただしスプライト枚数が増えるので処理落ちしやすく
 *  なります。
 *--------------------------------------------------------------
 */
void SMCO_PutColBox(Sint16 xposi, Sint16 yposi, Uint8 colino)
{
	        
    Uint16 hvrev, pmode, color, cgadr, cgsize, growno;
    XyInt posi[4];

#if _SMG2_COLLBOX_
    pmode = SPM_ecd + SPM_sdd;
    color = rgb15(28, 0, 0);
    posi[0].x = xposi + SMCO_colltbl[colino].cxl;
    posi[0].y = yposi + SMCO_colltbl[colino].cyl;
    posi[1].x = xposi + SMCO_colltbl[colino].cxh;
    posi[1].y = yposi + SMCO_colltbl[colino].cyl;
    posi[2].x = xposi + SMCO_colltbl[colino].cxh;
    posi[2].y = yposi + SMCO_colltbl[colino].cyh;
    posi[3].x = xposi + SMCO_colltbl[colino].cxl;
    posi[3].y = yposi + SMCO_colltbl[colino].cyh;
    SPR_2PolyLine(0, pmode, color, &posi[0], 0xffff);
#endif

}





/*==========================================================================*/
/*--------------------------------------------------------------
 *  スコア、プレーヤー残り表示
 *--------------------------------------------------------------
 *  「SCORE：」「LEFT：」の表示をします
 *  
 *  
 *--------------------------------------------------------------
 */
static void SMG2_ScoreDispAct(GAMEACT *ix)
{

/*  枚数,
    Charno, HVreverse, Ｈサイズ, Ｖサイズ, Ｈオフセット, Ｖオフセット */
    static const Sint16 score_pat[] = {
            11,
            ('S' - ' ') , 0, 0x0008, 0x0008,     0,   0,
            ('C' - ' ') , 0, 0x0008, 0x0008,     8,   0,
            ('O' - ' ') , 0, 0x0008, 0x0008,    16,   0,
            ('R' - ' ') , 0, 0x0008, 0x0008,    24,   0,
            ('E' - ' ') , 0, 0x0008, 0x0008,    32,   0,
            (':' - ' ') , 0, 0x0008, 0x0008,    40,   0,

            ('L' - ' ') , 0, 0x0008, 0x0008,     8,  10,
            ('E' - ' ') , 0, 0x0008, 0x0008,    16,  10,
            ('F' - ' ') , 0, 0x0008, 0x0008,    24,  10,
            ('T' - ' ') , 0, 0x0008, 0x0008,    32,  10,
            (':' - ' ') , 0, 0x0008, 0x0008,    40,  10,

        };


    static Sint16 *scdisp_pat[] = {
            score_pat,      /* 0 */
        };
		
    enum {
         INIT,
         MAIN
         };


    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_nrm;
            ix->sprite.pmode = SPM_ecd;
            ix->sprite.patbase = scdisp_pat;

            ix->sprite.patno = 0;              /* SCORE: */
            ix->sprite.xposi = 0x10 * 0x10000;
            ix->sprite.yposi = 0x04 * 0x10000;
            ix->sprite.color = 0x10;
            ix->mode++;
        case MAIN:
            SMG2_PutObject(&ix->sprite);
            break;
    }

}




/*--------------------------------------------------------------
 *  スコア、プレーヤー残り表示
 *--------------------------------------------------------------
 *  スコア、プレーヤー残り数を表示します。ワークRAMにスプライト
 *  表示のための作業バッファを設け、そこへ数字のキャラクタNoを
 *  書き込んでいます。
 *--------------------------------------------------------------
 */
static void SMG2_ScoreNumAct(GAMEACT *ix)
{

/*  枚数,
    Charno, HVreverse, Ｈサイズ, Ｖサイズ, Ｈオフセット, Ｖオフセット */
    static const Sint16 scorenum_pat[] = {
            11,
            ('0' - ' ') , 0, 0x0008, 0x0008,     0,   0,
            ('0' - ' ') , 0, 0x0008, 0x0008,     8,   0,
            ('0' - ' ') , 0, 0x0008, 0x0008,    16,   0,
            ('0' - ' ') , 0, 0x0008, 0x0008,    24,   0,
            ('0' - ' ') , 0, 0x0008, 0x0008,    32,   0,
            ('0' - ' ') , 0, 0x0008, 0x0008,    40,   0,
            ('0' - ' ') , 0, 0x0008, 0x0008,    48,   0,

            ('0' - ' ') , 0, 0x0008, 0x0008,     0,  10,
            ('0' - ' ') , 0, 0x0008, 0x0008,     8,  10,
            ('0' - ' ') , 0, 0x0008, 0x0008,    16,  10,

        };

    const Sint16 decchr_tbl[] = {
    ('0' - ' '), ('1' - ' '), ('2' - ' '), ('3' - ' '), ('4' - ' '), 
    ('5' - ' '), ('6' - ' '), ('7' - ' '), ('8' - ' '), ('9' - ' '),
    };


    static Sint16 *numdisp_pat[] = {
            &SMG2_scorepat[0],   /* WORKRAMに確保したバッファのアドレス */
        };
		
    enum {
         INIT,
         MAIN
         };

    Uint32 number;
    Sint16 scnt[7];
    Sint16 i, j , hofs, vofs, maisu;
    Sint16 *pattbl;
    Uint8 zeroflag;



    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_nrm;
            ix->sprite.pmode = SPM_ecd + SPM_sdd;
            ix->sprite.patbase = numdisp_pat;
            ix->sprite.patno = 0;              /* SCORE: */
            ix->sprite.xposi = 0x40 * 0x10000;
            ix->sprite.yposi = 0x04 * 0x10000;
            ix->sprite.color = 0x10;
            ix->mode++;
        case MAIN:
            number = SMG2_score;
            scnt[0] = decchr_tbl[SMMI_getcount(&number, 1000000)];
            scnt[1] = decchr_tbl[SMMI_getcount(&number, 100000)];
            scnt[2] = decchr_tbl[SMMI_getcount(&number, 10000)];
            scnt[3] = decchr_tbl[SMMI_getcount(&number, 1000)];
            scnt[4] = decchr_tbl[SMMI_getcount(&number, 100)];
            scnt[5] = decchr_tbl[SMMI_getcount(&number, 10)];
            scnt[6] = decchr_tbl[SMMI_getcount(&number, 1)];

            maisu = 0;
            hofs = 0;
            vofs = 0;
            zeroflag = 0;
            pattbl = &SMG2_scorepat[1];
            for (i = 0; i < 7; i++) {
                if (scnt[i] != ('0' - ' ') || (i == 6))
                    zeroflag = 1;

                if (zeroflag == 1) {
                    *pattbl++ = scnt[i];
                    *pattbl++ = 0;
                    *pattbl++ = 0x8;
                    *pattbl++ = 0x8;
                    *pattbl++ = hofs;
                    *pattbl++ = 0;
                    maisu++;
                    hofs += 8;
                }
            }



            number = SMG2_player_zanki;
            if (number > 9)
                number = 9;
            if (number < 0)
                number = 0;
            scnt[0] = decchr_tbl[SMMI_getcount(&number, 100)];
            scnt[1] = decchr_tbl[SMMI_getcount(&number, 10)];
            scnt[2] = decchr_tbl[SMMI_getcount(&number, 1)];

            hofs = 0;
            zeroflag = 0;
            for (i = 0; i < 3; i++) {
                if (scnt[i] != ('0' - ' ') || (i == 2))
                    zeroflag = 1;

                if (zeroflag == 1) {
                    *pattbl++ = scnt[i];
                    *pattbl++ = 0;
                    *pattbl++ = 0x8;
                    *pattbl++ = 0x8;
                    *pattbl++ = hofs;
                    *pattbl++ = 0xa;
                    maisu++;
                    hofs += 8;
                }
            }

            SMG2_scorepat[0] = maisu;
            SMG2_PutObject(&ix->sprite);
            break;
    }
}


/*--------------------------------------------------------------
 *  スコア加算
 *--------------------------------------------------------------
 *  スコアを加算します。スコアの上限は9,999,999です
 *  
 *  
 *--------------------------------------------------------------
 */
void SMG2_AddScore(Sint32 addpts)
{
    Sint32 d0;

    d0 = SMG2_score;
    d0 += addpts;

    if (d0 > 9999999)
        d0 = 9999999;

    if (d0 > SMG2_hiscore)
        SMG2_hiscore = d0;

    SMG2_score = d0;
}






/*
 *    背景ラインスクロールコントール(ステージ２のみ)
 */
void SMG2_Stage2scrollAct(STG2SCLACT *ix)
{

    enum {
        INIT,
        MAIN,
        NEXT 
    };

    Uint16 i;
    Sint16 hldirec, hzdirec;
    Sint32 hldata, hzdata;
    Sint32 vsdata;
    SclLineTb *lineptr;


    switch (ix->mode) {
        case INIT:
            ix->hlwk = 0;
            ix->vswk[0] = 0;
            ix->vswk[1] = 0;


            for (i = 0; i < 0xe0; i++) {
                 hzdirec = (i * 0x80) / 0xe0;
                 hzdata = 0x4000 + 0xc0 * SMMI_Sinset8(hzdirec);
                 SMV2_lparam.line_tbl[i].dh = hzdata;
            }
            ix->mode++;

        case MAIN:

            /* スクロール量更新 */
            ix->hlwk++;
            ix->hlwk += (SMG2_starlength / 0x80);

            lineptr = &SMV2_lparam.line_tbl[0];
             for (i = 0; i < 0xe0; i++) {
                  hzdata = lineptr->dh;
                  hldata = (WIDTH_H/2 * 0x10000) - (WIDTH_H/2 * hzdata);
                  hldata += (ix->hlwk * 0x10000);
                  vsdata = (ix->vswk[0] + i) * 0x10000;
                  lineptr->h = hldata;
                  lineptr->v = vsdata;
                  lineptr++;
             }
    }	    

    /* ラインスクロールパラメータセット */
    SCL_Open(SCL_NBG0); 
    SMV2_lparam.h_enbl = ON;        /* h方向スクロールする  */
    SMV2_lparam.v_enbl = ON;        /* v方向スクロールする  */
    SMV2_lparam.delta_enbl = ON;    /* ラインズームする     */
    SMV2_lparam.cell_enbl = OFF;    /* 縦セルスクロールする */
    SMV2_lparam.interval = 0;       /* 1ライン間隔          */
    SMV2_lparam.line_addr = NBG0_HLSCLTBL; /* H-LINESCRL TABLE VRAM ADD SET */
    SMV2_lparam.cell_addr = NBG0_VCSCLTBL; /* V-CELLSCRL TABLE VRAM ADD SET */
    SCL_SetLineParam(&SMV2_lparam);
    SCL_Close();        

}





