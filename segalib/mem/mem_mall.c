/*-----------------------------------------------------------------------------
 *  FILE: mem_mall.c
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
 *          <4> MEM_Malloc()    -   �������u���b�N�m��
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

/******************************************************************************
 *
 * NAME:    MEM_Malloc()            - �������u���b�N�m��
 *
 * PARAMETERS :
 *      (1) Uint32 mem_size         - <i>  �v���������u���b�N�T�C�Y(�o�C�g�P��)
 *
 * DESCRIPTION:
 *      �v�����������m�ۂ��܂��D
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

void *MEM_Malloc(Uint32 mem_size)
{
    MemHead *now;
    MemHead *before;
    
    unsigned all_size;
    /** BEGIN ****************************************************************/
    all_size = (mem_size + sizeof(MemHead) - 1) / sizeof(MemHead) + 1;
    before = MEM_empty_top;
    for(now = before->s.next; ; before = now, now = now->s.next){
        if(now->s.size >= all_size){
            if(now->s.size == all_size)
                before->s.next = now->s.next;
            else{
                now->s.size -= all_size;
                now += now->s.size;
                now->s.size = all_size;
            }
            MEM_empty_top = before;
            return(void *)(now + 1);
        }
        if(now == MEM_empty_top){
            now = MEM_empty_top;
            return NULL;
        }
    }
}
