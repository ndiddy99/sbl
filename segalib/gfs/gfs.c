/*****************************************************************************
 *	�\�t�g�E�F�A���C�u����
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:�t�@�C���V�X�e��
 * Module :���C�u�����C���^�t�F�[�X
 * File	  :gfs.c
 * Date   :1994-11-11
 * Version:1.16
 * Auther :M.S.
 *
 *****************************************************************************/

/*****************************************************************************
 *	�C���N���[�h�t�@�C��
 *****************************************************************************/
#include <string.h>

#include "sega_gfs.h"
#include <sega_int.h>
#include "gfs_buf.h"
#include "gfs_def.h"
#include "gfs_mmf.h"
#include "gfs_cdf.h"
#include "gfs_cdb.h"
#include "gfs_dir.h"
#include "gfs_trn.h"
#include "gfs_cdc.h"

#if defined(DEBUG_LIB)
#include "gfs_sf.h"
#endif

/*****************************************************************************
 *	�萔�}�N��
 *****************************************************************************/
/* ��{�{�����[���f�B�X�N�v���^�Ɋւ���萔 */
#define PVD_FAD		166		/* FAD */
#define PVD_BYTSIZ	GFS_MD1_SCTSIZ	/* �傫���i�o�C�g�P�ʁj*/
#define PVD_SCTSIZ	1		/* �傫���i�Z�N�^�P�ʁj */

/* root�f�B���N�g�������݂���FAD�̉��� */
#define ROOT_FAD_LOW_LIM	(150+16)

#define ROOT_RECPOS	156		/* ���[�g�f�B���N�g���̈ʒu */

/* SCDQ��6.7ms�Ԋu�̎���1s�̃^�C���A�E�g���� */
#define FREAD_TMOUT_COUNT	3000

/* �G���[�R�[�h */
#define SVR_NOHNDL	(-1)		/* �T�[�o�Ƀn���h�������o�^ */

/*****************************************************************************
 *	�񋓒萔
 *****************************************************************************/

/*****************************************************************************
 *	�����}�N��
 *****************************************************************************/
/* LSN -> FAD�ϊ� */
#define LSN_TO_FAD(lsn)	((lsn) += 150)

/* �f�B���N�g�����R�[�h�ǂݍ���		*/
#define DIRREC_EXT(rec)		((rec) + 6)
#define DIRREC_LEN(rec)		((rec) + 14)

/*****************************************************************************
 *	�f�[�^�^�̒�`
 *****************************************************************************/

/*****************************************************************************
 *	�֐��̐錾
 *****************************************************************************/

/* �t�@�C������֐��e�[�u���̏����� */
GFS_LOCAL void	gfs_mngSetupFuncTbl(void);

/* �Ǘ��e�[�u���̏����� */
GFS_LOCAL void	gfs_mngInitWork(Sint32 open_max);

/* �t�@�C�����̃J�E���g */
GFS_LOCAL Sint32	gfs_mngGetNumFile(GfsDirTbl *dirtbl);

/* �I�[�v���O���[�v�̉�� */
GFS_LOCAL void	gfs_mngFreeGrp(GfsFile *gfs);

/* �t�@�C���A�N�Z�X�������� */
GFS_LOCAL void	gfs_mngTermAccess(GfsHn gfs);

/* �G���[�֐��̌Ăт��� */
GFS_LOCAL Sint32	gfs_mngSetErrCode(Sint32 code);

/* �A�N�Z�X�T�[�o�̌��� */
GFS_LOCAL Sint32	gfs_svrSearchHndl(GfsHn gfs);

/* �A�N�Z�X�T�[�o�ւ̓o�^ */
GFS_LOCAL void	gfs_svrAddHndl(GfsHn gfs);

/* �A�N�Z�X�T�[�o����폜 */
GFS_LOCAL void	gfs_svrRemoveHndl(GfsHn gfs);

/* �{�����[���f�B�X�N���v�^�̓ǂݍ��� */
GFS_LOCAL Sint32	gfs_loadVol(Uint8 *buf);

/* ���[�g�f�B���N�g�����̎擾 */
GFS_LOCAL Sint32	gfs_getRootDir(GfsDirId *dir);

/* ���[�g�f�B���N�g�����R�[�h�̓ǂݍ��� */
GFS_LOCAL Sint32	gfs_loadRootDir(GfsDirTbl *dirtbl);

/* �f�[�^���\�[�X�o�b�t�@�ɗ������� */
GFS_LOCAL Sint32	gfs_flowIn(GfsHn gfs);

/* ���[�U�̈�ւ̃f�[�^�̓]�� */
GFS_LOCAL Sint32	gfs_transData(GfsHn gfs);

/* �ǂݍ��݁^�������݃Z�N�^���ݒ� */
GFS_LOCAL void	gfs_SetSct(GfsFlow *flow, Sint32 sct);

/* �t�@�C�����̎擾 */
GFS_LOCAL void	gfs_GetFinfo(GfsFinfo *finfo, Sint32 *fid, Sint32 *fad,
			 Sint32 *sctsiz, Sint32 *nsct, Sint32 *lastsiz,
			 Sint32 *fno, Sint32 *atr);

/* �t�@�C���N���[�Y�������֐� */
GFS_LOCAL void	gfs_closeSub(GfsHn gfs);

/* ���o�����[�h�̎擾 */
GFS_LOCAL Sint32	gfs_getGmode(GfsHn gfs);

/* �����������������𒲂ׂ� */
GFS_LOCAL Bool	gfs_isTaskDone(GfsHn gfs);

/* flow in�̏�Ԃ��G���[�R�[�h�ɕϊ����� */
GFS_LOCAL Sint32	gfs_convFinStat(GfsHn gfs, Sint32 fstat);


Sint32 bobtest;

/*****************************************************************************
 *	�ϐ��̒�`
 *****************************************************************************/

/* �Ǘ��e�[�u�� */
GfsMng *gfs_mng_ptr;

/* �������t�@�C�����g�p�\�̎��� */
/* ���̕ϐ��Ƀ������t�@�C���̈�̐擪�A�h���X��ݒ肷�� */
Sint8   *GFMC_base;

/*****************************************************************************
 *	�֐��̒�`
 *****************************************************************************/

/*--- �f�e�r�C���^�t�F�[�X�֐� ---------------------------------------------*/

/*****************************************************************************
 *  �t�@�C���V�X�e���̏������ACD-ROM�̃}�E���g
 *�m���́n
 *  open_max	�I�[�v���ł���t�@�C���̍ő吔
 *  work	���C�u�����̍�Ɨ̈�
 *  dirtbl	�f�B���N�g�����Ǘ��̈�
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �f�B���N�g�����B�G���[�������͕��̒l�B
 *�m�@�\�n
 *  �t�@�C���V�X�e���̊Ǘ��e�[�u�������������ACD-ROM���[�g�f�B���N�g����
 *  �ǂݍ��ށB
 *�m���l�n
 *  DEBUG_LIB�ł�CD�u���b�N�t�@�C���V�X�e�����g���Ȃ��B
 *  CDBFS���g���ƁA�t�@�C�������f�o�b�O�t�@�C���ƍ������Ȃ����߁B
 *  
 *****************************************************************************/
Sint32 GFS_Init(Sint32 open_max, void *work, GfsDirTbl *dirtbl)
{
    Sint32	ndir, ret;
    
    /* �����`�F�b�N */
    if (open_max < 1 || GFS_OPEN_MAX < open_max) {
	return GFS_ERR_OPENMAX;
    }
    if (((Uint32)work) & 3) {
	return GFS_ERR_ALIGN;
    }
    if (dirtbl != NULL) {
	if ((GFS_DIRTBL_TYPE(dirtbl) != GFS_DIR_ID &&
	     GFS_DIRTBL_TYPE(dirtbl) != GFS_DIR_NAME) ||
	    (GFS_DIRTBL_NDIR(dirtbl) < 2) ||
	    (GFS_DIRTBL_DIRID(dirtbl) == NULL)) {
	    return GFS_ERR_DIRTBL;
	}
    }
    /* SCU-DMA0�I�����荞�݂������� */
    INT_ChgMsk(INT_MSK_DMA0, INT_MSK_NULL);

    gfs_mng_ptr = work;
    /* �e���W���[�������� */
    gfs_mngInitWork(open_max);		/* �}�l�[�W�������� */
    ret = GFCF_Init((dirtbl == NULL)? TRUE: FALSE); /* CDF������ */
#if defined(DEBUG_LIB)
    if (dirtbl == NULL) {
	/* DEBUG_LIB�ł�CD�u���b�N�t�@�C���V�X�e���𗘗p�ł��Ȃ� */
	return gfs_mngSetErrCode(GFS_ERR_CDBFS);
    }
    /* �Ȉ�CD�G�~�����[�^�h���C�o������ */
    GFMF_Init();			/* MMF������ */
    GFSF_Init();			/* SCSI�t�@�C�������� */
#else
    if (ret != GFS_ERR_OK) {
	return gfs_mngSetErrCode(ret);
    }
    /* ���i�łł�CD�K�{ */
    if (!MNG_CDCON(gfs_mng_ptr)) {
	return gfs_mngSetErrCode(GFS_ERR_CDNODISC);
    }
#endif /* defined(DEBUG_LIB) */
    /* �f�B���N�g���Ǘ����ǂ����邩 */
    if (dirtbl == NULL) {
	/* CD�u���b�N�t�@�C���V�X�e���Ńf�B���N�g���Ǘ����s�Ȃ� */
	ndir = GFDR_ChgRoot();
	if (ndir < 0) {
	    return gfs_mngSetErrCode(GFS_ERR_CDBFS);
	}
    } else {
	/* GFS�Ńf�B���N�g�����Ǘ����� */
	ndir = gfs_loadRootDir(dirtbl); /* ���[�g�f�B���N�g���ǂݍ���	*/
	if (ndir <= 0) {
	    return gfs_mngSetErrCode(ndir);
	}
    }
    GFS_SetDir(dirtbl);		/* �J�������g�f�B���N�g��<-���[�g */
    gfs_mngSetErrCode(GFS_ERR_OK);
    return ndir;
}


