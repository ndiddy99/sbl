/*****************************************************************************
 *	�\�t�g�E�F�A���C�u����
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:�t�@�C���V�X�e��
 * Module :�]��
 * File	  :gfs_trn.c
 * Date   :1994-11-11
 * Version:1.16
 * Auther :M.S.
 *
 *****************************************************************************/

/* SCU�]�����g�p����Ȃ�#define���� */
#define USE_SCU_TRANS

/*****************************************************************************
 *	�C���N���[�h�t�@�C��
 *****************************************************************************/
#include "sega_gfs.h"
#include <sega_dma.h>
#include <sega_csh.h>
#include <sega_int.h>
#include "gfs_def.h"
#include "gfs_trn.h"
#include "gfs_buf.h"

/*****************************************************************************
 *	�萔�}�N��
 *****************************************************************************/
#define GFTR_TMAX_DFL	1		/*  �]���ő�Z�N�^���̏����l  */

#define GFTR_TMODE_DFL	GFS_TMODE_CPU	/* �]�����[�h�̏����l */

/* WORKRAM-L�̃A�h���X�͈�		*/
#define WORKRAM_L_ADR_START	0x00200000
#define WORKRAM_L_ADR_END	0x00300000

/* A-Bus�̃A�h���X�͈�			*/
#define ABUS_ADR_START		0x02000000
#define ABUS_ADR_END		0x05900000

/* B-Bus�̃A�h���X�͈�	from dma_scu1.c	*/
#define BBUS_ADR_START  	0x05a00000
#define BBUS_ADR_END    	0x05fe0000

/* �]�����f���^�C���A�E�g�܂ł̃��[�v�� */
#define STOP_TMOUT_COUNT	0x600000

/*****************************************************************************
 *	�񋓒萔
 *****************************************************************************/

/*****************************************************************************
 *	�����}�N��
 *****************************************************************************/
/* adr��cache address�ɕϊ� */
#define TO_CADR(adr)	(((Uint32)adr) & 0x0fffffff)

/* adr��WORKRAM-L��? */
#define IN_WORKRAM_L(adr)	\
    ((WORKRAM_L_ADR_START <= TO_CADR(adr))&&(TO_CADR(adr) < WORKRAM_L_ADR_END))

/* adr��A-Bus��? */
#define IN_ABUS(adr)		\
    ((ABUS_ADR_START <= TO_CADR(adr))&&(TO_CADR(adr) < ABUS_ADR_END))

/* adr��B-Bus��? */
#define IN_BBUS(adr)		\
    ((BBUS_ADR_START <= TO_CADR(adr))&&(TO_CADR(adr) < BBUS_ADR_END))


/*****************************************************************************
 *	�f�[�^�^�̒�`
 *****************************************************************************/

/* CPU-DMA�̒[������	*/
typedef	struct {
    Sint32	nbyte;
    Uint32	*src;
    Uint32	*dst;
} SdmaFrac;

/* CPU-DMA�̒[������(SdmaFrac)�̃A�N�Z�X�}�N��	*/
#define	SDMA_FRAC_NBYTE(sf)	((sf)->nbyte)
#define	SDMA_FRAC_SRC(sf)	((sf)->src)
#define	SDMA_FRAC_DST(sf)	((sf)->dst)


/* �჌�x���]���֐��\���� */
typedef struct {
    Bool   (*req)(void);		/* �]�����s�̗v���֐� */
    void   (*exec)(void *dst, Sint32 dmode,
		   void *src, Sint32 smode, Sint32 nbyte);
					/* �]���̎��s�֐� */
    Sint32 (*rem)(void);		/* �c��̓]���o�C�g���̎擾�֐�  */
} CopyFunc;

/* �჌�x���]���֐�(CopyFunc)�̃A�N�Z�X�}�N�� */
#define COPY_FUNC_REQ(tmode)	(gftr_cfunc[tmode].req)
#define COPY_FUNC_EXEC(tmode)	(gftr_cfunc[tmode].exec)
#define COPY_FUNC_REM(tmode)	(gftr_cfunc[tmode].rem)


/*****************************************************************************
 *	�ϐ��̐錾
 *****************************************************************************/

/*****************************************************************************
 *	�֐��̐錾
 *****************************************************************************/

/* �]���̎��s�i�������j*/
GFS_LOCAL Sint32 gftr_execSub(GfsHn gfs, Bool *flag);

/* �\�[�X�f�[�^�p�b�N�̎擾 */
GFS_LOCAL void gftr_getSrcPack(GfsHn gfs);

