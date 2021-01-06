/*	Copyright(c) 1994 SEGA			*/
/*
 *
 * ���k�W�J���[�`���W
 *
 *
 */

#include "smp_bios.h"


/*--------------------------------------------
 *    �X���C�h�����W�J
 *-------------------------------------------*/
void SMDC_sddecomp(Uint8 *src, Uint8 *dst)
{
    Uint8 *index;
    Uint8 flag, mask;
    Uint32 word;
    Uint32 i, j;

    word = 0;
start:
    flag = *src;
    src++;
    mask = 1;

    do {
        if ((flag & mask) == mask) {
            *dst = *src;              /* �r�b�g���P�̂Ƃ��񈳏k */
            dst++;
            src++;
        } else {
            word = *src;              /* �r�b�g���O�̂Ƃ����k   */
            src++;

            i = *src;
            src++;
            j = (i >> 4) + 3;         /* �]���T�C�Y             */
            word += (i << 8);         /* ���΃A�h���X           */

            if (word == 0) return;
            index = dst + (word | 0xfffff000);

            for (; j ; j--) {
                *dst = *index;
                dst++;
                index++;
            }

        }
        mask <<= 1;

    } while (mask != 0);  /* mask�r�b�g���P������܂� */

    goto start;


}