/*****************************************************************************
 *  �f�B���N�g�����̓ǂݍ���
 *�m���́n
 *  fid		�f�B���N�g���̎��ʎq
 *  dirtbl	�f�B���N�g�����Ǘ��\����
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �ǂݍ��񂾃f�B���N�g������Ԃ��B�G���[�̎��͕��̃G���[�R�[�h��Ԃ��B
 *�m�@�\�n
 *  �w�肵���f�B���N�g������dirtbl�ɓǂݍ��ށB
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFS_LoadDir(Sint32 fid, GfsDirTbl *dirtbl)
{
    GfsHn	gfs;
    Sint32	nsct;
    Sint32	readdir;

    if (dirtbl == NULL) {
	/* GFS_Init��dirtbl��NULL��ݒ肵�����`�F�b�N */
	if (DIRMNG_DIRID(&MNG_CURDIR(gfs_mng_ptr)) != NULL) {
	    return gfs_mngSetErrCode(GFS_ERR_CDBFS);
	}
	/* CD�u���b�N���̃f�B���N�g������fid�̂��̂�ݒ� */
	readdir = GFDR_ChgDir(fid);
	if (readdir < 0) {
	    return gfs_mngSetErrCode(readdir);
	}
	return readdir;
    }
    gfs = GFS_Open(fid);
    if (gfs == NULL) {
	return GFS_ERR_CODE(&MNG_ERROR(gfs_mng_ptr));
    }
    GFS_GetFileSize(gfs, NULL, &nsct, NULL);
    /* �f�B���N�g�����̓ǂݍ��� */
    readdir = GFDR_SetupDirTbl(gfs, dirtbl, nsct);
    GFS_Close(gfs);
    return readdir;
}


/*****************************************************************************
 *  �J�����g�f�B���N�g���̐ݒ�
 *�m���́n
 *  dirtbl	�f�B���N�g�����Ǘ��̈�
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �G���[�R�[�h
 *�m�@�\�n
 *  �w�肳�ꂽ�f�B���N�g�������J�����g�f�B���N�g���ɐݒ肷��A
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFS_SetDir(GfsDirTbl *dirtbl)
{
    GfsDirMng   *mng;

    mng = &MNG_CURDIR(gfs_mng_ptr);
    if (dirtbl == NULL) {
	/* GFS_Init��dirtbl��NULL��ݒ肵�����`�F�b�N */
	if (DIRMNG_DIRID(mng) != NULL) {
	    return gfs_mngSetErrCode(GFS_ERR_CDBFS);
	}
	/* CDBFS�Ńf�B���N�g���Ǘ����s�Ȃ� */
	DIRMNG_DIRID(mng) = NULL;
	DIRMNG_NDIR(mng) = DIRMNG_NFILE(mng) = 0;
    } else {
	/* GFS�Ńf�B���N�g���Ǘ����s�Ȃ� */
	DIRMNG_DIRTBL(mng) = *dirtbl;
	DIRMNG_NFILE(mng) = gfs_mngGetNumFile(dirtbl); /* �t�@�C�����ݒ� */
    }
    return gfs_mngSetErrCode(GFS_ERR_OK);
}


/*****************************************************************************
 *  �t�@�C��������t�@�C�����ʎq�ɕϊ�
 *�m���́n
 *  fname	�t�@�C����
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �ϊ������t�@�C�����ʎq�B�G���[���͕��̃G���[�R�[�h�B
 *�m�@�\�n
 *  �t�@�C�������t�@�C�����ʎq�ɕϊ�����B
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFS_NameToId(Sint8 *fname)
{
    Sint32	fid;
    GfsDirMng	*cdir = &MNG_CURDIR(gfs_mng_ptr);

    if (DIRMNG_DIRID(cdir) == NULL ||
	DIRMNG_TYPE(cdir) != GFS_DIR_NAME) {
	return gfs_mngSetErrCode(GFS_ERR_NONAME);
    }
    fid = GFDR_NameToId(DIRMNG_DIRNAME(cdir), fname, DIRMNG_NDIR(cdir));
    if (fid < 0) {
	return gfs_mngSetErrCode(GFS_ERR_NEXIST);
    }
    gfs_mngSetErrCode(GFS_ERR_OK);
    return fid;
}


/*****************************************************************************
 *  ���ʎq����t�@�C�����ւ̕ϊ�
 *�m���́n
 *  fid		�t�@�C�����ʎq
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �ϊ������t�@�C�����ւ̃|�C���^�B�G���[����NULL�B
 *�m�@�\�n
 *  �t�@�C�����ʎq���t�@�C�����ɕϊ�����B
 *�m���l�n
 *  
 *****************************************************************************/
Sint8 *GFS_IdToName(Sint32 fid)
{
    GfsDirName	*pdir;
    GfsDirMng	*cdir = &MNG_CURDIR(gfs_mng_ptr);

    /* CD�u���b�N���̃t�@�C�������g���� */
    if (DIRMNG_DIRID(cdir) == NULL ||
	DIRMNG_TYPE(cdir) != GFS_DIR_NAME) {
	gfs_mngSetErrCode(GFS_ERR_NONAME);
	return NULL;
    }
    /* �t�@�C�����ʎq�͈̔͂��`�F�b�N */
    if ((fid < 0) || (DIRMNG_NFILE(cdir) <= fid)) {
	gfs_mngSetErrCode(GFS_ERR_NEXIST);
	return NULL;
    }
    pdir = GFDR_GetInfoDirName(fid, DIRMNG_DIRNAME(cdir));
    gfs_mngSetErrCode(GFS_ERR_OK);
    return GFS_DIR_FNAME(pdir);
}


/*****************************************************************************
 *  �t�@�C���̃I�[�v��
 *�m���́n
 *  fid		�t�@�C�����ʎq
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �t�@�C���n���h���B�G���[����NULL�B
 *�m�@�\�n
 *  �w�肵���t�@�C�����J���B
 *�m���l�n
 *  
 *****************************************************************************/
GfsHn GFS_Open(Sint32 fid)
{
    GfsDirId	*dirrec;
    GfsHn	gfs;
    GfsDirMng	*cdir = &MNG_CURDIR(gfs_mng_ptr);

    if (DIRMNG_DIRID(cdir) != NULL &&
	((fid < 0) || (fid >= DIRMNG_NFILE(cdir)))) {
	gfs_mngSetErrCode(GFS_ERR_FID);
        return NULL;
    }
    gfs = GFS_mngAllocGrp();		/* �󂫃O���[�v�擾     */
    if (gfs  == NULL) {			/* �󂫗̈悪�����ꍇ   */
	gfs_mngSetErrCode(GFS_ERR_NOHNDL);
        return NULL;
    }
    /* �t�@�C�����擾 */
    if (DIRMNG_DIRID(cdir) == NULL) {
	dirrec = NULL;
    } else if (DIRMNG_TYPE(cdir) == GFS_DIR_ID) {
	dirrec = GFDR_GetInfoDirId(fid, DIRMNG_DIRID(cdir));
    } else {
	dirrec = (GfsDirId *)GFDR_GetInfoDirName(fid, DIRMNG_DIRNAME(cdir));
    }
    gfs = GFS_OpenSub(gfs, dirrec, fid);
    if (gfs == NULL) {
	gfs_mngSetErrCode(GFS_ERR_FID);
    } else {
	gfs_mngSetErrCode(GFS_ERR_OK);
    }
    return gfs;
}


/*****************************************************************************
 *  �t�@�C���̃N���[�Y
 *�m���́n
 *  gfs		�t�@�C���n���h��
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �Ȃ�
 *�m�@�\�n
 *  �w�肵���t�@�C�������B
 *�m���l�n
 *  
 *****************************************************************************/
void GFS_Close(GfsHn gfs)
{
    if (gfs != NULL) {
	gfs_mngTermAccess(gfs);		/* �A�N�Z�X�I�� */
	gfs_closeSub(gfs);
    }
    gfs_mngSetErrCode(GFS_ERR_OK);
}


/*****************************************************************************
 *  �A�N�Z�X�|�C���^�̈ړ�
 *�m���́n
 *  gfs		�t�@�C���n���h��
 *  ofs		�ړ��I�t�Z�b�g
 *  org		��ʒu
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �ړ���̃A�N�Z�X�|�C���^��Ԃ��B
 *�m�@�\�n
 *  �A�N�Z�X�|�C���^���ړ�����B
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFS_Seek(GfsHn gfs, Sint32 ofs, Sint32 org)
{
    GfsFileFunc	*func;
    Sint32 nsct;
    GfsFlow	*flow;
    GfsDtsrc	*dts;

    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    flow = &GFS_FILE_FLOW(gfs);
    dts = &GFS_FLW_DTSRC(flow);
    func = &MNG_FUNCTBL(gfs_mng_ptr, GFS_DTS_FTYPE(dts));
    GFS_GetFileSize(gfs, NULL, &nsct, NULL);
    if (org == GFS_SEEK_CUR) {		/* ���݈ʒu����V�[�N */
	ofs += (FUNC_TELL(func))(flow);
    } else if (org == GFS_SEEK_END) {	/* �I�[����V�[�N */
	ofs = nsct + ofs;
    } else if (org != GFS_SEEK_SET) {
	/* �s���Ȋ�ʒu */
	return gfs_mngSetErrCode(GFS_ERR_ORG);
    }
    if (ofs < 0) {
	return gfs_mngSetErrCode(GFS_ERR_SEEK);
    }
    if (ofs > nsct) {
	return gfs_mngSetErrCode(GFS_ERR_SEEK);
    }
    ofs = (FUNC_SEEK(func))(flow, ofs);
    gfs_mngSetErrCode(GFS_ERR_OK);
    return ofs;
}


