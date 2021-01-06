/*****************************************************************************
 *	�\�t�g�E�F�A���C�u����
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:�t�@�C���V�X�e��
 * Module :SCSI�o�b�t�@�Ǘ�
 * File	  :gfs_sfb.c
 * Date   :1994-11-11
 * Version:1.16
 * Auther :M.S.
 *
 *****************************************************************************/

/*****************************************************************************
 *	�C���N���[�h�t�@�C��
 *****************************************************************************/
#include <limits.h>
#include "sega_gfs.h"
#include "gfs_def.h"
#include "gfs_sf.h"
#include "gfs_scsi.h"
#include "gfs_trn.h"

/*****************************************************************************
 *	�萔�}�N��
 *****************************************************************************/
#define GFSB_TPARA_DFL	1		/* SCSI�ł�1�Z�N�^���]������ */

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

/* �A�N�Z�X�ʒu����w��Z�N�^���̗L���f�[�^�o�C�g�����擾 */
GFS_LOCAL Sint32 gfsb_getActSiz(GfsFlow *flow, Sint32 usct, Sint32 *len);

/*****************************************************************************
 *	�ϐ��̒�`
 *****************************************************************************/

/* �f�[�^�p�b�N�Ǘ�     */
GfsDataPack gfsb_dtpk;


/*****************************************************************************
 *	�֐��̒�`
 *****************************************************************************/
/* SCSI�p�f�[�^�p�b�N�̏����� */
void GFSB_Init(void)
{
    GfdpHn      dpk = &gfsb_dtpk;
    
    GFDP_DATA(dpk) = NULL;
    GFDP_ADLT(dpk) = 0;
    GFDP_LEN(dpk) = 0;
    GFDP_NSCT(dpk) = 0;
    GFDP_USE(dpk) = FALSE;
}


/* SCSI�t�@�C���o�b�t�@�̐ݒ� */
void GFSB_Setup(GfsFlow *flow, Sint32 fid)
{
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    GFS_DTS_FTYPE(dts) = SCSI_FILE;
    GFS_SCR_FID(dts) = fid;
    GFS_SCR_FILEPOS(dts) = GFS_SCR_SCTPOS(dts) = GFS_SCR_SCTNUM(dts) = 0;
}


/*****************************************************************************
 *  SCSI�t�@�C���̗L���Z�N�^�������߂�
 *�m���́n
 *  flow
 *  usct	�]���v���Z�N�^��
 *  nsct	�]���\�o�C�g��
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  �]���\�Z�N�^��
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFSB_GetLenData(GfsFlow *flow, Sint32 usct, Sint32 *len)
{
    Sint32      nsct, lastsize, ap;
    GfsFinfo	*finfo = &GFS_FLW_FINFO(flow);
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    if (usct == GFTB_BUF_END) {
	usct = LONG_MAX;
    }
    nsct = GFS_SCR_SCTNUM(dts) - GFS_SCR_SCTPOS(dts);
    nsct = MIN(nsct, usct);
    if (len != NULL) {
	*len = nsct * SFM_SCTSIZE;
	ap = GFS_SCR_FILEPOS(dts) + GFS_SCR_SCTPOS(dts); 
	/* �ŏI�Z�N�^���܂ޏꍇ */
	if (ap > GFS_FI_NSCT(finfo)) {
	    lastsize = GFS_FI_SIZE(finfo) % SFM_SCTSIZE;
	    if ((*len != 0) && (lastsize != 0)) {
		*len -= SFM_SCTSIZE - lastsize;
	    }
	}
    }
    return nsct;
}


/* �]�����f�[�^�p�b�N�擾 */
GfdpHn GFSB_GetSrcPack(GfsFlow *flow, Sint32 usct)
{
    Sint32      len, nsct;
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);
    GfdpHn      dpk = &gfsb_dtpk;
    
    /* �g�p���̏ꍇ */
    if (GFDP_USE(dpk)) {
        return NULL;
    }
    GFDP_DATA(dpk) = GFSD_GetBuf();
    GFDP_ADLT(dpk) = sizeof(Uint8);	/* �A�h���X���Z�� */
    nsct = gfsb_getActSiz(flow, usct, &len);
    /* SCSI�o�b�t�@�ɂP�Z�N�^�ǂݍ��� */
    GFSD_Read(GFS_SCR_FID(dts), GFS_SCR_FILEPOS(dts) + GFS_SCR_SCTPOS(dts));
    GFDP_LEN(dpk) = len;
    GFDP_NSCT(dpk) = nsct;		/* �]���Z�N�^�� */
    GFDP_USE(dpk) = TRUE;		/* �f�[�^�p�b�N�g�p�� */
    return dpk;
}


/* �f�[�^�p�b�N�Ԋ� */
void GFSB_RtnPk(GfsFlow *flow, GfdpHn dtpk, Sint32 nsct)
{
    Sint32      seekpos;
    GfdpHn      dpk = &gfsb_dtpk;
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    /* �g�p���̃f�[�^�p�b�N��ϊ����邱�Ƃ��m�F */
    if (GFDP_USE(dpk)) {
        /* �]�����I�����Ă���ꍇ,�A�N�Z�X�|�C���^��i�߂�  */
	if (nsct == GFTR_ALLSCT) {
	    nsct = GFDP_NSCT(dpk);
	}
	GFSF_Seek(flow, GFSF_Tell(flow) + nsct);
	/* �f�[�^�p�b�N�Ԋ� */
	GFDP_USE(dpk) = FALSE;		/* �f�[�^�p�b�N�g�p������ */
    }
}


/*---------------------------------------------------------------------------
 * ����J�֐�
 *---------------------------------------------------------------------------*/


/* �A�N�Z�X�ʒu����w��Z�N�^���̗L���f�[�^�o�C�g�����擾 */
GFS_LOCAL Sint32 gfsb_getActSiz(GfsFlow *flow, Sint32 usct, Sint32 *len)
{
    Sint32	nsct;
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    nsct = GFS_SCR_SCTNUM(dts) - GFS_SCR_SCTPOS(dts);
    nsct = MIN(nsct, usct);
    nsct = MIN(nsct, GFSB_TPARA_DFL);
    *len = nsct * SFM_SCTSIZE;
    return nsct;
}

/* end of file */
