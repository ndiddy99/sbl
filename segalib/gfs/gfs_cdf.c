/*****************************************************************************
 *	�\�t�g�E�F�A���C�u����
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:�t�@�C���V�X�e��
 * Module :CD�t�@�C���A�N�Z�X
 * File	  :gfs_cdf.c
 * Date   :1994-11-11
 * Version:1.16
 * Auther :M.S.
 *
 *****************************************************************************/

/*****************************************************************************
 *	�C���N���[�h�t�@�C��
 *****************************************************************************/
#include	<limits.h>

#include	"sega_gfs.h"
#include	"gfs_def.h"
#include	"gfs_cdf.h"
#include	"gfs_cdb.h"
#include	"gfs_cdc.h"

/*****************************************************************************
 *	�萔�}�N��
 *****************************************************************************/

/*****************************************************************************
 *	�񋓒萔
 *****************************************************************************/

/*****************************************************************************
 *	�����}�N��
 *****************************************************************************/

/*****************************************************************************
 *	�f�[�^�^�̒�`
 *****************************************************************************/

/*****************************************************************************
 *	�ϐ��̐錾
 *****************************************************************************/

/*****************************************************************************
 *	�֐��̐錾
 *****************************************************************************/
/*  �Z�N�^�T�C�Y�̎擾	*/
GFS_LOCAL Sint32 gfcf_getSctSize(Sint32 atr);

/* CD��Ԃ̊Ď� */
GFS_LOCAL Bool gfcf_isCdOk(GfsDtsrc *dts, Sint32 stat, Sint32 *err);

/* �Đ��͈͂�flow�ɐݒ� */
GFS_LOCAL Bool gfcf_getPlayRange(GfsFlow *flow, Sint32 *nsct, Sint32 *fstat);

/*****************************************************************************
 *	�ϐ��̒�`
 *****************************************************************************/

/*****************************************************************************
 *	�֐��̒�`
 *****************************************************************************/

/* CDF���W���[���̏����� */
Sint32 GFCF_Init(Bool use_cdbfs)
{
    Sint32 ret;

    MNG_FLAGS(gfs_mng_ptr) &= ~MNG_CDCON_MSK;
    ret = GFCD_Init(&MNG_CDBMNG(gfs_mng_ptr), use_cdbfs);
    if (ret == GFCD_ERR_OK) {
	MNG_FLAGS(gfs_mng_ptr) |= MNG_CDCON_MSK;
    }
    switch (ret) {
    case GFCD_ERR_OK:
	return GFS_ERR_OK;
    case GFCD_ERR_OPEN:
	return GFS_ERR_CDOPEN;
    case GFCD_ERR_NODISC:
	return GFS_ERR_CDNODISC;
    case GFCD_ERR_CDROM:
	return GFS_ERR_CDROM;
    case GFCD_ERR_FATAL:
	return GFS_ERR_FATAL;
    default:
	return GFS_ERR_CDRD;
    }
}


