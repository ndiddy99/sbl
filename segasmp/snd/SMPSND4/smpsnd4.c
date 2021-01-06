/*-----------------------------------------------------------------------------
 *  FILE: smpsnd4.c
 *
 *  Copyright(c) 1994 SEGA
 *
 *  PURPOSE:
 *
 *      �T�E���h�T���v���v���O����
 *
 *  DESCRIPTION:
 *
 *      PCM�̘A���Đ����s���܂��B(���荞�ݎg�p)
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *
 *  CAVEATS:
 *
 *      SCU2.1�ȍ~�œ��삵�܂��B
 *
 *  AUTHOR(S)
 *
 *      1994-05-19  N.T Ver.0.90
 *
 *  MOD HISTORY:
 *
 *      1994-10-15  N.T Ver.1.03
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

static Uint16 *move_pcm_adr;                    /* PCM�]���A�h���X           */
static Uint16 *l_move_pcm_adr;                  /* PCM�]���A�h���X           */
static Uint8 move_bnk;                          /* PCM�]����o���N           */
static Uint8 l_move_bnk;                        /* PCM�]����o���N           */
static Uint32 cnt = 0;

/* �����ݗD�惌�x���ύX�e�[�u�� */
/****************************************************************************/
/* (����)�@�ȉ��̊��荞�ݗD�惌�x���e�[�u����,�T�E���h���C�u�����Ŏg�p���Ă�*/
/*         DMA�����荞�ݏ������Ŏ��s�ł���悤�ɊȈՓI�ɍ쐬�������̂ł�.�� */
/*         ���ăQ�[���ł�,���̃Q�[���ɂ������e�[�u�����������쐬���Ă�������*/
/*         �쐬���@�̓V�X�e�����C�u�����̃f�B�X�N���h�L�������g�ɋL�q���Ă� */
/*         ��܂�.                                                          */
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
void PcmIntr(void);
void r_PcmIntr(void);
void l_PcmIntr(void);
void main(void)
{

    SndIniDt snd_init;                                  /* �V�X�e���N���f�[�^*/
    Uint32 i;
    SndPcmStartPrm pcm_start;
    SndPcmChgPrm pcm_chg;
    SndPcmPlayAdr pcm_adr;
    Uint8 before_pcm_adr = 0;

    /** BEGIN ****************************************************************/
    /*
     *  process 1   �i�ePAD��񏉊����j
     */

    SYS_CHGUIPR(pri_tbl);                   /* �����ݗD�惌�x���̕ύX        */

    move_pcm_adr = (Uint16 *)0x6010000;     /* �E�`���l���]����              */
    l_move_pcm_adr = (Uint16 *)0x6045000;   /* ���`���l���]����              */

	set_imask(0);
    INT_ChgMsk(INT_MSK_NULL, INT_MSK_SND);      /* �T�E���h���荞�݋���      */
    INT_SetScuFunc(INT_SCU_SND, PcmIntr);          /* ���荞�ݏ����o�^     */       INT_ChgMsk(INT_MSK_SND,INT_MSK_NULL);       /* �T�E���h���荞�݋���      */

    SND_INI_PRG_ADR(snd_init) = (Uint16 *)0x6080000;
    SND_INI_PRG_SZ(snd_init) = (Uint16 )0x6000;
    SND_INI_ARA_ADR(snd_init) = (Uint16 *)0x607c000;
    SND_INI_ARA_SZ(snd_init) = (Uint16)0x0014;
    SND_Init(&snd_init);						/* �T�E���h�V�X�e���N��		 */
    SND_ChgMap(0);	/* �T�E���h�G���A�}�b�v�ύX	 */

    SND_RESET_INT();                            /* �����݃X�e�[�^�X���Z�b�g  */
    SND_SET_ENA_INT(SND_INT_PCM_ADR);           /* PCM�����݋���(�T�E���h)   */

    move_bnk = 0;
    l_move_bnk = 0;

    /* RIGHT�`���l�� */
    r_PcmIntr();                       /* �E�`���l���]�� */
    r_PcmIntr();                       /* �E�`���l���]�� */
    /* LEFT�`���l�� */
    l_PcmIntr();                       /* ���`���l���]�� */
    l_PcmIntr();                       /* ���`���l���]�� */

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

    SND_StartPcm(&pcm_start, &pcm_chg);	    /* PCM�J�n			 */
	while(1){
        if(l_move_pcm_adr > (Uint16 *)0x6070000 ||
             move_pcm_adr > (Uint16 *)0x6070000){
            SND_StopPcm(2);
        }
    }
}
/*****************************************************************************/
/* �T�E���h���荞�ݏ��� */
/*****************************************************************************/
void PcmIntr(void)
{
    SND_RESET_INT();                            /* �����݃X�e�[�^�X���Z�b�g  */
    if(SND_GET_FCT_INT() == SND_FCT_PCM_ADR){   /* PCM adress�X�V�����݂��H  */
        if(SND_GET_INT_STAT() == 0x4){
            r_PcmIntr();
            l_PcmIntr();
        }
    }
}
/*****************************************************************************/
/* �E�`���l���]�� */
/*****************************************************************************/
void r_PcmIntr(void)
{
  	    SND_MoveData(move_pcm_adr,	            /* PCM�f�[�^�]��			 */
				 (Uint32)0x2000,				/* 32bit�ײ���				 */
				 SND_KD_TONE,
				 move_bnk);
        move_bnk = ~move_bnk & 1;
        move_pcm_adr += 0x1000;
}
/*****************************************************************************/
/* ���`���l���]�� */
/*****************************************************************************/
void l_PcmIntr(void)
{
  	    SND_MoveData(l_move_pcm_adr,	        /* PCM�f�[�^�]��			 */
				 (Uint32)0x2000,				/* 32bit�ײ���				 */
				 SND_KD_TONE,
				 l_move_bnk + 2);
        l_move_bnk = ~l_move_bnk & 1;
        l_move_pcm_adr += 0x1000;
}
