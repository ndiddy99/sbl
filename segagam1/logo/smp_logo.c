/*	Copyright(c) 1994 SEGA			*/
/*
 *
 *     SEGA LOGO MODE
 *
 */

#include "smp_bios.h"

#include "smp_lgcg.h"
#include "smp_logo.h"


/*
 *    �֐��v���g�^�C�v
 */
void SMLO_segalogo_cgset(void);
void SMLO_LogoAct(LOGOACT *ix);
void SMLO_color_change(Uint16 *clradd);

/*
 *    �X�v���C�g �L�����N�^�i���o �A�T�C��
 */
#define SMLO_logocharNo 0x50



/* DUMMY DATA */
const Uint8 SMLO_title[] = "SEGA LOGO";

/*
 *    ����, CharNo, �g�u���], �g�T�C�Y, �u�T�C�Y, �g�I�t�Z�b�g, �u�I�t�Z�b�g
 */
const Sint16 SMLO_logo_pat0[] = {
        1, SMLO_logocharNo, 0, 0x0060, 0x0020, -0x30, -0x10,
    };
Sint16 * const SMLO_logo_pattbl[] = {
        SMLO_logo_pat0,
    };



/*==========================================================================*/

/*
 *
 *  SEGA LOGO MAIN
 *
 */
void SMLO_SegaLogo(void)
{
    enum {
        INIT,
        MAIN,
        NEXT
      };


    while (SMMA_MainMode == LOGO_MODE) {
        /* �X�v���C�g�\������ */
        SMV1_SprCmdStart();

        switch (SMMA_MainLevel) {
            case INIT:
                SMV2_TvOff();
                SMMA_initall();

                /* �X�v���C�g�t���[���o�b�t�@�̃N���A */
                SMV1_SprCmdStart();
                SMV1_SprCmdEnd();
                SMMA_intwait();
                SMMA_intwait();

                /* �v���C�I���e�B�̃Z�b�g */
                SCL_SetPriority(SCL_SP0, 7);
                SCL_SetPriority(SCL_NBG0, 6);
                SCL_SetPriority(SCL_RBG0, 5);

                /* �J���[���Z�ݒ� */
                SCL_SetColMixMode(0, SCL_IF_FRONT);
                SCL_SetAutoColMix(SCL_SP0, 2, 2*32, 31, 1);

                /* �J���[���[�h�̐ݒ� */
                SCL_SetColRamMode(SCL_CRM15_2048); /* 15bit 2048 color mode */

                SMMA_intwait();
                SMV2_TvOff();

                SMV2_FilMem16((Uint16 *)SCR_VRAMA1,
                        (0x40 * 0x40 / sizeof (Uint16)), 0x0000);

                /* �Z�K���S�A�N�V�����Z�b�g */
                SMTA_MakeAction(SMLO_LogoAct);
                SMLO_segalogo_cgset();

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

                SMMA_MainMode = TITLE_MODE;
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
 *  SEGA LOGO ACTION
 *
 */
void SMLO_LogoAct(LOGOACT *ix)
{

    enum {
        LOGO_INIT,
        LOGO_ROTZOM,
        LOGO_ZOOM,
        LOGO_NORM1,
        LOGO_COLOR,
        LOGO_NORM2,
        LOGO_FADE,
        LOGO_NEXT
    };

    Uint16 swdata;
    Uint16 *logoadr;

    swdata = SMPA_pad_data1;

    switch (ix->mode) {
        case LOGO_INIT:
            ix->sprite.putmode = PMD_sprx_rotzoom;
            ix->sprite.pmode = SPM_ecd;
            ix->sprite.patbase = SMLO_logo_pattbl;
            ix->sprite.patno = 0;
            ix->sprite.xposi = 0xa0 * 0x10000;
            ix->sprite.yposi = 0x70 * 0x10000;
            ix->sprite.vzoom = 0x100;
            ix->sprite.hzoom = 0x100;
            ix->sprite.zdirec = 0;
            ix->sprite.color = 0;
            ix->wcnt[1] = 0;
            ix->mode++;
            break;

        case LOGO_ROTZOM:
            ix->sprite.vzoom += 0x8;
            ix->sprite.hzoom += 0x8;
            ix->sprite.zdirec += 4;
            if (ix->sprite.zdirec >= 0x100) {
                ix->sprite.zdirec = 0;
                ix->mode++;
            }
            if (swdata & PAD_START) {          /* PUSH START */
                ix->mode = LOGO_NORM1;
                ix->wcnt[1] = 0;
            }
            break;

        case LOGO_ZOOM:
            ix->sprite.vzoom -= 0x10;
            ix->sprite.hzoom -= 0x10;
            if (ix->sprite.vzoom == 0x100) {
                ix->mode++;
                ix->sprite.vzoom = 0x100;
                ix->sprite.hzoom = 0x100;
            }
            if (swdata & PAD_START) {          /* PUSH START */
                ix->mode = LOGO_NORM1;
                ix->wcnt[1] = 0;
            }
            break;

        case LOGO_NORM1:
            ix->wcnt[1]++;
            if (ix->wcnt[1] > 20) {
                ix->mode = LOGO_COLOR;
                ix->wcnt[0] = 4;     /* �J���[�`�F���W�C���^�[�o���J�E���^ */
                ix->wcnt[1] = 0x10;  /* �J���[�`�F���W�񐔃J�E���^ */
                ix->wcnt[2] = 0x12;  /* �J���[�e�[�u���I�t�Z�b�g */
            }
            ix->sprite.xposi = 0xa0 * 0x10000;
            ix->sprite.yposi = 0x70 * 0x10000;
            ix->sprite.vzoom = 0x100;
            ix->sprite.hzoom = 0x100;
            ix->sprite.zdirec = 0;
            ix->sprite.patbase = SMLO_logo_pattbl;
            ix->sprite.patno = 0;
            break;

        case LOGO_COLOR:
            if (ix->wcnt[1] != 0) {
                if (ix->wcnt[0]-- == 0) {
                    logoadr = &segalogo_clr1[ix->wcnt[2]];
                    SMLO_color_change(logoadr);
                    ix->wcnt[0] = 4;   /* �J�E���^�̏����� */
                    ix->wcnt[1]--;
                    ix->wcnt[2]--;
                }
            }
            else {
                ix->mode = LOGO_NORM2;
                ix->wcnt[0] = 0;
                ix->wcnt[1] = 0;
                ix->wcnt[2] = 0;
            }
            break;

        case LOGO_NORM2:
            ix->wcnt[1]++;
            if (ix->wcnt[1] > 20) {
                ix->mode = LOGO_FADE;
                ix->wcnt[1] = 0;
                SCL_SetColMixMode(0, SCL_IF_FRONT);
                SCL_SetAutoColMix(SCL_SP0, 0, 16, 1, 31);    /* FADE OUT */
            }
            break;

        case LOGO_FADE:
            ix->wcnt[1]++;
            if (ix->wcnt[1] > 60) {
                ix->mode = LOGO_NEXT;
            }
            break;

        case LOGO_NEXT:
            SMMA_MainLevel++;
            ix->mode = 0;
            break;
    }
    SMV1_PutSprite(&ix->sprite);
}


/*
 *
 *   SPRITE CG SET
 *
 */
void SMLO_segalogo_cgset(void)
{
    SPR_2ClrAllChar();
    SPR_2SetChar(SMLO_logocharNo,0,0,0x60, 0x20, (Uint8 *)segalogo_info1.cgadd);
    SMMI_Ldir16((Uint16 *)segalogo_info1.clradd, (Uint16 *)COLOR_RAM,
 segalogo_info1.clrcnt);
}


/*
 *
 *   SEGA LOGO COLOR CHANGE
 *
 */
void SMLO_color_change(Uint16 *clradd)
{
    SMMI_Ldir16(clradd, (Uint16 *)(COLOR_RAM + 4), 9);
}


