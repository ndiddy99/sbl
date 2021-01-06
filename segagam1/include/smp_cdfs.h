/*
 *
 * CDファイルシステムコントロール
 *
 */


#ifndef	__SMP_CDFS_H
#define	__SMP_CDFS_H



/*
 *    定数
 */
#define SMCD_MAX_OPEN  1      /* 同時にオープンするファイルの数     */
#define SMCD_MAX_DIR   32     /* ルートディレクトリにあるファイル数 */
#define SMCD_SECT_SIZE 2048   /* １セクタのバイト数                 */
#define SMCD_RD_SIZE   1      /* 読み込みセクタ数                   */


/*
 *   ファイル識別子定義
 */
#define SMCD_SYSTBL_TSK    2
#define SMCD_SDDRV_TSK     3
#define SMCD_GMMAP_BIN     4
#define SMCD_GMSEQ_BIN     5
#define SMCD_GMTONE_BIN    6
#define SMCD_OPN_STM       8
#define SMCD_LOGO_BIN      7

/*
 *    関数プロトタイプ宣言 
 */
extern Sint32 SMCD_FileLoad(Uint8 *fname, void *buf);
extern Sint32 SMCD_InitFileSys(void);
extern GfsHn SMCD_OpenFileByName(GfsDirTbl *dirtbl, Uint8 *fname);


/*
 *    ワークRAM
 */

/* GFSライブラリワーク領域 */
GLOBAL Sint32     SMCD_gfsret;
GLOBAL Sint32     SMCD_readflag;
GLOBAL Uint8      SMCD_gfslib_work[GFS_WORK_SIZE(SMCD_MAX_OPEN)];
GLOBAL GfsDirTbl  SMCD_dirtbl;              /* ディレクトリ情報管理領域 */
GLOBAL GfsDirId   SMCD_dirid[SMCD_MAX_DIR]; /* ファイル名を含まない情報 */
GLOBAL GfsDirName SMCD_dir_name[SMCD_MAX_DIR];


#endif