/* �f�X�e�B�l�[�V�����f�[�^�p�b�N�̎擾 */
GFS_LOCAL void gftr_getDstPack(GftrHn gftr);

/* �r�b�t�ɂ��]���ɑ΂���v��  */
GFS_LOCAL Bool gftr_reqScu(void);

/* �r�b�t�ɂ��]���̎��s  */
GFS_LOCAL void gftr_execScu(void *dst, Sint32 dmode, void *src, Sint32 smode,
			 Sint32 nbyte);

/* �r�b�t�ɂ��]���̎c��o�C�g��  */
GFS_LOCAL Sint32 gftr_remScu(void);

/* DMA���g�p�\���H */
GFS_LOCAL Bool gfdma_canUse(Sint32 ch);

/* DMA�]���J�n */
GFS_LOCAL void gfdma_exec(Sint32 ch, void *dst, Sint32 dmode,
		       void *src, Sint32 smode, Sint32 nbyte);

/* DMA �̏�Ԃ��擾���� */
GFS_LOCAL Sint32 gfdma_getBytecnt(Sint32 ch);

/* �c�l�`�O�ɂ��]���ɑ΂���v�� */
GFS_LOCAL Bool gftr_reqDma0(void);

/* �c�l�`�O�ɂ��]���̎��s */
GFS_LOCAL void gftr_execSdma0(void *dst, Sint32 dmode, void *src, Sint32 smode,
			   Sint32 nbyte);

/* �c�l�`�O�ɂ��]���̎c��o�C�g�� */
GFS_LOCAL Sint32 gftr_remDma0(void);

/* �c�l�`�P�ɂ��]���ɑ΂���v�� */
GFS_LOCAL Sint32 gftr_remDma1(void);

/* �c�l�`�O�ɂ��]���̎��s */
GFS_LOCAL void gftr_execSdma1(void *dst, Sint32 dmode, void *src, Sint32 smode,
			   Sint32 nbyte);

/* �c�l�`�P�ɂ��]���̎c��o�C�g�� */
GFS_LOCAL Bool gftr_reqDma1(void);

/* �b�o�t�]���ɑ΂���v��  */
GFS_LOCAL Bool gftr_reqCpu(void);

/* �b�o�t�]���̎��s  */
GFS_LOCAL void gftr_execCpu(void *dst, Sint32 dmode, void *src, Sint32 smode,
			 Sint32 nbyte);

/* �b�o�t�]���̎c��o�C�g��  */
GFS_LOCAL Sint32 gftr_remCpu(void);

/* �]�����s�̗v�� */
GFS_LOCAL Bool gftr_reqTrn(GftrHn gftr);

/* �]���̎��s  */
GFS_LOCAL Bool gftr_execTrn(GfsHn gfs);

/* �c��]���o�C�g���̎擾  */
GFS_LOCAL Sint32 gftr_remTrn(GftrHn gftr);

/* �S�o�C�g�ɖ����Ȃ������̓]�� */
GFS_LOCAL void gftr_copyFrac(Sint32 n, Uint32 *dp, Uint32 *sp);

/*****************************************************************************
 *	�ϐ��̒�`
 *****************************************************************************/
/* �჌�x���]���֐��e�[�u��  */
static const CopyFunc gftr_cfunc[] = {
    {gftr_reqScu, gftr_execScu, gftr_remScu},
    {gftr_reqDma0, gftr_execSdma0, gftr_remDma0},
    {gftr_reqDma1, gftr_execSdma1, gftr_remDma1},
    {gftr_reqCpu, gftr_execCpu, gftr_remCpu}
};


/* CPU-DMA�̒[������		*/
static SdmaFrac	gftr_sdmaFrac;

/*****************************************************************************
 *	�֐��̒�`
 *****************************************************************************/
/* �]����̐ݒ� */
GftrHn  GFTR_Setup(GftrHn gftr)
{
    GFS_TRN_BUF(gftr) = NULL;
    GFS_TRN_SIZE(gftr) = 0;
    GFS_TRN_WP(gftr) = 0;
    GFS_TRN_DPS(gftr) = NULL;
    GFS_TRN_DPD(gftr) = NULL;
    GFS_TRN_TSCTMAX(gftr) = GFTR_TMAX_DFL;
    GFS_TRN_TSCT(gftr) = 0;
    GFS_TRN_TSCTCNT(gftr) = 0;
    GFS_TRN_TBYTCNT(gftr) = 0;
    GFTR_SetUnit(gftr, 1);
    GFS_TRN_STAT(gftr) = GFTR_ST_NOACT;
    GFTR_SetMode(gftr, GFTR_TMODE_DFL);
    return gftr;
}


