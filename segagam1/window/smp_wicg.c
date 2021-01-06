/*	Copyright(c) 1994 SEGA			*/
/*
 *
 * SAMPLE CG & MAP FILE 
 *
 */


#include "smp_bios.h"

#include "smp_wicg.h"




const Uint8 lrun1_cg[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x30,0x00,0x00,0x00,
	0x00,0x00,0x33,0x32,0x33,0x33,0x00,0x20,0x00,0x03,0x23,0x32,0xb3,0x33,0x33,0x20,
	0x00,0x00,0x22,0x32,0xb3,0x33,0x33,0x30,0x00,0x00,0x02,0x33,0x44,0x43,0x33,0x43,
	0x00,0x00,0x22,0x24,0x66,0x64,0x34,0x64,0x00,0x02,0x22,0x46,0x66,0x66,0x44,0x65,
	0x00,0x22,0x22,0x46,0x66,0x91,0x68,0x15,0x00,0x22,0x22,0x46,0x66,0x91,0x68,0x15,
	0x00,0x02,0x22,0x46,0x66,0x91,0x68,0x15,0x00,0x00,0x22,0x46,0x66,0x91,0x68,0x94,
	0x00,0x00,0x02,0x24,0x66,0x66,0xa1,0x14,0x00,0x00,0x22,0x2b,0xaa,0xaa,0xaa,0xb0,
	0x00,0x02,0x22,0x22,0xbb,0xbb,0xbb,0x90,0x00,0x00,0xbb,0xb2,0x22,0x2b,0x88,0x89,
	0x07,0x7b,0x00,0x22,0xbb,0xb2,0x98,0x89,0x86,0x68,0x00,0x22,0xbb,0xb2,0x09,0x90,
	0x87,0x78,0x02,0x22,0x2b,0x22,0x00,0x0c,0x08,0x80,0xd0,0x02,0x34,0x20,0x00,0xcc,
	0x00,0x0d,0xd8,0x22,0x00,0x48,0xcc,0xc0,0x00,0xdd,0xd8,0x00,0x00,0x8c,0xcc,0x00,
	0x00,0xdd,0x00,0x00,0x00,0x0c,0xc0,0x00,0x0d,0xd0,0x00,0x00,0x00,0x00,0x00,0x00,

	/*** dummy data ***/
};

const Uint8 lrun2_cg[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x33,0x30,0x00,0x00,0x00,
	0x00,0x03,0x33,0x32,0x33,0x33,0x00,0x20,0x00,0x32,0x23,0x32,0xb3,0x33,0x33,0x20,
	0x00,0x00,0x23,0x32,0xbb,0x33,0x33,0x30,0x00,0x00,0x03,0x33,0x34,0x43,0x33,0x34,
	0x00,0x03,0x33,0x33,0x46,0x64,0x33,0x45,0x00,0x22,0x23,0x34,0x66,0x66,0x44,0x45,
	0x02,0x22,0x22,0x34,0x66,0x69,0x16,0x81,0x02,0x22,0x22,0x34,0x66,0x69,0x16,0x81,
	0x00,0x02,0x22,0x34,0x66,0x69,0x16,0x81,0x00,0x00,0x22,0x34,0x66,0x69,0x16,0x89,
	0x00,0x00,0x22,0x23,0x46,0x66,0x6a,0x11,0x00,0x02,0x22,0x22,0xba,0xaa,0xaa,0xb0,
	0x00,0x22,0x22,0x22,0x2b,0xbb,0xbb,0x00,0x00,0x00,0x0b,0xb2,0x22,0x2b,0x00,0x00,
	0x00,0x00,0xb2,0x66,0x8b,0xb2,0x00,0x00,0x00,0x00,0x0b,0x77,0x8b,0xb2,0x00,0x00,
	0x00,0x00,0x02,0x88,0x8b,0xb2,0x00,0x00,0x00,0x00,0x00,0x02,0x22,0x20,0x00,0x00,
	0x00,0x00,0x0d,0xc8,0x22,0x00,0x00,0x00,0x00,0x00,0x0d,0xcc,0xcc,0x00,0x00,0x00,
	0x00,0x00,0x00,0xdd,0xdd,0xdd,0x00,0x00,0x00,0x00,0x00,0x0d,0xdc,0x00,0x00,0x00,

	/*** dummy data ***/
};

