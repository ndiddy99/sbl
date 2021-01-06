/*	Copyright(c) 1994 SEGA			*/
/*
 *
 * Task & Action Control Header File
 *
 */

#ifndef __SMP_TASK_H
#define __SMP_TASK_H


/*---- CONST ----*/
#define	ACTWKMAX	0x80		/* ACTION WORK KOSUU  */
#define ACTWKSIZE       0x80            /* 1 ACTION WORK SIZE */



/* ---- ACTION WORK ASSIGN MACRO ---- */
#define SMTA_DefActWk(_actname_, _member_)                            \
typedef struct _actname_ {                                            \
    Uint8 actid;                       /* action id           1 0  */ \
    Uint8 level;                       /* action level        1 1  */ \
    Uint8 mode;                        /* mode number         1 2  */ \
    Uint8 status;                      /* action status       1 3  */ \
    void (*pcbuff)(struct _actname_*); /* action exec address 4 4  */ \
    struct _actname_ *msttbl;          /* master table ptr    4 8  */ \
    struct _actname_ *subtbl;          /* sub table ptr       4 12 */ \
    _member_                                                          \
  } _actname_


/*
 *    �_�~�[�A�N�V�������[�N
 */
SMTA_DefActWk(DUMMYACT,
         Uint8 akiwk;
);

/*
 *    �Œ�G���A�v�Z�p 
 */
SMTA_DefActWk(ACTWK,
         Uint8 free[ACTWKSIZE - sizeof(DUMMYACT) + 1];
);


/*
 *    �A�N�V�������m�ۂł������`�F�b�N����}�N��
 */
#define SMTA_ChkAction(_actwk_ptr_) ((_actwk_ptr_) != SMTA_DummyActwk)

/*
 *    �g�p�A�N�V�������[�N�̉��
 */
#define SMTA_KillAction(_ptr_) ((_ptr_)->actid = 0)


/*
 *    �O���[�o���ϐ�
 */    
GLOBAL ACTWK SMTA_ActionBuf[ACTWKMAX];
GLOBAL ACTWK SMTA_DummyActwk;


/*
 *    �֐��v���g�^�C�v
 */
extern void SMTA_ActWkInit(void);
extern void *SMTA_MakeAction(void *exeadd);
extern void* SMTA_MakeActionX(void *exeadd, Uint8 start, Uint8 count);
extern void* SMTA_SetAction(void *exeadd, Uint8 start);
extern void SMTA_ActionLoop(void);


#endif  /* endif __SMP_TASK_H */




