/*	Copyright(c) 1994 SEGA			*/
/*
 *
 * SATURN PAD CONTROL
 *
 */

#include "smp_bios.h"



#define SMALL_BOX  




/*--------------------*
 *    PAD I/O PORT    *
 *--------------------*/
#ifdef SMALL_BOX
#define PADADR 0x20100070
#define PAD1CTR (PADADR  + 0x09)
#define PAD1DAT (PADADR  + 0x05)
#define PAD2CTR (PADADR  + 0x0b)
#define PAD2DAT (PADADR  + 0x07)
#define SMPADMD (PADADR  + 0x0d)
#else
#define PADADR 0x25fc0100
#define PAD1CTR (PADADR  + 0x00)
#define PAD1DAT (PAD1CTR + 0x04)
#define PAD2CTR (PADADR  + 0x08)
#define PAD2DAT (PAD2CTR + 0x04)
#endif


extern void initevent(void);
extern void relsevent(void);
extern Uint16 getevent(Uint32 padno);


volatile Sint16 SMPA_pad_id1;         /* 1P PAD ID            */
volatile Sint16 SMPA_pad_data1;       /* 1P PAD DATA          */
volatile Sint16 SMPA_pad_edge1;       /* 1P PAD EDGE          */

volatile Sint16 SMPA_pad_id2;         /* 2P PAD ID            */
volatile Sint16 SMPA_pad_data2;       /* 2P PAD DATA          */
volatile Sint16 SMPA_pad_edge2;       /* 2P PAD EDGE          */

Sint16 SMPA_pad_repdata;	    /* 1P Repeat PAD DATA   */
Sint8  SMPA_pad_repcnt1;	    /* Repeat Counter1	    */


/*--------------------*/
/*    PadWork Init    */
/*--------------------*/
void SMPA_SwInit(void)
{
    SMPA_pad_id1 = 0;
    SMPA_pad_data1 = 0;
    SMPA_pad_edge1 = 0;

    SMPA_pad_id2 = 0;
    SMPA_pad_data2 = 0;
    SMPA_pad_edge2 = 0;

    SMPA_pad_repdata = 0;
    SMPA_pad_repcnt1 = 0;

}

/*-------------------*/
/*    Switch Read    */
/*-------------------*/
/*
   pn : port number 0 or 1
   3button : 1101 0000 1111 1111 1111 1111 SACB RLDU
   6button : 1101 0001 1111 1111 1111 MXYZ SACB RLDU
   8button : 1101 0001 1111 1111 111L RXYZ SACB RLDU
*/
void SMPA_SwRead(void)
{
    Uint8 pd[8], id = 0;
    volatile Uint8 *CTR, *DAT, *OUT;
    Uint32 RET;
    Uint16 new_id, new_data, old_data;

#if 0
    CTR = (Uint8 *)PAD2CTR;
    DAT = (Uint8 *)PAD2DAT;
    OUT = (Uint8 *)PAD2DAT;
#endif

    CTR = (Uint8 *)PAD1CTR;
    DAT = (Uint8 *)PAD1DAT;
    OUT = (Uint8 *)PAD1DAT;


#ifdef SMALL_BOX
    *(Uint8 *)SMPADMD = 0x03;		/* SMPC PADMODE SET TO DIRECT */
    SMPA_padwait();
#endif
        
    *CTR = 0x40;                            /* PAD ID READ */
    SMPA_padwait();
    *OUT = 0x40;
    SMPA_padwait();
    pd[0] = *DAT;
    SMPA_padwait();
    *OUT = 0x00;
    SMPA_padwait();
    pd[1] = *DAT;
    SMPA_padwait();

    if ((pd[1] & 0x03) != 0) 
        id |= 0x01;
    if ((pd[1] & 0x0c) != 0) 

        id |= 0x02;
    if ((pd[0] & 0x03) != 0) 
        id |= 0x04;
    if ((pd[0] & 0x0c) != 0) 
        id |= 0x08;


    switch (id) {                           /* PAD DATA READ */
        case 0x0d:                           /* 3B or 6B PAD */
	    *OUT = 0x40;
            SMPA_padwait();
	    pd[2] = *DAT;
            SMPA_padwait();
	    *OUT = 0x00;
            SMPA_padwait();
	    pd[3] = *DAT;
            SMPA_padwait();
	    *OUT = 0x40;
            SMPA_padwait();
	    pd[4] = *DAT;
            SMPA_padwait();
	    *OUT = 0x00;
            SMPA_padwait();
	    pd[5] = *DAT;
            SMPA_padwait();
	    *OUT = 0x40;
            SMPA_padwait();
	    pd[6] = *DAT;
            SMPA_padwait();
	    *OUT = 0x00;
            SMPA_padwait();
	    pd[7] = *DAT;
            SMPA_padwait();
	    *OUT = 0x40;
            SMPA_padwait();
	    if ((pd[5] & 0x0f) != 0x00)
                new_data = (pd[0] & 0x3f) + (pd[1] & 0x30) * 4;   /* 3B PAD */
	    else
                new_data = (pd[0] & 0x3f) + (pd[1] & 0x30) * 4
                           + (pd[6] & 0x0f) * 0x100; /* 6B PAD */
            break;

        case 0x0b:                          /* 8B PAD */
	    *CTR = 0x60;
            SMPA_padwait();
	    *OUT = 0x60;
            SMPA_padwait();
	    pd[2] = *DAT;
            SMPA_padwait();
	    *OUT = 0x20;
            SMPA_padwait();
	    pd[3] = *DAT;
            SMPA_padwait();
	    *OUT = 0x00;
            SMPA_padwait();
	    pd[4] = *DAT;
            SMPA_padwait();
	    *OUT = 0x40;
            SMPA_padwait();
	    pd[5] = *DAT;
            SMPA_padwait();
	    *OUT = 0x60;
            SMPA_padwait();
	    *CTR = 0x40;
#ifdef SMALL_BOX
	    new_data = (pd[3] & 0x0f) + (pd[5] & 0x0f) * 0x10
	             + (pd[4] & 0x0f) * 0x100 + (pd[2] & 0x08) * 0x200;
#else
	    new_data = (pd[3] & 0x01) * 0x002 + (pd[3] & 0x02) / 0x002 + 
                     + (pd[3] & 0x0c) +(pd[5] & 0x01) * 0x020
                     + (pd[5] & 0x02) * 0x008 + (pd[5] & 0x0c) * 0x10
	             + (pd[4] & 0x0f) * 0x100 + (pd[2] & 0x08) * 0x200;
#endif
	    break;

        default:                            /* NO CONNECT or ERROR */
	    new_data = 0xffff;
	    break;
    }

    SMPA_pad_id1 = id;
    new_data = (~new_data) & 0x1fff;         /* NOT new_data        */
    old_data = SMPA_pad_data1;
    SMPA_pad_edge1 = (~old_data) & new_data; /* Make Edge Data      */
    SMPA_pad_data1 = new_data;               /* Make New Data       */


    if (new_data == old_data) {
        if (SMPA_pad_repcnt1 != 0) {
            SMPA_pad_repcnt1--;
            SMPA_pad_repdata = 0;
            return;
        }
        SMPA_pad_repdata = new_data;
    } else {
        SMPA_pad_repcnt1 = SMPA_REPCNT1;
        SMPA_pad_repdata = new_data;
    }


}








