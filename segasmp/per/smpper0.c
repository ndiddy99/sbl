/*-----------------------------------------------------------------------------
 *  FILE: smpper0.c
 *
 *  Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *
 *      SMPCのペリフェラルコントロール系サンプルプログラム
 *
 *  DESCRIPTION:
 *
 *      ペリフェラルデータ、タイムデータ、システムデータを取得します。
 *      以下に書き込みアドレスを示します。
 *
 *          0x6040000 システムデータ
 *          0x6040020 タイムデータ
 *          0x6040030 ペリフェラルマルチタップ情報
 *          0x6040040 ペリフェラルデータ
 *
 *      ペリフェラルデータ取得の設定は以下のとおりです。
 *
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *
 *  CAVEATS:
 *
 *  AUTHOR(S)
 *
 *      1994-05-19  N.T Ver.0.90
 *      1994-08-10  N.T Ver.1.00
 *
 *  MOD HISTORY:
 *      1994-11-14  N.T Ver.1.01
 *
 *-----------------------------------------------------------------------------
 */

/*
 * C STANDARD LIBRARY FUNCTIONS/MACROS DEFINES
 */
#include "sega_xpt.h"
#include "sega_per.h"
#include "sega_tim.h"
#include <machine.h>
#include <string.h>

/*
 * C VIRTUAL TYPES DEFINITIONS
 */

/*
 * USER SUPPLIED INCLUDE FILES
 */

/*
 * GLOBAL DECLARATIONS
 */

/*
 * LOCAL DEFINES/MACROS
 */

#define ADR_BASE      ((Uint32 *)0x6040000)

#define ADR_OUTPUT  (ADR_BASE)
#define ADR_O_SYS     (ADR_OUTPUT)              /* system                    */

#define ADR_O_PER_MUL   ((Uint8 *)(ADR_OUTPUT + 0x30/4))    /* per           */
#define ADR_O_PER   ((Uint8 *)(ADR_OUTPUT + 0x40/4))     /* per              */
#define ADR_O_TIM   ((Uint8 *)(ADR_OUTPUT + 0x20/4))    /* tim               */

/*
 * TYPEDEF
 */
typedef struct {
    Uint8   id;
    Uint8   size;
    Uint8   data[2];
}SmpPerData;


/*
 * STATIC DECLARATIONS
 */
static Uint32 inital_flg;
static Uint32 get_sys_flg;
static PerKind set_kind;

static SmpPerData *per_data;
static PerMulInfo *mul_data;

static Uint8 *tim_data;
static PerGetSys *sys_data;
Uint8 per_work[500];

static Uint32 msk;
static Uint32 v_blank_out_cnt;

/*
 * STATIC FUNCTION PROTOTYPE DECLARATIONS
 */
void vblOutGo(void);
static void Init(void);

/******************************************************************************
 *
 * NAME:    main()      - メイン
 *
 ******************************************************************************
 */

void main(void)
{
    Uint8 set_tim_data[7];
    Uint32 back_v_blank_out_cnt;

    set_tim_data[0] = 0x58;
    set_tim_data[1] = 0x59;
    set_tim_data[2] = 0x23;
    set_tim_data[3] = 0x31;
    set_tim_data[4] = 0x0c;
    set_tim_data[5] = 0x94;
    set_tim_data[6] = 0x19;

    inital_flg = ON;

    PER_SMPC_SET_TIM(set_tim_data);         /* 時刻設定 */
    PER_SMPC_SET_SM(0x12345678);            /* SMPCメモリ設定 */

    get_sys_flg = ON;
    
    /* 割り込み設定 **********************************************************/
    set_imask(0);               /* ﾏｽｸの解除        */
    v_blank_out_cnt = 0;
    back_v_blank_out_cnt = 0;
    INT_SetScuFunc(INT_SCU_VBLK_OUT, vblOutGo);    /* V_BLANKのセット  */
    INT_ChgMsk(INT_MSK_VBLK_OUT, INT_MSK_NULL);

    while(1){     /* 終了待ち */
        if(msk == ON){
            while(NULL == (sys_data = PER_GET_SYS()));
            ((PerGetSys *)ADR_O_SYS)->cc = PER_GS_CC(sys_data);
            ((PerGetSys *)ADR_O_SYS)->ac = PER_GS_AC(sys_data);
            ((PerGetSys *)ADR_O_SYS)->ss = PER_GS_SS(sys_data);
            ((PerGetSys *)ADR_O_SYS)->sm = PER_GS_SM(sys_data);
            ((PerGetSys *)ADR_O_SYS)->stat = PER_GS_SMPC_STAT(sys_data);
            msk = OFF;
        }
        back_v_blank_out_cnt = v_blank_out_cnt;
        while(back_v_blank_out_cnt == v_blank_out_cnt);
        if(inital_flg == ON){               /* イントバック初期化        */
            if(get_sys_flg == ON){
                get_sys_flg = OFF;
                set_kind = PER_KD_SYS;
                Init();
                msk = ON;
            }else{
                set_kind = PER_KD_PERTIM;
                inital_flg = OFF;
                Init();
                INT_ChgMsk(INT_MSK_VBLK_OUT, INT_MSK_NULL);
            }
        }
    }
    for(;;);
}
/******************************************************************************
 *
 * NAME:    vblOutGo()   - V_BRANK OUT　割り込み処理
 *
 ******************************************************************************
*/

void vblOutGo(void)
{
    v_blank_out_cnt ++;
    if(inital_flg == ON){                       /* イントバック初期化        */
    }else{                                      /* ペリフェラル取得処理      */
        PER_LGetPer((PerGetPer **)&per_data, &mul_data);
        *(ADR_O_PER) = per_data[0].id;
        *(ADR_O_PER + 1) = per_data[0].size;
        *(ADR_O_PER + 2) = per_data[0].data[0];
        *(ADR_O_PER + 3) = per_data[0].data[1];
        *(ADR_O_PER + 4) = per_data[1].id;
        *(ADR_O_PER + 5) = per_data[1].size;
        *(ADR_O_PER + 6) = per_data[1].data[0];
        *(ADR_O_PER + 7) = per_data[1].data[1];

        *(ADR_O_PER_MUL) = mul_data[0].id;
        *(ADR_O_PER_MUL + 1) = mul_data[0].con;
        *(ADR_O_PER_MUL + 2) = mul_data[1].id;
        *(ADR_O_PER_MUL + 3) = mul_data[1].con;

        tim_data = PER_GET_TIM();
        *(ADR_O_TIM) = (*(tim_data + 0));
        *(ADR_O_TIM + 1) = (*(tim_data + 1));
        *(ADR_O_TIM + 2) = (*(tim_data + 2));
        *(ADR_O_TIM + 3) = (*(tim_data + 3));
        *(ADR_O_TIM + 4) = (*(tim_data + 4));
        *(ADR_O_TIM + 5) = (*(tim_data + 5));
        *(ADR_O_TIM + 6) = (*(tim_data + 6));
    }
}
/*****************************************************************************/
static void Init(void)
{
    PER_LInit(set_kind,     /* KIND */
             2,             /* NUM */
             2,             /* SIZE */
             per_work,      /* WORK */
             0);            /* V_BLANK SKIP */
}
