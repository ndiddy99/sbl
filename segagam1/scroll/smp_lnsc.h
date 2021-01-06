/*	Copyright(c) 1994 SEGA			*/
/*
 *
 *   �����C���X�N���[��
 *
 */


#ifndef __SMP_LNSC_H
#define __SMP_LNSC_H

#include "smp_def.h"


/*
 *     �A�N�V�������[�N�\����
 */
SMTA_DefActWk(LNSCACT,                /* ACTWK SYSTEM  0x10 bytes */
         SPRITE sprite;               /* �X�v���C�g�\��0x50 bytes */
         Sint16 hlwk[3];              /* hline work    0x06 bytes */
         Sint16 hzwk[3];              /* hzoom work    0x06 bytes */
         Sint16 vcwk[3];              /* vcell work    0x06 bytes */
         Sint16 vswk[3];              /* vscrl work    0x06 bytes */
         Sint16 exeno;
);



/*
 *     �֐��v���g�^�C�v
 */
void SMWI_WindMode(void);



#endif

