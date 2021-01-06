/* Copyright(c) 1994 SEGA			*/
/*
 *
 * サンプル2Dゲームエネミーアクションプログラム
 *
 */

#include "smp_bios.h"
#include "smp_g2cg.h"
#include "smp_g2d.h"
#include "smp_g2d1.h"


extern Sint32 SMG2_player_xposi;
extern Sint32 SMG2_player_yposi;
extern Sint32 SMG2_player_xspd;
extern Sint32 SMG2_player_yspd;
extern Sint32 SMG2_score;
extern Sint32 SMG2_hiscore;
extern Sint8 SMG2_player_zanki;
extern Uint8 SMG2_stageno;
extern Uint8 SMG2_gamestatus;
extern Uint8 SMG2_pauseflag;
extern Uint32 SMG2_mainloop_cnt;
extern Sint16 SMG2_starlength;
extern Uint16 SMG2_objcount;
extern SPRITE *SMG2_objbuff[0x100];
extern Sint16 SMG2_scorepat[64];


/*
 *    関数プロトタイプ
 */
static void SMG2_Enemy2TamaSet(GAMEACT *ix, Uint8 direc, Uint8 sped);
static Uint8 SMG2_Enemy2NextModeSet(GAMEACT *ix);
static void SMG2_Enemy2_REST(GAMEACT *ix);
static void SMG2_Enemy2_WAY3(GAMEACT *ix);
static void SMG2_Enemy2_HAJYO(GAMEACT *ix);
static void SMG2_Enemy2_HOMING(GAMEACT *ix);


static void SMG2_EneTamaAct(GAMEACT *ix);            /* Enemy's Normal Shot */
static void SMG2_MakeBokan(GAMEACT *ix);             /* Enemy Bokan set     */
static void SMG2_MakeBossBokan(GAMEACT *ix);         /* Boss  Bokan set     */

static void SMG2_EneTamaAct2(GAMEACT *ix);
static void SMG2_EneTamaAct2Sub(GAMEACT *ix);

static void SMG2_ItemAct(GAMEACT *ix);


/*
 *    ENEMY BOKAN SET
 */
static void SMG2_MakeBokan(GAMEACT *ix)
{
    ix->actid = 1;
    ix->pcbuff = SMG2_EnemyBokan;
    ix->mode = 0;
    ix->status = 0;
    ix->level = 0;                                 /* ZAKO BOKAN FLAG SET */
    ix->wcnt[1] = 0x20;                            /* ZOOM SPEED          */
}

/*
 *    ENEMY-BOSS class  BOKAN SET
 */
static void SMG2_MakeBossBokan(GAMEACT *ix)
{
    ix->actid = 1;
    ix->pcbuff = SMG2_EnemyBokan;
    ix->mode = 0;
    ix->status = 0;
    ix->level = 1;                                  /* BOSS BOKAN FLAG SET */
    ix->wcnt[1] = 0x100;                             /* ZOOM SPEED          */

}



/*==========================================================================*/
/*--------------------------------------------------------------
 *  エネミー１を発生させる
 *--------------------------------------------------------------
 *
 *  
 *  
 *--------------------------------------------------------------
 */
