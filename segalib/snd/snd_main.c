/*-----------------------------------------------------------------------------
 *  FILE: snd_main.c
 *
 *      Copyright (c) by SEGA.
 *
 *  PURPOSE:
 *
 *      �T�E���hI/F���C�u�����\�[�X�t�@�C���B
 *
 *  DESCRIPTION:
 *
 *      �T�E���h�h���C�o�Ƃ̃C���^�t�F�[�X����������B
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *
 *  CAVEATS:
 *
 *      �Ȃ��B
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
Uint8 *snd_adr_sys_int_work;                 /*���Ѳ��̪��ܰ��擪���ڽ�i�[*/
Uint32 snd_msk_work_work;                    /* sound priority msk        */

/*
 * LOCAL DEFINES/MACROS
 */
#define _DMA_SCU                                /* SCU DMA ���g�p����        */
/* �萔 */
/**** �A�h���X ***************************************************************/
#define ADR_SCSP_REG    ((volatile Uint8 *)0x25b00400)   /* SCSP���ʐ��䃌�W�X�^      */
#define ADR_SND_MEM     ((volatile Uint8 *)0x25a00000)   /* �T�E���h�������擪�A�h���X*/
#define ADR_SND_VECTOR  ((volatile Uint8 *)0x25a00000)   /* �T�E���h�x�N�^�A�h���X    */
#define ADR_SYS_TBL     (ADR_SND_MEM + 0x400)   /* ���Ѳ��̪���̈�           */

/**** �V�X�e���C���^�t�F�[�X�e�[�u���I�t�Z�b�g *******************************/
#define ADR_SYS_INFO    (0x00)                  /* ���я��ð��ٱ��ڽ        */
#define ADR_HOST_INT    (0x04)                  /* νĲ��̪��ܰ����ڽ        */
#define ADR_ARA_CRNT    (0x08)                /* ����޴رϯ��CRNTܰ��擪���ڽ*/
#define ADR_SYS_INT_WORK    (0x12)            /* ���Ѳ��̪��ܰ��擪���ڽ     */
#define ADR_HARD_CHK_STAT   (0x18)            /* ʰ���������ݽð���i�[ܰ�    */

/**** �V�X�e�����e�[�u���I�t�Z�b�g *****************************************/
#define ADR_PRG_ADR     (0x00)                  /* 68K��۸��ї̈�擪���ڽ   */
#define ADR_PRG_SIZE    (0x04)                  /* 68K��۸��ї̈滲��        */
#define ADR_ARA_ADR     (0x08)                  /* ����޴رϯ�ߗ̈�擪���ڽ */

/**** �z�X�g�C���^�t�F�[�X���[�N�I�t�Z�b�g ***********************************/
#define ADR_COM_DATA    (0x00)                  /* �R�}���h                  */
#define ADR_PRM_DATA    (0x02)                  /* �p�����[�^                */
#define ADR_SONG_STAT   (0x80)                  /* song status               */
#define ADR_TL_VL       (0x90)                  /* Total volume              */
#define ADR_TL_HZ_VL    (0x94)                  /* ���g���ш��Volume        */
#define ADR_PCM         (0xa0)                  /* PCM                       */
#define ADR_SEQ         (0xb0)                  /* Sequence                  */
/**** �z�X�g�C���^�t�F�[�X���[�N�萔 *****************************************/
#define SIZE_COM_BLOCK      (0x10)              /* �������ۯ�����          */
#define MAX_NUM_COM_BLOCK   8                   /* �������ۯ���              */

/**** �T�E���h�N�� ***********************************************************/
#define SCSP_REG_SET    0x0200                  /* SCSP���W�X�^�ݒ�l        */
#define MEM_CLR_SIZE    0xb000                  /* �������ظر����           */

/**** �T�E���h�G���A�}�b�v���(�ݸ�ܰ�ޒP��) *********************************/
#define ARA_MAP_SIZE        0x2                 /* ����޴رϯ�߻���          */
/******** �I�t�Z�b�g�l(�ݸ�ܰ�ޒP��) *****************************************/
#define ARA_MAP_0           0x0         /* �ް����,�ް��ԍ�,�ر�J�n���ڽ    */
#define ARA_MAP_4           0x1         /* �]���ς��ޯ�,�ر����              */
/******** �r�b�g�ʒu *********************************************************/
#define B_END_MARK          31                  /* �ް��I���ޯ�              */
#define B_DATA_ID           28                  /* �ް���ʁ@�@�@�@�@�@�@    */
#define B_ID_NUM            24                  /* �ް��ԍ�                  */
#define B_START_ADR         0                   /* ���ı��ڽ                 */
#define B_LOAD_MARK         31                  /* �]���ς��ޯ�              */
#define B_AREA_SIZE         0                   /* �ر����                   */
/******** �}�X�N�r�b�g *******************************************************/
#define M_END_MARK         (0x1  << B_END_MARK) /* �ް��I���ޯ�              */
#define M_DATA_ID          (0x7  << B_DATA_ID)  /* �ް���ʁ@�@�@�@�@�@�@    */
#define M_ID_NUM           (0xf  << B_ID_NUM)   /* �ް��ԍ�                  */
#define M_START_ADR        (0xfffff << B_START_ADR) /* ���ı��ڽ             */
#define M_LOAD_MARK        (0x1  << B_LOAD_MARK)/* �]���ς��ޯ�              */
#define M_AREA_SIZE        (0xfffff << B_AREA_SIZE) /* �ر����               */