/*****************************************************************************
 *  �ǂݍ��݊Ǘ��\���̂̏�����
 *�m���́n
 *  flow	�ǂݍ��݊Ǘ��\����
 *  dirrec	�f�B���N�g�����R�[�h
 *  fid		�t�@�C�����ʎq
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  ����I���Ȃ�TRUE
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
Bool GFCF_Setup(GfsFlow *flow, GfsDirId *dirrec, Sint32 fid)
{
    Sint32	fsize;
    GfsFinfo	*finfo = &GFS_FLW_FINFO(flow);

    /* �t�@�C�����ݒ� */
    if (dirrec == NULL) {
	/* CD�u���b�N�t�@�C���V�X�e�����p */
	GFS_FI_FID(finfo) = fid;
	if (GFCD_GetFileInfo(fid, &GFS_FI_FINFO(finfo)) != GFCD_ERR_OK) {
	    return FALSE;
	}
	fsize = GFS_FI_SIZE(finfo);
    } else {
	fsize = GFS_DIR_SIZE(dirrec);
	GFS_FI_FID(finfo) = fid;
	GFS_FI_FAD(finfo) = GFS_DIR_FAD(dirrec);
	GFS_FI_SIZE(finfo) = fsize;
	GFS_FI_UNIT(finfo) = GFS_DIR_UNIT(dirrec);
	GFS_FI_GAP(finfo) = GFS_DIR_GAP(dirrec);
	GFS_FI_FN(finfo) = GFS_DIR_FN(dirrec);
	GFS_FI_ATR(finfo) = GFS_DIR_ATR(dirrec);
    }
    GFS_FI_SCTSZ(finfo) = gfcf_getSctSize(GFS_DIR_ATR(dirrec));
    GFS_FI_NSCT(finfo) = GFS_BYTE_SCT(fsize, GFS_MD1_SCTSIZ);
    if (GFS_FI_SCTSZ(finfo) == 0) {
	/* form1/2���݂̂Ƃ� */
	GFS_FI_LSTRM(finfo) = 0;
    } else {
	GFS_FI_LSTRM(finfo) = GFS_LAST_ROOM(fsize, GFS_MD1_SCTSIZ);
    }
    /* �ǂݍ��݌��Ǘ����ݒ� */
    if (GFCB_Setup(flow) == FALSE) {	/* GFS_FLW_DTSRC(flow)������ */
	return FALSE;
    }
    /* ���̑��ǂݍ��݊Ǘ��f�[�^�ݒ� */
    GFS_FLW_GMODE(flow) = GFS_GMODE_ERASE;
    GFS_FLW_STAT(flow) = GFS_FIN_NOACT;
    GFS_FLW_SCT(flow) = GFS_FLW_SCTCNT(flow) = 0;
    GFS_FLW_SCTMAX(flow) = GFS_RPARA_DFL;
    return TRUE;
}


/* �ǂݍ��݃Z�N�^���ݒ� */
void GFCF_SetSct(GfsFlow *flow, Sint32 sct)
{
    Sint32	nsct;

    GFS_FLW_SCT(flow) = sct;
    nsct = GFCB_GetLenData(flow, GFS_RPARA_DFL, NULL);
    if (nsct < 0) {
	nsct = 0;
    }
    GFS_FLW_SCTCNT(flow) = nsct - GFS_CDR_SCTPOS(&GFS_FLW_DTSRC(flow));
}


