/*	Copyright(c) 1994 SEGA			*/
/*
 *
 * なんでもありの関数群
 *
 */


#ifndef	__SMP_MISC_H
#define	__SMP_MISC_H


/*
 *    関数プロトタイプ宣言 
 */
extern Sint16 SMMI_Sinset(Sint16);
extern Sint16 SMMI_Cosset(Sint16);
extern Sint16 SMMI_Sinset8(Sint16);
extern Sint16 SMMI_Cosset8(Sint16);
extern Sint16 SMMI_atan(Sint16 dx, Sint16 dy);

extern void SMMI_Ldir32(Uint32 *src, Uint32 *dst, Uint32 len);
extern void SMMI_Ldir16(Uint16 *src, Uint16 *dst, Uint32 len);
extern void SMMI_Ldir8(Uint8 *src, Uint8 *dst, Uint32 len);

extern void SMMI_CPUDmaLdirL0(Uint32 srcadd, Uint32 dstadd, Uint32 trcnt);
extern void SMMI_CPUDmaLdirW0(Uint32 srcadd, Uint32 dstadd, Uint32 trcnt);
extern void SMMI_CPUDmaLdirL1(Uint32 srcadd, Uint32 dstadd, Uint32 trcnt);
extern void SMMI_CPUDmaLdirW1(Uint32 srcadd, Uint32 dstadd, Uint32 trcnt);

extern void SMMI_PutHexNum32(Sint16 xposi, Sint16 yposi, Uint32 number);
extern Sint16 SMMI_getcount(Uint32 *data, Uint32 subdata);

extern Uint32 SMCA_sqrt(Uint32);

/*
 *    ワークRAM
 */
GLOBAL Sint32 SMMI_randwk;

#endif

