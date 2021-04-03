/*****************************************************************************
 *	�\�t�g�E�F�A���C�u����
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library: �X�g���[���V�X�e��
 * Module : ���������W���[��
 * File	  : stm_init.c
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

/* �X�g���[���Ǘ��̈�	*/
StmMng	*stm_mng_ptr;


/*****************************************************************************
 *	�֐��̒�`
 *****************************************************************************/

/*--- STMMNG functions -------------------------------------------------*/

/****************************************************
 * �X�g���[���V�X�e���}�l�[�W��������
 * grpmax : �����ɊJ���X�g���[���O���[�v�̍ő吔
 * stmmax : �����ɊJ���X�g���[���̍ő吔
 * work   : ��Ɨ̈�
 ****************************************************/
STM_LOCAL void	stmmng_init(Sint32 grpmax, Sint32 stmmax, void *work)
{
    stm_mng_ptr = (StmMng *)work;
    memset(stm_mng_ptr, SVR_UNUSE, STM_WORK_SIZE(grpmax, stmmax));
    MNG_GRPTBL(stm_mng_ptr) = (StmGrp *)(stm_mng_ptr + 1);
    MNG_GRPMAX(stm_mng_ptr) = grpmax;
    MNG_STMTBL(stm_mng_ptr) = (Stm *)(MNG_GRPTBL(stm_mng_ptr) + grpmax);
    MNG_STMMAX(stm_mng_ptr) = stmmax;
    MNG_SFAD(stm_mng_ptr) = -1;
    MNG_EFAD(stm_mng_ptr) = -1;
    MNG_SVREXEC(stm_mng_ptr) = FALSE;

    CMD_FUNC(&stm_cmd_area) = CMD_NULLFUNC;
    CMD_OBJ(&stm_cmd_area) = NULL;

    STM_ERR_CODE(&MNG_ERR(stm_mng_ptr)) = STM_ERR_OK;
    STM_ERR_WHERE(&MNG_ERR(stm_mng_ptr)) = STM_INIT;
    STM_ERR_FUNC(&MNG_ERR(stm_mng_ptr)) = STM_ERR_NULLFUNC;
    STM_ERR_OBJ(&MNG_ERR(stm_mng_ptr)) = NULL;
}



/****************************************************
 * ���g�p�O���[�v�̈�̎擾
 * �߂�l :	���g�p�X�g���[���O���[�v�̈�
 ****************************************************/
STM_LOCAL StmGrp	*stmmng_getUnuseGrp(void)
{
    Sint32	i;
    StmGrp	*grp;

    for (i = 0, grp = MNG_GRP(stm_mng_ptr, 0); i < MNG_GRPMAX(stm_mng_ptr); i++, grp++) {
	if (STM_GRP_SVRSTAT(grp) == SVR_UNUSE) {
	    STM_GRP_SFADLST(grp) = NULL;
	    STM_GRP_FLTLST(grp) = NULL;
	    STM_GRP_LOOPSTM(grp) = NULL;
	    STM_GRP_LSTSTM(grp) = NULL;
	    STM_GRP_NEXTTRNS(grp) = NULL;
	    STM_GRP_FAD(grp) = STM_FAD_CDTOP;
	    STM_GRP_ACSTAT(grp) = STM_EXEC_PAUSE;
	    STM_GRP_CHKSTM(grp) = STM_CDBUF_REST;
	    STM_GRP_CHKSCTNUM(grp) = 0;
	    STM_GRP_CHKFUNC(grp) = STM_CDBUF_NULLFUNC;
	    STM_GRP_CHKOBJ(grp) = NULL;
	    STM_GRP_LOOPMAX(grp) = STM_LOOP_ENDLESS;
	    STM_GRP_LOOPCNT(grp) = 0;
	    STM_GRP_PUID(grp) = -1;
	    STM_GRP_ATR(grp) = 0;
	    STM_GRP_SVRSTAT(grp) = SVR_USE;
	    return (grp);
	}
    }
    return (NULL);
}
    


/****************************************************
 * ���g�p�X�g���[���̈�̎擾
 * �߂�l :	���g�p�X�g���[���̈�
 ****************************************************/
STM_LOCAL Stm	*stmmng_getUnuseStm(void)
{
    Sint32	i;
    Stm		*stm;

    for (i = 0, stm = MNG_STM(stm_mng_ptr, 0); i < MNG_STMMAX(stm_mng_ptr); i++, stm++) {
	if (STM_SVRSTAT(stm) == SVR_UNUSE) {
	    return (stm);
	}
    }
    return (NULL);
}
    


/*****************************************************
 * �ŏIFAD�̎擾
 * grp	: �X�g���[���O���[�v
 * �߂�l: �ŏIFAD
 *****************************************************/
Sint32	STMMNG_GetLstFad(StmGrpHn grp)
{
    Sint32	sfad, efad;
    StmHn	lststm;

    lststm = STM_GRP_LSTSTM(grp);
    if (lststm == NULL) {
	return (STM_FAD_CDTOP);
    } else {
	STL_GetPlayArea(STM_FLOW(lststm), &sfad, &efad);
	return (efad);
    }
}



