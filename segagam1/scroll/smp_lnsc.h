/*	Copyright(c) 1994 SEGA			*/
/*
 *
 *   横ラインスクロール
 *
 */


#ifndef __SMP_LNSC_H
#define __SMP_LNSC_H

#include "smp_def.h"


/*
 *     アクションワーク構造体
 */
SMTA_DefActWk(LNSCACT,                /* ACTWK SYSTEM  0x10 bytes */
         SPRITE sprite;               /* スプライト表示0x50 bytes */
         Sint16 hlwk[3];              /* hline work    0x06 bytes */
         Sint16 hzwk[3];              /* hzoom work    0x06 bytes */
         Sint16 vcwk[3];              /* vcell work    0x06 bytes */
         Sint16 vswk[3];              /* vscrl work    0x06 bytes */
         Sint16 exeno;
);



/*
 *     関数プロトタイプ
 */
void SMWI_WindMode(void);



#endif

