/*-----------------------------------------------------------------------------  *  FILE: dma_cpu5.c
 *
 *      Copyright(c) 1994 SEGA.
 *
 *  PURPOSE:
 *
 *      CPU DMA�������B
 *
 *  DESCRIPTION:
 *
 *      CPU DMA�������]���@�\�B
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *          (5) DMA_CpuResult           -   �f�[�^�]���I���`�F�b�N
 *
 *  CAVEATS:
 *
 *
 *  AUTHOR(S)
 *
 *      1994-07-05  N.T Ver.1.00
 *
 *  MOD HISTORY:
 *
 *-----------------------------------------------------------------------------
 */

/*
 * C STANDARD LIBRARY FUNCTIONS/MACROS DEFINES
 */

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
#include "sega_mth.h"
#include "sega_csh.h"
#include "sega_xpt.h"

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include "dma_cpu0.h"
#include "dma_cpum.h"

/*
 * GLOBAL DECLARATIONS
 */

/*
 * LOCAL DEFINES/MACROS
 */

/*
 * STATIC DECLARATIONS
 */
void *dma_cpu_dis_adr;                   /* �߰�ޑΏ��ި�èȰ��ݱ��ڽ */
Uint32 dma_cpu_cnt;                      /* �߰�ޑΏ��ި�èȰ��ݶ���  */

/*
 * STATIC FUNCTION PROTOTYPE DECLARATIONS
 */


/******************************************************************************
 *
 * NAME:    DMA_CpuResult()     -   �f�[�^�]���I���`�F�b�N
 *
 * PARAMETERS :
 *      (2) void *src       <i>   �\�[�X�A�h���X.
 *      (3) Uint32 cnt      <i>   �]����(0�`16777215)
 *
 * DESCRIPTION:
 *      src�A�h���X����dst�A�h���X��16�o�C�g�f�[�^��cnt�񐔕��]�����܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      Uint32 DMA_CpuMemCopy1,2,4,16()�̌���.
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */

Uint32 DMA_CpuResult(void)
{
    DmaCpuComStatus com_status;             /* ���ʃX�e�[�^�X                */
    DmaCpuStatus status;                    /* �X�e�[�^�X                    */
                                            /*********************************/
    DMA_CpuGetComStatus(&com_status);       /* ���ʃX�e�[�^�X�擾            */
    if(com_status.ae == DMA_CPU_ADR_ERR){   /* ���ڽ�װ�����������ꍇ        */
        return(DMA_CPU_FAIL);               /* �ُ�I�������^�[��            */
    }
    status = DMA_CpuGetStatus(DMA_CPU_CH0); /* �X�e�[�^�X�擾                */
    if(status == DMA_CPU_TE_MV){            /* ���쒆�ł���ꍇ              */
        return(DMA_CPU_BUSY);               /* ���s�������^�[��              */
    }
    CSH_Purge(dma_cpu_dis_adr, dma_cpu_cnt);
    return(DMA_CPU_END);                    /* ����I�������^�[��            */
}

