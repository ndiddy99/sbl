/*******************************************************************
*
*                       サターン用データ圧縮
*
*                      Copyright(c) 1994 SEGA
*
*	Comment: 伸張ライブラリ／ランレングス
*	File   : CMPLRUN.C
*	Date   : 1994-01-28
*   Author : Y.T
*
*******************************************************************/
#include <stdio.h>
#include <ctype.h>
#include "sega_cmp.h"
#include "cmp.h"

Sint32 CMP_DecRunlen(void *in, void **out, Sint32 size)
{
	Uint8	*_in;
	Uint16	header;			/* 圧縮ファイルヘッダ */

	_in = in;
	mload_word(Uint16, header, Uint8 *, _in);
	switch (header & CMP_UNIT_MASK) {
	case CMP_UNIT_BYTE:	return CMP_DecRunlenByte(in, out, size);
	case CMP_UNIT_WORD:	return CMP_DecRunlenWord(in, out, size);
	case CMP_UNIT_DWORD:return CMP_DecRunlenDword(in, out, size);
	default:			return CMP_DEC_ERR_H_UNIT;
	}
	return CMP_DEC_ERR;
}