void SMG2_Enemy1Set(GAMEACT *ix)
{

    enum {
        INIT,
        ENTYPE1, 
        ENTYPE2,
        ENTYPE3
        };

    GAMEACT *iy;
    Sint16 d0, d1;
	
    switch (ix->mode) {
        case INIT:
            ix->wcnt[0] = 0;
            ix->wcnt[1] = 0;
            ix->wreg[0] = 0;
            
            switch (ix->level) {
                case 0:
                    ix->mode = ENTYPE1;
                    break;
                case 1:
                    ix->mode = ENTYPE2;
                    break;
                case 2:
                    ix->mode = ENTYPE3;
                    break;
            }
            break;

        case ENTYPE1:
            if (ix->wcnt[0] > 0x100) {
                iy = SMTA_MakeActionX(SMG2_Enemy1Act, 
                                      SMG2_ENWK_TOP, SMG2_ENWK_CNT);
                iy->sprite.xposi = ix->sprite.xposi;
                iy->sprite.yposi = ix->sprite.yposi;
                iy->sprite.xspd = -0x200;
                iy->sprite.yspd = 0;
                iy->wcnt[0] = 0x40;    /* TAMA DASI COUNT */

                if (ix->wcnt[1] == 5-1 )
                    iy->wreg[0] = 1;   /* ITEM ENEMY    */
                else
                    iy->wreg[0] = 0;   /* NO ITEM ENEMY */

                ix->wcnt[0] &= 0xff;
                ix->wcnt[1]++;
            }
            ix->wcnt[0] += 0x14;

            if (ix->wcnt[1] > (5 - 1)) 
                SMTA_KillAction(ix);
            break;


        case ENTYPE2:
            if (ix->wcnt[0] > 0x100) {
                iy = SMTA_MakeActionX(SMG2_Enemy1Act, 
                                      SMG2_ENWK_TOP, SMG2_ENWK_CNT);
                iy->sprite.xposi = ix->sprite.xposi;
                d0 = (MTH_GetRand() >> 16) & 0x7f;
                *(Sint16 *)&iy->sprite.yposi = *(Sint16 *)(&ix->sprite.yposi)
                                             + d0;

                iy->sprite.xspd = -0x400;
                if (*(Sint16 *)&iy->sprite.yposi > 0x70)
                    iy->sprite.yspd = -0x120;
                else
                    iy->sprite.yspd = 0x120;

                iy->wcnt[0] = 0x10;    /* TAMA DASI COUNT */
                iy->wreg[0] = 0;

                ix->wcnt[0] &= 0xff;
                ix->wcnt[1]++;
            }
            ix->wcnt[0] += 0x20;

            if (ix->wcnt[1] > (16 - 1)) 
                SMTA_KillAction(ix);
            break;

        case ENTYPE3:
            if (ix->wcnt[0] > 0x100) {
                iy = SMTA_MakeActionX(SMG2_Enemy1Act, 
                                      SMG2_ENWK_TOP, SMG2_ENWK_CNT);
                iy->sprite.xposi = ix->sprite.xposi;
                d0 = (MTH_GetRand() >> 16) & 0x7f;
                *(Sint16 *)&iy->sprite.yposi = *(Sint16 *)(&ix->sprite.yposi) 
                                             + d0;

                iy->sprite.xspd = -0x500;
                iy->sprite.yspd = 0;
                iy->wcnt[0] = 0x200;    /* TAMA DASI COUNT */
                iy->wreg[0] = 0;

                ix->wcnt[0] &= 0xff;
                ix->wcnt[1]++;
            }
            ix->wcnt[0] += 0x20;

            if (ix->wcnt[1] > (20 - 1)) 
                SMTA_KillAction(ix);
            break;

     }

}

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------
 *  エネミー１アクションコントロール
 *--------------------------------------------------------------
 *  ちびソニックが画面右から元気よく走ってくる
 *  
 *  
 *--------------------------------------------------------------
 */
void SMG2_Enemy1Act(GAMEACT *ix)
{

/*  枚数,
    Charno, HVreverse, Ｈサイズ, Ｖサイズ, Ｈオフセット, Ｖオフセット */
    static const Sint16 enmy_pat0[] = {
            1,
            SMG2_Lrun1CharNo, SPR_Hrev, 16, 24, -8, -12
        };
    static Sint16 enmy_pat1[] = {
            1,
            SMG2_Lrun2CharNo, SPR_Hrev, 16, 24, -8, -12
        };
    static Sint16 enmy_pat2[] = {
            1,
            SMG2_Lrun3CharNo, SPR_Hrev, 16, 24, -8, -12
        };
    static Sint16 enmy_pat3[] = {
            1,
            SMG2_Lrun4CharNo, SPR_Hrev, 16, 24, -8, -12
        };
    static Sint16 enmy_pat4[] = {
            1,
            SMG2_Lrun5CharNo, SPR_Hrev, 16, 24, -8, -12
        };


    static Sint16 *enemy_pat[] = {
            enmy_pat0, enmy_pat1, enmy_pat2, enmy_pat3, enmy_pat4
        };
		
    enum {
        INIT,
        MAIN
        };

    Uint8 flag;
    Sint16 d0, d1;
    GAMEACT *iy;
	
    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_nrm;
            ix->sprite.pmode = 0;
            ix->sprite.patbase = enemy_pat;
            ix->sprite.patno = 0;
            ix->sprite.color = SMG2_LrunPalNo;

            ix->colino = 4;
            ix->coliatr = SMCO_atr_ENEMY;
            ix->coliflg = 0;

            ix->wcnt[1] = 0;       /* TAMA DASI FLAG  */

            ix->mode++;
        case MAIN:

            /* 当たり判定のチェック */
            if (ix->coliflg & SMCO_dmg_PLSHOT) {
                SMG2_MakeBokan(ix);
                SMG2_AddScore(100);
                ix->coliflg = 0;
                if (ix->wreg[0] == 1) {
                    iy = SMTA_MakeActionX(SMG2_ItemAct, 
                                          SMG2_ITWK_TOP, SMG2_ITWK_CNT);
                    iy->sprite.xposi = ix->sprite.xposi;
                    iy->sprite.yposi = ix->sprite.yposi;
                }
                break;
            }

            /* パターンチェンジのタイミングをメインループカウンタで調整 */
            d0 = SMG2_mainloop_cnt;
            if (ix->sprite.xspd > -0x400) {
                if ((d0 & 3) == 0) {
                    ix->sprite.patno++;
                    if (ix->sprite.patno > 2)
                        ix->sprite.patno = 0;
                }
            } else {
                if ((d0 & 1) == 0)
                    ix->sprite.patno = 3;
                else
                    ix->sprite.patno = 4;
            }


            /* キャラクタの移動                               */
            /* xposi,yposiは32ビット, xspd,yspdは16ビットです */
            ix->sprite.xposi += (ix->sprite.xspd << 8);
            ix->sprite.yposi += (ix->sprite.yspd << 8);


            /* 弾だしのコントロール */
            ix->wcnt[0]--;
            if ((ix->wcnt[0] == 0) && (ix->wcnt[1] == 0)) {
                iy = SMTA_MakeActionX(SMG2_EneTamaAct, 
                                      SMG2_ENTM_TOP, SMG2_ENTM_CNT);
                iy->sprite.xposi = ix->sprite.xposi;
                iy->sprite.yposi = ix->sprite.yposi;
                d0 = *(Sint16 *)&SMG2_player_xposi 
                   - *(Sint16 *)&ix->sprite.xposi;
                d1 = *(Sint16 *)&SMG2_player_yposi 
                   - *(Sint16 *)&ix->sprite.yposi;
                d0 = SMMI_atan(d0, d1);
                iy->sprite.xspd = SMMI_Cosset8(d0) * 3;
                iy->sprite.yspd = SMMI_Sinset8(d0) * 3;
                ix->wcnt[0] = 0x40;
                ix->wcnt[1] = 1;
            }

            /* 画面左端まできたら自殺する */
            if (ix->sprite.xposi < -0x10) {
                SMTA_KillAction(ix);
                break;
	    }

            /* スプライト表示セット   */
            SMG2_PutObject(&ix->sprite);

            /* 当たり判定枠表示セット */
            SMCO_PutColBox(*(Sint16 *)&ix->sprite.xposi, 
                           *(Sint16 *)&ix->sprite.yposi, ix->colino);
            break;			
    }
	
}