/**** �R�}���h ***************************************************************/
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

/* �����}�N�� */
/**** ���������C�g ***********************************************************/
#define POKE_B(adr, data)   (*((volatile Uint8 *)(adr)) = ((Uint8)(data)))   /* �޲�  */
#define POKE_W(adr, data)   (*((volatile Uint16 *)(adr)) = ((Uint16)(data))) /* ܰ��  */
#define POKE_L(adr, data)   (*((volatile Uint32 *)(adr)) = ((Uint32)(data))) /* �ݸ�  */
/**** ���������[�h ***********************************************************/
#define PEEK_B(adr)         (*((volatile Uint8 *)(adr)))                     /* �޲�  */
#define PEEK_W(adr)         (*((volatile Uint16 *)(adr)))                    /* ܰ��  */
#define PEEK_L(adr)         (*((volatile Uint32 *)(adr)))                    /* �ݸ�  */
/**** �ݸ�ܰ���޳���ؕϊ�(Middle SCU�̐������ *******************************/
#define CHG_LONG(x)    (((x) * 2) + (0x4 - ( ((x) * 2) % 4) ))
/**** �z�X�g�C���^�t�F�[�X���[�N�}�N�� ***************************************/
#define MAX_ADR_COM_DATA                        /* �ő������ް����ڽ     */\
    (adr_host_int_work + ADR_COM_DATA + (SIZE_COM_BLOCK * MAX_NUM_COM_BLOCK))
#define NOW_ADR_COM_DATA                        /* ���ݺ�����ް����ڽ     */\
    (adr_com_block + ADR_COM_DATA)
/**** �T�E���h�h���C�o�R�}���h�u���b�N�ݒ�֐����������}�N�� *****************/
#define HOST_SET_INIT()\
    Uint32 msk;\
    do{\
        msk = get_imask();\
        set_imask(0xf);\
    }while(FALSE)
/**** �T�E���h�h���C�o�R�}���h�u���b�N�ݒ�֐����^�[���}�N�� *****************/
#define HOST_SET_RETURN(ret)\
    do{\
        set_imask(msk);\
        return (ret);\
    }while(FALSE)

/******************************************************************************
 *
 * NAME:    SET_COMMAND()       - �R�}���h�Z�b�g
 *
 * PARAMETERS :
 *      (1) Uint16 set_com      - <i>   �R�}���h
 *
 * DESCRIPTION:
 *      �R�}���h���C���^�t�F�[�X�̈�ɃZ�b�g���܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      �Ȃ�
 *
 * CAVEATS:
 *      ����ނւ�WRITE��16bit�ōs�Ȃ��܂��B16bit���̏��8bit�ֺ���ނ�Ă��܂�
 *
 ******************************************************************************
 */

#define SET_COMMAND(set_com)\
(POKE_W((adr_com_block + ADR_COM_DATA), (Uint16)(set_com) << 8)) /* �R�}���h�Z�b�g   */

/******************************************************************************
 *
 * NAME:    SET_PRM()           - �p�����[�^�Z�b�g
 *
 * PARAMETERS :
 *      (1) Uint32 no           - <i>   �p�����[�^�ԍ�
 *      (2) Uint8 set_prm       - <i>   �p�����[�^
 *
 * DESCRIPTION:
 *      �p�����[�^���C���^�t�F�[�X�̈�ɃZ�b�g���܂��B
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

#define SET_PRM(no, set_prm)\
(POKE_B(adr_com_block + ADR_PRM_DATA + (no), (set_prm))) /* ���Ұ��Z�b�g      */

/*
 * STATIC DECLARATIONS
 */ 
