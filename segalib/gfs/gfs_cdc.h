/******************************************************************************
 *	�\�t�g�E�F�A���C�u����
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:�t�@�C���V�X�e��
 * Module :CDC�Ƃ̃C���^�t�F�[�X���W���[���w�b�_
 * File	  :gfs_cdc.h
 * Date   :1994-11-11
 * Version:1.16
 * Auther :M.S.
 *
 *************************************************************************** */

/*****************************************************************************
 *	�C���N���[�h�t�@�C��
 *****************************************************************************/

/*****************************************************************************
 *	�萔�}�N��
 *****************************************************************************/
/* �s�b�N�A�b�v�̐��䌠���擾���Ă��Ȃ� */
#define GFCD_UID_NONE	-1

#define GFCD_ANY_FLT	-2		/* �U�o�̓R�l�N�^�̐ݒ��ύX���Ȃ� */

/* �^�C���A�E�g�֌W */
#define GFCD_TMOUT_COUNT	0x30000	/* �f�t�H���g�^�C���A�E�g�� */
#define GFCD_NO_TMOUT		0	/* �^�C���A�E�g����Ȃ��w�� */

/* �G���[�R�[�h */
#define GFCD_ERR_OK		0	/* ����I�� */
#define GFCD_ERR_WAIT		-1	/* �����҂� */
#define GFCD_ERR_NOCDBLK	-2	/* CD�u���b�N���ڑ�����Ă��Ȃ� */
#define GFCD_ERR_NOFILT		-3	/* �󂫍i�肪�Ȃ� */
#define GFCD_ERR_NOBUF		-4	/* �󂫋�悪�Ȃ� */
#define GFCD_ERR_INUSE		-5	/* �w�肳�ꂽ�������g�p�� */
#define GFCD_ERR_RANGE		-6	/* �������͈͊O */
#define GFCD_ERR_UNUSE		-7	/* ���m�ۂ̂��̂𑀍삵�悤�Ƃ��� */
#define GFCD_ERR_QFULL		-8	/* �R�}���h�L���[�������ς� */
#define GFCD_ERR_NOTOWNER	-9	/* �񏊗L�҂������𑀍삵�悤�Ƃ��� */
#define GFCD_ERR_CDC		-10	/* CDC����̃G���[ */
#define GFCD_ERR_CDBFS		-11	/* CD�u���b�N�t�@�C���V�X�e���G���[ */
#define GFCD_ERR_TMOUT		-12	/* �^�C���A�E�g */
#define GFCD_ERR_OPEN		-13	/* �g���C���J���Ă��� */
#define GFCD_ERR_NODISC		-14	/* �f�B�X�N�������Ă��Ȃ� */
#define GFCD_ERR_CDROM		-15	/* CD-ROM�łȂ��f�B�X�N�������Ă��� */
#define GFCD_ERR_FATAL		-16	/* �X�e�[�^�X��FATAL */

/* cdbfs�p�Z���N�^�ԍ� */
#define GFCD_SYS_SEL	(GFS_OPEN_MAX - 1)

/* GFCD_SetCsctFunc�p�萔 */
#define GFCD_NULLFUNC	((void (*)(void *))0)

/*****************************************************************************
 *	�񋓒萔
 *****************************************************************************/
/* GFCD_ExecServer�̏�� */
enum GfcdExecServerStat {
    GFCD_SVR_COMPLETED,			/* ���s���� */
    GFCD_SVR_BUSY,			/* ������ */
    GFCD_SVR_ERROR,			/* CDC�̃G���[���� */
    GFCD_SVR_TMOUT			/* �^�C���A�E�g */
};


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

/* GFCD���W���[���̏����� */
Sint32 GFCD_Init(GfsCdbMng *work, Bool use_cdbfs);

/* �o�b�t�@��抄�蓖�� */
Sint32 GFCD_AllocBuf(Sint32 *buf_no);

/* �o�b�t�@����� */
Sint32 GFCD_FreeBuf(Sint32 buf_no);

/* �i�芄�蓖�� */
Sint32 GFCD_AllocFilt(Sint32 *flt_no);

