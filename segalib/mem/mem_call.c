/*-----------------------------------------------------------------------------
 *  FILE: mem_call.c
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
 *          <2> MEM_Calloc()    -   配列領域確保
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

/******************************************************************************
 *
 * NAME:    MEM_Calloc()            - 配列領域確保
 *
 * PARAMETERS :
 *      (1) Uint32 arg_num          - <i>  配列要素数
 *      (2) Uint32 arg_size         - <i>  配列要素数のバイト数
 *
 * DESCRIPTION:
 *      配列領域を確保します．
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

void *MEM_Calloc(Uint32 arg_num, Uint32 arg_size)
{
    MemHead *now;
    MemHead *before;
    
    Uint32 all_size;
    /** BEGIN ****************************************************************/
    all_size = ((arg_size * arg_num) + sizeof(MemHead) - 1) /
                                              sizeof(MemHead) + 1;
    before = MEM_empty_top;
    for(now = before->s.next; ; before = now, now = now->s.next){
        if(now->s.size >= all_size){
            if(now->s.size == all_size)
                before->s.next = now->s.next;
            else{
                now->s.size -= all_size;
                now += now->s.size;
                now->s.size = all_size;
            }
            MEM_empty_top = before;
            MEM_ClrMemZero((now + 1),all_size);
            return(void *)(now + 1);
        }
        if(now == MEM_empty_top){
            now = MEM_empty_top;
            return NULL;
        }
    }
}
