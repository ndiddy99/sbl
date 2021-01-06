/*-----------------------------------------------------------------------------
 *  FILE: smpper1.c
 *
 *      Copyright (c) by SEGA.
 *
 *  PURPOSE:
 *
 *      SMPCのシステムマネージメント系サンプルプログラム
 *
 *  DESCRIPTION:
 *
 *      SMPCのシステムマネージメント系のコマンドを実行します。
 *
 *      *(Uint32 *)0x6050000に実行したいコマンドの番号を入力してください。又、
 *      コマンドに入力が必要な場合は、*(Uint8 *)0x6050030から入力してください。
 *      出力は*(Uint8 *)0x6050010に書き込みます。
 *
 *          コマンド番号 -> *(Uint32 *)0x6050000
 *          入力         -> *(Uint32 *)0x6050030
 *          出力         -> *(Uint32 *)0x6050010
 *
 *          +---------------------------------------+
 *          |コマンド番号    コマンド名称           |
 *          +---------------------------------------+
 *          |        0x01    マスタSH2 ON           |
 *          |        0x02    スレーブSH2 ON         |
 *          |        0x03    スレーブSH2 OFF        |
 *          |        0x04    サウンドON             |
 *          |        0x05    サウンドOFF            |
 *          |        0x06    CD ON                  |
 *          |        0x07    CD OFF                 |
 *          |        0x08    システム全体リセット   |
 *          |        0x09    NMIリクエスト          |
 *          |        0x0a    ホットリセットｲﾈｰﾌﾞﾙ   |
 *          |        0x0b    ホットリセットﾃﾞｨｾｰﾌﾞﾙ |
 *          |        0x0c    SMPCメモリ設定         |
 *          |        0x0d    時刻設定               |
 *          +---------------------------------------+
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *
 *  CAVEATS:
 *
 *  AUTHOR(S)
 *
 *      1994-08-11  N.T Ver.1.00
 *
 *  MOD HISTORY:
 *
 *-----------------------------------------------------------------------------
 */

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
#include "sega_xpt.h"
#include <machine.h>
#include <string.h>

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include "sega_per.h"

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

void main(void)
{
    Uint8 ireg[32];
    Uint8 oreg[32];
    Uint32 i;

    /** BEGIN ****************************************************************/
    /*
     *  process 1
     */
    
    for(i = 0; i < 32; i++){
        *((Uint8 *)0x6050010 + i) = 0x00;
    }
    
    switch((*(Uint32 *)0x6050000)){
        case  1:
            PER_SMPC_MSH_ON();
            break;
        case  2:
            PER_SMPC_SSH_ON();
            break;
        case  3:
            PER_SMPC_SSH_OFF();
            break;
        case  4:
            PER_SMPC_SND_ON();
            break;
        case  5:
            PER_SMPC_SND_OFF();
            break;
        case  6:
            PER_SMPC_CD_ON();
            break;
        case  7:
            PER_SMPC_CD_OFF();
            break;
        case  8:
            PER_SMPC_SYS_RES();
            break;
        case  9:
            PER_SMPC_NMI_REQ();
            break;
        case  0xa:
            PER_SMPC_RES_ENA();
            break;
        case  0xb:
            PER_SMPC_RES_DIS();
            break;
        case  0xc:
            ireg[0] = *(Uint8 *)0x6050030;
            ireg[1] = *(Uint8 *)0x6050031;
            ireg[2] = *(Uint8 *)0x6050032;
            ireg[3] = *(Uint8 *)0x6050033;
            PER_SMPC_SET_SM(*(Uint32 *)ireg);
            break;
        case  0xd:
            for(i = 0; i < 7; i++){
                ireg[i] = *((Uint8 *)0x6050030 + i);
            }
            PER_SMPC_SET_TIM(ireg);
            break;
    }

    while(1)
    {
    }
}
