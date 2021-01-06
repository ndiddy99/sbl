/*******************************************************************
*
*                       �T�^�[���p�f�[�^���k
*
*                      Copyright(c) 1994 SEGA
*
*   Comment: �L�����C�u�����^���܂��܂����G�p
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

/*�������E�u���b�N�̂ЂƂ̒l�ł̐ݒ�B�Q�o�C�g�P�ʁB
 * s = memset_word(to, a, n);	
 * void *s;			���^�[���E�|�C���^�ito��Ԃ��j
 * void *to;		�ړI�̃|�C���^
 * Uint16 a;		�����̒l�i�Q�o�C�g�^�P�����j
 * size_t n;		�������i�o�C�g���^�Q�j
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

/*�������E�u���b�N�̂ЂƂ̒l�ł̐ݒ�B�S�o�C�g�P�ʁB
 * s = memset_dword(to, a, n);
 * void *s;			���^�[���E�|�C���^�ito��Ԃ��j
 * void *to;		�ړI�̃|�C���^
 * Uint32 a;		�����̒l�i�S�o�C�g�^�P�����j
 * size_t n;		�������i�o�C�g���^�S�j
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

/* �o�C�g�P�ʂ̃������f�[�^�R�s�[
 * SH-C�� memcpy() �ł́A���̏ꍇ�ɈӐ}�ǂ���ɓ��삵�Ȃ��B
 * �@�@to,from �̗̈悪�d�Ȃ�A���A
 *     to,from ���S�o�C�g���E���炸��Ă���B
 */
void *memmove_byte(void *_to, void *_from, size_t n)
{
	char *to, *from;

	for (to = (char *)_to, from = (char *)_from; n > 0; n--) {
		*to++ = *from++;
	}
	return _to;
}

/* ��������ɂ���A���g���[���`���̃f�[�^���AUint16 �ϐ��ɐݒ肷��B
 * �\�[�X���̃|�C���^ *from ���Q�o�C�g�i�߂ĕԂ��B
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
