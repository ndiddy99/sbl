/*****************************************************************************
 *	�\�t�g�E�F�A���C�u����
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:�t�@�C���V�X�e��
 * Module :CD�o�b�t�@����
 * File	  :gfs_cdb.c
 * Date   :1994-11-11
 * Version:1.16
 * Auther :M.S.
 *
 *****************************************************************************/

/*****************************************************************************
 *	�C���N���[�h�t�@�C��
 *****************************************************************************/
#include "sega_gfs.h"
#include "gfs_def.h"
#include "gfs_cdf.h"
#include "gfs_cdb.h"
#include "gfs_cdc.h"
#include "gfs_trn.h"


/*****************************************************************************
 *	�萔�}�N��
 *****************************************************************************/
#define BUF_TOP         0		/* �b�c�o�b�t�@�擪 */

/* ���[�h�i��̃t�B���^���[�h	*/
#define FMODE_SELECT_FN		0x01	/* �t�@�C���ԍ��I���\ */
#define FMODE_SELECT_CN		0x02	/* �`���l���ԍ��I���\ */
#define FMODE_SELECT_SM		0x04	/* �T�u���[�h�I���\ */
#define FMODE_SELECT_CI		0x08	/* coding information �I���\ */
#define FMODE_REV_SUBH		0x10	/* �T�u�w�b�_�����𔽓]���� */
#define FMODE_SELECT_FRNG	0x40	/* �t���[���A�h���X�͈͗L�� */
#define FMODE_INIT_FMODE	0x80	/* �i����������������� */

#define	CDB_ADDR_DLT	0

#define GFCB_NOSEL	0		/* ���[�h���w�肵�Ȃ�   */

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
 *	�֐��̐錾
 *****************************************************************************/

#if	0
/* CD�o�b�t�@�̋󂫃Z�N�^���擾 */
GFS_LOCAL Sint32 gfcb_getSctRoom(void);
#endif

/*****************************************************************************
 *	�ϐ��̐錾
 *****************************************************************************/

/*****************************************************************************
 *	�֐��̒�`
 *****************************************************************************/

/*****************************************************************************
 *  CD�o�b�t�@�����Ǘ��\���̂̏�����
 *�m���́n
 *  flow	�ǂݍ��݊Ǘ��\����
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  ���������TRUE
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
Bool GFCB_Setup(GfsFlow *flow)
{
    GfsFinfo	*finfo = &GFS_FLW_FINFO(flow);
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);
    Sint32	bufno, fltno;
    CdcSubh	subh;

    GFS_DTS_FTYPE(dts) = CD_FILE;
    GFS_CDR_FMODE(dts) = FMODE_SELECT_FN;
    /* �i�胂�[�h�ݒ� */
    CDC_SUBH_FN(&subh) = GFS_FI_FN(finfo);
    CDC_SUBH_CN(&subh) = GFCB_NOSEL;
    CDC_SUBH_SMMSK(&subh) = GFCB_NOSEL;
    CDC_SUBH_SMVAL(&subh) = GFCB_NOSEL;
    CDC_SUBH_CIMSK(&subh) = 0;
    CDC_SUBH_CIVAL(&subh) = 0;
    GFS_CDR_SUBH(dts) = subh;
    GFS_CDR_PUID(dts) = GFCD_UID_NONE;
    GFS_CDR_SCTPOS(dts) = 0;
    GFS_CDR_FILEPOS(dts) = 0;
    GFS_CDR_SFAD(dts) = GFS_CDR_EFAD(dts) = 0;
    /* CD�o�b�t�@��抄�蓖�� */
    if (GFCD_AllocBuf(&bufno) != GFCD_ERR_OK) {
        return FALSE;
    }
    GFS_CDR_BUFNO(dts) = bufno;
    /* �i�芄�蓖�� */
    if (GFCD_AllocFilt(&fltno) != GFCD_ERR_OK) {
	GFCD_FreeBuf(bufno);
	return FALSE;
    }
    GFS_CDR_FLT(dts) = fltno;
    if (GFCD_SetFilt(fltno, GFS_CDR_FMODE(dts), &subh, 0, 0) != GFCD_ERR_OK) {
	return FALSE;
    }
    /* �i��̐ڑ���ݒ� */
    if (GFCD_SetFiltCon(fltno, bufno, CDC_NUL_SEL) != GFCD_ERR_OK) {
	return FALSE;
    }
    if (GFCD_WaitServer() != GFCD_ERR_OK) {
	return FALSE;
    }
    return TRUE;
}


