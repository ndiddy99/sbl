/*****************************************************************************
 *	�\�t�g�E�F�A���C�u����
 *
 *	Copyright (c) 1994 SEGA
 *
 * Module : �X�g���[���V�X�e���T���v���v���O����1
 *	    (���[�v�Đ��p�T���v���v���O����)
 * File	  : smpstm1.c
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

/* �t�@�C����			*/
#define	SMP_STM_FN1	"SMP_STM.FN1"
#define	SMP_STM_FN2	"SMP_STM.FN2"
#define	SMP_STM_FN3	"SMP_STM.FN3"

/* �ǂݍ��݃o�b�t�@�T�C�Y	*/
#define	FORM1_SCTSIZ	2048
#define	FN1_UNITNUM	5
#define	FN2_UNITNUM	10
#define	FN3_UNITNUM	15
#define	FN1_BUFSIZ	(FN1_UNITNUM * FORM1_SCTSIZ)
#define	FN2_BUFSIZ	(FN2_UNITNUM * FORM1_SCTSIZ)
#define	FN3_BUFSIZ	(FN3_UNITNUM * FORM1_SCTSIZ)

/* ���[�v��			*/
#define	LOOP_MAX	3

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

/* �ǂݍ��݃o�b�t�@		*/
Uint8		Fn1_Buf[FN1_BUFSIZ];
Uint8		Fn2_Buf[FN2_BUFSIZ];
Uint8		Fn3_Buf[FN3_BUFSIZ];


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
StmHn	SMP_Open(StmGrpHn grp, Sint8 *fname, void *buf, Sint32 nunit)
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
    stm = STM_OpenFid(grp, fid, &key, STM_LOOP_READ);
    if (stm == NULL) {
	return (NULL);
    }

    /* �ǂݍ��ݗ̈�̐ݒ�			*/
    STM_SetTrBuf(stm, buf, nunit, STM_UNIT_FORM1);

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
 * �ǂݍ��ݗ̈�̃`�F�b�N
 *-------------------------------*/
void	SMP_ChkBuf(StmGrpHn grp, Sint32 nstm, StmHn stmtbl[], Sint32 rscnt[])
{
    Sint32	i;
    Sint32	lpcnt;
    Bool	bful;

    lpcnt = STM_GetLoopCnt(grp);

    /* �ǂݍ��ݗ̈悪�����ς��ɂȂ����烊�Z�b�g����	*/
    for (i = 0; i < nstm; i++) {
	bful = STM_IsTrBufFull(stmtbl[i]);
	if (bful == TRUE) {
	    STM_ResetTrBuf(stmtbl[i]);
	    rscnt[i]++;
	}
    }
}



/*-------------------------------
 * �X�g���[���ǂݍ��ݎ��s
 *-------------------------------*/
void	SMP_Exec(StmGrpHn grp)
{
    StmHn	stmtbl[OPEN_MAX];
    Sint32	stat;
    Sint32	nstm;
    Sint32	i;
    Sint32	rscnt[OPEN_MAX];

    /* ���[�v�񐔂̐ݒ�			*/
    STM_SetLoop(grp, STM_LOOP_DFL, LOOP_MAX);

    /* ���s�O���[�v�̐ݒ�		*/
    STM_SetExecGrp(grp);

    /* �O���[�v�ɑ�����X�g���[���̎擾	*/
    nstm = STM_GetStmNum(grp);
    for (i = 0; i < nstm; i++) {
	stmtbl[i] = STM_GetStmHndl(grp, i);
	rscnt[i] = 0;
    }

    /* �ǂݍ��ݎ��s			*/
    while(1) {
	stat = STM_ExecServer();
	if (stat == STM_EXEC_COMPLETED) {
	    break;
	}
	
	/* �ǂݍ��ݗ̈�̃`�F�b�N	*/
	SMP_ChkBuf(grp, nstm, stmtbl, rscnt);

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
    StmHn	stm1, stm2, stm3;

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
    stm1 = SMP_Open(grp, SMP_STM_FN1, Fn1_Buf, FN1_UNITNUM);
    if (stm1 == NULL) {
	return;
    }
    stm2 = SMP_Open(grp, SMP_STM_FN2, Fn2_Buf, FN2_UNITNUM);
    if (stm2 == NULL) {
	return;
    }
    stm3 = SMP_Open(grp, SMP_STM_FN3, Fn3_Buf, FN3_UNITNUM);
    if (stm3 == NULL) {
	return;
    }

    /* �ǂݍ��ݎ��s	*/
    SMP_Exec(grp);
}

/* end of file */


