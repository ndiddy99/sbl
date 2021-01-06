/*******************************************************************
*
*                　　　 サターン用データ圧縮
*
*                      Copyright(c) 1994 SEGA
*
*   Comment: ツール・ライブラリ共用ヘッダ
*   File   : CMP.H
*   Date   : 1994-01-18
*   Author : Y.T
*
*******************************************************************/

/*******************************************************************
	  定数マクロ
*******************************************************************/

#define MAXFILENAME		(65)		/* max file name 		*/
#define CMP_BUFSIZE 	(64000U)

/* 圧縮ファイルヘッダ */
/* アルゴリズム */
#define		CMP_ALGO_MASK	(0xF000)	/* アルゴリズム記述ビット15..12	*/
#define		CMP_ALGO_RUNLEN	(0x0000)	/* ランレングス 	*/
#define		CMP_ALGO_SLIDE	(0x1000)	/* スライド辞書 	*/
#define		CMP_ALGO_HUFF	(0x2000)	/* ハフマン 		*/
/* 処理単位（１記号の幅） */
#define		CMP_UNIT_MASK 	(0x0C00)	/* 処理単位記述ビット11..10 	*/
#define		CMP_UNIT_BYTE 	(0x0000)	/* １バイト 	*/
#define		CMP_UNIT_WORD 	(0x0400)	/* ２バイト 	*/
#define		CMP_UNIT_DWORD 	(0x0C00)	/* ４バイト 	*/
/* 元サイズ幅 */
#define		CMP_ORGW_MASK	(0x0008)	/* 元サイズ幅記述ビット3	 	*/
#define		CMP_ORGW_WORD 	(0x0000)	/* ２バイト 	*/
#define		CMP_ORGW_DWORD	(0x0008)	/* ４バイト 	*/
/* 拡張符号語 */
#define		CMP_EXTC_MASK	(0x0001)	/* 拡張符号語記述ビット0	 	*/
#define		CMP_EXTC_NOUSE 	(0x0000)	/* 使用しない 	*/
#define		CMP_EXTC_USE	(0x0001)	/* 使用する 	*/

/* ランレングス処理 */
/* 入力系列状態 */
#define		CMP_RST_RUN1	(1)			/* ラン／ラン長１状態			*/
#define		CMP_RST_RUN2	(2)			/* ラン状態						*/
#define		CMP_RST_NRUN	(3)			/* 不一致系列状態				*/
#define		CMP_RST_NRUNR	(4)			/* 不一致系列／ランリーチ状態	*/
/* 表現可能な、最大一致系列長。 */
#define		CMP_MAX_EQRUN_BYTE		(0x0000007fL + 2L)
#define		CMP_MAX_EQRUN_WORD		(0x00007fffL + 2L)
#define		CMP_MAX_EQRUN_DWORD		(0x7fffffffL)
/* 表現可能な、最大不一致系列長。 */
#define		CMP_MAX_NEQRUN_BYTE		(0x00000080L)
#define		CMP_MAX_NEQRUN_WORD		(0x00008000L)
#define		CMP_MAX_NEQRUN_DWORD	(0x7fffffffL)
/* １記号（文字）のバイト数。 */
#define		CMP_BYTEBYTE		(1)
#define		CMP_BYTEWORD		(2)
#define		CMP_BYTEDWORD		(4)
/* バウンダリ調整数。 */
#define		CMP_BOUNBYTE		(CMP_BYTEBYTE - 1)
#define		CMP_BOUNWORD		(CMP_BYTEWORD - 1)
#define		CMP_BOUNDWORD		(CMP_BYTEDWORD - 1)

/* スライド辞書 */
/* １回の読み込みサイズ（4回でリングバッファ全体分） */
#define		CMP_SD_READ_SIZE		(4096)
/* 入力用リングバッファサイズ */
#define		CMP_SD_RINGBUF_SIZE		(4*CMP_SD_READ_SIZE)
/* シフトサイズ（ memmove するデータ量のこと、シフトする距離ではない）  */
#define		CMP_SD_SHIFT_SIZE		(CMP_SD_RINGBUF_SIZE - CMP_SD_READ_SIZE)
/* 追加読み込み位置（バッファ先頭からオフセット） */
#define		CMP_SD_READ_OFFSET		(CMP_SD_SHIFT_SIZE)
/* シフト処理トリガ位置（バッファ先頭からオフセット）  */
#define		CMP_SD_TRIGGER_OFFSET	(CMP_SD_RINGBUF_SIZE - CMP_SD_READ_SIZE)


