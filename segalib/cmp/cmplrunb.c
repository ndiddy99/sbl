/*******************************************************************
*
*                       �T�^�[���p�f�[�^���k
*
*                      Copyright(c) 1994 SEGA
*
*	Comment: �L�����C�u�����^���������O�X�L���^�a�x�s�d�P��
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

/*    ���������O�X�L���^�a�x�s�d�P��        */
Sint32 CMP_DecRunlenByte(void *_inp, void **_outp, Sint32 outbufsize)
{
	char	*inp, *outp;
	Sint32	orgfsize,		/* ���k�O�t�@�C���T�C�Y[byte]	*/
			outsize,		/* �o�̓f�[�^�T�C�Y[byte] 		*/
			outcnt,			/* �o�̓f�[�^�J�E���^	 		*/
			runlen,         /* ������ 						*/
			retval;         /* �߂�l						*/
	Uint16	header;			/* ���k�t�@�C���w�b�_	 		*/

	inp = _inp;
	outp = *_outp;
	mload_word(Uint16, header, char *, inp);
	switch (header & CMP_ORGW_MASK) {
	case CMP_ORGW_WORD:
		mload_word(Sint32, orgfsize, char *, inp);
		break;
	case CMP_ORGW_DWORD:
		inp += 2;/* �S�o�C�g���E�ׂ̈ɁA�Q�o�C�g�X�L�b�v������ɂ��� */
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
		if (runlen >= 0) {/* ��v�n�� */
			runlen += 2;
			if (outcnt + runlen > outsize) {
				runlen = outsize - outcnt;
			}
			memset(outp, *inp++, runlen);
		} else {/* �s��v�n�� */
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