/* �]�����[�h�̐ݒ� */
Sint32 GFTR_SetMode(GftrHn gftr, Sint32 tmode)
{
    Sint32 old_mode = GFS_TRN_MODE(gftr);

    GFS_TRN_MODE(gftr) = tmode;
    GFS_TRN_OBJ(gftr) = NULL;
    GFS_TRN_TFUNC(gftr) = GFTR_NULLFUNC;
    return old_mode;
}


/* �]���P�ʂ̐ݒ� */
void GFTR_SetUnit(GftrHn gftr, Sint32 unit)
{
    GFS_TRN_UNIT(gftr) = unit;
}


/* �]���Z�N�^���̐ݒ� */
void GFTR_SetTransPara(GftrHn gftr, Sint32 tsct)
{
    GFS_TRN_TSCT(gftr) = tsct;
    GFS_TRN_TSCTCNT(gftr) = 0;
    if (GFS_TRN_STAT(gftr) == GFTR_ST_END) {
	GFS_TRN_STAT(gftr) = GFTR_ST_NOACT;
    }
}


/* �S�]���Z�N�^���̎擾 */
Sint32 GFTR_GetTransPara(GftrHn gftr)
{
    return GFS_TRN_TSCT(gftr);
}


/* ���݂܂łɓ]�����ꂽ�Z�N�^���̎擾 */
Sint32 GFTR_GetNumTrans(GftrHn gftr)
{
    return GFS_TRN_TSCTCNT(gftr);
}


/* �P��̓]���ɂ�����ő�]���Z�N�^���̐ݒ� */
Sint32 GFTR_SetTransMax(GftrHn gftr, Sint32 tmax)
{
    Sint32 old_para = GFS_TRN_TSCTMAX(gftr);

    GFS_TRN_TSCTMAX(gftr) = tmax;
    return old_para;
}


/* �]���J�n */
void GFTR_Start(GftrHn gftr)
{
    GFS_TRN_ACTIVE(gftr) = TRUE;
}


/* �]���ꎞ��~ */
void GFTR_Pause(GftrHn gftr)
{
    GFS_TRN_ACTIVE(gftr) = FALSE;
}


/* �]�����~ */
void GFTR_Stop(GfsHn gfs, Sint32 nsct)
{
    Bool	flag;
    GfsTrans	*trn = &GFS_FILE_TRANS(gfs);

    MNG_TIMER(gfs_mng_ptr) = 0;
    while (GFS_TRN_STAT(trn) != GFTR_ST_NOACT &&
	   GFS_TRN_STAT(trn) != GFTR_ST_END &&
	   GFS_TRN_STAT(trn) != GFTR_ST_ERROR) {
	gftr_execSub(gfs, &flag);
	if (++MNG_TIMER(gfs_mng_ptr) > STOP_TMOUT_COUNT) {
	    break;
	}
    }
    if (GFS_TRN_DPS(trn) != NULL) {
	GFBF_RtnPk(gfs, GFS_TRN_DPS(trn), nsct);
	GFS_TRN_TSCTCNT(trn) += GFDP_NSCT(GFS_TRN_DPS(trn));
	GFS_TRN_DPS(trn) = NULL;
    }
    if (GFS_TRN_DPD(trn) != NULL) {
	GFTR_RtnDstPk(trn, GFS_TRN_DPD(trn));
	GFS_TRN_DPD(trn) = NULL;
    }
    GFS_TRN_STAT(trn) = GFTR_ST_END;
}


/*****************************************************************************
 *  �]���̎��s
 *�m���́n
 *  gfs
 *�m�o�́n
 *  flag	�]�����s�Ȃ�ꂽ��TRUE
 *�m�֐��l�n
 *  �]�����
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFTR_Exec(GfsHn gfs, Bool *flag)
{
    Sint32 i;
    Sint32 cntnum;
    Sint32 stat;
    GftrHn gftr;
    Sint32 tmode;

    /* �]���񐔂����߂� */
    gftr = &GFS_FILE_TRANS(gfs);
    tmode = GFS_TRN_MODE(gftr);
    switch (tmode) {
    case GFS_TMODE_SCU:
    case GFS_TMODE_SDMA0:
    case GFS_TMODE_SDMA1:
	if (GFS_TRN_STAT(gftr) != GFTR_ST_NOACT) {
	    /* �r���̏�Ԃ���n�܂鎞�̓X�L�b�v�����O�����s�Ȃ� */
	    cntnum = 2;
	} else {
	    cntnum = 1;
	}	    
	break;
    case GFS_TMODE_CPU:
    case GFS_TMODE_STM:
    default:
	cntnum = 1;
	break;
    }

    /* �]�������s���� */
    for (i = 0; i < cntnum; ++i) {
	stat = gftr_execSub(gfs, flag);
	if (stat == GFTR_ST_ERROR ||
	    (stat != GFTR_ST_NOACT && stat != GFTR_ST_END)) {
	    break;
	}
    }
    return stat;
}


