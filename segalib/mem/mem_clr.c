/*-----------------------------------------------------------------------------
 *  FILE: mem_clr.c
 *
 *      Copyright(c) 1994 SEGA.
 *
 *  PURPOSE:
 *
 *      ���������C�u�����\�[�X�t�@�C���B
 *
 *  DESCRIPTION:
 *
 *      �������Ǘ�����������B
 *
 *  INTERFACE:
 *
 *      < FUNCTIONS LIST >
 *          <6> MEM_ClrMemZero  -   �������[���N���A
 *
 *  CAVEATS:
 *
 *      �Ȃ��B
 *
 *  AUTHOR(S)
 *
 *      1994-07-06  N.T Ver.1.00
 *
 *  MOD HISTORY:
 *
 *-----------------------------------------------------------------------------
 */

/*
 * C VIRTUAL TYPES DEFINITIONS
 */
#include <stdio.h>
#include "sega_xpt.h"

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include "mem_main.h"

/*
 * GLOBAL DECLARATIONS
 */

/*
 * LOCAL DEFINES/MACROS
 */

/*
 * TYPEDEFS
 */

/*
 * STATIC DECLARATIONS
 */ 

/*
 * STATIC FUNCTION PROTOTYPE DECLARATIONS
 */

/*****************************************************************************/
/**** �����񋟊֐� ***********************************************************/
/*****************************************************************************/
/******************************************************************************
 *
 * NAME:    MEM_ClrMemZero              - �������[���N���A
 *
 * PARAMETERS :
 *      (1) void * adr              - <i>  �擪�A�h���X
 *      (2) Uint32 byte             - <i>  �o�C�g��
 *
 * DESCRIPTION:
 *      �w�胁�����Ƀ[���𖄂߂܂�.
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
void MEM_ClrMemZero(void *adr, Uint32 all_size)
{
    Uint32 i;
    all_size = (all_size - 1) * 2;
    for(i = 0; i < all_size; i++){
        *((Uint32 *)adr + i) = 0;
    }
}

