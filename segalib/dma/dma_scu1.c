/*-----------------------------------------------------------------------------
 *  FILE: dma_scu1.c
 *
 *      Copyright(c) 1994 SEGA.
 *
 *  PURPOSE:
 *
 *      SCU DMA�������B
 *
 *  DESCRIPTION:
 *
 *      SCU DMA�������]���@�\�B
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *          DMA_ScuInit             -   DMA������
 *          DMA_ScuMemCopy          -   DMA�f�[�^�]��
 *          DMA_ScuResult           -   DMA�f�[�^�]���I���`�F�b�N
 *
 *  CAVEATS:
 *
 *
 *  AUTHOR(S)
 *
 *      1994-08-31  N.T Ver.1.01
 *
 *  MOD HISTORY:
 *
 *      1994-10-08  N.T Ver.1.05
 *
 *-----------------------------------------------------------------------------
 */

/*
 * C STANDARD LIBRARY FUNCTIONS/MACROS DEFINES
 */

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
#include <machine.h>
#include "sega_xpt.h"
#include "sega_csh.h"
#include "sega_int.h"
/*
 * USER SUPPLIED INCLUDE FILES
 */
#include <string.h>
#include "dma_xpt.h"
#include "dma_scu0.h"
#include "dma_scu1.h"

/*
 * GLOBAL DECLARATIONS
 */

/*
 * LOCAL DEFINES/MACROS
 */
/* SCU Address */
#define CADR_B_BUS_START   ((Uint32)0x5a00000)
#define CADR_B_BUS_END     ((Uint32)0x5fe0000)
#define ADR_B_BUS_START    ((Uint32)0x25a00000)
#define ADR_B_BUS_END      ((Uint32)0x25fe0000)

#define CADR_WORKRAM_L_START   ((void *)0x200000)
#define CADR_WORKRAM_L_END     ((void *)0x300000)
#define ADR_WORKRAM_L_START    ((void *)0x20200000)
#define ADR_WORKRAM_L_END      ((void *)0x20300000)

/*
 * STATIC DECLARATIONS
 */
static void *dma_scu_dis_adr;
static Uint32 dma_scu_cnt;
static Uint8 dma_flg;                   /* DMA�̎g�p�L���t���O */
static Uint8 dma_start_flg;             /* DMA�̊J�n�L���t���O */

/*
 * STATIC FUNCTION PROTOTYPE DECLARATIONS
 */


/******************************************************************************
 *
 * NAME:    DMA_ScuInit()       -   DMA������
 *
 * PARAMETERS :
 *      �Ȃ��B
 *
 * DESCRIPTION:
 *      SCU�̏����������܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      �Ȃ��B
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */

void DMA_ScuInit(void)
{
    Uint32 msk;

    msk = get_imask();
    set_imask(15);

    INT_ChgMsk(INT_MSK_DMA0, INT_MSK_NULL);     /* DMA0���荞�ݏI���}�X�N����*/
    dma_start_flg = OFF;                        /* DMA�̓X�^�[�g���Ă��Ȃ�   */

    set_imask(msk);                                         /* ���荞��POP   */
}

/******************************************************************************
 *
 * NAME:    DMA_ScuMemCopy()    -   DMA�f�[�^�]��
 *
 * PARAMETERS :
 *      (1) void *dst   -   <i>   �f�B�X�e�B�l�[�V�����A�h���X.
 *      (2) void *src   -   <i>   �\�[�X�A�h���X.
 *      (3) Uint32 cnt  -   <i>   �]���o�C�g��.
 *
 * DESCRIPTION:
 *      SCU��DMA���[�h1�̒��ڃ��[�h�ɂ��DMA�]�������܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      �Ȃ��B
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */

void DMA_ScuMemCopy(void *dst, void *src, Uint32 cnt)
{
    DmaScuPrm prm;
    Uint32 msk;

    msk = get_imask();
    set_imask(15);

    if(((dst >= ADR_WORKRAM_L_START) && (dst < ADR_WORKRAM_L_END)) ||
       ((dst >= CADR_WORKRAM_L_START) && (dst < CADR_WORKRAM_L_END)) ||
       ((src >= ADR_WORKRAM_L_START) && (src < ADR_WORKRAM_L_END)) ||
       ((src >= CADR_WORKRAM_L_START) && (src < CADR_WORKRAM_L_END))){
    /* NO DMA ****************************************************************/
        dma_flg = OFF;
        memcpy(dst, src, cnt);
    /* DMA *******************************************************************/
    }else{
        dma_flg = ON;

    dma_scu_dis_adr = dst;
    dma_scu_cnt = (Uint32)cnt;

    prm.dxr = (Uint32)src;
    prm.dxw = (Uint32)dst;
    prm.dxc = cnt;
    prm.dxad_r = DMA_SCU_R4;

    if(((prm.dxw >= ADR_B_BUS_START) & (prm.dxw < ADR_B_BUS_END)) ||
       ((prm.dxw >= CADR_B_BUS_START) & (prm.dxw < CADR_B_BUS_END))){
       prm.dxad_w = DMA_SCU_W2;
    }else{
       prm.dxad_w = DMA_SCU_W4;
    }

    prm.dxmod = DMA_SCU_DIR;
    prm.dxrup = DMA_SCU_KEEP;
    prm.dxwup = DMA_SCU_KEEP;
    prm.dxft = DMA_SCU_F_DMA;
    prm.msk = DMA_SCU_M_DXR    |
              DMA_SCU_M_DXW    ;

    DMA_ScuSetPrm(&prm, DMA_SCU_CH0);
    DMA_ScuStart(DMA_SCU_CH0);
    }
    dma_start_flg = ON;                         /* DMA�̓X�^�[�g���Ă���     */
    set_imask(msk);                                         /* ���荞��POP   */
}

/******************************************************************************
 *
 * NAME:    DMA_ScuResult()     -   DMA�f�[�^�]���I���`�F�b�N
 *
 * PARAMETERS :
 *      �Ȃ��B
 *
 * DESCRIPTION:
 *      SCU��DMA���[�h2�̒��ڃ��[�h�ɂ��DMA�]�������܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      Uint32 DMA_ScuMemCopy()�̌��ʁB
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */

Uint32 DMA_ScuResult(void)
{
    DmaScuStatus status;
    Uint32 msk;

    msk = get_imask();
    set_imask(15);

    if(dma_start_flg == ON){                    /* DMA���J�n���Ă��鎞       */
        if(dma_flg == OFF){
            set_imask(msk);                                 /* ���荞��POP   */
            return(DMA_SCU_END);
        }else{
        DMA_ScuGetStatus(&status, DMA_SCU_CH0);
            if(status.dxmv == DMA_SCU_MV){
                set_imask(msk);                             /* ���荞��POP   */
                return(DMA_SCU_BUSY);
            }
            CSH_Purge(dma_scu_dis_adr, dma_scu_cnt);
            set_imask(msk);                                 /* ���荞��POP   */
            return(DMA_SCU_END);
        }
    }else{                                      /* DMA���J�n���Ă��Ȃ���     */
        set_imask(msk);                                     /* ���荞��POP   */
        return(DMA_SCU_END);
    }
}