/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------
 *  エネミー２アクションコントロール
 *--------------------------------------------------------------
 *  ラウンド１の最後にでてくるボスのアクション
 *  
 *  
 *--------------------------------------------------------------
 */
void SMG2_Enemy2Act(GAMEACT *ix)
{

/*  枚数,
    Charno, HVreverse, Ｈサイズ, Ｖサイズ, Ｈオフセット, Ｖオフセット */
    static Sint16 enmy2_pat0[] = {
            1,
            SMG2_BossCharNo, 0, 64, 64, -32, -32
        };



    static Sint16 *enemy2_pat[] = {
            enmy2_pat0
        };

    static void (*SMG2_Enemy2acttbl[])(GAMEACT *ix) = {
            SMG2_Enemy2_REST,
            SMG2_Enemy2_WAY3,
            SMG2_Enemy2_HAJYO,
            SMG2_Enemy2_HOMING
        };



		
    enum {
        INIT,
        MOVE1,
        MOVE2
        };

    Uint8 flag;
    Sint16 d0, d1;

    GAMEACT *iy;

	
    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_rotzoom;
            ix->sprite.pmode = 0;
            ix->sprite.patbase = enemy2_pat;
            ix->sprite.patno = 0;
            ix->sprite.patchgcnt = 0;
            ix->sprite.color = 0x0;
            ix->sprite.hzoom = 0x100;
            ix->sprite.vzoom = 0x100;
            ix->sprite.xspd = -0x100;
            ix->sprite.yspd = 0;
            ix->sprite.zdirec = 0;
            ix->sprite.color = SMG2_BossPalNo;

            ix->colino = 7;
            ix->coliatr = SMCO_atr_ENEMY;
            ix->coliflg = 0;
            ix->hp = 0x120;


            ix->wcnt[0] = 0;   /* 弾だしカウンタ */
            ix->wcnt[1] = 0;   /* 弾だしフラグ   */
            ix->wreg[0] = 0;   /* 攻撃パターンセレクト */
            ix->wreg[1] = 0;   /* 攻撃種類       */


            ix->mode++;
        case MOVE1:
            if (*(Sint16 *)&ix->sprite.xposi < 0x100) {
               ix->sprite.xspd = 0x0000;
               *(Sint16 *)&ix->sprite.xposi = 0x100;
               ix->mode++;
	    }
            ix->sprite.xposi += (ix->sprite.xspd << 8);
            ix->sprite.yposi += (ix->sprite.yspd << 8);
            break;

        case MOVE2:
            (SMG2_Enemy2acttbl[ix->wreg[1]])(ix);
             break;
    }

    ix->sprite.color = SMG2_BossPalNo;            /* COLLISION CHECK */
    if (ix->coliflg & SMCO_dmg_PLSHOT) {
        ix->coliflg = 0;
        ix->sprite.color = SMG2_DmgPalNo;
        if (ix->hp < 0) {
            SMG2_MakeBossBokan(ix);
            SMG2_AddScore(2000);
        }
    }

    ix->sprite.zdirec++;	
    SMG2_PutObject(&ix->sprite);
    SMCO_PutColBox(*(Sint16 *)&ix->sprite.xposi, *(Sint16 *)&ix->sprite.yposi,
                   ix->colino);
    d0 = *(Sint16 *)&ix->sprite.xposi - 0x20;
    d1 = *(Sint16 *)&ix->sprite.yposi + 0x18;
    SMMI_PutHexNum32(d0, d1, ix->hp);

}


/*
 *    弾セット
 */
