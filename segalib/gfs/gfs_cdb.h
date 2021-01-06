/******************************************************************************
 *	�\�t�g�E�F�A���C�u����
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library:�t�@�C���V�X�e��
 * Module :CD�o�b�t�@���W���[���w�b�_
 * File	  :gfs_cdb.h
 * Date   :1994-11-11
 * Version:1.16
 * Auther :M.S.
 *
 *************************************************************************** */

#if !defined(GFS_CDB_H)
/*****************************************************************************
 *	�C���N���[�h�t�@�C��
 *****************************************************************************/

/*****************************************************************************
 *	�萔�}�N��
 *****************************************************************************/
#define GFS_CDB_H

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

/*  CD�o�b�t�@�����Ǘ��\���̂̏����� */
Bool GFCB_Setup(GfsFlow *flow);

/* �g�p�����̉�� */
void GFCB_Close(GfsDtsrc *dts);

/* ���o�����[�h�̐ݒ� */
Sint32 GFCB_SetGmode(GfsDtsrc *dts, Sint32 gmode);

/* �b�c�o�b�t�@�̗L���Z�N�^���擾 */
Sint32 GFCB_GetLenData(GfsFlow *flow, Sint32 usct, Sint32 *asiz);

/* ���[�h�i��̐ݒ� */
void GFCB_SetFilt(GfsDtsrc *dts, Sint32 fmode, CdcSubh *subh);

/*  �b�c�o�b�t�@�S����	*/
Sint32 GFCB_DeleteAllData(GfsDtsrc *dts);

/*  �A�N�Z�X�|�C���^�擾 */
Sint32 GFCB_Tell(GfsDtsrc *dts);

/*  �A�N�Z�X�|�C���^�ݒ� */
Sint32 GFCB_Seek(GfsFlow *flow, Sint32 off, Sint32 org);

/*  �]�����f�[�^�p�b�N�p�����[�^�ݒ�	*/
GfdpHn GFCB_GetSrcPack(GfsFlow *flow, Sint32 nsct);

/* �]�����f�[�^�p�b�N�̏��� */
GfdpHn GFCB_ReadySrcPack(GfsFlow *flow, Sint32 nsct);

/* �f�[�^�p�b�N���擾������̃f�[�^�v�� */
Bool GFCB_GetSctData(GfdpHn dpk, GfsDtsrc *dts);

/*  �]����f�[�^�p�b�N�p�����[�^�ݒ�	*/
GfdpHn GFCB_GetDstPack(GfsDtsrc *dts, Sint32 len);

/*  �f�[�^�p�b�N�Ԋ�	*/
void GFCB_RtnPk(GfsFlow *flow, GfdpHn dpk, Sint32 nsct);

/* �s�b�N�A�b�v�̈ړ� */
Sint32 GFCB_MovePickup(GfsDtsrc *dts, CdcPos *pos);

/* CD�f�o�C�X�ڑ� */
Bool GFCB_SetCon(GfsDtsrc *dts);

/* �s�b�N�A�b�v�̐��䌠���m�ۂ��� */
Bool GFCB_GetPickup(GfsDtsrc *dts);

/* �s�b�N�A�b�v���m�ۂ��Ă��邩���ׂ� */
Bool GFCB_IsPuOwner(GfsDtsrc *dts);

/* �s�b�N�A�b�v��������� */
void GFCB_UngetPickup(GfsDtsrc *dts);


#endif      /* GFS_CDB_H    */

/* end of file */
