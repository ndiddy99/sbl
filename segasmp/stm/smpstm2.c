/*****************************************************************************
 *	ソフトウェアライブラリ
 *
 *	Copyright (c) 1994 SEGA
 *
 * Module : ストリームシステムサンプルプログラム2
 *	    (分岐再生用サンプルプログラム)
 * File	  : smpstm2.c
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

/* ファイル名の長さ		*/
#define	FNAME_LEN	16

/* 取り出しバッファサイズ	*/
#define	FORM1_SCTSIZ	2048
#define	BUF_UNITNUM	10
#define	BUF_SIZE	(BUF_UNITNUM * FORM1_SCTSIZ)

/* 扱うストリーム数		*/
#define	STM_NUM		5

/* 再生終了			*/
#define	PEND		(-1)

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

/* 取り出しバッファ		*/
Uint8		ReadBuf[BUF_SIZE];

/* ファイル名			*/
Sint8		MngTbl[STM_NUM][FNAME_LEN] = {	
                                                 "SMP_STM.1ST",
						 "SMP_STM.2ND",
						 "SMP_STM.3RD",
						 "SMP_STM.4TH",
						 "SMP_STM.5TH",
					     };

/* 再生順番号			*/
Sint32		PlayOrder[] = {0, 1, 2, 3, 4, 3, 2, 1, 0, 2, 1, 3, 4, PEND};


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
StmHn	SMP_Open(StmGrpHn grp, Sint8 *fname)
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
    stm = STM_OpenFid(grp, fid, &key, STM_LOOP_NOREAD);
    if (stm == NULL) {
	return (NULL);
    }

    /* 取り出し領域の設定			*/
    STM_SetTrBuf(stm, ReadBuf, BUF_UNITNUM, STM_UNIT_FORM1);

    /* 転送ゲートの設定				*/
    STM_SetTrGate(stm, STM_GATE_CLOSE);

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
 * ストリーム読み込み実行
 *-------------------------------*/
void	SMP_Exec(StmGrpHn grp)
{
    Sint32	i;
    Bool	complete;
    StmHn	stm, next;

    /* ループ回数の設定			*/
    STM_SetLoop(grp, STM_LOOP_DFL, STM_LOOP_ENDLESS);

    /* 実行グループの設定		*/
    STM_SetExecGrp(grp);

    /* 最初のストリームのオープン	*/
    complete = TRUE;
    stm = SMP_Open(grp, MngTbl[PlayOrder[0]]);
    next = NULL;

    /* ストリームアクセス実行		*/
    i = 1;
    while (stm != NULL) {
	/* 前のストリームのアクセスが完了したらゲートを開ける	*/
	if (complete == TRUE) {
	    STM_SetTrGate(stm, STM_GATE_OPEN);
	}

	STM_ExecServer();
	
	/* データの取り出しが始まったら次のストリームをオープンする	*/
	if ((next == NULL)&&
	    (STM_GetLenTrBuf(stm) > 0)&&(PlayOrder[i] != PEND)) {
	    next = SMP_Open(grp, MngTbl[PlayOrder[i]]);
	    i++;
	}

	/* 取り出し領域フルのチェック	*/
	if (STM_IsTrBufFull(stm) == TRUE) {
	    STM_ResetTrBuf(stm);
	}

	/* アクセス完了のチェック	*/
	if ((complete = STM_IsComplete(stm)) == TRUE) {
	    STM_Close(stm);
	    stm = next;
	    next = NULL;
	}

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

    /* 読み込み実行	*/
    SMP_Exec(grp);
}


/* end of file */
