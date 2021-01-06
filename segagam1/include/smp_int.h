/*	Copyright(c) 1994 SEGA			*/
/*
 *
 * V_BLANK割り込み処理
 *
 *
 */

#ifndef __SMP_INT_H
#define __SMP_INT_H


/* 関数プロトタイプ */
extern void SMIN_VblankIn(void);
extern void SMIN_VblankOut(void);
extern void SMIN_SpriteEnd(void);


/* グローバル変数　宣言 */
GLOBAL Uint32 SMIN_vintcnt;        /* V-BLANK IN カウンタ */
GLOBAL Uint8 SMIN_vintflag;        /* Vint Flag           */
GLOBAL Uint32 SMIN_vintdwcnt;
GLOBAL void (*SMIN_VintHook1)(void);

#endif  /* endif __SMP_INT_H */



