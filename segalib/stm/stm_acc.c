/*****************************************************************************
 *	�\�t�g�E�F�A���C�u����
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library: �X�g���[���V�X�e��
 * Module : ���̑��̊֐�
 * File	  : stm_acc.c
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
#include	"..\gfs\gfs_def.h"
#include	"..\gfs\gfs_cdc.h"
#include	"..\gfs\gfs_cdf.h"
#include	"..\gfs\gfs_cdb.h"
#include	"..\gfs\gfs_trn.h"
#include	"..\gfs\gfs_buf.h"
#include	"..\gfs\gfs_dir.h"
#include	"stm_loc.h"
#include	"stm_def.h"

/*****************************************************************************
 *	�֐��̒�`
 *****************************************************************************/

/*************************************************
 * �X�g���[�����̎擾(2.3)
 * grp	: �X�g���[���O���[�v�n���h��
 * �߂�l : �X�g���[����
 *************************************************/
Sint32	STM_GetStmNum(StmGrpHn grp)
{
    Sint32	nstm = 0;
    StmHn	stm;

    STMERR_SetFunc(STM_GETSTMNUM);
    if (grp == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (-1);
    }
    /* �X�g���[���̃��X�g��������	*/
    for (stm = STM_GRP_SFADLST(grp); stm != NULL; stm = STM_SFADLST(stm)) {
	nstm++;
    }
    return (nstm);
}


/*************************************************
 * �X�g���[���n���h���̎擾(2.4)
 * grp	: �X�g���[���O���[�v�n���h��
 * nstm	: �Đ����ԍ�(0 <= nstm < STM_GetStmNum)
 * �߂�l : �w�肵���Đ����̃X�g���[���n���h��
 *************************************************/
StmHn	STM_GetStmHndl(StmGrpHn grp, Sint32 nstm)
{
    Sint32	i;
    StmHn	stm = NULL;

    STMERR_SetFunc(STM_GETSTMHNDL);
    if (grp == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (NULL);
    }
    if (nstm < 0) {
	STMERR_SetCode(STM_ERR_NUM);
	STMERR_Exec();
	return (NULL);
    }
    for (i = 0, stm = STM_GRP_SFADLST(grp); (i < nstm)&&(stm != NULL);
	                                        stm = STM_SFADLST(stm)) {
	i++;
    }
    return (stm);
}


/*************************************************
 * �o�b�t�@�T�C�Y�Ď��֐��̓o�^(2.5)
 * grp	: �X�g���[���O���[�v�n���h��
 * stm	: �X�g���[���n���h��(�󂫃Z�N�^�`�F�b�N�̏ꍇSTM_CDBUF_REST)
 * nsct	: �󂫃Z�N�^��
 * func	: �Ăт����֐��i�������ꍇ STM_CDBUF_NULLFUNC�j
 * obj	: �Ăт����֐��̑�����
 *************************************************/
void	STM_SetCdbufFunc(StmGrpHn grp, StmHn stm,
			 Sint32 nsct, StmCdbufFunc func, void *obj)
{
    STMERR_SetFunc(STM_SETCDBUFFUNC);
    if (grp == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return ;
    }
    if (nsct < 0) {
	STMERR_SetCode(STM_ERR_NUM);
	STMERR_Exec();
	return ;
    }
    if (stm != STM_CDBUF_REST) {
	stm = STM_ALIAS(stm);
    }
    STM_GRP_CHKSTM(grp) = stm;
    STM_GRP_CHKSCTNUM(grp) = nsct;
    STM_GRP_CHKFUNC(grp) = func;
    STM_GRP_CHKOBJ(grp) = obj;
}


/*************************************************
 * �X�g���[�����̎擾(3.6)
 * stm	: �X�g���[���n���h��
 * sfid	: �t�@�C�����ʎq
 * frange : �Đ��͈�
 * bn	: �o�b�t�@���ԍ�
 * stmkey : �X�g���[���L�[
 * �߂�l : ��������X�g���[���O���[�v
 *************************************************/
