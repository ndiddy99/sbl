/*****************************************************************************
 *	�\�t�g�E�F�A���C�u����
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:�t�@�C���V�X�e��
 * Module :CDC�֐��Ƃ̃C���^�t�F�[�X
 * File	  :gfs_cdc.c
 * Date   :1994-11-11
 * Version:1.16
 * Auther :M.S.
 *
 * �@�\�T�v:
 *   CD�u���b�N�����̊Ǘ���CDC�֐����g����CD�u���b�N�ւ̃R�}���h���s���s�Ȃ��B
 *   ���̃��W���[���͎���CD�u���b�N�������Ǘ�����B
 *	�E�i��
 *	�E�o�b�t�@���
 *	�E�s�b�N�A�b�v
 *	�E�f�[�^���W�X�^�i�Z�N�^�f�[�^�ړ��A�폜���܂ށj
 *
 *****************************************************************************/

/*****************************************************************************
 *	�C���N���[�h�t�@�C��
 *****************************************************************************/
#include "sega_gfs.h"
#include "gfs_cdc.h"
#include "gfs_def.h"

/*****************************************************************************
 *	�萔�}�N��
 *****************************************************************************/
#define SCDQ_COUNT_SEC	60		/* SCDQ�t���O��1s�ɗ��� */
#define TMOUT_DRV_COUNT	(10 * SCDQ_COUNT_SEC) /* �h���C�u�֌W�̃^�C���A�E�g */

#define NOBODY	-1			/* ���L�҂͂��Ȃ� */

#define CDBLK_SWRST	0x00		/* CD�u���b�N�\�t�g���Z�b�g���Ȃ�   */
#define SUBCODE_DECODE	0x00		/* �T�u�R�[�h�f�R�[�h���Ȃ�         */
#define USE_MODE2	0x00		/* ���[�h2�T�u�w�b�_�F������        */
#define FORM2_RDRETRY	0x00		/* �t�H�[��2���[�h���g���C���Ȃ�    */
#define HIGH_SPEED	0x00		/* �{��                             */
#define NCHG_INIT_FLAG	0x00		/* �������t���O�ύX����             */

#define INIT_FLAG       (CDBLK_SWRST|SUBCODE_DECODE|USE_MODE2|FORM2_RDRETRY|\
			 HIGH_SPEED|NCHG_INIT_FLAG)

#define DEF_STBYTIM     0x0000		/* �f�t�H���g�^�C��(180�b) 	    */
#if	!defined(CHK_RDERR)
#define DEF_ECC         0x04		/* �f�t�H���gECC��(5��)           */
#define DEF_RETRY       0x0f		/* �f�t�H���g���g���C��(15��)     */
#else
#define DEF_ECC         0x00		/* �f�t�H���gECC��(5��)           */
#define DEF_RETRY       0x00		/* �f�t�H���g���g���C��(15��)     */
#endif

/* �Z���N�^�̃��Z�b�g�t���O */
#define RESET_FLG	0xfc

#define MAX_CMD_PER_EXEC	30	/* ExecServer���̍ő唭�s�R�}���h�� */

/* CDC_SetFiltCon��cflag */
#define FLTCON_BUF_SET	1		/* �^�o�͂�ݒ� */
#define FLTCON_FLN_SET	2		/* �U�o�͂�ݒ� */

#define CDSTAT_MASK	0x0f		/* ��ԕ����Ƃ肾���}�X�N */

#define	TRREG_UNUSED	(-1)		/* �f�[�^�]�����W�X�^�g�p��	*/

/* 40ms�̃^�C���A�E�g���[�v�� */
#define SCDQ_TMOUT_COUNT	(12186*2)

/*****************************************************************************
 *	�񋓒萔
 *****************************************************************************/
/* CD�u���b�N�̎����̎g�p�󋵁A���g�p�E�g�p�� */
enum CdbResStat {UNUSED, INUSE};

/* �e�T�[�o�[�^�X�N�̃X�e�[�^�X */
enum TaskStatus {
    GFCD_TSK_READY,			/* ���N�G�X�g�҂���� */
    GFCD_TSK_STAT1, GFCD_TSK_STAT2, GFCD_TSK_STAT3, GFCD_TSK_STAT4,
    GFCD_TSK_STAT5
};

/* �^�X�NID */
enum TskId {
    TSK_ID_SETFLT = 0,
    TSK_ID_SETCON,
    TSK_ID_GETLEN,
    TSK_ID_REQDAT,
    TSK_ID_DELSCT,
    TSK_ID_MOVSCT,
    TSK_ID_FLTCON,
    TSK_ID_CHGDIR,
    TSK_ID_END
    };


/*****************************************************************************
 *	�����}�N��
 *****************************************************************************/
#define GFS_WORD_BYTE(word)         ((word) * 2)

#define IS_ROM(toc)	((toc) & 0x40000000)

/* CD�u���b�N�Ǘ��\����(GfsCdbMng)�A�N�Z�X�}�N�� */
#define CDBMNG_USEBUF(mng, i)	((mng)->use_buf[i])
#define CDBMNG_USEFILT(mng, i)	((mng)->use_filt[i])
#define CDBMNG_USEPU(mng)	((mng)->use_pu)
#define CDBMNG_TRBUFNO(mng)	((mng)->tr_bufno)
#define CDBMNG_PUID(mng)	((mng)->puid)
#define CDBMNG_STAT(mng)	((mng)->stat)
#define CDBMNG_TIMER(mng)	((mng)->timer)
#define CDBMNG_FUNC(mng)	((mng)->func)
#define CDBMNG_OBJ(mng)		((mng)->obj)

/* �^�X�N���A�N�Z�X�}�N�� */
/* (0) GFCD_SetFilt */
#define TSK_SETFLT_LEN(mng)	((mng)->tsk_setflt.len)
#define TSK_SETFLT_STAT(mng)	((mng)->tsk_setflt.stat)
#define TSK_SETFLT_QELT(mng, i)	((mng)->tsk_setflt.selq[i])
#define TSK_SETFLT_FLT(mng, i)	(TSK_SETFLT_QELT(mng, i).flt)
#define TSK_SETFLT_FMODE(mng, i)	(TSK_SETFLT_QELT(mng, i).fmode)
#define TSK_SETFLT_SUBH(mng, i)	(TSK_SETFLT_QELT(mng, i).subh)
#define TSK_SETFLT_FAD(mng, i)	(TSK_SETFLT_QELT(mng, i).fad)
#define TSK_SETFLT_SNUM(mng, i)	(TSK_SETFLT_QELT(mng, i).snum)
/* (1) GFCD_SetCon */
#define TSK_SETCON_STAT(mng)	((mng)->tsk_setcon.stat)
#define TSK_SETCON_FLT(mng)	((mng)->tsk_setcon.flt)
/* (2) GFCD_GetLenData */
#define TSK_GETLEN_STAT(mng)	((mng)->tsk_getlen.stat)
#define TSK_GETLEN_BUFNO(mng)	((mng)->tsk_getlen.bufno)
#define TSK_GETLEN_SPOS(mng)	((mng)->tsk_getlen.spos)
#define TSK_GETLEN_USCT(mng)	((mng)->tsk_getlen.usct)
#define TSK_GETLEN_CNT(mng)	((mng)->tsk_getlen.cnt)
#define TSK_GETLEN_NSCT(mng)	((mng)->tsk_getlen.nsct)
#define TSK_GETLEN_NBYTE(mng)	((mng)->tsk_getlen.nbyte)
/* (3) GFCD_ReqData */
#define TSK_REQDAT_STAT(mng)	((mng)->tsk_reqdat.stat)
#define TSK_REQDAT_BUFNO(mng)	((mng)->tsk_reqdat.bufno)
#define TSK_REQDAT_SCTPOS(mng)	((mng)->tsk_reqdat.sctpos)
#define TSK_REQDAT_NSCT(mng)	((mng)->tsk_reqdat.nsct)
/* (4) GFCD_DelSctData */
#define TSK_DELSCT_STAT(mng)	((mng)->tsk_delsct.stat)
#define TSK_DELSCT_BUFNO(mng)	((mng)->tsk_delsct.bufno)
#define TSK_DELSCT_SCTPOS(mng)	((mng)->tsk_delsct.sctpos)
#define TSK_DELSCT_NSCT(mng)	((mng)->tsk_delsct.nsct)
/* (5) GFCD_MoveSctData */
#define TSK_MOVSCT_STAT(mng)	((mng)->tsk_movsct.stat)
#define TSK_MOVSCT_DST(mng)	((mng)->tsk_movsct.dst)
#define TSK_MOVSCT_SRC(mng)	((mng)->tsk_movsct.src)
#define TSK_MOVSCT_SPOS(mng)	((mng)->tsk_movsct.spos)
#define TSK_MOVSCT_SNUM(mng)	((mng)->tsk_movsct.snum)
#define TSK_MOVSCT_FMODE(mng)	((mng)->tsk_movsct.fmode)
/* (6) GFCD_ChgDir */
#define TSK_CHGDIR_STAT(mng)	((mng)->tsk_chgdir.stat)
#define TSK_CHGDIR_FID(mng)	((mng)->tsk_chgdir.fid)
#define TSK_CHGDIR_WORK(mng)	((mng)->tsk_chgdir.work)
#define TSK_CHGDIR_NDIR(mng)	((mng)->tsk_chgdir.ndir)
/* (7) GFCD_SetFiltCon */
#define TSK_FLTCON_LEN(mng)	((mng)->tsk_fltcon.len)
#define TSK_FLTCON_STAT(mng)	((mng)->tsk_fltcon.stat)
#define TSK_FLTCON_QELT(mng, i)	((mng)->tsk_fltcon.fconq[i])
#define TSK_FLTCON_FLT(mng, i)		(TSK_FLTCON_QELT(mng, i).flt)
#define TSK_FLTCON_BUF(mng, i)		(TSK_FLTCON_QELT(mng, i).buf)
#define TSK_FLTCON_FLNOUT(mng, i)	(TSK_FLTCON_QELT(mng, i).flnout)

