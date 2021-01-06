// Based off Sega's CDC Library. Written by Cyber Warrior X

#include "sega_cdc.h"
#include "cd_int.h"

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_E0(unsigned char R4, unsigned char R5)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0xE000;
   cdcmd.CR2 = R4;
   cdcmd.CR3 = R5 << 8;
   cdcmd.CR4 = 0;

   if (R4 == 1)
      return CDSUB_UpdCdstat(CDC_HIRQ_MPED, &cdcmd);
   else
      return CDSUB_UpdCdstat(CDC_HIRQ_EFLS, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_E1(unsigned char R4, unsigned short *R5, unsigned short *R6)
{
   cdcmd_struct cdcmd;
   cdcmd_struct cdcmdrsp;
   Sint32 ret;

   cdcmd.CR1 = 0xE100;
   cdcmd.CR2 = R4;
   cdcmd.CR3 = 0;
   cdcmd.CR4 = 0;
   
   ret = CDSUB_UpdStatus(0, &cdcmd, &cdcmdrsp);

   R5[0] = cdcmdrsp.CR2;
   R5[0] = cdcmdrsp.CR4;

   return ret;   
}

//////////////////////////////////////////////////////////////////////////////

Sint32 CDC_E2(unsigned char R4, unsigned long R5, unsigned short R6)
{
   cdcmd_struct cdcmd;

   cdcmd.CR1 = 0xE200 | ((R5 >> 16) & 0xFF);
   cdcmd.CR2 = (R5 & 0xFFFF);
   cdcmd.CR3 = R4 << 8;
   cdcmd.CR4 = R6;

   return CDSUB_UpdCdstat(CDC_HIRQ_MPED, &cdcmd);
}

//////////////////////////////////////////////////////////////////////////////
