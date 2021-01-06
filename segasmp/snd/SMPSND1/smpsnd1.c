/*-----------------------------------------------------------------------------
 *  FILE: smpsnd0.c
 *
 *  Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *
 *      サウンドサンプルプログラム
 *
 *  DESCRIPTION:
 *
 *      シーケンスデータを鳴らします。
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
 *
 *  MOD HISTORY:
 *
 *      1994-08-30  N.T Ver.1.01
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
#include "sega_snd.h"

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
void tubo(void);
void main(void)
{

    SndIniDt snd_init;                                  /* システム起動データ*/
    SndSeqStat status;                              /* シーケンスステータス  */
    Uint32 i;

    /** BEGIN ****************************************************************/
    /*
     *  process 1   （各PAD情報初期化）
     */

    SND_INI_PRG_ADR(snd_init) = (Uint16 *)0x6080000;
    SND_INI_PRG_SZ(snd_init) = (Uint16 )0x6000;
    SND_INI_ARA_ADR(snd_init) = (Uint16 *)0x6019800;
    SND_INI_ARA_SZ(snd_init) = (Uint16)0x004a;
    SND_Init(&snd_init);						/* サウンドシステム起動		 */
    SND_ChgMap(0);	/* サウンドエリアマップ変更	 */
  	SND_MoveData((Uint16 *)0x6010000,			/* 音色データ転送			 */
				 (Uint32)0x5000,				/* 32bitｱﾗｲﾒﾝﾄ				 */
				 SND_KD_TONE,
				 0);
	SND_MoveData((Uint16 *)0x6015000,			/* シーケンスデータ転送		 */
				 (Uint32)0x400, 				/* 32bitｱﾗｲﾒﾝﾄ				 */
				 SND_KD_SEQ,
				 0);
	SND_MoveData((Uint16 *)0x6015400,			/* シーケンスデータ転送		 */
				 (Uint32)0x2700, 				/* 32bitｱﾗｲﾒﾝﾄ				 */
				 SND_KD_SEQ,
				 1);
	SND_MoveData((Uint16 *)0x6018000,			/* シーケンスデータ転送		 */
				 (Uint32)0x600, 				/* 32bitｱﾗｲﾒﾝﾄ				 */
				 SND_KD_DSP_PRG,
				 0);
	SND_MoveData((Uint16 *)0x6018600,			/* シーケンスデータ転送		 */
				 (Uint32)0x600, 				/* 32bitｱﾗｲﾒﾝﾄ				 */
				 SND_KD_DSP_PRG,
				 1);
	SND_MoveData((Uint16 *)0x6018c00,			/* シーケンスデータ転送		 */
				 (Uint32)0x600, 				/* 32bitｱﾗｲﾒﾝﾄ				 */
				 SND_KD_DSP_PRG,
				 2);
	SND_MoveData((Uint16 *)0x6019200,			/* シーケンスデータ転送		 */
				 (Uint32)0x600, 				/* 32bitｱﾗｲﾒﾝﾄ				 */
				 SND_KD_DSP_PRG,
				 3);
	SND_StartSeq(1, 0, 2, 1);					/* シーケンス開始			 */
    while(1);
}
