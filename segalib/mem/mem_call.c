/*-----------------------------------------------------------------------------
 *  FILE: mem_call.c
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
 *          <2> MEM_Calloc()    -   �z��̈�m��
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
 * NAME:    MEM_Calloc()            - �z��̈�m��
 *
 * PARAMETERS :
 *      (1) Uint32 arg_num          - <i>  �z��v�f��
 *      (2) Uint32 arg_size         - <i>  �z��v�f���̃o�C�g��
 *
 * DESCRIPTION:
 *      �z��̈���m�ۂ��܂��D
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

void *MEM_Calloc(Uint32 arg_num, Uint32 arg_size)
{
    MemHead *now;
    MemHead *before;
    
    Uint32 all_size;
    /** BEGIN ****************************************************************/
    all_size = ((arg_size * arg_num) + sizeof(MemHead) - 1) /
                                              sizeof(MemHead) + 1;
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
            MEM_ClrMemZero((now + 1),all_size);
            return(void *)(now + 1);
        }
        if(now == MEM_empty_top){
            now = MEM_empty_top;
            return NULL;
        }
    }
}
