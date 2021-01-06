/*****************************************************************************
 *	�\�t�g�E�F�A���C�u����
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:�t�@�C���V�X�e��
 * Module :�f�B���N�g������
 * File	  :gfs_dir.c
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
#include "gfs_def.h"
#include "gfs_mmc.h"
#include "gfs_dir.h"
#include "gfs_cdc.h"
#if defined(DEBUG_LIB)
#include "gfs_scsi.h"
#endif

/*****************************************************************************
 *	�萔�}�N��
 *****************************************************************************/
#define SECT_BUF_SIZ    2048		/* �Z�N�^�o�b�t�@�̑傫�� */

#define MF_OVW      0x01	/* �������t�@�C�����b�c�t�@�C�����㏑������ */

/* ���ʈ�������f�B���N�g�� */
#define GFDR_CURRENT_DIR_ID	0	/* �J�����g�f�B���N�g����ID */
#define GFDR_PARENT_DIR_ID	1	/* �e�f�B���N�g����ID */

#define GFDR_BASE_MAX	8		/* �t�@�C�����̒����̍ő� */
#define GFDR_EXT_MAX	3		/* �g���q�̒����̍ő� */

/* �ȈՓ��̓X�g���[���̃o�b�t�@�Ƀf�[�^���ǂݍ��܂�Ă��Ȃ� */
#define GFDR_FILE_UNREAD    -1

/*****************************************************************************
 *	�񋓒萔
 *****************************************************************************/

/*****************************************************************************
 *	�����}�N��
 *****************************************************************************/
#define LSN2FAD(lsn)	((lsn) + 150)
#define FAD2LSN(fad)	((fad) - 150)

/* �L�^�����t�B�[���h�̒����i�o�C�g�j */
#define RECORDING_DATE_LEN	7

#define GFS_DIR_DIRREC(rec)	((rec)->dirrec)

/* padding field���K�v�� */
#define NEEDS_PADDING(n)   ((n + 1) & 1)

/*****************************************************************************
 *	�f�[�^�^�̒�`
 *****************************************************************************/
/* CD ROM XA�̃f�B���N�g�����̂����g�p������� */
typedef struct {
    Uint32	pos;		/* �G�N�X�e���g�̈ʒu */
    Uint32	len;		/* �f�[�^�� */
    Uint8	rsize;		/* �f�B���N�g�����R�[�h�̑傫�� */
    Uint8	atr;		/* �t�@�C���̑��� */
    Uint8	unitsize;
    Uint8	gapsize;
    Sint8	fname[GFS_FNAME_LEN + 1]; /* �t�@�C����(+1�͏I�[��0) */
    Uint8	fn;		/* �t�@�C���ԍ� */
} XaDirEnt;

#define XA_DIR_RSIZE(dir)	((dir)->rsize)
#define XA_DIR_POS(dir)		((dir)->pos)
#define XA_DIR_LEN(dir)		((dir)->len)
#define XA_DIR_ATR(dir)		((dir)->atr)
#define XA_DIR_USIZE(dir)	((dir)->unitsize)
#define XA_DIR_GSIZE(dir)	((dir)->gapsize)
#define XA_DIR_FNAME(dir)	((dir)->fname)
#define XA_DIR_FN(dir)		((dir)->fn)

#define XA_ATR_DIR		0x80
#define XA_ATR_INTLV		0x20
#define XA_ATR_FORM2		0x10
#define XA_ATR_FORM1		0x08
#define XA_ATR_ERR		0x0080	/* �t�@�C�������K�i�ɂ����ĂȂ� */

/* �ȈՓ��̓X�g���[���\���� */
/* GfsHn����o�C�g�P�ʂ̓��͂��\�ɂ��� */
typedef struct {
    GfsHn   gfs;
    Uint8   *buf;			/* �P�Z�N�^���̃o�b�t�@ */
    Sint32  rp;				/* buf �̎��̓ǂވʒu(�P�ʁF�o�C�g) */
    Sint32 ap;				/* ���݂̃Z�N�^�ʒu */
    Sint32 nsct;			/* �f�B���N�g���t�@�C���̑傫�� */
} GfdrFile;

#define GFDR_FILE_GFS(fp)       ((fp)->gfs)
#define GFDR_FILE_BUF(fp)       ((fp)->buf)
#define GFDR_FILE_RP(fp)        ((fp)->rp)
#define GFDR_FILE_AP(fp)	((fp)->ap)
#define GFDR_FILE_NSCT(fp)	((fp)->nsct)

/*****************************************************************************
 *	�ϐ��̐錾
 *****************************************************************************/

/*****************************************************************************
 *	�֐��̐錾
 *****************************************************************************/

/* �f�B���N�g�����R�[�h�e�[�u�����쐬���� */
GFS_LOCAL Sint32 gfdr_setupDirIdTbl(GfsHn gfs, GfsDirId *rectbl, Sint32 nrec);

