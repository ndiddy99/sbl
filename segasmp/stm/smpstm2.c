/*****************************************************************************
 *	�\�t�g�E�F�A���C�u����
 *
 *	Copyright (c) 1994 SEGA
 *
 * Module : �X�g���[���V�X�e���T���v���v���O����2
 *	    (����Đ��p�T���v���v���O����)
 * File	  : smpstm2.c
 * Date   : 1994-11-11
 * Auther : H.T
 *
 *****************************************************************************/

/*****************************************************************************
 *	�C���N���[�h�t�@�C��
 *****************************************************************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	"sega_stm.h"

/*****************************************************************************
 *	�萔�}�N��
 *****************************************************************************/

/* �t�@�C�����̒���		*/
#define	FNAME_LEN	16

/* ���o���o�b�t�@�T�C�Y	*/
#define	FORM1_SCTSIZ	2048
#define	BUF_UNITNUM	10
#define	BUF_SIZE	(BUF_UNITNUM * FORM1_SCTSIZ)

/* �����X�g���[����		*/
#define	STM_NUM		5

/* �Đ��I��			*/
#define	PEND		(-1)

/* ���C�u�����p�萔		*/
#define	FILE_NUM	30	/* �P�f�B���N�g��������̍ő�t�@�C����	*/
#define	OPEN_MAX	24	/* �����Ɉ����X�g���[���^�t�@�C����	*/
#define	GRP_MAX		5	/* �����Ɉ����O���[�v��			*/

/*****************************************************************************
 *	�ϐ��̒�`
 *****************************************************************************/

/* �t�@�C���V�X�e����Ɨ̈�	*/
Uint32		WorkGfs[GFS_WORK_SIZE(OPEN_MAX)/sizeof(Uint32)];

/* �X�g���[���V�X�e����Ɨ̈�	*/
Uint32		WorkStm[STM_WORK_SIZE(OPEN_MAX, GRP_MAX)/sizeof(Uint32)];

/* �f�B���N�g���ǂݍ��ݗ̈�	*/
GfsDirName	DirTbl[FILE_NUM];

/* ���o���o�b�t�@		*/
Uint8		ReadBuf[BUF_SIZE];

/* �t�@�C����			*/
Sint8		MngTbl[STM_NUM][FNAME_LEN] = {	
                                                 "SMP_STM.1ST",
						 "SMP_STM.2ND",
						 "SMP_STM.3RD",
						 "SMP_STM.4TH",
						 "SMP_STM.5TH",
					     };

/* �Đ����ԍ�			*/
Sint32		PlayOrder[] = {0, 1, 2, 3, 4, 3, 2, 1, 0, 2, 1, 3, 4, PEND};


/*****************************************************************************
 *	�֐��̒�`
 *****************************************************************************/

/*-------------------------------
 * ������
 * �߂�l : TRUE  ����������
 *          FALSE ���������s
 *-------------------------------*/
Bool	SMP_Init(void)
{
    GfsDirTbl	dir;
    Sint32	ndir;
    Bool	init;

   /* �t�@�C���V�X�e��������	*/
    GFS_DIRTBL_TYPE(&dir) = GFS_DIR_NAME;
    GFS_DIRTBL_NDIR(&dir) = FILE_NUM;
    GFS_DIRTBL_DIRNAME(&dir) = DirTbl;
    ndir = GFS_Init(OPEN_MAX, WorkGfs, &dir);
    if (ndir < 0) {
	return (FALSE);
    }

    /* �X�g���[���V�X�e��������	*/
    init = STM_Init(GRP_MAX, OPEN_MAX, WorkStm);
    if (init == FALSE) {
	return (FALSE);
    }
    return (TRUE);
}


/*-------------------------------
 * �X�g���[���̃I�[�v��
 * �߂�l : �X�g���[���n���h��
 *-------------------------------*/
StmHn	SMP_Open(StmGrpHn grp, Sint8 *fname)
{
    Sint32	fid;
    StmHn	stm;
    StmKey	key;

    /* �t�@�C��������t�@�C�����ʎq�ւ̕ϊ�	*/
    fid = GFS_NameToId(fname);
    if (fid < 0) {
	return (NULL);
    }

    /* �X�g���[���L�[�̐ݒ�			*/
    STM_KEY_CN(&key) = STM_KEY_SMMSK(&key) = STM_KEY_CIMSK(&key) = STM_KEY_NONE;

    /* �X�g���[���I�[�v��			*/
    stm = STM_OpenFid(grp, fid, &key, STM_LOOP_NOREAD);
    if (stm == NULL) {
	return (NULL);
    }

    /* ���o���̈�̐ݒ�			*/
    STM_SetTrBuf(stm, ReadBuf, BUF_UNITNUM, STM_UNIT_FORM1);

    /* �]���Q�[�g�̐ݒ�				*/
    STM_SetTrGate(stm, STM_GATE_CLOSE);

    return (stm);
}


/*-------------------------------
 * ���[�U�֐�
 *-------------------------------*/
void	SMP_User(void)
{
    /* �A�v���P�[�V�����Ǝ��̏����������Ȃ�	*/
}


/*-------------------------------
 * �X�g���[���ǂݍ��ݎ��s
 *-------------------------------*/
void	SMP_Exec(StmGrpHn grp)
{
    Sint32	i;
    Bool	complete;
    StmHn	stm, next;

    /* ���[�v�񐔂̐ݒ�			*/
    STM_SetLoop(grp, STM_LOOP_DFL, STM_LOOP_ENDLESS);

    /* ���s�O���[�v�̐ݒ�		*/
    STM_SetExecGrp(grp);

    /* �ŏ��̃X�g���[���̃I�[�v��	*/
    complete = TRUE;
    stm = SMP_Open(grp, MngTbl[PlayOrder[0]]);
    next = NULL;

    /* �X�g���[���A�N�Z�X���s		*/
    i = 1;
    while (stm != NULL) {
	/* �O�̃X�g���[���̃A�N�Z�X������������Q�[�g���J����	*/
	if (complete == TRUE) {
	    STM_SetTrGate(stm, STM_GATE_OPEN);
	}

	STM_ExecServer();
	
	/* �f�[�^�̎��o�����n�܂����玟�̃X�g���[�����I�[�v������	*/
	if ((next == NULL)&&
	    (STM_GetLenTrBuf(stm) > 0)&&(PlayOrder[i] != PEND)) {
	    next = SMP_Open(grp, MngTbl[PlayOrder[i]]);
	    i++;
	}

	/* ���o���̈�t���̃`�F�b�N	*/
	if (STM_IsTrBufFull(stm) == TRUE) {
	    STM_ResetTrBuf(stm);
	}

	/* �A�N�Z�X�����̃`�F�b�N	*/
	if ((complete = STM_IsComplete(stm)) == TRUE) {
	    STM_Close(stm);
	    stm = next;
	    next = NULL;
	}

	/* ���[�U�֐�	*/
	SMP_User();
    }
}
	

/*-------------------------------
 * ���C��
 *-------------------------------*/
void	main(void)
{
    Bool	init;
    StmGrpHn	grp;

    /* ������		*/
    init = SMP_Init();
    if (init == FALSE) {
	return;
    }

    /* �n���h���擾	*/
    grp = STM_OpenGrp();
    if (grp == NULL) {
	return;
    }

    /* �ǂݍ��ݎ��s	*/
    SMP_Exec(grp);
}


/* end of file */
