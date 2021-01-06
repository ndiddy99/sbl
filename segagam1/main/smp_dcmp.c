/*	Copyright(c) 1994 SEGA			*/
/*
 *
 * 圧縮展開ルーチン集
 *
 *
 */

#include "smp_bios.h"


/*--------------------------------------------
 *    スライド辞書展開
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
            *dst = *src;              /* ビットが１のとき非圧縮 */
            dst++;
            src++;
        } else {
            word = *src;              /* ビットが０のとき圧縮   */
            src++;

            i = *src;
            src++;
            j = (i >> 4) + 3;         /* 転送サイズ             */
            word += (i << 8);         /* 相対アドレス           */

            if (word == 0) return;
            index = dst + (word | 0xfffff000);

            for (; j ; j--) {
                *dst = *index;
                dst++;
                index++;
            }

        }
        mask <<= 1;

    } while (mask != 0);  /* maskビットが１周するまで */

    goto start;


}

