/*-----------------------------------------------------------------------------
 *  FILE: sega_per.h
 *
 *      Copyright(c) 1994 SEGA.
 *
 *  PURPOSE:
 *
 *      �V�X�e���^�y���t�F�������C�u�����g�p�w�b�_�t�@�C���B
 *
 *  DESCRIPTION:
 *
 *      �V�X�e���^�y���t�F�������C�u�������g�p����v���O�����ɃC���N���[�h����
 *      ���������B
 *
 *  AUTHOR(S)
 *
 *      1994-05-19  N.T Ver.0.90
 *
 *  MOD HISTORY:
 *      1994-08-26  N.T Ver.1.02
 *
 *-----------------------------------------------------------------------------
 */

#ifndef SEGA_PER_H
#define SEGA_PER_H

/********************************* per_xpt.h *********************************/
/*
 * GLOBAL DEFINES/MACROS DEFINES
 */
/**** ���W�X�^�A�h���X *******************************************************/
#ifndef _DEB2
#define PER_REG_COMREG  ((volatile Uint8 *)0x2010001f)   /* �R�}���h���W�X�^          */
#define PER_REG_SR      ((volatile Uint8 *)0x20100061)   /* �X�e�[�^�X���W�X�^        */
#define PER_REG_SF      ((volatile Uint8 *)0x20100063)   /* �X�e�[�^�X�t���O          */
#define PER_REG_IREG    ((volatile Uint8 *)0x20100001)   /* IREG                      */
#define PER_REG_OREG    ((volatile Uint8 *)0x20100021)   /* OREG                      */
#else
#define PER_REG_COMREG  ((Uint8 *)0x0603001f)   /* �R�}���h���W�X�^          */
#define PER_REG_SR      ((Uint8 *)0x06030061)   /* �X�e�[�^�X���W�X�^        */
#define PER_REG_SF      ((Uint8 *)0x06030063)   /* �X�e�[�^�X�t���O          */
#define PER_REG_IREG    ((Uint8 *)0x06030001)   /* IREG                      */
#define PER_REG_OREG    ((Uint8 *)0x06030021)   /* OREG                      */
#endif

/**** �r�b�g�ʒu *************************************************************/
#define PER_B_SF        0x1                     /* �X�e�[�^�X�t���O          */

/**** �֐��`���}�N�� *********************************************************/
#define PER_PokeByte(address,data)  (*(volatile Uint8 *)(address) = (Uint8)(data))
                                                    /* �޲��ް����ڽ���ڏ���-*/
                                                    /* ����                  */
#define PER_PeekByte(address)   (*(volatile Uint8 *)(address))
                                                    /* �޲��ް����ڽ���ړǂ�-*/
                                                    /* ����                  */

/*
 * TYPEDEFS
 */

/********************************* per_prt.h *********************************/
/*
 * GLOBAL DEFINES/MACROS DEFINES
 */
/* �y���t�F����ID */
#define PER_ID_NCON     0xff                    /* ���ڑ�                    */
#define PER_ID_UNKOWN   0xf0                    /* SMPC UNKNOWN              */
#define PER_ID_DGT      0x00                    /* �f�W�^���f�o�C�X          */
#define PER_ID_ANL      0x10                    /* �A�i���O�f�o�C�X          */
#define PER_ID_PNT      0x20                    /* �|�C���e�B���O�f�o�C�X    */
#define PER_ID_KBD      0x30                    /* �L�[�{�[�h�@�@�@          */
#define PER_ID_M3BP     0xe1                    /* ���K�h���C�u3�{�^���p�b�h */
#define PER_ID_M6BP     0xe2                    /* ���K�h���C�u6�{�^���p�b�h */

/* �y���t�F�����T�C�Y */
#define PER_SIZE_DGT    2                       /* �f�W�^���f�o�C�X          */
#define PER_SIZE_ANL    5                       /* �A�i���O�f�o�C�X          */
#define PER_SIZE_PNT    5                       /* �|�C���e�B���O�f�o�C�X    */
#define PER_SIZE_KBD    4                       /* �L�[�{�[�h�@�@�@          */
#define PER_SIZE_M3BP   1                       /* ���K�h���C�u3�{�^���p�b�h */
#define PER_SIZE_M6BP   2                       /* ���K�h���C�u6�{�^���p�b�h */