/*****************************************************
 * GFCD�̃G���[���X�g���[���̃G���[�ɕϊ����ݒ肷��
 * code	: �G���[�R�[�h
 *****************************************************/
void	STM_SetGfcdErr(Sint32 code)
{
    Sint32	stmerr;

    switch (code) {
    case	GFCD_ERR_TMOUT:
	stmerr = STM_ERR_TMOUT;
	break;
    case	GFCD_ERR_OPEN:
	stmerr = STM_ERR_CDOPEN;
	break;
    case	GFCD_ERR_NODISC:
	stmerr = STM_ERR_CDNODISC;
	break;
    case	GFCD_ERR_FATAL:
	stmerr = STM_ERR_FATAL;
	break;
    default:
	stmerr = STM_ERR_CDRD;
    }
    STMERR_SetCode(stmerr);
}



/*****************************************************
 * �X�g���[�����X�g�̐ݒ�
 * grp	  : �X�g���[���O���[�v
 * stm	  : �X�g���[���n���h��
 * atr	  : �A�g���r���[�g
 *****************************************************/
STM_LOCAL Bool	stmmng_setStmLst(StmGrpHn grp, StmHn stm, Uint8 atr)
{
    StmHn	*lst_stm;
    StmHn	before;
    Sint32	next;
    Sint32	stm_sfad, stm_efad;
    Sint32	lst_sfad, lst_efad;
    Sint32	ret;

    /* �X�g���[���̊J�nFAD���擾	*/
    STL_GetPlayArea(STM_FLOW(stm), &stm_sfad, &stm_efad);

    /* �J�nFAD�������ɂȂ�悤�ɑ}���ʒu�����߂�	*/
    for (lst_stm = &STM_GRP_SFADLST(grp); *lst_stm != NULL; 
	                                    lst_stm = &STM_SFADLST(*lst_stm)) {
	STL_GetPlayArea(STM_FLOW(*lst_stm), &lst_sfad, &lst_efad);
	if (stm_sfad < lst_sfad) {
	    break;
	}
    }
    /* �X�g���[���}��	*/
    STM_SFADLST(stm) = *lst_stm;	/* ���̃X�g���[���̐ݒ�		*/
    *lst_stm = stm;			/* �O�̃X�g���[���̐ݒ�		*/

    if (STM_IS_CDFILE(stm)) {
	/* �ŏI�X�g���[���ݒ�	*/
	lst_efad = STMMNG_GetLstFad(grp);
	if (stm_efad > lst_efad) {
	    STM_GRP_LSTSTM(grp) = stm;
	}

	/* �i��̍Č�������߂�	*/
	before = NULL;
	for (lst_stm = &STM_GRP_FLTLST(grp); *lst_stm != NULL; 
	                                    lst_stm = &STM_FLTLST(*lst_stm)) {
	    before = *lst_stm;
	}
	GFCD_SetFiltCon(GFS_CDR_FLT(STM_DTSRC(stm)), 
			GFS_CDR_BUFNO(STM_DTSRC(stm)), CDC_NUL_SEL);
	/* �i��ڑ��ݒ�	*/
	if (before != NULL) {
	    GFCD_SetFiltCon(GFS_CDR_FLT(STM_DTSRC(before)), 
			    GFS_CDR_BUFNO(STM_DTSRC(before)),
			    GFS_CDR_BUFNO(STM_DTSRC(stm)));
	} else if (STM_GRP_PUID(grp) >= 0) {
	    GFCD_SetCon(GFS_CDR_FLT(STM_DTSRC(stm)), STM_GRP_PUID(grp));
	}
	/* �X�g���[���ǉ�	*/
	*lst_stm = stm;
	STM_FLTLST(stm) = NULL;
	ret = GFCD_WaitServer();
	if (ret != GFCD_ERR_OK) {
	    STM_SetGfcdErr(ret);
	    return (FALSE);
	}
    }

    /* ���[�v�X�g���[���ݒ�	*/
    if ((IS_LOOPSPECIFY(grp) == FALSE)&&
	((atr & STM_ATR_RESI) == 0)&&
	(stm == STM_GRP_SFADLST(grp))) {
	STM_GRP_LOOPSTM(grp) = stm;
    }
    return (TRUE);
}



/*************************************************
 * �i�胂�[�h�̎擾
 * subh	: �i��p�^�[��
 * �߂�l : �i�胂�[�h
 *************************************************/
STM_LOCAL Sint32	stm_getFmode(StmKey *key)
{
    Sint32	fmode = FMODE_FAD;

    if (STM_KEY_FN(key) != STM_KEY_NONE) {
	fmode |= FMODE_FN;
    }
    if (STM_KEY_CN(key) != STM_KEY_NONE) {
	fmode |= FMODE_CN;
    }
    if (STM_KEY_SMMSK(key) != STM_KEY_NONE) {
	fmode |= FMODE_SM;
    }
    if (STM_KEY_CIMSK(key) != STM_KEY_NONE) {
	fmode |= FMODE_CI;
    }
    return (fmode);
}