/*****************************************************************************
 *  �A�N�Z�X�|�C���^�̎擾
 *�m���́n
 *  gfs		�t�@�C���n���h��
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �A�N�Z�X�|�C���^
 *�m�@�\�n
 *  �A�N�Z�X�|�C���^��Ԃ�
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFS_Tell(GfsHn gfs)
{
    GfsFileFunc	*func;
    Sint32	ofs;
    GfsFlow	*flow;
    GfsDtsrc	*dts;

    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    flow = &GFS_FILE_FLOW(gfs);
    dts = &GFS_FLW_DTSRC(flow);
    func = &MNG_FUNCTBL(gfs_mng_ptr, GFS_DTS_FTYPE(dts));
    ofs = (FUNC_TELL(func))(flow);
    gfs_mngSetErrCode(GFS_ERR_OK);
    return ofs;
}


/*****************************************************************************
 *  �t�@�C���I���̃`�F�b�N
 *�m���́n
 *  gfs		�t�@�C���n���h��
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  TRUE	�A�N�Z�X�|�C���^���t�@�C�����ȍ~�ɂ���
 *  FALSE	���̑�
 *�m�@�\�n
 *  �A�N�Z�X�|�C���^�t�@�C�����ɒB�������ǂ������ׂ�B
 *�m���l�n
 *  
 *****************************************************************************/
Bool GFS_IsEof(GfsHn gfs)
{
    Sint32	pos, nsct;
	
    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    pos = GFS_Tell(gfs);
    GFS_GetFileSize(gfs, NULL, &nsct, NULL);
    gfs_mngSetErrCode(GFS_ERR_OK);
    /* �t�@�C���̏I�[�ɒB���Ă���ꍇ	*/
    return (pos >= nsct)? TRUE: FALSE;
}


/*****************************************************************************
 *  �o�C�g������Z�N�^���ւ̕ϊ�
 *�m���́n
 *  gfs		�t�@�C���n���h��
 *  nbyte	�o�C�g��
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �Z�N�^�P�ʂɕϊ���������
 *�m�@�\�n
 *  �o�C�g����Z�N�^�ւ̒P�ʂ̕ϊ����s�Ȃ��B
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFS_ByteToSct(GfsHn gfs, Sint32 nbyte)
{
    Sint32	sctsz;

    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    GFS_GetFileSize(gfs, &sctsz, NULL, NULL);
    if (sctsz == 0) {
	return 0;
    }
    gfs_mngSetErrCode(GFS_ERR_OK);
    return GFS_BYTE_SCT(nbyte, sctsz);
}


/*****************************************************************************
 *  �t�@�C���T�C�Y�̎擾
 *�m���́n
 *  gfs		�t�@�C���T�C�Y
 *�m�o�́n
 *  sctsz	�Z�N�^�T�C�Y
 *  nsct	���Z�N�^��
 *  lstlen	�ŏI�Z�N�^���L���f�[�^��
 *�m�֐��l�n
 *  �Ȃ�
 *�m�@�\�n
 *  �t�@�C���̑傫���Ɋւ�������擾����B
 *�m���l�n
 *  
 *****************************************************************************/
void GFS_GetFileSize(GfsHn gfs, Sint32 *sctsz, Sint32 *nsct, Sint32 *lstlen)
{
    Sint32	atr;
    GfsFlow	*flow;
	
    if (gfs == NULL) {
	gfs_mngSetErrCode(GFS_ERR_HNDL);
    } else {
	flow = &GFS_FILE_FLOW(gfs);
	gfs_GetFinfo(&GFS_FLW_FINFO(flow), NULL, NULL,
		     sctsz, nsct, lstlen, NULL, &atr);
	/* FORM1,FORM2�����݂��Ă���ꍇ�̓Z�N�^�T�C�Y��ݒ肵�Ȃ� */
	if (IS_FORM1(atr) && IS_FORM2(atr)) {
	    *sctsz = 0;
	}
    }
    gfs_mngSetErrCode(GFS_ERR_OK);
}


/*****************************************************************************
 *  �t�@�C�����̎擾
 *�m���́n
 *  gfs		�t�@�C���n���h��
 *�m�o�́n
 *  fid		�t�@�C�����ʎq
 *  fn		�t�@�C���ԍ�
 *  fsize	�傫���i�P�ʁF�o�C�g�j
 *  atr		����
 *�m�֐��l�n
 *  �Ȃ�
 *�m�@�\�n
 *  �t�@�C�������擾����B
 *�m���l�n
 *  
 *****************************************************************************/
void GFS_GetFileInfo(GfsHn gfs, Sint32 *fid, Sint32 *fn, Sint32 *fsize,
		     Sint32 *atr)
{
    Sint32	nsct;
    Sint32	lstlen;
    GfsFlow	*flow;

    if (gfs == NULL) {
	gfs_mngSetErrCode(GFS_ERR_HNDL);
    } else {
	flow = &GFS_FILE_FLOW(gfs);
	gfs_GetFinfo(&GFS_FLW_FINFO(flow), fid, NULL, NULL,
		     &nsct, &lstlen, fn, atr);
	if (atr != NULL) {
	    *atr &= ~GFS_ATR_FTYPE_MASK;	/* �t�@�C����ʏ��� */
	}
	if (fsize != NULL) {
	    *fsize = (nsct - 1) * GFS_MD1_SCTSIZ + lstlen;
	}
    }
    gfs_mngSetErrCode(GFS_ERR_OK);
}


/*****************************************************************************
 *  �t�@�C���̈ꊇ�ǂݍ���
 *�m���́n
 *  fid		�t�@�C�����ʎq
 *  ofs		�ǂݍ��݊J�n�ʒu
 *  buf		�ǂݍ��ݗ̈�
 *  bsize	�ǂݍ��ݗ̈�T�C�Y
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �ǂݍ��݃o�C�g���B�G���[���͕��̒l�B
 *�m�@�\�n
 *  �t�@�C�����ʎq�Ŏw�肳�ꂽ�t�@�C����ǂݍ��ށB
 *  �t�@�C���̃I�[�v���ƃN���[�Y�����̊֐��ōs�Ȃ��B
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFS_Load(Sint32 fid, Sint32 ofs, void *buf, Sint32 bsize)
{
    GfsHn	gfs;
    Sint32	nsct;		/* �J�n�ʒu����I�[�܂ł̃Z�N�^�� */
    Sint32	bsct;
    Sint32	sctsiz1, sctsiz2;
    Sint32	readlen;
    Sint32	ap;
    GfsFinfo	*finfo;

    gfs = GFS_Open(fid);
    if (gfs == NULL) {
	return GFS_ERR_CODE(&MNG_ERROR(gfs_mng_ptr));
    }
    ap = GFS_Seek(gfs, ofs, GFS_SEEK_SET);
    if (ap < 0) {
	return gfs_mngSetErrCode(GFS_ERR_OFS);
    }
    finfo = &GFS_FLW_FINFO(&GFS_FILE_FLOW(gfs));
    sctsiz1 = sctsiz2 = GFS_FI_SCTSZ(finfo);
    /* Form1, Form2���݂̏ꍇ�o�b�t�@�T�C�Y->�Z�N�^���ϊ����ő�l�ɂ���	*/
    if (sctsiz1 == 0) {
	sctsiz1 = GFS_FM2_SCTSIZ;
	sctsiz2 = GFS_FM1_SCTSIZ;
    }
    nsct = GFS_FI_NSCT(finfo) - ofs;	/* �t�@�C�����܂ł̃Z�N�^��	*/
    /* GFS_BUFSIZ_INF���w�肳�ꂽ��t�@�C�����܂œǂݍ��� */
    if (bsize == GFS_BUFSIZ_INF) {
	bsize = nsct * sctsiz1;
    }
    bsct = (bsize + sctsiz1 - 1) / sctsiz2;	/* �o�b�t�@�T�C�Y�ɑΉ�����Z�N�^��	*/
    nsct = MIN(nsct, bsct);			/* �ǂݍ��ރZ�N�^��			*/
    readlen = GFS_Fread(gfs, nsct, buf, bsize);
    GFS_Close(gfs);
    if (readlen >= 0) {
	gfs_mngSetErrCode(GFS_ERR_OK);
    }
    return readlen;
}
	    

