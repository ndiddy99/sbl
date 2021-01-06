/*****************************************************************************
 *	�\�t�g�E�F�A���C�u����
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:�t�@�C���V�X�e��
 * Module :SIMM�t�@�C���A�N�Z�X
 * File	  :gfs_mmf.c
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
#include "gfs_mmf.h"
#include "gfs_mmb.h"
#include "gfs_mmc.h"

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

/*****************************************************************************
 *	�֐��̒�`
 *****************************************************************************/
/* MMF�̏����� */
Bool GFMF_Init(void)
{
    GFMB_Init();
    return (GFMC_GetBase() == NULL)? FALSE: TRUE;
}


/*****************************************************************************
 *  flow-in�Ǘ��\���̂̏�����
 *�m���́n
 *  flow	flow-in�Ǘ��\����
 *  dirrec	�f�B���N�g�����R�[�h
 *  fid		�t�@�C�����ʎq
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
void GFMF_Setup(GfsFlow *flow, GfsDirId *dirrec, Sint32 fid)
{
    Sint32      fsize;
    GfsFinfo	*finfo = &GFS_FLW_FINFO(flow);
    
    /* �t�@�C�����ݒ� */
    fsize = GFS_DIR_SIZE(dirrec);
    GFS_FI_FID(finfo) = fid;
    GFS_FI_FAD(finfo) = 0;
    GFS_FI_SIZE(finfo) = fsize;
    GFS_FI_UNIT(finfo) = GFS_DIR_UNIT(dirrec);
    GFS_FI_GAP(finfo) = GFS_DIR_GAP(dirrec);
    GFS_FI_FN(finfo) = 0;
    GFS_FI_ATR(finfo) = GFS_DIR_ATR(dirrec);
    GFS_FI_SCTSZ(finfo) = MMF_SCTSIZE;
    GFS_FI_NSCT(finfo) = GFS_BYTE_SCT(fsize, MMF_SCTSIZE);
    GFS_FI_LSTRM(finfo) = GFS_LAST_ROOM(fsize, MMF_SCTSIZE);
    /* �ǂݍ��݌��Ǘ����ݒ� */
    GFMB_Setup(flow, GFS_DIR_FAD(dirrec));
    /* ���̑��ǂݍ��݊Ǘ��f�[�^�ݒ� */
    GFS_FLW_GMODE(flow) = GFS_GMODE_ERASE;
    GFS_FLW_SCT(flow) = GFS_FLW_SCTCNT(flow) = 0;
    GFS_FLW_SCTMAX(flow) = GFS_RPARA_DFL;
}


/*****************************************************************************
 *  �������o�b�t�@�ւ̃f�[�^�ǂݍ���
 *�m���́n
 *  flow	flow-in�Ǘ��\����
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  �ǂݍ��ݏ��(GFS_FIN_START)
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFMF_FlowInBuf(GfsFlow *flow)
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
    nsct -= GFS_MEMR_SCTNUM(dts) - GFS_MEMR_SCTPOS(dts);
    if (nsct > 0) {
	GFS_MEMR_SCTNUM(dts) += nsct;
    }
    /* �ǂݍ��ݏI���̏ꍇ */
    if (GFS_FLW_SCTCNT(flow) >= GFS_FLW_SCT(flow)) {
	return GFS_FIN_END;		/* �ǂݍ��݂��������� */
    } else {
	return GFS_FIN_START;		/* �f�[�^��ǂݍ��� */
    }
}


void GFMF_StopInBuf(GfsFlow *flow, Bool stop_flag)
{
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    if (stop_flag) {
	GFS_MEMR_SCTPOS(dts) = GFS_MEMR_SCTNUM(dts) = 0;
    }
}


/* �A�N�Z�X�|�C���^�ݒ� */
Sint32 GFMF_Seek(GfsFlow *flow, Sint32 pos)
{
    Sint32      nsct, ofs;
    GfsFinfo	*finfo = &GFS_FLW_FINFO(flow);
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);
 
    nsct = GFS_FI_NSCT(finfo);		/* �t�@�C���T�C�Y */
    /* �t�@�C���T�C�Y���z���ăV�[�N���Ȃ��悤�ɂ��� */
    if (pos > nsct) {
        pos = nsct;
    }
    if (pos < 0) {
        pos = 0;
    }
    if (GFS_MEMR_FILEPOS(dts) <= pos &&
	pos <= GFS_MEMR_FILEPOS(dts) + GFS_MEMR_SCTNUM(dts)) {
	/* CD�o�b�t�@���ւ̃V�[�N */
	ofs = pos - GFS_MEMR_FILEPOS(dts);
	if (GFS_FLW_GMODE(flow) == GFS_GMODE_RESIDENT) {
	    GFS_MEMR_SCTPOS(dts) = ofs;
	} else {
	    GFS_MEMR_FILEPOS(dts) = pos;
	    GFS_MEMR_SCTNUM(dts) -= ofs;
	}
    } else {
	/* CD�o�b�t�@�O�ւ̃V�[�N */
	GFS_MEMR_FILEPOS(dts) = pos;
	GFS_MEMR_SCTPOS(dts) = 0;
	GFS_MEMR_SCTNUM(dts) = 0;
    }
    return pos;
}


/* �A�N�Z�X�|�C���^�擾 */
Sint32 GFMF_Tell(GfsFlow *flow)
{
    GfsDtsrc	*dts = &GFS_FLW_DTSRC(flow);

    return GFS_MEMR_FILEPOS(dts) + GFS_MEMR_SCTPOS(dts);
}


/* end of file */
