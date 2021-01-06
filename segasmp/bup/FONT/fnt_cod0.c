#include	<sega_xpt.h>

	Uint16	FntXsize;
	Uint16	FntYsize;
	Uint8	FntContry;
	Uint8	FntBitPat[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

void  FNT_SetBuffSize(Uint16 x,Uint16 y,Uint8 contry)
{
	FntXsize  = x;
	FntYsize  = y;
	FntContry = contry;
}