static Uint8 *adr_sys_info_tbl;                 /* ���я��ð��ٱ��ڽ�i�[    */
static Uint8 *adr_host_int_work;                /* νĲ��̪��ܰ��擪���ڽ�i�[*/
static Uint32 *adr_snd_area_crnt;             /* ����޴رϯ��CRNTܰ��擪���ڽ*/
static Uint16 *adr_song_stat;                   /* song status               */
static Uint16 *adr_tl_vl;                       /* Total volume              */
static Uint16 *adr_tl_hz_vl;                    /* ���g���ш��Volume        */
static Uint16 *adr_pcm;                         /* PCM                       */
static Uint16 *adr_seq;                         /* Sequence                  */
static Uint8  *adr_com_block;                   /* ���ݏ������ݺ������ۯ�      */

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
 * NAME:    SND_Init()  - �T�E���h�V�X�e���N��
 *
 * PARAMETERS :
 *      (1) SndIniDt *sys_ini       - <i>   �V�X�e���N���p�f�[�^
 *
 * DESCRIPTION:
 *      �V�X�e���N���p�f�[�^�ɏ]���āA�T�E���h�V�X�e�����N������B
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

void SND_Init(SndIniDt *sys_ini)
{
    /** BEGIN ****************************************************************/
#ifdef _DMA_SCU
    DMA_ScuInit();                              /* DMA SCU����������         */
#endif /* _DMA_SCU */

    PER_SMPC_SND_OFF();                         /* �T�E���hOFF               */
    POKE_W(ADR_SCSP_REG, SCSP_REG_SET); 
                                                /* SCSP����ڼ޽��ݒ�         */
    DmaClrZero((void *)ADR_SND_MEM, MEM_CLR_SIZE);      /* DMA�������[���N���A       */
    CopyMem((void *)ADR_SND_VECTOR,
                   (void *)(SND_INI_PRG_ADR(*sys_ini)),
                   SND_INI_PRG_SZ(*sys_ini));   /* 68K��۸��ѓ]��            */

    adr_sys_info_tbl = (Uint8 *)(ADR_SND_MEM + PEEK_L(ADR_SYS_TBL +
                                 ADR_SYS_INFO));
                                                /* ���я��ð��ٱ��ڽ�擾    */
    adr_host_int_work = (Uint8 *)(ADR_SND_MEM + PEEK_L(ADR_SYS_TBL +
                                  ADR_HOST_INT));
                                                /* νĲ��̪��ܰ����ڽ�擾    */
    snd_adr_sys_int_work = (Uint8 *)(ADR_SND_MEM + 
                 ((Uint32)PEEK_W(ADR_SYS_TBL + ADR_SYS_INT_WORK) << 16
                  | (Uint32)PEEK_W(ADR_SYS_TBL + ADR_SYS_INT_WORK + 2)));
                                                /* ���Ѳ��̪��ܰ����ڽ�擾   */
                                                
    adr_com_block = adr_host_int_work;  /* ���ݏ������ݺ������ۯ����ڽ������ */
                                                
    adr_snd_area_crnt = (Uint32 *)(ADR_SND_MEM + 
                                  PEEK_L(ADR_SYS_TBL + ADR_ARA_CRNT));
                                                /* ����޴رϯ��CRNTܰ��擾   */
    adr_song_stat = (Uint16 *)(adr_host_int_work + ADR_SONG_STAT);
    adr_pcm = (Uint16 *)(adr_host_int_work + ADR_PCM);
    adr_seq = (Uint16 *)(adr_host_int_work + ADR_SEQ);
    adr_tl_vl = (Uint16 *)(adr_host_int_work + ADR_TL_VL);
    adr_tl_hz_vl = (Uint16 *)(adr_host_int_work + ADR_TL_HZ_VL);

    CopyMem((void *)
                    (PEEK_L(adr_sys_info_tbl + ADR_ARA_ADR) + ADR_SND_MEM),
                   (void *)(SND_INI_ARA_ADR(*sys_ini)),
                   CHG_LONG(SND_INI_ARA_SZ(*sys_ini))); /* ����޴رϯ�ߓ]��  */

    PER_SMPC_SND_ON();                          /* �T�E���hON                */

}

/******************************************************************************
 *
 * NAME:    SND_MoveData()  - �T�E���h�f�[�^�]��
 *
 * PARAMETERS :
 *      (1) Uint16 *source      - <i>   �T�E���h�f�[�^�]�����A�h���X
 *      (2) Uint32 size         - <i>   �T�E���h�f�[�^�]���T�C�Y
 *      (3) Uint16 data_kind    - <i>   �f�[�^���
 *      (4) Uint16 data_no      - <i>   �f�[�^�ԍ�
 *
 * DESCRIPTION:
 *      �T�E���h�f�[�^���T�E���h�������֓]�����܂��B
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

void SND_MoveData(Uint16 *source, Uint32 size,Uint16 data_kind, Uint16 data_no)
{
    void *adr;                                  /* �]����A�h���X            */
    Uint32 *load_mark_adr;                      /* �]���ς݃r�b�g�ݒ���ڽ  */

    GetSndMapInfo(&adr, &load_mark_adr, data_kind, data_no);
                                                /* ����޴رϯ�ߏ��擾      */

    CopyMem(adr, (void *)source, size);         /* ����޴رϯ�ߓ]��         */
    POKE_L(load_mark_adr, (*load_mark_adr | M_LOAD_MARK));
                                                /* �]���ς��ޯ� ��            */
}