/*****************************************************************************
 *  �z�X�g���o�b�t�@�t����̋󑗂�]��
 *�m���́n
 *  gfs
 *�m�o�́n
 *
 *�m�֐��l�n
 *  �]�����
 *�m�@�\�n
 *  �z�X�g���o�b�t�@���t���ɂȂ�����A�]���Z�N�^���ɒB����܂ł̃f�[�^���̂Ă�
 *�m���l�n
 *  
 *****************************************************************************/
Sint32 GFTR_Idle(GfsHn gfs)
{
    Sint32 tsct, tlen;
    GfsTrans	*trn = &GFS_FILE_TRANS(gfs);

    if (GFS_TRN_TSCT(trn) <= 0) {
	return GFTR_ST_END;
    }
    if (!GFS_TRN_ACTIVE(trn)) {
	return GFS_TRN_STAT(trn);
    }
    switch (GFS_TRN_STAT(trn)) {
    case GFTR_ST_NOACT:
	gftr_getSrcPack(gfs);
	if (GFS_TRN_DPS(trn) == NULL) {
	    break;
	}
	GFS_TRN_STAT(trn) = GFTR_ST_STAT5;
	/* ���̏�Ԃɑ��� */
    case GFTR_ST_STAT5:
	GFBF_RtnPk(gfs, GFS_TRN_DPS(trn), GFTR_ALLSCT);
	GFS_TRN_TSCTCNT(trn) += GFDP_NSCT(GFS_TRN_DPS(trn));
	GFS_TRN_DPS(trn) = NULL;
	if (GFS_TRN_TSCTCNT(trn) >= GFS_TRN_TSCT(trn) ) {
	    GFS_TRN_STAT(trn) = GFTR_ST_END;
	} else {
	    GFS_TRN_STAT(trn) = GFTR_ST_NOACT;
	}
    }
    return GFS_TRN_STAT(trn);
}


/*****************************************************************************
 *  �]���̎��s�i�������j
 *�m���́n
 *  gfs
 *�m�o�́n
 *  flag	�]�����s�Ȃ�ꂽ��TRUE
 *�m�֐��l�n
 *  �]�����
 *�m�@�\�n
 *  
 *�m���l�n
 *  
 *****************************************************************************/
GFS_LOCAL Sint32 gftr_execSub(GfsHn gfs, Bool *flag)
{
    Sint32 tsct, tlen;
    Bool fl = FALSE;
    GfsTrans	*trn = &GFS_FILE_TRANS(gfs);

    if (GFS_TRN_TSCT(trn) <= 0) {
	*flag = fl;
	return GFTR_ST_END;
    }
    if (!GFS_TRN_ACTIVE(trn)) {
	*flag = fl;
	return GFS_TRN_STAT(trn);
    }
    switch (GFS_TRN_STAT(trn)) {
    case GFTR_ST_NOACT:
	gftr_getSrcPack(gfs);
	if (GFS_TRN_DPS(trn) == NULL) {
	    break;
	}
	GFS_TRN_STAT(trn) = GFTR_ST_STAT1;
	/* ���̏�Ԃɑ��� */
    case GFTR_ST_STAT1:
	gftr_getDstPack(trn);
	if (GFS_TRN_DPD(trn) == NULL) {
	    break;
	}
	GFS_TRN_STAT(trn) = GFTR_ST_STAT2;
	/* ���̏�Ԃɑ��� */
    case GFTR_ST_STAT2:
	if (gftr_reqTrn(trn) == FALSE) {
	    break;
	}
	GFS_TRN_STAT(trn) = GFTR_ST_STAT3;
	/* ���̏�Ԃɑ��� */
    case GFTR_ST_STAT3:
	if (gftr_execTrn(gfs) == FALSE) {
	    break;
	}
	GFS_TRN_STAT(trn) = GFTR_ST_STAT4;
	/* ���̏�Ԃɑ��� */
    case GFTR_ST_STAT4:
	if (gftr_remTrn(trn) > 0) {
	    break;
	}
	fl = TRUE;
	GFS_TRN_STAT(trn) = GFTR_ST_STAT5;
	/* ���̏�Ԃɑ��� */
    case GFTR_ST_STAT5:
	if (GFS_TRN_TBYTCNT(trn) == GFDP_LEN(GFS_TRN_DPS(trn))) {
	    GFBF_RtnPk(gfs, GFS_TRN_DPS(trn), GFTR_ALLSCT);
	    GFS_TRN_TSCTCNT(trn) += GFDP_NSCT(GFS_TRN_DPS(trn));
	    GFS_TRN_DPS(trn) = NULL;
	}
	GFTR_RtnDstPk(trn, GFS_TRN_DPD(trn));
	GFS_TRN_DPD(trn) = NULL;
	if (GFS_TRN_TSCTCNT(trn) >= GFS_TRN_TSCT(trn) ) {
	    GFS_TRN_STAT(trn) = GFTR_ST_END;
	} else {
	    GFS_TRN_STAT(trn) = GFTR_ST_NOACT;
	}
    }
    *flag = fl;
    return GFS_TRN_STAT(trn);
}


