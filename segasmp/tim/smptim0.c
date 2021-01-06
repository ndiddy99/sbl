/*-----------------------------------------------------------------------------
 *  FILE: smptim0.c
 *
 *  Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *
 *      タイマサンプルプログラム(SCU)
 *
 *  DESCRIPTION:
 *
 *      タイマを実行します。
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *
 *  CAVEATS:
 *
 *  AUTHOR(S)
 *
 *      1994-08-12  N.T Ver.1.00
 *
 *  MOD HISTORY:
 *
 *-----------------------------------------------------------------------------
 */

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
#include <stdio.h>
#include <string.h>
#include <machine.h>
#include "sega_xpt.h"
#include "sega_int.h"

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include "sega_tim.h"

/*
 * GLOBAL DECLARATIONS
 */

/*
 * LOCAL DEFINES/MACROS
 */

/*
 * STATIC DECLARATIONS
 */

/*
 * STATIC FUNCTION PROTOTYPE DECLARATIONS
 */

void Time0Int(void);
void Time1Int(void);

/******************************************************************************
 *
 * NAME:    main()      - メイン
 *
 * PARAMETERS :
 *      なし
 *
 * DESCRIPTION:
 *      メイン
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */

void main(void)
{

    /** BEGIN ****************************************************************/
    /*
     *  process 1 
     */

    set_imask(0);               /* ﾏｽｸの解除        */
    INT_SetScuFunc(INT_SCU_TIM1, Time1Int);
    INT_SetScuFunc(INT_SCU_TIM0, Time0Int);

    TIM_T0_SET_CMP(0x08);
    TIM_T1_SET_DATA(0x08);
    TIM_T1_SET_MODE(0x101);
    TIM_T1_ENABLE();
    TIM_T0_ENABLE();
}
void Time0Int(void)
{
    static Uint32 time0 = 1;
    *(Uint32 *)0x6050000 = time0;
    time0 ++;
}

void Time1Int(void)
{
    static Uint32 time1 = 1;
    *(Uint32 *)0x6050010 = time1;
    time1 ++;
}
