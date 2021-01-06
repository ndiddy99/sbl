/*
 *
 * CDファイルシステムコントロール
 *
 */


#include <string.h>

#include "smp_bios.h"

#define USE_SIMM 1   /* SIMMで動かすなら '1' */

extern Sint8 *GFMC_base;

Sint32     SMCD_gfsret;
Sint32     SMCD_readflag;
Uint8      SMCD_gfslib_work[GFS_WORK_SIZE(SMCD_MAX_OPEN)];
GfsDirTbl  SMCD_dirtbl;              /* ディレクトリ情報管理領域 */
GfsDirId   SMCD_dirid[SMCD_MAX_DIR]; /* ファイル名を含まない情報 */
GfsDirName SMCD_dir_name[SMCD_MAX_DIR];


/*
 *    ファイルの１括ロードをする
 *    In... fname...ファイルネームポインタ
 *          buf..ロード先アドレス
 *    Out.. 正の場合リードサイズ
 *          負の場合リードエラー
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
 *   ファイルシステムの初期化
 *   戻り値が２より大きくないと正常にアクセスできない
 */
Sint32 SMCD_InitFileSys(void)
{
#if USE_SIMM
    /*  SIMMファイルシステムで立ち上がったら  */
    if(strcmp((char *)0x04020004, "IP.BIN") == 0) {
        GFMC_base = (Sint8 *)0x4020000;    /* SIMM のとき必要 */
	}
#endif
      
      GFS_DIRTBL_TYPE(&SMCD_dirtbl) = GFS_DIR_NAME;
      GFS_DIRTBL_DIRNAME(&SMCD_dirtbl) =SMCD_dir_name;
      GFS_DIRTBL_NDIR(&SMCD_dirtbl) = SMCD_MAX_DIR;
      SMCD_gfsret = GFS_Init(SMCD_MAX_OPEN, &SMCD_gfslib_work, &SMCD_dirtbl);
      return SMCD_gfsret;
}


/*
 *   ファイル名指定でファイルをオープンする
 */
GfsHn SMCD_OpenFileByName(GfsDirTbl *dirtbl, Uint8 *fname)
{
     Sint32 fid;

     /* ファイル名からファイル識別子を求める */
     fid = GFS_NameToId((Sint8 *)fname);
     return GFS_Open(fid);

}