static void SMG2_Enemy2TamaSet(GAMEACT *ix, Uint8 direc, Uint8 sped)
{
    GAMEACT *iy;

    iy = SMTA_MakeActionX(SMG2_EneTamaAct, SMG2_ENTM_TOP, SMG2_ENTM_CNT);
    iy->sprite.xposi = ix->sprite.xposi;
    iy->sprite.yposi = ix->sprite.yposi;
    iy->sprite.xspd = SMMI_Cosset8(direc) * sped;
    iy->sprite.yspd = SMMI_Sinset8(direc) * sped;
}


/*
 *    次の攻撃を決める
 */
static Uint8 SMG2_Enemy2NextModeSet(GAMEACT *ix)
{

    enum {
        REST,
        WAY3,
        HAJYO,
        HOMING
        };

    static Uint8 atack_rd1[] = {WAY3, HAJYO, WAY3, WAY3, 
                                HAJYO, HOMING, REST, HAJYO, 
                                WAY3, REST, HAJYO, WAY3,
                                HOMING, REST, WAY3, HAJYO, REST };

    static Uint8 atack_rd2[] = {WAY3, WAY3, HAJYO, REST, 
                                HOMING, REST, HAJYO, REST, 
                                WAY3, REST, HAJYO, REST,
                                HOMING, WAY3, WAY3, HOMING, REST };

    static Uint8 atack_rd3[] = {HAJYO, WAY3, HAJYO, WAY3, 
                                WAY3, HOMING, WAY3, REST, 
                                HAJYO, WAY3, HOMING, HAJYO,
                                WAY3, HOMING, HAJYO, HOMING, REST };

    static Uint8 *atack_rdtbl[] = { atack_rd1, 
                                    atack_rd1, atack_rd2, atack_rd3};
    Uint8 atack;
    Uint8 *atackptr;

    atackptr = atack_rdtbl[SMG2_stageno];
    atack = *(atackptr + ix->wreg[0]);    
    ix->wreg[0]++;
    ix->wreg[0] &= 0xf;
    return atack;
}




/*
 *    休み
 */
static void SMG2_Enemy2_REST(GAMEACT *ix)
{
      if (ix->wcnt[0]++ > 80) {
          ix->wcnt[0] = 0;
          ix->wcnt[1] = 0;
          ix->wreg[1] = SMG2_Enemy2NextModeSet(ix);
      }
}



/*
 *     ３方向ショット
 */
static void SMG2_Enemy2_WAY3(GAMEACT *ix)
{
    Sint16 d0, d1;

    ix->wcnt[0]++;
    switch (ix->wcnt[0]) {
        case 0x20:
            d0 = *(Sint16 *)&SMG2_player_xposi 
                 - *(Sint16 *)&ix->sprite.xposi;
            d1 = *(Sint16 *)&SMG2_player_yposi 
                 - *(Sint16 *)&ix->sprite.yposi;
            d0 = SMMI_atan(d0, d1);
            SMG2_Enemy2TamaSet(ix, d0, 5);
            d0 -= 0x10;
            SMG2_Enemy2TamaSet(ix, d0, 5);
            d0 += 0x20;
            SMG2_Enemy2TamaSet(ix, d0, 5);
            break;

        case 0x24:
            d0 = *(Sint16 *)&SMG2_player_xposi 
                 - *(Sint16 *)&ix->sprite.xposi;
            d1 = *(Sint16 *)&SMG2_player_yposi 
                 - *(Sint16 *)&ix->sprite.yposi;
            d0 = SMMI_atan(d0, d1);
            SMG2_Enemy2TamaSet(ix, d0, 5);
            d0 -= 0x10;
            SMG2_Enemy2TamaSet(ix, d0, 5);
            d0 += 0x20;
            SMG2_Enemy2TamaSet(ix, d0, 5);
            break;

        case 0x28:
            d0 = *(Sint16 *)&SMG2_player_xposi 
                 - *(Sint16 *)&ix->sprite.xposi;
            d1 = *(Sint16 *)&SMG2_player_yposi 
                 - *(Sint16 *)&ix->sprite.yposi;
            d0 = SMMI_atan(d0, d1);
            SMG2_Enemy2TamaSet(ix, d0, 5);
            d0 -= 0x10;
            SMG2_Enemy2TamaSet(ix, d0, 5);
            d0 += 0x20;
            SMG2_Enemy2TamaSet(ix, d0, 5);
            break;

        case 0x30:
            ix->wcnt[0] = 0x18;
            if (ix->wcnt[1]++ > 4) {
                ix->wreg[1] = SMG2_Enemy2NextModeSet(ix);
                ix->wcnt[1] = 0;
            }
            break;
     }
}


/*
 *     波上攻撃
 */
static void SMG2_Enemy2_HAJYO(GAMEACT *ix)
{
    Sint16 d0, d1;   

    ix->wcnt[0]++;
      
    if (ix->wcnt[0] > 2) {
        ix->wcnt[0] = 0;
        ix->wcnt[1] += 0x8;
        d0 = ix->wcnt[1];
        d1 = -(d0 + 0x4); 
        SMG2_Enemy2TamaSet(ix, d0, 5);
        SMG2_Enemy2TamaSet(ix, d1, 5); 
        if (ix->wcnt[1] > 0x200) {
            ix->wcnt[0] = 0;
            ix->wcnt[1] = 0;
            ix->wreg[1] = SMG2_Enemy2NextModeSet(ix);
        }
    } 
}