/* �g�p�����̉�� */
void GFCB_Close(GfsDtsrc *dts)
{
    GFCD_FreeBuf(GFS_CDR_BUFNO(dts));
    GFCD_FreeFilt(GFS_CDR_FLT(dts));
}


/*--- �b�c�o�b�t�@���� -------------------------------------------------*/


/*****************************************************************************
 *  CD�o�b�t�@�̗L���Z�N�^���擾
 *�m���́n
 *  flow	�ǂݍ��݊Ǘ��̈�
 *  usct	�]���v���Z�N�^��
 *�m�o�́n
 *  asiz	�]���\�o�C�g��
 *�m�֐��l�n
 *  �]���\�Z�N�^��
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFCB_GetLenData(GfsFlow *flow, Sint32 usct, Sint32 *asiz)
{
    Sint32	ret, nsct;
    Sint32	sctsiz, fsct, lastsiz;
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    ret = GFCD_GetLenData(GFS_CDR_BUFNO(dts), GFS_CDR_SCTPOS(dts),
			  usct, &nsct, asiz);
    if (ret != GFCD_ERR_OK) {
	return 0;
    }
    if (GFCD_WaitServer() != GFCD_ERR_OK) {
	return 0;
    }
    if (asiz != NULL) {
	GFCF_GetFileInfo(flow,
			 NULL, NULL, &sctsiz, &fsct, &lastsiz, NULL, NULL);
	/* �ŏI�Z�N�^���܂ޏꍇ */
	if ((GFCB_Tell(dts) + nsct >= fsct) &&
	    (lastsiz != 0) && (nsct > 0)) {
	    *asiz -= sctsiz - lastsiz;
	}
    }
    return nsct;
}


/* CD�o�b�t�@�̋󂫃o�C�g���擾 */
Sint32 GFCB_GetLenRoom(GfsDtsrc *dts)
{
/*    return GFCB_GetSctRoom() * GFCF_MD1_SCTSIZ;*/
    return 0;				/* �������݂ł��Ȃ����� */
}


/* �i��̐ݒ� */
void GFCB_SetFilt(GfsDtsrc *dts, Sint32 fmode, CdcSubh *subh)
{
    Sint32 ret;

    GFS_CDR_FMODE(dts) = fmode;
    GFS_CDR_SUBH(dts) = *subh;
    ret = GFCD_SetFilt(GFS_CDR_FLT(dts), fmode, subh, 0, 0);
    if (ret != GFCD_ERR_OK) {
	return;
    }
    GFCD_WaitServer();
}


/* CD�o�b�t�@�S���� */
Sint32 GFCB_DeleteAllData(GfsDtsrc *dts)
{
    GFCD_DelSctData(GFS_CDR_BUFNO(dts), BUF_TOP, CDC_SNUM_END);
    return GFCD_WaitServer();
}