StmGrpHn STM_GetInfo(StmHn stm, Sint32 *fid, StmFrange *frange,
		                             Sint32 *bn, StmKey *stmkey)
{
    Sint32	sctsiz;
    Sint32	lastsiz;
    Sint32	atr;
    Sint32	fn;
    Sint32	fmode;
    Sint32	flt;
    CdcSubh	subh;

    STMERR_SetFunc(STM_GETINFO);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (NULL);
    }
    stm = STM_ALIAS(stm);
    /* �t�@�C�����	*/
    GFCF_GetFileInfo(STM_FLOW(stm), fid, &STM_FRANGE_SFAD(frange),
	&sctsiz, &STM_FRANGE_FASNUM(frange), &lastsiz, &fn, &atr);
    /* �Đ��͈͂ɂ��I�[�v�����ꂽ�ꍇ	*/
    if (*fid == FID_OPEN_FRANGE) {
	*fid = -1;
    }
    /* �i�胂�[�h	*/
    if (STM_IS_CDFILE(stm)) {
	*bn = GFS_CDR_BUFNO(STM_DTSRC(stm));	/* �o�b�t�@���ԍ�	*/
	flt = GFS_CDR_FLT(STM_DTSRC(stm));
	GFCD_GetFilt(flt, &fmode, &subh);
	STM_KEY_FN(stmkey) = (Sint16)CDC_SUBH_FN(&subh);
	STM_KEY_CN(stmkey) = (Sint16)CDC_SUBH_CN(&subh);
	STM_KEY_SMMSK(stmkey) = (Sint16)CDC_SUBH_SMMSK(&subh);
	STM_KEY_SMVAL(stmkey) = (Sint16)CDC_SUBH_SMVAL(&subh);
	STM_KEY_CIMSK(stmkey) = (Sint16)CDC_SUBH_CIMSK(&subh);
	STM_KEY_CIVAL(stmkey) = (Sint16)CDC_SUBH_CIVAL(&subh);
    } else {
	fmode = 0;
	*bn = -1;
    }
    if ((fmode & FMODE_FN) == 0) {		/* �t�@�C���ԍ��w��Ȃ�	*/
	STM_KEY_FN(stmkey) = STM_KEY_NONE;
    }
    if ((fmode & FMODE_CN) == 0) {		/* �`���l���ԍ��w��Ȃ�	*/
	STM_KEY_CN(stmkey) = STM_KEY_NONE;
    }
    if ((fmode & FMODE_SM) == 0) {		/* �T�u���[�h�w��Ȃ�	*/
	STM_KEY_SMMSK(stmkey) = STM_KEY_SMVAL(stmkey) = STM_KEY_NONE;
    }
    if ((fmode & FMODE_CI) == 0) {		/* �R�[�f�B���O���w��Ȃ� */
	STM_KEY_CIMSK(stmkey) = STM_KEY_CIVAL(stmkey) = STM_KEY_NONE;
    }
    return (STM_GRPHN(stm));
}


/*************************************************
 * �X�g���[���̐ڑ�(8.1)
 * keystm : �ڑ����X�g���[���n���h��
 * bufstm : �ڑ���X�g���[���n���h��(�Ȃ��ꍇNULL)
 *************************************************/
void	STM_ConnectCdbuf(StmHn keystm, StmHn bufstm)
{
    Sint32	bufno;
    Sint32	ret;

    STMERR_SetFunc(STM_CONNECTCDBUF);
    if (keystm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return ;
    }
    keystm = STM_ALIAS(keystm);
    if (bufstm == NULL) {
	bufno = CDC_NUL_SEL;
    } else {
	bufstm = STM_ALIAS(bufstm);
	bufno = GFS_CDR_BUFNO(STM_DTSRC(bufstm));
    }
    GFS_CDR_BUFNO(STM_DTSRC(keystm)) = bufno;
    GFCD_SetFiltCon(GFS_CDR_FLT(STM_DTSRC(keystm)), bufno, GFCD_ANY_FLT);
    ret = GFCD_WaitServer();
    if (ret != GFCD_ERR_OK) {
	STM_SetGfcdErr(ret);
	STMERR_Exec();
    }
}