/* �C���g�o�b�N��� */
#define PER_KD_SYS      0                       /* �V�X�e���f�[�^�擾        */
#define PER_KD_PER      1                       /* �y���t�F�����f�[�^�擾    */
#define PER_KD_PERTIM   2                       /* �y���t�F�����f�[�^�擾�{  */
                                                /* �����f�[�^�擾            */
/* �r�b�g�ʒu ****************************************************************/
/* �f�W�^���f�o�C�X�f�[�^�^	 */
#define	PER_DGT_U   (1 << 12)                   /* UP                        */
#define	PER_DGT_D   (1 << 13)                   /* DOWN                      */
#define	PER_DGT_R   (1 << 15)                   /* RIGHT                     */
#define	PER_DGT_L   (1 << 14)                   /* LEFT                      */
#define	PER_DGT_A   (1 << 10)                   /* A                         */
#define	PER_DGT_B   (1 << 8)                    /* B                         */
#define	PER_DGT_C   (1 << 9)                    /* C                         */
#define	PER_DGT_S   (1 << 11)                   /* START                     */
#define	PER_DGT_X   (1 << 6)                    /* X                         */
#define	PER_DGT_Y   (1 << 5)                    /* Y                         */
#define	PER_DGT_Z   (1 << 4)                    /* Z                         */
#define	PER_DGT_TR  (1 << 7)                    /* �g���K RIGHT              */
#define	PER_DGT_TL  (1 << 3)                    /* �g���K LEFT               */

/* �|�C���e�B���O�f�o�C�X�f�[�^�^ */
/***** �f�[�^ */
#define PER_PNT_R   (1 << 1)                    /* RIGHT                     */
#define PER_PNT_L   (1 << 0)                    /* LEFT                      */
#define PER_PNT_MID (1 << 2)                    /* MIDDLE                    */
#define PER_PNT_CNT (1 << 3)                    /* CENTER                    */
#define PER_PNT_XO  (1 << 6)                    /* X���I�[�o�t���[           */
#define PER_PNT_YO  (1 << 7)                    /* Y���I�[�o�t���[           */

/* �L�[�{�[�h�f�o�C�X�f�[�^�^ */
/***** ����L�[ */
#define PER_KBD_CL  (1 << 6)                    /* Caps Lock                 */
#define PER_KBD_NL  (1 << 5)                    /* Num Lock                  */
#define PER_KBD_SL  (1 << 4)                    /* Scrool Lock               */
#define PER_KBD_MK  (1 << 3)                    /* Make                      */
#define PER_KBD_BR  (1 << 0)                    /* Break                     */

/* ���K�h���C�u3�{�^���p�b�h�f�[�^�^ */
#define	PER_M3BP_U  PER_DGT_U                   /* UP                        */
#define	PER_M3BP_D  PER_DGT_D                   /* DOWN                      */
#define	PER_M3BP_R  PER_DGT_R                   /* RIGHT                     */
#define	PER_M3BP_L  PER_DGT_L                   /* LEFT                      */
#define	PER_M3BP_A  PER_DGT_A                   /* A                         */
#define	PER_M3BP_B  PER_DGT_B                   /* B                         */
#define	PER_M3BP_C  PER_DGT_C                   /* C                         */
#define	PER_M3BP_S  PER_DGT_S                   /* START                     */

/* ���K�h���C�u6�{�^���p�b�h�f�[�^�^ */
#define	PER_M6BP_U  PER_DGT_U                    /* UP                        */
#define	PER_M6BP_D  PER_DGT_D                    /* DOWN                      */
#define	PER_M6BP_R  PER_DGT_R                    /* RIGHT                     */
#define	PER_M6BP_L  PER_DGT_L                    /* LEFT                      */
#define	PER_M6BP_A  PER_DGT_A                    /* A                         */
#define	PER_M6BP_B  PER_DGT_B                    /* B                         */
#define	PER_M6BP_C  PER_DGT_C                    /* C                         */
#define	PER_M6BP_S  PER_DGT_S                    /* START                     */
#define	PER_M6BP_X  PER_DGT_X                    /* X                         */
#define	PER_M6BP_Y  PER_DGT_Y                    /* Y                         */
#define	PER_M6BP_Z  PER_DGT_Z                    /* Z                         */
#define	PER_M6BP_MD PER_DGT_TR                   /* MODE                      */