const Uint8 lrun3_cg[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x33,0x30,0x00,0x00,0x00,
	0x00,0x33,0x33,0x33,0x23,0x33,0x02,0x00,0x03,0x22,0x33,0x33,0x23,0x33,0x33,0x00,
	0x00,0x00,0x23,0x33,0x22,0x33,0x33,0x30,0x00,0x00,0x33,0x33,0x33,0x44,0x33,0x34,
	0x00,0x33,0x33,0x33,0x34,0x66,0x43,0x45,0x03,0x22,0x33,0x33,0x46,0x66,0x64,0x45,
	0x32,0x22,0x23,0x33,0x46,0x66,0x91,0x65,0x22,0x22,0x22,0x33,0x46,0x66,0x91,0x65,
	0x00,0x22,0x22,0x33,0x46,0x66,0x91,0x65,0x00,0x02,0x22,0x33,0x46,0x66,0x91,0x65,
	0x00,0x02,0x22,0x23,0x34,0x66,0x66,0xa1,0x00,0x22,0x22,0x22,0x3b,0xaa,0xaa,0xb0,
	0x02,0x22,0x22,0x22,0x22,0xbb,0x88,0x00,0x00,0x00,0x00,0x2b,0x22,0x28,0x77,0x80,
	0x00,0x09,0x9b,0x22,0xbb,0xb8,0x77,0x80,0x00,0x98,0x89,0x22,0x22,0xb2,0x88,0x00,
	0x00,0x98,0x82,0x22,0x22,0xb2,0x00,0x0d,0x00,0x09,0xd0,0x02,0x42,0x22,0x0d,0xdd,
	0x00,0x0d,0xc8,0x24,0x00,0x02,0x8d,0xd0,0x00,0xdc,0xc8,0x00,0x00,0x08,0xdd,0x00,
	0x0d,0xcc,0x00,0x00,0x00,0x0d,0xdd,0x00,0xdc,0x00,0x00,0x00,0x00,0x00,0xd0,0x00,

	/*** dummy data ***/
};


const Uint8 lrun4_cg[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x33,0x30,0x00,0x00,0x00,
	0x00,0x03,0x33,0x32,0x33,0x33,0x00,0x20,0x00,0x32,0x23,0x32,0xb3,0x33,0x33,0x20,
	0x00,0x00,0x23,0x32,0xbb,0x33,0x33,0x30,0x00,0x00,0x03,0x33,0x34,0x43,0x33,0x34,
	0x00,0x03,0x33,0x33,0x46,0x64,0x33,0x45,0x00,0x22,0x23,0x34,0x66,0x66,0x44,0x45,
	0x02,0x22,0x22,0x34,0x66,0x69,0x16,0x81,0x02,0x22,0x22,0x34,0x66,0x69,0x16,0x81,
	0x00,0x02,0x22,0x34,0x66,0x69,0x16,0x81,0x00,0x00,0x22,0x34,0x66,0x69,0x16,0x89,
	0x00,0x00,0x22,0x23,0x46,0x66,0x6a,0x11,0x00,0x02,0x22,0x22,0xba,0xaa,0xaa,0xb0,
	0x00,0x22,0x22,0x22,0x77,0xbb,0xbb,0x00,0x00,0x00,0x0b,0xcc,0xc7,0xb8,0x89,0x00,
	0x00,0x0c,0xcc,0xcc,0xc8,0x28,0x89,0x00,0x00,0xcc,0xcc,0xc8,0x8b,0x2d,0x90,0x00,
	0x0c,0xcc,0x02,0x22,0xb2,0x20,0xd0,0x00,0x0c,0x00,0x00,0x22,0x22,0x00,0xd0,0x00,
	0x0c,0x00,0x00,0x00,0x20,0x0d,0xd0,0x00,0x00,0x00,0x00,0x02,0x0d,0xdd,0x00,0x00,
	0x00,0x00,0x00,0xdd,0xdd,0xd0,0x00,0x00,0x00,0x00,0x0d,0xdd,0xdd,0x00,0x00,0x00,

	/*** dummy data ***/
};

