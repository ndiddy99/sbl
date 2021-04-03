/*****************************************************************************
 *	�\�t�g�E�F�A���C�u����
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library: �X�g���[���V�X�e��
 * Module : �T�[�o���W���[��
 * File	  : stm_svr.c
 * Date   : 1994-11-11
 * Version: 1.16
 * Auther : H.T
 *
 *****************************************************************************/

/*****************************************************************************
 *	�C���N���[�h�t�@�C��
 *****************************************************************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"sega_stm.h"
#include	"../gfs/gfs_def.h"
#include	"../gfs/gfs_cdc.h"
#include	"../gfs/gfs_cdf.h"
#include	"../gfs/gfs_cdb.h"
#include	"../gfs/gfs_trn.h"
#include	"../gfs/gfs_buf.h"
#include	"../gfs/gfs_dir.h"
#include	"stm_loc.h"
#include	"stm_def.h"

/*****************************************************************************
 *	�ϐ��̒�`
 *****************************************************************************/

/* �R�}���h�o�^�̈�	*/
StmCmd	stm_cmd_area;

/*****************************************************************************
 *	�֐��̒�`
 *****************************************************************************/
/*--- STMMNG functions -------------------------------------------------*/
extern	Sint32	STMMNG_ResetLstStm(StmGrpHn grp);

/*****************************************************
 * CD�X�g���[�����Ȃ����ǂ����𒲂ׂ�
 * grp	: �X�g���[���O���[�v
 * �߂�l : TRUE CD�X�g���[���͂Ȃ�
 *****************************************************/
STM_LOCAL Bool	stmmng_isNoCdFile(StmGrpHn grp)
{
    StmHn	stm;

    /* �풓�X�g���[���̓��[�v�X�g���[���ɂȂ�Ȃ�	*/
    if (STM_GRP_LOOPSTM(grp) != NULL) {
	return (FALSE);
    }
    for (stm = STM_GRP_SFADLST(grp); stm != NULL; stm = STM_SFADLST(stm)) {
	if (STM_IS_CDFILE(stm) == TRUE) {
	    return (FALSE);
	}
    }
    return (TRUE);
}


/*************************************************
 * CSCT�r�b�g�֐��̓o�^
 * grp : ���s���X�g���[���O���[�v
 *************************************************/
STM_LOCAL void	stm_execCsct(void *grp)
{
    Sint32	nsct;

    STM_GRP_ATR((StmGrpHn)grp) |= STMGRP_ATR_READ;
    if (STM_GRP_CHKFUNC((StmGrpHn)grp) != STM_CDBUF_NULLFUNC) {
	if (STM_GRP_CHKSTM((StmGrpHn)grp) == STM_CDBUF_REST) {
	    nsct = STL_CsctGetFreeSctnum();
	    if (nsct < STM_GRP_CHKSCTNUM((StmGrpHn)grp)) {
		STM_GRP_ATR((StmGrpHn)grp) |= STMGRP_ATR_CHKBUF;
	    }
	} else {
	    nsct = STL_CsctGetSctNum(STM_FLOW(STM_GRP_CHKSTM((StmGrpHn)grp)));
	    if (nsct >= STM_GRP_CHKSCTNUM((StmGrpHn)grp)) {
		STM_GRP_ATR((StmGrpHn)grp) |= STMGRP_ATR_CHKBUF;
	    }
	}
    }
}


/*************************************************
 * �X�e�[�^�X�ɂ��G���[�̐ݒ�
 * stat	: �h���C�u�̃X�e�[�^�X
 *************************************************/
STM_LOCAL void	stm_setErr(Sint32 stat)
{
    Sint32	code = STM_ERR_OK;

    if (stat == GFS_STAT_OPEN) {
	code = STM_ERR_CDOPEN;
    } else if (stat == GFS_STAT_NODISC) {
	code = STM_ERR_CDNODISC;
    } else if (stat == GFS_STAT_FATAL) {
	code = STM_ERR_FATAL;
    } else if (stat == GFS_STAT_ERROR) {
	code = STM_ERR_CDRD;
    }
    if (code != STM_ERR_OK) {
	STMERR_SetCode(code);
    }
}
	

/*--- stmtsk functions -------------------------------------------------*/

/*************************************************
 * �T�[�o���s�O���[�v�̕ύX
 * grp	  : �ύX��̃X�g���[���O���[�v�n���h��
 * �߂�l : TRUE	��������
            FALSE	������
 *************************************************/
