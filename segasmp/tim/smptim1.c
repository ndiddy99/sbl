/*-----------------------------------------------------------------------------
 *  FILE: smptim1.c
 *
 *  Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *
 *      タイマサンプルプログラム(FRT)
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
 *      1994-05-19  N.T Ver.0.90
 *
 *  MOD HISTORY:
 *
 *      1994-08-12  N.T Ver.1.00
 *
 *-----------------------------------------------------------------------------
 */

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
#include <stdio.h>
#include <string.h>
#include "sega_xpt.h"

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
void Time0Int(void);

void main(void)
{

    Sint8 *buf = (Sint8 *)0x6050010;
    /** BEGIN ****************************************************************/
    /*
     *  process 1 
     */

    switch(*(Uint32 *)0x6040000){
        case 0:
            TIM_FRT_INIT(TIM_CKS_8);
            break;
        case 1:
            TIM_FRT_INIT(TIM_CKS_32);
            break;
        case 2:
            TIM_FRT_INIT(TIM_CKS_128);
            break;
    }

    TIM_FRT_DELAY_16(20);
    *(Uint32 *)0x6050000 = TIM_FRT_GET_16();
    sprintf(buf, "get = %d",*(Uint32 *)0x6050000);
    buf += strlen(buf);
    
    *(Float32 *)0x6050004 = TIM_FRT_CNT_TO_MCR(*(Uint32 *)0x6050000);
    sprintf(buf, "get = %f",*(Float32 *)0x6050004);
    buf += strlen(buf);
    
    *(Float32 *)0x6050008 = TIM_FRT_MCR_TO_CNT(*(Float32 *)0x6050004);
    sprintf(buf, "get = %f",*(Float32 *)0x6050008);
    buf += strlen(buf);
}