/* �\�[�X�f�[�^�p�b�N�̎擾 */
GFS_LOCAL void gftr_getSrcPack(GfsHn gfs)
{
    Sint32	tsct, tsct2;
    GfsTrans	*trn = &GFS_FILE_TRANS(gfs);

    tsct = GFS_TRN_TSCT(trn) - GFS_TRN_TSCTCNT(trn);
    tsct = MIN(tsct, GFS_TRN_TSCTMAX(trn));
    if (GFS_TRN_UNIT(trn) > 1) {
	/* STM�̏ꍇ�����ł���A�K���Z�N�^�P�ʂł��� */
	if (GFS_TRN_DPS(trn) != NULL) {
	    tsct = (GFDP_LEN(GFS_TRN_DPS(trn)) - GFS_TRN_TBYTCNT(trn))
		/ GFS_TRN_UNIT(trn);
	}
	tsct2 = GFTR_GetLenRoom(trn) / GFS_TRN_UNIT(trn);
	tsct = MIN(tsct, tsct2);
    }
    if (GFS_TRN_DPS(trn) == NULL) {
	GFS_TRN_DPS(trn) = GFBF_GetSrcPk(gfs, tsct);
	if (GFS_TRN_DPS(trn) != NULL) {
	    GFS_TRN_TBYTCNT(trn) = 0;
	}
    }
}


/* �f�X�e�B�l�[�V�����f�[�^�p�b�N�̎擾 */
GFS_LOCAL void gftr_getDstPack(GftrHn gftr)
{
    Sint32 tlen;

    if (GFS_TRN_UNIT(gftr) > 1) {
	tlen = GFDP_LEN(GFS_TRN_DPS(gftr));
    } else {
	tlen = GFDP_LEN(GFS_TRN_DPS(gftr)) - GFS_TRN_TBYTCNT(gftr);
	tlen = MIN(tlen, GFTR_GetLenRoom(gftr));
    }
    GFS_TRN_DPD(gftr) = GFTR_GetDstPack(gftr, tlen);
}


/*---------------------------------------------------------------------------
 * SCU�]��
 *---------------------------------------------------------------------------*/

/* �r�b�t�ɂ��]���ɑ΂���v��  */
GFS_LOCAL Bool gftr_reqScu(void)
{
#ifdef USE_SCU_TRANS
    return TRUE;
#else
    return gftr_reqCpu();
#endif
}


/* �r�b�t�ɂ��]���̎��s */
GFS_LOCAL void gftr_execScu(void *dst, Sint32 dmode, void *src, Sint32 smode,
			 Sint32 nbyte)
{
#if defined(USE_SCU_TRANS)
    DmaScuPrm prm;

    if (dst == NULL) {
	return;
    }
    prm.dxr = (Uint32)src;
    prm.dxw = (Uint32)dst;
    prm.dxc = nbyte;

    /* �]�����̃A�h���X���Z�l	*/
    if (smode == 0) {
	prm.dxad_r = DMA_SCU_R0;
    } else {
	prm.dxad_r = DMA_SCU_R4;
    }

    /* �]����̃A�h���X���Z�l	*/
    if (dmode == 0) {
	prm.dxad_w = DMA_SCU_W0;
    } else if (IN_BBUS(dst)) {
	prm.dxad_w = DMA_SCU_W2;
    } else {
	prm.dxad_w = DMA_SCU_W4;
    }
    prm.dxmod = DMA_SCU_DIR;
    /* �A�h���X�X�V�r�b�g�ƃA�h���X���Z�l�̐�����������邽�߁A */
    /* DMA_SCU_KEEP�Ƃ��� */
    prm.dxrup = DMA_SCU_KEEP;
    prm.dxwup = DMA_SCU_KEEP;
    prm.dxft = DMA_SCU_F_DMA;
    prm.msk = DMA_SCU_M_DXR | DMA_SCU_M_DXW;
    DMA_ScuSetPrm(&prm, DMA_SCU_CH0);
    DMA_ScuStart(DMA_SCU_CH0);
    /* �]����̃L���b�V�����p�[�W */
    CSH_Purge(dst, nbyte);
#else
    gftr_execCpu(dst, dmode, src, smode, nbyte);
#endif
}


