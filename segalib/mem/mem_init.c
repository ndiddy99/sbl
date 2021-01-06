/*-----------------------------------------------------------------------------
 *  FILE: mem_init.c
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
 *          <1> MEM_Init()      -   �������Ǘ��̈�ݒ�
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

MemHead *MEM_empty_top;                  /* �󂫃Z���̐擪            */

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

/******************************************************************************
 *
 * NAME:    MEM_Init()              - �������Ǘ��̈�ݒ�
 *
 * PARAMETERS :
 *      (1) Uint32 top_address      - <i>  �������Ǘ��̈�擪�A�h���X
 *      (2) Uint32 mem_size         - <i>  �������Ǘ��̈�T�C�Y
 *
 * DESCRIPTION:
 *      �������Ǘ��̈��ݒ肵�܂�.
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

void MEM_Init(Uint32 top_address, Uint32 mem_size)
{
    (*(MemHead *)top_address).s.next = (MemHead *)top_address;
    MEM_empty_top = (MemHead *)top_address;
    (*(MemHead *)top_address).s.size = mem_size / sizeof(MemHead);
}
