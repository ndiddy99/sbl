/*
 *
 * �T�E���h�R���g���[��
 *
 */

#include "smp_bios.h"



/*
 *   �֐��v���g�^�C�v
 */
static void SMSN_Sound_ResetWaitB(void);
static void SMSN_Sound_ResetWaitA(void);
static void SMSN_Sound_Ldir8(Uint8 *src, Uint8 *dst, Uint32 len);


/*
 *   �O���Q��
 */

/*
 *   �萔
 */
#define SOUND_RAM     0x25a00000
#define SND_SYSTBLTOP 0x25a00400
#define SND_SDDRVTOP  0x25a01000

#define GM_MAP_TOP    0x25a0a000
#define GM_SEQ_TOP    0x25a10000
#define GM_TONE_TOP   0x25a20000


/*==========================*/
/*        �f�[�^            */
/*==========================*/
const Uint8 SMSN_fname_systbl[] = "SYSTBL.TSK";
const Uint8 SMSN_fname_sddrv[] = "SDDRV.TSK";
const Uint8 SMSN_fname_gmmap[] = "GMMAP.BIN";
const Uint8 SMSN_fname_gmseq[] = "GMSEQ.BIN";
const Uint8 SMSN_fname_gmtone[] = "GMTONE.BIN";

/*
 *    SOUND INIT
 */
void SMSN_IniSound(void)
{
    Uint8 *bport;
    Uint8 bdata;
    Uint32 i, ldata;
    Uint32 *lport;
    Sint32 gfs_ret;

#if 0 /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
    SMSN_Sound_ResetWaitB();         /* ���Z�b�g�v�� �O�E�G�C�g */
    bport = (Uint8 *)0x25fc00fc;     /* �T�E���h���Z�b�g�v�� */
    *bport = 2;
    SMSN_Sound_ResetWaitA();         /* ���Z�b�g�v�� ��E�G�C�g */


    bport = (Uint8 *)0x25b00400;     /* MEM4MB �Z�b�g */
    *bport = 2;

    bport = (Uint8 *)0x25a00000;     /* �T�E���h�h���C�o�p���[�N�̃N���A */
    for (i = 0; i < 0xb000; i++) {
        do {
             *bport = 0x00;
             bdata = *bport;
        } while (bdata != 0x00);
        bport++;
    }

    /* �T�E���h�t�@�C���̃��[�h */
       /* �h���C�o�̃��[�h */
       SMCD_FileLoad(SMSN_fname_systbl, (Uint8 *)SND_SYSTBLTOP);
       SMCD_FileLoad(SMSN_fname_sddrv,  (Uint8 *)SND_SDDRVTOP);

       /* �T�E���h�f�[�^�̃��[�h */
       SMCD_FileLoad(SMSN_fname_gmmap,  (Uint8 *)GM_MAP_TOP);
       SMCD_FileLoad(SMSN_fname_gmseq,  (Uint8 *)GM_SEQ_TOP);
       SMCD_FileLoad(SMSN_fname_gmtone, (Uint8 *)GM_TONE_TOP);
    

    /* 68000 PC & SP SET */
    lport = (Uint32 *)0x25a00000;
    *lport = 0x7f000;
    lport++;
    *lport = 0x1000;

    SMSN_Sound_ResetWaitB();         /* ���Z�b�g�v�� �O�E�G�C�g */
    bport = (Uint8 *)0x25fc00fc;     /* �T�E���h���Z�b�g���� */
    *bport = 1;
    SMSN_Sound_ResetWaitA();         /* ���Z�b�g�v�� ��E�G�C�g */

#endif /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

}


/*
 *   68000���Z�b�g�v���^�����ݒ� �O�E�G�C�g
 */
static void SMSN_Sound_ResetWaitB(void)
{
    Uint8 *bport;
    Uint8 bdata;

    bport = (Uint8 *)0x25fc00c4;
    do {
        bdata = *bport;
        bdata &= 0x01;
    } while (bdata != 0);

    bdata = *bport;
    bdata |= 0x01;
    *bport = bdata;

    do {
        bdata = *bport;
        bdata &= 0x01;
    } while (bdata != 1);
}


/*
 *   68000���Z�b�g�v���^�����ݒ� ��E�G�C�g
 */
static void SMSN_Sound_ResetWaitA(void)
{
    Uint8 *bport;
    Uint8 bdata;

    bport = (Uint8 *)0x25fc00c4;
    do {
        bdata = *bport;
        bdata &= 0x01;
    } while (bdata != 0);

}



/*
 *   �T�E���h�]���p
 */
static void SMSN_Sound_Ldir8(Uint8 *src, Uint8 *dst, Uint32 len)
{
    do {
        do {                    /* ���m�ɒl���������߂�܂ŏ����I�I */
            *dst = *src;
        } while (*dst != *src);
        dst++;
        src++;
        len--;
    } while (len != 0);

}



/*
 *  �T�E���h�h���C�o�R�}���h�E�G�C�g
 */
static void SMSN_Sound_Comwait(void)
{
    Uint16 *wport;
    Uint16 data;
    Uint32 count;

    wport = (Uint16 *)0x25a00480;
    count = 0;
    do {
         data = *wport;
         count++;
    } while (data != 0);

}


/*
 *  �T�E���h�Z�b�g
 */
void SMSN_SetSound(Uint16 soundno)
{
    Uint16 *wport;
    Uint16 data;
    Uint32 count;

    wport = (Uint16 *)0x25a00482;
    *wport = 0x00;
    wport--;
    *wport = 0x08;
    SMSN_Sound_Comwait();

    wport += 4; /* +8byte */
    *wport = 0x0000;    /* 8 */
    wport--;
    *wport = soundno;    /* 6 */
    wport--;
    *wport = 0x0000;    /* 4 */
    wport--;
    *wport = 0x0000;    /* 2 */
    wport--;
    *wport = 0x0001;
}

/*
 *  �T�E���h�~�߂�
 */
void SMSN_StopSound(Uint16 soundno)
{
    Uint16 *wport;
    Uint16 data;
    Uint32 count;

    wport = (Uint16 *)0x25a00482;
    *wport = 0x00;
    wport--;
    *wport = 0x08;
    SMSN_Sound_Comwait();

    wport += 1; /* +2byte */
    *wport = soundno;    /* 2 */
    wport--;
    *wport = 0x0001;
}

