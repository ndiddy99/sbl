/*-----------------------------------------------------------------------------
 *  FILE: smpper1.c
 *
 *      Copyright (c) by SEGA.
 *
 *  PURPOSE:
 *
 *      SMPC�̃V�X�e���}�l�[�W�����g�n�T���v���v���O����
 *
 *  DESCRIPTION:
 *
 *      SMPC�̃V�X�e���}�l�[�W�����g�n�̃R�}���h�����s���܂��B
 *
 *      *(Uint32 *)0x6050000�Ɏ��s�������R�}���h�̔ԍ�����͂��Ă��������B���A
 *      �R�}���h�ɓ��͂��K�v�ȏꍇ�́A*(Uint8 *)0x6050030������͂��Ă��������B
 *      �o�͂�*(Uint8 *)0x6050010�ɏ������݂܂��B
 *
 *          �R�}���h�ԍ� -> *(Uint32 *)0x6050000
 *          ����         -> *(Uint32 *)0x6050030
 *          �o��         -> *(Uint32 *)0x6050010
 *
 *          +---------------------------------------+
 *          |�R�}���h�ԍ�    �R�}���h����           |
 *          +---------------------------------------+
 *          |        0x01    �}�X�^SH2 ON           |
 *          |        0x02    �X���[�uSH2 ON         |
 *          |        0x03    �X���[�uSH2 OFF        |
 *          |        0x04    �T�E���hON             |
 *          |        0x05    �T�E���hOFF            |
 *          |        0x06    CD ON                  |
 *          |        0x07    CD OFF                 |
 *          |        0x08    �V�X�e���S�̃��Z�b�g   |
 *          |        0x09    NMI���N�G�X�g          |
 *          |        0x0a    �z�b�g���Z�b�g�Ȱ���   |
 *          |        0x0b    �z�b�g���Z�b�g�ި����� |
 *          |        0x0c    SMPC�������ݒ�         |
 *          |        0x0d    �����ݒ�               |
 *          +---------------------------------------+
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *
 *  CAVEATS:
 *
 *  AUTHOR(S)
 *
 *      1994-08-11  N.T Ver.1.00
 *
 *  MOD HISTORY:
 *
 *-----------------------------------------------------------------------------
 */

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
#include "sega_xpt.h"
#include <machine.h>
#include <string.h>

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include "sega_per.h"

/*
 * GLOBAL DECLARATIONS
 */

/*
 * LOCAL DEFINES/MACROS
 */

/*
 * STATIC DECLARATIONS
 */

/*
 * STATIC FUNCTION PROTOTYPE DECLARATIONS
 */

/******************************************************************************
 *
 * NAME:    main()      - ���C��
 *
 * PARAMETERS :
 *      �Ȃ�
 *
 * DESCRIPTION:
 *      ���C��
 *
 * PRECONDITIONS:
 *      �Ȃ��B
 *
 * POSTCONDITIONS:
 *      �Ȃ�
 *
 * CAVEATS:
 *      �Ȃ��B
 *
 ******************************************************************************
 */

void main(void)
{
    Uint8 ireg[32];
    Uint8 oreg[32];
    Uint32 i;

    /** BEGIN ****************************************************************/
    /*
     *  process 1
     */
    
    for(i = 0; i < 32; i++){
        *((Uint8 *)0x6050010 + i) = 0x00;
    }
    
    switch((*(Uint32 *)0x6050000)){
        case  1:
            PER_SMPC_MSH_ON();
            break;
        case  2:
            PER_SMPC_SSH_ON();
            break;
        case  3:
            PER_SMPC_SSH_OFF();
            break;
        case  4:
            PER_SMPC_SND_ON();
            break;
        case  5:
            PER_SMPC_SND_OFF();
            break;
        case  6:
            PER_SMPC_CD_ON();
            break;
        case  7:
            PER_SMPC_CD_OFF();
            break;
        case  8:
            PER_SMPC_SYS_RES();
            break;
        case  9:
            PER_SMPC_NMI_REQ();
            break;
        case  0xa:
            PER_SMPC_RES_ENA();
            break;
        case  0xb:
            PER_SMPC_RES_DIS();
            break;
        case  0xc:
            ireg[0] = *(Uint8 *)0x6050030;
            ireg[1] = *(Uint8 *)0x6050031;
            ireg[2] = *(Uint8 *)0x6050032;
            ireg[3] = *(Uint8 *)0x6050033;
            PER_SMPC_SET_SM(*(Uint32 *)ireg);
            break;
        case  0xd:
            for(i = 0; i < 7; i++){
                ireg[i] = *((Uint8 *)0x6050030 + i);
            }
            PER_SMPC_SET_TIM(ireg);
            break;
    }

    while(1)
    {
    }
}
