/*-----------------------------------------------------------------------------
 *  FILE: snd_main.c
 *
 *      Copyright (c) by SEGA.
 *
 *  PURPOSE:
 *
 *      サウンドI/Fライブラリソースファイル。
 *
 *  DESCRIPTION:
 *
 *      サウンドドライバとのインタフェースを実現する。
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *
 *  CAVEATS:
 *
 *      なし。
 *
 *  AUTHOR(S)
 *
 *      1994-05-18  N.T Ver.0.90
 *
 *  MOD HISTORY:
 *
 *      1994-11-08  N.T Ver.1.09
 *      1994-12-30  N.T Ver.1.10
 *
 *-----------------------------------------------------------------------------
 */


/*
 * C VIRTUAL TYPES DEFINITIONS
 */
#include <string.h>
#include <machine.h>
#include "sega_xpt.h"
#include "sega_int.h"
#include "sega_per.h"
#include "sega_dma.h"

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include "snd_main.h"

/*
 * GLOBAL DECLARATIONS
 */
Uint8 *snd_adr_sys_int_work;                 /*ｼｽﾃﾑｲﾝﾀﾌｪｰｽﾜｰｸ先頭ｱﾄﾞﾚｽ格納*/
Uint32 snd_msk_work_work;                    /* sound priority msk        */

/*
 * LOCAL DEFINES/MACROS
 */
#define _DMA_SCU                                /* SCU DMA を使用する        */
/* 定数 */
/**** アドレス ***************************************************************/
#define ADR_SCSP_REG    ((volatile Uint8 *)0x25b00400)   /* SCSP共通制御レジスタ      */
#define ADR_SND_MEM     ((volatile Uint8 *)0x25a00000)   /* サウンドメモリ先頭アドレス*/
#define ADR_SND_VECTOR  ((volatile Uint8 *)0x25a00000)   /* サウンドベクタアドレス    */
#define ADR_SYS_TBL     (ADR_SND_MEM + 0x400)   /* ｼｽﾃﾑｲﾝﾀﾌｪｰｽ領域           */

/**** システムインタフェーステーブルオフセット *******************************/
#define ADR_SYS_INFO    (0x00)                  /* ｼｽﾃﾑ情報ﾃｰﾌﾞﾙｱﾄﾞﾚｽ        */
#define ADR_HOST_INT    (0x04)                  /* ﾎｽﾄｲﾝﾀﾌｪｰｽﾜｰｸｱﾄﾞﾚｽ        */
#define ADR_ARA_CRNT    (0x08)                /* ｻｳﾝﾄﾞｴﾘｱﾏｯﾌﾟCRNTﾜｰｸ先頭ｱﾄﾞﾚｽ*/
#define ADR_SYS_INT_WORK    (0x12)            /* ｼｽﾃﾑｲﾝﾀﾌｪｰｽﾜｰｸ先頭ｱﾄﾞﾚｽ     */
#define ADR_HARD_CHK_STAT   (0x18)            /* ﾊｰﾄﾞﾁｪｯｸﾘﾀｰﾝｽﾃｰﾀｽ格納ﾜｰｸ    */

/**** システム情報テーブルオフセット *****************************************/
#define ADR_PRG_ADR     (0x00)                  /* 68Kﾌﾟﾛｸﾞﾗﾑ領域先頭ｱﾄﾞﾚｽ   */
#define ADR_PRG_SIZE    (0x04)                  /* 68Kﾌﾟﾛｸﾞﾗﾑ領域ｻｲｽﾞ        */
#define ADR_ARA_ADR     (0x08)                  /* ｻｳﾝﾄﾞｴﾘｱﾏｯﾌﾟ領域先頭ｱﾄﾞﾚｽ */

/**** ホストインタフェースワークオフセット ***********************************/
#define ADR_COM_DATA    (0x00)                  /* コマンド                  */
#define ADR_PRM_DATA    (0x02)                  /* パラメータ                */
#define ADR_SONG_STAT   (0x80)                  /* song status               */
#define ADR_TL_VL       (0x90)                  /* Total volume              */
#define ADR_TL_HZ_VL    (0x94)                  /* 周波数帯域別Volume        */
#define ADR_PCM         (0xa0)                  /* PCM                       */
#define ADR_SEQ         (0xb0)                  /* Sequence                  */
/**** ホストインタフェースワーク定数 *****************************************/
#define SIZE_COM_BLOCK      (0x10)              /* ｺﾏﾝﾄﾞﾌﾞﾛｯｸｻｲｽﾞ          */
#define MAX_NUM_COM_BLOCK   8                   /* ｺﾏﾝﾄﾞﾌﾞﾛｯｸ数              */

/**** サウンド起動 ***********************************************************/
#define SCSP_REG_SET    0x0200                  /* SCSPレジスタ設定値        */
#define MEM_CLR_SIZE    0xb000                  /* ｻｳﾝﾄﾞﾒﾓﾘｸﾘｱｻｲｽﾞ           */

/**** サウンドエリアマップ情報(ﾛﾝｸﾞﾜｰﾄﾞ単位) *********************************/
#define ARA_MAP_SIZE        0x2                 /* ｻｳﾝﾄﾞｴﾘｱﾏｯﾌﾟｻｲｽﾞ          */
/******** オフセット値(ﾛﾝｸﾞﾜｰﾄﾞ単位) *****************************************/
#define ARA_MAP_0           0x0         /* ﾃﾞｰﾀ種別,ﾃﾞｰﾀ番号,ｴﾘｱ開始ｱﾄﾞﾚｽ    */
#define ARA_MAP_4           0x1         /* 転送済みﾋﾞｯﾄ,ｴﾘｱｻｲｽﾞ              */
/******** ビット位置 *********************************************************/
#define B_END_MARK          31                  /* ﾃﾞｰﾀ終了ﾋﾞｯﾄ              */
#define B_DATA_ID           28                  /* ﾃﾞｰﾀ種別　　　　　　　    */
#define B_ID_NUM            24                  /* ﾃﾞｰﾀ番号                  */
#define B_START_ADR         0                   /* ｽﾀｰﾄｱﾄﾞﾚｽ                 */
#define B_LOAD_MARK         31                  /* 転送済みﾋﾞｯﾄ              */
#define B_AREA_SIZE         0                   /* ｴﾘｱｻｲｽﾞ                   */
/******** マスクビット *******************************************************/
#define M_END_MARK         (0x1  << B_END_MARK) /* ﾃﾞｰﾀ終了ﾋﾞｯﾄ              */
#define M_DATA_ID          (0x7  << B_DATA_ID)  /* ﾃﾞｰﾀ種別　　　　　　　    */
#define M_ID_NUM           (0xf  << B_ID_NUM)   /* ﾃﾞｰﾀ番号                  */
#define M_START_ADR        (0xfffff << B_START_ADR) /* ｽﾀｰﾄｱﾄﾞﾚｽ             */
#define M_LOAD_MARK        (0x1  << B_LOAD_MARK)/* 転送済みﾋﾞｯﾄ              */
#define M_AREA_SIZE        (0xfffff << B_AREA_SIZE) /* ｴﾘｱｻｲｽﾞ               */