/* �A�N�Z�X�|�C���^�擾 */
Sint32 GFCB_Tell(GfsDtsrc *dts)
{
    return GFS_CDR_FILEPOS(dts) + GFS_CDR_SCTPOS(dts);
}
 
 
/*****************************************************************************
 *  �A�N�Z�X�|�C���^�ݒ�
 *�m���́n
 *  cdb		�b�c�o�b�t�@�n���h��
 *  off		�I�t�Z�b�g
 *  org		�ړ��̊(GFS_SEEK_~)
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �A�N�Z�X�|�C���^
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFCB_Seek(GfsFlow *flow, Sint32 off, Sint32 org)
{
    Sint32	nsct;
    Sint32	ret;
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    if (off < 0) {
        off = 0;
    }
    if (org == GFS_SEEK_CUR) {
	nsct = off;
	off = GFCB_Tell(dts) + nsct;
    } else {
	if (GFCB_Tell(dts) == off) {
	    return off;
	}
	/* �o�b�t�@�ɂ���Z�N�^���݂̂��擾���� */
	ret = GFCD_GetLenData(GFS_CDR_BUFNO(dts), BUF_TOP, CDC_SNUM_END, &nsct,
			      NULL);
	if (ret != GFCD_ERR_OK) {
	    return -1;
	}
	if (GFCD_WaitServer() != GFCD_ERR_OK) {
	    return -1;
	}
	if (nsct < 0) {
	    nsct = 0;
	}
    }
    if (GFS_FLW_GMODE(flow) == GFS_GMODE_ERASE) { /* �폜���[�h */
	/* �폜���[�h�ł̓A�N�Z�X�|�C���^���O�̃Z�N�^�f�[�^�͍폜���� */
        /* �o�b�t�@�ɓǂݍ��ݍς݂̈ʒu�Ɉړ�����ꍇ */
        if ((GFS_CDR_FILEPOS(dts) <= off) &&
	    off < GFCB_Tell(dts)) {
            nsct = off - GFS_CDR_FILEPOS(dts);
        }
	if (nsct > 0) {
	    /* �o�b�t�@�f�[�^���� */
	    GFCD_DelSctData(GFS_CDR_BUFNO(dts), BUF_TOP, nsct);
	    if (GFCD_WaitServer() != GFCD_ERR_OK) {
		return -1;
	    }
	}
	GFS_CDR_FILEPOS(dts) = off;		/* �o�b�t�@�擪�ʒu */
	GFS_CDR_SCTPOS(dts) = 0;		/* �]���ʒu */
    } else if (GFS_FLW_GMODE(flow) == GFS_GMODE_RESIDENT) {    /* �풓���[�h */
        /* �b�c�o�b�t�@�ɓǂݍ��܂�Ă��Ȃ��ʒu�Ɉړ�����ꍇ */
	/* 1.�o�b�t�@���f�[�^���O�ɃV�[�N */
	/* 2.�o�b�t�@���f�[�^�����ŘA���łȂ��i�P�Z�N�^�ȏシ���Ԃ��󂭁j*/
        if ((off < GFS_CDR_FILEPOS(dts)) ||
	    (off > (GFCB_Tell(dts) + 1))) {
	    /* �b�c�o�b�t�@�N���A */
            if (GFCB_DeleteAllData(dts) == GFCD_ERR_OK) {
		return -1;
	    }
            GFS_CDR_FILEPOS(dts) = off;
        }
        GFS_CDR_SCTPOS(dts) = off - GFS_CDR_FILEPOS(dts);
    }
    return GFCB_Tell(dts);
}
 
 

/*--- �f�[�^�]�� -----------------------------------------------------*/

/*****************************************************************************
 *  �]�����f�[�^�p�b�N�擾
 *�m���́n
 *  flow
 *  nsct	�]���v���Z�N�^��
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �f�[�^�p�b�N�n���h��
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
GfdpHn GFCB_GetSrcPack(GfsFlow *flow, Sint32 usct)
{
    Sint32      len, ret, nsct;
    Sint32	sctsiz, fsct, lastsiz;
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);
    GfdpHn      dpk = &MNG_SRCPK(gfs_mng_ptr);

    /* �g�p���̏ꍇ */
    if (GFDP_USE(dpk)) {
        return NULL;
    }

    /* �]���T�C�Y�����߂� */
    if (GFCD_GetLenData(GFS_CDR_BUFNO(dts), GFS_CDR_SCTPOS(dts),
			usct, &nsct, &len) != GFCD_ERR_OK) {
	return NULL;
    }
    if (GFCD_WaitServer() != GFCD_ERR_OK) {
	return NULL;
    }
    if (len <= 0) {
	return NULL;
    }
    GFCF_GetFileInfo(flow, 
		     NULL, NULL, &sctsiz, &fsct, &lastsiz, NULL, NULL);
    /* �ŏI�Z�N�^���܂ޏꍇ */
    if ((GFCB_Tell(dts) + nsct >= fsct) &&
	(lastsiz != 0) && (nsct > 0)) {
	len -= sctsiz - lastsiz;
    }
    GFDP_DATA(dpk) = CDC_GetDataPtr();	/* �]���A�h���X     */
    GFDP_ADLT(dpk) = CDB_ADDR_DLT;	/* �A�h���X���Z��   */
    GFDP_LEN(dpk) = len;		/* �]���o�C�g��     */
    GFDP_NSCT(dpk) = nsct;		/* �]���Z�N�^��     */
    GFDP_USE(dpk) = TRUE;		/* �f�[�^�p�b�N�g�p��   */
    return dpk;
}


