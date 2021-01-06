/*	Copyright(c) 1994 SEGA			*/
/*
 *
 *      回転面の係数テーブルを使ったサンプル
 *
 */
 
#ifndef __SMP_RKEI_H
#define __SMP_RKEI_H


/*
 *    RBG0 KEISU SAMPLE VDP2 VRAM ASSIGN
 */
#define SMRK_NBG_PTGEN (SCR_VRAMA0 + 0x00000) /* NBG PTGEN                   */


#define SMRK_RBG0_PTGEN (SCR_VRAMA0 + 0x00000)    /* RBG0 PTGEN          */
#define SMRK_RBG0_RPAR  (SCR_VRAMA1 + 0x10000)    /* RBG0 回転パラメータ */
#define SMRK_BCLRTBLADD (SCR_VRAMA1 + 0x10100)    /* Backcolortable  */
#define SMRK_LCLRTBLADD (SCR_VRAMA1 + 0x10800)    /* ラインカラーテーブル */

#define SMRK_RBG0_PTNAM (SCR_VRAMB0 + 0x00000)    /* RBG0 PTNAM          */

#define SMRK_RBG0_KEISU (SCR_VRAMB1 + 0x06000)    /* RBG0 係数           */



/* 球スクロールコントロールアクションワークアサイン */
SMTA_DefActWk(SMRKACT,
              Sint32 xposi;
              Sint32 yposi;
              Sint32 zposi;
              Sint32 xspeed;
              Sint32 yspeed;
              Sint32 zspeed;
              Sint32 zoom;
              Sint16 wcnt[4];
              Sint16 wreg[4];
);


/* FUNCTION PROTOTYPE */
extern void SMRK_MainMode(void);

/* GLOBAL HENSUU */
ROTPARAM SMRK_rotregbufA; 
ROTPARAM SMRK_rotregbufB; 
Uint32 SMRK_RotregReq;
Uint32 SMRK_KeiEnd;
Sint16 SMRK_KeisuBuf[0xa000];
Sint16 SMRK_FilData;
Uint8  SMRK_sqrttbl[0x4000];
#endif
 
