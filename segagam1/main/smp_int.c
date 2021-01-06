/*	Copyright(c) 1994 SEGA			*/
/*
 *
 * V_BLANK 割り込み処理
 *
 */

#include "smp_bios.h"


/*
 *    外部参照
 */
extern void SCL_VblankStart(void);   /* ライブラリ標準 VBLANKIN 関数  */
extern void SCL_VblankEnd(void);     /* ライブラリ標準 VBLANKOUT関数  */


Uint32 SMIN_vintcnt;        /* V-BLANK IN カウンタ */
Uint8 SMIN_vintflag;        /* Vint Flag           */
Uint32 SMIN_vintdwcnt;
void (*SMIN_VintHook1)(void);


/* ========================================================================= */
/*
 *  V_BLANK IN 割り込み処理
 */
void SMIN_VblankIn(void)
{
    SCL_VblankStart();
    SMPA_SwRead(); 
    SMIN_vintcnt++;
    if (SMIN_vintdwcnt != 0) 
        SMIN_vintdwcnt--;

    if (SMIN_VintHook1)
        (*SMIN_VintHook1)();
}



/* ========================================================================= */
/*
 *    V_BLANK OUT 割り込み処理
 */
void SMIN_VblankOut(void)
{
    SCL_VblankEnd();

}




/* ========================================================================= */
/*
 *    SPRITE 描画終了割り込み
 */
void SMIN_SpriteEnd(void)
{

}