/* �V�X�e���f�[�^�o�� */
/* �V�X�e���X�e�[�^�X */
#define PER_SS_DOTSEL   (1 <<  6)               /* �J�[�g���b�W�R�[�h        */
#define PER_SS_SYSRES   (1 <<  1)               /* �G���A�R�[�h              */
#define PER_SS_MSHNMI   (1 <<  3)               /* �V�X�e���X�e�[�^�X        */
#define PER_SS_SNDRES   (1 <<  0)               /* SMPC������                */
#define PER_SS_CDRES    (1 << 14)               /* SMPC�X�e�[�^�X            */

/* SMPC������ */
/***** �}�X�N */
#define PER_MSK_LANGU   (0xf << 0)              /* ����}�X�N                */
#define PER_MSK_SE      (0x1 << 8)              /* SE                        */
#define PER_MSK_STEREO  (0x1 << 9)              /* STEREO or MONO            */
#define PER_MSK_HELP    (0x1 << 10)             /* HELP                      */
/***** ���� */
#define PER_ENGLISH     0x0                     /* English(�p��)             */
#define PER_DEUTSCH     0x1                     /* Deutsch(�h�C�c��)         */
#define PER_FRANCAIS    0x2                     /* francais(�t�����X��)      */
#define PER_ESPNOL      0x3                     /* Espnol(�X�y�C����)        */
#define PER_ITALIANO    0x4                     /* Italiano(�C�^���A��)      */
#define PER_JAPAN       0x5                     /* Japan(���{��)             */

/* SMPC�X�e�[�^�X */
#define PER_SS_RESET    (1 <<  6)               /* ���Z�b�g�}�X�N���        */
#define PER_SS_SETTIM   (1 <<  7)               /* �����ݒ���              */

/* �A�N�Z�X�}�N�� ************************************************************/
/* �f�W�^���f�o�C�X�f�[�^�^ */
#define PER_DGT(data)   ((PerDgtInfo *)(data))

/* �A�i���O�f�o�C�X�f�[�^�^  */
#define PER_ANL(data)   ((PerAnlInfo *)(data))

/* �|�C���e�B���O�f�o�C�X�f�[�^�^ */
#define PER_PNT(data)   ((PerPntInfo *)(data))

/* �L�[�{�[�h�f�o�C�X�f�[�^�^ */
#define PER_KBD(data)   ((PerKbdInfo *)(data))

/* ���K�h���C�u3�{�^���p�b�h�f�[�^�^ */
#define PER_M3BP(data)  ((PerM3bpInfo *)(data))

/* ���K�h���C�u6�{�^���p�b�h�f�[�^�^ */
#define PER_M6BP(data)  ((PerM6bpInfo *)(data))

/* �V�X�e���f�[�^�o�� */
#define PER_GS_CC(data)         ((data)->cc)    /* �J�[�g���b�W�R�[�h        */
#define PER_GS_AC(data)         ((data)->ac)    /* �G���A�R�[�h              */
#define PER_GS_SS(data)         ((data)->ss)    /* �V�X�e���X�e�[�^�X        */
#define PER_GS_SM(data)         ((data)->sm)    /* SMPC������                */
#define PER_GS_SMPC_STAT(data)  ((data)->stat)  /* SMPC�X�e�[�^�X            */

/* �萔 **********************************************************************/
#define PER_HOT_RES_ON  0x1                     /* �z�b�g���Z�b�gON          */
#define PER_HOT_RES_OFF 0x0                     /* �z�b�g���Z�b�gOFF         */

/* �C���g�o�b�N���s���      */
#define PER_INT_OK      0x0                     /* ����                      */
#define PER_INT_ERR     0x1                     /* �G���[                    */

/******************************************************************************
 *
 * NAME:    PER_GET_TIM             -   �����擾
 *
 ******************************************************************************
 */
#define PER_GET_TIM()  (per_get_time_adr)

