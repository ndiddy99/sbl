/*-----------------------------------------------------------------------------
 *  FILE: mem_main.c
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
 *          <2> MEM_Calloc()    -   �z��̈�m��
 *          <4> MEM_Malloc()    -   �������u���b�N�m��
 *          <5> MEM_Realloc()   -   �������Ċ���t��
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

/******************************************************************************
 *
 * NAME:    MEM_Free()              - �������u���b�N���
 *
 * PARAMETERS :
 *      (1) void *mem_ptr           - <i>  �������u���b�N�ւ̃|�C���^
 *
 * DESCRIPTION:
 *      �������u���b�N��������܂��D
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

void MEM_Free(void *mem_ptr)
{
    MemHead *cell_hd;
    MemHead *now;
    
    cell_hd = ((MemHead *)mem_ptr) - 1;
    for(now = MEM_empty_top; !((cell_hd > now) && (cell_hd < now->s.next));
                           now = now->s.next){
       if((now >= now->s.next) && ((cell_hd > now) || (cell_hd < now->s.next)))
            break;
    }
    if((cell_hd + cell_hd->s.size) == now->s.next){
        cell_hd->s.size += now->s.next->s.size;
        cell_hd->s.next = now->s.next->s.next;
    }else{
        cell_hd->s.next = now->s.next;
    }
    if((now + now->s.size) == cell_hd){
        now->s.size += cell_hd->s.size;
        now->s.next = cell_hd->s.next;
    }else{
        now->s.next = cell_hd;
    }
    MEM_empty_top = now;
}

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

/******************************************************************************
 *
 * NAME:    MEM_Realloc()           - �������Ċ���t��
 *
 * PARAMETERS :
 *      (1) void *mem_prt           - <i>  �ȑO�̃������u���b�N�ւ̃|�C���^
 *      (2) Uint32 mem_size         - <i>  �V�����������u���b�N�T�C�Y�i�o�C�g�w��j
 *
 * DESCRIPTION:
 *      ���������Ċ���t�����܂��B
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

void *MEM_Realloc(void *mem_ptr, Uint32 mem_size)
{
    Uint32 i;
    Uint8 *work;
    if((work = (Uint8 *)MEM_Malloc(mem_size)) == NULL)
        return(NULL);
    for(i = 0; i < mem_size; i++){
	*(work + i) = *((Uint8 *)mem_ptr + i);
    };
    MEM_Free(mem_ptr);
    return(work);
}

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

