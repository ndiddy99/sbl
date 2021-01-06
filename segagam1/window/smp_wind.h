/*	Copyright(c) 1994 SEGA			*/
/*
 *
 *   �E�B���h�E�@�\���g�����T���v��
 *
 */


#ifndef __SMP_WIND_H
#define __SMP_WIND_H


/*
 *     �A�N�V�������[�N�\����
 */
SMTA_DefActWk(WINDACT,                     /* ACTWK SYSTEM  0x10 bytes */
         SPRITE sprite;
         Sint16 wreg[4];                    /* etc work      0x08 bytes */
         Sint16 wcnt[4];                    /* etc work      0x08 bytes */
);


SMTA_DefActWk(NBWINDACT,                    /* ACTWK SYSTEM  0x10 bytes */
         Sint32 W0posi[2];      /* ���O�pXY�|�W�V���� */
         Sint16 W0speed[2];     /* ���O�pXY�X�s�[�h   */
         Sint16 W0size[2];      /* ���O�pXY�T�C�Y     */
         Sint32 W1posi[2];      /* ���P�pXY�|�W�V���� */
         Sint16 W1speed[2];     /* ���P�pXY�X�s�[�h   */
         Sint16 W1size[2];      /* ���P�pXY�T�C�Y     */
         Sint16 wcnt[4];        /* �ėp�J�E���^       */
);


/*
 *     �֐��v���g�^�C�v
 */
void SMWI_WindMode(void);



#endif