/******************************************************************************
 *
 * NAME:    PER_GET_SYS             -   �V�X�e���f�[�^�擾
 *
 ******************************************************************************
 */

#define PER_GET_SYS()   ((per_set_sys_flg == OFF) ? NULL : &per_get_sys_data)

#ifndef _PER_HOT_ENA
/******************************************************************************
 *
 * NAME:    PER_GET_HOT_RES         -   �z�b�g���Z�b�g�擾
 *
 ******************************************************************************
 */
#define PER_GET_HOT_RES()  (per_hot_res)
#endif  /* _PER_HOT_ENA */

/*
 * TYPEDEFS
 */
/* �f�o�C�X�f�[�^�^ */
typedef Uint16  PerDgtData;                     /* �f�W�^���f�o�C�X�f�[�^�^  */

typedef struct  {                               /* �A�i���O�f�o�C�X�f�[�^�^  */
    PerDgtData  dgt;                            /* �f�W�^���f�o�C�X�f�[�^�^  */
    Sint16      x;                              /* X����Βl(0�`255)         */
    Sint16      y;                              /* Y����Βl(0�`255)         */
    Sint16      z;                              /* Z����Βl(0�`255)         */
}PerAnlData;

typedef struct  {                       /* �|�C���e�B���O�f�o�C�X�f�[�^�^    */
    PerDgtData  dgt;                            /* �f�W�^���f�o�C�X�f�[�^�^  */
    Uint16      data;                           /* �f�[�^                    */
    Sint16      x;                              /* X���ړ���(-128�`127)      */
    Sint16      y;                              /* Y���ړ���(-128�`127)      */
}PerPntData;

typedef struct  {                           /* �L�[�{�[�h�f�o�C�X�f�[�^�^    */
    PerDgtData  dgt;                            /* �f�W�^���f�o�C�X�f�[�^�^  */
    Uint8       skey;                           /* ����L�[                  */
    Uint8       key;                            /* �L�[                      */
}PerKbdData;

typedef Uint8   PerM3bpData;            /* ���K�h���C�u3�{�^���p�b�h�f�[�^�^ */
typedef Uint16  PerM6bpData;            /* ���K�h���C�u6�{�^���p�b�h�f�[�^�^ */
typedef Uint8   PerId;                  /* �y���t�F����ID                    */
typedef Uint8   PerSize;                /* �y���t�F�����T�C�Y                */
typedef Uint8   PerKind;                        /* �C���g�o�b�N���          */
typedef Uint16  PerNum;                         /* �K�v�y���t�F������        */

/* �y���t�F�����f�[�^�o�� */
typedef struct  {                               /* �f�W�^���f�o�C�X          */
    PerDgtData  data;                           /* ���݃y���t�F�����f�[�^    */
    PerDgtData  push;                           /* �O�񖢉������݉����{�^��  */
    PerId       id;                             /* �y���t�F����ID            */
}PerDgtInfo;

typedef struct  {                               /* �A�i���O�f�o�C�X          */
    PerAnlData  data;                           /* ���݃y���t�F�����f�[�^    */
    PerAnlData  push;                           /* �O�񖢉������݉����{�^��  */
    PerId       id;                             /* �y���t�F����ID            */
}PerAnlInfo;

typedef struct  {                               /* �|�C���e�B���O�f�o�C�X    */
    PerPntData  data;                           /* ���݃y���t�F�����f�[�^    */
    PerPntData  push;                           /* �O�񖢉������݉����{�^��  */
    PerId       id;                             /* �y���t�F����ID            */
}PerPntInfo;

typedef struct  {                               /* �L�[�{�[�h�f�o�C�X        */
    PerKbdData  data;                           /* ���݃y���t�F�����f�[�^    */
    PerKbdData  push;                           /* �O�񖢉������݉����{�^��  */
    PerId       id;                             /* �y���t�F����ID            */
}PerKbdInfo;

typedef struct  {                               /* ���K�h���C�u3�{�^���p�b�h */
    PerM3bpData data;                           /* ���݃y���t�F�����f�[�^    */
    PerM3bpData push;                           /* �O�񖢉������݉����{�^��  */
    PerId       id;                             /* �y���t�F����ID            */
}PerM3bpInfo;