/* �f�B���N�g�����R�[�h�e�[�u�����쐬���� */
GFS_LOCAL Sint32 gfdr_setupDirNameTbl(GfsHn gfs, GfsDirName *rectbl, Sint32 nrec);

/* �ȈՓ��̓X�g���[���̏����� */
GFS_LOCAL void gfdr_open(GfdrFile *fp, GfsHn gfs);

/* �w�肳�ꂽ�o�C�g�������o�b�t�@�ɓǂݍ��� */
GFS_LOCAL Sint32 gfdr_read(GfdrFile *fp, void *buf, Uint32 size);

/* 1 byte�ǂݍ��� */
GFS_LOCAL Uint8 gfdr_readByte(GfdrFile *fp, Sint32 *ec);

/* 2 byte�ǂݍ��� */
GFS_LOCAL Uint16 gfdr_readWord(GfdrFile *fp, Sint32 *ec);

/* NB�^��2 byte������ǂݍ��� */
GFS_LOCAL Uint16 gfdr_readWordNB(GfdrFile *fp, Sint32 *ec);

/* NB�^��4 byte������ǂݍ��� */
GFS_LOCAL Uint32 gfdr_readLongNB(GfdrFile *fp, Sint32 *ec);

/* NM�^��4 byte������ǂݍ��� */
GFS_LOCAL Uint32 gfdr_readLongNM(GfdrFile *fp, Sint32 *ec);

/* ���̃f�B���N�g�����R�[�h���o�b�t�@�ɋl�ߍ��� */
GFS_LOCAL Sint32 gfdr_setNextRec(GfdrFile *fp, Sint32 *ec);

/* CD����f�B���N�g���G���g���̕K�v�ȏ���ǂݍ��� */
GFS_LOCAL Sint32 gfdr_readDirEnt(GfdrFile *fp, XaDirEnt *dir);

/* CD�̃f�B���N�g�����ݒ� */
GFS_LOCAL void gfdr_setDirrecCd(CdcFile *rec, XaDirEnt *dir);

#if defined(DEBUG_LIB)
/* SIMM�t�@�C���̃f�B���N�g�����ݒ� */
GFS_LOCAL void gfdr_setDirrecMem(CdcFile *rec, Sint32 fid, Sint32 len);

/* SCSI�t�@�C���̃f�B���N�g�����ݒ� */
GFS_LOCAL void gfdr_setDirrecScsi(CdcFile *rec, Sint32 fid, Sint32 len);

/* �t�@�C������SCSI�t�@�C����T�� */
GFS_LOCAL Bool gfdr_getScsiFinfo(Sint8 *fname, Sint32 *fid, Sint32 *len);

/* �w�肳�ꂽfid��SCSI�t�@�C�����o�^����Ă��邩���ׂ� */
GFS_LOCAL Bool gfdr_findScsiFile(GfsDirId *rec, GfsDirId *recend, Sint32 fid);

/* �t�@�C�����Łi�t�@�C�������j�f�B���N�g�����R�[�h�e�[�u�����猟������ */
GFS_LOCAL GfsDirName *gfdr_findFile(GfsDirName *rectbl, Sint32 n_dirent,
				 Sint8 *fname);

/* �������t�@�C����RSV�r�b�g��S�N���A */
GFS_LOCAL void gfdr_clearMemRsv(Sint32 n_memfile);
#endif


/*****************************************************************************
 *	�ϐ��̒�`
 *****************************************************************************/

/*****************************************************************************
 *	�֐��̒�`
 *****************************************************************************/

/*****************************************************************************
 *  �f�B���N�g���e�[�u���ǂݍ���
 *�m���́n
 *  gfs		�f�B���N�g���t�@�C���̃n���h��
 *  dirtbl	�f�B���N�g�����ݒ�̈�
 *  nsct	�f�B���N�g���t�@�C���̑傫���i�P�ʁF�Z�N�^�j
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  �ǂݍ��񂾃f�B���N�g����
 *�m�@�\�n
 *  
 *�m���l�n
 *  gfs == NULL �̂Ƃ��ACD����̓f�B���N�g�������擾���Ȃ��B
 *****************************************************************************/
Sint32 GFDR_SetupDirTbl(GfsHn gfs, GfsDirTbl *dirtbl, Sint32 nsct)
{
    Sint32	ndir, atr, ec;

#if !defined(DEBUG_LIB)
    if (gfs != NULL) {
	GFS_GetFileInfo(gfs, NULL, NULL, NULL, &atr);
	if ((atr & GFS_ATR_DIR) == 0) {
	    return GFS_ERR_DIR;
	}
	ec = GFS_NwCdRead(gfs, nsct);
	if (ec != GFS_ERR_OK) {
	    return ec;
	}
    }
#endif
    if (GFS_DIRTBL_TYPE(dirtbl) == GFS_DIR_ID) {
	ndir = gfdr_setupDirIdTbl(gfs, GFS_DIRTBL_DIRID(dirtbl),
				  GFS_DIRTBL_NDIR(dirtbl));
    } else {
	ndir = gfdr_setupDirNameTbl(gfs, GFS_DIRTBL_DIRNAME(dirtbl),
				    GFS_DIRTBL_NDIR(dirtbl));
    }
    return ndir;
}



