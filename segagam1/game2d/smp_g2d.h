/* Copyright(c) 1994 SEGA			*/
/*
 *
 *   SAMPLE YOKO SYUU GAME HEADER FILE
 *
 */


#ifndef __SMP_G2D_H
#define __SMP_G2D_H


/* 
 *    アクションワークの使用取り決め 
 */
#define SMG2_SYSWK_TOP 0        /* game system work top               */
#define SMG2_SYSWK_CNT 8        /* game system work search count      */

#define SMG2_BSTAR_TOP 8        /* back ground star work top          */
#define SMG2_BSTAR_CNT 16       /* back ground star work seatch count */

#define SMG2_PLWK_TOP 24        /* Player action work top             */
#define SMG2_PLTM_TOP 25        /* Player shot work top               */
#define SMG2_PLTM_CNT 16        /* Player shot work search count      */

#define SMG2_ENWK_TOP 41        /* enemy search work top              */
#define SMG2_ENWK_CNT 32        /* enemy work search count            */

#define SMG2_ENTM_TOP 73        /* enemy's tama search work top       */
#define SMG2_ENTM_CNT 32        /* enemy's tama search count          */

#define SMG2_ITWK_TOP 105       /* Item action search work top        */
#define SMG2_ITWK_CNT 8         /* Item action search count           */

#define SMG2_ETCWK_TOP 120      /* etc action work top                */
#define SMG2_ETCWK_CNT 8        /* etc action work search count       */


/*
 *     プレーヤーアクションの定数 
 */
#define SMG2_PL_XSPD 0x300      /* xスピード                          */
#define SMG2_PL_YSPD 0x280      /* Yスピード                          */
#define SMG2_PL_NXSPD 0x280     /* 斜めxスピード                      */
#define SMG2_PL_NYSPD 0x200     /* 斜めyスピード                      */
#define SMG2_PL_BSPD 0x100
#define SMG2_PL_NBSPD 0x100

#define SMG2_PL_LLIMIT 0x18           /* PLAYER X POSI LEFT LIMIT     */
#define SMG2_PL_RLIMIT (0x140 - 0x10) /* PLAYER X POSI RIGHT LIMIT    */
#define SMG2_PL_ULIMIT 0x24           /* PLAYER Y POSI UP LIMIT       */
#define SMG2_PL_DLIMIT (0xe0 - 0x10)  /* PLAYER Y POSI DOWN LIMIT     */


/*
 *     スプライトキャラクタ No 
 */
#define SMG2_PlayCharNo 0x40    /* プレーヤーキャラクタNo      */
#define SMG2_BossCharNo 0x41    /* ボスキャラクタ No           */

#define SMG2_Lrun1CharNo 0x42    /* ザコキャラクタNo            */
#define SMG2_Lrun2CharNo 0x43    /* ザコキャラクタNo            */
#define SMG2_Lrun3CharNo 0x44    /* ザコキャラクタNo            */
#define SMG2_Lrun4CharNo 0x45    /* ザコキャラクタNo            */
#define SMG2_Lrun5CharNo 0x46    /* ザコキャラクタNo            */


#define SMG2_Kemu0CharNo 0x47    /* 爆発キャラクタNo            */
#define SMG2_Kemu1CharNo 0x48    /* 爆発キャラクタNo            */
#define SMG2_Kemu2CharNo 0x49    /* 爆発キャラクタNo            */
#define SMG2_Kemu3CharNo 0x4a    /* 爆発キャラクタNo            */
#define SMG2_Kemu4CharNo 0x4b    /* 爆発キャラクタNo            */

#define SMG2_Item0CharNo  0x4c   /* アイテムキャラクタNo        */
#define SMG2_Item1CharNo  0x4d   /* アイテムキャラクタNo        */



/* 
 *    スプライトパレット No 
 */