/*************************************************
 * CD�o�b�t�@�f�[�^�̈ړ�(8.2)
 * src	: �]�����X�g���[���n���h��
 * spos	: �Z�N�^�ʒu(�擪�Z�N�^��0)
 * snum : �Z�N�^��
 * dst	: �]����X�g���[���n���h��
 *************************************************/
void	STM_MoveCdbuf(StmHn src, Sint32 spos, Sint32 snum, StmHn dst)
{
    Sint32	ret;

    STMERR_SetFunc(STM_MOVECDBUF);
    if ((src == NULL)||(dst == NULL)) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return ;
    }
    if ((spos < 0)||(snum < 0)) {
	STMERR_SetCode(STM_ERR_NUM);
	STMERR_Exec();
	return ;
    }
    src = STM_ALIAS(src);
    dst = STM_ALIAS(dst);
    GFCD_MoveSctData(GFS_CDR_BUFNO(STM_DTSRC(dst)), 
		     GFS_CDR_BUFNO(STM_DTSRC(src)), spos, snum);

    /* TREND�̏ꍇ�A�]���Z�N�^�����ݒ肳��Ă���̂ň�U�N���A����	*/
    STM_ATR(dst) &= ~STM_ATR_TREND;
    ret = GFCD_WaitServer();
    if (ret != GFCD_ERR_OK) {
	STM_SetGfcdErr(ret);
	STMERR_Exec();
    }
}


/*************************************************
 * CD�o�b�t�@�f�[�^�̏���(8.3)
 * stm	: �X�g���[���n���h��
 * spos	: �����ʒu
 * snum	: �����Z�N�^��
 *************************************************/
void	STM_EraseCdbuf(StmHn stm, Sint32 spos, Sint32 snum)
{
    Sint32	ret;

    STMERR_SetFunc(STM_ERASECDBUF);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return ;
    }
    if ((spos < 0)||(snum < 0)) {
	STMERR_SetCode(STM_ERR_NUM);
	STMERR_Exec();
	return ;
    }
    stm = STM_ALIAS(stm);
    GFTR_Stop(STM_GFS(stm), 0);			/* �f�[�^�p�b�N�Ԋ�	*/
    GFS_TRN_STAT(&GFS_FILE_TRANS(STM_GFS(stm))) = GFTR_ST_NOACT;
                                      /* END ��ԂɂȂ�Ȃ��悤�ɂ���	*/
    if (STM_IS_CDFILE(stm)) {
	STL_DeleteSctData(STM_FLOW(stm), spos, snum);
                                                /* �o�b�t�@�f�[�^����	*/
	ret = GFCD_WaitServer();
	if (ret != GFCD_ERR_OK) {
	    STM_SetGfcdErr(ret);
	    STMERR_Exec();
	}
    } else {
	GFS_Seek(STM_GFS(stm), 0, GFS_SEEK_SET);
    }
}


/*************************************************
 * �Z�N�^������o�C�g���ւ̕ϊ�(3.7)
 * stm	: �X�g���[���n���h��
 * nsct	: �Z�N�^��
 * �߂�l : �o�C�g��
 *************************************************/
Sint32	STM_SctToWord(StmHn stm, Sint32 nsct)
{
    Sint32	nbyte;

    STMERR_SetFunc(STM_SCTTOWORD);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (-1);
    }
    if (nsct < 0) {
	STMERR_SetCode(STM_ERR_NUM);
	STMERR_Exec();
	return (-1);
    }
    stm = STM_ALIAS(stm);
    GFBF_GetTranData(STM_GFS(stm), nsct, &nbyte);
    return (nbyte / sizeof(Uint16));
}


/*************************************************
 * ���[�h������Z�N�^���ւ̕ϊ�(3.8)
 * stm	: �X�g���[���n���h��
 * nword: ���[�h��
 * �߂�l : �Z�N�^��
 *************************************************/
