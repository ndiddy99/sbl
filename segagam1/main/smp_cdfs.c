/*
 *
 * CD�t�@�C���V�X�e���R���g���[��
 *
 */


#include <string.h>

#include "smp_bios.h"

#define USE_SIMM 1   /* SIMM�œ������Ȃ� '1' */

extern Sint8 *GFMC_base;

Sint32     SMCD_gfsret;
Sint32     SMCD_readflag;
Uint8      SMCD_gfslib_work[GFS_WORK_SIZE(SMCD_MAX_OPEN)];
GfsDirTbl  SMCD_dirtbl;              /* �f�B���N�g�����Ǘ��̈� */
GfsDirId   SMCD_dirid[SMCD_MAX_DIR]; /* �t�@�C�������܂܂Ȃ���� */
GfsDirName SMCD_dir_name[SMCD_MAX_DIR];


/*
 *    �t�@�C���̂P�����[�h������
 *    In... fname...�t�@�C���l�[���|�C���^
 *          buf..���[�h��A�h���X
 *    Out.. ���̏ꍇ���[�h�T�C�Y
 *          ���̏ꍇ���[�h�G���[
 */
Sint32 SMCD_FileLoad(Uint8 *fname, void *buf)
{
      GfsHn gfs;
      Sint32 fid;
      Sint32 SMCD_readflag;
  
      fid = GFS_NameToId((Sint8 *)fname);
      if (fid < 0) return fid;
      SMCD_readflag = GFS_Load(fid, 0, buf, GFS_BUFSIZ_INF);
      return SMCD_readflag;
}


/*
 *   �t�@�C���V�X�e���̏�����
 *   �߂�l���Q���傫���Ȃ��Ɛ���ɃA�N�Z�X�ł��Ȃ�
 */
Sint32 SMCD_InitFileSys(void)
{
#if USE_SIMM
    /*  SIMM�t�@�C���V�X�e���ŗ����オ������  */
    if(strcmp((char *)0x04020004, "IP.BIN") == 0) {
        GFMC_base = (Sint8 *)0x4020000;    /* SIMM �̂Ƃ��K�v */
	}
#endif
      
      GFS_DIRTBL_TYPE(&SMCD_dirtbl) = GFS_DIR_NAME;
      GFS_DIRTBL_DIRNAME(&SMCD_dirtbl) =SMCD_dir_name;
      GFS_DIRTBL_NDIR(&SMCD_dirtbl) = SMCD_MAX_DIR;
      SMCD_gfsret = GFS_Init(SMCD_MAX_OPEN, &SMCD_gfslib_work, &SMCD_dirtbl);
      return SMCD_gfsret;
}


/*
 *   �t�@�C�����w��Ńt�@�C�����I�[�v������
 */
GfsHn SMCD_OpenFileByName(GfsDirTbl *dirtbl, Uint8 *fname)
{
     Sint32 fid;

     /* �t�@�C��������t�@�C�����ʎq�����߂� */
     fid = GFS_NameToId((Sint8 *)fname);
     return GFS_Open(fid);

}