/*****************************************************************************
 *  �t�@�C�����̎擾
 *�m���́n
 *  flow	�ǂݍ��݊Ǘ�
 *�m�o�́n
 *  fid		�t�@�C�����ʎq
 *  sctsiz	�Z�N�^�T�C�Y
 *  nsct	�Z�N�^��
 *  lastsiz	�ŏI�Z�N�^�L���f�[�^��
 *  fno		�t�@�C���ԍ�
 *  atr		����
 *�m�֐��l�n
 *  
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
void GFCF_GetFileInfo(GfsFlow *flow,
		      Sint32 *fid, Sint32 *fad, Sint32 *sctsiz,
		      Sint32 *nsct, Sint32 *lastsiz, Sint32 *fno, Sint32 *atr)
{
    GfsFinfo	*finfo = &GFS_FLW_FINFO(flow);

    if (fid != NULL) {
	*fid = GFS_FI_FID(finfo);
    }
    if (fad != NULL) {
	*fad = GFS_FI_FAD(finfo);
    }
    if (fno != NULL) {
	*fno = GFS_FI_FN(finfo);
    }
    if (sctsiz != NULL) {
	*sctsiz = GFS_FI_SCTSZ(finfo);
    }
    if (nsct != NULL){
	*nsct = GFS_FI_NSCT(finfo);
    }
    if (atr != NULL) {
	*atr = GFS_FI_ATR(finfo);
    }
    if (lastsiz != NULL) {
	if (IS_FORM1(GFS_FI_ATR(finfo)) && IS_FORM2(GFS_FI_ATR(finfo))) {
	    *lastsiz = 0;
	} else {
	    *lastsiz = GFS_FI_SCTSZ(finfo) - GFS_FI_LSTRM(finfo);
	}
    }
}


/*****************************************************************************
 *  CD�o�b�t�@�ւ̃f�[�^�ǂݍ���
 *�m���́n
 *  flow	�ǂݍ��݊Ǘ�
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �ǂݍ��ݏ��(GFS_FIN_�`)
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFCF_FlowInBuf(GfsFlow *flow)
{
    Sint32	stat, fad, err, nsct;
    CdcPly	pinfo;
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    stat = GFCF_GetStat(&fad, NULL);
    if (gfcf_isCdOk(dts, stat, &err) == FALSE) {
	return err;
    }
    if (GFCB_IsPuOwner(dts) == FALSE) {
	/* ���䌠���܂��擾���ĂȂ������������ */
	if (gfcf_getPlayRange(flow, &nsct, &err) == FALSE) {
	    return err;
	}
	if (GFCB_GetPickup(dts) == FALSE) {
	    return GFS_FIN_ERROR;
	}
	if (GFCB_SetCon(dts) == FALSE) { /* CD�f�o�C�X�ڑ� */
	    return GFS_FIN_ERROR;
	}
	/* CdPlay�𔭍s���� */
	CDC_PLY_STYPE(&pinfo) = CDC_PTYPE_FAD;
	CDC_PLY_SFAD(&pinfo) = GFS_CDR_SFAD(dts);
	CDC_PLY_ETYPE(&pinfo) = CDC_PTYPE_FAD;
	CDC_PLY_EFAS(&pinfo) = GFS_CDR_EFAD(dts) - CDC_PLY_SFAD(&pinfo);
	CDC_PLY_PMODE(&pinfo) = CDC_PM_DFL;
	err = GFCD_Play(&pinfo, GFS_CDR_PUID(dts));
	if (err != GFCD_ERR_OK) {
	    return GFS_FIN_ERROR;
	}
	GFS_FLW_SCTCNT(flow) += nsct;	/* �ǂݍ��݃Z�N�^�����Z */
	MNG_SFAD(gfs_mng_ptr) = CDC_PLY_SFAD(&pinfo);
	MNG_EFAD(gfs_mng_ptr) = GFS_CDR_EFAD(dts);
	return GFS_FIN_START;		/* �f�[�^��ǂݍ��� */
    }
    /* �Đ����I�����������ׂ� */
    if (GFS_CDR_EFAD(dts) <= fad) {
	GFCB_UngetPickup(dts);
	if (GFS_FLW_SCT(flow) == GFS_FLW_SCTCNT(flow) &&
	    MNG_EFAD(gfs_mng_ptr) <= fad) {
	    /* ���ׂĂ̓ǂݍ��݂��������� */
	    MNG_SFAD(gfs_mng_ptr) = MNG_EFAD(gfs_mng_ptr);
	    return GFS_FIN_END;
	}
	/* �ǂݍ��ݒP�ʂ�flowin�͊������� */
	return GFS_FIN_PLAY;
    }
    return GFS_FIN_PLAY;		/* CD�ǂݍ��ݒ� */
}


/* CD�o�b�t�@�ւ̃f�[�^�ǂݍ��ݒ��~ */
void GFCF_StopInBuf(GfsFlow *flow, Bool stop_flag)
{
    CdcPos	pos;
    Sint32	fad;
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    if (stop_flag) {
	MNG_SFAD(gfs_mng_ptr) = MNG_EFAD(gfs_mng_ptr) = 0;
	GFCB_DeleteAllData(dts);
	CDC_POS_PTYPE(&pos) = CDC_PTYPE_NOCHG;
	GFCB_MovePickup(dts, &pos);
    }
    if (GFCB_IsPuOwner(dts)) {
	GFCB_UngetPickup(dts);
	/* �Đ��͈͂�ʉ߂�����N���A */
	GFCF_GetStat(&fad, NULL);
	if (MNG_EFAD(gfs_mng_ptr) <= fad) {
	    MNG_SFAD(gfs_mng_ptr) = MNG_EFAD(gfs_mng_ptr);
	}
    }
}


