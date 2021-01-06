#include	"sega_xpt.h"

Uint32 BackPalette[] = {
	0x00ffffff,0x00b5848c,0x00ad7384,0x00ad737b,
	0x00a56b7b,0x00a56b73,0x00a5636b,0x009c636b,
	0x009c5a6b,0x00945a63,0x008c5263,0x008c525a,
	0x0084525a,0x007b4a52,0x00734a52,0x0073424a,
	0x006b424a,0x006b4242,0x00633942,0x005a3942,
	0x005a3939,0x00523139,0x004a3131,0x004a2931,
	0x00312121,0x00311821,0x00181010,0x00100808,
	0x00080808,0x00080008,0x00000000,0x00181d1d,
	0x001d1d1d,0x001d1d1d,0x001d1d1d,0x001d1d1d,
	0x001d1d1d,0x001d1d1c,0x001d1c1b,0x000a1111,
	0x00100d09,0x00070509,0x00080505,0x00060707,
	0x00050505,0x00070a0a,0x00070a07,0x000a1117,
	0x000c0c0c,0x000e0c09,0x000c0c09,0x000c0608,
	0x000a090b,0x000a0909,0x0017140c,0x000a1101,
	0x000c0d0c,0x00090a0f,0x000d0c0c,0x000b0c0c,
	0x000a0a0c,0x000f0c0f,0x0001170c,0x00091111,
	0x000d0d0d,0x000e0f0d,0x000e0a0a,0x00100d0f,
	0x000f0f0f,0x000f100d,0x000f0d15,0x00090e0d,
	0x00110c0d,0x000a0b14,0x000d0d0a,0x000c0a09,
	0x000d0c0d,0x000d0e10,0x000e0d0f,0x00090d12,
	0x000d100d,0x00110d0c,0x000a0a0f,0x000f0e0e,
	0x00090e0f,0x00131112,0x000f1210,0x00090c0e,
	0x000d090c,0x00090a0d,0x00090c0a,0x0005070a,
	0x00090a0a,0x000a0e0c,0x000e0e0c,0x00091513,
	0x00130d0e,0x000e090a,0x00080a0c,0x000c0d0a,
	0x000b0c0b,0x000f0c0e,0x000e0e09,0x00090c0a,
	0x000c0c09,0x000b0b0d,0x000e0c0a,0x000c0d09,
	0x000c0d0b,0x000b0d0d,0x00141417,0x00090d0d,
	0x000d0c0b,0x000d0d07,0x00070707,0x000a0808,
	0x000c0b0b,0x000c0c0e,0x00100e0c,0x00091310,
	0x000d0e0e,0x000d0f13,0x000d0d0d,0x000e100e,
	0x000a0c0c,0x000d090c,0x000d0d0c,0x0009070c,
	0x000a0c0c,0x0010070c,0x000c0c0a,0x00070708,
	0x00090b0b,0x000a0c0c,0x000d140f,0x00090d10,
	0x000c090b,0x000a0707,0x000d0a0c,0x000c0a0a,
	0x00070a09,0x00090c09,0x000b0d0a,0x00090b0c,
	0x000a080a,0x000c0c0e,0x000b0b0c,0x000b0909,
	0x00100c0d,0x000c0c0a,0x00121215,0x00091010,
	0x000e0d0c,0x000c0907,0x000a0a0a,0x000d0c0e,
	0x000a0c0c,0x000d0e0e,0x000e0d0c,0x00090d0b,
	0x00100a0a,0x000a0c0e,0x000c0b09,0x000c0e0c,
	0x000a0909,0x00090c0c,0x000e0e11,0x00090b0c,
	0x00080e0e,0x000d0c0e,0x000c0a0d,0x000c0909,
	0x000a0b0b,0x000b0c0e,0x00101113,0x00080d16,
	0x0012100c,0x000a0905,0x0007070a,0x00080c0a,
	0x00070709,0x0008060b,0x000d1012,0x0008150f,
	0x000d0d0c,0x000d0c10,0x000e0d0d,0x000c0e0d,
	0x000d0e0d,0x000a0e0e,0x0010110d,0x0008100b,
	0x00090c0d,0x000a0907,0x000d0a0f,0x000d0c0c,
	0x000d0c09,0x000d0d0a,0x00141115,0x00071011,
	0x00131310,0x000d1311,0x000e0e0e,0x000d100a,
	0x000c090b,0x000c0c10,0x00100c07,0x00071013,
	0x00110d0c,0x000d090b,0x000a0805,0x0009090d,
	0x0008080a,0x000c090a,0x000e130c,0x00071312,
	0x00171510,0x000a0709,0x00090c13,0x0010100d,
	0x000c0a0c,0x0015100d,0x000e0d0d,0x00080c0d,
	0x000c0d0c,0x000a0c0c,0x00070706,0x00070708,
	0x0008090a,0x000c0f0c,0x00131715,0x00070b0d,
	0x000e0c0a,0x00070e0a,0x000c0c0c,0x000c0d0c,
	0x00070907,0x000b0a0d,0x000e0e0c,0x0007100d,
	0x000a0c0c,0x000c0909,0x00090a09,0x0007090c,
	0x000a0c0c,0x000c0f10,0x0013100d,0x00060e12,
	0x0012100f,0x000d0d0d,0x000c0c0a,0x000a0e0c,
	0x000a0a0c,0x000b0a0c,0x00100e0b,0x12170f06
};

