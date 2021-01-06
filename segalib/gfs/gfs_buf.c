/*****************************************************************************
 *	�\�t�g�E�F�A���C�u����
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:�t�@�C���V�X�e��
 * Module :�o�b�t�@����
 * File	  :gfs_buf.c
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
#include "gfs_buf.h"
#include "gfs_cdb.h"
#if defined(DEBUG_LIB)
#include "gfs_sf.h"
#include "gfs_mmb.h"
#endif

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
/* �o�b�t�@����֐��Ƃ̃C���^�t�F�[�X */
typedef struct {
    Sint32 (*getltran)(GfsFlow *flow, Sint32 usct, Sint32 *nsct);
    GfdpHn (*getsrcpk)(GfsFlow *flow, Sint32 nsct);
    Bool (*getsctdt)(GfdpHn dpk, GfsDtsrc *dts);
    void (*rtnpk)(GfsFlow *flow, GfdpHn dtpk, Sint32 nsct);
} GfsBufIf;


/*****************************************************************************
 *	�ϐ��̐錾
 *****************************************************************************/

/*****************************************************************************
 *	�֐��̐錾
 *****************************************************************************/
/* SCSI/SIMM�t�@�C���p�Z�N�^�f�[�^�擾 */
GFS_LOCAL Bool gfbf_getSctDt(GfdpHn dpk, GfsDtsrc *dts);

/*****************************************************************************
 *	�ϐ��̒�`
 *****************************************************************************/

/* �t�@�C����ʂ��Ƃ̃o�b�t�@����֐��e�[�u�� */
static const GfsBufIf bufif_func[FILE_TYPE] = {
    {GFCB_GetLenData, GFCB_GetSrcPack, GFCB_GetSctData, GFCB_RtnPk},
#if defined(DEBUG_LIB)
    {GFMB_GetLenData, GFMB_GetSrcPack, gfbf_getSctDt, GFMB_RtnPk},
    {GFSB_GetLenData, GFSB_GetSrcPack, gfbf_getSctDt, GFSB_RtnPk}
#endif
};

/*****************************************************************************
 *	�֐��̒�`
 *****************************************************************************/

/* �]���\�f�[�^���̎擾 */
Sint32 GFBF_GetTranData(GfsHn gfs, Sint32 rsct, Sint32 *len)
{
    GfsFlow	*flow = &GFS_FILE_FLOW(gfs);
    Sint32	ftype = GFS_DTS_FTYPE(&GFS_FLW_DTSRC(flow));

    return bufif_func[ftype].getltran(flow, rsct, len);
}


/* �]���\�Z�N�^���̎擾 */
Sint32 GFBF_GetNumData(GfsHn gfs)
{
    GfsFlow	*flow = &GFS_FILE_FLOW(gfs);
    Sint32	ftype = GFS_DTS_FTYPE(&GFS_FLW_DTSRC(flow));

    return bufif_func[ftype].getltran(flow, GFTB_BUF_END, NULL);
}


/* �]�����̃f�[�^�p�b�N�̎擾 */
GfdpHn GFBF_GetSrcPk(GfsHn gfs, Sint32 nsct)
{
    GfsFlow	*flow = &GFS_FILE_FLOW(gfs);
    Sint32	ftype = GFS_DTS_FTYPE(&GFS_FLW_DTSRC(flow));

    return bufif_func[ftype].getsrcpk(flow, nsct);
}


/* �Z�N�^�f�[�^�擾 */
Bool GFBF_GetSctData(GfdpHn dpk, GfsFlow *flow)
{
    Sint32	ftype = GFS_DTS_FTYPE(&GFS_FLW_DTSRC(flow));

    return bufif_func[ftype].getsctdt(dpk, &GFS_FLW_DTSRC(flow));
}


/* �f�[�^�p�b�N�̕Ԋ� */
void GFBF_RtnPk(GfsHn gfs, GfdpHn gpdp, Sint32 nsct)
{
    GfsFlow	*flow = &GFS_FILE_FLOW(gfs);
    Sint32	ftype = GFS_DTS_FTYPE(&GFS_FLW_DTSRC(flow));

    bufif_func[ftype].rtnpk(flow, gpdp, nsct);
}


/* SCSI/SIMM�t�@�C���p�Z�N�^�f�[�^�擾 */
GFS_LOCAL Bool gfbf_getSctDt(GfdpHn dpk, GfsDtsrc *dts)
{
    return TRUE;
}

/* end of file */