/* ���[�h�G���[�������̃��J�o��		*/
Sint32	GFCF_Recover(GfsFlow *flow)
{
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);
    CdcPly	pinfo;
    Sint32	err;

    if (GFCB_IsPuOwner(dts)) {
	/* CdPlay�𔭍s���� */
	CDC_PLY_STYPE(&pinfo) = CDC_PTYPE_NOCHG;
	CDC_PLY_ETYPE(&pinfo) = CDC_PTYPE_NOCHG;
	CDC_PLY_PMODE(&pinfo) = CDC_PM_NOCHG;
	err = GFCD_Play(&pinfo, GFS_CDR_PUID(dts));
	if (err != GFCD_ERR_OK) {
	    return GFS_FIN_ERROR;
	}
    } else {
	return GFS_FIN_ERROR;
    }
    return GFS_FIN_PLAY;		/* CD�ǂݍ��ݒ� */
}
    

/* �A�N�Z�X�|�C���^�擾 */
Sint32 GFCF_Tell(GfsFlow *flow)
{
    return GFCB_Tell(&GFS_FLW_DTSRC(flow));
}
 
 
/*****************************************************************************
 *  �A�N�Z�X�|�C���^�ݒ�
 *�m���́n
 *  flow	�ǂݍ��݊Ǘ�
 *  off		�I�t�Z�b�g
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  �ݒ��̃A�N�Z�X�|�C���^
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFCF_Seek(GfsFlow *flow, Sint32 off)
{
    return GFCB_Seek(flow, off, GFS_SEEK_SET);
}


/*****************************************************************************
 *  �s�b�N�A�b�v�ړ�
 *�m���́n
 *  flow	�ǂݍ��݊Ǘ�
 *  off		�t�@�C���擪����̈ړ��ʒu
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  �G���[�R�[�h
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFCF_MovePickup(GfsFlow *flow, Sint32 off)
{
    Sint32	err;
    CdcPos	pos;
    GfsFinfo	*finfo = &GFS_FLW_FINFO(flow);

    if (GFCF_GetStat(NULL, NULL) != GFS_STAT_NOACT) {
	return GFS_ERR_PUINUSE;
    }
    /* �s�b�N�A�b�v�̐��䌠���擾���邱�Ƃɂ����flow in���łȂ����Ƃ��m�F */
    if (GFCB_GetPickup(&GFS_FLW_DTSRC(flow)) == FALSE) {
	return GFS_ERR_PUINUSE;
    }
    CDC_POS_PTYPE(&pos) = CDC_PTYPE_FAD;
    CDC_POS_FAD(&pos) = GFCF_cnvIntlvOfs(finfo, off) + GFS_FI_FAD(finfo);
    switch (GFCB_MovePickup(&GFS_FLW_DTSRC(flow), &pos)) {
    case GFCD_ERR_OK:
	err = GFS_ERR_OK;
	break;
    case GFCD_ERR_NOTOWNER:
	err = GFS_ERR_PUINUSE;
	break;
    case GFCD_ERR_OPEN:
	err = GFS_ERR_CDOPEN;
	break;
    default:
	err = GFS_ERR_CDRD;
	break;
    }
    GFCB_UngetPickup(&GFS_FLW_DTSRC(flow));
    return err;
}


/* �C���^�[���[�u�̃M���b�v���܂߂��I�t�Z�b�g�̌v�Z */
Sint32 GFCF_cnvIntlvOfs(GfsFinfo *finfo, Sint32 ofs)
{
    if (GFS_FI_UNIT(finfo) == 0) {
	return ofs;
    }
    return (ofs / GFS_FI_UNIT(finfo) *
	    (GFS_FI_UNIT(finfo) + GFS_FI_GAP(finfo))) +
		ofs % GFS_FI_UNIT(finfo);
}


