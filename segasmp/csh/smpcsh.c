#include "sega_xpt.h"
#include "sega_csh.h"

void main(void)
{
	CSH_Init(CSH_4WAY);
	CSH_AllClr();
	CSH_Purge(0, 0x1fffffff);

	while(1)
		;
}