/**** コマンド ***************************************************************/
#define COM_START_SEQ      0x01                 /* Sequence Start            */
#define COM_STOP_SEQ       0x02                 /* Sequence Stop             */
#define COM_PAUSE_SEQ      0x03                 /* Sequence Pause            */
#define COM_CONT_SEQ       0x04                 /* Sequence Continue         */
#define COM_SET_SEQ_VL     0x05                 /* Sequence Volume           */
#define COM_CHG_TEMPO      0x07                 /* Tempo Change              */
#define COM_CHG_MAP        0x08                 /* map Change                */
#define COM_CTRL_DIR_MIDI  0x09                 /* MIDI direct control       */
#define COM_START_VL_ANL   0x0a                 /* Volume analize start      */
#define COM_STOP_VL_ANL    0x0b                 /* Volume analize stop       */
#define COM_STOP_DSP       0x0c                 /* DSP stop                  */
#define COM_OFF_ALL_SOUND  0x0d                 /* Sound all OFF             */
#define COM_SET_SEQ_PAN    0x0e                 /* Sequence PAN              */
#define COM_SET_CD_DA_LEV  0x80                 /* CD-DA Level               */
#define COM_SET_CD_DA_PAN  0x81                 /* CD-DA pan                 */
#define COM_SET_TL_VL      0x82                 /* Total Volume              */
#define COM_CHG_EFCT       0x83                 /* Effect Change             */
#define COM_START_PCM      0x85                 /* PCM start                 */
#define COM_STOP_PCM       0x86                 /* PCM stop                  */
#define COM_CHG_MIX        0x87                 /* Mixer change              */
#define COM_CHG_MIX_PRM    0x88                 /* Mixer parameter change    */
#define COM_CHK_HARD       0x89                 /* Hard check                */
#define COM_CHG_PCM_PRM    0x8a                 /* PCM parameter change      */

/* 処理マクロ */
/**** メモリライト ***********************************************************/
#define POKE_B(adr, data)   (*((volatile Uint8 *)(adr)) = ((Uint8)(data)))   /* ﾊﾞｲﾄ  */
#define POKE_W(adr, data)   (*((volatile Uint16 *)(adr)) = ((Uint16)(data))) /* ﾜｰﾄﾞ  */
#define POKE_L(adr, data)   (*((volatile Uint32 *)(adr)) = ((Uint32)(data))) /* ﾛﾝｸﾞ  */
/**** メモリリード ***********************************************************/
#define PEEK_B(adr)         (*((volatile Uint8 *)(adr)))                     /* ﾊﾞｲﾄ  */
#define PEEK_W(adr)         (*((volatile Uint16 *)(adr)))                    /* ﾜｰﾄﾞ  */
#define PEEK_L(adr)         (*((volatile Uint32 *)(adr)))                    /* ﾛﾝｸﾞ  */
/**** ﾛﾝｸﾞﾜｰﾄﾞﾊﾞｳﾝﾀﾞﾘ変換(Middle SCUの制限回避 *******************************/
#define CHG_LONG(x)    (((x) * 2) + (0x4 - ( ((x) * 2) % 4) ))
/**** ホストインタフェースワークマクロ ***************************************/
#define MAX_ADR_COM_DATA                        /* 最大ｺﾏﾝﾄﾞﾃﾞｰﾀｱﾄﾞﾚｽ     */\
    (adr_host_int_work + ADR_COM_DATA + (SIZE_COM_BLOCK * MAX_NUM_COM_BLOCK))
#define NOW_ADR_COM_DATA                        /* 現在ｺﾏﾝﾄﾞﾃﾞｰﾀｱﾄﾞﾚｽ     */\
    (adr_com_block + ADR_COM_DATA)
/**** サウンドドライバコマンドブロック設定関数初期処理マクロ *****************/
#define HOST_SET_INIT()\
    Uint32 msk;\
    do{\
        msk = get_imask();\
        set_imask(0xf);\
    }while(FALSE)
/**** サウンドドライバコマンドブロック設定関数リターンマクロ *****************/
#define HOST_SET_RETURN(ret)\
    do{\
        set_imask(msk);\
        return (ret);\
    }while(FALSE)

/******************************************************************************
 *
 * NAME:    SET_COMMAND()       - コマンドセット
 *
 * PARAMETERS :
 *      (1) Uint16 set_com      - <i>   コマンド
 *
 * DESCRIPTION:
 *      コマンドをインタフェース領域にセットします。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし
 *
 * CAVEATS:
 *      ｺﾏﾝﾄﾞへのWRITEは16bitで行ないます。16bit中の上位8bitへｺﾏﾝﾄﾞをｾｯﾄします
 *
 ******************************************************************************
 */

#define SET_COMMAND(set_com)\
(POKE_W((adr_com_block + ADR_COM_DATA), (Uint16)(set_com) << 8)) /* コマンドセット   */