/*************************************************
 * �f�t�H���g�i��̎擾
 * key	: �X�g���[���L�[
 * dirrec : �f�B���N�g�����R�[�h
 * subh	: �i��p�^�[���ݒ�̈�
 * �߂�l : �i�胂�[�h
 *************************************************/
STM_LOCAL Sint32	stm_getDefaultFilt(StmKey *key, GfsDirId *dirrec, CdcSubh *subh)
{
    Sint32	fmode;

    CDC_SUBH_FN(subh) = (Uint8)STM_KEY_FN(key);
    CDC_SUBH_CN(subh) = (Uint8)STM_KEY_CN(key);
    CDC_SUBH_SMMSK(subh) = (Uint8)STM_KEY_SMMSK(key);
    CDC_SUBH_SMVAL(subh) = (Uint8)STM_KEY_SMVAL(key);
    CDC_SUBH_CIMSK(subh) = (Uint8)STM_KEY_CIMSK(key);
    CDC_SUBH_CIVAL(subh) = (Uint8)STM_KEY_CIVAL(key);
    fmode = stm_getFmode(key);
    return (fmode);
}

    

/*****************************************************
 * �X�g���[���̈�̏�����
 * stm	  : �̈�
 * grp	  : �X�g���[���O���[�v
 * gfs	  : �t�@�C���n���h��
 * dirrec : �f�B���N�g�����R�[�h
 * key    : �X�g���[���L�[
 * atr	  : �A�g���r���[�g
 * �߂�l : �X�g���[���n���h��
 *****************************************************/
STM_LOCAL StmHn	stmmng_initStm(Stm *stm, StmGrpHn grp, GfsHn gfs, 
		       GfsDirId *dirrec, StmKey *key, Uint8 atr)
{
    GfsFlow	*flow;
    GfsFinfo	*finfo;
    Sint32	nsct;
    Sint32	sfad, efad;
    Sint32	fmode;
    Sint32	ret;
    Bool	lst;
    CdcSubh	subh;
    Sint32	fad;

    flow = &(GFS_FILE_FLOW(gfs));
    finfo = &(GFS_FLW_FINFO(flow));

    /* �Đ��Z�N�^�����C���^���[�u�M���v���܂߂����̂ɕϊ�	*/
    nsct = GFCF_cnvIntlvOfs(finfo, GFS_FI_NSCT(finfo));
    GFS_FI_NSCT(finfo) = nsct;
    GFS_FI_LSTRM(finfo) = 0;  	/* �ŏI�Z�N�^�͑S�ėL���ɂ���	*/

    STM_GRP(stm) = grp;
    STM_ALIAS(stm) = stm;
    STM_GFS(stm) = gfs;
    STM_FLOW(stm) = flow;
    STM_TRFUNC(stm) = STM_TR_NULLFUNC;
    STM_OBJ(stm) = NULL;
    STM_ATR(stm) = atr;
    STL_GetPlayArea(flow, &sfad, &efad);
    /* �ǂݍ��ݒ��ɃI�[�v�����ꂽ�ꍇ�ǂݍ��݃t���O�����Ă�	*/
    if ((IS_PLAYING(grp) == TRUE)&&(sfad >= STM_GRP_FAD(grp))) {
	STM_ATR(stm) |= STM_ATR_READ;
    }
    STM_SVRSTAT(stm) = SVR_USE;
    GFS_TRN_STAT(&GFS_FILE_TRANS(gfs)) = GFTR_ST_END;
    STM_TRFAD(stm) = STM_FAD_CDTOP;		/* �]��FAD�̐ݒ�	*/

    /* �i�胂�[�h�ݒ�			*/
    if (STM_IS_CDFILE(stm)) {
	fmode = stm_getDefaultFilt(key, dirrec, &subh);		/* �f�t�H���g�i�� */
	GFCD_SetFilt(GFS_CDR_FLT(&(GFS_FLW_DTSRC(flow))), fmode, &subh,
		     GFS_FI_FAD(finfo), GFS_FI_NSCT(finfo)); 	/* �i��ݒ�	  */
	ret = GFCD_WaitServer();
	if (ret != GFCD_ERR_OK) {
	    STM_SetGfcdErr(ret);
	    return (NULL);
	}
	GFCF_SetSct(flow, LONG_MAX);				/* �Ǎ��݃Z�N�^�� */
    }

    lst = stmmng_setStmLst(grp, stm, atr);
    if (lst == FALSE) {
	return (NULL);
    }
    /* �Đ����ɍŏI�X�g���[�����ς�����ꍇ	*/
    if ((IS_PLAYING(grp) == TRUE)&&		/* �ǂݍ��ݒ�		*/
	(IS_SEEKING(grp) == FALSE)&&		/* �V�[�N���łȂ�	*/
	(STM_GRP_LSTSTM(grp) == stm)) {
	GFCF_GetStat(&fad, NULL);
	/* �Đ��͈͂̏I�����10�Z�N�^�ȏ��O�Ȃ�I���ʒu��ύX����	*/
	/* 10�Z�N�^�ȉ��Ȃ�|�[�Y��҂���stm_read�ōēxPLAY�𔭍s����	*/
	if (fad < (MNG_EFAD(stm_mng_ptr) - 10)) {			/* �r�W�[�łȂ�		*/
	    ret = STL_ChgPlayEnd(grp, efad);	/* �Đ��͈͂̕ύX	*/
	    if (ret != STM_ERR_OK) {
		STMERR_SetCode(ret);
	    }
	}
    }
    return (stm);
}
   