/*******************************************************************
	  処理マクロ
*******************************************************************/
#define dfprintf 	DFP
#define DFP(a) 		fprintf a
#define MINCUSTM(a,outpend,outp)\
		MIN(a, (Uint8 *)outpend + 1 - *(Uint8 **)outp)

/* void mload_word(Uint16 _val, void *_from) */
#define mload_word(valtype, _val, fromtype, _from)\
{\
	char 	*from;\
	Uint16 	val;\
\
	from = (char *)_from;\
	val = *from++;\
	val = (val << 8) | (*from++ & 0x00ff);\
	_val = (valtype)val;\
	_from = (fromtype)from;\
}

/* void mload_sword(Sint16 _val, void *_from) */
#define mload_sword(valtype, _val, fromtype, _from)\
{\
	char 	*from;\
	Sint16 	val;\
\
	from = (char *)_from;\
	val = *from++;\
	val = (val << 8) | (*from++ & 0x00ff);\
	_val = (valtype)val;\
	_from = (fromtype)from;\
}
/* void mload_dword(Uint32 _val, void *_from) */
#define mload_dword(valtype, _val, fromtype, _from)\
{\
	char 	*from;\
	Uint32 	val;\
\
	from = (char *)_from;\
	val = *from++;\
	val = (val << 8) | (*from++ & 0x000000ff);\
	val = (val << 8) | (*from++ & 0x000000ff);\
	val = (val << 8) | (*from++ & 0x000000ff);\
	_val = (valtype)val;\
	_from = (fromtype)from;\
}
/*******************************************************************
	  プロトタイプ宣言
*******************************************************************/

extern char buf_in[];
extern char buf_out[];
extern Uint8 sd_ringbuf[CMP_SD_RINGBUF_SIZE];
extern Uint16 bittbl[16];
#ifndef	COMPILER_CC
extern int CMP_CodeRunlen(char *name_in, int unit, int opti, char *name_out);
extern int CMP_CodeRunlenByte(char *name_in, char *name_out, long *cmpfsize);
extern int CMP_CodeRunlenWord(char *name_in, char *name_out, long *cmpfsize);
extern int CMP_CodeRunlenDword(char *name_in, char *name_out, long *cmpfsize);
extern int CMP_DecSdBlkStd(void **inp, void **outp, void *outpend,int datasum);
extern int CMP_DecSdBlkExt(void **inp, void **outp, void *outpend,int datasum);
/* cmptool.c */
extern char name_tmp[MAXFILENAME];	/* テンポラリファイル名 */
extern int update(char *objname, char *tmpname);
extern int fput_word(Uint16 w, FILE *fp);
extern int fput_dword(Uint32 w, FILE *fp);
extern int fget_word(Uint16 *w, FILE *fp);
extern int fget_dword(Uint32 *w, FILE *fp);
extern void swap_word(Uint16 *w);
extern void swap_dword(Uint32 *a);
extern void print_rate(char *name_in, long orgfsize, char *name_out, long cmpfsize);
/* cmpltool.c */
extern void *memset_word(void *to, Uint16 a, size_t n);
extern void *memset_dword(void *to, Uint32 a, size_t n);
extern void *memmove_byte(void *_to, void *_from, size_t n);
/*
extern void mload_word(Uint16 *_val, void **_from);
extern void mload_dword(Uint32 *_val, void **_from);
*/
#else
extern int CMP_CodeRunlen();
extern int CMP_CodeRunlenByte();
extern int CMP_CodeRunlenWord();
extern int CMP_CodeRunlenDword();
extern int CMP_DecSdBlkStd();
extern int CMP_DecSdBlkExt();
/* cmptool.c */
extern char name_tmp[MAXFILENAME];	/* テンポラリファイル名 */
extern int update();
extern int fput_word();
extern int fput_dword();
extern int fget_word();
extern int fget_dword();
extern void swap_word();
extern void swap_dword();
extern void print_rate();
/* cmpltool.c */
extern void *memset_word();
extern void *memset_dword();
extern void *memmove_byte();
/*
extern void mload_word();
extern void mload_dword();
*/
#endif