Uint8 BackCharPatData[] = {
	/* 0 */
	0x04,0x02,0x01,0x01,0x01,0x01,0x01,0x03,
	0x05,0x0e,0x0f,0x0c,0x09,0x0a,0x0a,0x0a,
	0x05,0x10,0x01,0x17,0x0f,0x11,0x10,0x10,
	0x06,0x0f,0x17,0x12,0x0c,0x0c,0x0b,0x09,
	0x06,0x0e,0x12,0x0f,0x12,0x10,0x0f,0x0f,
	0x07,0x10,0x0d,0x0c,0x0a,0x0c,0x0c,0x10,
	0x07,0x0b,0x0d,0x10,0x0e,0x0c,0x0a,0x0a,
	0x08,0x0c,0x0d,0x0e,0x0c,0x0d,0x0c,0x09,
	/* 1 */
	0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
	0x0c,0x0c,0x0c,0x0a,0x0a,0x0b,0x0a,0x0c,
	0x0e,0x0b,0x0d,0x0d,0x10,0x10,0x11,0x0d,
	0x07,0x09,0x07,0x09,0x07,0x07,0x07,0x09,
	0x0d,0x0d,0x0d,0x0a,0x0c,0x0c,0x0a,0x0c,
	0x0c,0x09,0x09,0x09,0x09,0x0a,0x09,0x09,
	0x07,0x0e,0x0a,0x0d,0x0c,0x0c,0x0c,0x0a,
	0x0a,0x0c,0x0c,0x0b,0x07,0x07,0x06,0x08,
	/* 2 */
	0x07,0x13,0x12,0x0f,0x17,0x15,0x10,0x14,
	0x07,0x10,0x13,0x0c,0x11,0x0d,0x0c,0x0d,
	0x07,0x10,0x11,0x0e,0x13,0x13,0x10,0x10,
	0x08,0x10,0x0b,0x0e,0x09,0x0c,0x0d,0x08,
	0x08,0x15,0x0f,0x0c,0x0d,0x0d,0x0c,0x0d,
	0x08,0x0d,0x16,0x17,0x12,0x10,0x0c,0x0b,
	0x09,0x0b,0x0c,0x09,0x08,0x0e,0x0e,0x0d,
	0x09,0x0d,0x0b,0x0d,0x10,0x0a,0x0a,0x09,
	/* 3 */
	0x0a,0x07,0x09,0x07,0x09,0x0c,0x13,0x11,
	0x0d,0x09,0x0b,0x09,0x0a,0x08,0x05,0x05,
	0x0d,0x13,0x11,0x13,0x0e,0x0e,0x0e,0x0e,
	0x0a,0x09,0x07,0x07,0x0d,0x0a,0x0f,0x0e,
	0x0d,0x0c,0x10,0x10,0x0e,0x0d,0x0d,0x0d,
	0x0a,0x09,0x05,0x07,0x07,0x07,0x0a,0x0a,
	0x0d,0x0c,0x0e,0x0d,0x0c,0x0a,0x0d,0x0e,
	0x0a,0x0c,0x0e,0x10,0x0c,0x0b,0x09,0x0b,
	/* 4 */
	0x01,0x01,0x02,0x02,0x04,0x04,0x04,0x04,
	0x0c,0x0c,0x0c,0x0b,0x09,0x09,0x0a,0x0a,
	0x0c,0x0c,0x0b,0x0c,0x0b,0x0c,0x0e,0x0c,
	0x0c,0x09,0x09,0x0a,0x09,0x08,0x06,0x0a,
	0x0a,0x0e,0x0c,0x0a,0x0a,0x0a,0x0c,0x0c,
	0x07,0x09,0x0c,0x0c,0x0a,0x0c,0x0c,0x0d,
	0x0c,0x0d,0x0c,0x0d,0x07,0x09,0x07,0x08,
	0x07,0x07,0x08,0x08,0x08,0x09,0x0a,0x0a,
	/* 5 */
	0x04,0x04,0x04,0x02,0x02,0x05,0x02,0x19,
	0x0a,0x0a,0x0a,0x0d,0x0f,0x0f,0x0b,0x1e,
	0x0c,0x0a,0x0d,0x12,0x01,0x17,0x09,0x1c,
	0x0c,0x0b,0x0c,0x0f,0x17,0x14,0x0c,0x1b,
	0x0b,0x0a,0x0c,0x0e,0x10,0x0e,0x0b,0x1b,
	0x0c,0x0f,0x10,0x0e,0x13,0x10,0x0d,0x1b,
	0x0b,0x0a,0x0d,0x10,0x0e,0x0e,0x0c,0x1b,
	0x0c,0x0f,0x0c,0x0c,0x13,0x17,0x15,0x1a,
	/* 6 */
	0x10,0x10,0x0d,0x0c,0x0c,0x0a,0x0c,0x10,
	0x09,0x09,0x0d,0x09,0x08,0x08,0x0a,0x0b,
	0x0d,0x10,0x0a,0x0d,0x0c,0x09,0x0b,0x09,
	0x0d,0x0c,0x0c,0x0c,0x0d,0x0c,0x09,0x0b,
	0x0c,0x0e,0x0d,0x0d,0x0d,0x0e,0x0d,0x0c,
	0x08,0x0c,0x0a,0x07,0x07,0x07,0x09,0x09,
	0x0c,0x09,0x09,0x0c,0x0a,0x0b,0x0b,0x0c,
	0x0c,0x0e,0x0c,0x0a,0x0a,0x09,0x09,0x0a,
	/* 7 */
	0x15,0x10,0x0d,0x0f,0x0e,0x0d,0x0d,0x1b,
	0x0c,0x09,0x0a,0x10,0x0e,0x13,0x0c,0x1b,
	0x0c,0x0c,0x10,0x13,0x10,0x0c,0x07,0x1c,
	0x0d,0x0d,0x0a,0x0c,0x14,0x11,0x15,0x1a,
	0x0a,0x0e,0x0e,0x10,0x10,0x11,0x0d,0x1b,
	0x08,0x06,0x0b,0x0d,0x0d,0x10,0x12,0x1b,
	0x0b,0x0c,0x0e,0x0f,0x10,0x11,0x13,0x1a,
	0x09,0x0c,0x0c,0x0d,0x0e,0x0e,0x11,0x1b,
	/* 8 */
	0x09,0x10,0x10,0x0f,0x0e,0x0d,0x0c,0x0c,
	0x09,0x0b,0x0c,0x0d,0x0a,0x08,0x0a,0x0c,
	0x09,0x0d,0x10,0x09,0x0c,0x09,0x0b,0x0a,
	0x09,0x07,0x0c,0x0a,0x0a,0x0c,0x0c,0x0a,
	0x09,0x13,0x10,0x10,0x0d,0x0e,0x0e,0x0e,
	0x09,0x0d,0x0d,0x10,0x0d,0x0c,0x0b,0x0d,
	0x09,0x0c,0x0a,0x09,0x0c,0x0c,0x09,0x09,
	0x09,0x15,0x13,0x15,0x13,0x0d,0x0e,0x11,
	/* 9 */
	0x0c,0x09,0x07,0x07,0x0a,0x0a,0x0a,0x09,
	0x0c,0x0c,0x0e,0x0a,0x0b,0x0b,0x0c,0x0c,
	0x0a,0x07,0x07,0x0c,0x0d,0x0a,0x0c,0x0a,
	0x10,0x07,0x0c,0x0a,0x0c,0x0c,0x0a,0x0e,
	0x0d,0x0f,0x13,0x0f,0x0d,0x0d,0x0d,0x0e,
	0x0d,0x0d,0x07,0x07,0x07,0x07,0x07,0x0a,
	0x0b,0x0b,0x0d,0x10,0x0e,0x0c,0x0a,0x0c,
	0x0e,0x09,0x0a,0x0a,0x08,0x0a,0x0c,0x0c,
	/* 10 */
	0x09,0x0c,0x0e,0x0d,0x0d,0x09,0x0c,0x08,
	0x09,0x0d,0x12,0x10,0x0d,0x10,0x0d,0x0e,
	0x09,0x0e,0x0d,0x11,0x11,0x0c,0x0d,0x0a,
	0x09,0x11,0x11,0x11,0x0d,0x0d,0x0d,0x0d,
	0x0a,0x11,0x01,0x17,0x0c,0x0d,0x0c,0x0a,
	0x0a,0x11,0x17,0x14,0x0c,0x0c,0x0c,0x0a,
	0x0a,0x11,0x11,0x10,0x10,0x0d,0x09,0x07,
	0x18,0x1d,0x1d,0x1d,0x1d,0x1d,0x1d,0x1d,
	/* 11 */
	0x09,0x0a,0x0d,0x0c,0x09,0x0c,0x0a,0x0a,
	0x11,0x0d,0x0c,0x09,0x0a,0x0a,0x0f,0x0f,
	0x0a,0x0b,0x14,0x12,0x0d,0x0d,0x0a,0x0a,
	0x0e,0x0f,0x0d,0x0d,0x0e,0x0a,0x0a,0x0e,
	0x09,0x0a,0x0f,0x0c,0x0d,0x0c,0x0c,0x0a,
	0x0e,0x0c,0x09,0x0c,0x0c,0x0c,0x09,0x0c,
	0x07,0x05,0x09,0x09,0x08,0x05,0x05,0x05,
	0x1d,0x1d,0x1d,0x1d,0x1d,0x1d,0x1d,0x1d,
	/* 12 */
	0x0d,0x0c,0x0e,0x0a,0x0a,0x0c,0x0c,0x0c,
	0x0b,0x09,0x09,0x0f,0x10,0x0c,0x0d,0x09,
	0x0c,0x0a,0x0a,0x08,0x07,0x0a,0x09,0x0a,
	0x07,0x07,0x08,0x08,0x09,0x0b,0x0b,0x09,
	0x0e,0x10,0x0e,0x0c,0x0a,0x0c,0x0c,0x0c,
	0x0a,0x08,0x08,0x0a,0x0c,0x0b,0x0b,0x0c,
	0x0c,0x0d,0x09,0x0b,0x0c,0x0d,0x0b,0x0c,
	0x0c,0x0d,0x0a,0x0b,0x0b,0x0c,0x0b,0x10,
	/* 13 */
	0x0d,0x0e,0x0e,0x10,0x0e,0x0d,0x0c,0x1b,
	0x0c,0x0c,0x0a,0x0c,0x12,0x12,0x15,0x1b,
	0x09,0x0c,0x09,0x0b,0x0b,0x0d,0x0a,0x1b,
	0x0a,0x0c,0x0c,0x0f,0x0d,0x14,0x0f,0x1b,
	0x0d,0x09,0x0c,0x0d,0x0d,0x0d,0x0c,0x1b,
	0x0c,0x0c,0x0e,0x0a,0x10,0x0e,0x0c,0x1b,
	0x0b,0x0d,0x0d,0x12,0x14,0x14,0x17,0x1a,
	0x0f,0x0c,0x0e,0x10,0x0e,0x0e,0x09,0x1b,
	/* 14 */
	0x05,0x07,0x0a,0x0c,0x09,0x0a,0x0a,0x09,
	0x0f,0x0e,0x0e,0x0b,0x09,0x0e,0x0f,0x0c,
	0x0c,0x0a,0x09,0x0d,0x0d,0x0c,0x0d,0x0d,
	0x10,0x0d,0x0f,0x0d,0x0f,0x0f,0x0f,0x0e,
	0x0b,0x0c,0x0c,0x09,0x0a,0x0a,0x0c,0x0c,
	0x0c,0x06,0x08,0x08,0x0a,0x09,0x0b,0x0a,
	0x06,0x07,0x07,0x04,0x05,0x05,0x05,0x07,
	0x1d,0x1d,0x1d,0x1d,0x1d,0x1d,0x1d,0x1d,
	/* 15 */
	0x0a,0x0e,0x0c,0x0c,0x0e,0x0e,0x0c,0x1b,
	0x13,0x11,0x12,0x15,0x0f,0x12,0x10,0x1b,
	0x0d,0x0e,0x10,0x15,0x0e,0x0d,0x0f,0x1b,
	0x0f,0x10,0x0d,0x0f,0x0f,0x0d,0x15,0x1a,
	0x0f,0x0c,0x0f,0x0e,0x01,0x17,0x0c,0x1b,
	0x0a,0x09,0x09,0x0f,0x17,0x14,0x0c,0x1b,
	0x07,0x0a,0x0a,0x07,0x07,0x0a,0x07,0x1b,
	0x1d,0x1d,0x1c,0x1d,0x1d,0x1c,0x1b,0x1b,
	
};