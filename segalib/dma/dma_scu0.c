/*-----------------------------------------------------------------------------
 *  FILE: dma_scu0.c
 *
 *      Copyright(c) 1994 SEGA.
 *
 *  PURPOSE:
 *
 *      SCU DMA低水準。
 *
 *  DESCRIPTION:
 *
 *      SCU DMA低水準転送機能。
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *          (1) DMA_ScuSetPrm           -   DMA転送パラメータ設定
 *          (2) DMA_ScuStart            -   DMA転送開始
 *          (3) DMA_ScuGetStatus        -   DMAステータス取得
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
 *      1994-10-12  N.T Ver.1.04
 *
 *-----------------------------------------------------------------------------
 */

/*
 * C STANDARD LIBRARY FUNCTIONS/MACROS DEFINES
 */

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
#include "sega_xpt.h"
#include "sega_int.h"
#include <machine.h>

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include "dma_xpt.h"
#include "dma_scu0.h"

/*
 * GLOBAL DECLARATIONS
 */

/*
 * LOCAL DEFINES/MACROS
 */

/* 定数 */
/**** 共通 *******************************************************************/
#define MAX_CHANEL  3                       /* DMAチャネル数                 */
/**** DMAレジスタアドレス ****************************************************/
/******** チャネル0 **********************************************************/
#ifndef _DEB
#define REG_DXR     ((volatile Uint32 *)0x25fe0000)  /* 読み込みアドレス              */
#define REG_DXW     ((volatile Uint32 *)0x25fe0004)  /* 書き出しアドレス              */
#define REG_DXC     ((volatile Uint32 *)0x25fe0008)  /* 転送バイト数　　　            */
#define REG_DXAD    ((volatile Uint32 *)0x25fe000C)  /* 加算値レジスタ　　            */
#define REG_DXEN    ((volatile Uint32 *)0x25fe0010)  /* DMA許可レジスタ 　            */
#define REG_DXMD    ((volatile Uint32 *)0x25fe0014)  /* DMAﾓｰﾄﾞ、ｱﾄﾞﾚｽ更新、起動要因　*/
#else
#define REG_DXR   ((Uint32 *)0x06060000)  /* 読み込みアドレス              */
#define REG_DXW   ((Uint32 *)0x06060004)  /* 書き出しアドレス              */
#define REG_DXC   ((Uint32 *)0x06060008)  /* 転送バイト数　　　            */
#define REG_DXAD  ((Uint32 *)0x0606000C)  /* 加算値レジスタ　　            */
#define REG_DXEN  ((Uint32 *)0x06060010)  /* DMA許可レジスタ 　            */
#define REG_DXMD  ((Uint32 *)0x06060014)  /* DMAﾓｰﾄﾞ、ｱﾄﾞﾚｽ更新、起動要因　*/
#endif /* _DEB */
/******** チャネルオフセット *************************************************/
#define OFSET0_REG  0x00000008              /* REG_DXR   オフセット          */
                                            /* REG_DXW                       */
                                            /* REG_DXC                       */
                                            /* REG_DXAD                      */
                                            /* REG_DXEN                      */
                                            /* REG_DXMD                      */
/******** 共通 ***************************************************************/
#ifndef _DEB
#define REG_DSTP    ((volatile Uint32 *)0x25fe0060)  /* DMA強制停止レジスタ         　*/
#else
#define REG_DSTP   ((Uint32 *)0x06060060)  /* DMA強制停止レジスタ         　*/
#endif /* _DEB */

/***** DMAレジスタマスクビット ***********************************************/
/******** 加算値レジスタ *****************************************************/
#define R_DXAD_DXRAD (1 << DMA_DXAD_DXRAD)      /* 読み込みアドレス加算値    */
#define R_DXAD_DXWAD (3 << DMA_DXAD_DXWAD)      /* 書き込みアドレス加算値    */
/******** DMA許可レジスタ ****************************************************/
#define R_DXEN_DXGO  (1 << DMA_DXEN_DXGO)       /* DMA起動ビット             */
#define R_DXEN_DXEN  (1 << DMA_DXEN_DXEN)       /* DMA許可ビット             */
/******** DMAモード、アドレス更新、起動要因レジスタ **************************/
#define R_DXMD_DXMOD ( 1 << DMA_DXMD_DXMOD)     /* モードビット              */
#define R_DXMD_DXRUP ( 1 << DMA_DXMD_DXRUP)     /* 読み込みアドレス更新ビット*/
#define R_DXMD_DXWUP ( 1 << DMA_DXMD_DXWUP)     /* 書き出しアドレス更新ビット*/
#define R_DXMD_DXFT  ( 7 << DMA_DXMD_DXFT )     /* 起動要因選択ビット        */