STM_LOCAL Bool	stmtsk_setExecGrp(void *_grp)
{
    StmGrpHn	grp, oldgrp;
    StmHn	stm;
    Sint32	fltno;
    Sint32	fad;
    Sint32	lst_fad;
    Sint32	puid;
    Sint32	stat;
    Sint32	ret;
    StmHn	next;
    Uint16	hirq;
    Bool	result = TRUE;

    grp = (StmGrpHn)_grp;
    if (MNG_CURGRP(stm_mng_ptr) != grp) {
	oldgrp = MNG_CURGRP(stm_mng_ptr);
	/* �ǂݍ��ݒ��̃X�g���[���O���[�v���~����	*/
	if (oldgrp != NULL) {
	    stat = GFCF_GetStat(&fad, &hirq);
	    stm_setErr(stat);			/* �G���[�R�[�h�̐ݒ�	*/
	    /* �h���C�u���~����	*/
	    if ((IS_PLAYING(oldgrp) == TRUE)&&
		(IS_PAUSING(oldgrp) == FALSE)) {
		ret = STL_StopIn(oldgrp);
		if (ret != STM_ERR_OK) {
		    STMERR_SetCode(ret);
		}
		STM_GRP_ATR(oldgrp) |= STMGRP_ATR_PAUSE;
	    }
	    /* �s�b�N�A�b�v���������	*/
	    if (STM_GRP_PUID(oldgrp) >= 0) {
		GFCD_UngetPickup(STM_GRP_PUID(oldgrp));
		STM_GRP_PUID(oldgrp) = -1;
		GFCD_SetCsctFunc(GFCD_NULLFUNC, NULL);
	    }
	    /* �O���[�v���������̏ꍇ	*/
	    if (STM_GRP_ACSTAT(oldgrp) != STM_EXEC_COMPLETED) {
		STM_GRP_ACSTAT(oldgrp) = STM_EXEC_PAUSE;	/* �|�[�Y�ݒ�	*/
	        /* COMPLETED�̏ꍇ�X�e�[�^�X�͕ω����Ȃ�	*/
	    }
	    /* ��~����			*/
	    if (stat == GFS_STAT_NOACT) {
		next = STM_GRP_NEXTTRNS(oldgrp);
		if (next != NULL) {
		    GFTR_Stop(STM_GFS(next), GFTR_ALLSCT);
		    GFS_TRN_STAT(&GFS_FILE_TRANS(STM_GFS(next))) = GFTR_ST_NOACT;
                                      /* END ��ԂɂȂ�Ȃ��悤�ɂ���	*/
		}
		if (IS_PAUSING(oldgrp) == TRUE) {
		    lst_fad = STMMNG_GetLstFad(oldgrp);
		    /* �ǂݍ��݂��������Ă��Ȃ��ꍇFAD���X�V����	*/
		    fad = STL_GetCurFad();
		    if (STM_GRP_FAD(oldgrp) <= lst_fad) {
			STM_GRP_FAD(oldgrp) = fad;
		    }
		}
		STM_GRP_ATR(oldgrp) &= ~(STMGRP_ATR_PLAY|STMGRP_ATR_READ|STMGRP_ATR_SEEK|STMGRP_ATR_PAUSE);
		oldgrp = NULL;
	    }
	}

	/* �V�����O���[�v��ݒ肷��			*/
	if (oldgrp == NULL) {
	    if (grp != NULL) {
		if (STM_GRP_PUID(grp) < 0) {
		    puid = GFCD_GetPickup();
		    if (puid >= GFCD_ERR_OK) {
			STM_GRP_PUID(grp) = puid;
		    }
		    GFCD_SetCsctFunc(stm_execCsct, grp);
		}
		if (STM_GRP_PUID(grp) >= 0) {
		    /* CD�f�o�C�X�̐ڑ��ύX	*/
		    stm = STM_GRP_FLTLST(grp);
		    if (stm != NULL) {
			fltno = GFS_CDR_FLT(STM_DTSRC(stm));
		    } else {
			fltno = CDC_NUL_SEL;
		    }
		    GFCD_SetCon(fltno, STM_GRP_PUID(grp));
		    ret = GFCD_WaitServer();
		    if (ret != GFCD_ERR_OK) {
			STM_SetGfcdErr(ret);
		    }
		    STM_GRP_ATR(grp) &= ~(STMGRP_ATR_PLAY|STMGRP_ATR_READ|STMGRP_ATR_SEEK);
		} else {
		    STMERR_SetCode(STM_ERR_PUINUSE);
		}
	    }
	    MNG_CURGRP(stm_mng_ptr) = grp;
	    result = TRUE;
	} else {
	    result = FALSE;
	}
    }
    return (result);
}



/*--- STM functions ----------------------------------------------------*/

/*************************************************
 * �T�[�o���s�O���[�v�̎w��(7.1)
 * grp	: �X�g���[���O���[�v�n���h��
 *************************************************/
Bool	STM_NwSetExecGrp(StmGrpHn grp)
{
    if (CMD_FUNC(&stm_cmd_area) != CMD_NULLFUNC) {
	return (FALSE);
    }
    STMERR_SetFunc(STM_SETEXECGRP);
    CMD_FUNC(&stm_cmd_area) = stmtsk_setExecGrp;
    CMD_OBJ(&stm_cmd_area) = grp;
    return (TRUE);
}


/*************************************************
 * �T�[�o���s�O���[�v�̎w��(7.2)
 * grp	: �X�g���[���O���[�v�n���h��
 *************************************************/
Bool	STM_SetExecGrp(StmGrpHn grp)
{
    Bool	complete;
    Sint32	stat;

    complete = STM_NwSetExecGrp(grp);
    if (complete == FALSE) {
	return (FALSE);
    }
    do {
	stat = STM_ExecServer();
    } while (stat != STM_EXEC_TSKEND);
    return (TRUE);
}



/*************************************************
 * �X�g���[���f�[�^�]�����̎擾(6.3)
 * stm	: �X�g���[���n���h��
 * �߂�l : TRUE	�]����
 *	  : FALSE	�]�����Ă��Ȃ�
 *************************************************/
STM_LOCAL Bool	stmsvr_isTrans(StmHn stm)
{
    Bool	trans;

    if (GFS_TRN_DPS(&(GFS_FILE_TRANS(STM_GFS(stm)))) != NULL) {
	trans = TRUE;
    } else {
	trans = FALSE;
    }
    return (trans);
}


Bool	STM_IsTrans(StmHn stm)
{
    Bool	trans;

    STMERR_SetFunc(STM_ISTRANS);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (FALSE);
    }
    stm = STM_ALIAS(stm);
    trans = stmsvr_isTrans(stm);
    return (trans);
}



/*************************************************
 * �ǂݍ��݃t���O�̉���
 * grp	: �X�g���[���O���[�v�n���h��
 *************************************************/
STM_LOCAL void	stm_setReadFalse(StmGrpHn grp)
{
    Sint32	fad;
    StmHn	stm;
    Sint32	sfad, efad;

    fad = STM_GRP_FAD(grp);
    for (stm = STM_GRP_SFADLST(grp); stm != NULL; stm = STM_SFADLST(stm)) {
	/* �X�g���[���̏I��FAD���擾	*/
	STL_GetPlayArea(STM_FLOW(stm), &sfad, &efad);
	if (sfad > fad) {	/* �J�n�Z�N�^�����݈ʒu�ȍ~�̏ꍇ    */
	    GFS_FILE_ASTAT(STM_GFS(stm)) = GFS_SVR_COMPLETED;
	    STM_ATR(stm) &= ~STM_ATR_READ;
	}
    }
}


/*************************************************
 * �]���̈�̏�����
 * grp	: �X�g���[���O���[�v�n���h��
 *************************************************/
