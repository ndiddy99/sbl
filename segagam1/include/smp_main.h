/*	Copyright(c) 1994 SEGA			*/
/*
 *
 * smp_main.c header file 'SMP_MAIN.H'
 *
 *
 */

#ifndef __SMP_MAIN_H
#define __SMP_MAIN_H


/* MAIN MODE MODE NO ASSIGN */
enum {
        DUMMY_MODE,              /* NO EXECUTE MODE */
        LOGO_MODE,
        TITLE_MODE,
        SELECT_MODE,
        SMSC_SELECTMODE,
        SMSC_MODE,
        SMSC_BACK,
        SMSC_NORMSCLMODE,
        SMSC_MULTISCLMODE,
        SMSC_ROTSCLMODE,
        LNSC_MODE,
        LCLR_MODE,
        SPSAMPLE_MODE,
        WIND_MODE,
        GAME_MODE,
        DEMO214_MODE,
        SMRK_MODE
     };



/* 関数プロトタイプ宣言 */
extern void main(void);
extern void SMMA_initall(void);
extern void SMMA_intwait(void);

/* グローバル変数　宣言 */
GLOBAL Uint8 SMMA_MainMode;          /* MainProgram Mode No */
GLOBAL Uint8 SMMA_MainLevel;
GLOBAL Uint8 SMMA_MainLevel2;

#endif  /* endif __SMP_MAIN_H */







