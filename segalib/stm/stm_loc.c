/*****************************************************************************
 *	�\�t�g�E�F�A���C�u����
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library: �X�g���[���V�X�e��
 * Module : ���[�J���֐�(CD�u���b�N����)
 * File	  : stm_loc.c
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


#include	"sega_stm.h"
#include	"stm_def.h"
#include	"../gfs/gfs_def.h"
#include	"../gfs/gfs_cdb.h"
#include	"../gfs/gfs_cdc.h"
#include	"../gfs/gfs_cdf.h"
#include	"../gfs/gfs_dir.h"
#include	"stm_loc.h"

/*****************************************************************************
 *	�萔
 *****************************************************************************/
#define TIME_40MSEC	(12186*2)

/*****************************************************************************
 *	�֐��̒�`
 *****************************************************************************/

/*************************************************
 * ���݂�FAD�̎擾
 * �߂�l:���݂�FAD
 *************************************************/
Sint32	STL_GetCurFad(void)
{
    CdcStat	stat;
    Sint32	ret;

    do {
	ret = CDC_GetCurStat(&stat);
    } while (ret != CDC_ERR_OK);
    return CDC_STAT_FAD(&stat);
}


/*************************************************
 * ��~�҂�
 * stat	: �X�e�[�^�X
 *************************************************/
void	STL_WaitStop(void)
{
    CdcStat	st;

    do {
	GFCD_GetStat(&st);
    } while (((CDC_STAT_STATUS(&st) & CDC_ST_PAUSE) != CDC_ST_PAUSE)&&
	     ((CDC_STAT_STATUS(&st) & CDC_ST_STANDBY) != CDC_ST_STANDBY));
}


/*************************************************
 * �Đ��͈͂̎擾
 * flow	: �ǂݍ��݊Ǘ�
 * sfad	: �J�nFAD
 * efad : �I��FAD
 *************************************************/
void	STL_GetPlayArea(GfsFlow *flow, Sint32 *sfad, Sint32 *efad)
{
    Sint32	fid;
    Sint32	sctsiz;
    Sint32	nsct;
    Sint32	lastsiz;
    Sint32	fno;
    Sint32	atr;

    GFCF_GetFileInfo(flow, &fid, sfad, &sctsiz, &nsct, &lastsiz, &fno, &atr);
    if (IS_CDFILE(atr)) {	/* CD�t�@�C���̏ꍇ	*/
	*efad = *sfad + nsct - 1;
    } else {			/* SIMM, SCSI�̏ꍇ	*/
	*sfad = LONG_MAX;
	*efad = LONG_MAX;
    }
}

    

/*************************************************
 * �X�g���[���O���[�v�Đ�
 * grp	: �X�g���[���O���[�v
 * �߂�l: �G���[�R�[�h
 *************************************************/
Sint32	STL_FlowIn(StmGrpHn grp)
{
    Sint32	_sfad, sfad, efad;
    CdcPly	ply;
    Sint32	ret = STM_ERR_OK;

    /* ���[�v�X�g���[���̊J�nFAD���擾	*/
    sfad = STM_GRP_FAD(grp);			/* �J�nFAD�ݒ�	*/
    /* �ŏI�X�g���[���̏I��FAD���擾	*/
    STL_GetPlayArea(STM_FLOW(STM_GRP_LSTSTM(grp)), &_sfad, &efad);
    if (efad >= sfad) {
	/* �p�����[�^�ݒ� */
	CDC_PLY_STYPE(&ply) = CDC_PTYPE_FAD;
	CDC_PLY_SFAD(&ply) = sfad;
	CDC_PLY_ETYPE(&ply) = CDC_PTYPE_FAD;
	CDC_PLY_EFAS(&ply) = efad - sfad + 1;
	CDC_PLY_PMODE(&ply) = CDC_PM_DFL;
	ret = GFCD_Play(&ply, STM_GRP_PUID(grp));	/* �ǂݍ���	*/
	if (ret == GFCD_ERR_OPEN) {
	    ret = STM_ERR_CDOPEN;
	} else if (ret != GFCD_ERR_OK) {
	    ret = STM_ERR_CDRD;
	} else {
	    MNG_SFAD(stm_mng_ptr) = sfad;
	    MNG_EFAD(stm_mng_ptr) = efad;
	}
    }
    return (ret);
}



/*************************************************
 * �ǂݍ��ݒ�~
 * grp	: �X�g���[���O���[�v
 *************************************************/