STM_LOCAL void	stm_resetTrbuf(StmGrpHn grp)
{
    StmHn	stm;

    for (stm = STM_GRP_SFADLST(grp); stm != NULL; stm = STM_SFADLST(stm)) {
	GFS_Seek(STM_GFS(stm), 0, GFS_SEEK_SET);
	STMTRN_ResetTrBuf(stm);
    }
}


/*************************************************
 * ���[�v�J�n�X�g���[���̎w��(7.5)
 * grp	: �X�g���[���O���[�v�n���h��
 * stm	: ���[�v�J�n�X�g���[��
 * loopmax : ���[�v��(������̏ꍇSTM_LOOP_ENDLESS)
 *************************************************/
STM_LOCAL Bool	stmsvr_setLoop(StmGrpHn grp, StmHn stm, Sint32 loopmax)
{
    Bool	ret = TRUE;

    if (STM_GRP_ACSTAT(grp) == STM_EXEC_COMPLETED) {
	stm_resetTrbuf(grp);
    }
    if (stm == STM_LOOP_DFL) {
	/* �ǂݍ��݂̏I���Ă��Ȃ��擪�̃X�g���[�����T�[�`����	*/
	for (stm = STM_GRP_SFADLST(grp); stm != NULL; stm = STM_SFADLST(stm)) {
	    if (IS_STM_READEND(stm) == FALSE) {
		break;
	    }
	}
	STM_GRP_LOOPSTM(grp) = stm;
	CLR_LOOPSPECIFY(grp);
    } else if ((stm == NULL)||(IS_STM_RESI(stm) == FALSE)) {
	if (stm != NULL) {
	    stm = STM_ALIAS(stm);
	}
	STM_GRP_LOOPSTM(grp) = stm;
	SET_LOOPSPECIFY(grp);
    } else {
	ret = FALSE;
    }
    STM_GRP_LOOPMAX(grp) = loopmax;
    STM_GRP_LOOPCNT(grp) = 0;
    return (ret);
}


Bool	STM_SetLoop(StmGrpHn grp, StmHn stm, Sint32 loopmax)
{
    Bool	ret;

    STMERR_SetFunc(STM_SETLOOP);
    if (grp == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (FALSE);
    }
    if (loopmax < 0) {
	STMERR_SetCode(STM_ERR_NUM);
	STMERR_Exec();
	return (FALSE);
    }
    ret = stmsvr_setLoop(grp, stm, loopmax);
    return (ret);
}


/*************************************************
 * �ǂݍ��݃t���O�̐ݒ�
 * grp	: �X�g���[���O���[�v�n���h��
 *************************************************/
STM_LOCAL void	stm_setReadTrue(StmGrpHn grp)
{
    Sint32	fad;
    StmHn	stm;
    Sint32	sfad, efad;

    fad = STM_GRP_FAD(grp);
    for (stm = STM_GRP_SFADLST(grp); stm != NULL; stm = STM_SFADLST(stm)) {
	/* �X�g���[���̊J�nFAD���擾	*/
	STL_GetPlayArea(STM_FLOW(stm), &sfad, &efad);
	/* �J�n�Z�N�^�����݈ʒu�ȍ~,�܂��͏풓�X�g���[���̏ꍇ    */
	if (((sfad >= fad)&&(IS_STM_READEND(stm) == FALSE))||
	    (IS_STM_RESI(stm) == TRUE)) {
	    STM_ATR(stm) |= STM_ATR_READ;
	    GFS_FILE_ASTAT(STM_GFS(stm)) = GFS_SVR_BUSY;
	}
    }
}



/*************************************************
 * �ǂݍ��ݏI���t���O�̐ݒ�
 * grp	: �X�g���[���O���[�v�n���h��
 *************************************************/
void	STMSVR_SetReadEnd(StmGrpHn grp)
{
    Sint32	fad;
    StmHn	stm;
    Sint32	sfad, efad;
    Sint32	nsct;
    Sint32	loopmax, loopcnt;

    fad = STM_GRP_FAD(grp);
    for (stm = STM_GRP_FLTLST(grp); stm != NULL; stm = STM_FLTLST(stm)) {
	/* �X�g���[���̊J�nFAD���擾	*/
	STL_GetPlayArea(STM_FLOW(stm), &sfad, &efad);
	/* �I���Z�N�^�����݈ʒu�ȑO�ł��łɓǂݍ��܂�Ă���ꍇ	*/
	if ((efad < fad)&&(IS_STM_READ(stm) == TRUE)&&
	    (IS_STM_LOOPNOREAD(stm) == TRUE)) {
	    STM_ATR(stm) |= STM_ATR_READEND;
	    STMMNG_RemoveFlt(stm);
	    if (STM_GRP_LOOPSTM(grp) == stm) {
		loopmax = STM_GRP_LOOPMAX(grp);
		loopcnt = STM_GRP_LOOPCNT(grp);
		stmsvr_setLoop(grp, STM_LOOP_DFL, loopmax);
		STM_GRP_LOOPCNT(grp) = loopcnt;
	    }
	    if (STM_GRP_LSTSTM(grp) == stm) {
		STMMNG_ResetLstStm(grp);
	    }
	}
    }
}


/*************************************************
 * �Đ��͈͂̃`�F�b�N
 * grp	: �X�g���[���O���[�v
 * fad	: ���݂̍Đ�FAD
 * �߂�l: ��  �ړ���FAD
 *	   ��  �ړ��s�p
 *************************************************/
STM_LOCAL Sint32	stm_checkArea(StmGrpHn grp, Sint32 fad)
{
    StmHn	stm;
    Sint32	sfad, efad;

    if (fad < STM_GRP_FAD(grp)) {
	fad = STM_GRP_FAD(grp);
    }
    for (stm = STM_GRP_SFADLST(grp); stm != NULL; stm = STM_SFADLST(stm)) {
	if (IS_STM_READEND(stm) == FALSE) {
	    STL_GetPlayArea(STM_FLOW(stm), &sfad, &efad);
	    /* �Đ��ʒu���L���͈͓��̏ꍇ		*/
	    if ((sfad <= fad)&&(efad >= fad)) {
		break;
	    }
	    /* ���̃X�g���[���̊J�n�ʒu�Ɉړ�����	*/
	    if (sfad > fad) {
		return (sfad);
	    }
	}
    }
    return (-1);			/* �ړ�����K�v�Ȃ�		*/
}

	