/*****************************************************************************
 *	�f�[�^�^�̒�`
 *****************************************************************************/
/* �Z�b�V������� */
typedef union {
    Uint8 nses[4];
    Uint32 xfad;
} GfcdSes;

/* �Z�b�V�������(GfcdSes)�A�N�Z�X�}�N�� */
#define GFCD_SES_NSES(ses)	((ses)->nses[0])
#define GFCD_SES_SFAD(ses)	((ses)->xfad & 0x00ffffff)


/*****************************************************************************
 *	�֐��̐錾
 *****************************************************************************/
/* �^�X�N��Ԕ��� */
GFS_LOCAL Bool	gfcd_isIdleTask(Sint32 tsk);

/* �^�X�N���s */
GFS_LOCAL Sint32	gfcd_doTask(Sint32 tsk, Sint32 *ncmd);

/* SetFilt�^�X�N */
GFS_LOCAL Sint32	gfcd_doSetFiltTsk(Sint32 *ncmd);

/* GFCD_SetCon�^�X�N */
GFS_LOCAL Sint32	gfcd_doSetConTsk(Sint32 *ncmd);

/* GFCD_GetLenData�^�X�N */
GFS_LOCAL Sint32	gfcd_doGetLenDataTsk(Sint32 *ncmd);

/* GFCD_ReqData�^�X�N */
GFS_LOCAL Sint32	gfcd_doReqDataTsk(Sint32 *ncmd);

/* GFCD_DelSctData�^�X�N */
GFS_LOCAL Sint32	gfcd_doDelSctDataTsk(Sint32 *ncmd);

/* GFCD_MoveSctData�^�X�N */
GFS_LOCAL Sint32	gfcd_doMoveSctTsk(Sint32 *ncmd);

/* ChgDir�^�X�N */
GFS_LOCAL Sint32	gfcd_doChgDirTsk(Sint32 *ncmd);

/* SetFiltCon�^�X�N */
GFS_LOCAL Sint32	gfcd_doSetFiltConTsk(Sint32 *ncmd);

/* �I���t���O�𒲂ׂ� */
GFS_LOCAL Bool		gfcd_checkEflag(Uint16 mask);

/* CD�u���b�N�Ǘ��\���̏����� */
GFS_LOCAL void		gfcd_initCdbMng(void);

/* �X�e�[�^�X���X�V���� */
GFS_LOCAL void		gfcd_setStat(void);

/*****************************************************************************
 *	�ϐ��̒�`
 *****************************************************************************/
/* CD�u���b�N�Ǘ����W���[���̍�Ɨ̈�A�h���X */
GfsCdbMng *gfcd_work;

/*****************************************************************************
 *	�֐��̒�`
 *****************************************************************************/
/*****************************************************************************
 *  GFCD���W���[���̏�����
 *�m���́n
 *  work	GFCD���W���[���̍�Ɨ̈�
 *  use_cdbfs	CD�t�@�C���V�X�e���g�p�w��
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  �G���[�R�[�h
 *�m�@�\�n
 *  
 *�m���l�n
 *  �������A
 *****************************************************************************/
Sint32 GFCD_Init(GfsCdbMng *work, Bool use_cdbfs)
{
    Sint32 tm_count;
    Bool ok_flag;
    Uint32 *toc_tbl;
    Sint32 ret;
    CdcStat stat;
    GfcdSes ses;

    gfcd_work = work;
    gfcd_initCdbMng();
    /* CD�u���b�N�t�@�C���V�X�e���p�̃Z���N�^�ƃo�b�t�@���m�� */
    if (use_cdbfs) {
	CDBMNG_USEBUF(gfcd_work, GFCD_SYS_SEL) = INUSE;
	CDBMNG_USEFILT(gfcd_work, GFCD_SYS_SEL) = INUSE;
    } else {
	CDBMNG_USEBUF(gfcd_work, GFCD_SYS_SEL) = UNUSED;
	CDBMNG_USEFILT(gfcd_work, GFCD_SYS_SEL) = UNUSED;
    }
    GFCD_GetStat(&stat);
    switch (CDC_STAT_STATUS(&stat) & CDSTAT_MASK) {
    case CDC_ST_OPEN:
	return GFCD_ERR_OPEN;
    case CDC_ST_NODISC:
	return GFCD_ERR_NODISC;
    case CDC_ST_ERROR:
	return GFCD_ERR_CDC;
    case CDC_ST_FATAL:
	return GFCD_ERR_FATAL;
    }
    if (CDC_GetHirqReq() & CDC_HIRQ_DCHG) {
	return GFCD_ERR_OPEN;
    }

    /* CD�u���b�N�t�@�C���V�X�e���̎g�p�����l�������΍�	*/
    CDC_AbortFile();

    /* �|�[�Y��Ԃɂ����� */
    /* �Q�{���A�X�^���o�C�^�C���F180s, ECC:5, retry:15�� */
    ret = CDC_CdInit(INIT_FLAG, DEF_STBYTIM, DEF_ECC, DEF_RETRY);
    if (ret != CDC_ERR_OK) {
	return GFCD_ERR_NOCDBLK;
    }

    /* �Z�N�^���̐ݒ�	*/
    for (tm_count = 0; ;tm_count++) {
	ret = CDC_SetSctLen(CDC_SLEN_2048, CDC_SLEN_NOCHG);
	if (ret == CDC_ERR_OK) {
	    break;
	}
	if ((ret != CDC_ERR_CMDBUSY)||(tm_count > GFCD_TMOUT_COUNT)) {
	    return GFCD_ERR_NOCDBLK;
	}
    }
    CDBMNG_TIMER(gfcd_work) = GFCD_TMOUT_COUNT;
    while (gfcd_checkEflag(CDC_HIRQ_ESEL) == FALSE) {
	if (--CDBMNG_TIMER(gfcd_work) == 0) {
	    return GFCD_ERR_NOCDBLK;
	}
    }
    CDBMNG_TIMER(gfcd_work) = GFCD_NO_TMOUT;

    /* PEND, BFUL�̃N���A			*/
    CDC_ClrHirqReq(~(CDC_HIRQ_PEND|CDC_HIRQ_BFUL));
    
    /* CD�u���b�N�ڑ��`�F�b�N����~��ԑ҂�	*/
    tm_count = 0;
    ok_flag = FALSE;
    while (ok_flag == FALSE) {
	if (gfcd_checkEflag(CDC_HIRQ_SCDQ) == FALSE) {
	    continue;
	}
	CDC_ClrHirqReq(~CDC_HIRQ_SCDQ);
	++tm_count;
	if (tm_count > TMOUT_DRV_COUNT) {
	    return GFCD_ERR_NOCDBLK;
	}
	GFCD_GetStat(&stat);
	switch (CDC_STAT_STATUS(&stat) & CDSTAT_MASK) {
	case CDC_ST_ERROR:
	    return GFCD_ERR_CDC;
	case CDC_ST_FATAL:
	    return GFCD_ERR_FATAL;
	case CDC_ST_PAUSE:
	case CDC_ST_STANDBY:
	    ok_flag = TRUE;
	    break;
	}
    }
    /* �f�[�^�]�����I���iDRDY=1�҂��ŏI���𔭍s����Ɗ댯�j 		*/
    /* GFS�J�n��1msec���x�҂K�v������̂őO���Ɉړ����邱�Ƃ͕s��	*/
    CDC_DataEnd(&ret);

    /* CD-ROM�ł��邱�Ƃ̃`�F�b�N */
    toc_tbl = (Uint32 *)MNG_SECTBUF(gfs_mng_ptr);
    ret = CDC_TgetToc(toc_tbl);
    if (ret != CDC_ERR_OK) {
	/* OPEN��NODISC�̂͂������A�O�Ƀ`�F�b�N���Ă���̂ł���Ȃ͂��͂Ȃ� */
	return GFCD_ERR_NOCDBLK;
    }
    /* �ŏI�Z�b�V������P�g���b�N���f�[�^�g���b�N�ł��邱�Ƃ��m�F */
    CDC_GetSes(0, (Uint32 *)&ses);	/* �Z�b�V�������擾 */
    CDC_GetSes(GFCD_SES_NSES(&ses), (Uint32 *)&ses); /* �ŏI�Z�b�V�����̏�� */
    if (IS_ROM(toc_tbl[GFCD_SES_NSES(&ses) - 1]) == FALSE) {
	return GFCD_ERR_CDROM;
    }
    /* �S�Z���N�^���N���A */
    ret = CDC_ResetSelector(RESET_FLG, CDC_NUL_SEL);
    CDBMNG_TIMER(gfcd_work) = GFCD_TMOUT_COUNT;
    while (gfcd_checkEflag(CDC_HIRQ_ESEL) == FALSE) {
	if (--CDBMNG_TIMER(gfcd_work) == 0) {
	    return GFCD_ERR_NOCDBLK;
	}
    }
    CDBMNG_TIMER(gfcd_work) = GFCD_NO_TMOUT;
    GFCD_GetStat(&CDBMNG_STAT(gfcd_work));
    return GFCD_ERR_OK;
}


