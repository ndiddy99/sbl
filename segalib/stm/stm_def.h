/*****************************************************************************
 *	�\�t�g�E�F�A���C�u����
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library: �X�g���[���V�X�e��
 * Module : ���[�J���p�w�b�_�t�@�C��
 * File	  : stm_def.h
 * Date   : 1994-11-11
 * Version: 1.16
 * Auther : H.T
 *
 *****************************************************************************/
#if	!defined(STM_DEF_H)
#define	STM_DEF_H	1

#define	STM_LOCAL

/*****************************************************************************
 *	�萔�}�N��
 *****************************************************************************/

/* �T�[�o�������		*/
enum SvrStat {
    SVR_UNUSE	= FALSE,		/* �I�u�W�F�N�g�g�p���łȂ�	*/
    SVR_USE	= TRUE,			/* �I�u�W�F�N�g�g�p��		*/
    SVR_END
};


/* �X�g���[���̃X�e�[�^�X	*/
enum StmStat {
    STAT_NOREAD	= 0x20,			/* �A�N�Z�X����Ȃ�		*/
    STAT_DOING	= STM_EXEC_DOING,	/* ������			*/
    STAT_WAIT	= STM_EXEC_WAIT,	/* �]���҂�(�Q�[�g��, �̈�t��)	*/
    STAT_COMPLETED = STM_EXEC_COMPLETED,	/* �A�N�Z�X����		*/
    STAT_END
};


/* �X�g���[���A�g���r���[�g	*/
enum StmAtr {
    STM_ATR_READ	= 0x01,		/* �ǂݍ��݃X�g���[��		*/
    STM_ATR_RESI	= 0x02,		/* �풓�X�g���[��		*/
    STM_ATR_LOOPNOREAD	= 0x04,		/* ���[�v���ǂݍ��܂Ȃ�		*/
    STM_ATR_READEND	= 0x08,		/* �ǂݍ��ݍς�			*/
    STM_ATR_TRSTART	= 0x10,		/* �]���J�n�ݒ�ς�		*/
    STM_ATR_TREND	= 0x20,		/* �]���I���ݒ�ς�		*/
    STM_ATR_END
};


/* �X�g���[���O���[�v�A�g���r���[�g	*/
enum StmGrpAtr {
    STMGRP_ATR_LOOPSPECIFY	= 0x01,	/* ���[�v�X�g���[�����`���ꂽ	*/
    STMGRP_ATR_PLAY		= 0x02,	/* �Đ���			*/
    STMGRP_ATR_READ		= 0x04,	/* �ǂݍ��ݒ�			*/
    STMGRP_ATR_CHKBUF		= 0x08,	/* �o�b�t�@�T�C�Y�`�F�b�N	*/
    STMGRP_ATR_PAUSE		= 0x10,	/* �|�[�Y��			*/
    STMGRP_ATR_SEEK		= 0x20,	/* �V�[�N��			*/
    STMGRP_ATR_END
};


#define	FID_OPEN_FRANGE	0x3fffffff	/* �Z�N�^�ʒu�I�[�v������FID	*/

#define	TR_CONT		1		/* �]����			*/

#define	TMODE_DFL	STM_TR_CPU	/* �f�t�H���g�]�����[�h		*/

/*****************************************************************************
 *	�f�[�^�^�̒�`
 *****************************************************************************/

/* �R�}���h�֐�			*/
typedef	Bool	(*stmCmdFunc)(void *obj);

#define	CMD_NULLFUNC	((stmCmdFunc)0)	/* NULL�֐�			*/

/* �R�}���h			*/
typedef struct {
    stmCmdFunc	func;			/* �������R�}���h		*/
    void	*obj;			/* �������I�u�W�F�N�g		*/
} StmCmd;

#define	CMD_FUNC(cmd)	((cmd)->func)
#define	CMD_OBJ(cmd)	((cmd)->obj)