/*****************************************************************************
 *  �X�e�[�^�X�擾
 *�m���́n
 *  �Ȃ�
 *�m�o�́n
 *  fad		���݂̃t���[���A�h���X
 *�m�֐��l�n
 *   GFS_STAT_FREAD,GFS_STAT_SEEK,GFS_STAT_NOACT�̂ǂꂩ
 *�m�@�\�n
 *   �t���[���A�h���X�������̎���*fad == -1
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFCF_GetStat(Sint32 *fad, Uint16 *hirq)
{
    CdcStat	stat;
    Uint16	tmp;

    tmp = GFCD_GetStat(&stat);
    if (hirq != NULL) {
	*hirq = tmp;
    }
    if (fad != NULL) {
	*fad = CDC_STAT_FAD(&stat);
    }
    if (tmp & CDC_HIRQ_DCHG) {
	return GFS_STAT_OPEN;
    }
    switch (CDC_STAT_STATUS(&stat) & 0xf) {
    case CDC_ST_BUSY:			/* ��ԑJ�ڒ� */
	return GFS_STAT_SEEK;
    case CDC_ST_PAUSE:			/* �|�[�Y�� */
    case CDC_ST_STANDBY:		/* �X�^���o�C�i�h���C�u��~��ԁj */
	return GFS_STAT_NOACT;
    case CDC_ST_PLAY:			/* �b�c�Đ��� */
    case CDC_ST_RETRY:			/* ���[�h���g���C������ */
	return GFS_STAT_READ;
    case CDC_ST_SEEK:			/* �V�[�N�� */
	return GFS_STAT_SEEK;
    case CDC_ST_SCAN:			/* �X�L�����Đ��� */
	return GFS_STAT_READ;
    case CDC_ST_OPEN:			/* �g���C���J���Ă��� */
	return GFS_STAT_OPEN;
    case CDC_ST_NODISC:			/* �f�B�X�N���Ȃ� */
	return GFS_STAT_NODISC;
    case CDC_ST_FATAL:			/* �v���I�G���[���������� */
	return GFS_STAT_FATAL;
    case CDC_ST_ERROR:			/* ���[�h�f�[�^�G���[���������� */
    default:
	return GFS_STAT_ERROR;
    }
}

/*---------------------------------------------------------------------------
 * ����J�֐�
 *---------------------------------------------------------------------------*/

/* �Z�N�^�T�C�Y�̎擾 */
GFS_LOCAL Sint32 gfcf_getSctSize(Sint32 atr)
{
    Sint32      sctsz;

    if (IS_CDDA(atr)) {
	sctsz = GFS_CDDA_SCTSIZ;
    } else if (IS_FORM2(atr)) {
	if (IS_FORM1(atr)) {
	    sctsz = 0;
	} else {
	    sctsz = GFS_FM2_SCTSIZ;
	}
    } else if (IS_FORM1(atr)) {
        sctsz = GFS_FM1_SCTSIZ;
    } else {
        sctsz = GFS_MD1_SCTSIZ;
    }
    return sctsz;
}


/* CD��Ԃ̊Ď� */
GFS_LOCAL Bool gfcf_isCdOk(GfsDtsrc *dts, Sint32 stat,  Sint32 *err)
{
    Sint32	fad;

    switch (stat) {
    case GFS_STAT_SEEK:
    case GFS_STAT_READ:
	break;				/* ���� */
    case GFS_STAT_NOACT:
	if (GFCD_GetBufSiz() == 0) {
	    /* �󂫃Z�N�^�͂O�����ABFUL�t���O���P�Ƃ͌���Ȃ� */
	    *err = GFS_FIN_PAUSE;	/* �o�b�t�@�t���Ń|�[�Y */
	    return FALSE;
	}
	break;
    case GFS_STAT_OPEN:
	*err = GFS_FIN_OPEN;
	return FALSE;
    case GFS_STAT_NODISC:
	*err = GFS_FIN_NODISC;
	return FALSE;
    case GFS_STAT_FATAL:
	*err = GFS_FIN_FATAL;
	return FALSE;
    default:
	*err = GFS_FIN_ERROR;
	return FALSE;
    }
    *err = GFS_FIN_START;
    return TRUE;
}