/* �i���� */
Sint32 GFCD_FreeFilt(Sint32 flt_no);

/* �ŏI�Z�b�V�����̊J�nFAD���擾���� */
Sint32 GFCD_GetBaseFad(void);

/* �s�b�N�A�b�v���䌠�擾 */
Sint32 GFCD_GetPickup(void);

/* �s�b�N�A�b�v���䌠��� */
Sint32 GFCD_UngetPickup(Sint32 puid);

/* �s�b�N�A�b�v�̐��䌠���擾���Ă��邩���ׂ� */
Sint32 GFCD_IsPuOwner(Sint32 puid);

/* �i��ݒ� */
Sint32 GFCD_SetFilt(Sint32 flt, Sint32 fmode, CdcSubh *subh,
		    Sint32 fad, Sint32 snum);

/* �i��擾 */
Sint32 GFCD_GetFilt(Sint32 flt, Sint32 *fmode, CdcSubh *subh);

/* CD�f�o�C�X�̐ڑ� */
Sint32 GFCD_SetCon(Sint32 flt, Sint32 puid);

/* CD�Đ� */
Sint32 GFCD_Play(CdcPly *pinfo, Sint32 puid);

/* �����f�[�^���擾 */
Sint32 GFCD_GetLenData(Sint32 bufno, Sint32 spos, Sint32 usct, Sint32 *nsct,
		       Sint32 *nbyte);

/* �f�[�^�]���v�� */
Sint32 GFCD_ReqData(Sint32 bufno, Sint32 sctpos, Sint32 nsct);

/* �f�[�^�]���I���̒ʒm */
Sint32 GFCD_EndData(Sint32 *nsct);

/* �o�b�t�@�f�[�^���� */
Sint32 GFCD_DelSctData(Sint32 bufno, Sint32 sctpos, Sint32 nsct);

/* �o�b�t�@�f�[�^�����`�F�b�N */
Bool GFCD_CheckDelSctData(Sint32 bufno);

/* CD�X�e�[�^�X�擾 */
Uint16 GFCD_GetStat(CdcStat *stat);

/* �s�b�N�A�b�v�̈ړ� */
Sint32 GFCD_MovePickup(CdcPos *pos, Sint32 puid);

/* �o�b�t�@�f�[�^�̈ړ� */
Sint32 GFCD_MoveSctData(Sint32 dst, Sint32 src, Sint32 spos, Sint32 snum);

/* �ړ������̃`�F�b�N */
Bool GFCD_CheckMove(void);

/* �Z�N�^���̎擾 */
Sint32 GFCD_GetSctInfo(Sint32 bufno, Sint32 sn, CdcSct *info);

/* �t�@�C�����擾 */
Sint32 GFCD_GetFileInfo(Sint32 fid, CdcFile *finfo);

/* �o�b�t�@�̋󂫃Z�N�^���擾 */
Sint32 GFCD_GetBufSiz(void);

/* CD�u���b�N�Ƃ̒ʐM */
Sint32 GFCD_ExecServer(void);

/* �^�C���A�E�g�ݒ� */
void GFCD_SetTimer(Sint32 tm_count);

/* GFCD_ExecServer�I���҂� */
Sint32 GFCD_WaitServer(void);

/* �J�����g�f�B���N�g���̐ݒ� */
Sint32 GFCD_ChgDir(Sint32 fid, Sint32 work, Sint32 *ndir);

/* CDBFS���g�����t�@�C���̓ǂݍ��� */
Sint32 GFCD_ReadFile(Sint32 fltno, Sint32 fid, Sint32 ofs, Sint32 puid);

/* �i��ƃo�b�t�@�̐ڑ� */
Sint32 GFCD_SetFiltCon(Sint32 flt, Sint32 buf, Sint32 flnout);

/* CSCT�r�b�g���P�ɂȂ�����ĂԊ֐���o�^ */
void GFCD_SetCsctFunc(void (*func)(void *), void *obj);

/* SCDQ�t���O�����̂�҂� */
Bool GFCD_WaitScdqFlag(void);

/* end of file */
