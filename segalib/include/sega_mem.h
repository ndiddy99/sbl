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
 *      1994-05-18  N.T Ver.0.90
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

#ifndef SEGA_MEM_H
#define SEGA_MEM_H

/*
 * GLOBAL DEFINES/MACROS DEFINES
 */

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
void MEM_Init(Uint32, Uint32);
void *MEM_Calloc(Uint32, Uint32);
void *MEM_Malloc(Uint32);
void MEM_Free(void *);

#endif  /* ifndef SEGA_MEM_H */