/*****************************************************************************
 * �t�@�C��������t�@�C�����ʎq���擾���� 
 *�m���́n
 *  
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  �t�@�C�����ʎq
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFDR_NameToId(GfsDirName *rectbl, Sint8 *fname, Sint32 max)
{
    Sint32	i;

    for (i = 0; i < max; ++i, ++rectbl) {
	if (XSTRNCMP(fname, GFS_DIR_FNAME(rectbl), GFS_FNAME_LEN) == 0) {
	    return i;
	}
	if (GFS_DIR_ATR(rectbl) & GFS_ATR_END_TBL) {
	    break;
	}
    }
    return -1;
}


/* �f�B���N�g�����R�[�h�e�[�u���̏����擾���� */
GfsDirId *GFDR_GetInfoDirId(Sint32 fid, GfsDirId *rectbl)
{
    return &rectbl[fid];
}


/* �f�B���N�g�����R�[�h�e�[�u���̏����擾���� */
GfsDirName *GFDR_GetInfoDirName(Sint32 fid, GfsDirName *rectbl)
{
    return &rectbl[fid];
}


/* �t�@�C���̑����擾 */
Sint32 GFDR_GetAtrDirId(Sint32 fid, GfsDirId *rectbl)
{
    return GFS_DIR_ATR(&rectbl[fid]);
}


/* �t�@�C�����擾 */
Sint32 GFDR_GetAtrDirName(Sint32 fid, GfsDirName *rectbl)
{
    return GFS_DIR_ATR(&rectbl[fid]);
}


/*****************************************************************************
 *  CDBFS�Ń��[�g�f�B���N�g���Ɉړ�
 *�m���́n
 *  
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  �f�B���N�g���̌���Ԃ��B�G���[�Ȃ�-1��Ԃ��B
 *�m�@�\�n
 *  CD�u���b�N�̃J�����g�f�B���N�g�������[�g�f�B���N�g���ɕύX
 *�m���l�n
 *  �I�����Ƀf�B���N�g���������͏I�����Ă���B�������A�B
 *****************************************************************************/
Sint32 GFDR_ChgRoot(void)
{
    return GFDR_ChgDir(CDC_NUL_FID);
}


/*****************************************************************************
 *  CDBFS�ŃJ�����g�f�B���N�g���ύX
 *�m���́n
 *  
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  �f�B���N�g���̌���Ԃ��B�G���[�Ȃ畉�̃G���[�R�[�h��Ԃ��B
 *�m�@�\�n
 *  
 *�m���l�n
 *  �I�����Ƀf�B���N�g���������͏I�����Ă���B�������A�B
 *****************************************************************************/
Sint32 GFDR_ChgDir(Sint32 fid)
{
    Sint32 err, ndir;

    /* ChgDir���s */
    GFCD_SetTimer(GFCD_TMOUT_COUNT);
    do {
	err = GFCD_ChgDir(fid, GFCD_SYS_SEL, &ndir);
	if (err == GFCD_ERR_INUSE) {
	    return GFS_ERR_CDBFS;	/* CDBFS�g�p�錾���Ă��Ȃ� */
	}
	err = GFCD_ExecServer();
	if (err == GFCD_ERR_TMOUT) {
	    return GFS_ERR_TMOUT;
	} else if (err == GFCD_ERR_FATAL) {
	    return GFS_ERR_FATAL;
	}
    } while (err != GFCD_ERR_OK);
    /* ChgDir�̏����҂� */
    err = GFCD_WaitServer();
    if (err == GFCD_ERR_FATAL) {
	return GFS_ERR_FATAL;
    } else if (err != GFCD_SVR_COMPLETED) {
	return GFS_ERR_CDRD;
    }
    return ndir;			/* CDBFS���ێ����Ă��鐔 */
}

/*---------------------------------------------------------------------------
 * ����J�֐�
 *---------------------------------------------------------------------------*/


