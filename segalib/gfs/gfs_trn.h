/******************************************************************************
 *	�\�t�g�E�F�A���C�u����
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:�t�@�C���V�X�e��
 * Module :�]�����W���[���w�b�_
 * File	  :gfs_trn.h
 * Date   :1994-11-11
 * Version:1.16
 * Auther :M.S.
 *
 *************************************************************************** */

#if !defined(GFS_TRN_H)
#define GFS_TRN_H

/*****************************************************************************
 *	�C���N���[�h�t�@�C��
 *****************************************************************************/

/*****************************************************************************
 *	�萔�}�N��
 *****************************************************************************/
/* RtnPk�̈��� */
#define GFTR_ALLSCT	-1	/* GetSrcPack�ŗv�������Z�N�^��AP��i�߂� */

#define GFTR_NULLFUNC	((GfsTransFunc)0)

/*****************************************************************************
 *	�񋓒萔
 *****************************************************************************/
/* GFTR_Exec �̏������ */
enum {
    GFTR_ST_NOACT = 0,			/* ��~���	*/
    GFTR_ST_STAT1,			/* ���1	*/
    GFTR_ST_STAT2,			/* ���2	*/
    GFTR_ST_STAT3,			/* ���3	*/
    GFTR_ST_STAT4,			/* ���4	*/
    GFTR_ST_STAT5,			/* ���5	*/
    GFTR_ST_END,			/* �]���I�����	*/
    GFTR_ST_ERROR			/* �G���[��� */
    };


/*****************************************************************************
 *	�ϐ��̐錾
 *****************************************************************************/

/*****************************************************************************
 *	�֐��̐錾
 *****************************************************************************/
/* �]����̐ݒ�  */
GftrHn  GFTR_Setup(GfsTrans *gftr);

/* �]�����[�h�̐ݒ� */
Sint32 GFTR_SetMode(GftrHn gftr, Sint32 tmode);

/* �]���P�ʂ̐ݒ� */
void GFTR_SetUnit(GftrHn gftr, Sint32 unit);

/* �]���Z�N�^���̐ݒ� */
void GFTR_SetTransPara(GftrHn gftr, Sint32 tsct);

/* �S�]���Z�N�^���̎擾 */
Sint32 GFTR_GetTransPara(GftrHn gftr);

/* ���݂܂łɓ]�����ꂽ�Z�N�^���̎擾 */
Sint32 GFTR_GetNumTrans(GftrHn gftr);

/* �P��̓]���ɂ�����ő�]���Z�N�^���̐ݒ� */
Sint32 GFTR_SetTransMax(GftrHn gftr, Sint32 tmax);

/* �]���J�n */
void GFTR_Start(GftrHn gftr);

/* �]���ꎞ��~ */
void GFTR_Pause(GftrHn gftr);

/* �]�����~ */
void GFTR_Stop(GfsHn gfs, Sint32 nsct);

/* �]���̎��s */
Sint32 GFTR_Exec(GfsHn gfs, Bool *flag);

/* �z�X�g���o�b�t�@�t����̋󑗂�]�� */
Sint32 GFTR_Idle(GfsHn gfs);

/* �]�������o�C�g����Ԃ� */
Sint32 GFTR_GetLenData(GfsTrans *trn);

/* �������o�b�t�@�̋󂫗e�ʂ����߂� */
Sint32 GFTR_GetLenRoom(GfsTrans *trn);

/* �]����f�[�^�p�b�N�̎擾 */
GfdpHn GFTR_GetDstPack(GfsTrans *trn, Sint32 len);

/* �]����f�[�^�p�b�N�Ԋ� */
void GFTR_RtnDstPk(GfsTrans *trn, GfdpHn dpk);

/* �]���̈���擾 */
void GFTR_GetData(GfsTrans *trn, Uint8 **data, Sint32 *bsize, Sint32 *unit);

/* �f�[�^�̈�̐ݒ� */
void GFTR_SetData(GfsTrans *trn, Uint8 *data, Sint32 bsize, Sint32 unit);

/* ���[�U�]���֐��̓o�^ */
void GFTR_SetTransFunc(GfsHn gfs, GfsTransFunc func, void *obj);

#endif      /* !defined(GFS_TRN_H) */

/* end of file */
