/*-----------------------------------------------------------------------------
 *  FILE: smpsnd0.c
 *
 *  Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *
 *      �T�E���h�T���v���v���O����
 *
 *  DESCRIPTION:
 *
 *      �V�[�P���X�f�[�^��炵�܂��B
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
 * NAME:    main()      - ���C��
 *
 * PARAMETERS :
 *      �Ȃ�
 *
 * DESCRIPTION:
 *      ���C��
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      �Ȃ�
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */
void tubo(void);
void main(void)
{

    SndIniDt snd_init;                                  /* �V�X�e���N���f�[�^*/
    SndSeqStat status;                              /* �V�[�P���X�X�e�[�^�X  */
    Uint32 i;

    /** BEGIN ****************************************************************/
    /*
     *  process 1   �i�ePAD��񏉊����j
     */

    SND_INI_PRG_ADR(snd_init) = (Uint16 *)0x6080000;
    SND_INI_PRG_SZ(snd_init) = (Uint16 )0x6000;
    SND_INI_ARA_ADR(snd_init) = (Uint16 *)0x6019800;
    SND_INI_ARA_SZ(snd_init) = (Uint16)0x004a;
    SND_Init(&snd_init);						/* �T�E���h�V�X�e���N��		 */
    SND_ChgMap(0);	/* �T�E���h�G���A�}�b�v�ύX	 */
  	SND_MoveData((Uint16 *)0x6010000,			/* ���F�f�[�^�]��			 */
				 (Uint32)0x5000,				/* 32bit�ײ���				 */
				 SND_KD_TONE,
				 0);
	SND_MoveData((Uint16 *)0x6015000,			/* �V�[�P���X�f�[�^�]��		 */
				 (Uint32)0x400, 				/* 32bit�ײ���				 */
				 SND_KD_SEQ,
				 0);
	SND_MoveData((Uint16 *)0x6015400,			/* �V�[�P���X�f�[�^�]��		 */
				 (Uint32)0x2700, 				/* 32bit�ײ���				 */
				 SND_KD_SEQ,
				 1);
	SND_MoveData((Uint16 *)0x6018000,			/* �V�[�P���X�f�[�^�]��		 */
				 (Uint32)0x600, 				/* 32bit�ײ���				 */
				 SND_KD_DSP_PRG,
				 0);
	SND_MoveData((Uint16 *)0x6018600,			/* �V�[�P���X�f�[�^�]��		 */
				 (Uint32)0x600, 				/* 32bit�ײ���				 */
				 SND_KD_DSP_PRG,
				 1);
	SND_MoveData((Uint16 *)0x6018c00,			/* �V�[�P���X�f�[�^�]��		 */
				 (Uint32)0x600, 				/* 32bit�ײ���				 */
				 SND_KD_DSP_PRG,
				 2);
	SND_MoveData((Uint16 *)0x6019200,			/* �V�[�P���X�f�[�^�]��		 */
				 (Uint32)0x600, 				/* 32bit�ײ���				 */
				 SND_KD_DSP_PRG,
				 3);
	SND_StartSeq(1, 0, 2, 1);					/* �V�[�P���X�J�n			 */
    while(1);
}