/*****************************************************
 * �X�g���[���O���[�v�̈���
 * grp	:	�O���[�v�̈�
 *****************************************************/
STM_LOCAL void	stmmng_unuseGrp(StmGrpHn grp)
{
    STM_GRP_SFADLST(grp) = NULL;
    STM_GRP_FLTLST(grp) = NULL;
    STM_GRP_SVRSTAT(grp) = SVR_UNUSE;
}


/*****************************************************
 * �i��ڑ������X�g�̐ؒf
 * stm	: �X�g���[��
 *****************************************************/
Bool	STMMNG_RemoveFlt(StmHn stm)
{
    StmGrpHn	grp;
    StmHn	*next;
    StmHn	before;
    Sint32	fltno;
    Sint32	ret;

    if (STM_IS_CDFILE(stm)) {
	grp = STM_GRPHN(stm);
	before = NULL;
	for (next = &STM_GRP_FLTLST(grp); (*next != NULL)&&(*next != stm); 
	                                      next = &STM_FLTLST(*next)) {
	    before = *next;
	}
	if (*next != NULL) {
	    *next = STM_FLTLST(stm);
	    if (*next == NULL) {
		fltno = CDC_NUL_SEL;
	    } else {
		fltno = GFS_CDR_FLT(STM_DTSRC(*next));
	    }
	    if (before == NULL) {
		GFCD_SetCon(fltno, STM_GRP_PUID(grp));
	    } else {
		GFCD_SetFiltCon(GFS_CDR_FLT(STM_DTSRC(before)), 
				GFS_CDR_BUFNO(STM_DTSRC(before)), fltno);
	    }
	    ret = GFCD_WaitServer();
	    if (ret != GFCD_ERR_OK) {
		STM_SetGfcdErr(ret);
		return (FALSE);
	    }
	}
    }
    return (TRUE);
}



/*****************************************************
 * �X�g���[���̈���
 * stm	: �X�g���[��
 *****************************************************/
STM_LOCAL Bool	stmmng_unuseStm(StmHn stm)
{
    StmGrpHn	grp;
    StmHn	*next;
    StmHn	before;
    Sint32	bufno;
    Bool	ret = TRUE;;

    if (STM_SVRSTAT(stm) == SVR_USE) {
	STM_SVRSTAT(stm) = SVR_UNUSE;
	grp = STM_GRPHN(stm);
	if (grp == NULL) {
	    return FALSE;
	}
	/* �Đ������X�g����X�g���[����ؒf����		*/
	for (next = &STM_GRP_SFADLST(grp); (*next != NULL)&&(*next != stm); 
	                                      next = &STM_SFADLST(*next))
	    ;
	if (*next != NULL) {
	    *next = STM_SFADLST(stm);
	}

	/* �i��ڑ������X�g����X�g���[����ؒf����	*/
	ret = STMMNG_RemoveFlt(stm);
	STM_GRP(stm) = NULL;
    }
    return (ret);
}


/*****************************************************
 * �ŏI�X�g���[���̍Đݒ�
 * grp	: �X�g���[���O���[�v
 *****************************************************/
void	STMMNG_ResetLstStm(StmGrpHn grp)
{
    StmHn	stm;
    Sint32	sfad, efad;
    StmHn	lst_stm = NULL;
    Sint32	lst_efad = STM_FAD_CDTOP;

    for (stm = STM_GRP_FLTLST(grp); stm != NULL; stm = STM_FLTLST(stm)) {
	if (STM_IS_CDFILE(stm)) {
	    STL_GetPlayArea(STM_FLOW(stm), &sfad, &efad);
	    if (lst_efad < efad) {
		lst_efad = efad;
		lst_stm = stm;
	    }
	}
    }
    STM_GRP_LSTSTM(grp) = lst_stm;
}



/****************************************************
 * �X�g���[���V�X�e��������(1.1)
 * grpmax : �����ɊJ���X�g���[���O���[�v�̍ő吔
 * stmmax : �����ɊJ���X�g���[���̍ő吔
 * work   : ��Ɨ̈�
 * �߂�l :	TRUE	����I��
 *		FALSE	�ȏ�I��
 ***************************************************/
Bool	STM_Init(Sint32 grpmax, Sint32 stmmax, void *work)
{
    /* ��Ɨ̈斢�ݒ�̂��߁A�G���[�ݒ�͕s��	*/

    if (work == NULL) {
	return (FALSE);
    } else {
	stmmng_init(grpmax, stmmax, work);	/* �X�g���[���V�X�e���}�l�[�W��������	*/
	return (TRUE);
    }
}



