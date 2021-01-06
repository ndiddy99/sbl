/*-----------------------------------------------------------------------------
 *  FILE: dma_scu1.c
 *
 *      Copyright(c) 1994 SEGA.
 *
 *  PURPOSE:
 *
 *      SCU DMA高水準。
 *
 *  DESCRIPTION:
 *
 *      SCU DMA高水準転送機能。
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *          DMA_ScuInit             -   DMA初期化
 *          DMA_ScuMemCopy          -   DMAデータ転送
 *          DMA_ScuResult           -   DMAデータ転送終了チェック
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
static Uint8 dma_flg;                   /* DMAの使用有無フラグ */
static Uint8 dma_start_flg;             /* DMAの開始有無フラグ */

/*
 * STATIC FUNCTION PROTOTYPE DECLARATIONS
 */


/******************************************************************************
 *
 * NAME:    DMA_ScuInit()       -   DMA初期化
 *
 * PARAMETERS :
 *      なし。
 *
 * DESCRIPTION:
 *      SCUの初期化をします。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし。
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */

void DMA_ScuInit(void)
{
    Uint32 msk;

    msk = get_imask();
    set_imask(15);

    INT_ChgMsk(INT_MSK_DMA0, INT_MSK_NULL);     /* DMA0割り込み終了マスク解除*/
    dma_start_flg = OFF;                        /* DMAはスタートしていない   */

    set_imask(msk);                                         /* 割り込みPOP   */
}

/******************************************************************************
 *
 * NAME:    DMA_ScuMemCopy()    -   DMAデータ転送
 *
 * PARAMETERS :
 *      (1) void *dst   -   <i>   ディスティネーションアドレス.
 *      (2) void *src   -   <i>   ソースアドレス.
 *      (3) Uint32 cnt  -   <i>   転送バイト数.
 *
 * DESCRIPTION:
 *      SCUのDMAモード1の直接モードによるDMA転送をします。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし。
 *
 * CAVEATS:
 *      なし。
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
    dma_start_flg = ON;                         /* DMAはスタートしている     */
    set_imask(msk);                                         /* 割り込みPOP   */
}

/******************************************************************************
 *
 * NAME:    DMA_ScuResult()     -   DMAデータ転送終了チェック
 *
 * PARAMETERS :
 *      なし。
 *
 * DESCRIPTION:
 *      SCUのDMAモード2の直接モードによるDMA転送をします。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      Uint32 DMA_ScuMemCopy()の結果。
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */

Uint32 DMA_ScuResult(void)
{
    DmaScuStatus status;
    Uint32 msk;

    msk = get_imask();
    set_imask(15);

    if(dma_start_flg == ON){                    /* DMAを開始している時       */
        if(dma_flg == OFF){
            set_imask(msk);                                 /* 割り込みPOP   */
            return(DMA_SCU_END);
        }else{
        DMA_ScuGetStatus(&status, DMA_SCU_CH0);
            if(status.dxmv == DMA_SCU_MV){
                set_imask(msk);                             /* 割り込みPOP   */
                return(DMA_SCU_BUSY);
            }
            CSH_Purge(dma_scu_dis_adr, dma_scu_cnt);
            set_imask(msk);                                 /* 割り込みPOP   */
            return(DMA_SCU_END);
        }
    }else{                                      /* DMAを開始していない時     */
        set_imask(msk);                                     /* 割り込みPOP   */
        return(DMA_SCU_END);
    }
}

