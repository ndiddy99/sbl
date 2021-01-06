/*-----------------------------------------------------------------------------
 *  FILE: smpsnd5.c
 *
 *  Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *
 *      サウンドサンプルプログラム
 *
 *  DESCRIPTION:
 *
 *      PCMの連続再生とCD-DAの再生を行います。(割り込み使用)
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *
 *  CAVEATS:
 *
 *      ●SCU2.1以降で動作します。
 *      ●smpsnd4を改造したものです．追加したのは，CD-DA再生部分です。
 *        改造部分の開始、終了行に「 CD-DAのための処理 (開始)」を挿入して
 *        います。
 *
 *  AUTHOR(S)
 *
 *      1994-09-23  N.T Ver.1.00
 *
 *  MOD HISTORY:
 *
 *      1994-10-15  N.T Ver.1.01
 *
 *-----------------------------------------------------------------------------
 */

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
#include "sega_xpt.h"
#include <machine.h>

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include "sega_snd.h"
#include "sega_sys.h"
#include "sega_int.h"
#include "sega_cdc.h"

/*
 * GLOBAL DECLARATIONS
 */

/*
 * LOCAL DEFINES/MACROS
 */

/*
 * STATIC DECLARATIONS
 */

/*
 * STATIC FUNCTION PROTOTYPE DECLARATIONS
 */

static Uint16 *move_pcm_adr;                    /* PCM転送アドレス           */
static Uint16 *l_move_pcm_adr;                  /* PCM転送アドレス           */
static Uint8 move_bnk;                          /* PCM転送先バンク           */
static Uint8 l_move_bnk;                        /* PCM転送先バンク           */
static Uint32 cnt = 0;

/* 割込み優先レベル変更テーブル */
/****************************************************************************/
/* (注意)　以下の割り込み優先レベルテーブルは,サウンドライブラリで使用してい*/
/*         DMAを割り込み処理内で実行できるように簡易的に作成したものです.よ */
/*         ってゲームでは,そのゲームにあったテーブルを検討し作成してください*/
/*         作成方法はシステムライブラリのディスク内ドキュメントに記述してあ */
/*         ります.                                                          */
/****************************************************************************/
const Uint32 pri_tbl[32] = {
                                0x0000E17C,
                                0x0000FF7D,
                                0x0000E178,
                                0x0000E170,
                                0x0000E160,
                                0x0000E140,
                                0x0000C000,
                                0x0000FF7F,
                                0x0000C000,
                                0x0000E17D,
                                0x0000E17D,
                                0x0000E17D,
                                0x0000E17D,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000,
                                0x0000C000};

/******************************************************************************
 *
 * NAME:    main()      - メイン
 *
 * PARAMETERS :
 *      なし
 *
 * DESCRIPTION:
 *      メイン
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
void PcmIntr(void);
void r_PcmIntr(void);
void l_PcmIntr(void);
void main(void)
{

    SndIniDt snd_init;                                  /* システム起動データ*/
    Uint32 i;
    SndPcmStartPrm pcm_start;
    SndPcmChgPrm pcm_chg;
    SndPcmPlayAdr pcm_adr;
    Uint8 before_pcm_adr = 0;

/* CD-DAのための処理 (開始)*/
	CdcPly ply ;
    /** BEGIN ****************************************************************/
	CDC_CdInit(0x00,0x00,0x05,0x0f) ;               /* CDの初期化            */
/* CD-DAのための処理 (終了)*/

    SYS_CHGUIPR(pri_tbl);                   /* 割込み優先レベルの変更        */

    /*
     *  process 1   （各PAD情報初期化）
     */
    
    move_pcm_adr = (Uint16 *)0x6010000;     /* 右チャネル転送元              */
    l_move_pcm_adr = (Uint16 *)0x6045000;   /* 左チャネル転送元              */

	set_imask(0);
    INT_ChgMsk(INT_MSK_NULL, INT_MSK_SND);      /* サウンド割り込み許可      */
    INT_SetScuFunc(INT_SCU_SND, PcmIntr);          /* 割り込み処理登録     */       INT_ChgMsk(INT_MSK_SND,INT_MSK_NULL);       /* サウンド割り込み許可      */

    SND_INI_PRG_ADR(snd_init) = (Uint16 *)0x6080000;
    SND_INI_PRG_SZ(snd_init) = (Uint16 )0x6000;
    SND_INI_ARA_ADR(snd_init) = (Uint16 *)0x607c000;
    SND_INI_ARA_SZ(snd_init) = (Uint16)0x0014;
    SND_Init(&snd_init);						/* サウンドシステム起動		 */
    SND_ChgMap(0);	/* サウンドエリアマップ変更	 */

