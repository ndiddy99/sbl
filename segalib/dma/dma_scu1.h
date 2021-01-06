/*-----------------------------------------------------------------------------
 *  FILE: dma_scu1.h
 *
 *      Copyright(c) 1994 SEGA.
 *
 *  PURPOSE:
 *
 *      SCU DMA�������w�b�_�t�@�C���B
 *
 *  DESCRIPTION:
 *
 *      SCU DMA�������]���@�\�B
 *
 *  AUTHOR(S)
 *
 *      1994-07-05  N.T Ver.1.00
 *
 *  MOD HISTORY:
 *
 *      1994-09-30  N.T Ver.1.01
 *
 *-----------------------------------------------------------------------------
 */

#ifndef DMA_SCU1_H
#define DMA_SCU1_H

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
#include "sega_xpt.h"

/*
 * GLOBAL DEFINES/MACROS DEFINES
 */
#define DMA_SCU_END     0
#define DMA_SCU_FAIL    1
#define DMA_SCU_BUSY    2

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
void DMA_ScuInit(void);                         /* DMA�f�[�^������           */
void DMA_ScuMemCopy(void *, void *, Uint32);    /* DMA�f�[�^�]��             */
Uint32 DMA_ScuResult(void);                     /* DMA�f�[�^�]���I���`�F�b�N */

#endif  /* ifndef DMA_SCU1_H */