typedef struct  {                               /* ���K�h���C�u6�{�^���p�b�h */
    PerM6bpData data;                           /* ���݃y���t�F�����f�[�^    */
    PerM6bpData push;                           /* �O�񖢉������݉����{�^��  */
    PerId       id;                             /* �y���t�F����ID            */
}PerM6bpInfo;

typedef struct  {                               /* �V�X�e���o��              */
    Uint8   cc;                                 /* �J�[�g���b�W�R�[�h        */
    Uint8   ac;                                 /* �G���A�R�[�h              */
    Uint16  ss;                                 /* �V�X�e���X�e�[�^�X        */
    Uint32  sm;                                 /* SMPC������                */
    Uint8   stat;                               /* SMPC�X�e�[�^�X            */
}PerGetSys;

typedef void PerGetPer;                         /* �y���t�F�����f�[�^�o��    */

/*
 * EXTERNAL VARIABLE DECLARATIONS
 */
#ifndef _PER_HOT_ENA
extern Uint8 per_hot_res;                       /* �z�b�g���Z�b�g���        */
#endif  /* _PER_HOT_ENA */
extern Uint8 *per_get_time_adr;                 /* �����f�[�^�擾�擪�A�h���X*/
extern PerGetSys per_get_sys_data;              /* �V�X�e���f�[�^�擾�i�[    */
extern Uint8 per_set_sys_flg;                   /* �V�X�e���f�[�^�ݒ�t���O  */
extern Uint8 per_time_out_flg;                  /* �^�C���A�E�g�t���O        */

/*
 * EXTERNAL FUNCTION PROTOTYPE  DECLARATIONS
 */
Uint32 PER_Init(PerKind, PerNum, PerId, PerSize, Uint32 *, Uint8);
                                                /* �C���g�o�b�N������        */
Uint32 PER_GetPer(PerGetPer **);                /* �y���t�F�����f�[�^�擾    */
PerGetSys PER_GetSys(void);                     /* �V�X�e���f�[�^�擾        */
void PER_IntFunc(void);                         /* SMPC���荞�ݏ���          */

/********************************* per_smp.h *********************************/
/*
 * GLOBAL DEFINESMACROS DEFINES
 */

/**** �R�}���h ***************************************************************/
#define PER_SM_MSHON    0x00                    /* �}�X�^SH ON               */
#define PER_SM_SSHON    0x02                    /* �X���[�uSH ON             */
#define PER_SM_SSHOFF   0x03                    /* �X���[�uSH OFF            */
#define PER_SM_SNDON    0x06                    /* �T�E���hON                */
#define PER_SM_SNDOFF   0x07                    /* �T�E���hOFF               */
#define PER_SM_CDON     0x08                    /* CD ON                     */
#define PER_SM_CDOFF    0x09                    /* CD OFF                    */
#define PER_SM_SYSRES   0x0d                    /* �V�X�e���S�̃��Z�b�g      */
#define PER_SM_NMIREQ   0x18                    /* NMI���N�G�X�g             */
#define PER_SM_RESENA   0x19                    /* �z�b�g���Z�b�g�C�l�[�u��  */
#define PER_SM_RESDIS   0x1a                    /* �z�b�g���Z�b�g�f�B�Z�[�u��*/
#define PER_SM_SETSM    0x17                    /* SMPC�������ݒ�            */
#define PER_SM_SETTIM   0x16                    /* �����ݒ�                  */

/*****************************************************************************/
/*****************************************************************************/
/**** �ᐅ���}�N�� ***********************************************************/
/*****************************************************************************/
/*****************************************************************************/

/******************************************************************************
 *
 * NAME:    PER_SMPC_WAIT() - SMPC�҂����ԏ���
 *
 * PARAMETERS :
 *      �Ȃ��B
 *
 * DESCRIPTION:
 *      SMPC�𐳂������s���邽�߂́A�҂����Ԃ����B
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      �Ȃ��B
 *
 * CAVEATS:
 *      PER_SMPC_SET_IREG(),PER_SMPCCmdGo()�̑O�ɕK�����s���邱�ƁB
 *
 ******************************************************************************
 */