Sint32	STM_WordToSct(StmHn stm, Sint32 nword)
{
    Sint32	i;
    Sint32	nsct;
    Sint32	nw = 0;

    STMERR_SetFunc(STM_WORDTOSCT);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (-1);
    }
    if (nword < 0) {
	STMERR_SetCode(STM_ERR_NUM);
	STMERR_Exec();
	return (-1);
    }
    stm = STM_ALIAS(stm);
    if (nword == 0) {
	return (0);
    }
    if (STM_IS_CDFILE(stm)) {
	nsct = GFCB_GetLenData(STM_FLOW(stm), LONG_MAX, NULL);	
	                                              /* ���Z�N�^���擾	*/
	/* ���f�[�^�T�C�Y�擾 */
	for (i = 0; i < nsct; i++) {
	    nw += STL_GetActSiz(STM_FLOW(stm), i);
	    if (nw >= nword) {
		i++;
		break;
	    }
	}
    } else {
	for (i = 1; ; i++) {
	    nsct = GFBF_GetTranData(STM_GFS(stm), i, &nw);
	    nw /= sizeof(Uint16);
	    if (nw >= nword) {
		break;
	    }
	    if (i > nsct) {
		i--;
		break;
	    }
	}
    }
    return (i);
}


/*************************************************
 * �X�g���[���n���h���̃G�C���A�X�ݒ�(3.9)
 * alias : �G�C���A�X��ݒ肷��X�g���[���n���h��
 * stm	 : �A�N�Z�X����X�g���[���n���h��
 *************************************************/
Bool	STM_SetAlias(StmHn alias, StmHn stm)
{
    StmHn	chk;
    Sint32	i;

    STMERR_SetFunc(STM_SETALIAS);
    if (alias == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (FALSE);
    }
    /* ���ɃI�[�v������Ă���X�g���[���ł��邱�Ƃ��m�F����		*/
    if (STM_SVRSTAT(stm) == SVR_UNUSE) {
	return (FALSE);
    }
    /* ���ɃG�C���A�X���ݒ肳��Ă���X�g���[���łȂ����Ƃ��m�F����	*/
    for (i = 0, chk = MNG_STM(stm_mng_ptr, 0); i < MNG_STMMAX(stm_mng_ptr); i++, chk++) {
	if ((STM_SVRSTAT(chk) == SVR_USE)&&
	    (chk != stm)&&(STM_ALIAS(chk) == stm)) {
	    return (FALSE);
	}
    }
    STM_ALIAS(alias) = stm;
    return (TRUE);
}


/*************************************************
 * �X�g���[���n���h���̃G�C���A�X����(3.10)
 * alias : �G�C���A�X����������X�g���[���n���h��
 *************************************************/
void	STM_UnsetAlias(StmHn alias)
{
    STMERR_SetFunc(STM_UNSETALIAS);
    if (alias == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return ;
    }
    STM_ALIAS(alias) = alias;
}


/*************************************************
 * �]���Q�[�g�̊J��(4.4)
 * stm	: �X�g���[���n���h��
 * date	: �Q�[�g�̏��
 *************************************************/
void	STM_SetTrGate(StmHn stm, Sint32 gate)
{
    STMERR_SetFunc(STM_SETTRGATE);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return ;
    }
    stm = STM_ALIAS(stm);
    if (gate == STM_GATE_OPEN) {
	GFTR_Start(&(GFS_FILE_TRANS(STM_GFS(stm))));
    } else {
	GFTR_Pause(&(GFS_FILE_TRANS(STM_GFS(stm))));
    }
}


/*************************************************
 * �ő�]���Z�N�^���̐ݒ�(4.5)
 * stm	: �X�g���[���n���h��
 * tsct	: �ő�]���Z�N�^��
 *************************************************/
void	STM_SetTrPara(StmHn stm, Sint32 tsct)
{
    STMERR_SetFunc(STM_SETTRPARA);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return ;
    }
    if (tsct < 0) {
	STMERR_SetCode(STM_ERR_NUM);
	STMERR_Exec();
	return ;
    }
    stm = STM_ALIAS(stm);
    GFTR_SetTransMax(&(GFS_FILE_TRANS(STM_GFS(stm))), tsct);
}


/*************************************************
 * �]�����[�h�̐ݒ�(4.7)
 * stm	: �X�g���[���n���h��
 * tmode: �]�����[�h
 *************************************************/