#define	MNG_GRP(mng, i)		((mng)->stmgrptbl + i)
#define	MNG_GRPTBL(mng)		((mng)->stmgrptbl)
#define	MNG_GRPMAX(mng)		((mng)->stmgrpmax)
#define	MNG_STM(mng, i)		((mng)->stmtbl + i)
#define	MNG_STMTBL(mng)		((mng)->stmtbl)
#define	MNG_STMMAX(mng)		((mng)->stmmax)
#define	MNG_CURGRP(mng)		((mng)->curgrp)
#define	MNG_SFAD(mng)		((mng)->sfad)
#define	MNG_EFAD(mng)		((mng)->efad)
#define	MNG_ERR(mng)		((mng)->err)
#define	MNG_SVREXEC(mng)	((mng)->svrexec)

#define	STM_GRP(stm)		((stm)->grp)
#define	STM_GRPHN(stm)		((StmGrpHn)(STM_GRP(stm)))
#define	STM_ALIAS(stm)		((stm)->alias)
#define	STM_SFADLST(stm)	((stm)->sfadlst)
#define	STM_FLTLST(stm)		((stm)->fltlst)
#define	STM_GFS(stm)		((stm)->gfs)
#define	STM_FLOW(stm)		((stm)->flow)
#define	STM_DTSRC(stm)		(&((stm)->flow->dtsrc))
#define	STM_TRFUNC(stm)		((stm)->trfunc)
#define	STM_OBJ(stm)		((stm)->obj)
#define	STM_TRFAD(stm)		((stm)->trfad)
#define	STM_ATR(stm)		((stm)->atr)
#define	STM_SVRSTAT(stm)	((stm)->svrstat)

#define	STM_GRP_SFADLST(grp)	((grp)->sfadlst)
#define	STM_GRP_FLTLST(grp)	((grp)->fltlst)
#define	STM_GRP_LOOPSTM(grp)	((grp)->loopstm)
#define	STM_GRP_LSTSTM(grp)	((grp)->lststm)
#define	STM_GRP_NEXTTRNS(grp)	((grp)->nexttrns)
#define	STM_GRP_FAD(grp)	((grp)->fad)
#define	STM_GRP_ACSTAT(grp)	((grp)->acstat)
#define	STM_GRP_CHKSTM(grp)	((grp)->chkstm)
#define	STM_GRP_CHKSCTNUM(grp)	((grp)->chksctnum)
#define	STM_GRP_CHKFUNC(grp)	((grp)->chkfunc)
#define	STM_GRP_CHKOBJ(grp)	((grp)->chkobj)
#define	STM_GRP_LOOPMAX(grp)	((grp)->loopmax)
#define	STM_GRP_LOOPCNT(grp)	((grp)->loopcnt)
#define	STM_GRP_PUID(grp)	((grp)->puid)
#define	STM_GRP_ATR(grp)	((grp)->atr)
#define	STM_GRP_SVRSTAT(grp)	((grp)->svrstat)


/*****************************************************************************
 *	�����}�N��
 *****************************************************************************/

/* CD�t�@�C���̔��� */
#define STM_IS_CDFILE(stm)	\
    ((GFS_DTS_FTYPE(STM_DTSRC(stm)) == CD_FILE) ? TRUE: FALSE)

/* �ǂݍ��݃X�g���[���̔���	*/
#define	IS_STM_READ(stm)	\
    (((STM_ATR(stm) & STM_ATR_READ) != 0) ? TRUE : FALSE)

/* �풓�X�g���[���̔���	*/
#define	IS_STM_RESI(stm) \
    (((STM_ATR(stm) & STM_ATR_RESI) != 0) ? TRUE : FALSE)

/* ���[�v���ǂݍ��܂Ȃ�����	*/
#define	IS_STM_LOOPNOREAD(stm) \
    (((STM_ATR(stm) & STM_ATR_LOOPNOREAD) != 0) ? TRUE : FALSE)