/*****************************************************************************
 *  �f�[�^�̓ǂݍ���
 *�m���́n
 *  gfs		�t�@�C���n���h��
 *  nsct	�]���Z�N�^��
 *  buf		�ǂݍ��ݗ̈�
 *  bsize	�ǂݍ��ݗ̈�T�C�Y
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �ǂݍ��݃o�C�g��
 *�m�@�\�n
 *  �I�[�v�����ꂽ�t�@�C������f�[�^��ǂݍ��ށB
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFS_Fread(GfsHn gfs, Sint32 nsct, void *buf, Sint32 bsize)
{
    Sint32	ndata, err, amode, rpara_old, timer, atr;
    GfsFlow	*flow;
    GfsDtsrc	*dts;
    GfsTrans	*trn;
    CdcStat	stat;

    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    flow = &GFS_FILE_FLOW(gfs);
    /* �f�t�H���g�̓ǂݍ��݃p�����[�^���g�p */
    rpara_old = GFS_SetReadPara(gfs, GFS_RPARA_DFL);
    if (rpara_old != GFS_ERR_OK) {
	rpara_old = GFS_RPARA_DFL;
    }
    err = GFS_NwFread(gfs, nsct, buf, bsize);
    if (err != GFS_ERR_OK) {
	GFS_FLW_SCTMAX(flow) = rpara_old;
	return err;
    }
    dts = &GFS_FLW_DTSRC(flow);
    trn = &GFS_FILE_TRANS(gfs);
    GFS_GetFileInfo(gfs, NULL, NULL, NULL, &atr);
    for (timer = -1;;) {
	err = GFS_NwExecOne(gfs);
	if (err == GFS_SVR_COMPLETED) {
	    break;
	}
	if (err < 0) {
	    GFS_FLW_SCTMAX(flow) = rpara_old;
	    return err;
	}
	/* �]������ɏI������ */
	if (GFS_TRN_STAT(trn) == GFTR_ST_END) {
	    /* nsct���傫���l��GFS_NwCdRead����� */
	    /* �]��������ɏI��ꍇ������ */
	    break;
	}
	if (GFS_DTS_FTYPE(dts) == CD_FILE) {
	    if (timer < 0) {
		/* �܂��^�C���A�E�g�v�����n�܂��Ă��Ȃ� */
		GFCD_GetStat(&stat);
		if ((CDC_STAT_STATUS(&stat) & 0x0f) != CDC_ST_STANDBY &&
		    (atr & GFS_ATR_CDDA) == 0) {
		    /* <STANDBY>�łȂ��Ȃ�����^�C���A�E�g�v���J�n */
		    /* CDDA�t�@�C�������� */
		    timer = 0;
		}
	    }
	    /* SCDQ�t���O�����̂�҂��āAGFS_NwExecOne�̎��s�Ԋu�� */
	    /* ������x�󂯂� */
	    GFCD_WaitScdqFlag();
	    if (timer > FREAD_TMOUT_COUNT) {
		GFS_NwGetStat(gfs, &amode, &ndata); /* �]���o�C�g���擾 */
		if (ndata <= 0) {
		    GFS_FLW_SCTMAX(flow) = rpara_old;
		    return gfs_mngSetErrCode(GFS_ERR_TMOUT);
		}
	    }
	    if (timer >= 0) {
		++timer;
	    }
	}
    }
    GFS_NwGetStat(gfs, &amode, &ndata); /* �]���o�C�g���擾 */
    gfs_mngSetErrCode(GFS_ERR_OK);
    GFS_FLW_SCTMAX(flow) = rpara_old;
    return ndata;
}


/*****************************************************************************
 *  �f�[�^�ǂݍ��݂̊J�n
 *�m���́n
 *  gfs		�t�@�C���n���h��
 *  nsct	�ǂݍ��݃Z�N�^��
 *  buf		�ǂݍ��ݗ̈�
 *  bsize	�ǂݍ��ݗ̈�̑傫���i�P�ʁF�o�C�g�j
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �G���[�R�[�h
 *�m�@�\�n
 *  �ǂݍ��݃��N�G�X�g��o�^����B
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFS_NwFread(GfsHn gfs, Sint32 nsct, void *buf, Sint32 bsize)
{
    Sint32	remain, atr;
    Bool	add_flag = FALSE;
    GfsFlow	*flow;
    GfsFinfo	*finfo;
    GfsTrans	*trn;

    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    flow = &GFS_FILE_FLOW(gfs);
    finfo = &GFS_FLW_FINFO(flow);
    trn = &GFS_FILE_TRANS(gfs);
    remain = GFS_FI_NSCT(finfo) - GFS_Tell(gfs);
    remain = MIN(remain, nsct);		/* �ǂݍ��݃Z�N�^�� */
    if (remain < 0) {
	return gfs_mngSetErrCode(GFS_ERR_PARA);
    }
    if (GFS_FILE_AMODE(gfs) == GFS_NWSTAT_NOACT) {
	/* flow in�^�X�N��o�^ */
	gfs_SetSct(flow, remain);
	add_flag = TRUE;
    }
    GFS_FILE_AMODE(gfs) = GFS_NWSTAT_FREAD; /* �A�N�Z�X���[�h */
    GFS_GetFileInfo(gfs, NULL, NULL, NULL, &atr);
    if (atr & GFS_ATR_CDDA) {
	/* CDDA�t�@�C���͓]�����Ȃ� */
	GFTR_SetTransPara(trn, 0);	/* �]���Z�N�^�� */
	GFTR_SetData(trn, NULL, 0, 1);	/* �]����ݒ� */
    } else if (GFS_TRN_STAT(trn) == GFTR_ST_NOACT ||
	       GFS_TRN_STAT(trn) == GFTR_ST_END) {
	/* trans�^�X�N��o�^ */
	GFTR_SetTransPara(trn, remain); /* �]���Z�N�^�� */
	GFTR_SetData(trn, buf, bsize, 1); /* �]����ݒ� */
	GFTR_Start(trn);
	add_flag = TRUE;
    }
    if (add_flag == FALSE) {
	return gfs_mngSetErrCode(GFS_ERR_FBUSY);
    }
    gfs_svrAddHndl(gfs);	    	/* �A�N�Z�X�T�[�o�ւ̓o�^ */
    GFS_FILE_ASTAT(gfs) = GFS_SVR_BUSY;	/* �A�N�Z�X��� */
    return gfs_mngSetErrCode(GFS_ERR_OK);
}


/*****************************************************************************
 *  CD�o�b�t�@�ւ̐�ǂ݂̊J�n
 *�m���́n
 *  gfs		�t�@�C���n���h��
 *  nsct	�ǂݍ��݃Z�N�^��
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �G���[�R�[�h
 *�m�@�\�n
 *  CD�o�b�t�@�ւ̐�ǂ݃��N�G�X�g��o�^����B
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFS_NwCdRead(GfsHn gfs, Sint32 nsct)
{
    Sint32	remain;
    GfsFlow	*flow;
    GfsFinfo	*finfo;

    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    flow = &GFS_FILE_FLOW(gfs);
    finfo = &GFS_FLW_FINFO(flow);
    if (GFS_FILE_AMODE(gfs) != GFS_NWSTAT_NOACT) {
	return gfs_mngSetErrCode(GFS_ERR_FBUSY);
    }
    /* flow in�^�X�N�o�^ */
    remain = GFS_FI_NSCT(finfo) - GFS_Tell(gfs);
    remain = MIN(remain, nsct);		/* �ǂݍ��݃Z�N�^�� */
    gfs_SetSct(flow, remain);
    GFS_FILE_AMODE(gfs) = GFS_NWSTAT_CDREAD; /* �A�N�Z�X���[�h */
    GFS_FILE_ASTAT(gfs) = GFS_SVR_BUSY; /* �����t���O */
    gfs_svrAddHndl(gfs);		/* �A�N�Z�X�T�[�o�ւ̓o�^ */
    return gfs_mngSetErrCode(GFS_ERR_OK);
}


/*****************************************************************************
 *  �A�N�Z�X���슮���`�F�b�N
 *�m���́n
 *  gfs		�t�@�C���n���h��
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  TRUE	����
 *  FALSE	���쒆
 *�m�@�\�n
 *  �A�N�Z�X���삪�������������ׂ�
 *�m���l�n
 *  
 *****************************************************************************/
Bool GFS_NwIsComplete(GfsHn gfs)
{
    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    gfs_mngSetErrCode(GFS_ERR_OK);
    return (GFS_FILE_ASTAT(gfs) == GFS_SVR_COMPLETED)? TRUE: FALSE;
}


/*****************************************************************************
 *  �A�N�Z�X����̒��~
 *�m���́n
 *  gfs		�t�@�C���n���h��
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  ���݂̃A�N�Z�X�|�C���^
 *�m�@�\�n
 *  �A�N�Z�X������~������B
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFS_NwStop(GfsHn gfs)
{
    Sint32	ap;
	
    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    GFS_FILE_AMODE(gfs) = GFS_NWSTAT_NOACT;
    GFS_FILE_ASTAT(gfs) = GFS_SVR_COMPLETED; /* �����I�� */
    gfs_svrRemoveHndl(gfs);		/* �T�[�o����폜 */
    gfs_mngTermAccess(gfs);		/* �������� */
    ap = GFS_Tell(gfs);
    gfs_mngSetErrCode(GFS_ERR_OK);
    return ap;
}


/*****************************************************************************
 *  �A�N�Z�X��Ԃ̎擾
 *�m���́n
 *  gfs		�t�@�C���n���h��
 *�m�o�́n
 *  amode	�A�N�Z�X���[�h
 *  ndata	�]���f�[�^��
 *�m�֐��l�n
 *  �Ȃ�
 *�m�@�\�n
 *  �T�[�o�֐��̃A�N�Z�X���[�h�Ɠ]���f�[�^�����o�͂���B
 *  �]���f�[�^���̓A�N�Z�X���[�h���A
 *	GFS_NWSTAT_FREAD�̂Ƃ��A�]���o�C�g��
 *	GFS_NWSTAT_CDREAD�̂Ƃ��A�]���\�Z�N�^��
 *�m���l�n
 *  
 *****************************************************************************/
void GFS_NwGetStat(GfsHn gfs, Sint32 *amode, Sint32 *ndata)
{
    GfsTrans	*trn;
    Sint32	atr;

    if (gfs == NULL) {
	gfs_mngSetErrCode(GFS_ERR_HNDL);
    } else {
	trn = &GFS_FILE_TRANS(gfs);
	*amode = GFS_FILE_AMODE(gfs);
	GFS_GetFileInfo(gfs, NULL, NULL, NULL, &atr);
	if (atr & GFS_ATR_CDDA) {
	    /* CDDA�t�@�C���͓]�����s�Ȃ�Ȃ� */
	    *ndata = 0;
	} else if (GFS_FILE_AMODE(gfs) == GFS_NWSTAT_CDREAD) {
	    /* CD�o�b�t�@���Z�N�^�����o�� */
	    *ndata = GFBF_GetNumData(gfs);
	} else {
	    /* �]���o�C�g�����o�� */
	    *ndata = GFS_TRN_WP(trn);
	}
    }
    gfs_mngSetErrCode(GFS_ERR_OK);
}


