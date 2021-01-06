/*-----------------------------------------------------------------------------
 *  FILE: smpmem0.c
 *
 *  Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *
 *      メモリサンプルプログラム
 *
 *  DESCRIPTION:
 *
 *      メモリ管理を実行します。
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
#include "sega_mem.h"

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

    /** BEGIN ****************************************************************/
    /*
     *  process 1 
     */
    Uint16 *malloc_1;
    Uint16 *malloc_2;
    Sint8 *malloc_3;
    Sint8 *malloc_4;

    MEM_Init((Uint32)0x6040000,(Uint32)0x10000);

    malloc_1 = (Uint16 *)MEM_Calloc((Uint32)0x10 , 2);
    malloc_2 = (Uint16 *)MEM_Calloc((Uint32)0x10 , 2);
    malloc_3 = (Sint8 *)MEM_Malloc((Uint32)0x11);
    malloc_4 = (Sint8 *)MEM_Malloc((Uint32)0x11);

    strcpy(malloc_1, "1234");
    strcpy(malloc_2, "abcd");
    strcpy(malloc_3, "!#$%&'()=~");
    strcpy(malloc_4, "0987654321");

    malloc_2 = (Uint16 *)MEM_Realloc(malloc_2, 2);

    MEM_Free(malloc_2);
    MEM_Free(malloc_3);
}
