/*-----------------------------------------------------------------------------
 *  FILE: mem_main.c
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
 *          <1> MEM_Init()      -   メモリ管理領域設定
 *          <2> MEM_Calloc()    -   配列領域確保
 *          <4> MEM_Malloc()    -   メモリブロック確保
 *          <5> MEM_Realloc()   -   メモリ再割り付け
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

/******************************************************************************
 *
 * NAME:    MEM_Init()              - メモリ管理領域設定
 *
 * PARAMETERS :
 *      (1) Uint32 top_address      - <i>  メモリ管理領域先頭アドレス
 *      (2) Uint32 mem_size         - <i>  メモリ管理領域サイズ
 *
 * DESCRIPTION:
 *      メモリ管理領域を設定します.
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

void MEM_Init(Uint32 top_address, Uint32 mem_size)
{
    (*(MemHead *)top_address).s.next = (MemHead *)top_address;
    MEM_empty_top = (MemHead *)top_address;
    (*(MemHead *)top_address).s.size = mem_size / sizeof(MemHead);
}
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

/******************************************************************************
 *
 * NAME:    MEM_Malloc()            - メモリブロック確保
 *
 * PARAMETERS :
 *      (1) Uint32 mem_size         - <i>  要求メモリブロックサイズ(バイト単位)
 *
 * DESCRIPTION:
 *      要求メモリを確保します．
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

void *MEM_Malloc(Uint32 mem_size)
{
    MemHead *now;
    MemHead *before;
    
    unsigned all_size;
    /** BEGIN ****************************************************************/
    all_size = (mem_size + sizeof(MemHead) - 1) / sizeof(MemHead) + 1;
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
            return(void *)(now + 1);
        }
        if(now == MEM_empty_top){
            now = MEM_empty_top;
            return NULL;
        }
    }
}

/******************************************************************************
 *
 * NAME:    MEM_Realloc()           - メモリ再割り付け
 *
 * PARAMETERS :
 *      (1) void *mem_prt           - <i>  以前のメモリブロックへのポインタ
 *      (2) Uint32 mem_size         - <i>  新しいメモリブロックサイズ（バイト指定）
 *
 * DESCRIPTION:
 *      メモリを再割り付けします。
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

void *MEM_Realloc(void *mem_ptr, Uint32 mem_size)
{
    Uint32 i;
    Uint8 *work;
    if((work = (Uint8 *)MEM_Malloc(mem_size)) == NULL)
        return(NULL);
    for(i = 0; i < mem_size; i++){
	*(work + i) = *((Uint8 *)mem_ptr + i);
    };
    MEM_Free(mem_ptr);
    return(work);
}

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