/* �f�[�^�p�b�N���擾������̃f�[�^�v�� */
Bool GFCB_GetSctData(GfdpHn dpk, GfsDtsrc *dts)
{
    Sint32 ret;

    ret = GFCD_ReqData(GFS_CDR_BUFNO(dts), GFS_CDR_SCTPOS(dts),
		       GFDP_NSCT(dpk));
    if (ret != GFCD_ERR_OK) {
	return FALSE;
    }
    if (GFCD_WaitServer() != GFCD_ERR_OK) {
	return FALSE;
    }
    return TRUE;
}


/* �]����f�[�^�p�b�N�擾 */
GfdpHn GFCB_GetDstPack(GfsDtsrc *dts, Sint32 len)
{
    return NULL;
}


/*  �f�[�^�p�b�N�Ԋ� */
void GFCB_RtnPk(GfsFlow *flow, GfdpHn dpk, Sint32 nsct)
{
    Sint32	txw;
    Sint32      seekpos;
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    GFCD_EndData(&txw);
    /* �]�����I�����Ă���̂�,�A�N�Z�X�|�C���^��i�߂�  */
    if (nsct == GFTR_ALLSCT) {
	nsct = GFDP_NSCT(dpk);
    }
    GFCB_Seek(flow, nsct, GFS_SEEK_CUR);
    /* �f�[�^�p�b�N�Ԋ� */
    GFDP_USE(dpk) = FALSE;		/* �f�[�^�p�b�N�g�p������ */
}


/* �s�b�N�A�b�v�̈ړ� */
Sint32 GFCB_MovePickup(GfsDtsrc *dts, CdcPos *pos)
{
    return GFCD_MovePickup(pos, GFS_CDR_PUID(dts));
}


/* CD�f�o�C�X�ڑ� */
Bool GFCB_SetCon(GfsDtsrc *dts)
{
    Sint32 ec;

    ec = GFCD_SetCon(GFS_CDR_FLT(dts), GFS_CDR_PUID(dts));
    if (ec != GFCD_ERR_OK) {
	return FALSE;
    }
    ec = GFCD_WaitServer();
    if (ec != GFCD_ERR_OK) {
	return FALSE;
    }
    return TRUE;
}


/* �s�b�N�A�b�v�̐��䌠���m�ۂ��� */
Bool GFCB_GetPickup(GfsDtsrc *dts)
{
    Sint32	puid = GFCD_GetPickup();
    
    if (puid < GFCD_ERR_OK) {
	return FALSE;
    }
    GFS_CDR_PUID(dts) = puid;
    return TRUE;
}


/* �s�b�N�A�b�v���m�ۂ��Ă��邩���ׂ� */
Bool GFCB_IsPuOwner(GfsDtsrc *dts)
{
    return GFCD_IsPuOwner(GFS_CDR_PUID(dts));
}


/* �s�b�N�A�b�v��������� */
void GFCB_UngetPickup(GfsDtsrc *dts)
{
    GFCD_UngetPickup(GFS_CDR_PUID(dts));
}


#if	0
/* CD�o�b�t�@�̋󂫃Z�N�^���擾 */
Sint32 GFCB_GetSctRoom(void)
{
    Sint32      freesiz;		/* �Z�N�^�P��   */

    freesiz = GFCD_GetBufSiz();
    if (freesiz < 0) {
	freesiz = 0;
    }
    return freesiz;
}
#endif

/* end of file */