/* �r�b�t�ɂ��]���̎c��o�C�g�� */
GFS_LOCAL Sint32 gftr_remScu(void)
{
#if defined(USE_SCU_TRANS)
    DmaScuStatus stat;

    DMA_ScuGetStatus(&stat, DMA_SCU_CH0);
    if (stat.dxmv == DMA_SCU_MV) {
	return 1;
    }
    return 0;
#else
    return gftr_remCpu();
#endif
}


/*---------------------------------------------------------------------------
 * DMA�]������
 *---------------------------------------------------------------------------*/
/* DMA���g�p�\���H */
Bool gfdma_canUse(Sint32 ch)
{
    return TRUE;
}


/* DMA�]���J�n */
GFS_LOCAL void gfdma_exec(Sint32 ch, void *dst, Sint32 dmode,
		void *src, Sint32 smode, Sint32 nbyte)
{
    Sint32 i, cnt, frac_buf;
    Uint8 *frac;
    DmaCpuComPrm com_prm;
    DmaCpuPrm prm;

    DMA_CpuStop(ch);
    com_prm.pr = DMA_CPU_FIX;
    com_prm.dme = DMA_CPU_ENA;
    com_prm.msk = DMA_CPU_M_PR | DMA_CPU_M_AE | DMA_CPU_M_NMIF |
	DMA_CPU_M_DME;
    DMA_CpuSetComPrm(&com_prm);
    prm.sar = (Uint32)src;
    prm.dar = (Uint32)dst;
    prm.tcr = nbyte / sizeof(Uint32);
    if (dmode > 0) {
	prm.dm = DMA_CPU_AM_ADD;
    } else if (dmode < 0) {
	prm.dm = DMA_CPU_AM_SUB;
    } else {
	prm.dm = DMA_CPU_AM_NOM;
    }
    if (smode > 0) {
	prm.sm = DMA_CPU_AM_ADD;
    } else if (smode < 0) {
	prm.sm = DMA_CPU_AM_SUB;
    } else {
	prm.sm = DMA_CPU_AM_NOM;
    }
    prm.ts = DMA_CPU_4;
    prm.ar = DMA_CPU_AUTO;
    prm.ie = DMA_CPU_INT_DIS;
    prm.msk = DMA_CPU_M_SAR |
	DMA_CPU_M_DAR |
	DMA_CPU_M_TCR |
	DMA_CPU_M_DM |
	DMA_CPU_M_SM |
	DMA_CPU_M_TS |
	DMA_CPU_M_AR |
	DMA_CPU_M_IE |
	DMA_CPU_M_TE;
    DMA_CpuSetPrm(&prm, ch);
    DMA_CpuStart(ch);
    /* �[�������̐ݒ� */
    cnt = nbyte / sizeof(Uint32);
    SDMA_FRAC_NBYTE(&gftr_sdmaFrac) = nbyte % sizeof(Uint32);
    SDMA_FRAC_DST(&gftr_sdmaFrac) = ((Uint32 *)dst) + cnt * dmode;
    SDMA_FRAC_SRC(&gftr_sdmaFrac) = ((Uint32 *)src) + cnt * smode;
    CSH_Purge(dst, nbyte);
}


/* DMA �̏�Ԃ��擾���� */
GFS_LOCAL Sint32 gfdma_getBytecnt(Sint32 ch)
{
    if (DMA_CpuGetStatus(ch) == DMA_CPU_TE_MV) {
	return 1;
    }
    /* �[������	*/
    gftr_copyFrac(SDMA_FRAC_NBYTE(&gftr_sdmaFrac),
		  SDMA_FRAC_DST(&gftr_sdmaFrac),
		  SDMA_FRAC_SRC(&gftr_sdmaFrac));
    return 0;
}


/*---------------------------------------------------------------------------
 * DMA�]��
 *---------------------------------------------------------------------------*/
/* �c�l�`�O�ɂ��]���ɑ΂���v�� */
GFS_LOCAL Bool gftr_reqDma0(void)
{
    return gfdma_canUse(0);
}