/*****************************************************************************
 *  �i�芄�蓖��
 *�m���́n
 *  flt_no	���蓖�Ă�ꂽ�i��ԍ��̊i�[��
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFCD_AllocFilt(Sint32 *flt_no)
{
    Sint32 i;

    for (i = 0; i < GFS_CDBBUF_NR; ++i) {
	if (CDBMNG_USEFILT(gfcd_work, i) == UNUSED) {
	    CDBMNG_USEFILT(gfcd_work, i) = INUSE;
	    *flt_no = i;
	    return GFCD_ERR_OK;
	}
    }
    *flt_no = -1;
    return GFCD_ERR_NOFILT;
}


/*****************************************************************************
 *  �i����
 *�m���́n
 *  flt_no	�������i��̔ԍ�
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  
 *�m�@�\�n
 *  
 *�m���l�n
 *  �������A
 *****************************************************************************/
Sint32 GFCD_FreeFilt(Sint32 flt_no)
{
    if (flt_no < 0 || GFS_CDBBUF_NR <= flt_no) {
	return GFCD_ERR_RANGE;
    }
    if (CDBMNG_USEFILT(gfcd_work, flt_no) != INUSE) {
	return GFCD_ERR_UNUSE;
    }
    CDBMNG_USEFILT(gfcd_work, flt_no) = UNUSED;
    return GFCD_ERR_OK;
}


/*****************************************************************************
 *  �o�b�t�@��抄�蓖��
 *�m���́n
 *  buf_no	���蓖�Ă�ꂽ�o�b�t�@�ԍ��̊i�[��
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  
 *�m�@�\�n
 *  
 *�m���l�n
 *  �������A
 *****************************************************************************/
Sint32 GFCD_AllocBuf(Sint32 *buf_no)
{
    Sint32 i;

    for (i = 0; i < GFS_CDBBUF_NR; ++i) {
	if (CDBMNG_USEBUF(gfcd_work, i) == UNUSED) {
	    CDBMNG_USEBUF(gfcd_work, i) = INUSE;
	    *buf_no = i;
	    return GFCD_ERR_OK;
	}
    }
    *buf_no = -1;
    return GFCD_ERR_NOBUF;
}


/*****************************************************************************
 *  �o�b�t�@�����
 *�m���́n
 *  buf_no	�������o�b�t�@���
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  
 *�m�@�\�n
 *  ���蓖�Ă��Ă����o�b�t�@��������A�܂܂�Ă����Z�N�^�f�[�^��S�폜����B
 *�m���l�n
 *  �������A
 *****************************************************************************/
Sint32 GFCD_FreeBuf(Sint32 buf_no)
{
    Sint32 err;

    if (buf_no < 0 || GFS_CDBBUF_NR <= buf_no) {
	return GFCD_ERR_RANGE;
    }
    if (CDBMNG_USEBUF(gfcd_work, buf_no) != INUSE) {
	return GFCD_ERR_UNUSE;
    }
    /* bufno�̑S�Z�N�^�f�[�^�폜 */
    GFCD_DelSctData(buf_no, 0, CDC_SNUM_END);
    GFCD_SetTimer(GFCD_TMOUT_COUNT);
    for (;;) {
	err = GFCD_ExecServer();
	if (err == GFCD_SVR_TMOUT) {
	    return GFCD_ERR_TMOUT;
	} else if (err == GFCD_SVR_ERROR) {
	    return GFCD_ERR_FATAL;
	}
	if (GFCD_CheckDelSctData(buf_no) == TRUE) {
	    break;
	}
    }
    GFCD_SetTimer(GFCD_NO_TMOUT);
    CDBMNG_USEBUF(gfcd_work, buf_no) = UNUSED;
    return GFCD_ERR_OK;
}


/* �ŏI�Z�b�V�����̊J�nFAD���擾���� */
Sint32 GFCD_GetBaseFad(void)
{
    GfcdSes ses;

    CDC_GetSes(0, (Uint32 *)&ses);	/* �Z�b�V�������擾 */
    CDC_GetSes(GFCD_SES_NSES(&ses), (Uint32 *)&ses); /* �ŏI�Z�b�V�����̏�� */
    return GFCD_SES_SFAD(&ses);		/* �J�nFAD�擾 */
}


/*****************************************************************************
 *  �s�b�N�A�b�v���䌠�擾
 *�m���́n
 *  �Ȃ�
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 *  ���[�UID
 *�m�@�\�n
 *  �s�b�N�A�b�v�̐��䌠���擾�ł�����A���[�UID��Ԃ�
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFCD_GetPickup(void)
{
    /* �s�b�N�A�b�v�����g�p�ł��邱�Ƃ��m�F */
    if (CDBMNG_USEPU(gfcd_work) == INUSE) {
	return GFCD_ERR_INUSE;
    }
    CDBMNG_USEPU(gfcd_work) = INUSE;
    ++CDBMNG_PUID(gfcd_work);
    if (CDBMNG_PUID(gfcd_work) < 0) {
	CDBMNG_PUID(gfcd_work) = 0;
    }
    return CDBMNG_PUID(gfcd_work);
}


/*****************************************************************************
 *  �s�b�N�A�b�v���䌠���
 *�m���́n
 *  uid	�s�b�N�A�b�v�̐��䌠�������Ă���uid
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  
 *�m�@�\�n
 *  
 *�m���l�n
 *  �i��̊����ƃs�b�N�A�b�v�̐��䌠���擾���Ă��邱��
 *****************************************************************************/
Sint32 GFCD_UngetPickup(Sint32 puid)
{
    /* �s�b�N�A�b�v���g�p���ł��邱�Ƃ��m�F */
    if (CDBMNG_USEPU(gfcd_work) == UNUSED) {
	return GFCD_ERR_UNUSE;
    }
    if (CDBMNG_PUID(gfcd_work) != puid) {
	return GFCD_ERR_NOTOWNER;
    }
    /* ���g�p��Ԃɂ��� */
    CDBMNG_USEPU(gfcd_work) = UNUSED;
    return GFCD_ERR_OK;
}


/*****************************************************************************
 *  �s�b�N�A�b�v�̐��䌠���擾���Ă��邩���ׂ�
 *�m���́n
 *  puid	���[�UID
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFCD_IsPuOwner(Sint32 puid)
{
    if (CDBMNG_USEPU(gfcd_work) == UNUSED) {
	return FALSE;
    }
    return (CDBMNG_PUID(gfcd_work) == puid)? TRUE: FALSE;
}


/*****************************************************************************
 *  �i��ݒ�
 *�m���́n
 *  flt		�ݒ��i��̔ԍ�
 *  fmode	�i�����
 *  subh	�T�u�w�b�_����
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  
 *�m�@�\�n
 *  
 *�m���l�n
 *  �i��̊������󂯂Ă��邱��
 *****************************************************************************/
Sint32 GFCD_SetFilt(Sint32 flt, Sint32 fmode, CdcSubh *subh,
		    Sint32 fad, Sint32 snum)
{
    Sint32 ncmd;
    Sint32 len = TSK_SETFLT_LEN(gfcd_work);

    /* ���蓖�Ă�ꂽ�i��ɑ΂��Ă̑���ł��邱�Ƃ��m�F */
    if (CDBMNG_USEFILT(gfcd_work, flt) != INUSE) {
	return GFCD_ERR_UNUSE;
    }
    /* �L���[�ɋ󂫂����邱�Ƃ��m�F */
    if (len >= GFS_SELQ_MAX) {
	return GFCD_ERR_QFULL;
    }
    /* �L���[�ɓo�^ */
    if (TSK_SETFLT_STAT(gfcd_work) == GFCD_TSK_READY) {
	TSK_SETFLT_STAT(gfcd_work) = GFCD_TSK_STAT1;
    }
    TSK_SETFLT_FLT(gfcd_work, len) = flt;
    TSK_SETFLT_FMODE(gfcd_work, len) = fmode;
    TSK_SETFLT_SUBH(gfcd_work, len) = *subh;
    TSK_SETFLT_FAD(gfcd_work, len) = fad;
    TSK_SETFLT_SNUM(gfcd_work, len) = snum;
    ++TSK_SETFLT_LEN(gfcd_work);
    gfcd_doSetFiltTsk(&ncmd);
    return GFCD_ERR_OK;
}


/* �i��擾 */
Sint32 GFCD_GetFilt(Sint32 flt, Sint32 *fmode, CdcSubh *subh)
{
    Sint32 ret;

    ret = CDC_GetFiltMode(flt, fmode);
    if (ret != CDC_ERR_OK) {
	return GFCD_ERR_CDC;
    }
    ret = CDC_GetFiltSubh(flt, subh);
    if (ret != CDC_ERR_OK) {
	return GFCD_ERR_CDC;
    }
    gfcd_setStat();
    return GFCD_ERR_OK;
}


/*****************************************************************************
 *  CD�f�o�C�X�̐ڑ�
 *�m���́n
 *  flt	CD�f�o�C�X��ڑ�����i��̔ԍ�
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  
 *�m�@�\�n
 *  
 *�m���l�n
 *  �i��̊����ƃs�b�N�A�b�v�̐��䌠���擾���Ă��邱��
 *****************************************************************************/