/******************************************************************************
 *
 * NAME:    STOP_COM()  -   DMA終了共通処理
 *
 * PARAMETERS :
 *      (1) Uint32  ch  -   <i> DMAチャネル.
 *
 * DESCRIPTION:
 *      指定されたDMAチャネルのDMA終了処理を行います。
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
#define STOP_COM(ch, vector_num)\
    do{\
        INT_SetScuFunc((vector_num), dma_scu_vector[(ch)]);\
        INT_SetFunc((vector_num), dma_vector[(ch)]);\
        dma_int_status[(ch)] = DMA_SCU_NO_MV;\
    }while(FALSE)

/******************************************************************************
 *
 * NAME:    STOP_COM_CHANEL()   -   DMAチャネル別終了共通処理
 *
 * PARAMETERS :
 *      (1) Uint32  ch  -   <i> DMAチャネル.
 *
 * DESCRIPTION:
 *      指定されたDMAチャネルのDMA終了処理を行います。
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
#define STOP_COM_CHANEL(ch, vector_num)\
    do{\
        if(dma_int_status[(ch)] == DMA_SCU_MV){\
            switch((ch)){\
                case DMA_SCU_CH0:\
                    *(vector_num) = INT_SCU_DMA0;\
                    break;\
                case DMA_SCU_CH1:\
                    *(vector_num) = INT_SCU_DMA1;\
                    break;\
                case DMA_SCU_CH2:\
                    *(vector_num) = INT_SCU_DMA2;\
                    break;\
            }\
            STOP_COM((ch), *(vector_num));\
        }\
    }while(FALSE)\

/******************************************************************************
 *
 * NAME:    INTR_FUNC()         -   DMAチャネル別終了割り込み処理
 *
 * PARAMETERS :
 *      ch チャネル
 *      vector_num ベクタ番号
 *
 * DESCRIPTION:
 *      DMAチャネルのDMA終了割り込み処理を行います。
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
#define INTR_FUNC(ch, vector_num)\
    do{\
        Uint32 msk;\
        msk = get_imask();\
        set_imask(15);\
        dma_int_status[(ch)] = DMA_SCU_NO_MV;\
        STOP_COM((ch), (vector_num));           /* DMA終了共通処理          */\
        set_imask(msk);                                     /* 割り込みPOP  */\
	}while(FALSE)

/*
 * STATIC DECLARATIONS
 */

static void (*dma_scu_vector[MAX_CHANEL])(void);/* ベクタ記憶領域(SCU)       */
static void (*dma_vector[MAX_CHANEL])(void);    /* ベクタ記憶領域            */

static Uint8 dma_int_status[MAX_CHANEL];        /* 割り込みステータス        */

/*
 * STATIC FUNCTION PROTOTYPE DECLARATIONS
 */

void DMA_ScuIntFunc0(void);
void DMA_ScuIntFunc1(void);
void DMA_ScuIntFunc2(void);

/******************************************************************************
 *
 * NAME:    DMA_ScuSetPrm() -   DMA転送パラメータ設定
 *
 * PARAMETERS :
 *      (1) DmaScuPrm   *prm  - <i>   転送パラメータ.
 *      (2) Uint32      ch    - <i>   DMAチャネル..
 *
 * DESCRIPTION:
 *      指定されたDMAチャネルへ転送パラメータの値を設定します。
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

void DMA_ScuSetPrm(DmaScuPrm *prm, Uint32 ch)
{
    Uint32 work_reg;
    
    if((prm->msk & DMA_SCU_M_DXR) == DMA_SCU_M_DXR){
        DMA_POKE_L((REG_DXR + OFSET0_REG * ch), prm->dxr);
    }

    if((prm->msk & DMA_SCU_M_DXW) == DMA_SCU_M_DXW){
        DMA_POKE_L((REG_DXW + OFSET0_REG * ch), prm->dxw);
    }

    DMA_POKE_L((REG_DXC + OFSET0_REG * ch), prm->dxc);
    
    DMA_POKE_L((REG_DXAD + OFSET0_REG * ch), (prm->dxad_r | prm->dxad_w));
                                                        /* 加算値ｱﾄﾞﾚｽﾚｼﾞｽﾀ  */
    DMA_POKE_L((REG_DXMD + OFSET0_REG * ch), (prm->dxmod | 
               (prm->dxrup << DMA_DXMD_DXRUP )|
               (prm->dxwup << DMA_DXMD_DXWUP )| prm->dxft));
                                          /* モード、アドレス更新、起動要因  */
}