const Uint8 lrun5_cg[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x33,0x33,0x30,0x00,0x00,0x00,
	0x03,0x33,0x33,0x33,0x33,0x33,0x02,0x00,0x00,0x02,0x33,0x2b,0xb3,0x33,0x33,0x20,
	0x00,0x00,0x23,0x32,0xbb,0x33,0x33,0x30,0x00,0x00,0x03,0x33,0x34,0x43,0x33,0x34,
	0x00,0x00,0x33,0x33,0x46,0x64,0x33,0x45,0x00,0x32,0x23,0x34,0x66,0x66,0x44,0x45,
	0x02,0x22,0x22,0x34,0x66,0x69,0x16,0x81,0x22,0x22,0x22,0x34,0x66,0x69,0x16,0x81,
	0x00,0x02,0x22,0x34,0x66,0x69,0x16,0x81,0x00,0x00,0x22,0x34,0x66,0x69,0x16,0x89,
	0x00,0x00,0x22,0x23,0x46,0x66,0x6a,0x11,0x00,0x22,0x22,0x22,0xba,0xaa,0xaa,0xb0,
	0x02,0x22,0x22,0x22,0x77,0xbb,0xbb,0x00,0x00,0x00,0x0b,0x87,0xcc,0xc8,0x89,0x00,
	0x00,0x00,0xb2,0x88,0x88,0xcc,0x89,0x00,0x00,0xd0,0xbb,0x28,0x8b,0x2c,0xc0,0x00,
	0x0d,0xd0,0x22,0x2b,0xbb,0x2c,0xc0,0x00,0x0d,0xdd,0x20,0x24,0x44,0x4c,0xc0,0x00,
	0x00,0xdd,0x00,0x00,0x00,0xcc,0xc0,0x00,0x00,0xdd,0xd0,0x00,0x00,0x0c,0x00,0x00,
	0x00,0x0d,0xd0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xdd,0xdd,0x00,0x00,0x00,0x00,

	/*** dummy data ***/
};

const Uint16 lrun_cl[] = {
	0x7298, 0x0000, 0x6084, 0x7108, 0x718C, 0x7210, 0x739C, 0x5294, 
	0x4210, 0x2108, 0x429C, 0x2194, 0x001C, 0x0010, 0x0008, 0x039c, 
};



const Uint8 kemu0_cg[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x66,0x60,0x00,0x00,0x00,
	0x00,0x00,0x76,0x66,0x66,0x00,0x00,0x00,0x00,0x00,0x76,0x66,0x66,0x60,0x00,0x00,
	0x00,0x66,0x76,0x76,0x66,0x67,0x77,0x00,0x06,0x66,0x77,0x66,0x66,0x66,0x66,0x70,
	0x66,0x66,0x76,0x66,0x66,0x66,0x66,0x70,0x66,0x66,0x76,0x66,0x66,0x66,0x66,0x70,
	0x76,0x66,0x66,0x66,0x66,0x66,0x66,0x70,0x76,0x66,0x67,0x66,0x66,0x66,0x66,0x70,
	0x07,0x66,0x76,0x66,0x67,0x66,0x67,0x70,0x00,0x77,0x76,0x66,0x67,0x66,0x77,0x00,
	0x00,0x77,0x77,0x66,0x77,0x67,0x70,0x00,0x00,0x07,0x77,0x77,0x77,0x77,0x70,0x00,
	0x00,0x00,0x07,0x77,0x07,0x77,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*** dummy data ***/
};

const Uint8 kemu1_cg[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x66,0x00,0x00,0x00,0x00,
	0x00,0x00,0x06,0x66,0x60,0x00,0x00,0x00,0x00,0x00,0x07,0x66,0x66,0x00,0x00,0x00,
	0x00,0x00,0x07,0x76,0x66,0x06,0x66,0x00,0x07,0x66,0x00,0x77,0x76,0x66,0x66,0x60,
	0x76,0x66,0x66,0x76,0x66,0x66,0x66,0x60,0x76,0x66,0x66,0x66,0x67,0x66,0x66,0x70,
	0x77,0x66,0x76,0x66,0x66,0x77,0x66,0x70,0x07,0x77,0x76,0x66,0x67,0x70,0x77,0x00,
	0x00,0x07,0x66,0x66,0x67,0x00,0x00,0x00,0x00,0x07,0x66,0x66,0x70,0x66,0x60,0x00,
	0x00,0x07,0x76,0x67,0x70,0x76,0x66,0x00,0x00,0x00,0x77,0x77,0x00,0x76,0x67,0x00,
	0x00,0x00,0x00,0x00,0x00,0x07,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*** dummy data ***/
};

const Uint8 kemu2_cg[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x06,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x76,0x60,0x06,0x60,0x00,
	0x00,0x76,0x00,0x77,0x70,0x07,0x66,0x00,0x00,0x77,0x00,0x00,0x00,0x00,0x77,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x70,0x00,0x00,
	0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x06,0x60,0x70,0x00,0x06,0x60,0x00,
	0x00,0x07,0x70,0x00,0x00,0x07,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*** dummy data ***/
};