/* �f�B���N�g�����R�[�h�e�[�u�����쐬���� */
GFS_LOCAL Sint32 gfdr_setupDirIdTbl(GfsHn gfs, GfsDirId *rectbl, Sint32 nrec)
{
    GfsDirId	*r = rectbl;
    Sint32      n_dirent, ec;
    GfdrFile    fp;
    XaDirEnt    dir_ent;    /* ���ڂ��Ă���f�B���N�g���G���g�� */
#if defined(DEBUG_LIB)
    Sint32	n_memfile = GFMC_GetNumFile();
    Sint32	i, n_scsifile;
    Uint8       *dummy1;
    Sint32	dummy2, len;
    GfsdInfo	info;
#endif

#if defined(DEBUG_LIB)
    for (i = 0; i < n_memfile; ++i) {
	GFMC_SetRsv(i, 0);
    }
#endif
    gfdr_open(&fp, gfs);
    for (n_dirent = 0; nrec > 0; ++r, ++n_dirent, --nrec) {
        ec = gfdr_readDirEnt(&fp, &dir_ent);
	if (ec != GFS_ERR_OK) {
	    return ec;
	}
        /* �f�B���N�g�����R�[�h�̖����`�F�b�N */
        if (XA_DIR_RSIZE(&dir_ent) == 0) {  /* �f�B���N�g�����R�[�h�̑傫�� */
            break;
        }
#if defined(DEBUG_LIB)
        /* �������t�@�C���ɑ��݂��邩 */
        for (i = 0; i < n_memfile; ++i) {
            if (XSTRNCPY(XA_DIR_FNAME(&dir_ent), GFMC_GetFname(i),
                        GFS_FNAME_LEN) == 0) {
                GFMC_GetFileInfo(i, (void **)&dummy1, &len, &dummy2);
		gfdr_setDirrecMem(&GFS_DIR_DIRREC(r), i, len);
		break;
            }
        }
        if (i == n_memfile) {
            /* �����̃������t�@�C���͑��݂��Ȃ����� */
	    if (gfdr_getScsiFinfo(XA_DIR_FNAME(&dir_ent), &i, &len) == TRUE) {
		gfdr_setDirrecScsi(&GFS_DIR_DIRREC(r), i, len);
	    } else {
		/* ������SCSI�t�@�C�������݂��Ȃ����� */
#endif
		gfdr_setDirrecCd(&GFS_DIR_DIRREC(r), &dir_ent);
#if defined(DEBUG_LIB)
	    }
        }
#endif
    }
#if defined(DEBUG_LIB)
    /* �������t�@�C����̖��o�^���R�[�h��o�^ */
    for (i = 0; i < n_memfile && nrec > 0; ++i) {
        if (GFMC_GetRsv(i) != MF_OVW) {
            --nrec;
            GFMC_GetFileInfo(i, (void **)&dummy1, &len, &dummy2);
	    gfdr_setDirrecMem(&GFS_DIR_DIRREC(r), i, len);
            ++r;
	    ++n_dirent;
        }
    }
    gfdr_clearMemRsv(n_memfile);
    if (MNG_SCSICON(gfs_mng_ptr)) {
	/* ���o�^��SCSI�t�@�C�����R�[�h��o�^ */
	GFSD_GetFileNum(&n_scsifile);
	for (i = 0; i < n_scsifile && nrec > 0; ++i) {
	    if (gfdr_findScsiFile(rectbl, r, i) != TRUE) {
		GFSD_GetFileInfo(i, &info);
		gfdr_setDirrecScsi(&GFS_DIR_DIRREC(r), i, GFSD_INFO_LENGTH(&info));
	    }
	    ++r;
	    ++n_dirent;
	}
    }
#endif
    --r;
    GFS_DIR_ATR(r) |= GFS_ATR_END_TBL;
    return n_dirent;
}