/*************************************************
 * ���[�v�X�g���[����FAD�̎擾
 * grp	: �X�g���[���O���[�v
 *************************************************/
STM_LOCAL Sint32	stm_getLoopFad(StmGrpHn grp)
{
    Sint32	sfad, efad;

    /* ���[�v�X�g���[���̊J�nFAD���擾	*/
    STL_GetPlayArea(STM_FLOW(STM_GRP_LOOPSTM(grp)), &sfad, &efad);
    return (sfad);
}



/*************************************************
 * �]���t���O�̃��Z�b�g
 * grp	: �X�g���[���O���[�v
 *************************************************/
STM_LOCAL void	stm_resetTrflag(StmGrpHn grp)
{
    StmHn	stm;

    for (stm = STM_GRP_FLTLST(grp); stm != NULL; stm = STM_FLTLST(stm)) {
	STM_ATR(stm) &= ~STM_ATR_TRSTART;
	STM_ATR(stm) &= ~STM_ATR_TREND;
    }
}



/*************************************************
 * �Đ��p�����[�^�̎擾
 * grp	: �X�g���[���O���[�v
 * fad	: ���݂�FAD
 * efad	: �X�g���[���O���[�v�̏I��FAD
 * seekpos : �s�b�N�A�b�v�̈ړ���
 * start : �Đ����J�n���邩�ǂ���
 * �߂�l : TRUE �ǂݍ��ݏI��  FALSE �ǂݍ��ݒ�
 *************************************************/
STM_LOCAL Bool	stm_getReadParam(StmGrpHn grp, Sint32 fad, Sint32 efad, Sint32 stat,
			                         Sint32 *seekpos, Bool *start)
{
    Sint32	ret = FALSE;
    Uint16	hirq;
    Sint32	cdstat;

    if (STM_GRP_LOOPCNT(grp) == STM_GRP_LOOPMAX(grp)) {
	if (STM_GRP_FAD(grp) > efad) {
	    /* �S�Ă̓ǂݍ��݂��I�����Ă���ꍇ		*/
	    *seekpos = -1;
	    *start = FALSE;
	    ret = TRUE;
	    return (ret);
	} else if (stat == GFS_STAT_NOACT) {
	    /* �Đ��͈͂��z������,�X�g���[�����ǉ����ꂽ�ꍇ	*/
	    cdstat = GFCF_GetStat(NULL, &hirq);
	    stm_setErr(cdstat);			/* �G���[�R�[�h�̐ݒ�	*/
	    /* �Đ����I�����Ă���ꍇ		*/
	    if (fad > MNG_EFAD(stm_mng_ptr)) {
		STM_GRP_FAD(grp) = STL_GetCurFad();
		STM_GRP_LOOPCNT(grp)--;
	    }
	}
    } else {
	if ((STM_GRP_FAD(grp) > efad)&&
	    (IS_PLAYING(grp) == FALSE)) {
	    if (STM_GRP_LOOPSTM(grp) == NULL) {
		/* �S�Ă̓ǂݍ��݂��I�����Ă���ꍇ		*/
		*seekpos = -1;
		*start = FALSE;
		ret = TRUE;
		return (ret);
	    }
	    STM_GRP_FAD(grp) = stm_getLoopFad(grp);
	    stm_resetTrflag(grp);
	}
    }
    /* �ǂݍ��݂��J�n���Ă��Ȃ��ꍇ	*/
    if (IS_PLAYING(grp) == FALSE) {
	if (STM_GRP_PUID(grp) >= 0) {
	    /* �s�b�N�A�b�v���g�p�ł���ꍇ	*/
	    *seekpos = stm_checkArea(grp, STM_GRP_FAD(grp));
	    if (*seekpos >= 0) {
		STM_GRP_FAD(grp) = *seekpos;
	    }

	    *start = TRUE;			/* �ǂݍ��݂��J�n����	*/
	    STM_GRP_ACSTAT(grp) = STM_EXEC_DOING;
	    *seekpos = -1;
	} else {
	    /* �t�@�C���V�X�e���g�p���ɌĂ΂ꂽ�ꍇ�Ȃ�	*/
	    *seekpos = -1;
	    *start = FALSE;			/* �ǂݍ��݂��J�n���Ȃ�	*/
	}
    } else if (IS_READING(grp) == TRUE) {
	if (fad <= efad) {
	    cdstat = GFCF_GetStat(NULL, &hirq);
	    stm_setErr(cdstat);			/* �G���[�R�[�h�̐ݒ�	*/
	    /* �Đ����I�����Ă���ꍇ	*/
	    if ((stat == GFS_STAT_NOACT)&&(fad > MNG_EFAD(stm_mng_ptr))) {
		/* ����Đ��J�n������		*/
		STM_GRP_FAD(grp) = fad;
		/* �Đ��I��,�ǂݍ��ݏI��	*/
		STM_GRP_ATR(grp) &= ~(STMGRP_ATR_PLAY|STMGRP_ATR_READ|STMGRP_ATR_SEEK);
		*seekpos = -1;
	    } else {
		/* �ǂݍ��ݒ�		*/
		STM_GRP_FAD(grp) = fad;
		if (IS_SEEKING(grp) == FALSE) {
		    *seekpos = stm_checkArea(grp, fad);
		    if (*seekpos >= 0) {
			STM_GRP_ATR(grp) |= STMGRP_ATR_SEEK;
		    }
		} else {
		    if (stat != GFS_STAT_SEEK) {
			STM_GRP_ATR(grp) &= ~STMGRP_ATR_SEEK;
		    }
		    *seekpos = -1;
		}
	    }
	    *start = FALSE;			/* �ǂݍ��݂��J�n���Ȃ�	*/
	} else if (stat == GFS_STAT_NOACT) {
	    /* �ǂݍ��ݏI��		*/
	    STM_GRP_FAD(grp) = fad;
	    /* �Đ��I��,�ǂݍ��ݏI��	*/
	    STM_GRP_ATR(grp) &= ~(STMGRP_ATR_PLAY|STMGRP_ATR_READ|STMGRP_ATR_SEEK);
	    STMSVR_SetReadEnd(grp);		/* �ǂݍ��ݍς݃t���O�̐ݒ�	*/
	    STM_GRP_LOOPCNT(grp)++;
	    *start = FALSE;
	    *seekpos = -1;
	    if ((STM_GRP_LOOPSTM(grp) == NULL)||
		(STM_GRP_LOOPCNT(grp) >= STM_GRP_LOOPMAX(grp))){
		ret = TRUE;
	    }
	} else {
	    /* �ǂݍ��ݏI����̃r�W�[���	*/
	    *seekpos = -1;
	    *start = FALSE;
	}
    } else {	/* �Đ��J�n��̃r�W�[���	*/
	*seekpos = -1;
	*start = FALSE;
    }
    return (ret);
}



