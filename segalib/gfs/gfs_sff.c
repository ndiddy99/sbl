/*****************************************************************************
 *	�\�t�g�E�F�A���C�u����
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:�t�@�C���V�X�e��
 * Module :SCSI�t�@�C��
 * File	  :gfs_sff.c
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
#include "gfs_scsi.h"
#include "gfs_sf.h"

/*****************************************************************************
 *	�萔�}�N��
 *****************************************************************************/
#define GFSF_RPARA_DFL	1		/* SCSI�ł�1�Z�N�^����flow in���� */

/* �f�B�b�v�X�C�b�`�̐ݒ�𓾂邽�߂̃A�h���X */
#define BOOT_WORK_DIPSW	(*(Sint32 *)0x6000248)

/* SCSI�g�p�r�b�g:�f�B�b�v�X�C�b�`��bit4 */
#define DIP_USE_SCSI	0x10

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

/*****************************************************************************
 *	�֐��̒�`
 *****************************************************************************/

/* SCSI�t�@�C���Ǘ��̏����� */
void GFSF_Init(void)
{
    MNG_FLAGS(gfs_mng_ptr) &= ~MNG_SCSICON_MSK;
    if (BOOT_WORK_DIPSW & DIP_USE_SCSI) {
	MNG_FLAGS(gfs_mng_ptr) |= MNG_SCSICON_MSK;
    }
    GFSB_Init();
}


/*****************************************************************************
 *  SCSI�t�@�C���n���h���̏�����
 *�m���́n
 *  flow	flow-in�Ǘ��f�[�^
 *  dirrec	�f�B���N�g�����R�[�h�̈�
 *  fid		�t�@�C�����ʎq
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  SCSI�t�@�C���n���h��
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
Bool GFSF_Setup(GfsFlow *flow, GfsDirId *dirrec, Sint32 fid)
{
    Sint32	fsize;
    GfsFinfo	*finfo = &GFS_FLW_FINFO(flow);

    if (GFSD_Open(GFS_DIR_FAD(dirrec)) != GFSD_ERR_OK) {
	return FALSE;
    }
    fsize = GFS_DIR_SIZE(dirrec);
    /* �t�@�C�����ݒ� */
    GFS_FI_FID(finfo) = fid;
    GFS_FI_FAD(finfo) = 0;
    GFS_FI_SIZE(finfo) = fsize;
    GFS_FI_UNIT(finfo) = GFS_DIR_UNIT(dirrec);
    GFS_FI_GAP(finfo) = GFS_DIR_GAP(dirrec);
    GFS_FI_FN(finfo) = 0;
    GFS_FI_ATR(finfo) = GFS_DIR_ATR(dirrec);
    GFS_FI_SCTSZ(finfo) = SFM_SCTSIZE;
    GFS_FI_NSCT(finfo) = GFS_BYTE_SCT(fsize, SFM_SCTSIZE);
    GFS_FI_LSTRM(finfo) = GFS_LAST_ROOM(fsize, SFM_SCTSIZE);
    /* SCSI�t�@�C���Ǘ���� */
    GFSB_Setup(flow, GFS_DIR_FAD(dirrec));
    /* flow-in�Ǘ��f�[�^ */
    GFS_FLW_GMODE(flow) = GFS_GMODE_ERASE;
    GFS_FLW_SCT(flow) = GFS_FLW_SCTCNT(flow) = 0;
    GFS_FLW_SCTMAX(flow) = GFS_RPARA_DFL;
    return TRUE;
}



/*****************************************************************************
 *  SCSI�t�@�C������̃f�[�^�ǂݍ���
 *�m���́n
 *  flow	flow-in�Ǘ��\����
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  	�ǂݍ��ݏ��(���GFS_FIN_END)
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFSF_FlowInBuf(GfsFlow *flow)
{
    Sint32	remain;
    Sint32	nsct;
    GfsFinfo	*finfo = &GFS_FLW_FINFO(flow);
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);
    
    remain = GFS_FLW_SCT(flow) - GFS_FLW_SCTCNT(flow);
    if (remain <= 0) {
	return GFS_FIN_END;
    }
    nsct = MIN(remain, GFS_FLW_SCTMAX(flow));
    GFS_FLW_SCTCNT(flow) += nsct;
    /* �o�b�t�@���ɓ����Ă���Z�N�^���l�� */
    nsct -= GFS_SCR_SCTNUM(dts) - GFS_SCR_SCTPOS(dts);
    if (nsct > 0) {
	GFS_SCR_SCTNUM(dts) += nsct;
    }
    /* �ǂݍ��ݏI���̏ꍇ */
    if (GFS_FLW_SCTCNT(flow) >= GFS_FLW_SCT(flow)) {
	return GFS_FIN_END;		/* �ǂݍ��݂��������� */
    } else {
	return GFS_FIN_START;		/* �f�[�^��ǂݍ��� */
    }
}


void GFSF_StopInBuf(GfsFlow *flow, Bool stop_flag)
{
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    if (stop_flag) {
	GFS_SCR_SCTPOS(dts) = GFS_SCR_SCTNUM(dts) = 0;
    }
}


/* �A�N�Z�X�|�C���^�ݒ� */
Sint32 GFSF_Seek(GfsFlow *flow, Sint32 pos)
{
    Sint32      nsct, ofs;
    GfsFinfo	*finfo = &GFS_FLW_FINFO(flow);
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);
 
    nsct = GFS_FI_NSCT(finfo);		/* �t�@�C���T�C�Y   */
    /* �t�@�C���T�C�Y���z���ăV�[�N���Ȃ��悤�ɂ��� */
    if (pos > nsct) {
        pos = nsct;
    }
    if (pos < 0) {
        pos = 0;
    }
    if (GFS_SCR_FILEPOS(dts) <= pos &&
	pos <= GFS_SCR_FILEPOS(dts) + GFS_SCR_SCTNUM(dts)) {
	/* CD�o�b�t�@���ւ̃V�[�N */
	ofs = pos - GFS_SCR_FILEPOS(dts);
	if (GFS_FLW_GMODE(flow) == GFS_GMODE_RESIDENT) {
	    GFS_SCR_SCTPOS(dts) = ofs;
	} else {
	    GFS_SCR_FILEPOS(dts) = pos;
	    GFS_SCR_SCTNUM(dts) -= ofs;
	}
    } else {
	/* CD�o�b�t�@�O�ւ̃V�[�N */
	GFS_SCR_FILEPOS(dts) = pos;
	GFS_SCR_SCTPOS(dts) = 0;
	GFS_SCR_SCTNUM(dts) = 0;
    }
    return pos;
}


/* �A�N�Z�X�|�C���^�擾 */
Sint32 GFSF_Tell(GfsFlow *flow)
{
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);
 
    return GFS_SCR_FILEPOS(dts) + GFS_SCR_SCTPOS(dts);
}


/* SCSI�t�@�C���̃N���[�Y���� */
void GFSF_Close(GfsDtsrc *dts)
{
    GFSD_Close(GFS_SCR_FID(dts));
}


/* end of file */