#define PER_SMPC_WAIT()                        /* SMPC�҂����ԏ���         */\
            do{                                                               \
                while((PER_PeekByte(PER_REG_SF) & PER_B_SF) == PER_B_SF);\
                                                /* SF��"1"�̊ԌJ��Ԃ�      */\
                PER_PokeByte(PER_REG_SF, PER_B_SF);\
                                                /* SF<-"1"                  */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_GO_CMD() - SMPC�R�}���h���s
 *
 * PARAMETERS :
 *      (1) Uint8   smpc_cmd    - <i>   SMPC�}�N����
 *
 * DESCRIPTION:
 *      �擾����SMPC�}�N������SMPC��COMREG�i�R�}���h���W�X�^�j�փZ�b�g����B
 *  �i�Z�b�g���邱�Ƃɂ��ASMPC�n�[�h��COMREG�ɏ����ꂽ�R�}���h�����s����j
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

#define PER_SMPC_GO_CMD(smpc_cmd)                 /* SMPC�R�}���h���s         */\
            do{                                                               \
             PER_PokeByte(PER_REG_COMREG, smpc_cmd);/* COMREG�ɃR�}���hWRITE*/\
             while(PER_PeekByte(PER_REG_SF) & PER_B_SF);\
                                                    /* SF��"1"�łȂ��Ȃ�܂�*/\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_STATS_GET()  - SMPC�X�e�[�^�X�擾
 *
 * PARAMETERS :
 *      (1) Uint8   stats_reg   - <o>   �R�}���h���s�X�e�[�^�X
 *
 * DESCRIPTION:
 *      �R�}���h���s��̃X�e�[�^�X���擾����
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

#define PER_SMPC_STATS_GET(stats_reg)           /* SMPC�X�e�[�^�X�擾       */\
            do{                                                               \
                stats_reg = PER_PeekByte(PER_SR);                             \
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_SET_IREG()   - SMPC IREG�Z�b�g
 *
 * PARAMETERS :
 *      (1) Uint32  ireg_no     - <o>   IREG�ԍ�
 *      (2) Uint8   ireg_prm    - <i>   IREG�Z�b�g�l
 *
 * DESCRIPTION:
 *      �w�肳�ꂽIREG�ԍ��A�h���X��IREG�l���Z�b�g����B
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

#define PER_SMPC_SET_IREG(ireg_no, ireg_prm)    /* SMPC IREG�Z�b�g          */\
            do{                                                               \
                PER_PokeByte((PER_REG_IREG + (ireg_no * 2)), ireg_prm);   \
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_NO_IREG()    - IREG�ݒ薳���R�}���h���s
 *
 ******************************************************************************
 */

#define PER_SMPC_NO_IREG(com)\
            do{                                                               \
                PER_SMPC_WAIT();                /* SMPC�҂����ԏ���         */\
                PER_SMPC_GO_CMD(com);           /* SMPC�R�}���h���s         */\
            }while(FALSE)

/*****************************************************************************/
/*****************************************************************************/
/**** �������}�N�� ***********************************************************/
/*****************************************************************************/
/*****************************************************************************/

/******************************************************************************
 *
 * NAME:    PER_SMPC_MSH_ON()     - �}�X�^SH ON
 *
 ******************************************************************************
 */

#define PER_SMPC_MSH_ON()\
            do{                                                               \
              PER_SMPC_NO_IREG(PER_SM_MSHON);   /* IREG�ݒ薳���R�}���h���s */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_SSH_ON()     - �X���[�uSH ON
 *
 ******************************************************************************
 */

#define PER_SMPC_SSH_ON()\
            do{                                                               \
              PER_SMPC_NO_IREG(PER_SM_SSHON);   /* IREG�ݒ薳���R�}���h���s */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_SSH_OFF()    - �X���[�uSH OFF
 *
 ******************************************************************************
 */

#define PER_SMPC_SSH_OFF()\
            do{                                                               \
              PER_SMPC_NO_IREG(PER_SM_SSHOFF);  /* IREG�ݒ薳���R�}���h���s */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_SND_ON()     - �T�E���hON
 *
 ******************************************************************************
 */

#define PER_SMPC_SND_ON()\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_SNDON);   /* IREG�ݒ薳���R�}���h���s */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_SND_OFF()    - �T�E���hOFF
 *
 ******************************************************************************
 */

#define PER_SMPC_SND_OFF()\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_SNDOFF);  /* IREG�ݒ薳���R�}���h���s */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_CD_ON()      - CD ON
 *
 ******************************************************************************
 */

#define PER_SMPC_CD_ON()\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_CDON);    /* IREG�ݒ薳���R�}���h���s */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_CD_OFF()     - CD OFF
 *
 ******************************************************************************
 */

#define PER_SMPC_CD_OFF()\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_CDOFF);   /* IREG�ݒ薳���R�}���h���s */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_SYS_RES()    - �V�X�e���S�̃��Z�b�g
 *
 ******************************************************************************
 */

#define PER_SMPC_SYS_RES()\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_SYSRES);  /* IREG�ݒ薳���R�}���h���s */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_NMI_REQ()    - NMI���N�G�X�g
 *
 ******************************************************************************
 */

#define PER_SMPC_NMI_REQ()\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_NMIREQ);  /* IREG�ݒ薳���R�}���h���s */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_RES_ENA()    - �z�b�g���Z�b�g�C�l�[�u��
 *
 ******************************************************************************
 */

#define PER_SMPC_RES_ENA()\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_RESENA);  /* IREG�ݒ薳���R�}���h���s */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_RES_DIS()    - �z�b�g���Z�b�g�f�B�Z�[�u��
 *
 ******************************************************************************
 */

#define PER_SMPC_RES_DIS()\
            do{                                                               \
                PER_SMPC_NO_IREG(PER_SM_RESDIS);  /* IREG�ݒ薳���R�}���h���s */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_SET_SM()     - SMPC�������ݒ�
 *
 ******************************************************************************
 */

#define PER_SMPC_SET_SM(ireg)\
            do{                                                               \
                PER_SMPC_WAIT();                 /* SMPC�҂����ԏ���         */\
                PER_SMPC_SET_IREG(0, (ireg) >> 24); /* IREG0�Z�b�g          */\
                PER_SMPC_SET_IREG(1, (ireg) >> 16); /* IREG0�Z�b�g          */\
                PER_SMPC_SET_IREG(2, (ireg) >>  8); /* IREG0�Z�b�g          */\
                PER_SMPC_SET_IREG(3, (ireg) >>  0); /* IREG0�Z�b�g          */\
                PER_SMPC_GO_CMD(PER_SM_SETSM);    /* SMPC�R�}���h���s         */\
            }while(FALSE)

/******************************************************************************
 *
 * NAME:    PER_SMPC_SET_TIM()    - �����ݒ�
 *
 ******************************************************************************
 */

#define PER_SMPC_SET_TIM(ireg)\
            do{                                                               \
                PER_SMPC_WAIT();                 /* SMPC�҂����ԏ���         */\
                PER_SMPC_SET_IREG(6, *(ireg));    /* IREG6�Z�b�g              */\
                PER_SMPC_SET_IREG(5, *(ireg + 1));/* IREG5�Z�b�g              */\
                PER_SMPC_SET_IREG(4, *(ireg + 2));/* IREG4�Z�b�g              */\
                PER_SMPC_SET_IREG(3, *(ireg + 3));/* IREG3�Z�b�g              */\
                PER_SMPC_SET_IREG(2, *(ireg + 4));/* IREG2�Z�b�g              */\
                PER_SMPC_SET_IREG(1, *(ireg + 5));/* IREG1�Z�b�g              */\
                PER_SMPC_SET_IREG(0, *(ireg + 6));/* IREG0�Z�b�g              */\
                PER_SMPC_GO_CMD(PER_SM_SETTIM);   /* SMPC�R�}���h���s         */\
            }while(FALSE)

/*
 * STRUCTURE DECLARATIONS
 */

/*
 * TYPEDEFS
 */

/*
 * EXTERNAL VARIABLE DECLARATIONS
 */

/*
 * EXTERNAL FUNCTION PROTOTYPE  DECLARATIONS
 */

#endif  /* ifndef SEGA_PER_H */
/******************************* end of file *********************************/