#define SMG2_PlayPalNo 0x20    /* プレーヤーパレットNo      */
#define SMG2_BossPalNo 0x30    /* ボスパレット No           */
#define SMG2_LrunPalNo 0x50    /* ザコパレットNo            */
#define SMG2_KemuPalNo 0x50    /* 爆発パレット No           */
#define SMG2_DmgPalNo  0x40    /* ダメージパレット No       */
#define SMG2_ItemPalNo 0x50    /* アイテムパレットNo        */



/* 
 *    ゲームステータスフラグ
 */
#define SMG2_gmsts_GAMEOVER  0x1        /* ゲームオーバー             */
#define SMG2_gmsts_NEXTSTAGE 0x2        /* ステージクリア             */
#define SMG2_gmsts_ENSETOFF  0x4        /* エネミーセット禁止         */
#define SMG2_gmsts_PLDEAD    0x8        /* プレーヤーが死んでいる     */



/* 
 *     １にするとコリジョンの範囲をポリラインで表示します
 *     ただし描画枚数が増えるので処理落ちしやすくなります
 */
#define _SMG2_COLLBOX_ 0      /* 0..範囲表示なし 1..表示あり */


/* 
 *    コリジョンフラグアサイン
 */
#define SMCO_atr_PLAYER 0x01
#define SMCO_atr_PLSHOT 0x02
#define SMCO_atr_ENEMY  0x04
#define SMCO_atr_ENSHOT 0x08
#define SMCO_atr_ITEM   0x10

#define SMCO_dmg_PLAYER 0x01
#define SMCO_dmg_PLSHOT 0x02
#define SMCO_dmg_ENEMY  0x04
#define SMCO_dmg_ENSHOT 0x08
#define SMCO_dmg_ITEM   0x10



/*
 *     エネミー発生テーブル構造体 
 */
typedef struct ENSET_DATA {
    Uint16 timer;               /* Enemy set timer (1/60sec.)        */
    Uint16 level;               /* set level                         */
    void  *execadd;             /* exec action entry                 */
    Uint16 wkstart;             /* work start count                  */
    Uint16 srchcnt;             /* search count                      */
    Uint16 xposi;               /* set xposi                         */
    Uint16 yposi;               /* set yposi                         */
} ENSET_DATA;


/* 
 *     COLLISION TABLE 構造体
 */
typedef struct SMCO_COLLTBL {
    Sint16 cxl;
    Sint16 cyl;
    Sint16 cxh;
    Sint16 cyh;
} SMCO_COLLTBL;




/*
 *     ゲームアクションワークアサイン
 */
SMTA_DefActWk(GAMEACT,
         SPRITE sprite;            /* SPRITE DISPLAY          */
         Sint16 wreg[4];           /* 汎用レジスタ            */
         Sint16 wcnt[4];           /* 汎用カウンタ            */
         Uint8 colino;             /* collision table index   */
         Uint8 coliatr;            /* collision attribute     */
         Uint8 coliflg;            /* collision flag          */ 
         Uint8 atp;                /* kougeki ryoku           */
         Sint16 hp;                /* hit point               */
);


/*
 *     エネミーセットアクションワークアサイン
 */
SMTA_DefActWk(ENSETACT,
         struct ENSET_DATA *setptr;
         Sint16 wreg[4];
         Sint16 wcnt[4];
);


/*
 *     スクロールアクションワークアサイン
 */
SMTA_DefActWk(STG2SCLACT,
         Sint16 hlwk;
         Sint16 vswk[2];
);



/*
 *     関数プロトタイプ
 */
extern void SMG2_GameMode(void);
extern Uint8 SMG2_EnemyFrameOutChk(Sint16 xposi, Sint16 yposi);
extern void SMG2_StageStart(GAMEACT *ix);   /* 'READY'表示             */
extern void SMG2_EnemyBokan(GAMEACT *ix);   /* Enemy Bokan action      */
extern void SMG2_AddScore(Sint32 addpts);   /* ADD SCORE               */
extern void SMG2_PutObject(SPRITE *spr);
extern void SMCO_PutColBox(Sint16 xposi, Sint16 yposi, Uint8 colino);

extern const SMCO_COLLTBL SMCO_colltbl[];


/* 
 *    ワークRAM
 */



#endif