/******************************************************************************
 *
 * NAME:    DMA_ScuStart()  -   DMA転送開始
 *
 * PARAMETERS :
 *      (1) Uint32  ch  -   <i> DMAチャネル.
 *
 * DESCRIPTION:
 *      指定されたDMAチャネルのDMA転送を開始します。
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

void DMA_ScuStart(Uint32 ch)
{
    Uint32 msk;
    Uint32 now_vector_num;
    void (*intr_func)(void);

    msk = get_imask();
    set_imask(15);
    /* 割り込みベクタ登録 (start)*********************************************/
    switch(ch){
        case DMA_SCU_CH0:
            now_vector_num = INT_SCU_DMA0;
            intr_func = DMA_ScuIntFunc0;
            break;
        case DMA_SCU_CH1:
            now_vector_num = INT_SCU_DMA1;
            intr_func = DMA_ScuIntFunc1;
            break;
        case DMA_SCU_CH2:
            now_vector_num = INT_SCU_DMA2;
            intr_func = DMA_ScuIntFunc2;
            break;
    }
    dma_scu_vector[ch] = INT_GetScuFunc(now_vector_num);
                                                        /* ベクタ待避SCU     */
    dma_vector[ch] = INT_GetFunc(now_vector_num);
                                                        /* ベクタ待避        */
    dma_int_status[ch] = DMA_SCU_MV;
    INT_SetScuFunc(now_vector_num, intr_func);        /* ベクタ登録        */
    INT_SetFunc(now_vector_num, NULL);          /* SCU割り込み関数の再登録   */

    /* 割り込みベクタ登録 (end)***********************************************/

    DMA_POKE_L((REG_DXEN + OFSET0_REG * ch), (R_DXEN_DXGO | R_DXEN_DXEN));
                                                /* DMA許可ﾚｼﾞｽﾀ              */

    set_imask(msk);                                         /* 割り込みPOP   */
}
/******************************************************************************
 *
 * NAME:    DMA_ScuGetStatus() -   DMAステータス取得
 *
 * PARAMETERS :
 *      (1) Uint32  ch              -   <i> DMAチャネル.
 *      (2) DmaScuStatus *status    -   <o> ステータスポインタ.
 *
 * DESCRIPTION:
 *      指定されたDMAチャネルのステータスを取得します。
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

void DMA_ScuGetStatus(DmaScuStatus *status, Uint32 ch)
{
    Uint32 msk;
    msk = get_imask();
    set_imask(15);

    status->dxmv = (Uint32)dma_int_status[ch];

    set_imask(msk);                                         /* 割り込みPOP   */
}

/******************************************************************************
 *
 * NAME:    DMA_ScuIntFunc0()     -   DMA0割り込み処理
 *
 * PARAMETERS :
 *
 * DESCRIPTION:
 *      DMA0の終了割り込みルーチンです。
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

void DMA_ScuIntFunc0(void){
    INTR_FUNC(DMA_SCU_CH0, INT_SCU_DMA0);   /* DMAチャネル別終了割り込み処理 */
}

/******************************************************************************
 *
 * NAME:    DMA_ScuIntFunc1()     -   DMA1割り込み処理
 *
 * PARAMETERS :
 *
 * DESCRIPTION:
 *      DMA1の終了割り込みルーチンです。
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

void DMA_ScuIntFunc1(void){
    INTR_FUNC(DMA_SCU_CH1, INT_SCU_DMA1);   /* DMAチャネル別終了割り込み処理 */
}

/******************************************************************************
 *
 * NAME:    DMA_ScuIntFunc2()     -   DMA2割り込み処理
 *
 * PARAMETERS :
 *
 * DESCRIPTION:
 *      DMA2の終了割り込みルーチンです。
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

void DMA_ScuIntFunc2(void){
    INTR_FUNC(DMA_SCU_CH2, INT_SCU_DMA2);   /* DMAチャネル別終了割り込み処理 */
}
