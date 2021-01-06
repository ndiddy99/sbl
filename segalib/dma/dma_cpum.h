/*-----------------------------------------------------------------------------
 *  FILE: dma_cpum.h
 *
 *      Copyright(c) 1994 SEGA.
 *
 *  PURPOSE:
 *
 *      CPU DMA高水準ヘッダファイル。
 *
 *  DESCRIPTION:
 *
 *      CPU DMA高水準転送機能。
 *
 *  AUTHOR(S)
 *
 *      1994-07-05  N.T Ver.1.00
 *
 *  MOD HISTORY:
 *
 *-----------------------------------------------------------------------------
 */

#ifndef DMA_CPUM_H
#define DMA_CPUM_H

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
#include "sega_xpt.h"

/*
 * GLOBAL DEFINES/MACROS DEFINES
 */

/* 定数名 */
/**** データ転送終了チェック *************************************************/
#define DMA_CPU_END     0                       /* 正常終了                  */
#define DMA_CPU_FAIL    1                       /* 異常終了                  */
#define DMA_CPU_BUSY    2                       /* 実行中                    */

/*
 * STRUCTURE DECLARATIONS
 */

/*
 * TYPEDEFS
 */

/*
 * EXTERNAL VARIABLE DECLARATIONS
 */

/*
 * EXTERNAL FUNCTION PROTOTYPE  DECLARATIONS
 */
void DMA_CpuMemCopy1(void *, void *, Uint32);   /* データ転送(バイト単位)    */
void DMA_CpuMemCopy2(void *, void *, Uint32);   /* データ転送(ワード単位)    */
void DMA_CpuMemCopy4(void *, void *, Uint32);   /* データ転送(ロングワード-  */
                                                /* 単位)                     */
void DMA_CpuMemCopy16(void *, void *, Uint32);  /* データ転送(16バイト単位)  */
Uint32 DMA_CpuResult(void);                     /* データ転送終了チェック    */

#endif  /* ifndef DMA_CPUM_H */