/*
 *    ホーミング弾発射
 */
static void SMG2_Enemy2_HOMING(GAMEACT *ix)
{
    GAMEACT *iy;

    ix->wcnt[0]++;
    if (ix->wcnt[0] > 0x8) {
        ix->wcnt[0] = 0;
        iy = SMTA_MakeActionX(SMG2_EneTamaAct2, 
                                  SMG2_ENTM_TOP, SMG2_ENTM_CNT);
        iy->sprite.xposi = ix->sprite.xposi;
        iy->sprite.yposi = ix->sprite.yposi;
        iy->wreg[0] = ix->wcnt[1] & 7;

        if (ix->wcnt[1]++ > 8 - 1) {
            ix->wcnt[0] = 0;
            ix->wcnt[1] = 0;
            ix->wreg[1] = SMG2_Enemy2NextModeSet(ix);
        }
    } 
}



/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------
 *  エネミー３アクション発生コントロール
 *--------------------------------------------------------------
 *  
 *  
 *  
 *--------------------------------------------------------------
 */
void SMG2_Enemy3Set(GAMEACT *ix)
{

    enum {
        INIT,
        MAIN
        };

    GAMEACT *iy;
	
    switch (ix->mode) {
        case INIT:
            ix->wcnt[0] = 0;
            ix->wcnt[1] = 0;
            ix->mode++;
        case MAIN:
            if (ix->wcnt[0] > 0x100) {
                iy = SMTA_MakeActionX(SMG2_Enemy3Act, 
                                      SMG2_ENWK_TOP, SMG2_ENWK_CNT);
                iy->sprite.xposi = ix->sprite.xposi;
                iy->sprite.yposi = ix->sprite.yposi;
                iy->wreg[1] = 0x00;  /* SYOKI KAKUDO */

                iy = SMTA_MakeActionX(SMG2_Enemy3Act,
                                      SMG2_ENWK_TOP, SMG2_ENWK_CNT);
                iy->sprite.xposi = ix->sprite.xposi;
                iy->sprite.yposi = ix->sprite.yposi;
                iy->wreg[1] = 0x80;  /* SYOKI KAKUDO */

                ix->wcnt[0] &= 0xff;
                ix->wcnt[1]++;
            }
            ix->wcnt[0] += 0x10;

            if (ix->wcnt[1] > (6 - 1)) 
                SMTA_KillAction(ix);
            break;
     }

}

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------
 *  エネミー３アクションコントロール
 *--------------------------------------------------------------
 *  
 *  
 *  
 *--------------------------------------------------------------
 */
void SMG2_Enemy3Act(GAMEACT *ix)
{

/*  枚数,
 *  color, ofs_ax, ofs_ay, ofs_bx, ofs_by, ofs_cx, ofs_cy, ofs_dx, ofs_dy
 */
    static Sint16 enmy3_pat0[] = {
            1,
            rgb(31, 12, 0), -12, 0,  8, -8, 4, 0, 8, 8
        };

    static Sint16 enmy3_pat1[] = {
            1,
            rgb(16,  6, 0), -12, 0,  8, -8, 4, 0, 8, 8
        };

    static Sint16 *enemy3_pat[] = {
            enmy3_pat0,
            enmy3_pat1,
        };
		
    enum {
        INIT,
        MAIN
        };

    Uint8 flag;
    Sint16 d0, d1;
    GAMEACT *iy;
	
    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_zpolygon;
            ix->sprite.pmode = SPM_ecd+SPM_sdd;
            ix->sprite.patbase = enemy3_pat;
            ix->sprite.patno = 0;
            ix->sprite.patchgcnt = 0;
            ix->sprite.color = 0x0;
            ix->sprite.hzoom = 0x100;
            ix->sprite.vzoom = 0x100;
            ix->sprite.xspd = -0x200;
            ix->sprite.yspd = 0;

            ix->colino = 4;
            ix->coliatr = SMCO_atr_ENEMY;
            ix->coliflg = 0;

            ix->wcnt[0] = 0x30;    /* TAMA DASI COUNT */
            ix->wcnt[1] = 0;
            ix->wreg[0] = 0;
            ix->mode++;
        case MAIN:
            if (ix->coliflg & SMCO_dmg_PLSHOT) {
                SMG2_MakeBokan(ix);
                SMG2_AddScore(100);
                ix->coliflg = 0;
                break;
            }

            d0 = ix->wreg[1] += 0x4;
            ix->sprite.yspd  = SMMI_Sinset8(d0) * 2;
            
            

            ix->wreg[0] += 0x4;
            d0 = SMG2_mainloop_cnt;
            d0 = (d0 * 4) & 0xff;
            ix->sprite.vzoom = SMMI_Cosset8(d0) + 1;

            if ((d0 > 0x40) && (d0 < 0xc0))
                ix->sprite.patno = 1;
            else
                ix->sprite.patno = 0;

            ix->sprite.xposi += (ix->sprite.xspd << 8);
            ix->sprite.yposi += (ix->sprite.yspd << 8);
            

            ix->wcnt[0]--;
            if (ix->wcnt[0] == 0) {
                iy = SMTA_MakeActionX(SMG2_EneTamaAct, 
                                      SMG2_ENTM_TOP, SMG2_ENTM_CNT);
                iy->sprite.xposi = ix->sprite.xposi;
                iy->sprite.yposi = ix->sprite.yposi;
                d0 = *(Sint16 *)&SMG2_player_xposi 
                     - *(Sint16 *)&ix->sprite.xposi;
                d1 = *(Sint16 *)&SMG2_player_yposi 
                     - *(Sint16 *)&ix->sprite.yposi;
                d0 = SMMI_atan(d0, d1);
                iy->sprite.xspd = SMMI_Cosset8(d0) * 3;
                iy->sprite.yspd = SMMI_Sinset8(d0) * 3;

                ix->wcnt[0] = 0x60;  /* TAMADASI FLAG SET */
            }


            if (ix->sprite.xposi < -0x10) {
                SMTA_KillAction(ix);
                break;
	    }
            SMCO_PutColBox(*(Sint16 *)&ix->sprite.xposi, 
                           *(Sint16 *)&ix->sprite.yposi, ix->colino);
            SMG2_PutObject(&ix->sprite);
            break;			
    }
	
}