/* �Đ��͈͂�flow�ɐݒ� */
/* play�𔭍s����K�v������� TRUE��Ԃ� */
GFS_LOCAL Bool gfcf_getPlayRange(GfsFlow *flow, Sint32 *nsct, Sint32 *fstat)
{
    Sint32	ap_fad, bsct;
    GfsFinfo	*finfo = &GFS_FLW_FINFO(flow);
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    *nsct = GFS_FLW_SCT(flow) - GFS_FLW_SCTCNT(flow);
    /* �c��flowin�Z�N�^�����獡���flowin�Z�N�^���𓾂� */
    *nsct = MIN(*nsct, GFS_FLW_SCTMAX(flow));
    ap_fad = GFS_FI_FAD(finfo) + GFCF_cnvIntlvOfs(finfo, GFCB_Tell(dts));
    if (GFS_FLW_GMODE(flow) == GFS_GMODE_RESIDENT) {
	/* �o�b�t�@�ɕK�v�ȃf�[�^�������Ă��邩���ׂ� */
	bsct = GFCB_GetLenData(flow, LONG_MAX, NULL);
	if (bsct < 0) {
	    bsct = 0;
	}
	/* �܂��]������Ă��Ȃ��Z�N�^�f�[�^�̕������� */
	if (bsct > 0 && ap_fad < MNG_SFAD(gfs_mng_ptr)) {
	    bsct -= MNG_SFAD(gfs_mng_ptr) - ap_fad;
	}
    } else {
	bsct = 0;
    }
    if (bsct >= *nsct) {
	/* �o�b�t�@���̃Z�N�^�����K�v�ȃZ�N�^���������Ă��� */
	GFS_FLW_SCTCNT(flow) += *nsct; /* �ǂݍ��݃Z�N�^�����Z */
	if (GFS_FLW_SCT(flow) == GFS_FLW_SCTCNT(flow)) {
	    /* �S�Z�N�^��flowin���I������ */
	    *fstat = GFS_FIN_END;
	    MNG_SFAD(gfs_mng_ptr) = MNG_EFAD(gfs_mng_ptr);
	} else {
	    /* �ǂݍ��ݒP�ʂ�flowin���I������ */
	    *fstat = GFS_FIN_PLAY;
	    MNG_SFAD(gfs_mng_ptr) = ap_fad;
	}
	return FALSE;
    }
    *nsct -= bsct;
    GFS_CDR_SFAD(dts) = ap_fad;
    GFS_CDR_EFAD(dts) = GFS_FI_FAD(finfo) +
	GFCF_cnvIntlvOfs(finfo, *nsct + GFCB_Tell(dts));
    /* flowin���悤�Ƃ���͈͂��Đ��͈͂Ɋ��S�Ɋ܂܂�邩 */
    if (MNG_SFAD(gfs_mng_ptr) <= GFS_CDR_SFAD(dts) &&
	GFS_CDR_SFAD(dts) < MNG_EFAD(gfs_mng_ptr) &&
	MNG_SFAD(gfs_mng_ptr) < GFS_CDR_EFAD(dts) &&
	GFS_CDR_EFAD(dts) <= MNG_EFAD(gfs_mng_ptr)) {
	/* �Ȃ��CdPlay�𔭍s����K�v�͂Ȃ� */
	MNG_SFAD(gfs_mng_ptr) = GFS_CDR_SFAD(dts);
	*fstat = GFS_FIN_START;
	return FALSE;
    }
    return TRUE;
}

/* end of file */