/* �f�B���N�g�����R�[�h�e�[�u�����쐬���� */
GFS_LOCAL Sint32 gfdr_setupDirNameTbl(GfsHn gfs, GfsDirName *rectbl, Sint32 nrec)
{
    GfsDirName	*r = rectbl;
    Sint32      n_dirent, ec;
    GfdrFile   fp;
    XaDirEnt    dir_ent;    /* ���ڂ��Ă���f�B���N�g���G���g�� */
#ifdef DEBUG_LIB
    Sint32	n_memfile;
    Uint8       *dummy1;
    Sint32	dummy2, len, i, n_scsifile, n_new_scsi, n_new_memf;
    GfsdInfo	info;
#endif

    /* CD��̃f�B���N�g�����R�[�h��ǂݍ��� */
    gfdr_open(&fp, gfs);
    for (n_dirent = 0; nrec > 0; ++r, ++n_dirent, --nrec) {
        ec = gfdr_readDirEnt(&fp, &dir_ent);
	if (ec != GFS_ERR_OK) {
	    return ec;
	}
        /* �f�B���N�g�����R�[�h�̖����`�F�b�N */
        if (XA_DIR_RSIZE(&dir_ent) == 0) {  /* �f�B���N�g�����R�[�h�̑傫�� */
            break;
        }
	/* �O�̃t�@�C�����Ɠ����Ȃ�f�B���N�g�����R�[�h���̂Ă� */
	if (n_dirent > 2 &&
	    XSTRNCMP(GFS_DIR_FNAME(&r[-1]),
		    XA_DIR_FNAME(&dir_ent), GFS_FNAME_LEN) == 0) {
	    continue;
	}
	gfdr_setDirrecCd(&GFS_DIR_DIRREC(r), &dir_ent);
	XSTRNCPY(GFS_DIR_FNAME(r), XA_DIR_FNAME(&dir_ent), GFS_FNAME_LEN);
    }
#ifdef DEBUG_LIB
    if (MNG_SCSICON(gfs_mng_ptr)) {
	/* SCSI�t�@�C���̃f�B���N�g�����R�[�h���㏑������ */
	GFSD_GetFileNum(&n_scsifile);
	n_new_scsi = 0;
	for (i = 0; i < n_scsifile && n_new_scsi < nrec; ++i) {
	    GFSD_GetFileInfo(i, &info);
	    r = gfdr_findFile(rectbl, n_dirent, GFSD_INFO_FNAME(&info));
	    if (r == NULL) {
		/* ������Ȃ���Ζ����ɒǉ� */
		r = &rectbl[n_dirent + n_new_scsi];
		gfdr_setDirrecScsi(&GFS_DIR_DIRREC(r), i, GFSD_INFO_LENGTH(&info));
		XSTRNCPY(GFS_DIR_FNAME(r), GFSD_INFO_FNAME(&info), GFS_FNAME_LEN);
		++n_new_scsi;
	    } else {
		gfdr_setDirrecScsi(&GFS_DIR_DIRREC(r), i, GFSD_INFO_LENGTH(&info));
	    }
	}
	n_dirent += n_new_scsi;
    }
    /* �������t�@�C���̃f�B���N�g�����R�[�h���㏑������ */
    n_memfile = GFMC_GetNumFile();
    n_new_memf = 0;
    /* �������t�@�C����̖��o�^���R�[�h��o�^ */
    for (i = 0; i < n_memfile && n_new_memf < nrec; ++i) {
	r = gfdr_findFile(rectbl, n_dirent, GFMC_GetFname(i));
	GFMC_GetFileInfo(i, (void **)&dummy1, &len, &dummy2);
	if (r == NULL) {
	    /* ������Ȃ���Ζ����ɒǉ� */
	    r = &rectbl[n_dirent + n_new_memf];
	    gfdr_setDirrecMem(&GFS_DIR_DIRREC(r), i, len);
            XSTRNCPY(GFS_DIR_FNAME(r), GFMC_GetFname(i), GFS_FNAME_LEN);
	    ++n_new_memf;
	} else {
	    gfdr_setDirrecMem(&GFS_DIR_DIRREC(r), i, len);
	}
    }
    gfdr_clearMemRsv(n_memfile);
    n_dirent += n_new_memf;
    r = &rectbl[n_dirent];
#endif
    --r;
    GFS_DIR_ATR(r) |= GFS_ATR_END_TBL;
    return n_dirent;
}


/* �ȈՓ��̓X�g���[���̏����� */
GFS_LOCAL void gfdr_open(GfdrFile *fp, GfsHn gfs)
{
    GfsFinfo	*finfo = &GFS_FLW_FINFO(&GFS_FILE_FLOW(gfs));

    GFDR_FILE_GFS(fp) = gfs;
    GFDR_FILE_BUF(fp) = MNG_SECTBUF(gfs_mng_ptr);
    GFDR_FILE_RP(fp) = GFDR_FILE_UNREAD;
    GFDR_FILE_NSCT(fp) = GFS_FI_NSCT(finfo);
    GFDR_FILE_AP(fp) = 0;
}


/* �w�肳�ꂽ�o�C�g�������o�b�t�@�ɓǂݍ��� */
/* �G���[�R�[�h��Ԃ� */
GFS_LOCAL Sint32 gfdr_read(GfdrFile *fp, void *buf, Uint32 size)
{
    Sint32  i, ret;

    /* �������t�@�C�����炾���ǂގ� */
    if (GFDR_FILE_GFS(fp) == NULL) {
	memset(buf, 0, size);
	return GFS_ERR_OK;
    }
    if ((GFDR_FILE_RP(fp) == GFDR_FILE_UNREAD) ||
        (GFDR_FILE_RP(fp) >= SECT_BUF_SIZ)) {
        ret = GFS_Fread(GFDR_FILE_GFS(fp), 1, GFDR_FILE_BUF(fp), SECT_BUF_SIZ);
	if (ret != SECT_BUF_SIZ) {
	    if (ret < GFS_ERR_OK) {
		return ret;
	    }
	    return GFS_ERR_CDRD;
	}
        GFDR_FILE_RP(fp) = 0;
    }
    /* �ǂݍ��ރf�[�^�̂����o�b�t�@��ɂ�����̂̑傫�� */
    i = SECT_BUF_SIZ - GFDR_FILE_RP(fp);
    i = MIN(i, size);
    memcpy(buf, GFDR_FILE_BUF(fp) + GFDR_FILE_RP(fp), i);
    GFDR_FILE_RP(fp) += i;
    if (i < size) {
        size -= i;
        ret = GFS_Fread(GFDR_FILE_GFS(fp), 1, GFDR_FILE_BUF(fp), SECT_BUF_SIZ);
	if (ret != SECT_BUF_SIZ) {
	    if (ret < GFS_ERR_OK) {
		return ret;
	    }
	    return GFS_ERR_CDRD;
	}
        memcpy((Uint8 *)buf + i, GFDR_FILE_BUF(fp), size);
        GFDR_FILE_RP(fp) = size;
    }
    return GFS_ERR_OK;
}


