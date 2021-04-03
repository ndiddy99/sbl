/*****************************************************************************
 *	�\�t�g�E�F�A���C�u����
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library: �X�g���[���V�X�e��
 * Module : �]�����W���[��
 * File	  : stm_trn.c
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
#include	"../gfs/gfs_cdf.h"
#include	"../gfs/gfs_cdb.h"
#include	"../gfs/gfs_trn.h"
#include	"../gfs/gfs_buf.h"
#include	"../gfs/gfs_dir.h"
#include	"stm_loc.h"
#include	"stm_def.h"

/*****************************************************************************
 *	�֐��̒�`
 *****************************************************************************/

/*************************************************
 * �]���֐��̌Ăт���
 * stm	: �X�g���[���n���h��
 * nsct : �]���\�Z�N�^��
 * �߂�l: �]�������Z�N�^��
 *************************************************/
STM_LOCAL Sint32	stm_trFunc(void *stm, Sint32 nsct)
{
    Sint32	remain;

    /* �]���֐��Ăт���		*/
    remain = STM_TRFUNC((StmHn)stm)(STM_OBJ((StmHn)stm), (StmHn)stm, nsct);
    return (remain);
}

    

/*************************************************
 * �]���֐��̐ݒ�(4.2)
 * stm	: �X�g���[���n���h��
 * func	: �]�����s�֐�
 * obj	: �]���֐��̑�����
 *************************************************/
void	STM_SetTrFunc(StmHn stm, StmTrFunc func, void *obj)
{
    GfsTransFunc	tfunc;

    STMERR_SetFunc(STM_SETTRFUNC);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return ;
    }
    stm = STM_ALIAS(stm);
    STM_TRFUNC(stm) = func;
    if (func == STM_TR_NULLFUNC) {	/* �]���֐����������ꍇ	*/
	GFTR_SetData(&(GFS_FILE_TRANS(STM_GFS(stm))), NULL, 0, STM_UNIT_FORM1);
	GFTR_SetMode(&(GFS_FILE_TRANS(STM_GFS(stm))), TMODE_DFL);
	GFTR_SetTransFunc(STM_GFS(stm), GFTR_NULLFUNC, NULL);
	STM_OBJ(stm) = NULL;
    } else {				/* �]���֐��ݒ�			*/
	GFTR_SetData(&(GFS_FILE_TRANS(STM_GFS(stm))), NULL, 
		     LONG_MAX/STM_UNIT_FORM1, STM_UNIT_FORM1);
	GFTR_SetMode(&(GFS_FILE_TRANS(STM_GFS(stm))), GFS_TMODE_STM);
	GFTR_SetTransFunc(STM_GFS(stm), stm_trFunc, stm);
	STM_OBJ(stm) = obj;
    }
}


/*************************************************
 * �]���֐��ɂ�����]���J�n(4.3)
 * stm	: �X�g���[���n���h��
 * adlt	: 1���[�h�]�����Ƃ̓]���A�h���X�ω���
 *************************************************/
Uint32	*STM_StartTrans(StmHn stm, Sint32 *adlt)
{
    GfdpHn	dpk;

    STMERR_SetFunc(STM_STARTTRANS);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (NULL);
    }
    stm = STM_ALIAS(stm);
    dpk = GFS_TRN_DPS(&(GFS_FILE_TRANS(STM_GFS(stm))));
    if (STM_IS_CDFILE(stm)) {
	GFCB_GetSctData(dpk, STM_DTSRC(stm));	/* �Z�N�^�f�[�^�̎擾	*/
    }
    *adlt = GFDP_ADLT(dpk);			/* �A�h���X�ω���	*/
    return (GFDP_DATA(dpk));			/* �]���A�h���X		*/
}


/*************************************************
 * �]���̈�̐ݒ�(4.1)
 * stm	: �X�g���[���n���h��
 * buffer : �]���̈�
 * bufsiz : �]���̈�̃T�C�Y
 *************************************************/