Sint32 GFCD_SetCon(Sint32 flt, Sint32 puid)
{
    Sint32 ncmd;
    
    /* ���蓖�Ă�ꂽ�i��ɑ΂��Ă̑���ł��邱�Ƃ��m�F */
    if (flt != CDC_NUL_SEL && CDBMNG_USEFILT(gfcd_work, flt) != INUSE) {
	return GFCD_ERR_UNUSE;
    }
    /* �s�b�N�A�b�v�̐��䌠���擾���Ă���i��łȂ���΃G���[ */
    if (CDBMNG_PUID(gfcd_work) != puid) {
	return GFCD_ERR_NOTOWNER;
    }
    TSK_SETCON_STAT(gfcd_work) = GFCD_TSK_STAT1;
    TSK_SETCON_FLT(gfcd_work) = flt;
    gfcd_doSetConTsk(&ncmd);
    return GFCD_ERR_OK;
}


/*****************************************************************************
 *  CD�Đ�
 *�m���́n
 *  pinfo	�Đ��p�����[�^
 *  puid	�s�b�N�A�b�v���䌠���擾���Ă���puid
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  
 *�m�@�\�n
 *  
 *�m���l�n
 *  �s�b�N�A�b�v�̐��䌠���擾���Ă��邱��
 *****************************************************************************/
Sint32 GFCD_Play(CdcPly *pinfo, Sint32 puid)
{
    Uint16 hirq;
    CdcStat stat;

    /* �s�b�N�A�b�v�̐��䌠���擾���Ă��Ȃ���΃G���[ */
    if (CDBMNG_PUID(gfcd_work) != puid) {
	return GFCD_ERR_NOTOWNER;
    }
    hirq = GFCD_GetStat(&stat);
    if (hirq & CDC_HIRQ_DCHG) {
	return GFCD_ERR_OPEN;
    }
    if ((CDC_STAT_STATUS(&stat) & 0x0f) == CDC_ST_NODISC) {
	return GFCD_ERR_NODISC;
    }
    CDC_ClrHirqReq(~CDC_HIRQ_PEND);
    CDC_CdPlay(pinfo);
    return GFCD_ERR_OK;
}


/*****************************************************************************
 *  �����f�[�^���擾
 *�m���́n
 *  
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  
 *�m�@�\�n
 *  
 *�m���l�n
 *  nbyte==NULL�Ńo�C�g���̏o�͂�}���ł���B
 *****************************************************************************/
Sint32 GFCD_GetLenData(Sint32 bufno, Sint32 spos, Sint32 usct, Sint32 *nsct,
		       Sint32 *nbyte)
{
    Sint32 ncmd;
    
    /* GetLenData�^�X�N�����s���łȂ����Ƃ��m�F���� */
    if (TSK_GETLEN_STAT(gfcd_work) != GFCD_TSK_READY) {
	return GFCD_ERR_WAIT;
    }
    TSK_GETLEN_STAT(gfcd_work) = GFCD_TSK_STAT1;
    TSK_GETLEN_BUFNO(gfcd_work) = bufno;
    TSK_GETLEN_SPOS(gfcd_work) = spos;
    TSK_GETLEN_USCT(gfcd_work) = usct;
    TSK_GETLEN_CNT(gfcd_work) = 0;
    TSK_GETLEN_NSCT(gfcd_work) = nsct;
    TSK_GETLEN_NBYTE(gfcd_work) = nbyte;
    gfcd_doGetLenDataTsk(&ncmd);
    return GFCD_ERR_OK;
}


/*****************************************************************************
 *  �f�[�^�]���v��
 *�m���́n
 *  
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFCD_ReqData(Sint32 bufno, Sint32 sctpos, Sint32 nsct)
{
    Sint32 ncmd;
    
    if (TSK_REQDAT_STAT(gfcd_work) != GFCD_TSK_READY) {
	return GFCD_ERR_WAIT;
    }
    /* �f�[�^���W�X�^���g���Ă��Ȃ����Ƃ��m�F���� */
    if (CDBMNG_TRBUFNO(gfcd_work) != TRREG_UNUSED) {
	return GFCD_ERR_INUSE;
    }
    CDBMNG_TRBUFNO(gfcd_work) = bufno;
    TSK_REQDAT_STAT(gfcd_work) = GFCD_TSK_STAT1;
    TSK_REQDAT_BUFNO(gfcd_work) = bufno;
    TSK_REQDAT_SCTPOS(gfcd_work) = sctpos;
    TSK_REQDAT_NSCT(gfcd_work) = nsct;
    gfcd_doReqDataTsk(&ncmd);
    return GFCD_ERR_OK;
}


/*****************************************************************************
 *  �f�[�^�]���I���̒ʒm
 *�m���́n
 *  
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFCD_EndData(Sint32 *nsct)
{
    Sint32 ret;

    /* �f�[�^���W�X�^���g�p�󋵂����āAReqData�����s���ꂽ���Ƃ��m�F���� */
    if (CDBMNG_TRBUFNO(gfcd_work) == TRREG_UNUSED) {
	return GFCD_ERR_UNUSE;
    }
    ret = CDC_DataEnd(nsct);
    CDBMNG_TRBUFNO(gfcd_work) = TRREG_UNUSED;
    if (ret != CDC_ERR_OK) {
	return GFCD_ERR_CDC;
    }
    gfcd_setStat();
    return GFCD_ERR_OK;
}


/*****************************************************************************
 *  �o�b�t�@�f�[�^����
 *�m���́n
 *	bufno	�o�b�t�@�ԍ�
 *	sctpos	�����擪�Z�N�^�ʒu
 *	nsct	�����Z�N�^��
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  
 *�m�@�\�n
 *  nsct��CDC_SNUM_END�̎��͍ŏI�Z�N�^�܂ł̃Z�N�^����\���B
 *�m���l�n
 *  �����̊����`�F�b�N��GFCD_CheckDelSctData���g�����ƁB
 *****************************************************************************/
Sint32 GFCD_DelSctData(Sint32 bufno, Sint32 sctpos, Sint32 nsct)
{
    Sint32 ncmd;
    
    if (TSK_DELSCT_STAT(gfcd_work) != GFCD_TSK_READY) {
	return GFCD_ERR_WAIT;
    }
    /* �f�[�^�]�����łȂ����Ƃ��m�F���� */
    if (CDBMNG_TRBUFNO(gfcd_work) == bufno) {
	return GFCD_ERR_INUSE;		/* �g���Ă���̃o�O */
    }
    TSK_DELSCT_STAT(gfcd_work) = GFCD_TSK_STAT1;
    TSK_DELSCT_BUFNO(gfcd_work) = bufno;
    TSK_DELSCT_SCTPOS(gfcd_work) = sctpos;
    TSK_DELSCT_NSCT(gfcd_work) = nsct;
    gfcd_doDelSctDataTsk(&ncmd);
    return GFCD_ERR_OK;
}


/*****************************************************************************
 *  �o�b�t�@�f�[�^�����`�F�b�N
 *�m���́n
 *  
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  
 *�m�@�\�n
 *   �����������I���Ă����TRUE�A�����łȂ����FALSE��Ԃ��B
 *   �����������s�Ȃ��Ă��Ȃ��ꍇ�͏I�����Ƃ݂Ȃ��B
 *  
 *�m���l�n
 *  
 *****************************************************************************/
Bool GFCD_CheckDelSctData(Sint32 bufno)
{
    /* bufno�ɑ΂��Ă�DELSCT�^�X�N�̑��݃`�F�b�N */
    if (TSK_DELSCT_STAT(gfcd_work) != GFCD_TSK_READY &&
	TSK_DELSCT_BUFNO(gfcd_work) == bufno) {
	return FALSE;
    }
    return TRUE;
}



/* CD�X�e�[�^�X�擾 */
Uint16 GFCD_GetStat(CdcStat *stat)
{
    Sint32 ret;
    Uint16 hirq;
    CdcPos	pos;

    if (stat != NULL) {
	ret = CDC_GetPeriStat(stat);
	if (ret != CDC_ERR_OK) {
	    CDC_GetCurStat(stat);
	}
    }
    hirq = CDC_GetHirqReq();
    if ((CDC_STAT_STATUS(stat) & 0x0f) == CDC_ST_FATAL) {
	if ((hirq & CDC_HIRQ_DCHG) == 0) {
	    CDC_POS_PTYPE(&pos) = CDC_PTYPE_DFL;
	    CDC_CdSeek(&pos);
	    /* SCDQ�t���O��2��҂�	*/
	    GFCD_WaitScdqFlag();
	    GFCD_WaitScdqFlag();
	}
    }
    return hirq;
}


/* �s�b�N�A�b�v�̈ړ� */
Sint32 GFCD_MovePickup(CdcPos *pos, Sint32 puid)
{
    Uint16 hirq;
    CdcStat stat;

    /* �s�b�N�A�b�v�̐��䌠���擾���Ă��Ȃ���΃G���[ */
    if (CDBMNG_PUID(gfcd_work) != puid) {
	return GFCD_ERR_NOTOWNER;
    }
    hirq = GFCD_GetStat(&stat);
    if (hirq & CDC_HIRQ_DCHG) {
	return GFCD_ERR_OPEN;
    }
    if ((CDC_STAT_STATUS(&stat) & 0x0f) == CDC_ST_NODISC) {
	return GFCD_ERR_NODISC;
    }
    CDC_CdSeek(pos);
    return GFCD_ERR_OK;
}