/******************************************************************************
 *
 * NAME:    SET_PRM()           - パラメータセット
 *
 * PARAMETERS :
 *      (1) Uint32 no           - <i>   パラメータ番号
 *      (2) Uint8 set_prm       - <i>   パラメータ
 *
 * DESCRIPTION:
 *      パラメータをインタフェース領域にセットします。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */

#define SET_PRM(no, set_prm)\
(POKE_B(adr_com_block + ADR_PRM_DATA + (no), (set_prm))) /* ﾊﾟﾗﾒｰﾀセット      */

/*
 * STATIC DECLARATIONS
 */ 
static Uint8 *adr_sys_info_tbl;                 /* ｼｽﾃﾑ情報ﾃｰﾌﾞﾙｱﾄﾞﾚｽ格納    */
static Uint8 *adr_host_int_work;                /* ﾎｽﾄｲﾝﾀﾌｪｰｽﾜｰｸ先頭ｱﾄﾞﾚｽ格納*/
static Uint32 *adr_snd_area_crnt;             /* ｻｳﾝﾄﾞｴﾘｱﾏｯﾌﾟCRNTﾜｰｸ先頭ｱﾄﾞﾚｽ*/
static Uint16 *adr_song_stat;                   /* song status               */
static Uint16 *adr_tl_vl;                       /* Total volume              */
static Uint16 *adr_tl_hz_vl;                    /* 周波数帯域別Volume        */
static Uint16 *adr_pcm;                         /* PCM                       */
static Uint16 *adr_seq;                         /* Sequence                  */
static Uint8  *adr_com_block;                   /* 現在書き込みｺﾏﾝﾄﾞﾌﾞﾛｯｸ      */

/*
 * STATIC FUNCTION PROTOTYPE DECLARATIONS
 */
static void DmaClrZero(void *, Uint32);
static void GetSndMapInfo(void **, Uint32 **, Uint16, Uint16);
static Uint16 ChgPan(SndPan);
static void CopyMem(void *,void *,Uint32);
static Uint8 GetComBlockAdr(void);

/******************************************************************************
 *
 * NAME:    SND_Init()  - サウンドシステム起動
 *
 * PARAMETERS :
 *      (1) SndIniDt *sys_ini       - <i>   システム起動用データ
 *
 * DESCRIPTION:
 *      システム起動用データに従って、サウンドシステムを起動する。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */

void SND_Init(SndIniDt *sys_ini)
{
    /** BEGIN ****************************************************************/
#ifdef _DMA_SCU
    DMA_ScuInit();                              /* DMA SCU初期化処理         */
#endif /* _DMA_SCU */

    PER_SMPC_SND_OFF();                         /* サウンドOFF               */
    POKE_W(ADR_SCSP_REG, SCSP_REG_SET); 
                                                /* SCSP共通ﾚｼﾞｽﾀ設定         */
    DmaClrZero((void *)ADR_SND_MEM, MEM_CLR_SIZE);      /* DMAメモリゼロクリア       */
    CopyMem((void *)ADR_SND_VECTOR,
                   (void *)(SND_INI_PRG_ADR(*sys_ini)),
                   SND_INI_PRG_SZ(*sys_ini));   /* 68Kﾌﾟﾛｸﾞﾗﾑ転送            */

    adr_sys_info_tbl = (Uint8 *)(ADR_SND_MEM + PEEK_L(ADR_SYS_TBL +
                                 ADR_SYS_INFO));
                                                /* ｼｽﾃﾑ情報ﾃｰﾌﾞﾙｱﾄﾞﾚｽ取得    */
    adr_host_int_work = (Uint8 *)(ADR_SND_MEM + PEEK_L(ADR_SYS_TBL +
                                  ADR_HOST_INT));
                                                /* ﾎｽﾄｲﾝﾀﾌｪｰｽﾜｰｸｱﾄﾞﾚｽ取得    */
    snd_adr_sys_int_work = (Uint8 *)(ADR_SND_MEM + 
                 ((Uint32)PEEK_W(ADR_SYS_TBL + ADR_SYS_INT_WORK) << 16
                  | (Uint32)PEEK_W(ADR_SYS_TBL + ADR_SYS_INT_WORK + 2)));
                                                /* ｼｽﾃﾑｲﾝﾀﾌｪｰｽﾜｰｸｱﾄﾞﾚｽ取得   */
                                                
    adr_com_block = adr_host_int_work;  /* 現在書き込みｺﾏﾝﾄﾞﾌﾞﾛｯｸｱﾄﾞﾚｽ初期化 */
                                                
    adr_snd_area_crnt = (Uint32 *)(ADR_SND_MEM + 
                                  PEEK_L(ADR_SYS_TBL + ADR_ARA_CRNT));
                                                /* ｻｳﾝﾄﾞｴﾘｱﾏｯﾌﾟCRNTﾜｰｸ取得   */
    adr_song_stat = (Uint16 *)(adr_host_int_work + ADR_SONG_STAT);
    adr_pcm = (Uint16 *)(adr_host_int_work + ADR_PCM);
    adr_seq = (Uint16 *)(adr_host_int_work + ADR_SEQ);
    adr_tl_vl = (Uint16 *)(adr_host_int_work + ADR_TL_VL);
    adr_tl_hz_vl = (Uint16 *)(adr_host_int_work + ADR_TL_HZ_VL);

    CopyMem((void *)
                    (PEEK_L(adr_sys_info_tbl + ADR_ARA_ADR) + ADR_SND_MEM),
                   (void *)(SND_INI_ARA_ADR(*sys_ini)),
                   CHG_LONG(SND_INI_ARA_SZ(*sys_ini))); /* ｻｳﾝﾄﾞｴﾘｱﾏｯﾌﾟ転送  */

    PER_SMPC_SND_ON();                          /* サウンドON                */

}

/******************************************************************************
 *
 * NAME:    SND_MoveData()  - サウンドデータ転送
 *
 * PARAMETERS :
 *      (1) Uint16 *source      - <i>   サウンドデータ転送元アドレス
 *      (2) Uint32 size         - <i>   サウンドデータ転送サイズ
 *      (3) Uint16 data_kind    - <i>   データ種別
 *      (4) Uint16 data_no      - <i>   データ番号
 *
 * DESCRIPTION:
 *      サウンドデータをサウンドメモリへ転送します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */

void SND_MoveData(Uint16 *source, Uint32 size,Uint16 data_kind, Uint16 data_no)
{
    void *adr;                                  /* 転送先アドレス            */
    Uint32 *load_mark_adr;                      /* 転送済みビット設定ｱﾄﾞﾚｽ  */

    GetSndMapInfo(&adr, &load_mark_adr, data_kind, data_no);
                                                /* ｻｳﾝﾄﾞｴﾘｱﾏｯﾌﾟ情報取得      */

    CopyMem(adr, (void *)source, size);         /* ｻｳﾝﾄﾞｴﾘｱﾏｯﾌﾟ転送         */
    POKE_L(load_mark_adr, (*load_mark_adr | M_LOAD_MARK));
                                                /* 転送済みﾋﾞｯﾄ ｵﾝ            */
}

/******************************************************************************
 *
 * NAME:    SND_ChgMap()    - サウンドエリアマップ変更
 *
 * PARAMETERS :
 *      (1) SndAreaMap area_no  - <i>   サウンドエリアマップ番号
 *
 * DESCRIPTION:
 *      サウンドエリアマップを変更します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   コマンド実行状態
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
 
 SndRet SND_ChgMap(SndAreaMap area_no)
 {

    HOST_SET_INIT();                            /* ﾎｽﾄｲﾝﾀﾌｪｰｽｴﾘｱ設定初期処理 */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, area_no);                        /* パラメータセット          */
    SET_COMMAND(COM_CHG_MAP);                   /* コマンドセット            */

    while(PEEK_W(adr_com_block + ADR_COM_DATA));
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, area_no);                        /* パラメータセット          */
    SET_COMMAND(COM_CHG_MAP);                   /* コマンドセット            */
    while(PEEK_W(adr_com_block + ADR_COM_DATA));
    HOST_SET_RETURN(SND_RET_SET);
}
/******************************************************************************
 *
 * NAME:    SND_SetTlVl()   - 全体音量設定
 *
 * PARAMETERS :
 *      (1) SndTlVl vol         - <i>   全体音量
 *
 * DESCRIPTION:
 *      全体音量を設定します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   コマンド実行状態
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
 
 SndRet SND_SetTlVl(SndTlVl vol)
 {
    HOST_SET_INIT();                            /* ﾎｽﾄｲﾝﾀﾌｪｰｽｴﾘｱ設定初期処理 */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, vol);                            /* パラメータセット          */
    SET_COMMAND(COM_SET_TL_VL);                 /* コマンドセット            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_ChgEfct()   - エフェクト変更
 *
 * PARAMETERS :
 *      (1) SndEfctBnkNum efct_no <i>   Effect bank number
 *
 * DESCRIPTION:
 *      エフェクトを変更します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   コマンド実行状態
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
 
 SndRet SND_ChgEfct(SndEfctBnkNum efct_no)
 {
    HOST_SET_INIT();                            /* ﾎｽﾄｲﾝﾀﾌｪｰｽｴﾘｱ設定初期処理 */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, efct_no);                        /* パラメータセット          */
    SET_COMMAND(COM_CHG_EFCT);                  /* コマンドセット            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_ChgMix()    - ミキサ変更
 *
 * PARAMETERS :
 *      (1) SndToneBnkNum tone_no   <i>   音色 bank number
 *      (2) SndMixBnkNum mix_no     <i>   MIxer bank number
 *
 * DESCRIPTION:
 *      ミキサを変更します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   コマンド実行状態
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
 
 SndRet SND_ChgMix(SndToneBnkNum tone_no, SndMixBnkNum mix_no)
 {
    HOST_SET_INIT();                            /* ﾎｽﾄｲﾝﾀﾌｪｰｽｴﾘｱ設定初期処理 */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, tone_no);                        /* パラメータセット          */
    SET_PRM(1, mix_no);                         /* パラメータセット          */
    SET_COMMAND(COM_CHG_MIX);                   /* コマンドセット            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_ChgMixPrm()     - ミキサパラメータ変更
 *
 * PARAMETERS :
 *      (1) SndEfctOut efct_out     <i>   Effect out select
 *      (2) SndLev level            <i>   Effect return Level
 *      (3) SndPan pan              <i>   Effect pan
 *
 * DESCRIPTION:
 *      ミキサパラメータを変更します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   コマンド実行状態
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
 
 SndRet SND_ChgMixPrm(SndEfctOut efct_out, SndLev level, SndPan pan)
 {
    HOST_SET_INIT();                            /* ﾎｽﾄｲﾝﾀﾌｪｰｽｴﾘｱ設定初期処理 */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, efct_out);                       /* パラメータセット          */
    SET_PRM(1, ChgPan(pan) | (level << 5));     /* パラメータセット          */
    SET_COMMAND(COM_CHG_MIX_PRM);               /* コマンドセット            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_ChkHard()       - ハードウェアチェック
 *
 * PARAMETERS :
 *      (1) SndHardStat *stat       <o>   ﾊｰﾄﾞﾁｪｯｸｽﾃｰﾀｽ
 *      (2) SndHardPrm prm         <i>   ﾊｰﾄﾞﾁｪｯｸﾊﾟﾗﾒｰﾀ
 *
 * DESCRIPTION:
 *      ハードウェアをチェックします。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   コマンド実行状態
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
 
 SndRet SND_ChkHard(SndHardStat *stat, SndHardPrm prm)
 {
    HOST_SET_INIT();                            /* ﾎｽﾄｲﾝﾀﾌｪｰｽｴﾘｱ設定初期処理 */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    *(Uint16 *)(ADR_SYS_TBL + ADR_HARD_CHK_STAT) = 0;
    SET_PRM(0, prm);                            /* パラメータセット          */
    SET_COMMAND(COM_CHK_HARD);                  /* コマンドセット            */
    while((*stat = *(Uint16 *)(ADR_SYS_TBL + ADR_HARD_CHK_STAT)) == 0);
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_StopDsp()   - DSP中止
 *
 * PARAMETERS :
 *      なし。
 *
 * DESCRIPTION:
 *      DSPを中止します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   コマンド実行状態
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
 
 SndRet SND_StopDsp(void)
 {
    HOST_SET_INIT();                            /* ﾎｽﾄｲﾝﾀﾌｪｰｽｴﾘｱ設定初期処理 */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_COMMAND(COM_STOP_DSP);                  /* コマンドセット            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_OffAllSound()       - 全サウンド発音スロット停止
 *
 * PARAMETERS :
 *      なし。
 *
 * DESCRIPTION:
 *      全サウンド発音スロットを停止します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   コマンド実行状態
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
 
 SndRet SND_OffAllSound(void)
 {
    HOST_SET_INIT();                            /* ﾎｽﾄｲﾝﾀﾌｪｰｽｴﾘｱ設定初期処理 */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_COMMAND(COM_OFF_ALL_SOUND);             /* コマンドセット            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_StartSeq()  - シーケンス開始
 *
 * PARAMETERS :
 *      (1) SndSeqNum seq_no        -   <i>     発音管理番号
 *      (2) SndSeqBnkNum seq_bk_no  -   <i>     Sequence bank number
 *      (3) SndSeqSongNum song_no   -   <i>     Sequence song number
 *      (4) SndSeqPri pri_lev       -   <i>     Priorty level
 *
 * DESCRIPTION:
 *      シーケンスを開始します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   コマンド実行状態
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
 
 SndRet SND_StartSeq(SndSeqNum seq_no, SndSeqBnkNum seq_bk_no,
                    SndSeqSongNum song_no, SndSeqPri pri_lev)
 {
    HOST_SET_INIT();                            /* ﾎｽﾄｲﾝﾀﾌｪｰｽｴﾘｱ設定初期処理 */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, seq_no);                         /* パラメータセット          */
    SET_PRM(1, seq_bk_no);                      /* パラメータセット          */
    SET_PRM(2, song_no);                        /* パラメータセット          */
    SET_PRM(3, pri_lev);                        /* パラメータセット          */
    SET_COMMAND(COM_START_SEQ);                 /* コマンドセット            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_StopSeq()   - シーケンス中止
 *
 * PARAMETERS :
 *      (1) SndSeqNum seq_no    - <i>   発音管理番号
 *
 * DESCRIPTION:
 *      シーケンスを中止します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   コマンド実行状態
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
 
 SndRet SND_StopSeq(SndSeqNum seq_no)
 {
    HOST_SET_INIT();                            /* ﾎｽﾄｲﾝﾀﾌｪｰｽｴﾘｱ設定初期処理 */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, seq_no);                         /* パラメータセット          */
    SET_COMMAND(COM_STOP_SEQ);                  /* コマンドセット            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_PauseSeq()  - シーケンス一時中断
 *
 * PARAMETERS :
 *      (1) SndSeqNum seq_no    - <i>   発音管理番号
 *
 * DESCRIPTION:
 *      シーケンスを一時中断します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   コマンド実行状態
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
 
 SndRet SND_PauseSeq(SndSeqNum seq_no)
 {
    HOST_SET_INIT();                            /* ﾎｽﾄｲﾝﾀﾌｪｰｽｴﾘｱ設定初期処理 */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, seq_no);                         /* パラメータセット          */
    SET_COMMAND(COM_PAUSE_SEQ);                 /* コマンドセット            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_ContSeq()   - シーケンス一時中断解除
 *
 * PARAMETERS :
 *      (1) SndSeqNum seq_no    - <i>   発音管理番号
 *
 * DESCRIPTION:
 *      シーケンスの一時中断を解除します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   コマンド実行状態
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
 
 SndRet SND_ContSeq(SndSeqNum seq_no)
 {
    HOST_SET_INIT();                            /* ﾎｽﾄｲﾝﾀﾌｪｰｽｴﾘｱ設定初期処理 */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, seq_no);                         /* パラメータセット          */
    SET_COMMAND(COM_CONT_SEQ);                  /* コマンドセット            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_SetSeqVl()      - シーケンス音量設定
 *
 * PARAMETERS :
 *      (1) SndSeqNum seq_no        -   <i>     発音管理番号
 *      (2) SndSeqVl seq_vl         -   <i>     Sequence Volume
 *      (3) SndFade fade            -   <i>     fade Rate
 *
 * DESCRIPTION:
 *      シーケンスの音量を設定します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   コマンド実行状態
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
 
 SndRet SND_SetSeqVl(SndSeqNum seq_no, SndSeqVl seq_vl, SndFade fade)
 {
    HOST_SET_INIT();                            /* ﾎｽﾄｲﾝﾀﾌｪｰｽｴﾘｱ設定初期処理 */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, seq_no);                         /* パラメータセット          */
    SET_PRM(1, seq_vl);                         /* パラメータセット          */
    SET_PRM(2, fade);                           /* パラメータセット          */
    SET_COMMAND(COM_SET_SEQ_VL);                /* コマンドセット            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_ChgTempo()  - テンポ変更
 *
 * PARAMETERS :
 *      (1) SndSeqNum seq_no    - <i>   発音管理番号
 *      (2) SndTempo tempo      - <i>   Tempo
 *
 * DESCRIPTION:
 *      シーケンスのテンポを変更します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   コマンド実行状態
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
 
 SndRet SND_ChgTempo(SndSeqNum seq_no, SndTempo tempo)
 {
    HOST_SET_INIT();                            /* ﾎｽﾄｲﾝﾀﾌｪｰｽｴﾘｱ設定初期処理 */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, seq_no);                         /* パラメータセット          */
    SET_PRM(2, tempo >> 8);                     /* パラメータセット          */
    SET_PRM(3, tempo);                          /* パラメータセット          */
    SET_COMMAND(COM_CHG_TEMPO);                 /* コマンドセット            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_CtrlDirMidi()   -   MIDIダイレクトコントロール
 *
 * PARAMETERS :
 *      (1) SndSeqNum seq_no    - <i>   発音管理番号
 *      (2) SndSeqPri seq_pri   - <i>   Priorty Level
 *      (3) Uint8 md_com        - <i>   MIDI command
 *      (4) Uint8 ch            - <i>   MIDI channel
 *      (5) Uint8 dt1           - <i>   MIDI data1
 *      (6) Uint8 dt2           - <i>   MIDI data2
 *
 * DESCRIPTION:
 *      MIDIをダイレクトにコントロールします。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   コマンド実行状態
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
 
 SndRet SND_CtrlDirMidi(SndSeqNum seq_no, SndSeqPri seq_pri, Uint8 md_com,
                        Uint8 ch, Uint8 dt1, Uint8 dt2)
 {
    HOST_SET_INIT();                            /* ﾎｽﾄｲﾝﾀﾌｪｰｽｴﾘｱ設定初期処理 */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, (seq_pri << 3) | md_com);        /* パラメータセット          */
    SET_PRM(1, (seq_no << 5) | ch);             /* パラメータセット          */
    SET_PRM(2, dt1);                            /* パラメータセット          */
    SET_PRM(3, dt2);                            /* パラメータセット          */
    SET_COMMAND(COM_CTRL_DIR_MIDI);             /* コマンドセット            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_SetSeqPan()     -   シーケンスPAN設定
 *
 * PARAMETERS :
 *      (1) SndSeqNum seq_no    - <i>   発音管理番号
 *      (2) Uint8 ctrl_sw       - <i>   Control ON/OFF
 *      (3) Uint8 md_pan        - <i>   MIDI PAN data
 *
 * DESCRIPTION:
 *      シーケンスPANを設定します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   コマンド実行状態
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
 
 SndRet SND_SetSeqPan(SndSeqNum seq_no, Uint8 ctrl_sw, Uint8 md_pan)
 {
    HOST_SET_INIT();                            /* ﾎｽﾄｲﾝﾀﾌｪｰｽｴﾘｱ設定初期処理 */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, seq_no);                         /* パラメータセット          */
    SET_PRM(1, (ctrl_sw | md_pan));             /* パラメータセット          */
    SET_COMMAND(COM_CTRL_DIR_MIDI);             /* コマンドセット            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_StartPcm()  - PCM開始
 *
 * PARAMETERS :
 *      (1) SndPcmStartPrm *sprm    - <i>   PCM開始パラメータポインタ
 *      (2) SndPcmChgPrm *cprm      - <i>   PCM変更パラメータポインタ
 *
 * DESCRIPTION:
 *      PCMデータを再生します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   コマンド実行状態
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
 
 SndRet SND_StartPcm(SndPcmStartPrm *sprm, SndPcmChgPrm *cprm)
 {
    HOST_SET_INIT();                            /* ﾎｽﾄｲﾝﾀﾌｪｰｽｴﾘｱ設定初期処理 */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, SND_PRM_MODE(*sprm) | SND_PRM_NUM(*cprm));
    SET_PRM(1, (SND_PRM_LEV(*cprm) << 5) | ChgPan(SND_PRM_PAN(*cprm)));
    SET_PRM(2, SND_PRM_SADR(*sprm) >> 8);
    SET_PRM(3, SND_PRM_SADR(*sprm));
    SET_PRM(4, SND_PRM_SIZE(*sprm) >> 8);
    SET_PRM(5, SND_PRM_SIZE(*sprm));
    SET_PRM(6, SND_PRM_PICH(*cprm) >> 8);
    SET_PRM(7, SND_PRM_PICH(*cprm));
    SET_PRM(8, (SND_R_EFCT_IN(*cprm) << 3) | SND_R_EFCT_LEV(*cprm));
    SET_PRM(9, (SND_L_EFCT_IN(*cprm) << 3) | SND_L_EFCT_LEV(*cprm));
    SET_PRM(11, 0);
    SET_COMMAND(COM_START_PCM);                 /* コマンドセット            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_StopPcm()   - PCM中止
 *
 * PARAMETERS :
 *      (1) SndPcmNum pcm_num       - <i>   再生停止PCMストリーム再生番号
 *
 * DESCRIPTION:
 *      PCMを中止します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   コマンド実行状態
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
 
 SndRet SND_StopPcm(SndPcmNum pcm_num)
 {
    HOST_SET_INIT();                            /* ﾎｽﾄｲﾝﾀﾌｪｰｽｴﾘｱ設定初期処理 */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, pcm_num);                        /* パラメータセット          */
    SET_COMMAND(COM_STOP_PCM);                  /* コマンドセット            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_ChgPcm()    - PCM変更
 *
 * PARAMETERS :
 *      (1) SndPcmChgPrm *cprm      - <i>   PCM変更パラメータポインタ
 *
 * DESCRIPTION:
 *      PCMデータを再生します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   コマンド実行状態
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
 
 SndRet SND_ChgPcm(SndPcmChgPrm *cprm)
 {
    HOST_SET_INIT();                            /* ﾎｽﾄｲﾝﾀﾌｪｰｽｴﾘｱ設定初期処理 */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, SND_PRM_NUM(*cprm));
    SET_PRM(1, (SND_PRM_LEV(*cprm) << 5) | ChgPan(SND_PRM_PAN(*cprm)));
    SET_PRM(2, SND_PRM_PICH(*cprm) >> 8);
    SET_PRM(3, SND_PRM_PICH(*cprm));
    SET_PRM(4, (SND_R_EFCT_IN(*cprm) << 3) | SND_R_EFCT_LEV(*cprm));
    SET_PRM(5, (SND_L_EFCT_IN(*cprm) << 3) | SND_L_EFCT_LEV(*cprm));
    SET_COMMAND(COM_CHG_PCM_PRM);               /* コマンドセット            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_StartVlAnl()    - 音量解析開始
 *
 * PARAMETERS :
 *      なし。
 *
 * DESCRIPTION:
 *      音量解析を行います。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   コマンド実行状態
 *
 * CAVEATS:
 *      周波数帯域別音量解析をする場合は、エフェクト変更にて専用のDSPプログラ
 *      ムを実行してください。
 *
 ******************************************************************************
 */
 
 SndRet SND_StartVlAnl(void)
 {
    HOST_SET_INIT();                            /* ﾎｽﾄｲﾝﾀﾌｪｰｽｴﾘｱ設定初期処理 */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_COMMAND(COM_START_VL_ANL);              /* コマンドセット            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_StopVlAnl()     - 音量解析中止
 *
 * PARAMETERS :
 *      なし。
 *
 * DESCRIPTION:
 *      音量解析を中止します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   コマンド実行状態
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
 
 SndRet SND_StopVlAnl(void)
 {
    HOST_SET_INIT();                            /* ﾎｽﾄｲﾝﾀﾌｪｰｽｴﾘｱ設定初期処理 */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_COMMAND(COM_STOP_VL_ANL);               /* コマンドセット            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_SetCdDaLev()    - CD-DA Level設定
 *
 * PARAMETERS :
 *      (1) SndLev left         - <i>   左出力の音量
 *      (2) SndLev right        - <i>   右出力の音量
 *
 * DESCRIPTION:
 *      現在のステレオ音量を変更します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   コマンド実行状態
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
 
 SndRet SND_SetCdDaLev(SndLev left, SndLev right)
 {
    HOST_SET_INIT();                            /* ﾎｽﾄｲﾝﾀﾌｪｰｽｴﾘｱ設定初期処理 */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, (left) * 2 << 4);                /* パラメータセット          */
    SET_PRM(1, (right) * 2 << 4);               /* パラメータセット          */
    SET_COMMAND(COM_SET_CD_DA_LEV);             /* コマンドセット            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_SetCdDaPan()    - CD-DA Pan設定
 *
 * PARAMETERS :
 *      (1) SndPan  left        - <i>   左出力のPAN
 *      (2) SndPan  right       - <i>   右出力のPAN
 *
 * DESCRIPTION:
 *      現在のステレオPANを変更します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   コマンド実行状態
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
 
 SndRet SND_SetCdDaPan(SndPan left, SndPan right)
 {
    HOST_SET_INIT();                            /* ﾎｽﾄｲﾝﾀﾌｪｰｽｴﾘｱ設定初期処理 */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, ChgPan(left));                   /* パラメータセット          */
    SET_PRM(1, ChgPan(right));                  /* パラメータセット          */
    SET_COMMAND(COM_SET_CD_DA_PAN);             /* コマンドセット            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_GetSeqStat()    - シーケンスステータス取得
 *
 * PARAMETERS :
 *      (1) SndSeqStat *status  - <o>   シーケンスステータスポインタ
 *      (2) SndSeqNum seq_no    - <i>   発音管理番号
 *
 * DESCRIPTION:
 *      シーケンスステータスを取得します。
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
 
 void SND_GetSeqStat(SndSeqStat *status, SndSeqNum seq_no)
 {
    SND_SEQ_STAT_MODE(*status) = (PEEK_W(adr_song_stat + seq_no)) & 0xff00;
    SND_SEQ_STAT_STAT(*status) = (Uint8)(PEEK_W(adr_song_stat + seq_no));
}

/******************************************************************************
 *
 * NAME:    SND_GetSeqPlayPos() - 発音管理番号再生位置取得
 *
 * PARAMETERS :
 *      (1) SndSeqPlayPos *pos  - <o>   発音管理番号再生位置
 *      (2) SndSeqNum seq_no    - <i>   発音管理番号
 *
 * DESCRIPTION:
 *      発音管理番号再生位置を取得します。
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
 
 void SND_GetSeqPlayPos(SndSeqPlayPos *pos, SndSeqNum seq_no)
 {
    *pos = PEEK_W(adr_seq + seq_no);
}

/******************************************************************************
 *
 * NAME:    SND_GetPcmPlayAdr() - PCM実行アドレス取得
 *
 * PARAMETERS :
 *      (1) SndPcmPlayAdr *pcm_adr  - <o>   PCM実行アドレス
 *      (2) SndPcmNum num           - <i>   PCM再生番号
 *
 * DESCRIPTION:
 *      PCM実行アドレスを取得します。
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
 
 void SND_GetPcmPlayAdr(SndPcmPlayAdr *pcm_adr, SndPcmNum num)
 {
    SND_PCM_RADR(*pcm_adr) = (Uint8)(PEEK_W(adr_pcm + num) >> 8);
    SND_PCM_LADR(*pcm_adr) = (Uint8)PEEK_W(adr_pcm + num);
 }

/******************************************************************************
 *
 * NAME:    SND_GetAnlTlVl()    - 解析全体ステレオ音量取得
 *
 * PARAMETERS :
 *      (1) SndCdVlAnl *left    - <o>   左出力の全体解析音量
 *      (2) SndCdVlAnl *right   - <o>   右出力の全体解析音量
 *
 * DESCRIPTION:
 *      全体ステレオ解析音量を取得します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
 
 void SND_GetAnlTlVl(SndCdVlAnl *left, SndCdVlAnl *right)
 {
    *left = (SndCdVlAnl)PEEK_W(adr_tl_vl);
    *right = (SndCdVlAnl)PEEK_W(adr_tl_vl + 1);
 }

/******************************************************************************
 *
 * NAME:    SND_GetAnlHzVl()    - 解析周波数帯域別ステレオ音量取得
 *
 * PARAMETERS :
 *      (1) SndCdHzSrVl *hz_vl - <o>   周波数帯域別ステレオ解析音量
 *
 * DESCRIPTION:
 *      周波数帯域別ステレオ解析音量を取得します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
 
 void SND_GetAnlHzVl(SndCdHzSrVl *hz_vl)
 {
    SND_CD_LHIGH(*hz_vl) = (SndCdVlAnl)PEEK_W(adr_tl_hz_vl + 0);
    SND_CD_RHIGH(*hz_vl) = (SndCdVlAnl)PEEK_W(adr_tl_hz_vl + 1);
    SND_CD_LMID(*hz_vl) = (SndCdVlAnl)PEEK_W(adr_tl_hz_vl + 2);
    SND_CD_RMID(*hz_vl) = (SndCdVlAnl)PEEK_W(adr_tl_hz_vl + 3);
    SND_CD_LLOW(*hz_vl) = (SndCdVlAnl)PEEK_W(adr_tl_hz_vl + 4);
    SND_CD_RLOW(*hz_vl) = (SndCdVlAnl)PEEK_W(adr_tl_hz_vl + 5);
 }

/*****************************************************************************/
/**** 内部提供関数 ***********************************************************/
/*****************************************************************************/
/******************************************************************************
 *
 * NAME:    DmaClrZero()    - DMAによるメモリのゼロクリア
 *
 * PARAMETERS :
 *      (1) void *dst       - <i>   ディスティネーションアドレス
 *      (2) Uint32 cnt      - <i>   クリアバイト数
 *
 * DESCRIPTION:
 *      DMAを使用してメモリをゼロクリアします。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */

static void DmaClrZero(void *dst, Uint32 cnt)
{
    memset(dst, 0x00, cnt);
}

/******************************************************************************
 *
 * NAME:    GetSndMapInfo()     - サウンドエリアマップ情報取得
 *
 * PARAMETERS :
 *      (1) void **adr          - <o>   転送先アドレス
 *      (2) Uint32 **ladr        - <o>   転送済みビットアドレス
 *      (3) Uint16 data_kind    - <i>   データ種別
 *      (4) Uint16 data_no      - <i>   データ番号
 *
 * DESCRIPTION:
 *      サウンドエリアマップの情報を取得します。
 *
 * PRECONDITIONS:
 *      なし。
 *
 * POSTCONDITIONS:
 *      なし
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */

static void GetSndMapInfo(void **adr, Uint32 **ladr, Uint16 data_kind, Uint16 data_no)
{
    Uint32 i = 0;
    Uint32 map0;

    map0 = PEEK_L(adr_snd_area_crnt + ARA_MAP_0);

    for(i = 1; (map0 & M_END_MARK) != M_END_MARK; i++){
        if((((map0 & M_DATA_ID) >> B_DATA_ID) == (Uint32)data_kind) &&
           (((map0 & M_ID_NUM) >> B_ID_NUM) == (Uint32)data_no)){
            *adr = (void *)(ADR_SND_MEM +
                       ((map0 & M_START_ADR) >> B_START_ADR)
              );
            *ladr = (Uint32 *)(adr_snd_area_crnt + ARA_MAP_SIZE * (i - 1) +
                     ARA_MAP_4);
            break;
        }
        map0 = PEEK_L(adr_snd_area_crnt + ARA_MAP_SIZE * i + ARA_MAP_0);
    }
}
/******************************************************************************
 *
 * NAME:    ChgPan()            - PANデータ変換
 *
 * PARAMETERS :
 *      (1) SndPan pan          - <i>   PAN
 *
 * DESCRIPTION:
 *      登録用のPANに変換します
 *
 * PRECONDITIONS:
 *      (1) Uint16              - <o>   変換後PAN
 *
 * POSTCONDITIONS:
 *      なし
 *
 * CAVEATS:
 *      なし。
 *
 ******************************************************************************
 */
