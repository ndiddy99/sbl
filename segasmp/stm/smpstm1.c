/*****************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994 SEGA
 *
 * Module : ストリームシステムサンプルプログラム1
 *	    (ループ再生用サンプルプログラム)
 * File	  : smpstm1.c
 * Date   : 1994-11-11
 * Auther : H.T
 *
 *****************************************************************************/

/*****************************************************************************
 *	インクルードファイル
 *****************************************************************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	"sega_stm.h"

/*****************************************************************************
 *	定数マクロ
 *****************************************************************************/

/* ファイル名			*/
#define	SMP_STM_FN1	"SMP_STM.FN1"
#define	SMP_STM_FN2	"SMP_STM.FN2"
#define	SMP_STM_FN3	"SMP_STM.FN3"

/* 読み込みバッファサイズ	*/
#define	FORM1_SCTSIZ	2048
#define	FN1_UNITNUM	5
#define	FN2_UNITNUM	10
#define	FN3_UNITNUM	15
#define	FN1_BUFSIZ	(FN1_UNITNUM * FORM1_SCTSIZ)
#define	FN2_BUFSIZ	(FN2_UNITNUM * FORM1_SCTSIZ)
#define	FN3_BUFSIZ	(FN3_UNITNUM * FORM1_SCTSIZ)

/* ループ回数			*/
#define	LOOP_MAX	3

/* ライブラリ用定数		*/
#define	FILE_NUM	30	/* １ディレクトリ当たりの最大ファイル数	*/
#define	OPEN_MAX	24	/* 同時に扱うストリーム／ファイル数	*/
#define	GRP_MAX		5	/* 同時に扱うグループ数			*/

/*****************************************************************************
 *	変数の定義
 *****************************************************************************/
/* ファイルシステム作業領域	*/
Uint32		WorkGfs[GFS_WORK_SIZE(OPEN_MAX)/sizeof(Uint32)];

/* ストリームシステム作業領域	*/
Uint32		WorkStm[STM_WORK_SIZE(OPEN_MAX, GRP_MAX)/sizeof(Uint32)];

/* ディレクトリ読み込み領域	*/
GfsDirName	DirTbl[FILE_NUM];

/* 読み込みバッファ		*/
Uint8		Fn1_Buf[FN1_BUFSIZ];
Uint8		Fn2_Buf[FN2_BUFSIZ];
Uint8		Fn3_Buf[FN3_BUFSIZ];


/*****************************************************************************
 *	関数の定義
 *****************************************************************************/
/*-------------------------------
 * 初期化
 * 戻り値 : TRUE  初期化成功
 *          FALSE 初期化失敗
 *-------------------------------*/
Bool	SMP_Init(void)
{
    GfsDirTbl	dir;
    Sint32	ndir;
    Bool	init;

   /* ファイルシステム初期化	*/
    GFS_DIRTBL_TYPE(&dir) = GFS_DIR_NAME;
    GFS_DIRTBL_NDIR(&dir) = FILE_NUM;
    GFS_DIRTBL_DIRNAME(&dir) = DirTbl;
    ndir = GFS_Init(OPEN_MAX, WorkGfs, &dir);
    if (ndir < 0) {
	return (FALSE);
    }

    /* ストリームシステム初期化	*/
    init = STM_Init(GRP_MAX, OPEN_MAX, WorkStm);
    if (init == FALSE) {
	return (FALSE);
    }
    return (TRUE);
}


/*-------------------------------
 * ストリームのオープン
 * 戻り値 : ストリームハンドル
 *-------------------------------*/
StmHn	SMP_Open(StmGrpHn grp, Sint8 *fname, void *buf, Sint32 nunit)
{
    Sint32	fid;
    StmHn	stm;
    StmKey	key;

    /* ファイル名からファイル識別子への変換	*/
    fid = GFS_NameToId(fname);
    if (fid < 0) {
	return (NULL);
    }

    /* ストリームキーの設定			*/
    STM_KEY_CN(&key) = STM_KEY_SMMSK(&key) = STM_KEY_CIMSK(&key) = STM_KEY_NONE;

    /* ストリームオープン			*/
    stm = STM_OpenFid(grp, fid, &key, STM_LOOP_READ);
    if (stm == NULL) {
	return (NULL);
    }

    /* 読み込み領域の設定			*/
    STM_SetTrBuf(stm, buf, nunit, STM_UNIT_FORM1);

    return (stm);
}


/*-------------------------------
 * ユーザ関数
 *-------------------------------*/
void	SMP_User(void)
{
    /* アプリケーション独自の処理をおこなう	*/
}



/*-------------------------------
 * 読み込み領域のチェック
 *-------------------------------*/
void	SMP_ChkBuf(StmGrpHn grp, Sint32 nstm, StmHn stmtbl[], Sint32 rscnt[])
{
    Sint32	i;
    Sint32	lpcnt;
    Bool	bful;

    lpcnt = STM_GetLoopCnt(grp);

    /* 読み込み領域がいっぱいになったらリセットする	*/
    for (i = 0; i < nstm; i++) {
	bful = STM_IsTrBufFull(stmtbl[i]);
	if (bful == TRUE) {
	    STM_ResetTrBuf(stmtbl[i]);
	    rscnt[i]++;
	}
    }
}



/*-------------------------------
 * ストリーム読み込み実行
 *-------------------------------*/
void	SMP_Exec(StmGrpHn grp)
{
    StmHn	stmtbl[OPEN_MAX];
    Sint32	stat;
    Sint32	nstm;
    Sint32	i;
    Sint32	rscnt[OPEN_MAX];

    /* ループ回数の設定			*/
    STM_SetLoop(grp, STM_LOOP_DFL, LOOP_MAX);

    /* 実行グループの設定		*/
    STM_SetExecGrp(grp);

    /* グループに属するストリームの取得	*/
    nstm = STM_GetStmNum(grp);
    for (i = 0; i < nstm; i++) {
	stmtbl[i] = STM_GetStmHndl(grp, i);
	rscnt[i] = 0;
    }

    /* 読み込み実行			*/
    while(1) {
	stat = STM_ExecServer();
	if (stat == STM_EXEC_COMPLETED) {
	    break;
	}
	
	/* 読み込み領域のチェック	*/
	SMP_ChkBuf(grp, nstm, stmtbl, rscnt);

	/* ユーザ関数	*/
	SMP_User();
    }
}
	

/*-------------------------------
 * メイン
 *-------------------------------*/
void	main(void)
{
    Bool	init;
    StmGrpHn	grp;
    StmHn	stm1, stm2, stm3;

    /* 初期化		*/
    init = SMP_Init();
    if (init == FALSE) {
	return;
    }

    /* ハンドル取得	*/
    grp = STM_OpenGrp();
    if (grp == NULL) {
	return;
    }
    stm1 = SMP_Open(grp, SMP_STM_FN1, Fn1_Buf, FN1_UNITNUM);
    if (stm1 == NULL) {
	return;
    }
    stm2 = SMP_Open(grp, SMP_STM_FN2, Fn2_Buf, FN2_UNITNUM);
    if (stm2 == NULL) {
	return;
    }
    stm3 = SMP_Open(grp, SMP_STM_FN3, Fn3_Buf, FN3_UNITNUM);
    if (stm3 == NULL) {
	return;
    }

    /* 読み込み実行	*/
    SMP_Exec(grp);
}

/* end of file */