/*************************************************
 * �Đ��R�}���h�̔��s
 * grp	: �X�g���[���O���[�v
 * seekpos : �s�b�N�A�b�v�̈ړ���
 * efad : �X�g���[���O���[�v�̍ŏIFAD
 * start : �Đ����J�n���邩�ǂ���
 *************************************************/
STM_LOCAL Sint32	stm_putPlayCmd(StmGrpHn grp, Sint32 seekpos, Sint32 efad, Bool start)
{
    Sint32	ret = STM_ERR_OK;

    if (seekpos >= 0) {			/* ���̃X�g���[���Ɉړ�	*/
	ret = STL_MovePlaypos(grp, seekpos);
	STM_GRP_FAD(grp) = seekpos;		/* ���݂�FAD		*/
    } else if (STM_GRP_FAD(grp) <= efad){
	/* �܂��ǂݍ��ݏ�ԂɂȂ��Ă��Ȃ��ꍇ	*/
	if ((start == TRUE)&&(IS_PLAYING(grp) == FALSE)) {
	    stm_setReadTrue(grp);		/* �ǂݍ��݃t���O�̐ݒ� */
	    ret = STL_FlowIn(grp);		/* �ǂݍ��݊J�n		*/
	    if (ret == STM_ERR_OK) {
		STM_GRP_ATR(grp) |= STMGRP_ATR_PLAY;
	    }
	}
    }
    return (ret);
}



/*************************************************
 * �X�g���[���ǂݍ���
 * grp	: �Đ��X�g���[���O���[�v
 * fad	: ���݂�FAD
 * efad : �X�g���[���O���[�v�̍ŏIFAD
 * stat	: CD�u���b�N�̏��
 * �߂�l : TRUE �ǂݍ��ݏI��    FALSE �ǂݍ��ݒ�
 *************************************************/
STM_LOCAL Bool	stm_read(StmGrpHn grp, Sint32 fad, Sint32 efad, Sint32 stat)
{
    Sint32	seekpos;
    Bool	start;
    Bool	ret;
    Sint32	err;

    /* �S�Ă̓ǂݍ��݂��I�������ꍇ			*/
    if (STM_GRP_FLTLST(grp) == NULL) {
	/* �S�ď풓�X�g���[���̏ꍇ�A�]���̂ݎ��s����	*/
	if (((STM_GRP_ACSTAT(grp) == STM_EXEC_PAUSE)||
	    (STM_GRP_ACSTAT(grp) == STM_EXEC_COMPLETED))&&
	    (STM_GRP_SFADLST(grp) != NULL)){
	    stm_setReadTrue(grp);
	    STM_GRP_FAD(grp) = STM_FAD_CDEND;
	    STM_GRP_ACSTAT(grp) = STM_EXEC_DOING;
	}
	return (TRUE);
    }

    /* �Đ��p�����[�^�̎擾				*/
    ret = stm_getReadParam(grp, fad, efad, stat, &seekpos, &start);

    /* �Đ��R�}���h�̔��s				*/
    err = stm_putPlayCmd(grp, seekpos, efad, start);
    if (err != STM_ERR_OK) {
	STMERR_SetCode(err);
    }
    return (ret);
}



/*************************************************
 * CD�o�b�t�@���Z�N�^���̎擾(5.1)
 * stm	: �X�g���[���n���h��
 * �߂�l: CD�o�b�t�@���̃Z�N�^��
 *************************************************/
Sint32	STMSVR_GetNumCdbuf(StmHn stm)
{
    Sint32	nsct;
    Sint32	ret;
    
    if (STM_IS_CDFILE(stm)) {
	ret = GFCD_GetLenData(GFS_CDR_BUFNO(STM_DTSRC(stm)), 0, 
			      LONG_MAX, &nsct, NULL);
	if (ret != GFCD_ERR_OK) {
	    STM_SetGfcdErr(ret);
	    return 0;
	}
	ret = GFCD_WaitServer();
	if (ret != GFCD_ERR_OK) {
	    STM_SetGfcdErr(ret);
	    return 0;
	}
    } else {
	nsct = GFBF_GetNumData(STM_GFS(stm));
    }
    return (nsct);
}


Sint32	STM_GetNumCdbuf(StmHn stm)
{
    Sint32	nsct;
    Sint32	ret;
    
    STMERR_SetFunc(STM_GETNUMCDBUF);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (-1);
    }
    stm = STM_ALIAS(stm);
    nsct = STMSVR_GetNumCdbuf(stm);
    return (nsct);
}


/*************************************************
 * �X�g���[����Ԃ̎擾
 * stm	: �X�g���[���n���h��
 * sfad	: �X�g���[���̊J�nFAD
 * efad	: �X�g���[���̏I��FAD
 * nsct : CD�o�b�t�@�̃Z�N�^��
 * �߂�l: STAT_NOREAD		�A�N�Z�X����Ȃ�
 *	   STAT_DOING		������
 *	   STAT_WAIT		�]���҂�(�Q�[�g��, �̈�t��)
 *	   STAT_COMPLETED	�A�N�Z�X����
 *************************************************/