/**************************************************
 * �X�g���[���O���[�v�̃I�[�v��(2.1)
 * �߂�l :	�X�g���[���O���[�v�n���h��(���蓖�Ă��Ȃ��ꍇNULL)
 **************************************************/
StmGrpHn	STM_OpenGrp(void)
{
    StmGrp	*grp;

    STMERR_SetFunc(STM_OPENGRP);
    grp = stmmng_getUnuseGrp();		/* ���g�p�̈�̎擾		*/
    if (grp == NULL) {			/* ���蓖�ė̈悪�Ȃ��ꍇ	*/
	return (NULL);
    }
    return (grp);
}



/*****************************************************
 * �X�g���[���̃N���[�Y����
 * stm	: �X�g���[��
 *****************************************************/
STM_LOCAL void	stminit_close(StmHn stm)
{
    StmHn	next;
    StmGrpHn	grp;
    Sint32	sfad, efad;
    Sint32	ret;

    grp = STM_GRPHN(stm);
    if (STM_SVRSTAT(stm) == SVR_USE) {    /* �I�[�v���X�g���[��  */
	/* ���[�v�X�g���[���̏ꍇ�͏풓�łȂ��擪�X�g���[����ݒ�	*/
	if (stm == STM_GRP_LOOPSTM(grp)) {
	    for (next = STM_GRP_SFADLST(grp);
		 next != NULL; next = STM_SFADLST(next)) {
		if ((next != stm)&&(IS_STM_RESI(next) == FALSE)) {
		    break;
		}
	    }
	    STM_GRP_LOOPSTM(grp) = next;
	    CLR_LOOPSPECIFY(grp);
	}
	/* �]���X�g���[���̏ꍇ�͎��̃X�g���[����ݒ�	*/
	if (stm == STM_GRP_NEXTTRNS(grp)) {
	    GFTR_Stop(STM_GFS(stm), GFTR_ALLSCT);
	    GFS_TRN_STAT(&GFS_FILE_TRANS(STM_GFS(stm))) = GFTR_ST_NOACT;
                                        /* END ��ԂɂȂ�Ȃ��悤�ɂ���	*/
	    STM_GRP_NEXTTRNS(grp) = STM_SFADLST(stm);
	    /* ���̃X�g���[�����Ȃ��ꍇ�͐擪�̃X�g���[��	*/
	    if (STM_GRP_NEXTTRNS(grp) == NULL) {
		STM_GRP_NEXTTRNS(grp) = STM_GRP_SFADLST(grp);
	    }
	    /* �擪���������g�̏ꍇ�͓]���X�g���[����ݒ肵�Ȃ�	*/
	    if (STM_GRP_NEXTTRNS(grp) == stm){
		STM_GRP_NEXTTRNS(grp) = NULL;
	    }
	}
	/* �X�g���[�����ڑ�����Ă��邱�Ƃ�����̂Ńf�t�H���g��Ԃɖ߂�	*/
	GFS_CDR_BUFNO(STM_DTSRC(stm)) = GFS_CDR_FLT(STM_DTSRC(stm));
	stmmng_unuseStm(stm);			/* �̈���		*/
	/* �Đ����̏ꍇ�A�i���؂�����CSCT��҂��ăo�b�t�@���N���A����	*/
	if (IS_PLAYING(grp) == TRUE) {
	    STL_WaitCsct();
	}
	GFS_Close(STM_GFS(stm));		/* �t�@�C���n���h�����	*/
    }

    /* �ŏI�X�g���[���̏ꍇ		*/
    if (stm == STM_GRP_LSTSTM(grp)) {
	STMMNG_ResetLstStm(grp);	/* �ŏI�X�g���[���̍Đݒ�	*/
	if ((STM_GRP_LSTSTM(grp) != NULL)&&
	    (IS_PLAYING(grp) == TRUE)) {
	    STL_GetPlayArea(STM_FLOW(STM_GRP_LSTSTM(grp)), &sfad, &efad);
	    ret = STL_ChgPlayEnd(grp, efad);
	    if (ret != STM_ERR_OK) {
		STMERR_SetCode(ret);
	    }
	}
    }
    /* �Đ����ɂ��ׂĂ̓ǂݍ��ނׂ��X�g���[�����N���[�Y�����ꍇ	*/
    if ((STM_GRP_FLTLST(grp) == NULL)&&
	(IS_PLAYING(grp) == TRUE)) {
	ret = STL_StopIn(grp);				/* �ǂݍ��ݒ�~	*/
	if (ret != STM_ERR_OK) {
	    STMERR_SetCode(ret);
	} else {
	    STMSVR_SetReadEnd(grp);		/* �ǂݍ��ݍς݃t���O�̐ݒ�	*/
	    STM_GRP_LOOPCNT(grp)++;
	    STM_GRP_FAD(grp) = STM_FAD_CDTOP;
	    STM_GRP_ACSTAT(grp) = STM_EXEC_PAUSE;
	    /* �Đ��I��,�ǂݍ��ݏI��	*/
	    STM_GRP_ATR(grp) &= ~(STMGRP_ATR_PLAY|STMGRP_ATR_READ|STMGRP_ATR_SEEK);
	}
    }

}