static Uint16 ChgPan(SndPan pan)
{
    return(((pan) < 0) ? (~(pan) + 0x10 + 1) : (pan));
}

/******************************************************************************
 *
 * NAME:    CopyMem()           - メモリコピ－
 *
 * PARAMETERS :
 *
 * DESCRIPTION:
 *
 * PRECONDITIONS:
 *
 * POSTCONDITIONS:
 *
 * CAVEATS:
 *
 ******************************************************************************
 */

static void CopyMem(void *dst, void *src, Uint32 cnt)
{
#ifndef _DMA_SCU
    memcpy(dst, src, cnt);
#else
                                                /*****************************/
    DMA_ScuMemCopy(dst, src, cnt);
    while(DMA_SCU_END != DMA_ScuResult());
#endif /* _DMA_SCU */
}

/******************************************************************************
 *
 * NAME:    GetComBlockAdr()            - コマンドブロック
 *
 * PARAMETERS :
 *
 * DESCRIPTION:
 *
 * PRECONDITIONS:
 *
 * POSTCONDITIONS:
 *      (1) Uint8               - <ret> コマンドブロックの空き状態
 *
 * CAVEATS:
 *
 ******************************************************************************
 */

static Uint8 GetComBlockAdr(void)
{
    if(*NOW_ADR_COM_DATA){              /* 以前のﾌﾞﾛｯｸが引き取り済みでないか?*/
        /* 次コマンドブロックアドレス設定処理 ********************************/
        if(NOW_ADR_COM_DATA >= (MAX_ADR_COM_DATA - SIZE_COM_BLOCK)){
                                                    /* 最大値か?            */
            return OFF;                             /* ﾌﾞﾛｯｸ空き無し      */
        }else{
            adr_com_block += SIZE_COM_BLOCK;        /* 現在ｺﾏﾝﾄﾞﾌﾞﾛｯｸｶｳﾝﾄｱｯﾌﾟ*/
            while(NOW_ADR_COM_DATA < (MAX_ADR_COM_DATA - SIZE_COM_BLOCK)){
                if(*NOW_ADR_COM_DATA){
                    adr_com_block += SIZE_COM_BLOCK;
                }else{
                    return ON;                      /* ﾌﾞﾛｯｸ空き有り         */
                }
            }
            return OFF;                             /* ﾌﾞﾛｯｸ空き無し         */
        }
    }else{
        adr_com_block = adr_host_int_work;  /* ﾌﾞﾛｯｸの先頭へ              */
        while(NOW_ADR_COM_DATA < (MAX_ADR_COM_DATA - SIZE_COM_BLOCK)){
            if(*NOW_ADR_COM_DATA){
                adr_com_block += SIZE_COM_BLOCK;
            }else{
                return ON;                          /* ﾌﾞﾛｯｸ空き有り         */
            }
        }
        return OFF;                                 /* ﾌﾞﾛｯｸ空き無し         */
    }
}