STM_LOCAL void	stmtrn_setTrBuf(StmHn stm, void *buffer, Sint32 nunit, Sint32 unitsize)
{
    GfsFinfo	*finfo;

    if (unitsize == STM_UNIT_WORD) {
	nunit *= sizeof(Uint16);
	unitsize = STM_UNIT_BYTE;
    }
    finfo = &GFS_FLW_FINFO(STM_FLOW(stm));
    if ((unitsize == STM_UNIT_BYTE)||(GFS_FI_SCTSZ(finfo) != 0)) {
	GFS_FI_LSTRM(finfo) = GFS_LAST_ROOM(GFS_FI_SIZE(finfo), GFS_MD1_SCTSIZ);
    } else {
	GFS_FI_LSTRM(finfo) = 0;
    }
    GFTR_SetData(&(GFS_FILE_TRANS(STM_GFS(stm))), (Uint8 *)buffer, nunit, unitsize);
}


void	STM_SetTrBuf(StmHn stm, void *buffer, Sint32 nunit, Sint32 unitsize)
{
    Sint32	bufsize;

    STMERR_SetFunc(STM_SETTRBUF);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return ;
    }
    if (nunit < 0) {
	STMERR_SetCode(STM_ERR_NUM);
	STMERR_Exec();
	return ;
    }
    if ((unitsize != STM_UNIT_BYTE)&&(unitsize != STM_UNIT_WORD)&&
	(unitsize != STM_UNIT_FORM1)&&(unitsize != STM_UNIT_FORM2)) {
	STMERR_SetCode(STM_ERR_PARA);
	STMERR_Exec();
	return ;
    }
    stm = STM_ALIAS(stm);
    stmtrn_setTrBuf(stm, buffer, nunit, unitsize);
}


/*************************************************
 * �]���̈������������(4.8)
 * stm	: �X�g���[���n���h��
 *************************************************/
void	STMTRN_ResetTrBuf(StmHn stm)
{
    Uint8	*data;
    Sint32	bsize;
    Sint32	unitsize;

    GFTR_SetTransPara(&(GFS_FILE_TRANS(STM_GFS(stm))), LONG_MAX);
    if (GFS_TRN_BUF(&(GFS_FILE_TRANS(STM_GFS(stm)))) != NULL) {
	GFTR_GetData(&(GFS_FILE_TRANS(STM_GFS(stm))), &data, &bsize, &unitsize);
	GFTR_SetData(&(GFS_FILE_TRANS(STM_GFS(stm))), data, bsize, unitsize);
    }
}

void	STM_ResetTrBuf(StmHn stm)
{
    Uint8	*data;
    Sint32	bsize;

    STMERR_SetFunc(STM_RESETTRBUF);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return ;
    }
    stm = STM_ALIAS(stm);
    STMTRN_ResetTrBuf(stm);
}

    
/*************************************************
 * �]���̈�̃f�[�^���̎擾(6.1)
 * stm	: �X�g���[���n���h��
 * �߂�l: �f�[�^��(���[�h�P��)
 *************************************************/
Sint32	STM_GetLenTrBuf(StmHn stm)
{
    Sint32	nbyte;

    STMERR_SetFunc(STM_GETLENTRBUF);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (-1);
    }
    stm = STM_ALIAS(stm);
    nbyte = GFTR_GetLenData(&GFS_FILE_TRANS(STM_GFS(stm)));
    return (nbyte / sizeof(Uint16));
}



/*************************************************
 * �]���̈�t���̃`�F�b�N(6.2)
 * stm	: �X�g���[���n���h��
 * �߂�l: TRUE  �̈�T�C�Y�ɒB���Ă���  FALSE  �̈�T�C�Y�ɒB���Ă��Ȃ�
 *************************************************/
Bool	STMTRN_IsTrBufFull(StmHn stm)
{
    Sint32	nroom;

    nroom = GFTR_GetLenRoom(&(GFS_FILE_TRANS(STM_GFS(stm))));
                                /* �󂫗̈�̑傫�����擾	*/
    if (nroom == 0) {		/* �󂫗̈悪�Ȃ�		*/
	return (TRUE);
    } else {			/* �󂫗̈悪����		*/
	return (FALSE);
    }
}


Bool	STM_IsTrBufFull(StmHn stm)
{
    Bool	full;

    STMERR_SetFunc(STM_ISTRBUFFULL);
    if (stm == NULL) {
	STMERR_SetCode(STM_ERR_HNDL);
	STMERR_Exec();
	return (FALSE);
    }
    stm = STM_ALIAS(stm);
    full = STMTRN_IsTrBufFull(stm);
    return (full);
}


/* end of stm_trn.c	*/







