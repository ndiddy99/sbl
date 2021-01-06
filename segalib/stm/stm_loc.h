/*****************************************************************************
 *	�\�t�g�E�F�A���C�u����
 *
 *	Copyright (c) 1994 SEGA
 *
 * Library: �X�g���[���V�X�e��
 * Module : ���[�J���֐��p�w�b�_�t�@�C��
 * File	  : stm_loc.h
 * Date   : 1994-11-11
 * Version: 1.16
 * Auther : H.T
 *
 *****************************************************************************/
#if	!defined(STM_LOC_H)
#define	STM_LOC_H	1

/*****************************************************************************
 *	�񋓒萔
 *****************************************************************************/

/* �i�胂�[�h			*/
enum FiltMode {
    FMODE_FN	= 0x01,			/* �t�@�C���ԍ��I��L��		*/
    FMODE_CN	= 0x02,			/* �`���l���ԍ��I��L��		*/
    FMODE_SM	= 0x04,			/* �T�u���[�h�I��L��		*/
    FMODE_CI	= 0x08,			/* CI�I��L��			*/
    FMODE_REVSH	= 0x10,			/* �T�u�w�b�_�I���������]	*/
    FMODE_FAD	= 0x40,			/* �Z�N�^�͈͗L��		*/
    FMODE_INIT	= 0x80,			/* �i�����������		*/
    FMODE_END
};


/*****************************************************************************
 *	�֐��̐錾
 *****************************************************************************/

/* ���݂�FAD�̎擾		*/
Sint32	STL_GetCurFad(void);

/* ��~�҂�			*/
void	STL_WaitStop(void);

/* �Đ��͈͂̎擾		*/
void	STL_GetPlayArea(GfsFlow *flow, Sint32 *sfad, Sint32 *efad);

/* �X�g���[���O���[�v�Đ�	*/
Sint32	STL_FlowIn(StmGrpHn grp);

/* �ǂݍ��ݒ�~			*/
Sint32	STL_StopIn(StmGrpHn grp);

/* ���[�h�G���[�������̃��J�o��	*/
Sint32	STL_Recover(StmGrpHn grp);

/* �s�b�N�A�b�v�̈ړ�(�Đ��p��)	*/
Sint32	STL_MovePlaypos(StmGrpHn grp, Sint32 fad);

/* �Đ��I���ʒu�̕ύX		*/
Sint32	STL_ChgPlayEnd(StmGrpHn grp, Sint32 efad);

/* �ǂݍ��݃Z�N�^���̎擾	*/
Bool	STL_GetSctInfo(GfsFlow *flow, Sint32 sn, StmSct *sinfo);

/* �Z�N�^�̎��f�[�^�T�C�Y�̎擾	*/
Sint32	STL_GetActSiz(GfsFlow *flow, Sint32 spos);

/* CD�o�b�t�@�f�[�^�̏���	*/
void	STL_DeleteSctData(GfsFlow *flow, Sint32 spos, Sint32 snum);

/* CSCT�֐��pCD�o�b�t�@�g�p�Z�N�^���̎擾	*/
Sint32	STL_CsctGetSctNum(GfsFlow *flow);

/* CSCT�֐��pCD�o�b�t�@�󂫃T�C�Y�̎擾	*/
Sint32	STL_CsctGetFreeSctnum(void);

/* CSCT�r�b�g�����̂�҂�	*/
void	STL_WaitCsct(void);

/* end of stm_loc.h	*/

#endif

