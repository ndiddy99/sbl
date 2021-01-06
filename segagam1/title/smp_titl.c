/*	Copyright(c) 1994 SEGA			*/

/*
 *
 *     TITLE MODE
 *
 */

#include "smp_bios.h"
#include "smp_titl.h"


/*
 *    �֐��v���g�^�C�v
 */
void SMTI_TitleAct(TITLEACT *ix);
void SMTI_setmosaic(Uint16 msrate);

/*
 *    �萔��`
 */
const Uint8 disp_title01[] = "---- SAMPLE PROGRAM ----";
const Uint8 disp_title02[] = "PRESS START BUTTON";
const Uint8 disp_title03[] = "(C) 1994 SEGA";
const Uint8 disp_title04[] = "VER.1.00 ";

/*==========================================================================*/

/*
 *
 *  TITLE MAIN
 *
 */
void SMTI_Title(void)
{
    enum {
        INIT,
        MAIN,
        NEXT
      };


    while (SMMA_MainMode == TITLE_MODE) {
        /* �X�v���C�g�\������ */
        SMV1_SprCmdStart();

        switch (SMMA_MainLevel) {
            case INIT:
                SMV2_TvOff();
                SMMA_initall();

                /* �v���C�I���e�B�̃Z�b�g */
                SCL_SetPriority(SCL_SP0, 7);
                SCL_SetPriority(SCL_NBG0, 6);
                SCL_SetPriority(SCL_RBG0, 5);

                /* �J���[���[�h�̐ݒ� */
                SCL_SetColRamMode(SCL_CRM15_2048); /* 15bit 2048 color mode */

                /* �X�v���C�g�t���[���o�b�t�@�̃N���A */
                SMV2_TvOff();
                SMV1_SprCmdStart();
                SMV1_SprCmdEnd();
                SMMA_intwait();
                SMMA_intwait();

                SMV2_FilMem16((Uint16 *)SCR_VRAMA1,
                        (0x40 * 0x40 / sizeof (Uint16)), 0x0000);

                /* �^�C�g���A�N�V�����Z�b�g */
                SMTA_MakeAction(SMTI_TitleAct);

                /* �J���[�Z�b�g */
                poke_w(COLOR_RAM + 0, 0x7000);
                poke_w(COLOR_RAM + 2, 0x739c);

                SMV2_TvOn();
                SMMA_MainLevel++;
                break;

            case MAIN:
                /* �A�N�V�������s */
                SMTA_ActionLoop();
                break;

            case NEXT:
                /* �����u�d�̋����I�� */
                SCL_AbortAutoVe();

                SMMA_MainMode = SELECT_MODE;
                SMMA_MainLevel = 0;
                break;
	}
        /* �X�v���C�g�\���I���R�}���h�Z�b�g */
        SMV1_SprCmdEnd();

        /* V-BLANK�C���^���v�g�҂�*/
        SMMA_intwait();
    }
}


/*
 *
 *  TITLE ACTION
 *
 */
void SMTI_TitleAct(TITLEACT *ix)
{

    enum {
        TITL_INIT,
        TITL_IN,
        TITL_DISP1,
        TITL_DISP2,
        TITL_OUT,
        TITL_NEXT
	};

    Uint16 swdata;
    SclRgb *start;
    SclRgb *end;
    Fixed32 sx,sy;

    swdata = SMPA_pad_data1;

    switch (ix->mode) {
        case TITL_INIT:
            ix->wcnt[0] = 0x10;                /* MOZAIC INITIAL HV COUNT */
            ix->wcnt[1] = 0x30;                /* COLOR OFFSET COUNT */
            ix->lcnt[0] = 1;                   /* SCALE CONTROL COUNT */
            SMV2_SetFont8((Uint8 *)NBG_PTGEN, 1, 0);
            SMV2_PutAsc8((Uint16 *)(NBG0_PTNAMA + 8 * 2 + 4 * 0x80),
                    disp_title01);    /* TITLE */

            SMV2_PutAsc8((Uint16 *)(NBG0_PTNAMA + 22 * 2 + 6 * 0x80),
                    disp_title04);    /* VER */

            SMV2_PutAsc8((Uint16 *)(NBG0_PTNAMA + 22 * 2 + 26 * 0x80),
                    disp_title03);    /* SEGA */

            /* �J���[�I�t�Z�b�g���Z�b�g */
            SCL_SetColOffset(SCL_OFFSET_A, SCL_NBG1, -0x100, -0x100, -0x100);
            ix->mode++;
            /* NO BREAK */

        case TITL_IN:
            if (--ix->wcnt[0] == 0) {
                SMV2_PutAsc8((Uint16 *)(NBG1_PTNAMA + 11 * 2 + 16 * 0x80),
                        disp_title02);    /* START */

                ix->mode++;
            }
            SMTI_setmosaic(ix->wcnt[0]);
            break;

        case TITL_DISP1:
            if (swdata & PAD_START) {          /* PUSH START */
                ix->mode = TITL_OUT;
                ix->wcnt[0] = 0;
            }
            else if (--ix->wcnt[1] == 0) {
                ix->mode++;
                ix->wcnt[1] = 0x30;

                /* �����J���[���Z���Z�b�g */
                SCL_SetAutoColMix(SCL_NBG1, 1, 0x80, 0x01, 0x00);
                break;
            }

            /* �J���[�I�t�Z�b�g���Z�b�g */
            SCL_IncColOffset(SCL_OFFSET_A, 8,8,8);
            break;

        case TITL_DISP2:
            if (swdata & PAD_START) {          /* PUSH START */
                ix->mode = TITL_OUT;
                ix->wcnt[0] = 0;
            }
            else if (--ix->wcnt[1] == 0) {
                ix->mode--;
                ix->wcnt[1] = 0x30;
                break;
            }

            /* �J���[�I�t�Z�b�g���Z�b�g */
            SCL_IncColOffset(SCL_OFFSET_A, -8,-8,-8);
            break;

        case TITL_OUT:
            if (++ix->wcnt[0] > 0x0e) {
                ix->mode++;
            }
            SMTI_setmosaic(ix->wcnt[0]);

            SCL_SetColOffset(SCL_OFFSET_A, SCL_NBG0 | SCL_NBG1,
                                           (0x200-ix->wcnt[0]*0x10),
                                              (0x200-ix->wcnt[0]*0x10),
                                                    (0x200-ix->wcnt[0]*0x10));
                          

            break;

        case TITL_NEXT:
            SMMA_MainLevel++;
            ix->mode = 0;
            break;
    }
}


/*
 *
 *    MOZAIC REGISTER SET
 *
 */
void SMTI_setmosaic(Uint16 msrate)
{
    msrate = msrate & 0xf;
    SCL_SetMosaic(SCL_NBG0 | SCL_NBG1, msrate, 1);

}