/* �ǂݍ��ݍς݂̔���		*/
#define	IS_STM_READEND(stm) \
    (((STM_ATR(stm) & STM_ATR_READEND) != 0) ? TRUE : FALSE)

/* �Đ������ǂ����̔���		*/
#define	IS_PLAYING(grp)	\
    (((STM_GRP_ATR(grp) & STMGRP_ATR_PLAY) != 0) ? TRUE : FALSE)

/* �ǂݍ��ݒ����ǂ����̔���	*/
#define	IS_READING(grp)	\
    (((STM_GRP_ATR(grp) & STMGRP_ATR_READ) != 0) ? TRUE : FALSE)

/* �V�[�N�����ǂ����̔���	*/
#define	IS_SEEKING(grp)	\
    (((STM_GRP_ATR(grp) & STMGRP_ATR_SEEK) != 0) ? TRUE : FALSE)

/* �|�[�Y�����ǂ����̔���	*/
#define	IS_PAUSING(grp)	\
    (((STM_GRP_ATR(grp) & STMGRP_ATR_PAUSE) != 0) ? TRUE : FALSE)

/* ���[�v�X�g���[�����ݒ肳�ꂽ���ǂ����̔���	*/
#define	IS_LOOPSPECIFY(grp)	\
    (((STM_GRP_ATR(grp) & STMGRP_ATR_LOOPSPECIFY) != 0) ? TRUE : FALSE)

/* ���[�v�X�g���[���w��t���O�̐ݒ�		*/
#define	SET_LOOPSPECIFY(grp)	(STM_GRP_ATR(grp) |= STMGRP_ATR_LOOPSPECIFY)

/* ���[�v�X�g���[���w��t���O�̉���		*/
#define	CLR_LOOPSPECIFY(grp)	(STM_GRP_ATR(grp) &= ~STMGRP_ATR_LOOPSPECIFY)

/* �G���[���ݒ肳��Ă��邩�ǂ����̔���		*/
#define	IS_SET_ERRCODE(mng)	\
    ((STM_ERR_CODE(&MNG_ERR(mng)) != STM_ERR_OK) ? TRUE : FALSE)

/*****************************************************************************
 *	�ϐ��̐錾
 *****************************************************************************/

/* �X�g���[���Ǘ��̈�	*/
extern StmMng	*stm_mng_ptr;

/* �R�}���h�o�^�̈�	*/
extern StmCmd	stm_cmd_area;

/*****************************************************************************
 *	�֐��̐錾
 *****************************************************************************/

/*--- STMMNG functions -------------------------------------------------*/

/* �ŏIFAD�̎擾			*/
extern Sint32	STMMNG_GetLstFad(StmGrpHn grp);

/* �i��ڑ������X�g�̐ؒf		*/
extern Bool	STMMNG_RemoveFlt(StmHn stm);

/* GFCD�̃G���[���X�g���[���̃G���[�ɕϊ����ݒ肷��	*/
extern void	STM_SetGfcdErr(Sint32 code);

/* �ǂݍ��ݏI���t���O�̐ݒ�		*/
extern void	STMSVR_SetReadEnd(StmGrpHn grp);

/* CD�o�b�t�@���Z�N�^���̎擾		*/
extern Sint32	STMSVR_GetNumCdbuf(StmHn stm);

/* �]���̈������������			*/
extern void	STMTRN_ResetTrBuf(StmHn stm);

/* �]���̈�t���̃`�F�b�N		*/
extern Bool	STMTRN_IsTrBufFull(StmHn stm);

/*--- STMERR functions -------------------------------------------------*/

/* �����֐��̐ݒ�			*/
extern void	STMERR_SetFunc(Sint32 funcid);

/* �G���[�R�[�h�̐ݒ�			*/
extern void	STMERR_SetCode(Sint32 code);

/* �G���[�֐��̌Ăт���			*/
extern void	STMERR_Exec(void);

#endif