/*************************************************
 * �X�g���[���O���[�v�̃N���[�Y(2.2)
 * grp	: �X�g���[���O���[�v�n���h��
 *************************************************/
void	STM_CloseGrp(StmGrpHn grp)
{
    StmHn		stm;
    Bool		close_flag = TRUE;

    STMERR_SetFunc(STM_CLOSEGRP);
    if (grp == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return;
    }
    /* ���ׂẴX�g���[�����N���[�Y����		*/
    while ((stm = STM_GRP_SFADLST(grp)) != NULL) {
	stminit_close(stm);
    }
    /* ���s���̃X�g���[���O���[�v�̏ꍇ		*/
    if (STM_GRP_PUID(grp) >= 0) {
	MNG_CURGRP(stm_mng_ptr) = NULL;
	GFCD_UngetPickup(STM_GRP_PUID(grp));
	STM_GRP_PUID(grp) = -1;
	GFCD_SetCsctFunc(GFCD_NULLFUNC, NULL);
	STL_WaitStop();		/* �h���C�u����~����̂�҂�	*/
    }
    stmmng_unuseGrp(grp);
    if (IS_SET_ERRCODE(stm_mng_ptr) == TRUE) {
	STMERR_Exec();
    }
}


/*************************************************
 * �X�g���[���I�[�v���̃T�u���[�`��
 * grp	  : �X�g���[���O���[�v
 * dirrec : �f�B���N�g�����R�[�h
 * fid	  : �t�@�C�����ʎq
 * key	  : �X�g���[���L�[
 * atr	  : �A�g���r���[�g
 * �߂�l : �X�g���[���n���h��
 ************************************************/
STM_LOCAL StmHn	stminit_open(StmGrpHn grp, GfsDirId *dirrec, Sint32 fid, StmKey *key,
		                                                    Uint8 atr)
{
    Stm		*stm;
    GfsHn	gfs;
    StmHn	stmHn;
    Sint32	stat;
    Sint32	nsct, rdsct;

    /* �X�g���[�����g�p�̈�̎擾	*/
    stm = stmmng_getUnuseStm();
    if (stm == NULL) {			/* �̈悪�Ȃ�		*/
	STMERR_SetCode(STM_ERR_NOHNDL);
	STMERR_Exec();
	return (NULL);
    }
    /* �t�@�C���V�X�e���̃I�[�v���̈�̎擾	*/
    gfs = GFS_mngAllocGrp();
    if (gfs == NULL) {
	stmmng_unuseStm(stm);		/* �擾�����X�g���[���̈�̉��	*/
	STMERR_SetCode(STM_ERR_NOHNDL);
	STMERR_Exec();
	return (NULL);
    }
    /* �I�[�v������			*/
    gfs = GFS_OpenSub(gfs, dirrec, fid);	        /* �t�@�C���I�[�v�� */
    stmHn = stmmng_initStm(stm, grp, gfs, dirrec, key, atr);	/* �n���h�������� */
    if (stmHn == NULL) {
	STMERR_Exec();
	return (NULL);
    }

    /* �풓�X�g���[���̏ꍇ		*/
    if ((atr & STM_ATR_RESI) != 0) {
	GFS_SetGmode(gfs, GFS_GMODE_RESIDENT);	/* �풓���[�h		*/
    }

    /* CD�t�@�C���ȊO�͓ǂݍ��݂��ɍs�Ȃ�	*/
    if (!(STM_IS_CDFILE(stm))) {
	GFS_GetFileSize(gfs, NULL, &nsct, NULL);
	GFS_NwCdRead(gfs, nsct);
	while (1) {
	    GFS_NwExecOne(gfs);
	    GFS_NwGetStat(gfs, &stat, &rdsct);
	    if (rdsct >= nsct) {
		break;
	    }
	}
    }

    /* �p�����[�^�ݒ�               */
    GFS_FILE_AMODE(STM_GFS(stmHn)) = GFS_NWSTAT_FREAD;	/* �A�N�Z�X���[�h */
    GFS_FILE_ASTAT(STM_GFS(stmHn)) = GFS_SVR_BUSY;		/* �����t���O     */
    GFTR_Start(&(GFS_FILE_TRANS(gfs)));
    return (stmHn);
}



/***********************************************
 * �t�@�C�����ʎq�ɂ��X�g���[���̃I�[�v��(�T�u���[�`��)
 * grp	  : �X�g���[���O���[�v
 * fid	  : �t�@�C�����ʎq
 * key	  : �X�g���[���L�[
 * atr	  : �A�g���r���[�g
 * �߂�l : �X�g���[���n���h��(�I�[�v���ł��Ȃ��ꍇNULL)
 ***********************************************/