/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------
 *  エネミー４アクション発生コントロール
 *--------------------------------------------------------------
 *  
 *  
 *  
 *--------------------------------------------------------------
 */
void SMG2_Enemy4Set(GAMEACT *ix)
{

    enum {
        INIT,
        MAIN
        };

    GAMEACT *iy;
	
    switch (ix->mode) {
        case INIT:
            ix->wcnt[0] = 0;
            ix->wcnt[1] = 0;
            ix->mode++;
        case MAIN:
            if (ix->wcnt[0] > 0x100) {
                iy = SMTA_MakeActionX(SMG2_Enemy4Act, 
                                      SMG2_ENWK_TOP, SMG2_ENWK_CNT);
                iy->sprite.xposi = ix->sprite.xposi;
                iy->sprite.yposi = ix->sprite.yposi;
                iy->wreg[1] = 0x00;  /* SYOKI KAKUDO */

                ix->wcnt[0] &= 0xff;
                ix->wcnt[1]++;
            }
            ix->wcnt[0] += 0x10;

            if (ix->wcnt[1] > (10 - 1)) 
                SMTA_KillAction(ix);
            break;
     }

}

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------
 *  エネミー４アクションコントロール
 *--------------------------------------------------------------
 *  
 *  
 *  
 *--------------------------------------------------------------
 */