/* �c�l�`�O�ɂ��]���̎��s */
GFS_LOCAL void gftr_execSdma0(void *dst, Sint32 dmode, void *src, Sint32 smode,
			   Sint32 nbyte)
{
    gfdma_exec(0, dst, dmode, src, smode, nbyte);
}


/* �c�l�`�O�ɂ��]���̎c��o�C�g�� */
GFS_LOCAL Sint32 gftr_remDma0(void)
{
    return gfdma_getBytecnt(0);
}


/* �c�l�`�P�ɂ��]���ɑ΂���v�� */
GFS_LOCAL Bool gftr_reqDma1(void)
{
    return gfdma_canUse(1);
}


/* �c�l�`�P�ɂ��]���̎��s */
void gftr_execSdma1(void *dst, Sint32 dmode, void *src, Sint32 smode,
			   Sint32 nbyte)
{
    gfdma_exec(1, dst, dmode, src, smode, nbyte);
}


/* �c�l�`�P�ɂ��]���̎c��o�C�g�� */
GFS_LOCAL Sint32 gftr_remDma1(void)
{
    return gfdma_getBytecnt(1);
}


/*---------------------------------------------------------------------------
 * �\�t�g�E�F�A�]�����[�`��
 *---------------------------------------------------------------------------*/

/* �b�o�t�]���ɑ΂���v�� */
GFS_LOCAL Bool gftr_reqCpu(void)
{
    return TRUE;
}


/* �b�o�t�]���̎��s */
GFS_LOCAL void gftr_execCpu(void *dst, Sint32 dmode, void *src, Sint32 smode,
			 Sint32 nbyte)
{
    Uint32	*sp = src, *dp = dst;
    Sint32	i;

    if (dst == NULL) {
	return;
    }
    for (i = 0; i < nbyte / sizeof(Uint32); ++i) {
	*dp = *sp;
	dp += dmode;
	sp += smode;
    }
    gftr_copyFrac(nbyte % sizeof(Uint32), dp, sp);
}


/* �b�o�t�]���̎c��o�C�g�� */
GFS_LOCAL Sint32 gftr_remCpu(void)
{
    return 0;
}


/*---------------------------------------------------------------------------
 * �]���C���^�t�F�[�X 
 *---------------------------------------------------------------------------*/
/* �]�����s�̗v�� */
GFS_LOCAL Bool gftr_reqTrn(GftrHn gftr)
{
    Sint32 tmode = GFS_TRN_MODE(gftr);
    GfdpHn	dpd = GFS_TRN_DPD(gftr);
    Uint8	*dst = GFDP_DATA(dpd);

    if (tmode == GFS_TMODE_STM) {
	return TRUE;
    }
    if (IN_WORKRAM_L(dst) || IN_ABUS(dst)) {
	return COPY_FUNC_REQ(GFS_TMODE_CPU)();
    }
    return COPY_FUNC_REQ(tmode)();
}


/* �]���̎��s */
/* TRUE: �]�����J�n����   FALSE: �]���J�n���Ȃ� */
GFS_LOCAL Bool gftr_execTrn(GfsHn gfs)
{
    GftrHn	gftr = &GFS_FILE_TRANS(gfs);
    Uint8	*dst, *src;
    Sint32	dmode, smode, nbyte, nsct, len;
    GfdpHn	dps = GFS_TRN_DPS(gftr);
    GfdpHn	dpd = GFS_TRN_DPD(gftr);
    Sint32	tmode = GFS_TRN_MODE(gftr);

    if (tmode == GFS_TMODE_STM) {
	nsct = GFS_TRN_TFUNC(gftr)(GFS_TRN_OBJ(gftr), GFDP_NSCT(dps));
	if (nsct >= 0) {
	    GFDP_NSCT(dps) = nsct;
	    GFDP_NSCT(dpd) = nsct;
	    /* ���̎� GFS_TRN_UNIT(gftr) > 1 */
	    len = nsct * GFS_TRN_UNIT(gftr);
	    GFDP_LEN(dps) = len;
	    GFDP_LEN(dpd) = len;
	    GFS_TRN_TBYTCNT(gftr) = len;
	} else {
	    /* �]�����̎� */
	    GFDP_LEN(dps) = nsct;
	    return FALSE;
	}
    } else {
	if (GFS_TRN_TBYTCNT(gftr) == 0) {
	    if (GFBF_GetSctData(dps, &GFS_FILE_FLOW(gfs)) == FALSE) {
		return FALSE;
	    }
	}
	smode = GFDP_ADLT(dps);
	src   = (Uint8 *)GFDP_DATA(dps) + smode * GFS_TRN_TBYTCNT(gftr);
	dmode = GFDP_ADLT(dpd);
	dst   = GFDP_DATA(dpd);
	nbyte = GFDP_LEN(dpd);
	if (IN_WORKRAM_L(dst) || IN_ABUS(dst)) {
	    tmode = GFS_TMODE_CPU;
	} else {
	    tmode = GFS_TRN_MODE(gftr);
	}
	COPY_FUNC_EXEC(tmode)(dst, dmode, src, smode, nbyte);
	GFS_TRN_TBYTCNT(gftr) += nbyte;
    }
    return TRUE;
}