STM_LOCAL StmHn	stminit_openFid(StmGrpHn grp, Sint32 fid, StmKey *key, Uint8 atr)
{
    GfsDirId	dirrec;
    StmKey	_key;
    StmHn	stm;
    Sint32	err;

    err = GFS_GetDirInfo(fid, &dirrec);		/* �f�B���N�g�����擾	*/
    if (err == GFS_ERR_OK) {
	memcpy(&_key, key, sizeof(StmKey));
	STM_KEY_FN(&_key) = GFS_DIR_FN(&dirrec);
	stm = stminit_open(grp, &dirrec, fid, &_key, atr);  /* �I�[�v��	*/
    } else {
	STMERR_SetCode(err);
	STMERR_Exec();
	stm = NULL;
    }
    return (stm);
}
    

/***********************************************
 * �t�@�C�����ʎq�ɂ��X�g���[���̃I�[�v��(3.1)
 * grp	  : �X�g���[���O���[�v
 * fid	  : �t�@�C�����ʎq
 * key	  : �X�g���[���L�[
 * lpread : ���[�v�Đ����̓ǂݍ��ݎw��
 * �߂�l : �X�g���[���n���h��(�I�[�v���ł��Ȃ��ꍇNULL)
 ***********************************************/
StmHn	STM_OpenFid(StmGrpHn grp, Sint32 fid, StmKey *key, Sint32 lpread)
{
    StmHn	stm;
    Uint8	atr;

    STMERR_SetFunc(STM_OPENFID);
    if (grp == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (NULL);
    }
    if (lpread == STM_LOOP_NOREAD) {
	atr = STM_ATR_LOOPNOREAD;
    } else if (lpread == STM_LOOP_READ) {
	atr = 0;
    } else {
	STMERR_SetCode(STM_ERR_PARA);
	STMERR_Exec();
	return (NULL);
    }
    stm = stminit_openFid(grp, fid, key, atr);
    return (stm);
}



/***********************************************
 * �Z�N�^�ʒu�ɂ��X�g���[���̃I�[�v��(3.2)
 * grp	  : �X�g���[���O���[�v�n���h��
 * frange : �Đ��͈�
 * key	  : �X�g���[���L�[
 * lpread : ���[�v�Đ����̓ǂݍ��ݎw��
 * �߂�l : �X�g���[���n���h��(�I�[�v���ł��Ȃ��ꍇNULL)
 ***********************************************/
StmHn	STM_OpenFrange(StmGrpHn grp, StmFrange *frange, StmKey *key, Sint32 lpread)
{
    GfsDirId	dirrec;
    StmHn	stm;
    Uint8	atr;

    STMERR_SetFunc(STM_OPENFRANGE);
    if (grp == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (NULL);
    }
    if ((STM_FRANGE_SFAD(frange) < STM_FAD_CDTOP)||
	(STM_FRANGE_FASNUM(frange) < 0)) {
	STMERR_SetCode(STM_ERR_NUM);
	STMERR_Exec();
	return (NULL);
    }
	
    /* �f�B���N�g�����R�[�h�Z�b�g�A�b�v	*/
    GFS_DIR_FAD(&dirrec) = STM_FRANGE_SFAD(frange);
    GFS_DIR_SIZE(&dirrec) = STM_FRANGE_FASNUM(frange) * GFS_MD1_SCTSIZ;
    GFS_DIR_FN(&dirrec) = (Uint8)STM_KEY_FN(key);
    GFS_DIR_ATR(&dirrec) = 0;
    GFS_DIR_UNIT(&dirrec) = 1;
    GFS_DIR_GAP(&dirrec) = 0;
    if (lpread == STM_LOOP_NOREAD) {
	atr = STM_ATR_LOOPNOREAD;
    } else if (lpread == STM_LOOP_READ) {
	atr = 0;
    } else {
	STMERR_SetCode(STM_ERR_PARA);
	STMERR_Exec();
	return (NULL);
    }

    /* �I�[�v��		*/
    stm = stminit_open(grp, &dirrec, FID_OPEN_FRANGE, key, atr);
    return (stm);
}



/*************************************************
 * �]���J�nFAD�̐ݒ�(4.6)
 * stm	: �X�g���[���n���h��
 * fad	: �]���J�nFAD
 *************************************************/
STM_LOCAL void	stminit_setTrFad(StmHn stm, Sint32 fad)
{
    if (IS_STM_RESI(stm)) {
	GFS_Seek(STM_GFS(stm), 0, GFS_SEEK_SET);
    }
    GFS_TRN_STAT(&GFS_FILE_TRANS(STM_GFS(stm))) = GFTR_ST_NOACT;
                                      /* END ��ԂɂȂ�Ȃ��悤�ɂ���	*/
    STM_TRFAD(stm) = fad;
}


void	STM_SetTrFad(StmHn stm, Sint32 fad)
{
    STMERR_SetFunc(STM_SETTRFAD);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return;
    }
    if (fad < 0) {
	STMERR_SetCode(STM_ERR_NUM);
	STMERR_Exec();
	return ;
    }
    stm = STM_ALIAS(stm);
    stminit_setTrFad(stm, fad);
}