/*****************************************************************************
 *  ���[�h�G���[�������̃��J�o������
 *�m���́n
 *  gfs		�t�@�C���n���h��
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �������
 *�m�@�\�n
 *  ���[�h�G���[�������̃v���C�p��
 *�m���l�n
 *  
 *****************************************************************************/
GFS_LOCAL Sint32	gfs_recovRdErr(GfsHn gfs)
{
    Sint32	fstat;
    GfsDtsrc	*dtsrc;
    GfsFlow	*flow;

    flow = &GFS_FILE_FLOW(gfs);
    dtsrc = &GFS_FLW_DTSRC(flow);
    if (GFS_DTS_FTYPE(dtsrc) == CD_FILE) {
	fstat = GFCF_Recover(flow);
	if (fstat == GFS_FIN_PLAY) {
	    GFS_FILE_ASTAT(gfs) = GFS_SVR_BUSY;	/* ���J�o������	*/
	}
    }
    return GFS_FILE_ASTAT(gfs);
}



/*****************************************************************************
 *  �t�@�C���P�ʂł̃A�N�Z�X����̎��s
 *�m���́n
 *  gfs		�t�@�C���n���h��
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �������
 *�m�@�\�n
 *  ���N�G�X�g�ɑ΂���A�N�Z�X�������s�Ȃ��B
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFS_NwExecOne(GfsHn gfs)
{
    Sint32	tstat, ec;

    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    if (GFS_FILE_ASTAT(gfs) != GFS_SVR_COMPLETED) {
	if (GFS_FILE_ASTAT(gfs) == GFS_ERR_CDRD) {
	    ec = gfs_recovRdErr(gfs);
	    return ec;
	}
	ec = gfs_flowIn(gfs);
	if (ec < 0) {
	    return ec;
	}
	tstat = gfs_transData(gfs);
	if (tstat == GFTR_ST_END) {
	    /* �]������flowin�̏�Ԃ��J�ڂ���ꍇ������̂� */
	    ec = gfs_flowIn(gfs);
	    if (ec < 0) {
		return ec;
	    }
	    if (gfs_isTaskDone(gfs)) {
		GFS_FILE_AMODE(gfs) = GFS_NWSTAT_NOACT;
		GFS_FILE_ASTAT(gfs) = GFS_SVR_COMPLETED;
		gfs_svrRemoveHndl(gfs);	/* �T�[�o����폜 */
	    }
	}
    }
    gfs_mngSetErrCode(GFS_ERR_OK);
    return GFS_FILE_ASTAT(gfs);
}


/*****************************************************************************
 *  �����t�@�C���̃A�N�Z�X����̎��s
 *�m���́n
 *  �Ȃ�
 *�m�o�́n
 *  now_gfs	�A�N�Z�X���t�@�C���̃n���h��
 *�m�֐��l�n
 *  �A�N�Z�X�T�[�o�̏��
 *�m�@�\�n
 *  ���N�G�X�g�֐��ɑ΂���t�@�C���A�N�Z�X�������s�Ȃ��B
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFS_NwExecServer(GfsHn *now_gfs)
{
    GfsSvr	*svr;
    Sint32	stat, i;

    svr = &MNG_SVR(gfs_mng_ptr);
    for (i = 0; i < SVR_NFILE(svr); ++i) {
	stat = GFS_NwExecOne(SVR_ACFILE(svr, i));
	if (stat < 0) {
	    break;
	}
	/* ���̃t�@�C���ɑ΂��Đ�ǂ݂����݂� */
	if (stat == GFS_SVR_COMPLETED) {
	    /* �҂��s�񂪋l�߂�ꂽ */
	    --i;
	}
    }
    if (SVR_NFILE(svr) == 0) {
	*now_gfs = NULL;
	stat = GFS_SVR_COMPLETED;	/* �����I�� */
    } else {
	*now_gfs = SVR_ACFILE(svr, 0);	/* �������n���h�� */
	if (stat == GFS_ERR_PUINUSE && i > 0) {
	    stat = GFS_SVR_BUSY;
	}
    }
    return stat;
}


/*****************************************************************************
 *  ���o�����[�h�̐ݒ�
 *�m���́n
 *  gfs		�t�@�C���n���h��
 *  gmode	���o�����[�h�iGFS_GMODE_ERASE, GFS_GMODE_RESIDENT�j
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �ݒ�O�̎��o�����[�h
 *�m�@�\�n
 *  ���o�����[�h��ݒ肷��B
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFS_SetGmode(GfsHn gfs, Sint32 gmode)
{
    Sint32 old_mode, atr;
    GfsFlow	*flow;
    GfsDtsrc	*dtsrc;

    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    /* CDDA�t�@�C���ɑ΂��Ă͎��o�����[�h��ύX�ł��Ȃ� */
    GFS_GetFileInfo(gfs, NULL, NULL, NULL, &atr);
    if (atr & GFS_ATR_CDDA) {
	return GFS_ERR_PARA;
    }
    flow = &GFS_FILE_FLOW(gfs);
    dtsrc = &GFS_FLW_DTSRC(flow);
    old_mode = GFS_FLW_GMODE(flow);
    GFS_FLW_GMODE(flow) = gmode;
    gfs_mngSetErrCode(GFS_ERR_OK);
    return old_mode;
}



/*****************************************************************************
 *  �]�����[�h�̐ݒ�
 *�m���́n
 *  gfs		�t�@�C���n���h��
 *  tmode	�]�����[�h�iGFS_TMODE_SCU,GFS_TMODE_SDMA[01],GFS_TMODE_CPU�j
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �ݒ�O�̓]�����[�h
 *�m�@�\�n
 *  �]�����[�h��ݒ肷��
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFS_SetTmode(GfsHn gfs, Sint32 tmode)
{
    GfsFlow	*flow;
    GfsDtsrc	*dtsrc;
    GfsTrans	*trans;
    Sint32	old_mode, atr;

    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    /* CDDA�t�@�C���ɑ΂��Ă͓]�����[�h��ύX�ł��Ȃ� */
    GFS_GetFileInfo(gfs, NULL, NULL, NULL, &atr);
    if (atr & GFS_ATR_CDDA) {
	return GFS_ERR_PARA;
    }
    flow = &GFS_FILE_FLOW(gfs);
    dtsrc = &GFS_FLW_DTSRC(flow);
    trans = &GFS_FILE_TRANS(gfs);
    old_mode = GFS_TRN_MODE(trans);
    GFS_TRN_MODE(trans) = tmode;
    old_mode = GFTR_SetMode(trans, tmode);
    gfs_mngSetErrCode(GFS_ERR_OK);
    return old_mode;
}


/*****************************************************************************
 *  CD�o�b�t�@�ւ̓ǂݍ��ݗʂ̐ݒ�
 *�m���́n
 *  gfs		�t�@�C���n���h��
 *  nsct	�P��̍ő�ǂݍ��ݗʁi�P�ʁF�Z�N�^�j
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �ݒ�O�̓ǂݍ��ݗ�
 *�m�@�\�n
 *  �P��̓ǂݍ��ݗʂ�ݒ肷��
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFS_SetReadPara(GfsHn gfs, Sint32 nsct)
{
    GfsFlow	*flow;
    Sint32	old_para, atr;

    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    /* CDDA�t�@�C���ɑ΂��Ă͓ǂݍ��ݗʂ�ύX�ł��Ȃ� */
    GFS_GetFileInfo(gfs, NULL, NULL, NULL, &atr);
    if (atr & GFS_ATR_CDDA) {
	return GFS_ERR_PARA;
    }
    flow = &GFS_FILE_FLOW(gfs);
    old_para = GFS_FLW_SCTMAX(flow);
    GFS_FLW_SCTMAX(flow) = nsct;
    gfs_mngSetErrCode(GFS_ERR_OK);
    return old_para;
}


/*****************************************************************************
 *  CD�o�b�t�@����̓]���ʂ̐ݒ�
 *�m���́n
 *  gfs		�t�@�C���n���h��
 *  tsize	�P��̓]���ʁi�P�ʁF�Z�N�^�j
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �ݒ�O�̓]����
 *�m�@�\�n
 *  �P��̍ő�]���ʂ�ݒ肷��
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFS_SetTransPara(GfsHn gfs, Sint32 tsize)
{
    Sint32 old_para, atr;

    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    /* CDDA�t�@�C���ɑ΂��Ă͓]���ʂ�ύX�ł��Ȃ� */
    GFS_GetFileInfo(gfs, NULL, NULL, NULL, &atr);
    if (atr & GFS_ATR_CDDA) {
	return GFS_ERR_PARA;
    }
    old_para = GFTR_SetTransMax(&GFS_FILE_TRANS(gfs), tsize);
    gfs_mngSetErrCode(GFS_ERR_OK);
    return old_para;
}



/*****************************************************************************
 *  CD�̃s�b�N�A�b�v�̈ړ�
 *�m���́n
 *  gfs		�t�@�C���n���h��
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �G���[�R�[�h
 *�m�@�\�n
 *  �s�b�N�A�b�v���A�N�Z�X�|�C���^�̈ʒu�Ɉړ�����B
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFS_CdMovePickup(GfsHn gfs)
{
    Sint32	ap, err;
    GfsFlow	*flow;
    GfsDtsrc	*dtsrc;

    if (gfs == NULL) {
	return gfs_mngSetErrCode(GFS_ERR_HNDL);
    }
    flow = &GFS_FILE_FLOW(gfs);
    dtsrc = &GFS_FLW_DTSRC(flow);
    if (GFS_DTS_FTYPE(dtsrc) == CD_FILE) {
	ap = GFS_Tell(gfs);
	err = GFCF_MovePickup(flow, ap);
	if (err != GFS_ERR_OK) {
	    return gfs_mngSetErrCode(err);
	}
	MNG_PICKUP(gfs_mng_ptr) = gfs;
    }
    return gfs_mngSetErrCode(GFS_ERR_OK);
}


/*****************************************************************************
 *  �G���[�����֐��̐ݒ�
 *�m���́n
 *  func	�G���[�����֐�
 *  obj		func�̑�P����
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �Ȃ�
 *�m�@�\�n
 *  �G���[�����֐���o�^����
 *�m���l�n
 *  
 *****************************************************************************/
