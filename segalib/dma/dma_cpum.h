/*-----------------------------------------------------------------------------
 *  FILE: dma_cpum.h
 *
 *      Copyright(c) 1994 SEGA.
 *
 *  PURPOSE:
 *
 *      CPU DMA�������w�b�_�t�@�C���B
 *
 *  DESCRIPTION:
 *
 *      CPU DMA�������]���@�\�B
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

/* �萔�� */
/**** �f�[�^�]���I���`�F�b�N *************************************************/
#define DMA_CPU_END     0                       /* ����I��                  */
#define DMA_CPU_FAIL    1                       /* �ُ�I��                  */
#define DMA_CPU_BUSY    2                       /* ���s��                    */

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
void DMA_CpuMemCopy1(void *, void *, Uint32);   /* �f�[�^�]��(�o�C�g�P��)    */
void DMA_CpuMemCopy2(void *, void *, Uint32);   /* �f�[�^�]��(���[�h�P��)    */
void DMA_CpuMemCopy4(void *, void *, Uint32);   /* �f�[�^�]��(�����O���[�h-  */
                                                /* �P��)                     */
void DMA_CpuMemCopy16(void *, void *, Uint32);  /* �f�[�^�]��(16�o�C�g�P��)  */
Uint32 DMA_CpuResult(void);                     /* �f�[�^�]���I���`�F�b�N    */

#endif  /* ifndef DMA_CPUM_H */