void	STM_SetTrMode(StmHn stm, Sint32 tmode)
{
    STMERR_SetFunc(STM_SETTRMODE);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return ;
    }
    if ((tmode != STM_TR_SCU)&&(tmode != STM_TR_SDMA0)&&
	(tmode != STM_TR_SDMA1)&&(tmode != STM_TR_CPU)) {
	STMERR_SetCode(STM_ERR_PARA);
	STMERR_Exec();
	return ;
    }
    stm = STM_ALIAS(stm);
    GFTR_SetMode(&(GFS_FILE_TRANS(STM_GFS(stm))), tmode);
}


/*************************************************
 * �ǂݍ��݃Z�N�^���̎擾(5.2)
 * stm	: �X�g���[���n���h��
 * sn	: �Z�N�^�ԍ��i�擪�Z�N�^�͂O�j
 * sinfo: �Z�N�^���
 * �߂�l: TRUE �w��Z�N�^����  FALSE �w��Z�N�^�Ȃ�
 *************************************************/
Bool	STM_GetSctInfo(StmHn stm, Sint32 sn, StmSct *sinfo)
{
    Bool	ret;

    STMERR_SetFunc(STM_GETSCTINFO);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (FALSE);
    }
    if (sn < 0) {
	STMERR_SetCode(STM_ERR_NUM);
	STMERR_Exec();
	return (FALSE);
    }
    stm = STM_ALIAS(stm);
    if (STM_IS_CDFILE(stm)) {
	ret = STL_GetSctInfo(STM_FLOW(stm), sn, sinfo);
    } else {
	if (sn < STMSVR_GetNumCdbuf(stm)) {
	    STM_SCT_FAD(sinfo) = LONG_MAX;
	    STM_SCT_FN(sinfo) = 0;
	    STM_SCT_CN(sinfo) = 0;
	    STM_SCT_SM(sinfo) = 0;
	    STM_SCT_CI(sinfo) = 0;
	    ret = TRUE;
	} else {
	    ret = FALSE;
	}
    }
    return (ret);
}


/*************************************************
 * ���[�v�J�E���^�̎擾(7.6)
 * grp	  : �X�g���[���O���[�v�n���h��
 * �߂�l : ���[�v�J�E���^
 *************************************************/
Sint32	STM_GetLoopCnt(StmGrpHn grp)
{
    STMERR_SetFunc(STM_GETLOOPCNT);
    if (grp == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (-1);
    }
    return (STM_GRP_LOOPCNT(grp));
}


/*************************************************
 * ���s��Ԃ̎擾(7.7)
 * grp	: �X�g���[���O���[�v�n���h��
 * fad	: �Đ�����FAD
 *************************************************/
Sint32	STM_GetExecStat(StmGrpHn grp, Sint32 *fad)
{
    Sint32	pos;
    Uint16	hirq;

    STMERR_SetFunc(STM_GETEXECSTAT);
    if (grp == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (-1);
    }
    /* �������O���[�v�̏ꍇ	*/
    if (MNG_CURGRP(stm_mng_ptr) == grp) {
	if ((IS_PLAYING(grp) == TRUE)&&
	    (IS_READING(grp) == TRUE)) {
	    GFCF_GetStat(&pos, &hirq);	/* �Đ�FAD���擾	*/
	    if (pos >= 0) {
		STM_GRP_FAD(grp) = pos;
	    }
	}
    }
    *fad = STM_GRP_FAD(grp);
    return (STM_GRP_ACSTAT(grp));
}


/*************************************************
 * �G���[�������̌Ăт����֐��̓o�^(9.1)
 * func	: �Ăт����֐�
 * obj	: �Ăт����֐�������
 *************************************************/
void	STM_SetErrFunc(StmErrFunc func, void *obj)
{
    STM_ERR_CODE(&MNG_ERR(stm_mng_ptr)) = STM_ERR_OK;
    STM_ERR_FUNC(&MNG_ERR(stm_mng_ptr)) = func;
    STM_ERR_OBJ(&MNG_ERR(stm_mng_ptr)) = obj;
}


/*************************************************
 * �G���[��Ԃ̎擾(9.2)
 * stat : �G���[���
 *************************************************/
void	STM_GetErrStat(StmErrStat *stat)
{
    memcpy(stat, &MNG_ERR(stm_mng_ptr), sizeof(StmErrStat));
}


/* end of stm_acc.c	*/