void SMG2_Enemy4Act(GAMEACT *ix)
{

/*  枚数,
    color, ofs_ax, ofs_ay, ofs_bx, ofs_by, ofs_cx, ofs_cy, ofs_dx, ofs_dy
 */
    static Sint16 enmy4_pat0[] = {
            1,
            rgb(28, 16, 0), -12, 0,  8, -8, 4, 0, 8, 8
        };

    static Sint16 enmy4_pat1[] = {
            1,
            rgb(14,  8, 0), -12, 0,  8, -8, 4, 0, 8, 8
        };

    static Sint16 *enemy4_pat[] = {
            enmy4_pat0,
            enmy4_pat1,
        };
		
    enum {
        INIT,
        MOVE1,
        MOVE2
        };

    Uint8 flag;
    Sint16 d0, d1;
    GAMEACT *iy;
	
    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_zpolygon;
            ix->sprite.pmode = SPM_ecd+SPM_sdd;
            ix->sprite.patbase = enemy4_pat;
            ix->sprite.patno = 0;
            ix->sprite.patchgcnt = 0;
            ix->sprite.color = 0x0;
            ix->sprite.hzoom = 0x100;
            ix->sprite.vzoom = 0x100;

            ix->sprite.xspd = 0;
           
            if (*(Sint16 *)&ix->sprite.yposi > 0x70)
                ix->sprite.yspd = -0x200;
            else
                ix->sprite.yspd = 0x200;

            ix->colino = 4;
            ix->coliatr = SMCO_atr_ENEMY;
            ix->coliflg = 0;

            ix->wcnt[0] = 0x30;    /* TAMA DASI COUNT */
            ix->wcnt[1] = 0;
            ix->wreg[0] = 0;
            ix->mode++;

        case MOVE1:
            d0 = *(Sint16 *)&SMG2_player_yposi - *(Sint16 *)&ix->sprite.yposi;
            if (d0 < 0) 
                d0 = 0 - d0;

            if (d0 < 0x8) {
                ix->mode++;
                ix->sprite.yspd = 0;
                if (*(Sint16 *)&SMG2_player_xposi > 
                    *(Sint16 *)&ix->sprite.xposi)
                    ix->sprite.xspd = 0x300;
                else
                    ix->sprite.xspd = -0x300;
            }

        case MOVE2:
            if (ix->coliflg & SMCO_dmg_PLSHOT) {
                SMG2_MakeBokan(ix);
                SMG2_AddScore(100);
                ix->coliflg = 0;
                break;
            }

            ix->wreg[0] += 0x4;
            ix->sprite.vzoom = SMMI_Cosset8(ix->wreg[0]) + 1;
            
            if ((ix->wreg[0] > 0x40) && (ix->wreg[0] < 0xc0))
                ix->sprite.patno = 1;
            else
                ix->sprite.patno = 0;

            ix->sprite.xposi += (ix->sprite.xspd << 8);
            ix->sprite.yposi += (ix->sprite.yspd << 8);
            

            ix->wcnt[0]--;
            if (ix->wcnt[0] == 0) {
                iy = SMTA_MakeActionX(SMG2_EneTamaAct, 
                                      SMG2_ENTM_TOP, SMG2_ENTM_CNT);
                iy->sprite.xposi = ix->sprite.xposi;
                iy->sprite.yposi = ix->sprite.yposi;
                d0 = *(Sint16 *)&SMG2_player_xposi 
                   - *(Sint16 *)&ix->sprite.xposi;
                d1 = *(Sint16 *)&SMG2_player_yposi 
                   - *(Sint16 *)&ix->sprite.yposi;
                d0 = SMMI_atan(d0, d1);
                iy->sprite.xspd = SMMI_Cosset8(d0) * 3;
                iy->sprite.yspd = SMMI_Sinset8(d0) * 3;
                ix->wcnt[0] = 0x30;  /* TAMADASI FLAG SET */
            }


            flag = SMG2_EnemyFrameOutChk(*(Sint16 *)&ix->sprite.xposi,
                                         *(Sint16 *)&ix->sprite.yposi);
            if (flag == 1) {
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
 *  エネミーの出す弾のアクションコントロール
 *--------------------------------------------------------------
 *  等速移動の弾  
 *  
 *  
 *--------------------------------------------------------------
 */
static void SMG2_EneTamaAct(GAMEACT *ix)
{

/*  枚数,
    color, ofs_ax, ofs_ay, ofs_bx, ofs_by, ofs_cx, ofs_cy, ofs_dx, ofs_dy
 */
    static Sint16 tama_pat0[] = {
            1,
            rgb(12, 31, 4), -2,-2, 2,-2, 2,2, -2,2
        };

    static Sint16 *tama_pat[] = {
            tama_pat0,
        };
		
    enum {
        INIT,
        MAIN
        };

    Uint8 flag;
    Sint16 d0, d1;
	
    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_npolygon;
            ix->sprite.pmode = SPM_ecd+SPM_sdd;
            ix->sprite.patbase = tama_pat;
            ix->sprite.patno = 0;
            ix->sprite.patchgcnt = 0;
            ix->sprite.color = 0x0;
            ix->colino = 1;
            ix->coliatr = SMCO_atr_ENSHOT;
            ix->coliflg = 0;
            ix->mode++;
        case MAIN:
            if (ix->coliflg & SMCO_dmg_PLAYER) {
                SMTA_KillAction(ix);
                break;
            }
            ix->coliflg = 0;


            ix->sprite.xposi += (ix->sprite.xspd << 8);
            ix->sprite.yposi += (ix->sprite.yspd << 8);

            /* フレームアウトチェック */
            flag = SMG2_EnemyFrameOutChk(*(Sint16 *)&ix->sprite.xposi,
                                         *(Sint16 *)&ix->sprite.yposi);
            if (flag == 1) {
                SMTA_KillAction(ix);
                break;
	    }

            SMCO_PutColBox(*(Sint16 *)&ix->sprite.xposi, 
                           *(Sint16 *)&ix->sprite.yposi, ix->colino);
            SMG2_PutObject(&ix->sprite);
            break;			
    }
	
}







/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------
 *  エネミーの出す弾のアクションコントロール
 *--------------------------------------------------------------
 *  指定した場所へ向かう弾
 *  
 *  
 *--------------------------------------------------------------
 */
static void SMG2_EneTamaAct2(GAMEACT *ix)
{

/*  枚数,
    color, ofs_ax, ofs_ay, ofs_bx, ofs_by, ofs_cx, ofs_cy, ofs_dx, ofs_dy
 */
    static Sint16 tama2_pat0[] = {
            1,
            rgb(25, 31, 4), -4, -3, 4, 0, 4, 0, -4, 3
        };

    static Sint16 *tama2_pat[] = {
            tama2_pat0,
        };
		
    enum {
        INIT,
        MAIN
        };

    static Uint8 tama2_dirtbl[] = { 0x20, 0x10, 0xf0, 0x10, 
                                    0x10, 0xf0, 0x10, 0xf0 };

    Uint8 flag;
    Sint32 d0, d1;
	
    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_rzpolygon;
            ix->sprite.pmode = SPM_ecd+SPM_sdd;
            ix->sprite.patbase = tama2_pat;
            ix->sprite.patno = 0;
            ix->sprite.patchgcnt = 0;
            ix->sprite.color = 0x0;
            ix->sprite.hzoom = 0x100;
            ix->sprite.vzoom = 0x100;

            
            ix->sprite.xspd =  SMMI_Cosset8(tama2_dirtbl[ix->wreg[0]]) * 0x6;
            ix->sprite.yspd =  SMMI_Sinset8(tama2_dirtbl[ix->wreg[0]]) * 0x6;
            ix->sprite.xacc = 0;
            ix->sprite.yacc = 0;
            ix->colino = 1;
            ix->coliatr = SMCO_atr_ENSHOT;
            ix->coliflg = 0;

            SMG2_EneTamaAct2Sub(ix);   /* SPEED SET SUB */

            ix->mode++;

        case MAIN:
            if (ix->coliflg & SMCO_dmg_PLAYER) {
                SMTA_KillAction(ix);
                break;
            }
            ix->coliflg = 0;

            ix->sprite.zdirec = SMMI_atan(ix->sprite.xspd, ix->sprite.yspd);

            ix->sprite.xposi += (ix->sprite.xspd << 8);
            ix->sprite.yposi += (ix->sprite.yspd << 8);
            ix->sprite.xspd += ix->sprite.xacc;
            ix->sprite.yspd += ix->sprite.yacc;

            d0 = *(Sint16 *)&ix->sprite.xposi;
            d1 = *(Sint16 *)&ix->sprite.yposi;

            d0 = *(Sint16 *)&ix->sprite.xposi;
            d1 = *(Sint16 *)&ix->sprite.yposi;
   	    if ((d1 > -0x40) && (d1 < 0x120) && (d0 > -0x20) && (d0 < 0x200))
               flag = 0;
            else
               flag = 1;
            if (flag == 1) {
                SMTA_KillAction(ix);
                break;
	    }

            SMG2_PutObject(&ix->sprite);
            break;			
    }
	
}