/******************************************************************************
 *
 * NAME:    SND_ChgMap()    - �T�E���h�G���A�}�b�v�ύX
 *
 * PARAMETERS :
 *      (1) SndAreaMap area_no  - <i>   �T�E���h�G���A�}�b�v�ԍ�
 *
 * DESCRIPTION:
 *      �T�E���h�G���A�}�b�v��ύX���܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   �R�}���h���s���
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */
 
 SndRet SND_ChgMap(SndAreaMap area_no)
 {

    HOST_SET_INIT();                            /* νĲ��̪���ر�ݒ菉������ */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, area_no);                        /* �p�����[�^�Z�b�g          */
    SET_COMMAND(COM_CHG_MAP);                   /* �R�}���h�Z�b�g            */

    while(PEEK_W(adr_com_block + ADR_COM_DATA));
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, area_no);                        /* �p�����[�^�Z�b�g          */
    SET_COMMAND(COM_CHG_MAP);                   /* �R�}���h�Z�b�g            */
    while(PEEK_W(adr_com_block + ADR_COM_DATA));
    HOST_SET_RETURN(SND_RET_SET);
}
/******************************************************************************
 *
 * NAME:    SND_SetTlVl()   - �S�̉��ʐݒ�
 *
 * PARAMETERS :
 *      (1) SndTlVl vol         - <i>   �S�̉���
 *
 * DESCRIPTION:
 *      �S�̉��ʂ�ݒ肵�܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   �R�}���h���s���
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */
 
 SndRet SND_SetTlVl(SndTlVl vol)
 {
    HOST_SET_INIT();                            /* νĲ��̪���ر�ݒ菉������ */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, vol);                            /* �p�����[�^�Z�b�g          */
    SET_COMMAND(COM_SET_TL_VL);                 /* �R�}���h�Z�b�g            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_ChgEfct()   - �G�t�F�N�g�ύX
 *
 * PARAMETERS :
 *      (1) SndEfctBnkNum efct_no <i>   Effect bank number
 *
 * DESCRIPTION:
 *      �G�t�F�N�g��ύX���܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   �R�}���h���s���
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */
 
 SndRet SND_ChgEfct(SndEfctBnkNum efct_no)
 {
    HOST_SET_INIT();                            /* νĲ��̪���ر�ݒ菉������ */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, efct_no);                        /* �p�����[�^�Z�b�g          */
    SET_COMMAND(COM_CHG_EFCT);                  /* �R�}���h�Z�b�g            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_ChgMix()    - �~�L�T�ύX
 *
 * PARAMETERS :
 *      (1) SndToneBnkNum tone_no   <i>   ���F bank number
 *      (2) SndMixBnkNum mix_no     <i>   MIxer bank number
 *
 * DESCRIPTION:
 *      �~�L�T��ύX���܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   �R�}���h���s���
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */
 
 SndRet SND_ChgMix(SndToneBnkNum tone_no, SndMixBnkNum mix_no)
 {
    HOST_SET_INIT();                            /* νĲ��̪���ر�ݒ菉������ */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, tone_no);                        /* �p�����[�^�Z�b�g          */
    SET_PRM(1, mix_no);                         /* �p�����[�^�Z�b�g          */
    SET_COMMAND(COM_CHG_MIX);                   /* �R�}���h�Z�b�g            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_ChgMixPrm()     - �~�L�T�p�����[�^�ύX
 *
 * PARAMETERS :
 *      (1) SndEfctOut efct_out     <i>   Effect out select
 *      (2) SndLev level            <i>   Effect return Level
 *      (3) SndPan pan              <i>   Effect pan
 *
 * DESCRIPTION:
 *      �~�L�T�p�����[�^��ύX���܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   �R�}���h���s���
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */
 
 SndRet SND_ChgMixPrm(SndEfctOut efct_out, SndLev level, SndPan pan)
 {
    HOST_SET_INIT();                            /* νĲ��̪���ر�ݒ菉������ */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, efct_out);                       /* �p�����[�^�Z�b�g          */
    SET_PRM(1, ChgPan(pan) | (level << 5));     /* �p�����[�^�Z�b�g          */
    SET_COMMAND(COM_CHG_MIX_PRM);               /* �R�}���h�Z�b�g            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_ChkHard()       - �n�[�h�E�F�A�`�F�b�N
 *
 * PARAMETERS :
 *      (1) SndHardStat *stat       <o>   ʰ�������ð��
 *      (2) SndHardPrm prm         <i>   ʰ���������Ұ�
 *
 * DESCRIPTION:
 *      �n�[�h�E�F�A���`�F�b�N���܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   �R�}���h���s���
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */
 
 SndRet SND_ChkHard(SndHardStat *stat, SndHardPrm prm)
 {
    HOST_SET_INIT();                            /* νĲ��̪���ر�ݒ菉������ */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    *(Uint16 *)(ADR_SYS_TBL + ADR_HARD_CHK_STAT) = 0;
    SET_PRM(0, prm);                            /* �p�����[�^�Z�b�g          */
    SET_COMMAND(COM_CHK_HARD);                  /* �R�}���h�Z�b�g            */
    while((*stat = *(Uint16 *)(ADR_SYS_TBL + ADR_HARD_CHK_STAT)) == 0);
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_StopDsp()   - DSP���~
 *
 * PARAMETERS :
 *      �Ȃ��B
 *
 * DESCRIPTION:
 *      DSP�𒆎~���܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   �R�}���h���s���
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */
 
 SndRet SND_StopDsp(void)
 {
    HOST_SET_INIT();                            /* νĲ��̪���ر�ݒ菉������ */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_COMMAND(COM_STOP_DSP);                  /* �R�}���h�Z�b�g            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_OffAllSound()       - �S�T�E���h�����X���b�g��~
 *
 * PARAMETERS :
 *      �Ȃ��B
 *
 * DESCRIPTION:
 *      �S�T�E���h�����X���b�g���~���܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   �R�}���h���s���
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */
 
 SndRet SND_OffAllSound(void)
 {
    HOST_SET_INIT();                            /* νĲ��̪���ر�ݒ菉������ */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_COMMAND(COM_OFF_ALL_SOUND);             /* �R�}���h�Z�b�g            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_StartSeq()  - �V�[�P���X�J�n
 *
 * PARAMETERS :
 *      (1) SndSeqNum seq_no        -   <i>     �����Ǘ��ԍ�
 *      (2) SndSeqBnkNum seq_bk_no  -   <i>     Sequence bank number
 *      (3) SndSeqSongNum song_no   -   <i>     Sequence song number
 *      (4) SndSeqPri pri_lev       -   <i>     Priorty level
 *
 * DESCRIPTION:
 *      �V�[�P���X���J�n���܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   �R�}���h���s���
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */
 
 SndRet SND_StartSeq(SndSeqNum seq_no, SndSeqBnkNum seq_bk_no,
                    SndSeqSongNum song_no, SndSeqPri pri_lev)
 {
    HOST_SET_INIT();                            /* νĲ��̪���ر�ݒ菉������ */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, seq_no);                         /* �p�����[�^�Z�b�g          */
    SET_PRM(1, seq_bk_no);                      /* �p�����[�^�Z�b�g          */
    SET_PRM(2, song_no);                        /* �p�����[�^�Z�b�g          */
    SET_PRM(3, pri_lev);                        /* �p�����[�^�Z�b�g          */
    SET_COMMAND(COM_START_SEQ);                 /* �R�}���h�Z�b�g            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_StopSeq()   - �V�[�P���X���~
 *
 * PARAMETERS :
 *      (1) SndSeqNum seq_no    - <i>   �����Ǘ��ԍ�
 *
 * DESCRIPTION:
 *      �V�[�P���X�𒆎~���܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   �R�}���h���s���
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */
 
 SndRet SND_StopSeq(SndSeqNum seq_no)
 {
    HOST_SET_INIT();                            /* νĲ��̪���ر�ݒ菉������ */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, seq_no);                         /* �p�����[�^�Z�b�g          */
    SET_COMMAND(COM_STOP_SEQ);                  /* �R�}���h�Z�b�g            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_PauseSeq()  - �V�[�P���X�ꎞ���f
 *
 * PARAMETERS :
 *      (1) SndSeqNum seq_no    - <i>   �����Ǘ��ԍ�
 *
 * DESCRIPTION:
 *      �V�[�P���X���ꎞ���f���܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   �R�}���h���s���
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */
 
 SndRet SND_PauseSeq(SndSeqNum seq_no)
 {
    HOST_SET_INIT();                            /* νĲ��̪���ر�ݒ菉������ */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, seq_no);                         /* �p�����[�^�Z�b�g          */
    SET_COMMAND(COM_PAUSE_SEQ);                 /* �R�}���h�Z�b�g            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_ContSeq()   - �V�[�P���X�ꎞ���f����
 *
 * PARAMETERS :
 *      (1) SndSeqNum seq_no    - <i>   �����Ǘ��ԍ�
 *
 * DESCRIPTION:
 *      �V�[�P���X�̈ꎞ���f���������܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   �R�}���h���s���
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */
 
 SndRet SND_ContSeq(SndSeqNum seq_no)
 {
    HOST_SET_INIT();                            /* νĲ��̪���ر�ݒ菉������ */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, seq_no);                         /* �p�����[�^�Z�b�g          */
    SET_COMMAND(COM_CONT_SEQ);                  /* �R�}���h�Z�b�g            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_SetSeqVl()      - �V�[�P���X���ʐݒ�
 *
 * PARAMETERS :
 *      (1) SndSeqNum seq_no        -   <i>     �����Ǘ��ԍ�
 *      (2) SndSeqVl seq_vl         -   <i>     Sequence Volume
 *      (3) SndFade fade            -   <i>     fade Rate
 *
 * DESCRIPTION:
 *      �V�[�P���X�̉��ʂ�ݒ肵�܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   �R�}���h���s���
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */
 
 SndRet SND_SetSeqVl(SndSeqNum seq_no, SndSeqVl seq_vl, SndFade fade)
 {
    HOST_SET_INIT();                            /* νĲ��̪���ر�ݒ菉������ */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, seq_no);                         /* �p�����[�^�Z�b�g          */
    SET_PRM(1, seq_vl);                         /* �p�����[�^�Z�b�g          */
    SET_PRM(2, fade);                           /* �p�����[�^�Z�b�g          */
    SET_COMMAND(COM_SET_SEQ_VL);                /* �R�}���h�Z�b�g            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_ChgTempo()  - �e���|�ύX
 *
 * PARAMETERS :
 *      (1) SndSeqNum seq_no    - <i>   �����Ǘ��ԍ�
 *      (2) SndTempo tempo      - <i>   Tempo
 *
 * DESCRIPTION:
 *      �V�[�P���X�̃e���|��ύX���܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   �R�}���h���s���
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */
 
 SndRet SND_ChgTempo(SndSeqNum seq_no, SndTempo tempo)
 {
    HOST_SET_INIT();                            /* νĲ��̪���ر�ݒ菉������ */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, seq_no);                         /* �p�����[�^�Z�b�g          */
    SET_PRM(2, tempo >> 8);                     /* �p�����[�^�Z�b�g          */
    SET_PRM(3, tempo);                          /* �p�����[�^�Z�b�g          */
    SET_COMMAND(COM_CHG_TEMPO);                 /* �R�}���h�Z�b�g            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_CtrlDirMidi()   -   MIDI�_�C���N�g�R���g���[��
 *
 * PARAMETERS :
 *      (1) SndSeqNum seq_no    - <i>   �����Ǘ��ԍ�
 *      (2) SndSeqPri seq_pri   - <i>   Priorty Level
 *      (3) Uint8 md_com        - <i>   MIDI command
 *      (4) Uint8 ch            - <i>   MIDI channel
 *      (5) Uint8 dt1           - <i>   MIDI data1
 *      (6) Uint8 dt2           - <i>   MIDI data2
 *
 * DESCRIPTION:
 *      MIDI���_�C���N�g�ɃR���g���[�����܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   �R�}���h���s���
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */
 
 SndRet SND_CtrlDirMidi(SndSeqNum seq_no, SndSeqPri seq_pri, Uint8 md_com,
                        Uint8 ch, Uint8 dt1, Uint8 dt2)
 {
    HOST_SET_INIT();                            /* νĲ��̪���ر�ݒ菉������ */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, (seq_pri << 3) | md_com);        /* �p�����[�^�Z�b�g          */
    SET_PRM(1, (seq_no << 5) | ch);             /* �p�����[�^�Z�b�g          */
    SET_PRM(2, dt1);                            /* �p�����[�^�Z�b�g          */
    SET_PRM(3, dt2);                            /* �p�����[�^�Z�b�g          */
    SET_COMMAND(COM_CTRL_DIR_MIDI);             /* �R�}���h�Z�b�g            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_SetSeqPan()     -   �V�[�P���XPAN�ݒ�
 *
 * PARAMETERS :
 *      (1) SndSeqNum seq_no    - <i>   �����Ǘ��ԍ�
 *      (2) Uint8 ctrl_sw       - <i>   Control ON/OFF
 *      (3) Uint8 md_pan        - <i>   MIDI PAN data
 *
 * DESCRIPTION:
 *      �V�[�P���XPAN��ݒ肵�܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   �R�}���h���s���
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */
 
 SndRet SND_SetSeqPan(SndSeqNum seq_no, Uint8 ctrl_sw, Uint8 md_pan)
 {
    HOST_SET_INIT();                            /* νĲ��̪���ر�ݒ菉������ */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, seq_no);                         /* �p�����[�^�Z�b�g          */
    SET_PRM(1, (ctrl_sw | md_pan));             /* �p�����[�^�Z�b�g          */
    SET_COMMAND(COM_CTRL_DIR_MIDI);             /* �R�}���h�Z�b�g            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_StartPcm()  - PCM�J�n
 *
 * PARAMETERS :
 *      (1) SndPcmStartPrm *sprm    - <i>   PCM�J�n�p�����[�^�|�C���^
 *      (2) SndPcmChgPrm *cprm      - <i>   PCM�ύX�p�����[�^�|�C���^
 *
 * DESCRIPTION:
 *      PCM�f�[�^���Đ����܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   �R�}���h���s���
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */
 
 SndRet SND_StartPcm(SndPcmStartPrm *sprm, SndPcmChgPrm *cprm)
 {
    HOST_SET_INIT();                            /* νĲ��̪���ر�ݒ菉������ */
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
    SET_COMMAND(COM_START_PCM);                 /* �R�}���h�Z�b�g            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_StopPcm()   - PCM���~
 *
 * PARAMETERS :
 *      (1) SndPcmNum pcm_num       - <i>   �Đ���~PCM�X�g���[���Đ��ԍ�
 *
 * DESCRIPTION:
 *      PCM�𒆎~���܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   �R�}���h���s���
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */
 
 SndRet SND_StopPcm(SndPcmNum pcm_num)
 {
    HOST_SET_INIT();                            /* νĲ��̪���ر�ݒ菉������ */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, pcm_num);                        /* �p�����[�^�Z�b�g          */
    SET_COMMAND(COM_STOP_PCM);                  /* �R�}���h�Z�b�g            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_ChgPcm()    - PCM�ύX
 *
 * PARAMETERS :
 *      (1) SndPcmChgPrm *cprm      - <i>   PCM�ύX�p�����[�^�|�C���^
 *
 * DESCRIPTION:
 *      PCM�f�[�^���Đ����܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   �R�}���h���s���
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */
 
 SndRet SND_ChgPcm(SndPcmChgPrm *cprm)
 {
    HOST_SET_INIT();                            /* νĲ��̪���ر�ݒ菉������ */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, SND_PRM_NUM(*cprm));
    SET_PRM(1, (SND_PRM_LEV(*cprm) << 5) | ChgPan(SND_PRM_PAN(*cprm)));
    SET_PRM(2, SND_PRM_PICH(*cprm) >> 8);
    SET_PRM(3, SND_PRM_PICH(*cprm));
    SET_PRM(4, (SND_R_EFCT_IN(*cprm) << 3) | SND_R_EFCT_LEV(*cprm));
    SET_PRM(5, (SND_L_EFCT_IN(*cprm) << 3) | SND_L_EFCT_LEV(*cprm));
    SET_COMMAND(COM_CHG_PCM_PRM);               /* �R�}���h�Z�b�g            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_StartVlAnl()    - ���ʉ�͊J�n
 *
 * PARAMETERS :
 *      �Ȃ��B
 *
 * DESCRIPTION:
 *      ���ʉ�͂��s���܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   �R�}���h���s���
 *
 * CAVEATS:
 *      ���g���ш�ʉ��ʉ�͂�����ꍇ�́A�G�t�F�N�g�ύX�ɂĐ�p��DSP�v���O��
 *      �������s���Ă��������B
 *
 ******************************************************************************
 */
 
 SndRet SND_StartVlAnl(void)
 {
    HOST_SET_INIT();                            /* νĲ��̪���ر�ݒ菉������ */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_COMMAND(COM_START_VL_ANL);              /* �R�}���h�Z�b�g            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_StopVlAnl()     - ���ʉ�͒��~
 *
 * PARAMETERS :
 *      �Ȃ��B
 *
 * DESCRIPTION:
 *      ���ʉ�͂𒆎~���܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   �R�}���h���s���
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */
 
 SndRet SND_StopVlAnl(void)
 {
    HOST_SET_INIT();                            /* νĲ��̪���ر�ݒ菉������ */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_COMMAND(COM_STOP_VL_ANL);               /* �R�}���h�Z�b�g            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_SetCdDaLev()    - CD-DA Level�ݒ�
 *
 * PARAMETERS :
 *      (1) SndLev left         - <i>   ���o�͂̉���
 *      (2) SndLev right        - <i>   �E�o�͂̉���
 *
 * DESCRIPTION:
 *      ���݂̃X�e���I���ʂ�ύX���܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   �R�}���h���s���
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */
 
 SndRet SND_SetCdDaLev(SndLev left, SndLev right)
 {
    HOST_SET_INIT();                            /* νĲ��̪���ر�ݒ菉������ */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, (left) * 2 << 4);                /* �p�����[�^�Z�b�g          */
    SET_PRM(1, (right) * 2 << 4);               /* �p�����[�^�Z�b�g          */
    SET_COMMAND(COM_SET_CD_DA_LEV);             /* �R�}���h�Z�b�g            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_SetCdDaPan()    - CD-DA Pan�ݒ�
 *
 * PARAMETERS :
 *      (1) SndPan  left        - <i>   ���o�͂�PAN
 *      (2) SndPan  right       - <i>   �E�o�͂�PAN
 *
 * DESCRIPTION:
 *      ���݂̃X�e���IPAN��ύX���܂��B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      (1) SndRet              - <o>   �R�}���h���s���
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */
 
 SndRet SND_SetCdDaPan(SndPan left, SndPan right)
 {
    HOST_SET_INIT();                            /* νĲ��̪���ر�ݒ菉������ */
    if(GetComBlockAdr() == OFF) HOST_SET_RETURN(SND_RET_NSET);
    SET_PRM(0, ChgPan(left));                   /* �p�����[�^�Z�b�g          */
    SET_PRM(1, ChgPan(right));                  /* �p�����[�^�Z�b�g          */
    SET_COMMAND(COM_SET_CD_DA_PAN);             /* �R�}���h�Z�b�g            */
    HOST_SET_RETURN(SND_RET_SET);
}

/******************************************************************************
 *
 * NAME:    SND_GetSeqStat()    - �V�[�P���X�X�e�[�^�X�擾
 *
 * PARAMETERS :
 *      (1) SndSeqStat *status  - <o>   �V�[�P���X�X�e�[�^�X�|�C���^
 *      (2) SndSeqNum seq_no    - <i>   �����Ǘ��ԍ�
 *
 * DESCRIPTION:
 *      �V�[�P���X�X�e�[�^�X���擾���܂��B
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
 
 void SND_GetSeqStat(SndSeqStat *status, SndSeqNum seq_no)
 {
    SND_SEQ_STAT_MODE(*status) = (PEEK_W(adr_song_stat + seq_no)) & 0xff00;
    SND_SEQ_STAT_STAT(*status) = (Uint8)(PEEK_W(adr_song_stat + seq_no));
}

/******************************************************************************
 *
 * NAME:    SND_GetSeqPlayPos() - �����Ǘ��ԍ��Đ��ʒu�擾
 *
 * PARAMETERS :
 *      (1) SndSeqPlayPos *pos  - <o>   �����Ǘ��ԍ��Đ��ʒu
 *      (2) SndSeqNum seq_no    - <i>   �����Ǘ��ԍ�
 *
 * DESCRIPTION:
 *      �����Ǘ��ԍ��Đ��ʒu���擾���܂��B
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
 
 void SND_GetSeqPlayPos(SndSeqPlayPos *pos, SndSeqNum seq_no)
 {
    *pos = PEEK_W(adr_seq + seq_no);
}

/******************************************************************************
 *
 * NAME:    SND_GetPcmPlayAdr() - PCM���s�A�h���X�擾
 *
 * PARAMETERS :
 *      (1) SndPcmPlayAdr *pcm_adr  - <o>   PCM���s�A�h���X
 *      (2) SndPcmNum num           - <i>   PCM�Đ��ԍ�
 *
 * DESCRIPTION:
 *      PCM���s�A�h���X���擾���܂��B
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
 
 void SND_GetPcmPlayAdr(SndPcmPlayAdr *pcm_adr, SndPcmNum num)
 {
    SND_PCM_RADR(*pcm_adr) = (Uint8)(PEEK_W(adr_pcm + num) >> 8);
    SND_PCM_LADR(*pcm_adr) = (Uint8)PEEK_W(adr_pcm + num);
 }

/******************************************************************************
 *
 * NAME:    SND_GetAnlTlVl()    - ��͑S�̃X�e���I���ʎ擾
 *
 * PARAMETERS :
 *      (1) SndCdVlAnl *left    - <o>   ���o�͂̑S�̉�͉���
 *      (2) SndCdVlAnl *right   - <o>   �E�o�͂̑S�̉�͉���
 *
 * DESCRIPTION:
 *      �S�̃X�e���I��͉��ʂ��擾���܂��B
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
 
 void SND_GetAnlTlVl(SndCdVlAnl *left, SndCdVlAnl *right)
 {
    *left = (SndCdVlAnl)PEEK_W(adr_tl_vl);
    *right = (SndCdVlAnl)PEEK_W(adr_tl_vl + 1);
 }

/******************************************************************************
 *
 * NAME:    SND_GetAnlHzVl()    - ��͎��g���ш�ʃX�e���I���ʎ擾
 *
 * PARAMETERS :
 *      (1) SndCdHzSrVl *hz_vl - <o>   ���g���ш�ʃX�e���I��͉���
 *
 * DESCRIPTION:
 *      ���g���ш�ʃX�e���I��͉��ʂ��擾���܂��B
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
/**** �����񋟊֐� ***********************************************************/
/*****************************************************************************/
/******************************************************************************
 *
 * NAME:    DmaClrZero()    - DMA�ɂ�郁�����̃[���N���A
 *
 * PARAMETERS :
 *      (1) void *dst       - <i>   �f�B�X�e�B�l�[�V�����A�h���X
 *      (2) Uint32 cnt      - <i>   �N���A�o�C�g��
 *
 * DESCRIPTION:
 *      DMA���g�p���ă��������[���N���A���܂��B
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

static void DmaClrZero(void *dst, Uint32 cnt)
{
    memset(dst, 0x00, cnt);
}

/******************************************************************************
 *
 * NAME:    GetSndMapInfo()     - �T�E���h�G���A�}�b�v���擾
 *
 * PARAMETERS :
 *      (1) void **adr          - <o>   �]����A�h���X
 *      (2) Uint32 **ladr        - <o>   �]���ς݃r�b�g�A�h���X
 *      (3) Uint16 data_kind    - <i>   �f�[�^���
 *      (4) Uint16 data_no      - <i>   �f�[�^�ԍ�
 *
 * DESCRIPTION:
 *      �T�E���h�G���A�}�b�v�̏����擾���܂��B
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
 * NAME:    ChgPan()            - PAN�f�[�^�ϊ�
 *
 * PARAMETERS :
 *      (1) SndPan pan          - <i>   PAN
 *
 * DESCRIPTION:
 *      �o�^�p��PAN�ɕϊ����܂�
 *
 * PRECONDITIONS:
 *      (1) Uint16              - <o>   �ϊ���PAN
 *
 * POSTCONDITIONS:
 *      �Ȃ�
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */
static Uint16 ChgPan(SndPan pan)
{
    return(((pan) < 0) ? (~(pan) + 0x10 + 1) : (pan));
}

/******************************************************************************
 *
 * NAME:    CopyMem()           - �������R�s�|
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
 * NAME:    GetComBlockAdr()            - �R�}���h�u���b�N
 *
 * PARAMETERS :
 *
 * DESCRIPTION:
 *
 * PRECONDITIONS:
 *
 * POSTCONDITIONS:
 *      (1) Uint8               - <ret> �R�}���h�u���b�N�̋󂫏��
 *
 * CAVEATS:
 *
 ******************************************************************************
 */

static Uint8 GetComBlockAdr(void)
{
    if(*NOW_ADR_COM_DATA){              /* �ȑO����ۯ����������ς݂łȂ���?*/
        /* ���R�}���h�u���b�N�A�h���X�ݒ菈�� ********************************/
        if(NOW_ADR_COM_DATA >= (MAX_ADR_COM_DATA - SIZE_COM_BLOCK)){
                                                    /* �ő�l��?            */
            return OFF;                             /* ��ۯ��󂫖���      */
        }else{
            adr_com_block += SIZE_COM_BLOCK;        /* ���ݺ������ۯ����ı���*/
            while(NOW_ADR_COM_DATA < (MAX_ADR_COM_DATA - SIZE_COM_BLOCK)){
                if(*NOW_ADR_COM_DATA){
                    adr_com_block += SIZE_COM_BLOCK;
                }else{
                    return ON;                      /* ��ۯ��󂫗L��         */
                }
            }
            return OFF;                             /* ��ۯ��󂫖���         */
        }
    }else{
        adr_com_block = adr_host_int_work;  /* ��ۯ��̐擪��              */
        while(NOW_ADR_COM_DATA < (MAX_ADR_COM_DATA - SIZE_COM_BLOCK)){
            if(*NOW_ADR_COM_DATA){
                adr_com_block += SIZE_COM_BLOCK;
            }else{
                return ON;                          /* ��ۯ��󂫗L��         */
            }
        }
        return OFF;                                 /* ��ۯ��󂫖���         */
    }
}