STM_LOCAL Sint32	stm_getStat(StmHn stm, Sint32 sfad, Sint32 efad, Sint32 nsct)
{
    Sint32	fad;
    Sint32	trfad;
    Uint16	hirq;
    Sint32	stat;

    fad = STM_GRP_FAD(STM_GRPHN(stm));
    if ((IS_STM_READEND(stm) == TRUE)&&(nsct == 0)) {
	/* �A�N�Z�X�����������X�g���[�� */
	return (STAT_COMPLETED);
    }
    if ((fad <= efad)&&(STM_IS_CDFILE(stm))) {		/* �ǂݍ��ݒ�	*/
	if ((IS_STM_READ(stm) == TRUE)||
	    (IS_STM_LOOPNOREAD(stm) == FALSE)) {
	    stat = GFCF_GetStat(NULL, &hirq);
	    stm_setErr(stat);			/* �G���[�R�[�h�̐ݒ�	*/
	    if ((hirq & CDC_HIRQ_BFUL) == 0) {
		/* CD�o�b�t�@�ɋ󂫗̈悠��	*/
		return (STAT_DOING);
	    } else if (nsct == 0) {		/* CD�o�b�t�@�̃f�[�^�͑S�đ���	*/
		return (STAT_WAIT);		/* �X�g���[���̃f�[�^		*/
	    }
	} else {
	    return (STAT_NOREAD);
	}
    } else if ((IS_STM_READ(stm) == TRUE)&&(nsct == 0)) {
	/* �ǂݍ��݊�����,CD�o�b�t�@�Ƀf�[�^���Ȃ�	*/
	return (STAT_COMPLETED);	/* �]������			*/
    }
    if (STM_TRFAD(stm) == STM_FAD_CDEND) {
	trfad = STMMNG_GetLstFad(STM_GRPHN(stm));
    } else {
	trfad = STM_TRFAD(stm);
    }
    /* �]���Q�[�g�����Ă���,�]��FAD����,�o�b�t�@�t��	*/
    if ((!GFS_TRN_ACTIVE(&(GFS_FILE_TRANS(STM_GFS(stm)))))||
	((fad >= sfad)&&(fad < trfad))||
	(STMTRN_IsTrBufFull(stm) == TRUE)) {
	return (STAT_WAIT);
    }
    return (STAT_DOING);			/* �]����			*/
}



/*************************************************
 * �X�g���[���f�[�^�̓]��
 * stm	: �X�g���[���n���h��
 * �߂�l : TRUE  �]������
 *	    FALSE �]���ł��Ȃ�����
 *************************************************/
STM_LOCAL Bool	stmsvr_execTrans(StmHn stm)
{
    GfsHn	gfs;
    Bool	trans;

    gfs = STM_GFS(stm);
    if (((STM_TRFUNC(stm) != STM_TR_NULLFUNC)||
	(GFS_TRN_BUF(&GFS_FILE_TRANS(gfs)) != NULL))&&
	(GFTR_GetLenRoom(&GFS_FILE_TRANS(gfs)) > 0)) {
	GFTR_Exec(gfs, &trans);
    } else {
	trans = FALSE;
    }
    return (trans);
}



/*************************************************
 * �o�b�t�@�T�C�Y�Ď��֐��̎��s
 * grp	: �X�g���[���O���[�v�n���h��
 *************************************************/
STM_LOCAL void	stm_execCdbufFunc(StmGrpHn grp)
{

    if ((STM_GRP_CHKFUNC(grp) != STM_CDBUF_NULLFUNC)&&
	((STM_GRP_ATR(grp) & STMGRP_ATR_CHKBUF) != 0)) {
	STM_GRP_CHKFUNC(grp)(STM_GRP_CHKOBJ(grp));
	STM_GRP_ATR(grp) &= ~STMGRP_ATR_CHKBUF;
    }
}
    

/*************************************************
 * �f�[�^�̓]��
 * grp	: �X�g���[���O���[�v
 * first : �ŏ��ɓ]�������X�g���[��
 * cdstat : CD�̃X�e�[�^�X
 * bflag  : ���̃X�g���[���̓]�����s�Ȃ�Ȃ��ꍇ  TRUE
 * stat	  : ���ɓ]������X�g���[���̃X�e�[�^�X
 * �߂�l : �S�X�g���[���̓]�������������ꍇ TRUE
 *************************************************/