const Uint8 kemu3_cg[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x06,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x70,0x07,0x70,0x00,
	0x00,0x70,0x00,0x00,0x00,0x07,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x70,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x06,0x00,0x00,0x00,0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*** dummy data ***/
};


const Uint8 kemu4_cg[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*** dummy data ***/
};



/* POWER UP ITEM CG */
const Uint8 SPEED_cg[] = {
	0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0xbf,0xff,0xff,0xff,0xff,0xff,0xff,0xfb,
	0xf1,0x11,0x11,0x11,0x11,0x11,0x11,0x1f,0xf1,0x11,0x11,0x11,0x11,0x11,0x11,0x1f,
	0xf1,0x11,0x11,0x11,0x11,0x11,0x11,0x1f,0xf1,0x11,0x77,0x66,0x61,0x11,0x11,0x1f,
	0xf1,0x11,0x87,0x76,0x91,0x11,0x11,0x1f,0xf1,0x11,0x77,0x66,0x61,0x11,0x11,0x1f,
	0xf1,0x11,0xdc,0xcd,0xd1,0x11,0x11,0x1f,0xf1,0x11,0xcc,0xcc,0xcc,0x11,0x11,0x1f,
	0xf1,0x11,0xcc,0xcc,0xcc,0xcc,0xd1,0x1f,0xf1,0x11,0xee,0xee,0xee,0xee,0xe1,0x1f,
	0xf1,0x11,0x11,0x11,0x11,0x11,0x11,0x1f,0xf1,0x11,0x11,0x11,0x11,0x11,0x11,0x1f,
	0xbf,0xff,0xff,0xff,0xff,0xff,0xff,0xfb,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,

	/*** dummy data ***/
};


const Uint8 RING10_cg[] = {
	0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0xbf,0xff,0xff,0xff,0xff,0xff,0xff,0xfb,
	0xf1,0x11,0x11,0x11,0x11,0x11,0x11,0x1f,0xf1,0x11,0x11,0x11,0x11,0x11,0x11,0x1f,
	0xf1,0x11,0x11,0xff,0x66,0x11,0x11,0x1f,0xf1,0x11,0x1f,0xbb,0xba,0x61,0x11,0x1f,
	0xf1,0x11,0xfb,0x11,0x11,0xa6,0x11,0x1f,0xf1,0x11,0xfb,0x11,0x11,0xb6,0x11,0x1f,
	0xf1,0x11,0xf6,0x11,0x11,0xbf,0x11,0x1f,0xf1,0x11,0xf6,0x11,0x11,0xbf,0x11,0x1f,
	0xf1,0x11,0x1f,0x66,0xbb,0xf1,0x11,0x1f,0xf1,0x11,0x11,0xff,0xff,0x11,0x11,0x1f,
	0xf1,0x11,0x11,0x11,0x11,0x11,0x11,0x1f,0xf1,0x11,0x11,0x11,0x11,0x11,0x11,0x1f,
	0xbf,0xff,0xff,0xff,0xff,0xff,0xff,0xfb,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,

	/*** dummy data ***/
};



/* L_RUN CGDATA information */
SPRCG const LRUN1_info = {
    lrun1_cg,
    (sizeof lrun1_cg) / (sizeof lrun1_cg[0]),
    lrun_cl,
    (sizeof lrun_cl) / (sizeof lrun_cl[0]),
    };

SPRCG const LRUN2_info = {
    lrun2_cg,
    (sizeof lrun2_cg) / (sizeof lrun2_cg[0]),
    lrun_cl,
    (sizeof lrun_cl) / (sizeof lrun_cl[0]),
    };

SPRCG const LRUN3_info = {
    lrun3_cg,
    (sizeof lrun3_cg) / (sizeof lrun3_cg[0]),
    lrun_cl,
    (sizeof lrun_cl) / (sizeof lrun_cl[0]),
    };

SPRCG const LRUN4_info = {
    lrun4_cg,
    (sizeof lrun4_cg) / (sizeof lrun4_cg[0]),
    lrun_cl,
    (sizeof lrun_cl) / (sizeof lrun_cl[0]),
    };

SPRCG const LRUN5_info = {
    lrun5_cg,
    (sizeof lrun5_cg) / (sizeof lrun5_cg[0]),
    lrun_cl,
    (sizeof lrun_cl) / (sizeof lrun_cl[0]),
    };