/* �o�b�t�@�f�[�^�̈ړ� */
Sint32 GFCD_MoveSctData(Sint32 dst, Sint32 src, Sint32 spos, Sint32 snum)
{
    Sint32 ncmd;
    
    if (CDBMNG_USEFILT(gfcd_work, dst) == UNUSED ||
	CDBMNG_USEFILT(gfcd_work, src) == UNUSED) {
	return GFCD_ERR_UNUSE;
    }
    if (TSK_MOVSCT_STAT(gfcd_work) != GFCD_TSK_READY) {
	return GFCD_ERR_WAIT;
    }
    TSK_MOVSCT_STAT(gfcd_work) = GFCD_TSK_STAT1;
    TSK_MOVSCT_DST(gfcd_work) = dst;
    TSK_MOVSCT_SRC(gfcd_work) = src;
    TSK_MOVSCT_SPOS(gfcd_work) = spos;
    TSK_MOVSCT_SNUM(gfcd_work) = snum;
    gfcd_doMoveSctTsk(&ncmd);
    return GFCD_ERR_OK;
}


/*****************************************************************************
 *  �ړ������̃`�F�b�N
 *�m���́n
 *  
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  
 *�m�@�\�n
 *   �Z�N�^�f�[�^�ړ��^�X�N���I�����Ă��邩�`�F�b�N����B
 *   �Z�N�^�f�[�^�ړ��^�X�N���Ȃ���ΏI�����Ă���Ƃ݂Ȃ��B
 *�m���l�n
 *  
 *****************************************************************************/
Bool GFCD_CheckMove(void)
{
    if (TSK_MOVSCT_STAT(gfcd_work) != GFCD_TSK_READY) {
	return FALSE;
    }
    return TRUE;
}



/* �Z�N�^���̎擾 */
Sint32 GFCD_GetSctInfo(Sint32 bufno, Sint32 sn, CdcSct *info)
{
    Sint32 ret;

    if (bufno < 0 || GFS_CDBBUF_NR <= bufno) {
	return GFCD_ERR_RANGE;
    }
    if (CDBMNG_USEBUF(gfcd_work, bufno) == UNUSED) {
	return GFCD_ERR_UNUSE;
    }
    ret = CDC_GetSctInfo(bufno, sn, info);
    if (ret != CDC_ERR_OK) {
	return GFCD_ERR_CDC;
    }
    gfcd_setStat();
    return GFCD_ERR_OK;
}



/* �J�����g�f�B���N�g���̐ݒ� */
Sint32 GFCD_ChgDir(Sint32 fid, Sint32 work, Sint32 *ndir)
{
    Sint32 ncmd;
    
    /* �Z���N�^���g�p�\���`�F�b�N */
    if (CDBMNG_USEFILT(gfcd_work, work) != INUSE ||
	CDBMNG_USEBUF(gfcd_work, work) != INUSE) {
	return GFCD_ERR_INUSE;
    }
    if (TSK_CHGDIR_STAT(gfcd_work) != GFCD_TSK_READY) {
	return GFCD_ERR_WAIT;
    }
    /* �^�X�N��o�^ */
    if (fid == CDC_NUL_FID) {
	TSK_CHGDIR_STAT(gfcd_work) = GFCD_TSK_STAT3;
    } else {
	TSK_CHGDIR_STAT(gfcd_work) = GFCD_TSK_STAT1;
    }
    TSK_CHGDIR_FID(gfcd_work) = fid;
    TSK_CHGDIR_WORK(gfcd_work) = work;
    TSK_CHGDIR_NDIR(gfcd_work) = ndir;
    gfcd_doChgDirTsk(&ncmd);
    return GFCD_ERR_OK;
}



/*****************************************************************************
 *  �i��ƃo�b�t�@�̐ڑ�
 *�m���́n
 *  
 *�m�o�́n
 *  
 *�m�֐��l�n
 *  
 *�m�@�\�n
 *   flnout��GFCD_ANY_FLT�Ȃ�U�o�̓R�l�N�^�̐ݒ��ύX���Ȃ�
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFCD_SetFiltCon(Sint32 flt, Sint32 buf, Sint32 flnout)
{
    Sint32 ncmd;
    Sint32 len = TSK_FLTCON_LEN(gfcd_work);

    /* �͈̓`�F�b�N */
    if ((flt != CDC_NUL_SEL && flt != GFCD_ANY_FLT &&
	 (flt < 0 || GFS_CDBBUF_NR <= flt)) ||
	(buf < 0 || GFS_CDBBUF_NR <= buf) ||
	(flnout != CDC_NUL_SEL && flnout != GFCD_ANY_FLT &&
	 (flnout < 0 || GFS_CDBBUF_NR <= flnout))) {
	return GFCD_ERR_RANGE;
    }
    /* ���蓖�Ă�ꂽ�����ɑ΂��Ă̏����ł��邱�Ƃ��m�F */
    if ((flt != CDC_NUL_SEL && flt != GFCD_ANY_FLT &&
	 CDBMNG_USEFILT(gfcd_work, flt) != INUSE) ||
	(flnout != CDC_NUL_SEL && flnout != GFCD_ANY_FLT &&
	 CDBMNG_USEFILT(gfcd_work, flnout) != INUSE) ||
	CDBMNG_USEBUF(gfcd_work, buf) != INUSE) {
	return GFCD_ERR_UNUSE;
    }
    /* �L���[�ɋ󂫂����邱�Ƃ��m�F */
    if (len >= GFS_FCONQ_MAX) {
	return GFCD_ERR_QFULL;
    }
    /* �^�X�N�̓o�^ */
    if (TSK_FLTCON_STAT(gfcd_work) == GFCD_TSK_READY) {
	TSK_FLTCON_STAT(gfcd_work) = GFCD_TSK_STAT1;
    }
    TSK_FLTCON_FLT(gfcd_work, len) = flt;
    TSK_FLTCON_BUF(gfcd_work, len) = buf;
    TSK_FLTCON_FLNOUT(gfcd_work, len) = flnout;
    ++TSK_FLTCON_LEN(gfcd_work);
    gfcd_doSetFiltConTsk(&ncmd);
    return GFCD_ERR_OK;
}


/* �t�@�C�����擾 */
Sint32 GFCD_GetFileInfo(Sint32 fid, CdcFile *finfo)
{
    Sint32 ret;
    Sint32 base_fid, infnum;
    Bool drend;

    ret = CDC_GetFileScope(&base_fid, &infnum, &drend);
    if (ret != CDC_ERR_OK) {
	return GFCD_ERR_CDBFS;
    }
    /* fid���L���͈͂ɂ͂����Ă��邩 */
    if (fid < base_fid || base_fid + infnum <= fid) {
	ret = CDC_ReadDir(GFCD_SYS_SEL, fid);
	if (ret != CDC_ERR_OK) {
	    return GFCD_ERR_CDBFS;	/* �G���[ */
	}
	/* ReadDir�I���҂� */
	while (gfcd_checkEflag(CDC_HIRQ_EFLS) == FALSE) {
	}
	ret = CDC_GetFileScope(&base_fid, &infnum, &drend);
	if (ret != CDC_ERR_OK ||
	    (fid < base_fid && base_fid + infnum <= fid)) {
	    return GFCD_ERR_CDBFS;
	}
    }
    /* �t�@�C�����擾 */
    ret = CDC_TgetFileInfo(fid, finfo);
    if (ret != CDC_ERR_OK) {
	return GFCD_ERR_CDBFS;
    }
    /* �f�B���N�g���̍Ōォ�H */
    if (drend && (base_fid + infnum - 1 == fid)) {
	CDC_FILE_ATR(finfo) |= GFS_ATR_END_TBL;
    }
    gfcd_setStat();
    return GFCD_ERR_OK;
}


/* �o�b�t�@�̋󂫃Z�N�^���擾 */
Sint32 GFCD_GetBufSiz(void)
{
    Sint32	ret;
    Sint32      bufsiz, partsiz, freesiz;   /* �Z�N�^�P��   */

    ret = CDC_GetBufSiz(&bufsiz, &partsiz, &freesiz);
    if (ret != CDC_ERR_OK) {
	return GFCD_ERR_CDC;
    }
    gfcd_setStat();
    return freesiz;
}


/*****************************************************************************
 *  CD�u���b�N�Ƃ̒ʐM
 *�m���́n
 *  �Ȃ�
 *�m�o�́n
 *  �Ȃ�
 *�m�֐��l�n
 * �c��^�X�N�̏��
 *	GFCD_SVR_COMPLETED	���s���̃^�X�N�͂Ȃ�
 *	GFCD_SVR_BUSY		���s���̃^�X�N�����݂���
 *	GFCD_SVR_ERROR		���s�ł��Ȃ�CD�̃G���[����������
 *	GFCD_SVR_TMOUT		�^�C���A�E�g
 *�m�@�\�n
 *   CD�u���b�N�̏�Ԃ��Ď����Ȃ���A�K�v��CD�R�}���h�𔭍s����B
 *   �������̐ݒ��CD�u���b�N�ɑ΂��čs�Ȃ������A����炪���ׂĊ����������Ƃ�
 * �֐��l�ɂ���Ē��ׂ邱�Ƃ��ł���B(�t�ɁA�ʂɂ͒��ׂ��Ȃ��B)
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFCD_ExecServer(void)
{
    Sint32 ncmd = 0;			/* ���s�����R�}���h�̐� */
    Sint32 ntsk = 0;			/* �c��^�X�N�� */
    Sint32 tsk;
    Sint32 ret;
    CdcStat stat;

    if (CDBMNG_TIMER(gfcd_work) != GFCD_NO_TMOUT) {
	if (--CDBMNG_TIMER(gfcd_work) <= 0) {
	    /* �^�C���A�E�g�̎���CD�u���b�N�Ǘ��\���̏����� */
	    gfcd_initCdbMng();
	    return GFCD_SVR_TMOUT;
	}
    }
    if (gfcd_checkEflag(CDC_HIRQ_SCDQ) == TRUE) {
	CDC_ClrHirqReq(~CDC_HIRQ_SCDQ);
	GFCD_GetStat(&CDBMNG_STAT(gfcd_work));
	if (CDBMNG_FUNC(gfcd_work) != NULL) {
	    CDBMNG_FUNC(gfcd_work)(CDBMNG_OBJ(gfcd_work));
	}
    } else {
	GFCD_GetStat(&stat);
	if ((CDC_STAT_STATUS(&stat) & 0x0f) == CDC_ST_FATAL) {
	    return GFCD_SVR_ERROR;
	}
	CDBMNG_STAT(gfcd_work) = stat;
    }
    /* �e�^�X�N���s */
    for (tsk = 0; tsk < TSK_ID_END; ++tsk) {
	if (gfcd_isIdleTask(tsk)) {
	    continue;
	}
	ret = gfcd_doTask(tsk, &ncmd);
	if (ret < 0) {
	    return ret;
	}
	ntsk += ret;
	if (ncmd > MAX_CMD_PER_EXEC) {
	    return GFCD_SVR_BUSY;
	}
    }
    return (ntsk > 0)? GFCD_SVR_BUSY: GFCD_SVR_COMPLETED;
}



