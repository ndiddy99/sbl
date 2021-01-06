/*******************************************************************
*
*                       サターン用データ圧縮
*
*                      Copyright(c) 1994 SEGA
*
*	Comment: 伸張ライブラリ／ランレングス伸張／ＢＹＴＥ単位
*	File   : CMPLRUNB.C
*	Date   : 1994-01-28
*   Author : Y.T
*
*******************************************************************/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "sega_cmp.h"
#include "cmp.h"

/*    ランレングス伸張／ＢＹＴＥ単位        */
Sint32 CMP_DecRunlenByte(void *_inp, void **_outp, Sint32 outbufsize)
{
	char	*inp, *outp;
	Sint32	orgfsize,		/* 圧縮前ファイルサイズ[byte]	*/
			outsize,		/* 出力データサイズ[byte] 		*/
			outcnt,			/* 出力データカウンタ	 		*/
			runlen,         /* ラン長 						*/
			retval;         /* 戻り値						*/
	Uint16	header;			/* 圧縮ファイルヘッダ	 		*/

	inp = _inp;
	outp = *_outp;
	mload_word(Uint16, header, char *, inp);
	switch (header & CMP_ORGW_MASK) {
	case CMP_ORGW_WORD:
		mload_word(Sint32, orgfsize, char *, inp);
		break;
	case CMP_ORGW_DWORD:
		inp += 2;/* ４バイト境界の為に、２バイトスキップした後にある */
		mload_dword(Sint32, orgfsize, char *, inp);
		break;
	}
	if (orgfsize > outbufsize) {
		retval = CMP_DEC_STOP;
		outsize = outbufsize;
	} else {
		retval = CMP_DEC_OK;
		outsize = orgfsize;
	}
	for (outcnt = 0; outcnt < outsize;) {
		runlen = *(Sint8 *)inp++;
		if (runlen >= 0) {/* 一致系列長 */
			runlen += 2;
			if (outcnt + runlen > outsize) {
				runlen = outsize - outcnt;
			}
			memset(outp, *inp++, runlen);
		} else {/* 不一致系列長 */
			runlen *= -1;
			if (outcnt + runlen > outsize) {
				runlen = outsize - outcnt;
			}
			memmove_byte(outp, inp, runlen);
			inp += runlen;
		}
		outp += runlen;
		outcnt += runlen;
	}
	*_outp = outp;
	return retval;
}