Sint32	STL_StopIn(StmGrpHn grp)
{
    CdcPos	pos;
    Sint32	ret = STM_ERR_OK;

    CDC_POS_PTYPE(&pos) = CDC_PTYPE_NOCHG;
    ret = GFCD_MovePickup(&pos, STM_GRP_PUID(grp));
    if (ret == GFCD_ERR_OPEN) {
	ret = STM_ERR_CDOPEN;
    } else if (ret != GFCD_ERR_OK) {
	ret = STM_ERR_CDRD;
    } else {
	MNG_SFAD(stm_mng_ptr) = -1;
	MNG_EFAD(stm_mng_ptr) = -1;
    }
    return (ret);
}



/*************************************************
 * ���[�h�G���[�������̃��J�o��
 * grp	: �X�g���[���O���[�v
 *************************************************/
Sint32	STL_Recover(StmGrpHn grp)
{
    CdcPly	pinfo;
    Sint32	err;

    /* CdPlay�𔭍s���� */
    CDC_PLY_STYPE(&pinfo) = CDC_PTYPE_NOCHG;
    CDC_PLY_ETYPE(&pinfo) = CDC_PTYPE_NOCHG;
    CDC_PLY_PMODE(&pinfo) = CDC_PM_NOCHG;
    err = GFCD_Play(&pinfo, STM_GRP_PUID(grp));
    if (err != GFCD_ERR_OK) {
	return STM_ERR_CDRD;
    } else {
	return STM_ERR_OK;
    }
}
    

/*************************************************
 * �s�b�N�A�b�v�̈ړ�(�Đ��p��)
 * fad	: �ړ���FAD
 * �߂�l:�G���[�R�[�h
 *************************************************/
Sint32	STL_MovePlaypos(StmGrpHn grp, Sint32 fad)
{
    CdcPly	ply;
    Sint32	ret = STM_ERR_OK;

    /* �Đ��J�n�ʒu�̂ݕύX	*/
    CDC_PLY_STYPE(&ply) = CDC_PTYPE_FAD;
    CDC_PLY_SFAD(&ply) = fad;
    CDC_PLY_ETYPE(&ply) = CDC_PTYPE_NOCHG;	/* �I���ʒu�ύX���Ȃ�	*/
    CDC_PLY_PMODE(&ply) = CDC_PM_DFL;
    ret = GFCD_Play(&ply, STM_GRP_PUID(grp));
    if (ret == GFCD_ERR_OPEN) {
	ret = STM_ERR_CDOPEN;
    } else if (ret != GFCD_ERR_OK) {
	ret = STM_ERR_CDRD;
    } else {
	MNG_SFAD(stm_mng_ptr) = fad;
    }
    return (ret);
}
    

/*************************************************
 * �Đ��I���ʒu�̕ύX
 * grp	: �X�g���[���O���[�v
 * efad : �I���ʒu
 *************************************************/
Sint32	STL_ChgPlayEnd(StmGrpHn grp, Sint32 efad)
{
    CdcPly	ply;
    Sint32	ret = STM_ERR_OK;

    /* �Đ��J�n�ʒu�̂ݕύX	*/
    CDC_PLY_STYPE(&ply) = CDC_PTYPE_FAD;
    CDC_PLY_SFAD(&ply) = MNG_SFAD(stm_mng_ptr);
    CDC_PLY_ETYPE(&ply) = CDC_PTYPE_FAD;	/* �I���ʒu�ύX����	*/
    CDC_PLY_EFAS(&ply) = efad - MNG_SFAD(stm_mng_ptr) + 1;
    CDC_PLY_PMODE(&ply) = CDC_PM_NOCHG;
    ret = GFCD_Play(&ply, STM_GRP_PUID(grp));
    if (ret == GFCD_ERR_OPEN) {
	ret = STM_ERR_CDOPEN;
    } else if (ret != GFCD_ERR_OK) {
	ret = STM_ERR_CDRD;
    } else {
	MNG_EFAD(stm_mng_ptr) = efad;
    }
    return (ret);
}
    

/*************************************************
 * �ǂݍ��݃Z�N�^���̎擾
 * flow	: �ǂݍ��݊Ǘ�
 * sn	: �Z�N�^�ԍ��i�擪�Z�N�^�͂O�j
 * sinfo: �Z�N�^���
 * �߂�l: TRUE �w��Z�N�^����  FALSE �w��Z�N�^�Ȃ�
 *************************************************/
