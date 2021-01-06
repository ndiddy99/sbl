/*******************************************************************
*
*                       �T�^�[���p�f�[�^���k
*
*                      Copyright(c) 1994 SEGA
*
*	Comment: �L�����C�u�����^���������O�X�L���^�v�n�q�c�P��
*	File   : CMPLRUNW.C
*	Date   : 1994-01-28
*   Author : Y.T
*
*******************************************************************/
#include <stdio.h>
#include <ctype.h>
#include "sega_cmp.h"
#include "cmp.h"

/*    ���������O�X�L���^�v�n�q�c�P��        */
Sint32 CMP_DecRunlenWord(void *_inp, void **_outp, Sint32 outbufsize)
{
	Uint8	*tmp;
	Uint16	*inp, *outp;
	Sint32	orgfsize,		/* ���k�O�t�@�C���T�C�Y[byte]			*/
			outsize,		/* �o�̓f�[�^�T�C�Y[byte] 				*/
			boundary_offset,/* �o�̓f�[�^�T�C�Y���S�o�C�g�o�E���_��	*/
							/* �ɑ�����ׂɕt�������o�C�g���B		*/
			outcnt,			/* �o�̓f�[�^�J�E���^	 				*/
			runlen,         /* ������ 								*/
			retval;         /* �߂�l								*/
	Uint16	header;			/* ���k�t�@�C���w�b�_					*/

	inp = _inp;
	outp = *_outp;
	mload_word(Uint16, header, Uint16 *, inp);
	switch (header & CMP_ORGW_MASK) {
	case CMP_ORGW_WORD:
		mload_word(Sint32, orgfsize, Uint16 *, inp);
		break;
	case CMP_ORGW_DWORD:
		inp++;/* �S�o�C�g���E�ׂ̈ɁA�Q�o�C�g�X�L�b�v������ɂ��� */
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
		if (runlen >= 0) {/* ��v�n�� */
			runlen += 2;
			if (outcnt + CMP_BYTEWORD*runlen > outsize) {
				runlen = (outsize - outcnt)/CMP_BYTEWORD;
			}
			memset_word(outp, *inp++, runlen);
		} else {/* �s��v�n�� */
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