/*************************************************
 * �풓�X�g���[���̃I�[�v��(3.3)
 * grp	: �X�g���[���O�[�v
 * fid	: �t�@�C�����ʎq
 * key	: �X�g���[���L�[
 * fad	: �]���J�nFAD
 * �߂�l : �X�g���[���n���h��(�I�[�v���ł��Ȃ��ꍇNULL)
 ***********************************************/
StmHn	STM_OpenResi(StmGrpHn grp, Sint32 fid, StmKey *key, Sint32 fad)
{
    StmHn	stm;

    STMERR_SetFunc(STM_OPENRESI);
    if (grp == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (NULL);
    }
    if (fad < 0) {
	STMERR_SetCode(STM_ERR_NUM);
	STMERR_Exec();
	return (NULL);
    }
    stm = stminit_openFid(grp, fid, key, STM_ATR_LOOPNOREAD|STM_ATR_RESI);
                                       /* �t�@�C�����ʎq�ɂ��I�[�v��	*/
    if (stm == NULL) {
	return (NULL);
    }

    /* �]���J�nFAD�̐ݒ�	*/
    stminit_setTrFad(stm, fad);
    return (stm);
}



/*************************************************
 * �X�g���[���̃N���[�Y(3.4)
 * stm	: �X�g���[���n���h��
 *************************************************/
void	STM_Close(StmHn stm)
{
    STMERR_SetFunc(STM_CLOSE);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
    }
    stm = STM_ALIAS(stm);
    stminit_close(stm);
    if (IS_SET_ERRCODE(stm_mng_ptr) == TRUE) {
	STMERR_Exec();
    }
}


/*************************************************
 * �X�g���[���L�[�̐ݒ�(3.5)
 * stm	: �X�g���[���n���h��
 * stmkey : �X�g���[���L�[
 *************************************************/
void	STM_SetKey(StmHn stm, StmKey *stmkey)
{
    CdcSubh	subh;
    Sint32	fmode;
    GfsFinfo	*finfo;
    Sint32	ret;

    STMERR_SetFunc(STM_SETKEY);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return ;
    }

    stm = STM_ALIAS(stm);
    if (STM_IS_CDFILE(stm)) {
	/* �T�u�w�b�_���ݒ�	*/
	CDC_SUBH_FN(&subh) = (Uint8)STM_KEY_FN(stmkey);
	CDC_SUBH_CN(&subh) = (Uint8)STM_KEY_CN(stmkey);
	CDC_SUBH_SMMSK(&subh) = (Uint8)STM_KEY_SMMSK(stmkey);
	CDC_SUBH_SMVAL(&subh) = (Uint8)STM_KEY_SMVAL(stmkey);
	CDC_SUBH_CIMSK(&subh) = (Uint8)STM_KEY_CIMSK(stmkey);
	CDC_SUBH_CIVAL(&subh) = (Uint8)STM_KEY_CIVAL(stmkey);
	fmode = stm_getFmode(stmkey);		/* �i�胂�[�h�擾	*/

	finfo = &(GFS_FLW_FINFO(STM_FLOW(stm)));
	GFCD_SetFilt(GFS_CDR_FLT(&GFS_FLW_DTSRC(STM_FLOW(stm))), fmode, &subh,
		     GFS_FI_FAD(finfo), GFS_FI_NSCT(finfo)); 	/* �i��ݒ�	  */
	ret = GFCD_WaitServer();
	if (ret != GFCD_ERR_OK) {
	    STM_SetGfcdErr(ret);
	    STMERR_Exec();
	}
    }
}


/*--- STMERR functions -------------------------------------------------*/

/*****************************************************
 * �����֐��̐ݒ�
 * funcid : �֐�ID
 *****************************************************/
void	STMERR_SetFunc(Sint32 funcid)
{
    STM_ERR_WHERE(&MNG_ERR(stm_mng_ptr)) = funcid;
}


/*****************************************************
 * �G���[�R�[�h�̐ݒ�
 * code	: �G���[�R�[�h
 *****************************************************/
void	STMERR_SetCode(Sint32 code)
{
    if (IS_SET_ERRCODE(stm_mng_ptr) == FALSE) {
	STM_ERR_CODE(&MNG_ERR(stm_mng_ptr)) = code;
    }
}


/*****************************************************
 * �G���[�֐��̌Ăт���
 * code	: �G���[�R�[�h
 *****************************************************/
void	STMERR_Exec(void)
{
    if ((STM_ERR_FUNC(&MNG_ERR(stm_mng_ptr)) != STM_ERR_NULLFUNC)&&
	(IS_SET_ERRCODE(stm_mng_ptr) == TRUE)) {
	STM_ERR_FUNC(&MNG_ERR(stm_mng_ptr))(STM_ERR_OBJ(&MNG_ERR(stm_mng_ptr)), 
					STM_ERR_CODE(&MNG_ERR(stm_mng_ptr)));
    }
    STM_ERR_CODE(&MNG_ERR(stm_mng_ptr)) = STM_ERR_OK;
}


/* end of stm_init.c	*/


