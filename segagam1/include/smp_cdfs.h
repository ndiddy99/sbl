/*
 *
 * CD�t�@�C���V�X�e���R���g���[��
 *
 */


#ifndef	__SMP_CDFS_H
#define	__SMP_CDFS_H



/*
 *    �萔
 */
#define SMCD_MAX_OPEN  1      /* �����ɃI�[�v������t�@�C���̐�     */
#define SMCD_MAX_DIR   32     /* ���[�g�f�B���N�g���ɂ���t�@�C���� */
#define SMCD_SECT_SIZE 2048   /* �P�Z�N�^�̃o�C�g��                 */
#define SMCD_RD_SIZE   1      /* �ǂݍ��݃Z�N�^��                   */


/*
 *   �t�@�C�����ʎq��`
 */
#define SMCD_SYSTBL_TSK    2
#define SMCD_SDDRV_TSK     3
#define SMCD_GMMAP_BIN     4
#define SMCD_GMSEQ_BIN     5
#define SMCD_GMTONE_BIN    6
#define SMCD_OPN_STM       8
#define SMCD_LOGO_BIN      7

/*
 *    �֐��v���g�^�C�v�錾 
 */
extern Sint32 SMCD_FileLoad(Uint8 *fname, void *buf);
extern Sint32 SMCD_InitFileSys(void);
extern GfsHn SMCD_OpenFileByName(GfsDirTbl *dirtbl, Uint8 *fname);


/*
 *    ���[�NRAM
 */

/* GFS���C�u�������[�N�̈� */
GLOBAL Sint32     SMCD_gfsret;
GLOBAL Sint32     SMCD_readflag;
GLOBAL Uint8      SMCD_gfslib_work[GFS_WORK_SIZE(SMCD_MAX_OPEN)];
GLOBAL GfsDirTbl  SMCD_dirtbl;              /* �f�B���N�g�����Ǘ��̈� */
GLOBAL GfsDirId   SMCD_dirid[SMCD_MAX_DIR]; /* �t�@�C�������܂܂Ȃ���� */
GLOBAL GfsDirName SMCD_dir_name[SMCD_MAX_DIR];


#endif