void GFS_SetErrFunc(GfsErrFunc func, void *obj)
{
    GFS_ERR_FUNC(&MNG_ERROR(gfs_mng_ptr)) = func;
    GFS_ERR_OBJ(&MNG_ERROR(gfs_mng_ptr)) = obj;
    gfs_mngSetErrCode(GFS_ERR_OK);
}


/*****************************************************************************
 *  �G���[��Ԃ̎擾
 *�m���́n
 *  �Ȃ�
 *�m�o�́n
 *  stat	�G���[���
 *�m�֐��l�n
 *  �Ȃ�
 *�m�@�\�n
 *  �G���[��Ԃ��擾����
 *�m���l�n
 *  
 *****************************************************************************/
void GFS_GetErrStat(GfsErrStat *stat)
{
    *stat = MNG_ERROR(gfs_mng_ptr);
}


/*****************************************************************************
 *  �t�@�C�����ʎq����f�B���N�g�����R�[�h�擾
 *�m���́n
 *  fid		�t�@�C�����ʎq
 *�m�o�́n
 *  dirrec	�f�B���N�g�����R�[�h
 *�m�֐��l�n
 *  �G���[�R�[�h
 *�m�@�\�n
 *  �t�@�C�����ʎq����f�B���N�g�������擾����
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFS_GetDirInfo(Sint32 fid, GfsDirId *dirrec)
{
    GfsDirName	*pdirn;
    GfsDirId	*pdir;
    GfsDirMng	*cdir = &MNG_CURDIR(gfs_mng_ptr);

    /* CD�u���b�N���̃t�@�C�������g���� */
    /* �͈͊O�̎� */
    if ((DIRMNG_DIRID(cdir) == NULL) ||
	(fid < 0) || (fid >= DIRMNG_NFILE(cdir))) {
	return gfs_mngSetErrCode(GFS_ERR_FID);
    }
    if ((DIRMNG_TYPE(cdir) == GFS_DIR_NAME)) {
	pdirn = GFDR_GetInfoDirName(fid, DIRMNG_DIRNAME(cdir));
	memcpy(dirrec, pdirn, sizeof(GfsDirId));
    } else {
	pdir = GFDR_GetInfoDirId(fid, DIRMNG_DIRID(cdir));
	memcpy(dirrec, pdir, sizeof(GfsDirId));
    }
    return gfs_mngSetErrCode(GFS_ERR_OK);
}


/*--- �f�e�r�}�l�[�W�� -----------------------------------------------------*/

/*****************************************************************************
 *  �t�@�C������֐��e�[�u���̏�����
 *�m���́n
 *  �Ȃ�
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �Ȃ�
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
GFS_LOCAL void gfs_mngSetupFuncTbl(void)
{
    GfsFileFunc		*func;

    /* CD�t�@�C������֐��̐ݒ�	*/
    func = &MNG_FUNCTBL(gfs_mng_ptr, CD_FILE);
    FUNC_FLOWIN(func) = GFCF_FlowInBuf;
    FUNC_STOPIN(func) = GFCF_StopInBuf;
    FUNC_SEEK(func) = GFCF_Seek;
    FUNC_TELL(func) = GFCF_Tell;
#if defined(DEBUG_LIB)
    /* �������t�@�C������֐��̐ݒ�	*/
    func = &MNG_FUNCTBL(gfs_mng_ptr, MEM_FILE);
    FUNC_FLOWIN(func) = GFMF_FlowInBuf;
    FUNC_STOPIN(func) = GFMF_StopInBuf;
    FUNC_SEEK(func) = GFMF_Seek;
    FUNC_TELL(func) = GFMF_Tell;
    /* SCSI�t�@�C������֐��̐ݒ� */
    func = &MNG_FUNCTBL(gfs_mng_ptr, SCSI_FILE);
    FUNC_FLOWIN(func) = GFSF_FlowInBuf;
    FUNC_STOPIN(func) = GFSF_StopInBuf;
    FUNC_SEEK(func) = GFSF_Seek;
    FUNC_TELL(func) = GFSF_Tell;
#endif
}


/*****************************************************************************
 *  �Ǘ��e�[�u���̏�����
 *�m���́n
 *  open_max	�����I�[�v���ő吔
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �Ȃ�
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
GFS_LOCAL void gfs_mngInitWork(Sint32 open_max)
{
    Sint32      i;
    GfsFile	*fp;
    GfdpHn	dpk;

    MNG_OPENMAX(gfs_mng_ptr) = open_max;
    /* �t�@�C������֐��̐ݒ� */
    gfs_mngSetupFuncTbl();
    SVR_NFILE(&MNG_SVR(gfs_mng_ptr)) = 0;	/* �L���[�̒����͂O */
    MNG_PICKUP(gfs_mng_ptr) = MNG_TRANS(gfs_mng_ptr) = NULL;
    MNG_SFAD(gfs_mng_ptr) = MNG_EFAD(gfs_mng_ptr) = 0;
    GFS_ERR_FUNC(&MNG_ERROR(gfs_mng_ptr)) = NULL;
    MNG_FLAGS(gfs_mng_ptr) = 0;
    fp = MNG_FILE(gfs_mng_ptr);
    for (i = 0; i < open_max; i++) {
        GFS_FILE_USED(fp) = FALSE;
	++fp;
    }
    dpk = &MNG_SRCPK(gfs_mng_ptr);
    GFDP_DATA(dpk) = NULL;
    GFDP_ADLT(dpk) = GFDP_LEN(dpk) =  GFDP_NSCT(dpk) = 0;
    GFDP_USE(dpk) = FALSE;
    dpk = &MNG_DSTPK(gfs_mng_ptr);
    GFDP_DATA(dpk) = NULL;
    GFDP_ADLT(dpk) = GFDP_LEN(dpk) =  GFDP_NSCT(dpk) = 0;
    GFDP_USE(dpk) = FALSE;
}


/*****************************************************************************
 *  �t�@�C�����̃J�E���g
 *�m���́n
 *  dirtbl	�f�B���N�g�����Ǘ��̈�
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �f�B���N�g���G���g���̐�
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
GFS_LOCAL Sint32 gfs_mngGetNumFile(GfsDirTbl *dirtbl)
{
    Sint32	i;
    Sint32	atr;

    for (i = 0; i < GFS_DIRTBL_NDIR(dirtbl); i++) {
	if (GFS_DIRTBL_TYPE(dirtbl) == GFS_DIR_ID) {
	    /* �t�@�C�����ʎq���g�p */
	    atr = GFDR_GetAtrDirId(i, GFS_DIRTBL_DIRID(dirtbl));
	} else {
	    /* �t�@�C�������g�p */
	    atr = GFDR_GetAtrDirName(i, GFS_DIRTBL_DIRNAME(dirtbl));
	}
	if (IS_ENDTBL(atr)) {
	    break;
	}
    }
    if (i > GFS_DIRTBL_NDIR(dirtbl)) {
	return 0;
    }
    return i + 1;
}

 
/* �g�p���Ă��Ȃ��t�@�C���n���h���̎擾 */
GfsFile *GFS_mngAllocGrp(void)
{
    Sint32      i;
    GfsFile	*gfs;

    /* �g�p���Ă��Ȃ��n���h����T�� */
    gfs = MNG_FILE(gfs_mng_ptr);
    for (i = 0; i < MNG_OPENMAX(gfs_mng_ptr); i++) {
        if (GFS_FILE_USED(gfs) == FALSE) {
            break;
        }
	++gfs;
    }
    /* ���ׂĎg�p���Ă���ꍇ */
    if (i == MNG_OPENMAX(gfs_mng_ptr)) {
        gfs = NULL;
    } else {
	GFS_FILE_USED(gfs) = TRUE;	/* �g�p���t���O�ݒ� */
    }
    return gfs;
}


/* �I�[�v���O���[�v�̉�� */
GFS_LOCAL void gfs_mngFreeGrp(GfsFile *gfs)
{
    GFS_FILE_USED(gfs) = FALSE;		/* �g�p���t���O���� */
}


/* �t�@�C���A�N�Z�X�������� */
GFS_LOCAL void gfs_mngTermAccess(GfsHn gfs)
{
    GfsFileFunc	*func;
    GfsFlow	*flow = &GFS_FILE_FLOW(gfs);

    if (MNG_PICKUP(gfs_mng_ptr) == gfs ||
	MNG_PICKUP(gfs_mng_ptr) == NULL) {	/* �s�b�N�A�b�v�o�^����	*/
	MNG_PICKUP(gfs_mng_ptr) = NULL;
	func = &MNG_FUNCTBL(gfs_mng_ptr, GFS_DTS_FTYPE(&GFS_FLW_DTSRC(flow)));
	(FUNC_STOPIN(func))(flow, TRUE);
	GFS_FLW_STAT(flow) = GFS_FIN_NOACT;
	GFS_FLW_SCTCNT(flow) = 0;
    }
    if (MNG_TRANS(gfs_mng_ptr) == gfs) {	/* �]������ */
	MNG_TRANS(gfs_mng_ptr) = NULL;
	GFTR_Stop(gfs, GFTR_ALLSCT);	/* �]����~ */
    }
}


