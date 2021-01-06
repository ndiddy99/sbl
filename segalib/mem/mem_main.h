/*-----------------------------------------------------------------------------
 *  FILE: mem_main.h
 *
 *      Copyright(c) 1994 SEGA.
 *
 *  PURPOSE:
 *
 *      ���������C�u�����w�b�_�t�@�C���B
 *
 *  DESCRIPTION:
 *
 *      �������Ǘ�����������B
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
#include "sega_xpt.h"

/*
 * USER SUPPLIED INCLUDE FILES
 */

#ifndef MEM_MAIN_H
#define MEM_MAIN_H

/*
 * GLOBAL DEFINES/MACROS DEFINES
 */

/*
 * STRUCTURE DECLARATIONS
 */

/*
 * TYPEDEFS
 */
typedef double MemAlign;                        /* 64�r�b�g�̃A���C�����g    */
union mem_head {                                /* �Z���̃w�b�_              */
    struct {
        union mem_head *next;                   /* ���̃Z���|�C���^          */
        unsigned size;                          /* �Z���̃T�C�Y              */
    }s;
    MemAlign damy;
};

typedef union mem_head MemHead;                 /* �Z���w�b�_�f�[�^�^        */

/*
 * EXTERNAL VARIABLE DECLARATIONS
 */
extern MemHead *MEM_empty_top;                  /* �󂫃Z���̐擪            */

/*
 * EXTERNAL FUNCTION PROTOTYPE  DECLARATIONS
 */
void MEM_Init(Uint32, Uint32);
void *MEM_Calloc(Uint32, Uint32);
void *MEM_Malloc(Uint32);
void MEM_Free(void *);
void MEM_ClrMemZero(void *, Uint32);

#endif  /* ifndef MEM_MAIN_H */