/* 1 byte�ǂݍ��� */
GFS_LOCAL Uint8 gfdr_readByte(GfdrFile *fp, Sint32 *ec)
{
    Uint8   tmp;

    *ec = gfdr_read(fp, &tmp, sizeof(tmp));
    return tmp;
}


/* 2 byte�ǂݍ��� */
GFS_LOCAL Uint16 gfdr_readWord(GfdrFile *fp, Sint32 *ec)
{
    Uint16  tmp;

    *ec = gfdr_read(fp, &tmp, sizeof(tmp));
    return tmp;
}


/* NB�^��2 byte������ǂݍ��� */
GFS_LOCAL Uint16 gfdr_readWordNB(GfdrFile *fp, Sint32 *ec)
{
    Uint16  tmp, dummy;

    /* LSBF�\�L�����̓ǂݎ̂� */
    *ec = gfdr_read(fp, &dummy, sizeof(dummy));
    if (*ec != GFS_ERR_OK) {
	return 0;
    }
    /* MSBF�\�L�����̓ǂݍ��� */
    *ec = gfdr_read(fp, &tmp, sizeof(tmp));
    if (*ec != GFS_ERR_OK) {
	return 0;
    }
    return tmp;
}


/* NB�^��4 byte������ǂݍ��� */
GFS_LOCAL Uint32 gfdr_readLongNB(GfdrFile *fp, Sint32 *ec)
{
    Uint32  tmp, dummy;

    /* LSBF�\�L�����̓ǂݎ̂� */
    *ec = gfdr_read(fp, &dummy, sizeof(dummy));
    if (*ec != GFS_ERR_OK) {
	return 0;
    }
    /* MSBF�\�L�����̓ǂݍ��� */
    *ec = gfdr_read(fp, &tmp, sizeof(tmp));
    if (*ec != GFS_ERR_OK) {
	return 0;
    }
    return tmp;
}


/* NM�^��4 byte������ǂݍ��� */
GFS_LOCAL Uint32 gfdr_readLongNM(GfdrFile *fp, Sint32 *ec)
{
    Uint32  tmp;

    *ec = gfdr_read(fp, &tmp, sizeof(tmp));
    return tmp;
}


/* ���̃f�B���N�g�����R�[�h���o�b�t�@�ɋl�ߍ��� */
GFS_LOCAL Sint32 gfdr_setNextRec(GfdrFile *fp, Sint32 *ec)
{
    Sint32 rsize, ret;

    rsize = gfdr_readByte(fp, ec);
    if (*ec != GFS_ERR_OK) {
	return 0;
    }
    if (rsize != 0) {
	return rsize;
    }

    /* �ǂݍ��߂Ȃ������ꍇ */
    ++GFDR_FILE_AP(fp);
    if (GFDR_FILE_AP(fp) >= GFDR_FILE_NSCT(fp)) {
	return rsize;
    }
    ret = GFS_Fread(GFDR_FILE_GFS(fp), 1, GFDR_FILE_BUF(fp), SECT_BUF_SIZ);
    if (ret != SECT_BUF_SIZ) {
	if (ret > 0) {
	    *ec = GFS_ERR_CDRD;
	} else {
	    *ec = ret;
	}
	return 0;
    }
    GFDR_FILE_RP(fp) = 0;
    rsize = gfdr_readByte(fp, ec);
    if (*ec != GFS_ERR_OK) {
	return 0;
    }
    return rsize;
}