/* �G���[�֐��̌Ăт��� */
GFS_LOCAL Sint32 gfs_mngSetErrCode(Sint32 code)
{
    GfsErrStat	*err;

    err = &MNG_ERROR(gfs_mng_ptr);
    GFS_ERR_CODE(err) = code;
    /* �G���[�֐����o�^����Ă���ꍇ */
    if (code != GFS_ERR_OK && GFS_ERR_FUNC(err) != NULL) {
	GFS_ERR_FUNC(err)(GFS_ERR_OBJ(err), code); /* �G���[�����֐����s */
    }
    return code;
}


/*--- �A�N�Z�X�T�[�o -------------------------------------------------------*/

/* �A�N�Z�X�T�[�o�̌��� */
GFS_LOCAL Sint32 gfs_svrSearchHndl(GfsHn gfs)
{
    Sint32      i;
    GfsSvr      *svr;

    svr = &MNG_SVR(gfs_mng_ptr);
    /* �T�[�o��gfs���o�^����Ă��邩�T�[�`���� */
    for (i = 0; i < SVR_NFILE(svr); i++) {
        if (gfs == SVR_ACFILE(svr, i)) {
            break;
        }
    }
    if (i == SVR_NFILE(svr)) {		/* �o�^����Ă��Ȃ��ꍇ	*/
        i = SVR_NOHNDL;
    }
    return i;
}


/* �A�N�Z�X�T�[�o�ւ̓o�^ */
GFS_LOCAL void gfs_svrAddHndl(GfsHn gfs)
{
    GfsSvr      *svr;
    Sint32      i;
    
    i = gfs_svrSearchHndl(gfs);     /* �n���h�����o�^�ς݂��ǂ������ׂ� */
    /* �����ꍇ�o�^ */
    if (i == SVR_NOHNDL) {
        svr = &MNG_SVR(gfs_mng_ptr);
        SVR_ACFILE(svr, SVR_NFILE(svr)) = gfs;
        ++SVR_NFILE(svr);
    }
}


/* �A�N�Z�X�T�[�o����폜 */
GFS_LOCAL void gfs_svrRemoveHndl(GfsHn gfs)
{
    GfsSvr      *svr;
    Sint32      i, nfile;

    if (gfs == NULL) {
	return;
    }
    i = gfs_svrSearchHndl(gfs);		/* �n���h�����o�^�ς݂��ǂ������ׂ� */
    /* �o�^����Ă���ꍇ */
    if (i != SVR_NOHNDL) {
	svr = &MNG_SVR(gfs_mng_ptr);
        --SVR_NFILE(svr);		/* �o�^�������炷 */
        nfile = SVR_NFILE(svr);
        for ( ; i < nfile; i++) {       /* �����߂� */
            SVR_ACFILE(svr, i) = SVR_ACFILE(svr, i + 1);
        }
	SVR_ACFILE(svr, i) = NULL;
    }
}


/*--- ���̑� -----------------------------------------------------*/

/*****************************************************************************
 *  �t�@�C���n���h���̊����ăT�u�֐�
 *�m���́n
 *  opngrp	���蓖�Ă�t�@�C���Ǘ��\����
 *  dirrec	�f�B���N�g�����R�[�h
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  ���蓖�Ă�ꂽ�f�e�r�̈�
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
GfsHn GFS_OpenSub(GfsFile *gfs, GfsDirId *dirrec, Sint32 fid)
{
    GftrHn	gftr;
    Bool	ret;

    if (gfs == NULL) {
	return NULL;
    }
    gftr = GFTR_Setup(&GFS_FILE_TRANS(gfs));
#if defined(DEBUG_LIB)
    if (dirrec != NULL) {
	/* �\�[�X�o�b�t�@�̐ݒ� */
	if (IS_MEMFILE(GFS_DIR_ATR(dirrec))) {
	    /* �������t�@�C���̏ꍇ */
	    GFMF_Setup(&GFS_FILE_FLOW(gfs), dirrec, fid);
	    ret = TRUE;
	} else if (IS_SCSIFILE(GFS_DIR_ATR(dirrec))) {
	    /* SCSI�t�@�C���̏ꍇ */
	    ret = GFSF_Setup(&GFS_FILE_FLOW(gfs), dirrec, fid);
	} else {
	    /* �b�c�t�@�C���̏ꍇ   */
	    ret = GFCF_Setup(&GFS_FILE_FLOW(gfs), dirrec, fid);
	}
    } else {
	/* CD�u���b�N�t�@�C���V�X�e���g�p */
	ret = GFCF_Setup(&GFS_FILE_FLOW(gfs), dirrec, fid);
    }
#else
    ret = GFCF_Setup(&GFS_FILE_FLOW(gfs), dirrec, fid);
#endif /* defined(DEBUG_LIB) */
    if (ret == FALSE) {
	return NULL;
    }
    GFS_FILE_AMODE(gfs) = GFS_NWSTAT_NOACT; /* �A�N�Z�X���[�h */
    GFS_FILE_ASTAT(gfs) = GFS_SVR_COMPLETED; /* �����t���O */
    return gfs;
}


/*--- �f�B���N�g���T�u�֐� -------------------------------------------------*/

/*****************************************************************************
 *  �{�����[���f�B�X�N���v�^�̓ǂݍ���
 *�m���́n
 *  buf		�ǂݍ��ݗ̈�
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
GFS_LOCAL Sint32 gfs_loadVol(Uint8 *buf)
{
    GfsDirId	dirrec;
    GfsHn	gfs;
    Sint32	ret;
    GfsErrStat	stat;
    static const Sint8 pvd_id[] = {1, 'C', 'D', '0', '0', '1'};

    switch (GFCF_GetStat(NULL, NULL)) {
    case GFS_STAT_NODISC:
	/* �f�B�X�N���Z�b�g����Ă��Ȃ��ꍇ */
	return GFS_ERR_CDNODISC;
    case GFS_STAT_ERROR:
	return GFS_ERR_CDRD;
    case GFS_STAT_FATAL:
	return GFS_ERR_FATAL;
    }
    GFS_DIR_FAD(&dirrec) = PVD_FAD + GFCD_GetBaseFad();
    /* �{�����[���f�B�X�N�v���^���t�@�C���Ƃ��ĊJ�� */
    GFS_DIR_SIZE(&dirrec) = PVD_BYTSIZ; /* �T�C�Y�ݒ� */
    GFS_DIR_FN(&dirrec) = 0;
    GFS_DIR_ATR(&dirrec) = 0;
    GFS_DIR_UNIT(&dirrec) = 0;
    GFS_DIR_GAP(&dirrec) = 0;
    gfs = GFS_mngAllocGrp();
    gfs = GFS_OpenSub(gfs, &dirrec, 0);
    if (gfs != NULL) {
	ret = GFS_Fread(gfs, PVD_SCTSIZ, buf, PVD_BYTSIZ);
	if (ret != PVD_BYTSIZ) {
	    return GFS_ERR_CDRD;
	}
	GFS_GetErrStat(&stat);
	ret = GFS_ERR_CODE(&stat);
    }
    if (ret == GFS_ERR_OK) {
	/* �K�i���ʎq�̃`�F�b�N */
	if (memcmp(buf, pvd_id, sizeof(pvd_id))) {
	    ret = GFS_ERR_CDROM;
	}
    }
    GFS_Close(gfs);
    return ret;
}


/*****************************************************************************
 *  ���[�g�f�B���N�g�����擾
 *�m���́n
 *  dir		���[�g�f�B���N�g���i�[�̈�
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �G���[�R�[�h
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
GFS_LOCAL Sint32 gfs_getRootDir(GfsDirId *dir)
{
    Sint32	ret;
    Uint8	*rec;			/* �f�B���N�g�����R�[�h�̈� */

    if (!MNG_CDCON(gfs_mng_ptr)) {
	return GFS_ERR_CDNODISC;
    }
    /* �{�����[���L�q�q�ǂݍ��� */
    ret = gfs_loadVol(MNG_SECTBUF(gfs_mng_ptr));
    if (ret != GFS_ERR_OK) {
	return ret;
    }
    rec = MNG_SECTBUF(gfs_mng_ptr) + ROOT_RECPOS;
    memcpy(&GFS_DIR_FAD(dir), DIRREC_EXT(rec), sizeof(Sint32)); /* FAD�ݒ� */
    LSN_TO_FAD(GFS_DIR_FAD(dir));
    if (GFS_DIR_FAD(dir) < ROOT_FAD_LOW_LIM) {
	return GFS_ERR_CDROM;
    }
    /* �T�C�Y�擾 */
    memcpy(&GFS_DIR_SIZE(dir), DIRREC_LEN(rec), sizeof(Sint32));
    if (GFS_DIR_SIZE(dir) <= 0) {
	return GFS_ERR_CDROM;
    }
    GFS_DIR_FN(dir) = 0;
    GFS_DIR_ATR(dir) = GFS_ATR_DIR;
    GFS_DIR_UNIT(dir) = 0;
    GFS_DIR_GAP(dir) = 0;
    return GFS_ERR_OK;
}


/*****************************************************************************
 *  ���[�g�f�B���N�g�����R�[�h�̓ǂݍ���
 *�m���́n
 *  dirtbl	�f�B���N�g�����R�[�h�ݒ�̈�
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
GFS_LOCAL Sint32 gfs_loadRootDir(GfsDirTbl *dirtbl)
{
    GfsHn	gfs;
    Sint32	ndir, ret;
    GfsDirId	dir;

    ret = gfs_getRootDir(&dir);
    if (ret != GFS_ERR_OK) {
	gfs = NULL;
    } else {
	gfs = GFS_mngAllocGrp();
	gfs = GFS_OpenSub(gfs, &dir, 0);
    }
    /* CD���ڑ�����Ă���̂ɃI�[�v���ł��Ȃ�������G���[ */
    if (MNG_CDCON(gfs_mng_ptr) && (gfs == NULL)) {
	return ret;
    }
    /* �f�B���N�g�����R�[�h�ǂݍ��� */
    ndir = GFDR_SetupDirTbl(gfs, dirtbl, GFS_DIR_SIZE(&dir));
    if (gfs != NULL) {
	GFS_Close(gfs);
    }
    bobtest = ndir;
    return ndir;
}