STM_LOCAL Bool	stm_trans_sub(StmGrpHn grp, StmHn first, Uint8 cdstat, 
		                                Bool *bflag, Sint32 *stat)
{
    Sint32	i;
    StmHn	stm, _stm;
    Sint32	nsct;
    Sint32	sfad, efad;
    Sint32	trfad;
    Bool	req;
    Bool	complete = TRUE;
    Bool	set = FALSE;
    Uint8	atr;

    *bflag = FALSE;
    stm = STM_GRP_NEXTTRNS(grp);		/* �]���X�g���[��	*/
    if (STM_IS_CDFILE(stm)) {
	STL_GetPlayArea(STM_FLOW(stm), &sfad, &efad);
    } else {
	sfad = efad = STM_FAD_CDTOP;
    }
    atr = STM_ATR(stm);
    if (STM_TRFAD(stm) == STM_FAD_CDEND) {
	trfad = STMMNG_GetLstFad(grp);
    } else {
	trfad = STM_TRFAD(stm);
    }
    if (((atr & STM_ATR_TRSTART) == 0)&&(STM_GRP_FAD(grp) > sfad)&&
	(STM_GRP_FAD(grp) >= trfad)) {
	STM_ATR(stm) |= STM_ATR_TRSTART;
    }
    if (((atr & STM_ATR_TREND) == 0)&&(STM_GRP_FAD(grp) > efad)&&
	(STM_GRP_FAD(grp) >= trfad)) {
	STM_ATR(stm) |= STM_ATR_TREND;
    }
    if (((atr & STM_ATR_TREND) == 0)&&((STM_ATR(stm) & STM_ATR_TREND) != 0)) {
	/* �Đ��͈͂��z������CD�o�b�t�@�ɂ��镪������]������	*/
	nsct = GFBF_GetNumData(STM_GFS(stm));
	GFTR_SetTransPara(&(GFS_FILE_TRANS(STM_GFS(stm))), nsct);
    } else if (((atr & STM_ATR_TRSTART) == 0)&&((STM_ATR(stm) & STM_ATR_TRSTART) != 0)) {
	/* �Đ��͈͂ɂȂ����炷�ׂẴf�[�^��]������		*/
	GFTR_SetTransPara(&(GFS_FILE_TRANS(STM_GFS(stm))), LONG_MAX);
    }
    stmsvr_execTrans(stm);			/* �]�����s		*/
    if (stmsvr_isTrans(stm) == TRUE) {
	/* �]�����̏ꍇ		*/
	*bflag = TRUE;
	*stat = stm_getStat(stm, sfad, efad, 1);
	                                        /* �]�����̓f�[�^������	*/
	return (FALSE);
    }
    /* ���̓]���X�g���[���̐ݒ�		*/
    _stm = stm;
    for (i = 0; (i < 2)&&(set != TRUE); i++) {
	for (stm = STM_SFADLST(stm); ; stm = STM_SFADLST(stm)) {
	    /* �Ō�Ȃ�擪�ɖ߂�	*/
	    if (stm == NULL) {
		stm = STM_GRP_SFADLST(grp);
	    }
	    if (stm == first) {
		*bflag = TRUE;
	    }
	    nsct = GFBF_GetNumData(STM_GFS(stm));
	    if (STM_IS_CDFILE(stm)) {
		STL_GetPlayArea(STM_FLOW(stm), &sfad, &efad);
	    } else {
		sfad = efad = STM_FAD_CDTOP;
	    }
	    *stat = stm_getStat(stm, sfad, efad, nsct);
	    /* 1��ڂ̓A�N�Z�X���̃X�g���[����T��	*/
	    /* 2��ڂ͓]���҂��̃X�g���[����T��	*/
	    if (((nsct > 0)||(cdstat == GFS_STAT_SEEK)||(cdstat == GFS_STAT_READ))&&
		(((i == 0)&&(*stat == STAT_DOING))||
		 ((i == 1)&&(*stat == STAT_WAIT)))) {
		complete = FALSE;
		set = TRUE;
		break;
	    }
	    if ((*stat == STAT_DOING)||(*stat == STAT_WAIT)) {
		complete = FALSE;
	    }
	    if (stm == _stm) {
		break;
	    }
	}
    }
    STM_GRP_NEXTTRNS(grp) = stm;
    if ((set == FALSE)&&(complete == FALSE)) {
	*stat = STAT_DOING;
    }
    return (complete);
}


/*************************************************
 * �f�[�^�̓]��
 * grp	: �X�g���[���O���[�v
 * cdstat : CD�̃X�e�[�^�X
 * �߂�l : TRUE �]���I��  FALSE �]����
 *************************************************/
STM_LOCAL Bool	stm_trans(StmGrpHn grp, Uint8 cdstat)
{
    StmHn	stm, first;
    Sint32	stat;
    Bool	bflag;
    Bool	ret;

    /* �]���X�g���[�������ݒ�̏ꍇ		*/
    if (STM_GRP_NEXTTRNS(grp) == NULL) {
	STM_GRP_NEXTTRNS(grp) = STM_GRP_SFADLST(grp);
    }
    first = STM_GRP_NEXTTRNS(grp);

    /* �e�X�g���[�����P�x�Â��s�����݂�	*/
    /* �]�����܂��͂P���܂�����炨���		*/
    do {
	stm = STM_GRP_NEXTTRNS(grp);
	ret = stm_trans_sub(grp, first, cdstat, &bflag, &stat);
    } while(bflag == FALSE);

    /* WAIT <-> DOING�̈ڍs	*/
    if ((stat == STM_EXEC_WAIT)||(STM_GRP_ACSTAT(grp) == STM_EXEC_WAIT)) {
	STM_GRP_ACSTAT(grp) = stat;
    } else if (STM_GRP_FLTLST(grp) == NULL) {
	/* �풓�݂̂̏ꍇ�S�Ă̓]�����I�������烋�[�v�������Ƃɂ���	*/
	STM_GRP_ACSTAT(grp) = stat;
	if (stat == STM_EXEC_COMPLETED) {
	    STM_GRP_LOOPCNT(grp)++;
	    if (STM_GRP_LOOPCNT(grp) < STM_GRP_LOOPMAX(grp)) {
		STM_GRP_FAD(grp) = STM_FAD_CDTOP;
	    }
	}
    }
    return (ret);
}

    

/*************************************************
 * �A�N�Z�X�|�C���^�̏�����
 * grp	: �X�g���[���O���[�v�n���h��
 *************************************************/
STM_LOCAL void	stm_resetAcp(StmGrpHn grp)
{
    StmHn	stm;

    for (stm = STM_GRP_SFADLST(grp); stm != NULL; stm = STM_SFADLST(stm)) {
	GFS_Seek(STM_GFS(stm), 0, GFS_SEEK_SET);
    }
}


/*************************************************
 * �T�[�o�̎��s(7.3)
 * �߂�l: STM_EXEC_COMPLETED	�A�N�Z�X�I��
 *	   STM_EXEC_PAUSE	�ꎞ��~
 *	   STM_EXEC_DOING	�A�N�Z�X��
 *	   STM_EXEC_WAIT	�]���҂����
 *************************************************/
