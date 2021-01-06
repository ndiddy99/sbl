/*------------------------------------------------------------------------
 *  FILE:	csh_main.c
 *      $Author: $Y.K
 *      $Date: $1994-05-10
 *      $Locker: $
 *      $Revision: $0.90
 *      $Source: $
 *      $State: $
 *
 *	Copyright(c) 1994 SEGA
 *      Copyright (c) by SEGA Enterprises Ltd. 1994. All rights reserved. 
 *
 *  PURPOSE:
 *		CPU Cache Operation function program
 *
 *  DESCRIPTION:
 *
 *
 *  INTERFACE
 *
 *
 *  CAVEATS
 *
 *
 *  AUTHOR(S):
 *		Y.K
 *		
 *  MOD HISTORY:
 *      $Log: $
 * 
 * 
 *------------------------------------------------------------------------
 */
#include "sega_xpt.h"
#include "sega_csh.h"

/****************************************************************************/
/*	�������֐�																*/
/****************************************************************************/

/****************************************************************************/
/*	�L���b�V���̏�����														*/
/****************************************************************************/
void CSH_Init(Uint16 sw)
{
	Uint32 way;									/*	�A�N�Z�X�E�F�C����ϐ�	*/
	Uint32 i;									/*	�A�N�Z�X���C������ϐ�	*/
	Uint32 *adrs;								/*	���ڽ�ڲ�̈�擪���ڽ	*/
	
	CSH_SET_ENABLE(CSH_DISABLE);				/*	�L���b�V���f�B�Z�[�u��	*/
	for (way = 0; way < 4; way++) {				/*	�S�E�F�C���[�v			*/
		CSH_SET_ACS_WAY(way);					/*	�A�N�Z�X�E�F�C�w��		*/
		adrs = (Uint32 *)0x60000000;			/*	���ڽ�ڲ�̈�擪���ڽ	*/
		for (i = 0; i < 64; i++) {				/*	64���C�����[�v			*/
			*adrs = 0;							/*	�P���C���̏��N���A	*/
			adrs += 4;							/*	���̃��C���ɐi��		*/
		}										/*	end for i				*/
	}											/*	end for way				*/
	CSH_SET_WAY_MODE(sw);						/*	�E�F�C���[�h�ݒ�		*/
	CSH_SET_CODE_FILL(CSH_CODE_ENABLE);			/*	�R�[�h�t�B���C�l�[�u��	*/
	CSH_SET_DATA_FILL(CSH_DATA_ENABLE);			/*	�f�[�^�t�B���C�l�[�u��	*/
	CSH_SET_ENABLE(CSH_ENABLE);					/*	�L���b�V���C�l�[�u��	*/
}

/****************************************************************************/
/*	�L���b�V���̑S�N���A													*/
/****************************************************************************/
void CSH_AllClr(void)
{
	Uint32 way;									/*	�A�N�Z�X�E�F�C����ϐ�	*/
	Uint32 i;									/*	�A�N�Z�X���C������ϐ�	*/
	Uint32 *adrs;								/*	���ڽ�ڲ�̈�擪���ڽ	*/
	
	CSH_SET_ENABLE(CSH_DISABLE);				/*	�L���b�V���f�B�Z�[�u��	*/
	for (way = 0; way < 4; way++) {				/*	�S�E�F�C���[�v			*/
		CSH_SET_ACS_WAY(way);					/*	�A�N�Z�X�E�F�C�w��		*/
		adrs = (Uint32 *)0x60000000;			/*	���ڽ�ڲ�̈�擪���ڽ	*/
		for (i = 0; i < 64; i++) {				/*	64���C�����[�v			*/
			*adrs = 0;							/*	�P���C���̏��N���A	*/
			adrs += 4;							/*	���̃��C���ɐi��		*/
		}										/*	end for i				*/
	}											/*	end for way				*/
	CSH_SET_ENABLE(CSH_ENABLE);					/*	�L���b�V���C�l�[�u��	*/
}

/****************************************************************************/
/*	�Ώۗ̈�̘A�z�p�[�W													*/
/****************************************************************************/
void CSH_Purge(void *adrs, Uint32 P_size)
{
	void *ea;									/*	�I���A�h���X			*/

	adrs = (void *)((Uint32)adrs
	 & 0x1ffffff0 | 0x40000000);				/*	�擪������ްı��ڽ�쐬	*/
	ea = (void *)((Uint32)adrs + P_size);		/*	�I���A�h���X�{�P�Z�o	*/
	while ((Uint32)adrs < (Uint32)ea) {			/*	�Ώۃ��C�����J��Ԃ�	*/
		*(Uint32 *)adrs = 0;					/*	�C���o���f�[�g			*/
		adrs = (void *)((Uint32)adrs + 16);		/*	���̏���				*/
	}											/*	end	while				*/
}

/****************************************************************************/
/*	End of File																*/
/****************************************************************************/
