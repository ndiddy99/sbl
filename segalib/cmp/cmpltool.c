/*******************************************************************
*
*                       サターン用データ圧縮
*
*                      Copyright(c) 1994 SEGA
*
*   Comment: 伸張ライブラリ／こまごました雑用
*   File   : CMPLTOOL.C
*   Date   : 1994-01-28
*   Author : Y.T
*
*******************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include "sega_cmp.h"
#include "cmp.h"

/*メモリ・ブロックのひとつの値での設定。２バイト単位。
 * s = memset_word(to, a, n);	
 * void *s;			リターン・ポインタ（toを返す）
 * void *to;		目的のポインタ
 * Uint16 a;		文字の値（２バイト／１文字）
 * size_t n;		文字数（バイト数／２）
 */
void *memset_word(void *to, Uint16 a, size_t n)
{
	Uint16 *p;

	p = (Uint16 *)to;
	for (; n > 0; n--) {
		*p++ = a;
	}
	return to;
}

/*メモリ・ブロックのひとつの値での設定。４バイト単位。
 * s = memset_dword(to, a, n);
 * void *s;			リターン・ポインタ（toを返す）
 * void *to;		目的のポインタ
 * Uint32 a;		文字の値（４バイト／１文字）
 * size_t n;		文字数（バイト数／４）
 */
void *memset_dword(void *to, Uint32 a, size_t n)
{
	Uint32 *p;

	p = (Uint32 *)to;
	for (; n > 0; n--) {
		*p++ = a;
	}
	return to;
}

/* バイト単位のメモリデータコピー
 * SH-Cの memcpy() では、次の場合に意図どうりに動作しない。
 * 　　to,from の領域が重なり、かつ、
 *     to,from が４バイト境界からずれている。
 */
void *memmove_byte(void *_to, void *_from, size_t n)
{
	char *to, *from;

	for (to = (char *)_to, from = (char *)_from; n > 0; n--) {
		*to++ = *from++;
	}
	return _to;
}

/* メモリ上にある、モトローラ形式のデータを、Uint16 変数に設定する。
 * ソース側のポインタ *from を２バイト進めて返す。
 */
/*
void mload_word(Uint16 *_val, void **_from)
{
	char 	*from;
	Uint16 	val;

	from = (char *)_from;
	val = *from++;
	val = (val << 8) | *from++;
	*_val = val;
	*_from = (void *)from;
}

void mload_dword(Uint32 *_val, void **_from)
{
	char 	*from;
	Uint32 	val;

	from = (char *)_from;
	val = *from++;
	val = (val << 8) | *from++;
	val = (val << 8) | *from++;
	val = (val << 8) | *from++;
	*_val = val;
	*_from = (void *)from;
}
*/