/* CD����f�B���N�g���G���g���̕K�v�ȏ���ǂݍ��� */
GFS_LOCAL Sint32 gfdr_readDirEnt(GfdrFile *fp, XaDirEnt *dir)
{
    Sint32	ec, i, rdsz, fn_err = FALSE;
    Uint8       fn_len, file_flg;
    Sint8	ch, *fn_buf, *ext_pos;

    if (GFDR_FILE_GFS(fp) == NULL) {
	XA_DIR_RSIZE(dir) = 0;
	return GFS_ERR_OK;
    }
    XA_DIR_RSIZE(dir) = gfdr_setNextRec(fp, &ec);
    if (ec != GFS_ERR_OK) {
	return ec;
    }
    if (XA_DIR_RSIZE(dir) == 0) {
	return GFS_ERR_OK;
    }
    gfdr_readByte(fp, &ec);		/* �g���������R�[�h���ǂݎ̂� */
    if (ec != GFS_ERR_OK) {
	return ec;
    }
    XA_DIR_POS(dir) = LSN2FAD(gfdr_readLongNB(fp, &ec));
    if (ec != GFS_ERR_OK) {
	return ec;
    }
    XA_DIR_LEN(dir) = gfdr_readLongNB(fp, &ec);
    if (ec != GFS_ERR_OK) {
	return ec;
    }
    /* ���t�f�[�^�ǂݎ̂� */
    for (i =0; i < RECORDING_DATE_LEN; ++i) {
	gfdr_readByte(fp, &ec);
	if (ec != GFS_ERR_OK) {
	    return ec;
	}
    }
    file_flg = gfdr_readByte(fp, &ec);
    if (ec != GFS_ERR_OK) {
	return ec;
    }
    XA_DIR_USIZE(dir) = gfdr_readByte(fp, &ec); /* File Unit Size */
    if (ec != GFS_ERR_OK) {
	return ec;
    }
    XA_DIR_GSIZE(dir) = gfdr_readByte(fp, &ec); /* interleave gap size */
    if (ec != GFS_ERR_OK) {
	return ec;
    }
    gfdr_readWordNB(fp, &ec);		/* �{�����[�������ԍ��ǂݎ̂� */
    if (ec != GFS_ERR_OK) {
	return ec;
    }
    fn_len = gfdr_readByte(fp, &ec);
    if (ec != GFS_ERR_OK) {
	return ec;
    }
    /* �����܂ŌŒ蒷 */
    rdsz = 33 + fn_len;
    fn_buf = XA_DIR_FNAME(dir);
    i = 0;
    while (i < MIN(GFDR_BASE_MAX, fn_len)) {
	/* �t�@�C�������� */
	ch = gfdr_readByte(fp, &ec), ++i;
	if (ec != GFS_ERR_OK) {
	    return ec;
	}
	if (ch == ';') {
	    break;
	}
	*fn_buf++ = ch;
	if (ch == '.') {
	    ext_pos = fn_buf;		/* �g���q�̊J�n�ʒu�ۑ� */
	    break;
	}
    }
    if (ch == ';') {
	/* �g���q�Ȃ��A�o�[�W�����ԍ��ǂݎ̂� */
	while (i < fn_len) {
	    gfdr_readByte(fp, &ec), ++i;
	    if (ec != GFS_ERR_OK) {
		return ec;
	    }
	}
    } else if (ch != '.') {
	/* ���߂���t�@�C������ǂݎ̂� */
	while (i < fn_len) {
	    ch = gfdr_readByte(fp, &ec), ++i;
	    if (ec != GFS_ERR_OK) {
		return ec;
	    }
	    if (ch == '.') {
		*fn_buf++ = ch;
		ext_pos = fn_buf;	/* �g���q�̊J�n�ʒu�ۑ� */
		break;
	    }
	    fn_err = TRUE;
	}
    }
    while (i < fn_len) {
	/* �g���q���� */
	ch = gfdr_readByte(fp, &ec), ++i;
	if (ec != GFS_ERR_OK) {
	    return ec;
	}
	if (ch == ';') {
	    /* �o�[�W�����ԍ��ǂݎ̂� */
	    while (i < fn_len) {
		gfdr_readByte(fp, &ec), ++i;
		if (ec != GFS_ERR_OK) {
		    return ec;
		}
	    }
	    break;
	}
	if (fn_buf - ext_pos > GFDR_EXT_MAX) {
	    fn_err = TRUE;
	    continue;
	}
	if (fn_buf - XA_DIR_FNAME(dir) < GFS_FNAME_LEN) {
	    *fn_buf++ = ch;
	}
    }
    if (fn_buf - XA_DIR_FNAME(dir) < GFS_FNAME_LEN) {
	*fn_buf = '\0';
    }
    /* �l�ߍ��݂O���X�L�b�v */
    if (NEEDS_PADDING(fn_len)) {
        gfdr_readByte(fp, &ec);
	if (ec != GFS_ERR_OK) {
	    return ec;
	}
	++rdsz;
    }
    /* ����ȃt�@�C�����̕ϊ� */
    if (XA_DIR_FNAME(dir)[0] == GFDR_CURRENT_DIR_ID) {
	XSTRCPY(XA_DIR_FNAME(dir), ".");
    } else if (XA_DIR_FNAME(dir)[0] == GFDR_PARENT_DIR_ID) {
	XSTRCPY(XA_DIR_FNAME(dir), "..");
    }
    /* �V�X�e����񂪂���Γǂ� */
    if (rdsz < XA_DIR_RSIZE(dir)) {
	gfdr_readLongNM(fp, &ec);	/* skip Owner ID */
	if (ec != GFS_ERR_OK) {
	    return ec;
	}
	XA_DIR_ATR(dir) = gfdr_readByte(fp, &ec) &
	    (GFS_ATR_DIR|GFS_ATR_CDDA|GFS_ATR_INTLV|
	     GFS_ATR_FORM2|GFS_ATR_FORM1);
	if (ec != GFS_ERR_OK) {
	    return ec;
	}
	gfdr_readByte(fp, &ec);		/* �������ʂP�o�C�g�ǂݎ̂� */
	if (ec != GFS_ERR_OK) {
	    return ec;
	}
	gfdr_readWord(fp, &ec);		/* "XA"�ǂ� */
	if (ec != GFS_ERR_OK) {
	    return ec;
	}
	XA_DIR_FN(dir) = gfdr_readByte(fp, &ec);
	if (ec != GFS_ERR_OK) {
	    return ec;
	}
	/* �c��̃f�B���N�g�����R�[�h�ǂݎ̂� */
	for (i = rdsz + 9; i < XA_DIR_RSIZE(dir); ++i) {
	    gfdr_readByte(fp, &ec);
	    if (ec != GFS_ERR_OK) {
		return ec;
	    }
	}
    } else {
	/* �V�X�e����񂪂Ȃ��Ȃ� */
	/* �t�@�C���t���O�����ăf�B���N�g�������f���� */
	XA_DIR_ATR(dir) = (file_flg & 2)? XA_ATR_DIR: 0;
	XA_DIR_FN(dir) = 0;		/* �t�@�C���ԍ��Ȃ� */
    }
    return GFS_ERR_OK;
}


