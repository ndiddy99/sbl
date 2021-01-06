/*-----------------------------------------------------------------------------
 *  FILE: mem_main.h
 *
 *      Copyright(c) 1994 SEGA.
 *
 *  PURPOSE:
 *
 *      メモリライブラリヘッダファイル。
 *
 *  DESCRIPTION:
 *
 *      メモリ管理を実現する。
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
#include "sega_xpt.h"

/*
 * USER SUPPLIED INCLUDE FILES
 */

#ifndef MEM_MAIN_H
#define MEM_MAIN_H

/*
 * GLOBAL DEFINES/MACROS DEFINES
 */

/*
 * STRUCTURE DECLARATIONS
 */

/*
 * TYPEDEFS
 */
typedef double MemAlign;                        /* 64ビットのアライメント    */
union mem_head {                                /* セルのヘッダ              */
    struct {
        union mem_head *next;                   /* 次のセルポインタ          */
        unsigned size;                          /* セルのサイズ              */
    }s;
    MemAlign damy;
};

typedef union mem_head MemHead;                 /* セルヘッダデータ型        */

/*
 * EXTERNAL VARIABLE DECLARATIONS
 */
extern MemHead *MEM_empty_top;                  /* 空きセルの先頭            */

/*
 * EXTERNAL FUNCTION PROTOTYPE  DECLARATIONS
 */
void MEM_Init(Uint32, Uint32);
void *MEM_Calloc(Uint32, Uint32);
void *MEM_Malloc(Uint32);
void MEM_Free(void *);
void MEM_ClrMemZero(void *, Uint32);

#endif  /* ifndef MEM_MAIN_H */