Bool	STL_GetSctInfo(GfsFlow *flow, Sint32 sn, StmSct *sinfo)
{
    Sint32	nsct;
    CdcSct	info;

    /* �Z�N�^���̃`�F�b�N	*/
    nsct = GFCB_GetLenData(flow, LONG_MAX, NULL);
    if (sn >= nsct) {    /* �Z�N�^�����w�肳�ꂽ�Z�N�^�ԍ��ɖ����Ȃ�	*/
	return (FALSE);
    }

    /* �Z�N�^���̎擾		*/
    GFCD_GetSctInfo(GFS_CDR_BUFNO(&GFS_FLW_DTSRC(flow)), sn, &info);

    STM_SCT_FAD(sinfo) = CDC_SCT_FAD(&info);
    STM_SCT_FN(sinfo) = CDC_SCT_FN(&info);
    STM_SCT_CN(sinfo) = CDC_SCT_CN(&info);
    STM_SCT_SM(sinfo) = CDC_SCT_SM(&info);
    STM_SCT_CI(sinfo) = CDC_SCT_CI(&info);
    return (TRUE);
}


/*************************************************
 * �Z�N�^�̎��f�[�^�T�C�Y�̎擾
 * flow	: �ǂݍ��݊Ǘ�
 * spos	: �Z�N�^�ʒu
 * �߂�l: �Z�N�^�̎��f�[�^�T�C�Y(���[�h)
 *************************************************/
Sint32	STL_GetActSiz(GfsFlow *flow, Sint32 spos)
{
    Sint32	nword;
    Sint32	nsct;

    GFCD_GetLenData(GFS_CDR_BUFNO(&(GFS_FLW_DTSRC(flow))), 
		    GFS_CDR_SCTPOS(&(GFS_FLW_DTSRC(flow))) + spos, 1, 
		    &nsct, &nword);
    GFCD_WaitServer();
    return (nword / sizeof(Uint16));
}


/*************************************************
 * CD�o�b�t�@�f�[�^�̏���
 * flow	: �ǂݍ��݊Ǘ�
 * spos	: �Z�N�^�ʒu
 * snum	: �Z�N�^��
 *************************************************/
void	STL_DeleteSctData(GfsFlow *flow, Sint32 spos, Sint32 snum)
{
    Sint32	nsct;

    nsct = GFCB_GetLenData(flow, LONG_MAX, NULL);
    if ((spos + snum) > nsct) {
	snum = nsct - spos;
    }
    if (snum > 0) {
	GFCD_DelSctData(GFS_CDR_BUFNO(&(GFS_FLW_DTSRC(flow))), spos, snum);
    }
}


/*************************************************
 * CSCT�֐��pCD�o�b�t�@�g�p�Z�N�^���̎擾
 * �߂�l : �g�p�Z�N�^��
 *************************************************/
Sint32	STL_CsctGetSctNum(GfsFlow *flow)
{
    Sint32	bufno;
    Sint32	nsct;
    Sint32	ret;

    bufno = GFS_CDR_BUFNO(&(GFS_FLW_DTSRC(flow)));
    /* ���̊֐���CSCT�֐�����Ă΂��̂Œ���CDC���Ă�	*/
    do {
	ret = CDC_GetSctNum(bufno, &nsct);
    } while (ret != CDC_ERR_OK);
    return (nsct);
}


/*************************************************
 * CSCT�֐��pCD�o�b�t�@�󂫃Z�N�^�̎擾
 * �߂�l : �󂫃Z�N�^��
 *************************************************/
Sint32	STL_CsctGetFreeSctnum(void)
{
    Sint32	total, nblk, free;
    Sint32	ret;

    /* ���̊֐���CSCT�֐�����Ă΂��̂Œ���CDC���Ă�	*/
    do {
	ret = CDC_GetBufSiz(&total, &nblk, &free);
    } while (ret != CDC_ERR_OK);
    return (free);
}


/*************************************************
 * CSCT�r�b�g�����̂�҂�
 *************************************************/
void	STL_WaitCsct(void)
{
    Sint32	i;
    Uint16	hirq;

    CDC_ClrHirqReq(~CDC_HIRQ_CSCT);
    for (i = 0; i < TIME_40MSEC; i++) {
	hirq = CDC_GetHirqReq();
	if ((hirq & CDC_HIRQ_CSCT) != 0) {
	    break;
	}
    }
}


/* end of stm_loc.c	*/









