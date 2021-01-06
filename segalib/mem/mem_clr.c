/*-----------------------------------------------------------------------------
 *  FILE: mem_clr.c
 *
 *      Copyright(c) 1994 SEGA.
 *
 *  PURPOSE:
 *
 *      メモリライブラリソースファイル。
 *
 *  DESCRIPTION:
 *
 *      メモリ管理を実現する。
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *          <6> MEM_ClrMemZero  -   メモリゼロクリア
 *
 *  CAVEATS:
 *
 *      なし。
 *
 *  AUTHOR(S)
 *
 *      1994-07-06  N.T Ver.1.00
 *
 *  MOD HISTORY:
 *
 *-----------------------------------------------------------------------------
 */

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
#include <stdio.h>
#include "sega_xpt.h"

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include "mem_main.h"

/*
 * GLOBAL DECLARATIONS
 */

/*
 * LOCAL DEFINES/MACROS
 */

/*
 * TYPEDEFS
 */

/*
 * STATIC DECLARATIONS
 */ 

/*
 * STATIC FUNCTION PROTOTYPE DECLARATIONS
 */

/*****************************************************************************/
/**** 内部提供関数 ***********************************************************/
/*****************************************************************************/
/******************************************************************************
 *
 * NAME:    MEM_ClrMemZero              - メモリゼロクリア
 *
 * PARAMETERS :
 *      (1) void * adr              - <i>  先頭アドレス
 *      (2) Uint32 byte             - <i>  バイト数
 *
 * DESCRIPTION:
 *      指定メモリにゼロを埋めます.
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
void MEM_ClrMemZero(void *adr, Uint32 all_size)
{
    Uint32 i;
    all_size = (all_size - 1) * 2;
    for(i = 0; i < all_size; i++){
        *((Uint32 *)adr + i) = 0;
    }
}