/* �^�X�N��Ԕ��� */
GFS_LOCAL Bool 	gfcd_isIdleTask(Sint32 tsk)
{
    Sint32 stat;

    switch (tsk) {
    case TSK_ID_SETFLT:
	stat = TSK_SETFLT_STAT(gfcd_work);
	break;
    case TSK_ID_SETCON:
	stat = TSK_SETCON_STAT(gfcd_work);
	break;
    case TSK_ID_GETLEN:
	stat = TSK_GETLEN_STAT(gfcd_work);
	break;
    case TSK_ID_REQDAT:
	stat = TSK_REQDAT_STAT(gfcd_work);
	break;
    case TSK_ID_DELSCT:
	stat = TSK_DELSCT_STAT(gfcd_work);
	break;
    case TSK_ID_MOVSCT:
	stat = TSK_MOVSCT_STAT(gfcd_work);
	break;
    case TSK_ID_FLTCON:
	stat = TSK_FLTCON_STAT(gfcd_work);
	break;
    case TSK_ID_CHGDIR:
	stat = TSK_CHGDIR_STAT(gfcd_work);
	break;
    }
    return (stat == GFCD_TSK_READY)? TRUE: FALSE;
}


/* �^�X�N���s */
GFS_LOCAL Sint32	gfcd_doTask(Sint32 tsk, Sint32 *ncmd)
{
    switch (tsk) {
    case TSK_ID_SETFLT:
	return gfcd_doSetFiltTsk(ncmd);
    case TSK_ID_SETCON:
	return gfcd_doSetConTsk(ncmd);
    case TSK_ID_GETLEN:
	return gfcd_doGetLenDataTsk(ncmd);
    case TSK_ID_REQDAT:
	return gfcd_doReqDataTsk(ncmd);
    case TSK_ID_DELSCT:
	return gfcd_doDelSctDataTsk(ncmd);
    case TSK_ID_MOVSCT:
	return gfcd_doMoveSctTsk(ncmd);
    case TSK_ID_FLTCON:
	return gfcd_doSetFiltConTsk(ncmd);
    case TSK_ID_CHGDIR:
	return gfcd_doChgDirTsk(ncmd);
    }
}


/* �^�C���A�E�g�ݒ� */
GFS_LOCAL void GFCD_SetTimer(Sint32 tm_count)
{
    CDBMNG_TIMER(gfcd_work) = tm_count;
}


/* GFCD_ExecServer�̃^�X�N���I������̂�҂� */
Sint32 GFCD_WaitServer(void)
{
    Sint32 err;

    GFCD_SetTimer(GFCD_TMOUT_COUNT);
    for (;;) {
	err = GFCD_ExecServer();
	if (err != GFCD_SVR_BUSY) {
	    break;
	}
    }
    GFCD_SetTimer(GFCD_NO_TMOUT);
    return err;
}


/* SetFilt�^�X�N */
GFS_LOCAL Sint32	gfcd_doSetFiltTsk(Sint32 *ncmd)
{
    Sint32 i, j;
    Sint32 ret;

    for (i = 0; i < TSK_SETFLT_LEN(gfcd_work); ++i) {
	if (TSK_SETFLT_STAT(gfcd_work) == GFCD_TSK_STAT1) {
	    /* �i�胂�[�h�ݒ� */
	    ret = CDC_SetFiltMode(TSK_SETFLT_FLT(gfcd_work, i),
				  TSK_SETFLT_FMODE(gfcd_work, i));
	    if (ret != CDC_ERR_OK) {
		break;
	    }
	    gfcd_setStat();
	    ++*ncmd;
	    TSK_SETFLT_STAT(gfcd_work) = GFCD_TSK_STAT2;
	}
	if (TSK_SETFLT_STAT(gfcd_work) == GFCD_TSK_STAT2) {
	    /* �i������ݒ� */
	    ret = CDC_SetFiltSubh(TSK_SETFLT_FLT(gfcd_work, i),
				  &TSK_SETFLT_SUBH(gfcd_work, i));
	    if (ret != CDC_ERR_OK) {
		break;
	    }
	    gfcd_setStat();
	    ++*ncmd;
	    if (TSK_SETFLT_SNUM(gfcd_work, i) != 0) {
		TSK_SETFLT_STAT(gfcd_work) = GFCD_TSK_STAT3;
	    } else {
		TSK_SETFLT_STAT(gfcd_work) = GFCD_TSK_STAT1;
	    }
	}
	if (TSK_SETFLT_STAT(gfcd_work) == GFCD_TSK_STAT3) {
	    /* �i��FAD�͈͂̐ݒ� */
	    ret = CDC_SetFiltRange(TSK_SETFLT_FLT(gfcd_work, i),
				   TSK_SETFLT_FAD(gfcd_work, i),
				   TSK_SETFLT_SNUM(gfcd_work, i));
	    if (ret != CDC_ERR_OK) {
		break;
	    }
	    gfcd_setStat();
	    ++*ncmd;
	    TSK_SETFLT_STAT(gfcd_work) = GFCD_TSK_STAT1;
	}
    }
    /* �L���[���߂� */
    if (i >= TSK_SETFLT_LEN(gfcd_work)) {
	TSK_SETFLT_LEN(gfcd_work) = 0;
    } else {
	/* �܂��c�肪���� */
	for (j = 0; i < TSK_SETFLT_LEN(gfcd_work); ++i, ++j) {
	    TSK_SETFLT_QELT(gfcd_work, j) = TSK_SETFLT_QELT(gfcd_work, i);
	}
	TSK_SETFLT_LEN(gfcd_work) = j;
    }
    if (gfcd_checkEflag(CDC_HIRQ_ESEL)) {
	if (TSK_SETFLT_LEN(gfcd_work) == 0) {
	    TSK_SETFLT_STAT(gfcd_work) = GFCD_TSK_READY;
	}
    }
    return TSK_SETFLT_LEN(gfcd_work);
}


/* GFCD_SetCon�^�X�N */
GFS_LOCAL Sint32	gfcd_doSetConTsk(Sint32 *ncmd)
{
    Sint32 ret;

    if (TSK_SETCON_STAT(gfcd_work) == GFCD_TSK_STAT1) {
	ret = CDC_CdSetCon(TSK_SETCON_FLT(gfcd_work));
	++*ncmd;
	if (ret == CDC_ERR_OK) {
	    TSK_SETCON_STAT(gfcd_work) = GFCD_TSK_STAT2;
	}
	gfcd_setStat();
    }
    if (TSK_SETCON_STAT(gfcd_work) == GFCD_TSK_STAT2) {
	if (gfcd_checkEflag(CDC_HIRQ_ESEL)) {
	    TSK_SETCON_STAT(gfcd_work) = GFCD_TSK_READY;
	}
    }
    return TSK_SETCON_STAT(gfcd_work);
}


