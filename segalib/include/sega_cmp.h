/*******************************************************************
*
*                　　　 サターン用データ圧縮
*
*                      Copyright(c) 1994 SEGA
*
*   Comment: 伸張ライブラリ／利用者用ヘッダ
*   File   : SEGA_CMP.H
*   Date   : 1994-01-18
*   Author : Y.T
*
*******************************************************************/
#ifndef SEGA_CMP2_H
#ifndef SEGA_CMP_H
#define SEGA_CMP_H

/* 基本的なデータ形などを定義 */
#include "sega_xpt.h"


/*******************************************************************
      定数マクロ
*******************************************************************/

/* 伸張ライブラリ関数の戻り値。（全てのライブラリ関数に共通） 	*/
#define CMP_DEC_OK 			(0L)		/* 正常終了 			*/
#define CMP_DEC_STOP 		(1L)		/* 伸張処理中断 		*/
#define CMP_DEC_ERR 		(-1L)		/* 異常終了 			*/
										/* 不当ヘッダ			*/
#define CMP_DEC_ERR_H_ALGO 	(-2L)		/* 　未対応アルゴリズム	*/
#define CMP_DEC_ERR_H_UNIT 	(-3L)		/* 　未対応処理単位		*/


/*******************************************************************
      プロトタイプ宣言
*******************************************************************/

/*    ランレングス伸張        */
#ifndef COMPILER_CC
extern Sint32 CMP_DecRunlen(void *in, void **out, Sint32 size);
#else
extern Sint32 CMP_DecRunlen();
#endif

/*    ランレングス伸張／ＤＷＯＲＤ        */
#ifndef COMPILER_CC
extern Sint32 CMP_DecRunlenDword(void *in, void **out, Sint32 size);
#else
extern Sint32 CMP_DecRunlenDword();
#endif

/*    ランレングス伸張／ＷＯＲＤ単位        */
#ifndef COMPILER_CC
extern Sint32 CMP_DecRunlenWord(void *in, void **out, Sint32 size);
#else
extern Sint32 CMP_DecRunlenWord();
#endif

/*    ランレングス伸張／ＢＹＴＥ単位        */
#ifndef COMPILER_CC
extern Sint32 CMP_DecRunlenByte(void *in, void **out, Sint32 size);
#else
extern Sint32 CMP_DecRunlenByte();
#endif

#endif	/* SEGA_CMP_H */
#endif	/* SEGA_CMP2_H */