/* �c��]���o�C�g���̎擾 */
GFS_LOCAL Sint32 gftr_remTrn(GftrHn gftr)
{
    Sint32	tmode = GFS_TRN_MODE(gftr);
    GfdpHn	dpd = GFS_TRN_DPD(gftr);
    Uint8	*dst = GFDP_DATA(dpd);

    if (tmode == GFS_TMODE_STM) {
	return (GFDP_NSCT(GFS_TRN_DPD(gftr)) >= 0)? 0: 1;
    } else if (IN_WORKRAM_L(dst) || IN_ABUS(dst)) {
	return COPY_FUNC_REM(GFS_TMODE_CPU)();
    }
    return COPY_FUNC_REM(tmode)();
}


/* �S�o�C�g�ɖ����Ȃ������̓]�� */
GFS_LOCAL void gftr_copyFrac(Sint32 n, Uint32 *dp, Uint32 *sp)
{
    Uint8 *frac;
    Sint32 frac_buf, i;
    
    if (n > 0) {
	frac = (Uint8 *)dp;
	frac_buf = *sp;
	for (i = 0; i < n; ++i) {
	    *frac++ = ((Uint8 *)&frac_buf)[i];
	}
    }
}


/* �]�������o�C�g����Ԃ� */
Sint32 GFTR_GetLenData(GfsTrans *trn)
{
    return GFS_TRN_WP(trn);
}


/* �������o�b�t�@�̋󂫗e�ʂ����߂� */
Sint32 GFTR_GetLenRoom(GfsTrans *trn)
{
    return GFS_TRN_SIZE(trn) * GFS_TRN_UNIT(trn) - GFS_TRN_WP(trn);
}


/* �]����f�[�^�p�b�N�̎擾 */
GfdpHn GFTR_GetDstPack(GfsTrans *trn, Sint32 len)
{
    GfdpHn  dpk = &MNG_DSTPK(gfs_mng_ptr);
    
    /* �f�[�^�p�b�N���g�p���̏ꍇ */
    if (GFDP_USE(dpk)) {
        return (NULL);
    }
    /* �f�[�^�̈� */
    GFDP_DATA(dpk) = (Uint8 *)GFS_TRN_BUF(trn) + GFS_TRN_WP(trn);
    GFDP_ADLT(dpk) = sizeof(Uint8);	/* �A�h���X���Z�� */
    GFDP_LEN(dpk) = len;		/* �o�C�g�� */
    GFDP_USE(dpk) = TRUE;		/* �g�p���t���O�ݒ� */
    return dpk;
}


/* �]����f�[�^�p�b�N�Ԋ� */
void GFTR_RtnDstPk(GfsTrans *trn, GfdpHn dpk)
{
    GFS_TRN_WP(trn) += GFDP_LEN(dpk);	/* ���C�g�|�C���^��]�����i�߂� */
    GFDP_USE(dpk) = FALSE;		/* ���g�p�ɂ��� */
}


/* �������o�b�t�@���擾 */
void GFTR_GetData(GfsTrans *trn, Uint8 **data, Sint32 *bsize, Sint32 *unit)
{
    *data = GFS_TRN_BUF(trn);
    *bsize = GFS_TRN_SIZE(trn);
    *unit = GFS_TRN_UNIT(trn);
}


/* �f�[�^�̈�̐ݒ� */
void GFTR_SetData(GfsTrans *trn, Uint8 *data, Sint32 bsize, Sint32 unit)
{
    GFS_TRN_BUF(trn) = data;
    GFS_TRN_SIZE(trn) = bsize;
    GFS_TRN_UNIT(trn) = unit;
    GFS_TRN_WP(trn) = 0;
}



/* ���[�U�]���֐��̓o�^ */
void GFTR_SetTransFunc(GfsHn gfs, GfsTransFunc func, void *obj)
{
    GfsTrans	*gftr = &GFS_FILE_TRANS(gfs);

    GFS_TRN_OBJ(gftr) = obj;
    GFS_TRN_TFUNC(gftr) = func;
    GFS_TRN_MODE(gftr) = GFS_TMODE_STM;
}

/* end of file */
