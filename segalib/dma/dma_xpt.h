/*-----------------------------------------------------------------------------
 *  FILE: dma_xpt.h
 *
 *      Copyright(c) 1994 SEGA.
 *
 *  PURPOSE:
 *
 *      DMA共通ヘッダファイル。
 *
 *  DESCRIPTION:
 *
 *
 *  AUTHOR(S)
 *
 *      1994-05-09  N.T Ver.0.90
 *
 *  MOD HISTORY:
 *
 *-----------------------------------------------------------------------------
 */

#ifndef DMA_XPT_H
#define DMA_XPT_H

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
#include "sega_xpt.h"

/*
 * GLOBAL DEFINES/MACROS DEFINES
 */
/* 関数形式マクロ */
/**** メモリライト ***********************************************************/
#define DMA_POKE_B(adr, data)   (*((volatile Uint8  *)(adr))) = (Uint8 )(data)
#define DMA_POKE_W(adr, data)   (*((volatile Uint16 *)(adr))) = (Uint16)(data)
#define DMA_POKE_L(adr, data)   (*((volatile Uint32 *)(adr))) = (Uint32)(data)

/**** メモリリード ***********************************************************/
#define DMA_PEEK_B(adr) (*(volatile Uint8  *)(adr))
#define DMA_PEEK_W(adr) (*(volatile Uint16 *)(adr))
#define DMA_PEEK_L(adr) (*(volatile Uint32 *)(adr))

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

#endif  /* ifndef DMA_XPT_H */
