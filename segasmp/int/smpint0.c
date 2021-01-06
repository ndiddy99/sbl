/****************************************************************************/
/*  filename "smpint0.c"                                                     */
/*  ÇhÇmÇsÉTÉìÉvÉãÉvÉçÉOÉâÉÄ                                                */
/*  Copyright(c) 1994 SEGA                                                  */
/*  Written by N.T on 1994-05-18 Ver.0.90                                   */
/*  Updated by N.T on 1994-05-18 Ver.0.90                                   */
/****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "sega_xpt.h"
#include "sega_int.h"

void VBlkInInt(void);
void VBlkIn(void);

static Uint32 status;

main()
{
/*** 1 ***/
    INT_SetMsk(INT_MSK_ALL & ~(INT_MSK_VBLK_IN | INT_MSK_HBLK_IN));
    *(Uint32 *)0x6040000 = INT_GetMsk();
    INT_SetMsk(INT_MSK_ALL);
    *(Uint32 *)0x6040004 = INT_GetMsk();
    
/*** 2 ***/
    INT_SetMsk(0x0000bf00);
    INT_ChgMsk(0x00000f00, 0x0000000f);
    *(Uint32 *)0x6040050 = INT_GetMsk();

/*** 4 ***/
    INT_ChgMsk((INT_MSK_HBLK_IN | INT_MSK_VBLK_IN), INT_MSK_NULL);
    status = INT_GetStat();
    while((status & INT_ST_VBLK_IN) != INT_ST_VBLK_IN){
        status = INT_GetStat();
    }
    *(Uint32 *)0x6040100 = status; 
    INT_ResStat(INT_ST_VBLK_IN);
    *(Uint32 *)0x6040104 = INT_GetStat();
    
/*** 5 ***/
    INT_SetAck(INT_ACK_ENA);
    *(Uint32 *)0x6040110 = INT_GetAck();
    INT_SetAck(INT_ACK_DIS);
    *(Uint32 *)0x6040114 = INT_GetAck();
    
/*** 6 ***/
    INT_SetFunc(INT_SCU_VBLK_IN, VBlkInInt);
    *(Uint32 *)0x6040124 = (Uint32)VBlkInInt;
    INT_ChgMsk(INT_MSK_VBLK_IN, INT_MSK_NULL);
    *(Uint32 *)0x6040128 = (Uint32)INT_GetFunc(INT_SCU_VBLK_IN);
    
/*** 7 ***/
    INT_SetFunc(INT_SCU_VBLK_IN, 0);
    INT_SetScuFunc(INT_SCU_VBLK_IN, VBlkIn);
    *(Uint32 *)0x6040134 = (Uint32)VBlkIn;
    INT_ChgMsk(INT_MSK_VBLK_IN, INT_MSK_NULL);
    *(Uint32 *)0x6040138 = (Uint32)INT_GetScuFunc(INT_SCU_VBLK_IN);
    
}

void VBlkIn(void)
{
    *(Uint32 *)0x6040130 = 0x87654321;
}

/****************************************************************************/
/*  End of File                                                             */
/****************************************************************************/