/* GFCD_GetLenData�^�X�N */
GFS_LOCAL Sint32 gfcd_doGetLenDataTsk(Sint32 *ncmd)
{
    Sint32	ret;
    Sint32	actwsiz, nsct, st;

    if (TSK_GETLEN_STAT(gfcd_work) == GFCD_TSK_STAT1) {
	ret = CDC_GetSctNum(TSK_GETLEN_BUFNO(gfcd_work), &nsct);
	gfcd_setStat();
	++*ncmd;
	nsct -= TSK_GETLEN_SPOS(gfcd_work);
	nsct =  MIN(nsct, TSK_GETLEN_USCT(gfcd_work));
	*TSK_GETLEN_NSCT(gfcd_work) = nsct;
	/* �o�C�g�������߂�K�v�����邩���ׂ� */
	if (TSK_GETLEN_NBYTE(gfcd_work) == NULL) {
	    TSK_GETLEN_STAT(gfcd_work) = GFCD_TSK_READY;
	    return GFCD_TSK_READY;
	}
	/* �Z�N�^�����O�Ȃ�o�C�g�������߂�K�v�͂Ȃ� */
	if (nsct <= 0) {
	    TSK_GETLEN_STAT(gfcd_work) = GFCD_TSK_READY;
	    *TSK_GETLEN_NBYTE(gfcd_work) = 0;
	    return GFCD_TSK_READY;
	}
	TSK_GETLEN_STAT(gfcd_work) = GFCD_TSK_STAT2;
    }
    if (TSK_GETLEN_STAT(gfcd_work) ==  GFCD_TSK_STAT2) {
	if (CDBMNG_TRBUFNO(gfcd_work) != TRREG_UNUSED) {
	    return GFCD_TSK_STAT2;
	}
	CDBMNG_TRBUFNO(gfcd_work) = TSK_GETLEN_BUFNO(gfcd_work);
	ret = CDC_CalActSiz(TSK_GETLEN_BUFNO(gfcd_work),
			    TSK_GETLEN_SPOS(gfcd_work),
			    *TSK_GETLEN_NSCT(gfcd_work));
	if (ret != CDC_ERR_OK) {
	    return GFCD_TSK_STAT2;
	}
	gfcd_setStat();
	++*ncmd;
	TSK_GETLEN_STAT(gfcd_work) = GFCD_TSK_STAT3;
    }
    if (TSK_GETLEN_STAT(gfcd_work) == GFCD_TSK_STAT3) {
	ret = CDC_GetActSiz(&actwsiz);
	if (ret != CDC_ERR_OK) {
	    return GFCD_TSK_STAT1;
	}
	gfcd_setStat();
	++*ncmd;
	TSK_GETLEN_STAT(gfcd_work) = GFCD_TSK_READY;
	*TSK_GETLEN_NBYTE(gfcd_work) =  GFS_WORD_BYTE(actwsiz);
	CDBMNG_TRBUFNO(gfcd_work) = TRREG_UNUSED;
	return GFCD_TSK_READY;
    }
    return GFCD_TSK_STAT1;
}


/* GFCD_ReqData�^�X�N */
GFS_LOCAL Sint32 gfcd_doReqDataTsk(Sint32 *ncmd)
{
    Sint32 ret;

    if (TSK_REQDAT_STAT(gfcd_work) == GFCD_TSK_STAT1) {
	ret = CDC_GetSctData(TSK_REQDAT_BUFNO(gfcd_work),
			     TSK_REQDAT_SCTPOS(gfcd_work),
			     TSK_REQDAT_NSCT(gfcd_work));
	if (ret != CDC_ERR_OK) {
	    return GFCD_TSK_STAT1;
	}
	gfcd_setStat();
	++*ncmd;
	TSK_REQDAT_STAT(gfcd_work) = GFCD_TSK_STAT2;
    }
    if (TSK_REQDAT_STAT(gfcd_work) == GFCD_TSK_STAT2) {
	ret = CDC_DataReady(CDC_DRDY_GET); /* �]���\�`�F�b�N */
	if (ret == CDC_ERR_OK) {
	    TSK_REQDAT_STAT(gfcd_work) = GFCD_TSK_READY;
	    return GFCD_TSK_READY;
	}
    }
    return GFCD_TSK_STAT1;
}


/* GFCD_DelSctData�^�X�N */
GFS_LOCAL Sint32 gfcd_doDelSctDataTsk(Sint32 *ncmd)
{
    Sint32 ret;
    Sint32 nsct;
    Bool tsk_stat;			/* �^�X�N�̎��s��� */

    tsk_stat = TRUE;		/* �^�X�N�͎��s�� */
    if (TSK_DELSCT_STAT(gfcd_work) == GFCD_TSK_STAT1) {
	if (TSK_DELSCT_SCTPOS(gfcd_work) == 0 &&
	    TSK_DELSCT_NSCT(gfcd_work) == CDC_SNUM_END) {
	    /* ���̑S�f�[�^���Ώۂ̎� */
	    ret = CDC_ResetSelector(0, TSK_DELSCT_BUFNO(gfcd_work));
	    if (ret != CDC_ERR_OK) {
		return GFCD_TSK_STAT1;
	    }
	    gfcd_setStat();
	    ++*ncmd;
	    TSK_DELSCT_STAT(gfcd_work) = GFCD_TSK_STAT4;
	} else {
	    nsct = TSK_DELSCT_NSCT(gfcd_work);
	    if (nsct == 0) {
		/* �����Z�N�^�����O�̎� */
		tsk_stat = FALSE;
	    } else {
		TSK_DELSCT_STAT(gfcd_work) = GFCD_TSK_STAT2;
	    }
	}
    }
    if (TSK_DELSCT_STAT(gfcd_work) ==GFCD_TSK_STAT2) {
	ret = CDC_DelSctData(TSK_DELSCT_BUFNO(gfcd_work),
			     TSK_DELSCT_SCTPOS(gfcd_work),
			     TSK_DELSCT_NSCT(gfcd_work));
	++*ncmd;
	if (ret == CDC_ERR_OK) {
	    TSK_DELSCT_STAT(gfcd_work) = GFCD_TSK_STAT3;
	    gfcd_setStat();
	}
    }
    if (TSK_DELSCT_STAT(gfcd_work) == GFCD_TSK_STAT3) {
	/* DelSct�̏I�����`�F�b�N */
	if (gfcd_checkEflag(CDC_HIRQ_EHST)) {
	    tsk_stat = FALSE;
	}
    }
    if (TSK_DELSCT_STAT(gfcd_work) == GFCD_TSK_STAT4) {
	/* ResetSelector�̏I���`�F�b�N */
	if (gfcd_checkEflag(CDC_HIRQ_ESEL)) {
	    tsk_stat = FALSE;
	}
    }
    if (tsk_stat == TRUE) {
	return GFCD_TSK_STAT1;
    }
    CDC_ClrHirqReq(~CDC_HIRQ_BFUL);		/* BFUL�N���A */
    TSK_DELSCT_STAT(gfcd_work) = GFCD_TSK_READY;
    return GFCD_TSK_READY;
}


/* GFCD_MoveSctData�^�X�N */
GFS_LOCAL Sint32 gfcd_doMoveSctTsk(Sint32 *ncmd)
{
    Sint32 ret;

    if (TSK_MOVSCT_STAT(gfcd_work) == GFCD_TSK_STAT1) {
	/* ���݂̍i�������ޔ�	*/
	ret = CDC_GetFiltMode(TSK_MOVSCT_DST(gfcd_work),
			      &TSK_MOVSCT_FMODE(gfcd_work));
	++*ncmd;
	gfcd_setStat();
	/* �i�������S�Ė����ɂ��� */
	ret = CDC_SetFiltMode(TSK_MOVSCT_DST(gfcd_work), 0);
	if (ret != CDC_ERR_OK) {
	    return GFCD_TSK_STAT1;
	}
	gfcd_setStat();
	++*ncmd;
	TSK_MOVSCT_STAT(gfcd_work) = GFCD_TSK_STAT2;
    }
    if (TSK_MOVSCT_STAT(gfcd_work) == GFCD_TSK_STAT2) {
	/* �Z�N�^�f�[�^���ړ����� */
	ret = CDC_MoveSctData(TSK_MOVSCT_SRC(gfcd_work),
			      TSK_MOVSCT_SPOS(gfcd_work),
			      TSK_MOVSCT_SNUM(gfcd_work),
			      TSK_MOVSCT_DST(gfcd_work));
	if (ret != CDC_ERR_OK) {
	    return GFCD_TSK_STAT1;
	}
	gfcd_setStat();
	++*ncmd;
	TSK_MOVSCT_STAT(gfcd_work) = GFCD_TSK_STAT3;
    }
    if (TSK_MOVSCT_STAT(gfcd_work) == GFCD_TSK_STAT3) {
	/* �R�s�[�����̏I���`�F�b�N */
	if (gfcd_checkEflag(CDC_HIRQ_ECPY)) {
	    TSK_MOVSCT_STAT(gfcd_work) = GFCD_TSK_STAT4;
	}
    }
    if (TSK_MOVSCT_STAT(gfcd_work) == GFCD_TSK_STAT4) {
	/* �i������𕜋A���� */
	ret = CDC_SetFiltMode(TSK_MOVSCT_DST(gfcd_work),
			      TSK_MOVSCT_FMODE(gfcd_work));
	if (ret != CDC_ERR_OK) {
	    return GFCD_TSK_STAT1;
	}
	++*ncmd;
	TSK_MOVSCT_STAT(gfcd_work) = GFCD_TSK_STAT5;
    }
    if (TSK_MOVSCT_STAT(gfcd_work) == GFCD_TSK_STAT5) {
	if (gfcd_checkEflag(CDC_HIRQ_ESEL) == TRUE) {
	    TSK_MOVSCT_STAT(gfcd_work) = GFCD_TSK_READY;
	}
    }
    return TSK_MOVSCT_STAT(gfcd_work);
}


