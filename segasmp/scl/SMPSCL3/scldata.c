#include	<stdio.h>
#include	<sega_xpt.h>
#include	<sega_def.h>
#include	<sega_mth.h>
#include	<sega_scl.h>

#include	"..\graphic\grandu.cha"
#include	"..\graphic\grandu.col"
#include	"..\graphic\grandu.map"
#include	"..\graphic\skyu.cha"
#include	"..\graphic\skyu.col"
#include	"..\graphic\skyu.map"


void WordCopyforVRAM(void *dest,void *src,Uint32 tcnt);
void MapCopyforVRAM(void *dest,void *src,Uint32 Offset,Uint32 tcnt);
void MapCopyforVRAM2(void *dest,Uint32 src,Uint32 Offset,Uint32 tcnt);


void FirstData(Uint32 sclnum)
{
    Uint32	char_ram;
    Uint32	col_ram;
    Uint32	Offset;
    Uint16	i;

    char_ram = SCL_VDP2_VRAM_A0;
    col_ram  = SCL_AllocColRam(sclnum,256,ON);

    Offset = 0;

    /* Set Charater Data into VRAM */
    WordCopyforVRAM((void *)char_ram, grandu_char, sizeof(grandu_char));

    /* Set Map Data into VRAM */
    char_ram = SCL_VDP2_VRAM_A1;
    for(i=0;i<16;i++)
    {
	MapCopyforVRAM((void *)char_ram             , &grandu_map[i*16],Offset,16*2);
	MapCopyforVRAM((void *)(char_ram+16*4)      , &grandu_map[i*16],Offset,16*2);
	MapCopyforVRAM((void *)(char_ram+32*4)      , &grandu_map[i*16],Offset,16*2);
	MapCopyforVRAM((void *)(char_ram+48*4)      , &grandu_map[i*16],Offset,16*2);
	MapCopyforVRAM((void *)(char_ram+4096)      , &grandu_map[i*16],Offset,16*2);
	MapCopyforVRAM((void *)(char_ram+4096+16*4) , &grandu_map[i*16],Offset,16*2);
	MapCopyforVRAM((void *)(char_ram+4096+32*4) , &grandu_map[i*16],Offset,16*2);
	MapCopyforVRAM((void *)(char_ram+4096+48*4) , &grandu_map[i*16],Offset,16*2);
	MapCopyforVRAM((void *)(char_ram+8192)      , &grandu_map[i*16],Offset,16*2);
	MapCopyforVRAM((void *)(char_ram+8192+16*4) , &grandu_map[i*16],Offset,16*2);
	MapCopyforVRAM((void *)(char_ram+8192+32*4) , &grandu_map[i*16],Offset,16*2);
	MapCopyforVRAM((void *)(char_ram+8192+48*4) , &grandu_map[i*16],Offset,16*2);
	MapCopyforVRAM((void *)(char_ram+12288)     , &grandu_map[i*16],Offset,16*2);
	MapCopyforVRAM((void *)(char_ram+12288+16*4), &grandu_map[i*16],Offset,16*2);
	MapCopyforVRAM((void *)(char_ram+12288+32*4), &grandu_map[i*16],Offset,16*2);
	MapCopyforVRAM((void *)(char_ram+12288+48*4), &grandu_map[i*16],Offset,16*2);
	char_ram += 64*4;
    }

    /* Set Color Data into Color RAM */
    WordCopyforVRAM((void *)col_ram, grandu_col,sizeof(grandu_col));
}

Uint32 SecondData(Uint32 sclnum)
{
    Uint32	char_ram;
    Uint32	col_ram;
    Uint32	Offset;
    Uint16	i;

    char_ram = SCL_VDP2_VRAM_A0+8*8*256; /*sizeof(grandu_char);*/
    col_ram  = SCL_AllocColRam(sclnum,256,OFF);

    Offset = 256*2;

    /* Set Charater Data into VRAM */
    WordCopyforVRAM((void *)char_ram, skyu_char, sizeof(skyu_char));

    /* Set Map Data into VRAM */
    char_ram = SCL_VDP2_VRAM_A1+(64*64*4);
    for(i=0;i<16;i++)
    {
	MapCopyforVRAM((void *)char_ram       ,&skyu_map[i*16],Offset,16*2);
	MapCopyforVRAM((void *)(char_ram+16*4),&skyu_map[i*16],Offset,16*2);
	MapCopyforVRAM((void *)(char_ram+32*4),&skyu_map[i*16],Offset,16*2);
	MapCopyforVRAM((void *)(char_ram+48*4),&skyu_map[i*16],Offset,16*2);
	char_ram += 64*4;
    }
    char_ram = SCL_VDP2_VRAM_A1 + ((64+16)*64*4);
    MapCopyforVRAM2((void *)char_ram,(Uint32 )skyu_map[16*16],Offset,48*64);

    /* Set Color Data into Color RAM */
    WordCopyforVRAM((void *)col_ram, skyu_col,sizeof(skyu_col));

    return(SCL_VDP2_VRAM_A1+(64*64*4));
}


/****************************************************************************
 *  Žw’è‚³‚ê‚½‚u‚q‚`‚l‚ð‚P‚Â‚Ìƒf[ƒ^‚Å–„‚ß‚é
 ****************************************************************************/
void MapCopyforVRAM2(void *dest,Uint32 src,Uint32 Offset,Uint32 tcnt)
{
    Uint32 tcr,Work;

    for (tcr = 0;tcr < tcnt;tcr++){
	Work = src;
	Work += Offset;
	*((Uint32 *)dest) = Work;
	dest = (Uint8 *)dest + 4;
    }
}


void WordCopyforVRAM(void *dest,void *src,Uint32 tcnt)
{
    Uint32 tcr,tsize;

    tsize=tcnt;
    tsize=tsize/2;

    for (tcr = 0;tcr < tsize;tcr++){
	*((Uint16 *)dest) = *((Uint16 *)src);
	dest = (Uint8 *)dest + 2;
	src  = (Uint8 *)src  + 2;
    }
}

void MapCopyforVRAM(void *dest,void *src,Uint32 Offset,Uint32 tcnt)
{
    Uint32 tcr,tsize,Work;

    tsize=tcnt;
    tsize=tsize/2;

    for (tcr = 0;tcr < tsize;tcr++){
	Work = *((Uint16 *)src);
	Work += Offset;
	*((Uint32 *)dest) = Work;
	dest = (Uint8 *)dest + 4;
	src  = (Uint8 *)src  + 2;
    }
}