/* CD-DAのための処理 (開始)*/
    SND_SetCdDaLev(7,7);                        /* CD-DAレベルの設定         */
    SND_SetCdDaPan(0,0);                        /* CD-DAのPAN設定            */
    SND_SetTlVl(15);                            /* Total Volumeの設定        */

	CDC_PLY_STYPE(&ply) = CDC_PTYPE_TNO ;       /* CDC_CdPlayのパラメータ設定*/
	CDC_PLY_STNO(&ply)  = 3 ;
	CDC_PLY_SIDX(&ply) = 1 ;
	CDC_PLY_ETYPE(&ply) = CDC_PTYPE_TNO ;
	CDC_PLY_ETNO(&ply)  = 4 ;
	CDC_PLY_EIDX(&ply) = 99 ;
	CDC_PLY_PMODE(&ply) = CDC_PM_DFL+ 1 ;
    CDC_CdPlay(&ply) ;                          /* CD-DAの再生実行           */
/* CD-DAのための処理 (終了)*/

    SND_RESET_INT();                            /* 割込みステータスリセット  */
    SND_SET_ENA_INT(SND_INT_PCM_ADR);           /* PCM割込み許可(サウンド)   */

    move_bnk = 0;
    l_move_bnk = 0;

    /* RIGHTチャネル */
    r_PcmIntr();                       /* 右チャネル転送 */
    r_PcmIntr();                       /* 右チャネル転送 */
    /* LEFTチャネル */
    l_PcmIntr();                       /* 左チャネル転送 */
    l_PcmIntr();                       /* 左チャネル転送 */

    SND_PRM_MODE(pcm_start) = SND_MD_STEREO | SND_MD_16;
    SND_PRM_SADR(pcm_start) = 0x1000;
    SND_PRM_SIZE(pcm_start) = 0x2000;
    SND_PRM_NUM(pcm_chg) = 2;
    SND_PRM_LEV(pcm_chg) = 7;
    SND_PRM_PAN(pcm_chg) = 0;
    SND_PRM_PICH(pcm_chg) = 0;
    SND_L_EFCT_IN(pcm_chg) = 0;
    SND_L_EFCT_LEV(pcm_chg) = 7;
    SND_R_EFCT_IN(pcm_chg) = 0;
    SND_R_EFCT_LEV(pcm_chg) =  7;

    SND_StartPcm(&pcm_start, &pcm_chg);	    /* PCM開始			 */
	while(1){
        if(l_move_pcm_adr > (Uint16 *)0x6070000 ||
             move_pcm_adr > (Uint16 *)0x6070000){
            SND_StopPcm(2);
            break;
        }
    }
}
/*****************************************************************************/
/* サウンド割り込み処理 */
/*****************************************************************************/
void PcmIntr(void)
{
    SND_RESET_INT();                            /* 割込みステータスリセット  */
    if(SND_GET_FCT_INT() == SND_FCT_PCM_ADR){   /* PCM adress更新割込みか？  */
        if(SND_GET_INT_STAT() == 0x4){
            r_PcmIntr();
            l_PcmIntr();
        }
    }
}
/*****************************************************************************/
/* 右チャネル転送 */
/*****************************************************************************/
void r_PcmIntr(void)
{
  	    SND_MoveData(move_pcm_adr,	            /* PCMデータ転送			 */
				 (Uint32)0x2000,				/* 32bitｱﾗｲﾒﾝﾄ				 */
				 SND_KD_TONE,
				 move_bnk);
        move_bnk = ~move_bnk & 1;
        move_pcm_adr += 0x1000;
}
/*****************************************************************************/
/* 左チャネル転送 */
/*****************************************************************************/
void l_PcmIntr(void)
{
  	    SND_MoveData(l_move_pcm_adr,	        /* PCMデータ転送			 */
				 (Uint32)0x2000,				/* 32bitｱﾗｲﾒﾝﾄ				 */
				 SND_KD_TONE,
				 l_move_bnk + 2);
        l_move_bnk = ~l_move_bnk & 1;
        l_move_pcm_adr += 0x1000;
}
