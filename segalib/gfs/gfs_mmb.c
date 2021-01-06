/*****************************************************************************
 *	�\�t�g�E�F�A���C�u����
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:�t�@�C���V�X�e��
 * Module :SIMM�t�@�C���o�b�t�@�Ǘ�
 * File	  :gfs_mmb.c
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
#include "gfs_mmc.h"
#include "gfs_mmb.h"
#include "gfs_trn.h"

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

/*****************************************************************************
 *	�ϐ��̒�`
 *****************************************************************************/

/* �f�[�^�p�b�N�Ǘ�     */
GfsDataPack gfmb_dtpk;

/*****************************************************************************
 *	�֐��̒�`
 *****************************************************************************/
/* �������t�@�C���p�f�[�^�p�b�N�̏����� */
void GFMB_Init(void)
{
    GfdpHn	dtk = &gfmb_dtpk;
    
    GFDP_DATA(dtk) = NULL;
    GFDP_ADLT(dtk) = 0;
    GFDP_LEN(dtk) = 0;
    GFDP_NSCT(dtk) = 0;
    GFDP_USE(dtk) = FALSE;
}


/* �������o�b�t�@�̐ݒ� */
void GFMB_Setup(GfsFlow *flow, Sint32 fid)
{
    void	*area;
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);
    
    if (fid >= 0) {
	GFMC_GetFileInfo(fid, &area, NULL, NULL);
    } else {
	area = NULL;
    }
    GFS_DTS_FTYPE(dts) = MEM_FILE;
    GFS_MEMR_DATA(dts) = area;
    GFS_MEMR_FILEPOS(dts) = GFS_MEMR_SCTPOS(dts) = GFS_MEMR_SCTNUM(dts) = 0;
}


/*****************************************************************************
 *  �������o�b�t�@�̗L���Z�N�^�������߂�
 *�m���́n
 *  flow	flow-in�Ǘ��\����
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
Sint32 GFMB_GetLenData(GfsFlow *flow, Sint32 usct, Sint32 *len)
{
    Sint32	ap;
    Sint32      nsct;
    Sint32	lastsize;
    GfsFinfo	*finfo = &GFS_FLW_FINFO(flow);
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    if (usct == GFTB_BUF_END) {
	usct = LONG_MAX;
    }
    nsct = GFS_MEMR_SCTNUM(dts) - GFS_MEMR_SCTPOS(dts);
    nsct = MIN(nsct, usct);
    if (len != NULL) {
	*len = nsct * MMF_SCTSIZE;
	ap = GFS_MEMR_FILEPOS(dts) + GFS_MEMR_SCTPOS(dts);
	/* �ŏI�Z�N�^���܂ޏꍇ */
	if (ap > GFS_FI_NSCT(finfo) &&
	    lastsize != 0 && GFS_MEMR_SCTPOS(dts) > 0) {
	    lastsize = GFS_FI_SIZE(finfo) % MMF_SCTSIZE;
	    if ((*len != 0) && (lastsize != 0)) {
		*len -= MMF_SCTSIZE - lastsize;
	    }
	}
    }
    return nsct;
}


/* �]�����f�[�^�p�b�N�擾 */
GfdpHn GFMB_GetSrcPack(GfsFlow *flow, Sint32 nsct)
{
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);
    GfdpHn	dtk = &gfmb_dtpk;
    
    /* �f�[�^�p�b�N���g�p���̏ꍇ   */
    if (GFDP_USE(dtk)) {
        return NULL;
    }
    /* �f�[�^�̈� */
    GFDP_DATA(dtk) = (Uint8 *)GFS_MEMR_DATA(dts) +
	(GFS_MEMR_FILEPOS(dts) +GFS_MEMR_SCTPOS(dts)) * MMF_SCTSIZE;
    GFDP_ADLT(dtk) = sizeof(Uint8);	/* �A�h���X���Z�� */
    GFDP_LEN(dtk) = nsct * MMF_SCTSIZE;	/* �o�C�g�� */
    GFDP_NSCT(dtk) = nsct;		/* �Z�N�^�� */
    GFDP_USE(dtk) = TRUE;		/* �g�p���t���O�ݒ� */
    return dtk;
}


/* �]�����f�[�^�p�b�N�Ԋ� */
void GFMB_RtnPk(GfsFlow *flow, GfdpHn dpk, Sint32 nsct)
{
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);
    GfdpHn	dtk = &gfmb_dtpk;

    /* �]�����I�����Ă���ꍇ   */
    if (nsct == GFTR_ALLSCT) {
	nsct = GFDP_LEN(dpk) / MMF_SCTSIZE;
    }
    GFMF_Seek(flow, GFMF_Tell(flow) + nsct);
    GFDP_USE(dtk) = FALSE;		/* �t���O���� */
}


/* end of file */
