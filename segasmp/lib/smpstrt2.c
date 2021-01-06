/*======================================================================
 *		smpstrt2.c -- �T���v���X�^�[�g���W���[���Q
 *  	Copyright(c) 1994 SEGA
 *  	Written by H.E on 1994-02-15 Ver.0.80
 *  	Updated by M.K on 1994-11-11 Ver.1.10
 *	Note:
 *  	�b���W���[�����s���������������W���[���ňȉ��̃��[�`�����܂ށB
 *  		_INIT		-- �b���W���[�����s�J�n�G���g�����[�`��
 *  		_INITSCT    -- �f�[�^�Z�N�V��������������
 *======================================================================*/
#include <stdlib.h>
#include <machine.h>
#include <sega_xpt.h>

extern Uint8 *_SEGA_D_ROM, *_SEGA_D_BGN, *_SEGA_D_END;
extern Uint8 *_SEGA_B_BGN, *_SEGA_B_END;
extern Uint8 *_D_ROM, *_D_BGN, *_D_END;
extern Uint8 *_B_BGN, *_B_END;

extern void main(void);

void _INITSCT();

void _INIT()
{
	_INITSCT();
    errno = 0;
    main();
    for (;;)
		;
}

void _INITSCT()
{
	Uint8 *p, *q;

	/*  Clear SEGA_B Section */
	for (p = _SEGA_B_BGN; p < _SEGA_B_END; p++)
		*p = 0;

	/* Copy ROM data to RAM */
	for (p = _SEGA_D_BGN, q = _SEGA_D_ROM; p < _SEGA_D_END; p++, q++)
		*p = *q;

	/*  Clear B Section */
	for (p = _B_BGN; p < _B_END; p++)
		*p = 0;

	/* Copy ROM data to RAM */
	for (p = _D_BGN, q = _D_ROM; p < _D_END; p++, q++)
		*p = *q;
}

/*===== End of file ====================================================*/
