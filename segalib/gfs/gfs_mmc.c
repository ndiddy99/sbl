/*****************************************************************************
 *	�\�t�g�E�F�A���C�u����
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:�t�@�C���V�X�e��
 * Module :SIMM�t�@�C���Ǘ�
 * File	  :gfs_mmc.c
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
#include "gfs_mmc.h"

/*****************************************************************************
 *	�萔�}�N��
 *****************************************************************************/
#define GFMC_FNAME_LEN  12              /* �t�@�C�����ő咷 */

/*****************************************************************************
 *	�񋓒萔
 *****************************************************************************/

/*****************************************************************************
 *	�����}�N��
 *****************************************************************************/

/* �������t�@�C���w�b�_     */
typedef struct {
    Uint32      fblksiz;                /* ���̃t�@�C���w�b�_�܂ł̃o�C�g�� */
    Sint8       fname[GFMC_FNAME_LEN];  /* �t�@�C����                       */
    Sint16      version;                /* �o�[�W����                       */
    Sint16      ftype;                  /* �t�@�C���^�C�v                   */
    Sint32      filesiz;                /* �T�C�Y                           */
    Uint32      datetime;               /* �X�V����                         */
    Sint16      sctsiz;                 /* �Z�N�^�T�C�Y                     */
    Sint16      rsv;                    /* �\��                             */
} MmcHeader;

#define MMCH_FBLKSIZ(mmch)	DWORD(&((mmch)->fblksiz))
#define MMCH_FNAME(mmch)	((mmch)->fname)
#define MMCH_VERSION(mmch)	((mmch)->version)
#define MMCH_FTYPE(mmch)	((mmch)->ftype)
#define MMCH_FILESIZ(mmch)	((mmch)->filesiz)
#define MMCH_DATETIME(mmch)	((mmch)->datetime)
#define MMCH_SCTSIZ(mmch)	((mmch)->sctsiz)
#define MMCH_RSV(mmch)		((mmch)->rsv)

#define DWORD(x)    (*((Uint32 *)(x)))
#define WORD(x)     (*((Uint16 *)(x)))

/*****************************************************************************
 *	�f�[�^�^�̒�`
 *****************************************************************************/

/*****************************************************************************
 *	�ϐ��̐錾
 *****************************************************************************/

/*****************************************************************************
 *	�֐��̐錾
 *****************************************************************************/
/*  SIMM�t�@�C���w�b�_�̃A�h���X���擾���� */
GFS_LOCAL MmcHeader *gfmc_getHeaderAddr(Sint32 fid);

/*****************************************************************************
 *	�ϐ��̒�`
 *****************************************************************************/

/*****************************************************************************
 *	�֐��̒�`
 *****************************************************************************/

/* SIMM�t�@�C���C���[�W�̃��[�h�A�h���X���擾���� */
void *GFMC_GetBase(void)
{
    return (void *)GFMC_base;
}



/*****************************************************************************
 *  SIMM�t�@�C�������擾����
 *�m���́n
 *  fid		�������t�@�C�����ʎq
 *�m�o�́n
 *  area	�t�@�C���f�[�^�̃A�h���X
 *  size	�t�@�C���T�C�Y
 *  sct_siz	�Z�N�^�T�C�Y
 *�m�֐��l�n
 *  
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
void GFMC_GetFileInfo(Sint32 fid, void **area, Sint32 *size, Sint32 *sct_siz)
{
    MmcHeader   *hp;
    void	*a;
    Sint32	fsize;
    Sint32	ssize;

    hp = gfmc_getHeaderAddr(fid);
    if (hp == NULL) {                           /* ������Ȃ��ꍇ */
        a = NULL;
        fsize = ssize = 0;
    } else {                                    /* ���������ꍇ */
        a = (Sint8 *)(hp + 1);
        fsize = DWORD(&(MMCH_FILESIZ(hp)));
        ssize = WORD(&(MMCH_SCTSIZ(hp)));
    }
    if (area != NULL) {
	*area = a;
    }
    if (size != NULL) {
	*size = fsize;
    }
    if (sct_siz != NULL) {
	*sct_siz = ssize;
    }
}


/* SIMM�t�@�C���̐����擾���� */
Sint32 GFMC_GetNumFile(void)
{
    Sint8   *bp;
    Uint32  blksiz;
    Uint32  i;

    bp = GFMC_GetBase();
    if (bp == NULL) {
	return 0;
    }
    for (i = 0; (blksiz = MMCH_FBLKSIZ((MmcHeader *)bp)) != 0L; i++) {
        bp += blksiz;
    }
    return i;
}


/* SIMM�t�@�C�������擾���� */
Sint8 *GFMC_GetFname(Sint32 fid)
{
    MmcHeader   *hp;

    hp = gfmc_getHeaderAddr(fid);
    if (hp == NULL) {
        return NULL;
    } else {
        return MMCH_FNAME(hp);
    }
}


/* ���U�[�u�̈��ݒ肷�� */
void GFMC_SetRsv(Sint32 fid, Sint16 rsv)
{
    MmcHeader   *hp;
    
    hp = gfmc_getHeaderAddr(fid);
    if (hp != NULL) {
        MMCH_RSV(hp) = rsv;
    }
}


/* ���U�[�u�̈���擾���� */
Sint16 GFMC_GetRsv(Sint32 fid)
{
    MmcHeader   *hp;
    
    hp = gfmc_getHeaderAddr(fid);
    if (hp != NULL) {
        return MMCH_RSV(hp);
    } else {
        return MMC_ERROR;
    }
}


/*****************************************************************************
 *  SIMM�t�@�C���w�b�_�̃A�h���X���擾����
 *�m���́n
 *  fid		�t�@�C�����ʎq
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  �t�@�C���w�b�_�̃A�h���X
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
GFS_LOCAL MmcHeader *gfmc_getHeaderAddr(Sint32 fid)
{
    Sint8       *bp;
    Uint32      blksiz;
    Sint32      i;
    MmcHeader   *hp;

    bp = (Sint8 *)GFMC_GetBase();
    if (bp == NULL) {
	return NULL;
    }
    for (i = 0; i < fid; i++) {
        hp = (MmcHeader *)bp;
        blksiz = MMCH_FBLKSIZ(hp);
        if (blksiz == 0L) {                 /* ������Ȃ��ꍇ */
            return NULL;
        }
        bp += blksiz;
    }
    return (MmcHeader *)bp;
}

/* end of file */
