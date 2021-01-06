/*-----------------------------------------------------------------------------
 *  FILE: mem_free.c
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
 *          <3> MEM_Free()      -   メモリ管理領域設定
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
 * NAME:    MEM_Free()              - メモリブロック解放
 *
 * PARAMETERS :
 *      (1) void *mem_ptr           - <i>  メモリブロックへのポインタ
 *
 * DESCRIPTION:
 *      メモリブロックを解放します．
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

void MEM_Free(void *mem_ptr)
{
    MemHead *cell_hd;
    MemHead *now;
    
    cell_hd = ((MemHead *)mem_ptr) - 1;
    for(now = MEM_empty_top; !((cell_hd > now) && (cell_hd < now->s.next));
                           now = now->s.next){
       if((now >= now->s.next) && ((cell_hd > now) || (cell_hd < now->s.next)))
            break;
    }
    if((cell_hd + cell_hd->s.size) == now->s.next){
        cell_hd->s.size += now->s.next->s.size;
        cell_hd->s.next = now->s.next->s.next;
    }else{
        cell_hd->s.next = now->s.next;
    }
    if((now + now->s.size) == cell_hd){
        now->s.size += cell_hd->s.size;
        now->s.next = cell_hd->s.next;
    }else{
        now->s.next = cell_hd;
    }
    MEM_empty_top = now;
}