/*--- �t�@�C���A�N�Z�X -----------------------------------------------------*/

/*****************************************************************************
 *  �f�[�^���\�[�X�o�b�t�@�ɗ�������
 *�m���́n
 *  gfs		�t�@�C���n���h��
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �ǂݍ��ݏ�� GFS_FIN_START, GFS_FIN_PAUSE, GFS_FIN_END
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
GFS_LOCAL Sint32 gfs_flowIn(GfsHn gfs)
{
    Sint32	fstat;
    GfsFileFunc	*func;
    GfsFlow	*flow;

    flow = &GFS_FILE_FLOW(gfs);
    /* �I�����Ă����牽�����Ȃ� */
    if (GFS_FLW_STAT(flow) == GFS_FIN_END) {
	return GFS_FLW_STAT(flow);
    }
    /* �t�@�C���n���h���Ɏg���Ă��Ȃ����Ƃ��m�F */
    if (MNG_PICKUP(gfs_mng_ptr) != NULL && MNG_PICKUP(gfs_mng_ptr) != gfs) {
	return GFS_FIN_PUINUSE;
    }
    /* �ǂݍ��݊֐��̎��s */
    func = &MNG_FUNCTBL(gfs_mng_ptr, GFS_DTS_FTYPE(&GFS_FLW_DTSRC(flow)));
    fstat = (FUNC_FLOWIN(func))(flow);
    if (fstat == GFS_FIN_START) {	/* �ǂݍ��݊J�n�����ꍇ */
	MNG_PICKUP(gfs_mng_ptr) = gfs;
    } else if (fstat == GFS_FIN_END) {	/* �S�Ă̓ǂݍ��݂��I�������ꍇ	*/
	if (MNG_PICKUP(gfs_mng_ptr) == gfs) {
	    MNG_PICKUP(gfs_mng_ptr) = NULL;
	    MNG_SFAD(gfs_mng_ptr) = MNG_EFAD(gfs_mng_ptr) = 0;
	}
    }
    GFS_FLW_STAT(flow) = fstat;
    return gfs_convFinStat(gfs, fstat);
}


/*****************************************************************************
 *  ���[�U�̈�ւ̃f�[�^�̓]��
 *�m���́n
 *  gfs		�t�@�C���n���h��
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �]�����
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
GFS_LOCAL Sint32 gfs_transData(GfsHn gfs)
{
    Sint32	stat, atr, room;
    Bool	flag;
    GfsTrans	*trn = &GFS_FILE_TRANS(gfs);

    /* ���̃t�@�C�����]���������ׂ� */
    if (MNG_TRANS(gfs_mng_ptr) != gfs && MNG_TRANS(gfs_mng_ptr) != NULL) {
	return GFS_SVR_BUSY;
    }
    GFS_GetFileInfo(gfs, NULL, NULL, NULL, &atr);
    if (atr & GFS_ATR_CDDA) {
	return GFTR_ST_END;
    }
    room = GFTR_GetLenRoom(&GFS_FILE_TRANS(gfs));
    if (room == 0) {
	stat = GFTR_Idle(gfs);
    } else {
	stat = GFTR_Exec(gfs, &flag);
    }
    /* �]���I�� */
    if (stat == GFTR_ST_END) {
	if (MNG_TRANS(gfs_mng_ptr) == gfs) {
	    MNG_TRANS(gfs_mng_ptr) = NULL;
	}
    } else if (MNG_TRANS(gfs_mng_ptr) == NULL) {
	MNG_TRANS(gfs_mng_ptr) = gfs;
    }
    return stat;
}




/*****************************************************************************
 *  �t���[���A�h���X���擾����
 *�m���́n
 *  fid		�t�@�C�����ʎq
 *  ofs		�I�t�Z�b�g
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �t���[���A�h���X
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFS_GetFad(Sint32 fid, Sint32 ofs)
{
    GfsDirId	dir;
    Sint32	unit;

    GFS_GetDirInfo(fid, &dir);
    unit = GFS_DIR_UNIT(&dir);
    if (unit == 0) {
	return GFS_DIR_FAD(&dir) + ofs;
    } else {
	return GFS_DIR_FAD(&dir) + (ofs / unit * (unit + GFS_DIR_GAP(&dir)));
    }
}



/* �ǂݍ��݃Z�N�^���ݒ� */
GFS_LOCAL void gfs_SetSct(GfsFlow *flow, Sint32 sct)
{
    GfsDtsrc	*dtsrc = &GFS_FLW_DTSRC(flow);

    if (GFS_DTS_FTYPE(dtsrc) == CD_FILE) {
	GFCF_SetSct(flow, sct);
    } else {
	GFS_FLW_SCT(flow) = sct;
	GFS_FLW_SCTCNT(flow) = 0;
    }
    GFS_FLW_STAT(flow) = GFS_FIN_NOACT;
}


/*****************************************************************************
 *  �t�@�C�����̎擾
 *�m���́n
 *  finfo	�t�@�C�����
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
void gfs_GetFinfo(GfsFinfo *finfo, Sint32 *fid, Sint32 *fad,
			 Sint32 *sctsiz, Sint32 *nsct, Sint32 *lastsiz,
			 Sint32 *fno, Sint32 *atr)
{
    if (fid != NULL) {
	*fid = GFS_FI_FID(finfo);
    }
    if (fad != NULL) {
	*fad = GFS_FI_FAD(finfo);
    }
    if (sctsiz != NULL) {
	*sctsiz = GFS_FI_SCTSZ(finfo);
    }
    if (nsct != NULL) {
	*nsct = GFS_FI_NSCT(finfo);
    }
    if (lastsiz != NULL) {
	*lastsiz = GFS_FI_SCTSZ(finfo) - GFS_FI_LSTRM(finfo);
    }
    if (fno != NULL) {
	*fno = GFS_FI_FN(finfo);
    }
    if (atr != NULL) {
	*atr = GFS_FI_ATR(finfo);
    }
}


/* �t�@�C���N���[�Y�������֐� */
GFS_LOCAL void gfs_closeSub(GfsHn gfs)
{
    GfsFlow	*flow;
    GfsDtsrc	*dts;

    if (gfs == NULL) {
	return;
    }
    flow = &GFS_FILE_FLOW(gfs);
    dts = &GFS_FLW_DTSRC(flow);
    gfs_svrRemoveHndl(gfs);		/* �A�N�Z�X�T�[�o����폜 */
    gfs_mngFreeGrp(gfs);
#if defined(DEBUG_LIB)
    if (GFS_DTS_FTYPE(dts) == CD_FILE) {
#endif
	GFCB_Close(dts);
#if defined(DEBUG_LIB)
    } else if (GFS_DTS_FTYPE(dts) == SCSI_FILE) {
	/* SCSI�t�@�C���̃N���[�Y���� */
	GFSF_Close(dts);
    }
#endif
}    



/* ���o�����[�h�̎擾 */
GFS_LOCAL Sint32 gfs_getGmode(GfsHn gfs)
{
    GfsFlow	*flow = &GFS_FILE_FLOW(gfs);

    return GFS_FLW_GMODE(flow);
}


/* �����������������𒲂ׂ� */
GFS_LOCAL Bool gfs_isTaskDone(GfsHn gfs)
{
    Sint32	atr;
    GfsFlow	*flow = &GFS_FILE_FLOW(gfs);
    GfsTrans	*tran = &GFS_FILE_TRANS(gfs);

    GFS_GetFileInfo(gfs, NULL, NULL, NULL, &atr);
    if (GFS_FLW_STAT(flow) == GFS_FIN_END &&
	((GFBF_GetNumData(gfs) == 0) ||
	 (atr & GFS_ATR_CDDA))) {
	return TRUE;
    }
    return FALSE;
}


/* flow in�̏�Ԃ��G���[�R�[�h�ɕϊ����� */
GFS_LOCAL Sint32 gfs_convFinStat(GfsHn gfs, Sint32 fstat)
{
    switch (fstat) {
    case GFS_FIN_FATAL:
	GFS_FILE_ASTAT(gfs) = GFS_ERR_FATAL;
	return gfs_mngSetErrCode(GFS_ERR_FATAL);
    case GFS_FIN_ERROR:
	GFS_FILE_ASTAT(gfs) = GFS_ERR_CDRD;
	return gfs_mngSetErrCode(GFS_ERR_CDRD);
    case GFS_FIN_OPEN:
	GFS_FILE_ASTAT(gfs) = GFS_ERR_CDOPEN;
	return gfs_mngSetErrCode(GFS_ERR_CDOPEN);
    case GFS_FIN_NODISC:
	GFS_FILE_ASTAT(gfs) = GFS_ERR_CDNODISC;
	return gfs_mngSetErrCode(GFS_ERR_CDNODISC);
    case GFS_FIN_PUINUSE:
	GFS_FILE_ASTAT(gfs) = GFS_ERR_PUINUSE;
	return gfs_mngSetErrCode(GFS_ERR_PUINUSE);
    case GFS_FIN_PAUSE:
	/* �풓���[�h�Ńo�b�t�@�t���ɂȂ�����G���[ */
	if (gfs_getGmode(gfs) == GFS_GMODE_RESIDENT) {
	    GFS_FILE_ASTAT(gfs) = GFS_ERR_BFUL;
	    return gfs_mngSetErrCode(GFS_ERR_BFUL);
	}
	break;
    default:
	GFS_FILE_ASTAT(gfs) = GFS_SVR_BUSY;
	break;
    }
    return GFS_ERR_OK;
}

/* end of file */
