/*******************************************************************
*
*                       サターン用データ圧縮
*
*                      Copyright(c) 1994 SEGA
*
*	Comment: 伸張ライブラリ／ランレングス伸張／ＷＯＲＤ単位
*	File   : CMPLRUNW.C
*	Date   : 1994-01-28
*   Author : Y.T
*
*******************************************************************/
#include <stdio.h>
#include <ctype.h>
#include "sega_cmp.h"
#include "cmp.h"

/*    ランレングス伸張／ＷＯＲＤ単位        */
Sint32 CMP_DecRunlenWord(void *_inp, void **_outp, Sint32 outbufsize)
{
	Uint8	*tmp;
	Uint16	*inp, *outp;
	Sint32	orgfsize,		/* 圧縮前ファイルサイズ[byte]			*/
			outsize,		/* 出力データサイズ[byte] 				*/
			boundary_offset,/* 出力データサイズを４バイトバウンダリ	*/
							/* に揃える為に付加したバイト数。		*/
			outcnt,			/* 出力データカウンタ	 				*/
			runlen,         /* ラン長 								*/
			retval;         /* 戻り値								*/
	Uint16	header;			/* 圧縮ファイルヘッダ					*/

	inp = _inp;
	outp = *_outp;
	mload_word(Uint16, header, Uint16 *, inp);
	switch (header & CMP_ORGW_MASK) {
	case CMP_ORGW_WORD:
		mload_word(Sint32, orgfsize, Uint16 *, inp);
		break;
	case CMP_ORGW_DWORD:
		inp++;/* ４バイト境界の為に、２バイトスキップした後にある */
		mload_dword(Sint32, orgfsize, Uint16 *, inp);
		break;
	}
	boundary_offset = orgfsize & CMP_BOUNWORD;
	orgfsize = (((orgfsize+CMP_BOUNWORD)>>1)<<1);
	if (orgfsize > outbufsize) {
		retval = CMP_DEC_STOP;
		outsize = outbufsize;
	} else {
		retval = CMP_DEC_OK;
		outsize = orgfsize;
	}
	for (outcnt = 0; outcnt < outsize;) {
		mload_sword(Sint32, runlen, Uint16 *, inp);
		if (runlen >= 0) {/* 一致系列長 */
			runlen += 2;
			if (outcnt + CMP_BYTEWORD*runlen > outsize) {
				runlen = (outsize - outcnt)/CMP_BYTEWORD;
			}
			memset_word(outp, *inp++, runlen);
		} else {/* 不一致系列長 */
			runlen *= -1;
			if (outcnt + CMP_BYTEWORD*runlen > outsize) {
				runlen = (outsize - outcnt)/CMP_BYTEWORD;
			}
			memmove_byte(outp, inp, CMP_BYTEWORD*runlen);
			inp += runlen;
		}
		outp += runlen;
		outcnt += CMP_BYTEWORD*runlen;
	}
	tmp = (Uint8 *)outp;
	tmp -= boundary_offset;
	*_outp = (void *)tmp;
	return retval;
}