/* ChgDir�^�X�N */
GFS_LOCAL Sint32 gfcd_doChgDirTsk(Sint32 *ncmd)
{
    Sint32 base_fid, infnum, ret;
    Bool drend;
    Uint16 hirq;
    CdcStat stat;

    if (TSK_CHGDIR_STAT(gfcd_work) == GFCD_TSK_STAT1) {
	/* fid�͈̔̓`�F�b�N��� */
	ret = CDC_GetFileScope(&base_fid, &infnum, &drend);
	if (ret != CDC_ERR_OK) {
	    TSK_CHGDIR_STAT(gfcd_work) = GFCD_TSK_READY;
	    *TSK_CHGDIR_NDIR(gfcd_work) = GFS_ERR_CDRD;
	    return GFCD_ERR_CDBFS;	/* �G���[ */
	}
	++*ncmd;
	/* fid���L���͈͂ɂ͂����Ă��邩 */
	if (TSK_CHGDIR_FID(gfcd_work) == 0 ||
	    TSK_CHGDIR_FID(gfcd_work) == 1 ||
	    (base_fid <= TSK_CHGDIR_FID(gfcd_work) &&
	     TSK_CHGDIR_FID(gfcd_work) < base_fid + infnum)) {
	    TSK_CHGDIR_STAT(gfcd_work) = GFCD_TSK_STAT3;
	} else {
	    ret = CDC_ReadDir(GFCD_SYS_SEL, TSK_CHGDIR_FID(gfcd_work));
	    if (ret != CDC_ERR_OK) {
		TSK_CHGDIR_STAT(gfcd_work) = GFCD_TSK_READY;
		*TSK_CHGDIR_NDIR(gfcd_work) = GFS_ERR_CDRD;
		return GFCD_ERR_CDBFS;	/* �G���[ */
	    }
	    ++*ncmd;
	    TSK_CHGDIR_STAT(gfcd_work) = GFCD_TSK_STAT2;
	}
    }
    if (TSK_CHGDIR_STAT(gfcd_work) == GFCD_TSK_STAT2) {
	/* ReadDir�I���҂� */
	if (gfcd_checkEflag(CDC_HIRQ_EFLS)) {
	    ret = CDC_GetFileScope(&base_fid, &infnum, &drend);
	    if (ret != CDC_ERR_OK ||
		(TSK_CHGDIR_FID(gfcd_work) < base_fid &&
		 base_fid + infnum <= TSK_CHGDIR_FID(gfcd_work))) {
		TSK_CHGDIR_STAT(gfcd_work) = GFCD_TSK_READY;
		*TSK_CHGDIR_NDIR(gfcd_work) = GFS_ERR_CDRD;
		return GFCD_ERR_CDBFS;
	    }
	    TSK_CHGDIR_STAT(gfcd_work) = GFCD_TSK_STAT3;
	}
    }
    if (TSK_CHGDIR_STAT(gfcd_work) == GFCD_TSK_STAT3) {
	hirq = GFCD_GetStat(&stat);
	if (hirq & CDC_HIRQ_DCHG) {
	    return GFCD_ERR_OPEN;
	}
	if ((CDC_STAT_STATUS(&stat) & 0x0f) == CDC_ST_NODISC) {
	    return GFCD_ERR_NODISC;
	}
	ret = CDC_ChgDir(TSK_CHGDIR_WORK(gfcd_work),
			 TSK_CHGDIR_FID(gfcd_work));
	if (ret != CDC_ERR_OK) {
	    return GFCD_TSK_STAT1;
	}
	gfcd_setStat();
	++*ncmd;
	TSK_CHGDIR_STAT(gfcd_work) = GFCD_TSK_STAT4;
    }
    if (TSK_CHGDIR_STAT(gfcd_work) == GFCD_TSK_STAT4) {
	/* �I�����`�F�b�N */
	if (gfcd_checkEflag(CDC_HIRQ_EFLS)) {
	    TSK_CHGDIR_STAT(gfcd_work) = GFCD_TSK_READY;
	    ret = CDC_GetFileScope(&base_fid, &infnum, &drend);
	    if (ret != CDC_ERR_OK) {
		TSK_CHGDIR_STAT(gfcd_work) = GFCD_TSK_READY;
		*TSK_CHGDIR_NDIR(gfcd_work) = GFS_ERR_CDRD;
		return GFCD_ERR_CDBFS;	/* �G���[ */
	    }
	    *TSK_CHGDIR_NDIR(gfcd_work) = infnum;
	}
    }
    return TSK_CHGDIR_STAT(gfcd_work);
}


/* SetFiltCon�^�X�N */
GFS_LOCAL Sint32 gfcd_doSetFiltConTsk(Sint32 *ncmd)
{
    Sint32 ret;
    Sint32 i, j;
    Sint32 flag, flout, flnout;

    for (i = 0; i < TSK_FLTCON_LEN(gfcd_work); ++i) {
	/* �t���O�쐬 */
	if (TSK_FLTCON_BUF(gfcd_work, i) == GFCD_ANY_FLT) {
	    flag = 0;
	    flout = CDC_NUL_SEL;
	} else {
	    flag = FLTCON_BUF_SET;
	    flout = TSK_FLTCON_BUF(gfcd_work, i);
	}
	if (TSK_FLTCON_FLNOUT(gfcd_work, i) == GFCD_ANY_FLT) {
	    flnout = CDC_NUL_SEL;
	} else {
	    flag |= FLTCON_FLN_SET;
	    flnout = TSK_FLTCON_FLNOUT(gfcd_work, i);
	}
	/* �R�}���h���s */
	ret = CDC_SetFiltCon(TSK_FLTCON_FLT(gfcd_work, i), flag, flout,
			     flnout);
	if (ret != CDC_ERR_OK) {
	    break;
	}
	gfcd_setStat();
	++*ncmd;
    }
    /* �L���[���߂� */
    if (i >= TSK_FLTCON_LEN(gfcd_work)) {
	TSK_FLTCON_LEN(gfcd_work) = 0;
    } else {
	/* �܂��c�肪���� */
	for (j = 0; i < TSK_FLTCON_LEN(gfcd_work); ++i, ++j) {
	    TSK_FLTCON_FLT(gfcd_work, j) = TSK_FLTCON_FLT(gfcd_work, i);
	    TSK_FLTCON_BUF(gfcd_work, j) = TSK_FLTCON_BUF(gfcd_work, i);
	    TSK_FLTCON_FLNOUT(gfcd_work, j) = TSK_FLTCON_FLNOUT(gfcd_work, i);
	}
	TSK_FLTCON_LEN(gfcd_work) = j;
    }
    if (gfcd_checkEflag(CDC_HIRQ_ESEL)) {
	if (TSK_FLTCON_LEN(gfcd_work) == 0) {
	    TSK_FLTCON_STAT(gfcd_work) = GFCD_TSK_READY;
	}
    }
    return TSK_FLTCON_LEN(gfcd_work);
}


/* �I���t���O�𒲂ׂ� */
GFS_LOCAL Bool gfcd_checkEflag(Uint16 mask)
{
    Uint16 hirq;

    hirq = CDC_GetHirqReq();
    return (mask & hirq)? TRUE: FALSE;
}


/* CD�u���b�N�Ǘ��\���̏����� */
GFS_LOCAL void gfcd_initCdbMng(void)
{
    Sint32 i;

    /* �e�����𖢎g�p�ɏ����� */
    /*  CDBFS�p�Z���N�^�͏��������Ȃ� */
    /*  GFCD_Init���ɂ͂��̌㏉�������A�^�C���A�E�g���ɂ͕ύX���Ȃ� */
    for (i = 0; i < GFS_OPEN_MAX - 1; ++i) {
	CDBMNG_USEBUF(gfcd_work, i) = UNUSED;
	CDBMNG_USEFILT(gfcd_work, i) = UNUSED;
    }
    CDBMNG_USEPU(gfcd_work) = UNUSED;
    CDBMNG_TRBUFNO(gfcd_work) = TRREG_UNUSED;
    CDBMNG_PUID(gfcd_work) = NOBODY;
    CDBMNG_TIMER(gfcd_work) = GFCD_NO_TMOUT;
    CDBMNG_FUNC(gfcd_work) = GFCD_NULLFUNC;
    CDBMNG_OBJ(gfcd_work) = NULL;
    /* �e�^�X�N�������� */
    TSK_SETFLT_LEN(gfcd_work) = 0;
    TSK_SETFLT_STAT(gfcd_work) = GFCD_TSK_READY;
    TSK_SETCON_STAT(gfcd_work) = GFCD_TSK_READY;
    TSK_GETLEN_STAT(gfcd_work) = GFCD_TSK_READY;
    TSK_REQDAT_STAT(gfcd_work) = GFCD_TSK_READY;
    TSK_DELSCT_STAT(gfcd_work) = GFCD_TSK_READY;
    TSK_MOVSCT_STAT(gfcd_work) = GFCD_TSK_READY;
    TSK_CHGDIR_STAT(gfcd_work) = GFCD_TSK_READY;
    TSK_FLTCON_LEN(gfcd_work) = 0;
    TSK_FLTCON_STAT(gfcd_work) = GFCD_TSK_READY;
}


/* �X�e�[�^�X���X�V���� */
GFS_LOCAL void gfcd_setStat(void)
{
    CdcStat	stat;

    CDC_GetLastStat(&stat);
    CDC_STAT_STATUS(&CDBMNG_STAT(gfcd_work)) = CDC_STAT_STATUS(&stat);
}


/* CSCT�r�b�g���P�ɂȂ�����ĂԊ֐���o�^ */
void GFCD_SetCsctFunc(void (*func)(void *), void *obj)
{
    CDBMNG_FUNC(gfcd_work) = func;
    CDBMNG_OBJ(gfcd_work) = obj;
}


/* SCDQ�t���O�����̂�҂� */
/* �^�C���A�E�g������TRUE��Ԃ� */
Bool GFCD_WaitScdqFlag(void)
{
    Sint32 timer;

    for (timer = 0; timer < SCDQ_TMOUT_COUNT; ++timer) {
	if (CDC_GetHirqReq() & CDC_HIRQ_SCDQ) {
	    CDC_ClrHirqReq(~CDC_HIRQ_SCDQ);
	    return FALSE;
	}
    }
    return TRUE;
}

/* end of file */