static void SMG2_EneTamaAct2Sub(GAMEACT *ix)
{
    Sint32 d0, d1;

    d0 = SMG2_player_xposi - ix->sprite.xposi;
    d1 = SMG2_player_yposi - ix->sprite.yposi;
    ix->sprite.xacc = -((ix->sprite.xspd * 2) / 0x80);
    ix->sprite.xspd = ix->sprite.xspd + ((d0 / 0x80) >> 8);
    ix->sprite.yacc = -((ix->sprite.yspd * 2) / 0x80);
    ix->sprite.yspd = ix->sprite.yspd + ((d1 / 0x80) >> 8);

}








/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------
 *  アイテムアクションコントロール
 *--------------------------------------------------------------
 *  いまのところスコアが入るだけです
 *  
 *  
 *--------------------------------------------------------------
 */
static void SMG2_ItemAct(GAMEACT *ix)
{


/*  枚数,
    Charno, HVreverse, Ｈサイズ, Ｖサイズ, Ｈオフセット, Ｖオフセット */
    static Sint16 speed_pat[] = {
            1,
            SMG2_Item0CharNo, 0, 8, 8, -8, -8
        };
    static Sint16 ring10_pat[] = {
            1,
            SMG2_Item1CharNo, 0, 8, 8, -8, -8
        };


    static Sint16 *item_pat[] = {
            speed_pat, ring10_pat
        };

		
    enum {
        INIT,
        MAIN
        };

    Uint8 flag;
    Sint16 d0, d1;
    GAMEACT *iy;
	
    switch (ix->mode) {
        case INIT:
            ix->sprite.putmode = PMD_sprx_nrm;
            ix->sprite.pmode = SPM_ecd+SPM_sdd;
            ix->sprite.patbase = item_pat;
            ix->sprite.patno = 1;
            ix->sprite.patchgcnt = 0;
            ix->sprite.color = SMG2_ItemPalNo;

            d0 = *(Sint16 *)&SMG2_player_xposi - *(Sint16 *)&ix->sprite.xposi;
            d1 = *(Sint16 *)&SMG2_player_yposi - *(Sint16 *)&ix->sprite.yposi;
            d0 = SMMI_atan(d0, d1);
            ix->sprite.xspd = SMMI_Cosset8(d0) * 2;
            ix->sprite.yspd = SMMI_Sinset8(d0) * 3;

            ix->colino = 4;
            ix->coliatr = SMCO_atr_ITEM;
            ix->coliflg = 0;

            ix->mode++;
        case MAIN:
            if (ix->coliflg & SMCO_dmg_PLAYER) {
                SMG2_AddScore(1000);
                SMTA_KillAction(ix);
                break;
            }

            ix->sprite.xposi += (ix->sprite.xspd << 8);
            ix->sprite.yposi += (ix->sprite.yspd << 8);
            
            flag = SMG2_EnemyFrameOutChk(*(Sint16 *)&ix->sprite.xposi,
                                         *(Sint16 *)&ix->sprite.yposi);
            if (flag == 1) {
                SMTA_KillAction(ix);
                break;
	    }


            SMG2_PutObject(&ix->sprite);
            SMCO_PutColBox(*(Sint16 *)&ix->sprite.xposi, 
                           *(Sint16 *)&ix->sprite.yposi, ix->colino);
            break;			
    }
	
}



