Sint32	STM_ExecServer(void)
{
    StmGrpHn	grp;
    Sint32	fad, efad;
    Sint32	ret;
    Bool	read, trans;
    Bool	complete;
    Uint16	hirq;
    Sint32	stat;

    /* �������̃^�X�N������ꍇ	*/
    if (CMD_FUNC(&stm_cmd_area) != CMD_NULLFUNC) {
	complete = CMD_FUNC(&stm_cmd_area)(CMD_OBJ(&stm_cmd_area));
	if (complete == TRUE) {
	    CMD_FUNC(&stm_cmd_area) = CMD_NULLFUNC;
	    return (STM_EXEC_TSKEND);
	}
	return (STM_ERR_WHERE(&MNG_ERR(stm_mng_ptr)));
    }

    /* �X�g���[���A�N�Z�X	*/
    STMERR_SetFunc(STM_EXECSERVER);
    grp = MNG_CURGRP(stm_mng_ptr);
    /* �������̃X�g���[��������ꍇ	*/
    if ((grp != NULL)&&(STM_GRP_SFADLST(grp) != NULL)) {
	if (MNG_SVREXEC(stm_mng_ptr) == FALSE) {
	    MNG_SVREXEC(stm_mng_ptr) = TRUE;
	    /* �X�g���[���A�N�Z�X����	*/
	    stm_execCdbufFunc(grp);		/* �o�b�t�@�T�C�Y�Ď��֐��̎��s	*/
	    stat = GFCF_GetStat(&fad, &hirq);	/* ���݂�FAD�擾	*/
	    stm_setErr(stat);			/* �G���[�R�[�h�̐ݒ�	*/
	    if (IS_SET_ERRCODE(stm_mng_ptr) == TRUE) {
		STMERR_Exec();
	    } else {
		efad = STMMNG_GetLstFad(grp);		/* �ŏIFAD�̎擾	*/
		read = stm_read(grp, fad, efad, stat);	/* �X�g���[���̓ǂݍ���	*/
		trans = stm_trans(grp, stat);		/* �X�g���[���̎��o��	*/
		if ((read == TRUE)&&(trans == TRUE)) {
		    STM_GRP_ACSTAT(grp) = STM_EXEC_COMPLETED;
		    stm_resetAcp(grp);
		}
	    }
	    MNG_SVREXEC(stm_mng_ptr) = FALSE;
	}
	ret = STM_GRP_ACSTAT(grp);
    } else {				/* �������̃X�g���[�����Ȃ��ꍇ	*/
	ret = STM_EXEC_PAUSE;
    }
    return (ret);
}



/*************************************************
 * �s�b�N�A�b�v�̈ړ�(7.4)
 * stm	: �X�g���[���n���h��
 * off	: �X�g���[���擪����̃I�t�Z�b�g
 *************************************************/
void	STM_MovePickup(StmHn stm, Sint32 off)
{
    StmGrpHn	grp;
    Sint32	sfad, efad;
    Sint32	ret;
    Uint16	hirq;
    Sint32	stat;

    STMERR_SetFunc(STM_MOVEPICKUP);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return ;
    }
    stm = STM_ALIAS(stm);
    grp = STM_GRPHN(stm);
    if (MNG_CURGRP(stm_mng_ptr) == grp) {		/* �������O���[�v�̏ꍇ	*/
	if (IS_PLAYING(grp) == TRUE) {
	    ret = STL_StopIn(STM_GRPHN(stm));	/* �ǂݍ��ݒ�~		*/
	    if (ret != STM_ERR_OK) {
		STMERR_SetCode(ret);
	    }
	    STM_GRP_ACSTAT(grp) = STM_EXEC_PAUSE;	/* ����T�[�o�ɂĈړ�	*/
	}
	stat = GFCF_GetStat(&sfad, &hirq);	/* ���݂�FAD���擾	*/
	stm_setErr(stat);			/* �G���[�R�[�h�̐ݒ�	*/
	if (IS_SET_ERRCODE(stm_mng_ptr) == TRUE) {
	    STMERR_Exec();
	    return;
	}
	if (sfad >= 0) {
	    STM_GRP_FAD(grp) = sfad;		/* ���݈ʒu��ݒ�	*/
	}
	/* �Đ��I��,�ǂݍ��ݏI��	*/
	STM_GRP_ATR(grp) &= ~(STMGRP_ATR_PLAY|STMGRP_ATR_READ|STMGRP_ATR_SEEK);
    }
    STMSVR_SetReadEnd(grp);	/* �ǂݍ��ݍς݃t���O�̐ݒ�		*/
    stm_setReadFalse(grp);	/* �ǂݍ��݈ʒu�ȍ~�̃t���O��FALSE�ɐݒ�  */

    /* �ړ���FAD���擾	*/
    STL_GetPlayArea(STM_FLOW(stm), &sfad, &efad);

    STM_GRP_FAD(grp) = sfad + off;
}


/*************************************************
 * ���[�h�G���[�������̃��J�o��(9.3)
 * �߂�l : STM_ERR_CDRD  ���J�o�����s
 *	    STM_ERR_OK	  ���J�o������
 *************************************************/
Sint32	STM_Recover(void)
{
    Sint32	ret;

    ret = STL_Recover(MNG_CURGRP(stm_mng_ptr));
    return (ret);
}


/*************************************************
 * �A�N�Z�X�I���̃`�F�b�N(7.8)
 * stm	: �X�g���[���n���h��
 * �߂�l : TRUE  �X�g���[���A�N�Z�X���I������
 *	    FALSE �X�g���[���A�N�Z�X���I�����Ă��Ȃ�
 *************************************************/
Bool	STM_IsComplete(StmHn stm)
{
    Sint32	nsct;
    Sint32	stat;
    Sint32	sfad, efad;
    Uint16	hirq;

    STMERR_SetFunc(STM_ISCOMPLETE);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (FALSE);
    }
    stm = STM_ALIAS(stm);
    nsct = STMSVR_GetNumCdbuf(stm);		/* �o�b�t�@�Z�N�^��	*/
    STL_GetPlayArea(STM_FLOW(stm), &sfad, &efad);
    stat = stm_getStat(stm, sfad, efad, nsct);		/* �X�g���[���X�e�[�^�X	*/
    if (stat == STAT_COMPLETED) {		/* �A�N�Z�X�����̏ꍇ	*/
	return (TRUE);
    }
    if ((stat == STAT_WAIT)&&(!GFS_TRN_ACTIVE(&(GFS_FILE_TRANS(STM_GFS(stm)))))) {
	                                      /* �E�G�C�g��ԁA�Q�[�g��	*/
	return (TRUE);
    }
    return (FALSE);
}


/*************************************************
 * �X�g���[���f�[�^�̓]��(7.9)
 * stm	: �X�g���[���n���h��
 * �߂�l : TRUE  �]������
 *	    FALSE �]���ł��Ȃ�����
 *************************************************/
Bool	STM_ExecTrans(StmHn stm)
{

    GfsHn	gfs;
    Bool	trans;

    STMERR_SetFunc(STM_EXECTRANS);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (FALSE);
    }
    stm = STM_ALIAS(stm);
    trans = stmsvr_execTrans(stm);
    return (trans);
}



/* end of stm_svr.c	*/