/* CD�̃f�B���N�g�����ݒ� */
GFS_LOCAL void gfdr_setDirrecCd(CdcFile *rec, XaDirEnt *dir)
{
    CDC_FILE_FAD(rec) = XA_DIR_POS(dir);
    CDC_FILE_SIZE(rec) = XA_DIR_LEN(dir);
    CDC_FILE_FN(rec) = XA_DIR_FN(dir);
    CDC_FILE_ATR(rec) = XA_DIR_ATR(dir);
    CDC_FILE_UNIT(rec) = XA_DIR_USIZE(dir);
    CDC_FILE_GAP(rec) = XA_DIR_GSIZE(dir);
}


#if defined(DEBUG_LIB)
/* SIMM�t�@�C���̃f�B���N�g�����ݒ� */
GFS_LOCAL void gfdr_setDirrecMem(CdcFile *rec, Sint32 fid, Sint32 len)
{
    CDC_FILE_FAD(rec) = fid;
    CDC_FILE_SIZE(rec) = len;
    CDC_FILE_FN(rec) = 0;
    CDC_FILE_ATR(rec) = GFS_ATR_MEM_FILE;
    CDC_FILE_UNIT(rec) = 0;
    CDC_FILE_GAP(rec) = 0;
    GFMC_SetRsv(fid, MF_OVW);
}


/* SCSI�t�@�C���̃f�B���N�g�����ݒ� */
GFS_LOCAL void gfdr_setDirrecScsi(CdcFile *rec, Sint32 fid, Sint32 len)
{
    CDC_FILE_FAD(rec) = fid;
    CDC_FILE_SIZE(rec) = len;
    CDC_FILE_FN(rec) = 0;
    CDC_FILE_ATR(rec) = GFS_ATR_SCSI_FILE;
    CDC_FILE_UNIT(rec) = 0;
    CDC_FILE_GAP(rec) = 0;
}


/* �t�@�C������SCSI�t�@�C����T�� */
GFS_LOCAL Bool gfdr_getScsiFinfo(Sint8 *fname, Sint32 *fid, Sint32 *len)
{
    Sint32	i, n_scsi_file;
    GfsdInfo	info;

    if (MNG_SCSICON(gfs_mng_ptr)) {
	GFSD_GetFileNum(&n_scsi_file);
	for (i = 0; i < n_scsi_file; ++i) {
	    GFSD_GetFileInfo(i, &info);
	    if (XSTRNCMP(fname, GFSD_INFO_FNAME(&info), GFS_FNAME_LEN) == 0) {
		*fid = i;
		*len = GFSD_INFO_LENGTH(&info);
		return TRUE;
	    }
	}
    }
    return FALSE;
}


/* �w�肳�ꂽfid��SCSI�t�@�C�����o�^����Ă��邩���ׂ� */
GFS_LOCAL Bool gfdr_findScsiFile(GfsDirId *rec, GfsDirId *recend, Sint32 fid)
{
    while (rec < recend) {
	if (GFS_DIR_ATR(rec) == GFS_ATR_SCSI_FILE && GFS_DIR_FAD(rec) == fid) {
	    return TRUE;
	}
	++rec;
    }
    return FALSE;
}


/* �t�@�C�����Łi�t�@�C�������j�f�B���N�g�����R�[�h�e�[�u�����猟������ */
GFS_LOCAL GfsDirName *gfdr_findFile(GfsDirName *rectbl, Sint32 n_dirent,
				 Sint8 *fname)
{
    Sint32	i;

    for (i = 0; i < n_dirent; ++i) {
	if (XSTRNCMP(GFS_DIR_FNAME(rectbl), fname, GFS_FNAME_LEN) == 0) {
	    return rectbl;
	}
	++rectbl;
    }
    return NULL;
}


/* �������t�@�C����RSV�r�b�g��S�N���A */
GFS_LOCAL void gfdr_clearMemRsv(Sint32 n_memfile)
{
    Sint32 i;

    for (i = 0; i < n_memfile; ++i) {
	GFMC_SetRsv(i, 0);
    }
}

#endif /* defined(DEBUG_LIB) */

/* end of file */
