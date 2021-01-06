#include	<sega_xpt.h>

#define		FNT_JAPAN	1

extern	Uint8	FntAsciiFontData[];/* 8x16 ASCII Font アルファベット部分 */
extern	Uint8	FntKanaFontData[]; /* 8x16 ASCII Font かな部分 */
extern	Uint8	FntEouFontData[];  /* 8x16 ASCII Font ヨーロッパ文字？部分 */

extern	Uint16	FntXsize;
extern	Uint16	FntYsize;
extern	Uint8	FntContry;
extern	Uint8	FntBitPat[];

static	Uint8 *FNT_SerchFont8(Uint8 code)
{
	if(code >= 0x20 && code < 0x80)
	    return((Uint8 *)(FntAsciiFontData+((Uint32 )(code-0x20))*16));
	else if(code > 0xa0) {
	    if(FntContry = FNT_JAPAN)
		return((Uint8 *)(FntKanaFontData+((Uint32 )(code-0xa1))*16) );
	    else
		return((Uint8 *)(FntEouFontData+((Uint32 )(code-0xa1))*16) );
	}
	return((Uint8 *)FntAsciiFontData);
}

static	void  FNT_Print1Char256_8x16(Uint8 *vram,Uint8 code,Uint16 x,Uint16 y,
					Uint16 col,Uint16 back)
{
Uint32	wx,wy,wp;
Uint32	i,j;
Uint8	*codep;

	codep = FNT_SerchFont8(code);

	wx = x;
	wy = y;

	wp = wx	+ wy * FntXsize;
	if(back < 256) {
	    for(i=0;i<16;i++) {
		for(j=0;j<8;j++) {
			if(codep[i] & FntBitPat[j])
				vram[wp]= (Uint8 )col;
			else
				vram[wp]= (Uint8 )back;
			wp++;
		}
		wp += (FntXsize - 8);
	    }
	}else{
	    for(i=0;i<16;i++) {
		for(j=0;j<8;j++) {
			if(codep[i] & FntBitPat[j])
				vram[wp]= (Uint8 )col;
			wp++;
		}
		wp += (FntXsize - 8);
	    }
	}
}

void  FNT_Print256(Uint8 *vram,Uint8 *str,Uint16 x,Uint16 y,Uint16 col,Uint16 back)
{
Uint32	i;
Uint16	wx,wy;

	wx = x;wy = y;

	/* センタリング */
	if(wx > FntXsize)	wx = FntXsize/2 - strlen(str)*4;

	i=0;
	while(str[i]) {
		if(str[i] > 0x80 && str[i] < 0xA0) {
			/* シフトＪＩＳ全角コードだったらスペースをあける。 */
			wx += 16;
			i++;
		}else if(str[i]=='\\'){
			i++;
			if(str[i]=='n') {
				wx = x;wy += 18;/* 改行 */
				if(wy > FntYsize-15)	return;
			}
		}else if(str[i]==0x0a) {
			wx = x;wy += 18;/* 改行 */
			if(wy > FntYsize-15)	return;
		}else{
			FNT_Print1Char256_8x16(vram,str[i],wx,wy,col,back);
			wx += 8;
		}
		if(wx+8 >= FntXsize) {
			wx = x;wy += 18;/* 改行 */
			if(wy > FntYsize-15)	return;
		}
		i++;
	}
}
