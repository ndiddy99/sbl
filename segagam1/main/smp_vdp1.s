	.file	"smp_vdp1.c"
	.data

! Hitachi SH cc1 (cygnus-2.6-95q1-SOA-950317) (release I-1) arguments: -O
! -g -fdefer-pop -fomit-frame-pointer -fthread-jumps -fpeephole
! -ffunction-cse -finline -freg-struct-return -fdelayed-branch -fcommon
! -fgnu-linker -m2

! 1010 500
gcc2_compiled.:
___gnu_compiled_c:
	.text
	.def	_Uint8;	.scl	13;	.type	014;	.endef
	.def	_Sint8;	.scl	13;	.type	02;	.endef
	.def	_Uint16;	.scl	13;	.type	015;	.endef
	.def	_Sint16;	.scl	13;	.type	03;	.endef
	.def	_Uint32;	.scl	13;	.type	017;	.endef
	.def	_Sint32;	.scl	13;	.type	05;	.endef
	.def	_Float32;	.scl	13;	.type	06;	.endef
	.def	_Float64;	.scl	13;	.type	07;	.endef
	.def	_Int;	.scl	13;	.type	04;	.endef
	.def	_Bool;	.scl	13;	.type	04;	.endef
	.def	_BooleanLogic;	.scl	15;	.type	012;	.size	4;	.endef
	.def	_FALSE;	.val	0;	.scl	16;	.type	013;	.endef
	.def	_TRUE;	.val	1;	.scl	16;	.type	013;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_BooleanLogic;	.size	4;	.endef
	.def	_BooleanSwitch;	.scl	15;	.type	012;	.size	4;	.endef
	.def	_OFF;	.val	0;	.scl	16;	.type	013;	.endef
	.def	_ON;	.val	1;	.scl	16;	.type	013;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_BooleanSwitch;	.size	4;	.endef
	.def	_Judgement;	.scl	15;	.type	012;	.size	4;	.endef
	.def	_OK;	.val	0;	.scl	16;	.type	013;	.endef
	.def	_NG;	.val	-1;	.scl	16;	.type	013;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_Judgement;	.size	4;	.endef
	.def	_Rgb16;	.scl	13;	.type	015;	.endef
	.def	_Rgb32;	.scl	13;	.type	017;	.endef
	.def	_XyInt;	.scl	10;	.type	010;	.size	4;	.endef
	.def	_x;	.val	0;	.scl	8;	.type	03;	.endef
	.def	_y;	.val	2;	.scl	8;	.type	03;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_XyInt;	.size	4;	.endef
	.def	_XyInt;	.scl	13;	.tag	_XyInt;	.size	4;	.type	010;	.endef
	.def	_Fixed32;	.scl	13;	.type	05;	.endef
	.def	_MthXy;	.scl	10;	.type	010;	.size	8;	.endef
	.def	_x;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_y;	.val	4;	.scl	8;	.type	05;	.endef
	.def	.eos;	.val	8;	.scl	102;	.tag	_MthXy;	.size	8;	.endef
	.def	_MthXy;	.scl	13;	.tag	_MthXy;	.size	8;	.type	010;	.endef
	.def	_MthXyz;	.scl	10;	.type	010;	.size	12;	.endef
	.def	_x;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_y;	.val	4;	.scl	8;	.type	05;	.endef
	.def	_z;	.val	8;	.scl	8;	.type	05;	.endef
	.def	.eos;	.val	12;	.scl	102;	.tag	_MthXyz;	.size	12;	.endef
	.def	_MthXyz;	.scl	13;	.tag	_MthXyz;	.size	12;	.type	010;	.endef
	.def	_MthMatrix;	.scl	10;	.type	010;	.size	48;	.endef
	.def	_val;	.val	0;	.scl	8;	.dim	3,4;	.size	48;	.type	0365;	.endef
	.def	.eos;	.val	48;	.scl	102;	.tag	_MthMatrix;	.size	48;	.endef
	.def	_MthMatrix;	.scl	13;	.tag	_MthMatrix;	.size	48;	.type	010;	.endef
	.def	_MthMatrixTbl;	.scl	10;	.type	010;	.size	12;	.endef
	.def	_stackSize;	.val	0;	.scl	8;	.type	015;	.endef
	.def	_current;	.val	4;	.scl	8;	.tag	_MthMatrix;	.size	48;	.type	030;	.endef
	.def	_stack;	.val	8;	.scl	8;	.tag	_MthMatrix;	.size	48;	.type	030;	.endef
	.def	.eos;	.val	12;	.scl	102;	.tag	_MthMatrixTbl;	.size	12;	.endef
	.def	_MthMatrixTbl;	.scl	13;	.tag	_MthMatrixTbl;	.size	12;	.type	010;	.endef
	.def	_MthViewLight;	.scl	10;	.type	010;	.size	108;	.endef
	.def	_viewMatrix;	.val	0;	.scl	8;	.tag	_MthMatrix;	.size	48;	.type	010;	.endef
	.def	_lightVector;	.val	48;	.scl	8;	.tag	_MthXyz;	.size	12;	.type	010;	.endef
	.def	_worldMatrix;	.val	60;	.scl	8;	.tag	_MthMatrix;	.size	48;	.type	010;	.endef
	.def	.eos;	.val	108;	.scl	102;	.tag	_MthViewLight;	.size	108;	.endef
	.def	_MthViewLight;	.scl	13;	.tag	_MthViewLight;	.size	108;	.type	010;	.endef
	.def	_MthPolyTransParm;	.scl	10;	.type	010;	.size	56;	.endef
	.def	_viewLight;	.val	0;	.scl	8;	.tag	_MthViewLight;	.size	108;	.type	030;	.endef
	.def	_surfCount;	.val	4;	.scl	8;	.type	017;	.endef
	.def	_surfPoint;	.val	8;	.scl	8;	.tag	_MthXyz;	.size	12;	.type	030;	.endef
	.def	_surfNormal;	.val	12;	.scl	8;	.tag	_MthXyz;	.size	12;	.type	030;	.endef
	.def	_surfBright;	.val	16;	.scl	8;	.type	025;	.endef
	.def	_transViewVertCount;	.val	20;	.scl	8;	.type	017;	.endef
	.def	_transViewVertSrc;	.val	24;	.scl	8;	.tag	_MthXyz;	.size	12;	.type	030;	.endef
	.def	_transViewVertAns;	.val	28;	.scl	8;	.tag	_MthXyz;	.size	12;	.type	030;	.endef
	.def	_gourVertCount;	.val	32;	.scl	8;	.type	017;	.endef
	.def	_vertNormal;	.val	36;	.scl	8;	.tag	_MthXyz;	.size	12;	.type	030;	.endef
	.def	_vertBright;	.val	40;	.scl	8;	.type	025;	.endef
	.def	_transWorldVertCount;	.val	44;	.scl	8;	.type	017;	.endef
	.def	_transWorldVertSrc;	.val	48;	.scl	8;	.tag	_MthXyz;	.size	12;	.type	030;	.endef
	.def	_transWorldVertAns;	.val	52;	.scl	8;	.tag	_MthXyz;	.size	12;	.type	030;	.endef
	.def	.eos;	.val	56;	.scl	102;	.tag	_MthPolyTransParm;	.size	56;	.endef
	.def	_MthPolyTransParm;	.scl	13;	.tag	_MthPolyTransParm;	.size	56;	.type	010;	.endef
	.def	_.0fake;	.scl	10;	.type	010;	.size	40;	.endef
	.def	_num;	.val	0;	.scl	8;	.type	017;	.endef
	.def	_MTH_crvh;	.val	4;	.scl	8;	.type	025;	.endef
	.def	_MTH_crvd;	.val	8;	.scl	8;	.type	025;	.endef
	.def	_MTH_crvx;	.val	12;	.scl	8;	.type	025;	.endef
	.def	_MTH_crvy;	.val	16;	.scl	8;	.type	025;	.endef
	.def	_MTH_crvz;	.val	20;	.scl	8;	.type	025;	.endef
	.def	_MTH_crvp;	.val	24;	.scl	8;	.type	025;	.endef
	.def	_MTH_crva;	.val	28;	.scl	8;	.type	025;	.endef
	.def	_MTH_crvb;	.val	32;	.scl	8;	.type	025;	.endef
	.def	_MTH_crvc;	.val	36;	.scl	8;	.type	025;	.endef
	.def	.eos;	.val	40;	.scl	102;	.tag	_.0fake;	.size	40;	.endef
	.def	_MthCurveWork;	.scl	13;	.tag	_.0fake;	.size	40;	.type	010;	.endef
	.def	_SprSpStatus;	.scl	10;	.type	010;	.size	16;	.endef
	.def	_frameChgMode;	.val	0;	.scl	8;	.type	015;	.endef
	.def	_frameEraseMode;	.val	2;	.scl	8;	.type	015;	.endef
	.def	_vbInterval;	.val	4;	.scl	8;	.type	015;	.endef
	.def	_eraseData;	.val	6;	.scl	8;	.type	015;	.endef
	.def	_eraseLeftX;	.val	8;	.scl	8;	.type	015;	.endef
	.def	_eraseTopY;	.val	10;	.scl	8;	.type	015;	.endef
	.def	_eraseRightX;	.val	12;	.scl	8;	.type	015;	.endef
	.def	_eraseBotY;	.val	14;	.scl	8;	.type	015;	.endef
	.def	.eos;	.val	16;	.scl	102;	.tag	_SprSpStatus;	.size	16;	.endef
	.def	_SprSpStatus;	.scl	13;	.tag	_SprSpStatus;	.size	16;	.type	010;	.endef
	.def	_SprSpCmd;	.scl	10;	.type	010;	.size	32;	.endef
	.def	_control;	.val	0;	.scl	8;	.type	015;	.endef
	.def	_link;	.val	2;	.scl	8;	.type	015;	.endef
	.def	_drawMode;	.val	4;	.scl	8;	.type	015;	.endef
	.def	_color;	.val	6;	.scl	8;	.type	015;	.endef
	.def	_charAddr;	.val	8;	.scl	8;	.type	015;	.endef
	.def	_charSize;	.val	10;	.scl	8;	.type	015;	.endef
	.def	_ax;	.val	12;	.scl	8;	.type	03;	.endef
	.def	_ay;	.val	14;	.scl	8;	.type	03;	.endef
	.def	_bx;	.val	16;	.scl	8;	.type	03;	.endef
	.def	_by;	.val	18;	.scl	8;	.type	03;	.endef
	.def	_cx;	.val	20;	.scl	8;	.type	03;	.endef
	.def	_cy;	.val	22;	.scl	8;	.type	03;	.endef
	.def	_dx;	.val	24;	.scl	8;	.type	03;	.endef
	.def	_dy;	.val	26;	.scl	8;	.type	03;	.endef
	.def	_grshAddr;	.val	28;	.scl	8;	.type	015;	.endef
	.def	_dummy;	.val	30;	.scl	8;	.type	015;	.endef
	.def	.eos;	.val	32;	.scl	102;	.tag	_SprSpCmd;	.size	32;	.endef
	.def	_SprGourTbl;	.scl	10;	.type	010;	.size	8;	.endef
	.def	_entry;	.val	0;	.scl	8;	.dim	4;	.size	8;	.type	075;	.endef
	.def	.eos;	.val	8;	.scl	102;	.tag	_SprGourTbl;	.size	8;	.endef
	.def	_SprLookupTbl;	.scl	10;	.type	010;	.size	32;	.endef
	.def	_entry;	.val	0;	.scl	8;	.dim	16;	.size	32;	.type	075;	.endef
	.def	.eos;	.val	32;	.scl	102;	.tag	_SprLookupTbl;	.size	32;	.endef
	.def	_Vaddr;	.scl	13;	.type	015;	.endef
	.def	_SprSpCmd;	.scl	13;	.tag	_SprSpCmd;	.size	32;	.type	010;	.endef
	.def	_SprGourTbl;	.scl	13;	.tag	_SprGourTbl;	.size	8;	.type	010;	.endef
	.def	_SprLookupTbl;	.scl	13;	.tag	_SprLookupTbl;	.size	32;	.type	010;	.endef
	.def	_SprCharTbl;	.scl	13;	.type	010;	.endef
	.def	_SprCmdChain;	.scl	13;	.type	010;	.endef
	.def	_SprDrawPrtyBlk;	.scl	13;	.type	010;	.endef
	.def	_Spr2WorkArea;	.scl	13;	.type	010;	.endef
	.def	_SprAreaSize;	.scl	13;	.type	010;	.endef
	.def	_SprCharTbl;	.scl	10;	.type	010;	.size	10;	.endef
	.def	_addr;	.val	0;	.scl	8;	.type	015;	.endef
	.def	_xySize;	.val	2;	.scl	8;	.type	015;	.endef
	.def	_mode;	.val	4;	.scl	8;	.type	015;	.endef
	.def	_color;	.val	6;	.scl	8;	.type	015;	.endef
	.def	_size;	.val	8;	.scl	8;	.type	015;	.endef
	.def	.eos;	.val	10;	.scl	102;	.tag	_SprCharTbl;	.size	10;	.endef
	.def	_SprCmdChain;	.scl	10;	.type	010;	.size	4;	.endef
	.def	_nextNo;	.val	0;	.scl	8;	.type	015;	.endef
	.def	_cmdNo;	.val	2;	.scl	8;	.type	015;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_SprCmdChain;	.size	4;	.endef
	.def	_SprDrawPrtyBlk;	.scl	10;	.type	010;	.size	4;	.endef
	.def	_topNo;	.val	0;	.scl	8;	.type	015;	.endef
	.def	_botNo;	.val	2;	.scl	8;	.type	015;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_SprDrawPrtyBlk;	.size	4;	.endef
	.def	_Spr2WorkArea;	.scl	10;	.type	010;	.size	24;	.endef
	.def	_commandMax;	.val	0;	.scl	8;	.type	015;	.endef
	.def	_gourTblMax;	.val	2;	.scl	8;	.type	015;	.endef
	.def	_lookupTblMax;	.val	4;	.scl	8;	.type	015;	.endef
	.def	_charMax;	.val	6;	.scl	8;	.type	015;	.endef
	.def	_drawPrtyBlkMax;	.val	8;	.scl	8;	.type	015;	.endef
	.def	_charTbl;	.val	12;	.scl	8;	.tag	_SprCharTbl;	.size	10;	.type	030;	.endef
	.def	_cmdChain;	.val	16;	.scl	8;	.tag	_SprCmdChain;	.size	4;	.type	030;	.endef
	.def	_drawPrtyBlk;	.val	20;	.scl	8;	.tag	_SprDrawPrtyBlk;	.size	4;	.type	030;	.endef
	.def	.eos;	.val	24;	.scl	102;	.tag	_Spr2WorkArea;	.size	24;	.endef
	.def	_SprAreaSize;	.scl	10;	.type	010;	.size	16;	.endef
	.def	_commandMax;	.val	0;	.scl	8;	.type	015;	.endef
	.def	_gourTblMax;	.val	2;	.scl	8;	.type	015;	.endef
	.def	_lookupTblMax;	.val	4;	.scl	8;	.type	015;	.endef
	.def	_charMax;	.val	6;	.scl	8;	.type	015;	.endef
	.def	_drawPrtyBlkMax;	.val	8;	.scl	8;	.type	015;	.endef
	.def	_useCommandCount;	.val	10;	.scl	8;	.type	015;	.endef
	.def	_charRemainBlkCount;	.val	12;	.scl	8;	.type	015;	.endef
	.def	_charNullBlkMaxSize;	.val	14;	.scl	8;	.type	015;	.endef
	.def	.eos;	.val	16;	.scl	102;	.tag	_SprAreaSize;	.size	16;	.endef
	.def	_SprCluster;	.scl	13;	.type	010;	.endef
	.def	_SprObject3D;	.scl	13;	.type	010;	.endef
	.def	_SprSurface;	.scl	13;	.type	010;	.endef
	.def	_SprInbetInf;	.scl	13;	.type	010;	.endef
	.def	_SprTexture;	.scl	13;	.type	010;	.endef
	.def	_Spr3dStatus;	.scl	13;	.type	010;	.endef
	.def	_Spr3WorkArea;	.scl	13;	.type	010;	.endef
	.def	_SprCluster;	.scl	10;	.type	010;	.size	56;	.endef
	.def	_no;	.val	0;	.scl	8;	.type	015;	.endef
	.def	_angleSeq;	.val	2;	.scl	8;	.type	015;	.endef
	.def	_angle;	.val	4;	.scl	8;	.tag	_MthXyz;	.size	12;	.type	010;	.endef
	.def	_point;	.val	16;	.scl	8;	.tag	_MthXyz;	.size	12;	.type	010;	.endef
	.def	_object;	.val	28;	.scl	8;	.type	030;	.endef
	.def	_next;	.val	32;	.scl	8;	.tag	_SprCluster;	.size	56;	.type	030;	.endef
	.def	_child;	.val	36;	.scl	8;	.tag	_SprCluster;	.size	56;	.type	030;	.endef
	.def	_inbetInf;	.val	40;	.scl	8;	.type	030;	.endef
	.def	_transStart;	.val	44;	.scl	8;	.type	0221;	.endef
	.def	_transEnd;	.val	48;	.scl	8;	.type	0221;	.endef
	.def	_context;	.val	52;	.scl	8;	.type	021;	.endef
	.def	.eos;	.val	56;	.scl	102;	.tag	_SprCluster;	.size	56;	.endef
	.def	_SprObject3D;	.scl	10;	.type	010;	.size	40;	.endef
	.def	_no;	.val	0;	.scl	8;	.type	015;	.endef
	.def	_dispFlag;	.val	2;	.scl	8;	.type	015;	.endef
	.def	_vertCount;	.val	4;	.scl	8;	.type	015;	.endef
	.def	_surfaceCount;	.val	6;	.scl	8;	.type	015;	.endef
	.def	_vertPoint;	.val	8;	.scl	8;	.tag	_MthXyz;	.size	12;	.type	030;	.endef
	.def	_vertNormal;	.val	12;	.scl	8;	.tag	_MthXyz;	.size	12;	.type	030;	.endef
	.def	_surface;	.val	16;	.scl	8;	.type	030;	.endef
	.def	_surfaceNormal;	.val	20;	.scl	8;	.tag	_MthXyz;	.size	12;	.type	030;	.endef
	.def	_surfaceVert;	.val	24;	.scl	8;	.tag	_MthXyz;	.size	12;	.type	030;	.endef
	.def	_shdIdxTbl;	.val	28;	.scl	8;	.type	0135;	.endef
	.def	_surfNormK;	.val	32;	.scl	8;	.type	05;	.endef
	.def	_next;	.val	36;	.scl	8;	.tag	_SprObject3D;	.size	40;	.type	030;	.endef
	.def	.eos;	.val	40;	.scl	102;	.tag	_SprObject3D;	.size	40;	.endef
	.def	_SprSurface;	.scl	10;	.type	010;	.size	12;	.endef
	.def	_vertNo;	.val	0;	.scl	8;	.dim	4;	.size	8;	.type	075;	.endef
	.def	_drawMode;	.val	8;	.scl	8;	.type	015;	.endef
	.def	_color;	.val	10;	.scl	8;	.type	015;	.endef
	.def	.eos;	.val	12;	.scl	102;	.tag	_SprSurface;	.size	12;	.endef
	.def	_SprInbetInf;	.scl	10;	.type	010;	.size	24;	.endef
	.def	_vertCount;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_fromObj;	.val	4;	.scl	8;	.tag	_SprObject3D;	.size	40;	.type	030;	.endef
	.def	_fromVertNo;	.val	8;	.scl	8;	.type	035;	.endef
	.def	_toObj;	.val	12;	.scl	8;	.tag	_SprObject3D;	.size	40;	.type	030;	.endef
	.def	_toVertNo;	.val	16;	.scl	8;	.type	035;	.endef
	.def	_next;	.val	20;	.scl	8;	.tag	_SprInbetInf;	.size	24;	.type	030;	.endef
	.def	.eos;	.val	24;	.scl	102;	.tag	_SprInbetInf;	.size	24;	.endef
	.def	_SprTexture;	.scl	10;	.type	010;	.size	20;	.endef
	.def	_charNo;	.val	0;	.scl	8;	.type	015;	.endef
	.def	_colorMode;	.val	2;	.scl	8;	.type	015;	.endef
	.def	_color;	.val	4;	.scl	8;	.type	015;	.endef
	.def	_width;	.val	6;	.scl	8;	.type	015;	.endef
	.def	_height;	.val	8;	.scl	8;	.type	015;	.endef
	.def	_charData;	.val	12;	.scl	8;	.type	034;	.endef
	.def	_lookupTbl;	.val	16;	.scl	8;	.tag	_SprLookupTbl;	.size	32;	.type	030;	.endef
	.def	.eos;	.val	20;	.scl	102;	.tag	_SprTexture;	.size	20;	.endef
	.def	_Spr3WorkArea;	.scl	10;	.type	010;	.size	44;	.endef
	.def	_objSurfMax;	.val	0;	.scl	8;	.type	015;	.endef
	.def	_objVertMax;	.val	2;	.scl	8;	.type	015;	.endef
	.def	_surfBright;	.val	4;	.scl	8;	.dim	2;	.size	8;	.type	0165;	.endef
	.def	_vertBright;	.val	12;	.scl	8;	.dim	2;	.size	8;	.type	0165;	.endef
	.def	_coordView3D;	.val	20;	.scl	8;	.tag	_MthXyz;	.dim	2;	.size	8;	.type	0170;	.endef
	.def	_coordWorld3D;	.val	28;	.scl	8;	.tag	_MthXyz;	.dim	2;	.size	8;	.type	0170;	.endef
	.def	_coord2D;	.val	36;	.scl	8;	.tag	_XyInt;	.size	4;	.type	030;	.endef
	.def	_coord2DFlag;	.val	40;	.scl	8;	.type	034;	.endef
	.def	.eos;	.val	44;	.scl	102;	.tag	_Spr3WorkArea;	.size	44;	.endef
	.def	_Spr3dStatus;	.scl	10;	.type	010;	.size	84;	.endef
	.def	_lightAngle;	.val	0;	.scl	8;	.tag	_MthXyz;	.size	12;	.type	010;	.endef
	.def	_viewCoordPoint;	.val	12;	.scl	8;	.tag	_MthXyz;	.size	12;	.type	010;	.endef
	.def	_viewPoint;	.val	24;	.scl	8;	.tag	_MthXyz;	.size	12;	.type	010;	.endef
	.def	_viewAngle;	.val	36;	.scl	8;	.tag	_MthXyz;	.size	12;	.type	010;	.endef
	.def	_viewAngleSeq;	.val	48;	.scl	8;	.type	05;	.endef
	.def	_zSortMode;	.val	52;	.scl	8;	.type	05;	.endef
	.def	_zSortZMin;	.val	56;	.scl	8;	.type	05;	.endef
	.def	_zSortZMax;	.val	60;	.scl	8;	.type	05;	.endef
	.def	_clipZMin;	.val	64;	.scl	8;	.type	05;	.endef
	.def	_clipZMax;	.val	68;	.scl	8;	.type	05;	.endef
	.def	_clipLevel;	.val	72;	.scl	8;	.type	05;	.endef
	.def	_unitPixel;	.val	76;	.scl	8;	.tag	_MthXy;	.size	8;	.type	010;	.endef
	.def	.eos;	.val	84;	.scl	102;	.tag	_Spr3dStatus;	.size	84;	.endef
	.def	_SclSysreg;	.scl	10;	.type	010;	.size	40;	.endef
	.def	_tvmode;	.val	0;	.scl	8;	.type	015;	.endef
	.def	_extenbl;	.val	2;	.scl	8;	.type	015;	.endef
	.def	_tvstatus;	.val	4;	.scl	8;	.type	015;	.endef
	.def	_vramsize;	.val	6;	.scl	8;	.type	015;	.endef
	.def	_H_val;	.val	8;	.scl	8;	.type	015;	.endef
	.def	_V_val;	.val	10;	.scl	8;	.type	015;	.endef
	.def	_vramchg;	.val	12;	.scl	8;	.type	015;	.endef
	.def	_ramcontrl;	.val	14;	.scl	8;	.type	015;	.endef
	.def	_vramcyc;	.val	16;	.scl	8;	.dim	8;	.size	16;	.type	075;	.endef
	.def	_dispenbl;	.val	32;	.scl	8;	.type	015;	.endef
	.def	_mosaic;	.val	34;	.scl	8;	.type	015;	.endef
	.def	_specialcode_sel;	.val	36;	.scl	8;	.type	015;	.endef
	.def	_specialcode;	.val	38;	.scl	8;	.type	015;	.endef
	.def	.eos;	.val	40;	.scl	102;	.tag	_SclSysreg;	.size	40;	.endef
	.def	_SclSysreg;	.scl	13;	.tag	_SclSysreg;	.size	40;	.type	010;	.endef
	.def	_SclDataset;	.scl	10;	.type	010;	.size	72;	.endef
	.def	_charcontrl0;	.val	0;	.scl	8;	.type	015;	.endef
	.def	_charcontrl1;	.val	2;	.scl	8;	.type	015;	.endef
	.def	_bmpalnum0;	.val	4;	.scl	8;	.type	015;	.endef
	.def	_bmpalnum1;	.val	6;	.scl	8;	.type	015;	.endef
	.def	_patnamecontrl;	.val	8;	.scl	8;	.dim	5;	.size	10;	.type	075;	.endef
	.def	_platesize;	.val	18;	.scl	8;	.type	015;	.endef
	.def	_mapoffset0;	.val	20;	.scl	8;	.type	015;	.endef
	.def	_mapoffset1;	.val	22;	.scl	8;	.type	015;	.endef
	.def	_normap;	.val	24;	.scl	8;	.dim	8;	.size	16;	.type	075;	.endef
	.def	_rotmap;	.val	40;	.scl	8;	.dim	16;	.size	32;	.type	075;	.endef
	.def	.eos;	.val	72;	.scl	102;	.tag	_SclDataset;	.size	72;	.endef
	.def	_SclDataset;	.scl	13;	.tag	_SclDataset;	.size	72;	.type	010;	.endef
	.def	_SclNorscl;	.scl	10;	.type	010;	.size	64;	.endef
	.def	_n0_move_x;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_n0_move_y;	.val	4;	.scl	8;	.type	05;	.endef
	.def	_n0_delta_x;	.val	8;	.scl	8;	.type	05;	.endef
	.def	_n0_delta_y;	.val	12;	.scl	8;	.type	05;	.endef
	.def	_n1_move_x;	.val	16;	.scl	8;	.type	05;	.endef
	.def	_n1_move_y;	.val	20;	.scl	8;	.type	05;	.endef
	.def	_n1_delta_x;	.val	24;	.scl	8;	.type	05;	.endef
	.def	_n1_delta_y;	.val	28;	.scl	8;	.type	05;	.endef
	.def	_n2_move_x;	.val	32;	.scl	8;	.type	015;	.endef
	.def	_n2_move_y;	.val	34;	.scl	8;	.type	015;	.endef
	.def	_n3_move_x;	.val	36;	.scl	8;	.type	015;	.endef
	.def	_n3_move_y;	.val	38;	.scl	8;	.type	015;	.endef
	.def	_zoomenbl;	.val	40;	.scl	8;	.type	015;	.endef
	.def	_linecontrl;	.val	42;	.scl	8;	.type	015;	.endef
	.def	_celladdr;	.val	44;	.scl	8;	.type	017;	.endef
	.def	_lineaddr;	.val	48;	.scl	8;	.dim	2;	.size	8;	.type	077;	.endef
	.def	_linecolmode;	.val	56;	.scl	8;	.type	017;	.endef
	.def	_backcolmode;	.val	60;	.scl	8;	.type	017;	.endef
	.def	.eos;	.val	64;	.scl	102;	.tag	_SclNorscl;	.size	64;	.endef
	.def	_SclNorscl;	.scl	13;	.tag	_SclNorscl;	.size	64;	.type	010;	.endef
	.def	_SclRotscl;	.scl	10;	.type	010;	.size	16;	.endef
	.def	_paramode;	.val	0;	.scl	8;	.type	015;	.endef
	.def	_paramcontrl;	.val	2;	.scl	8;	.type	015;	.endef
	.def	_k_contrl;	.val	4;	.scl	8;	.type	015;	.endef
	.def	_k_offset;	.val	6;	.scl	8;	.type	015;	.endef
	.def	_mapover;	.val	8;	.scl	8;	.dim	2;	.size	4;	.type	075;	.endef
	.def	_paramaddr;	.val	12;	.scl	8;	.type	017;	.endef
	.def	.eos;	.val	16;	.scl	102;	.tag	_SclRotscl;	.size	16;	.endef
	.def	_SclRotscl;	.scl	13;	.tag	_SclRotscl;	.size	16;	.type	010;	.endef
	.def	_SclWinscl;	.scl	10;	.type	010;	.size	32;	.endef
	.def	_win0_start;	.val	0;	.scl	8;	.dim	2;	.size	4;	.type	075;	.endef
	.def	_win0_end;	.val	4;	.scl	8;	.dim	2;	.size	4;	.type	075;	.endef
	.def	_win1_start;	.val	8;	.scl	8;	.dim	2;	.size	4;	.type	075;	.endef
	.def	_win1_end;	.val	12;	.scl	8;	.dim	2;	.size	4;	.type	075;	.endef
	.def	_wincontrl;	.val	16;	.scl	8;	.dim	4;	.size	8;	.type	075;	.endef
	.def	_linewin0_addr;	.val	24;	.scl	8;	.type	017;	.endef
	.def	_linewin1_addr;	.val	28;	.scl	8;	.type	017;	.endef
	.def	.eos;	.val	32;	.scl	102;	.tag	_SclWinscl;	.size	32;	.endef
	.def	_SclWinscl;	.scl	13;	.tag	_SclWinscl;	.size	32;	.type	010;	.endef
	.def	_SclPrior;	.scl	10;	.type	010;	.size	2;	.endef
	.def	_prnum;	.val	0;	.scl	8;	.type	015;	.endef
	.def	.eos;	.val	2;	.scl	102;	.tag	_SclPrior;	.size	2;	.endef
	.def	_SclPrior;	.scl	13;	.tag	_SclPrior;	.size	2;	.type	010;	.endef
	.def	_SclXy;	.scl	10;	.type	010;	.size	8;	.endef
	.def	_x;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_y;	.val	4;	.scl	8;	.type	05;	.endef
	.def	.eos;	.val	8;	.scl	102;	.tag	_SclXy;	.size	8;	.endef
	.def	_SclXy;	.scl	13;	.tag	_SclXy;	.size	8;	.type	010;	.endef
	.def	_SclXyz;	.scl	10;	.type	010;	.size	12;	.endef
	.def	_x;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_y;	.val	4;	.scl	8;	.type	05;	.endef
	.def	_z;	.val	8;	.scl	8;	.type	05;	.endef
	.def	.eos;	.val	12;	.scl	102;	.tag	_SclXyz;	.size	12;	.endef
	.def	_SclXyz;	.scl	13;	.tag	_SclXyz;	.size	12;	.type	010;	.endef
	.def	_SclXy16;	.scl	10;	.type	010;	.size	4;	.endef
	.def	_x;	.val	0;	.scl	8;	.type	015;	.endef
	.def	_y;	.val	2;	.scl	8;	.type	015;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_SclXy16;	.size	4;	.endef
	.def	_SclXy16;	.scl	13;	.tag	_SclXy16;	.size	4;	.type	010;	.endef
	.def	_SclXyz16;	.scl	10;	.type	010;	.size	6;	.endef
	.def	_x;	.val	0;	.scl	8;	.type	03;	.endef
	.def	_y;	.val	2;	.scl	8;	.type	03;	.endef
	.def	_z;	.val	4;	.scl	8;	.type	03;	.endef
	.def	.eos;	.val	6;	.scl	102;	.tag	_SclXyz16;	.size	6;	.endef
	.def	_SclXyz16;	.scl	13;	.tag	_SclXyz16;	.size	6;	.type	010;	.endef
	.def	_SclRgb;	.scl	10;	.type	010;	.size	6;	.endef
	.def	_red;	.val	0;	.scl	8;	.type	03;	.endef
	.def	_green;	.val	2;	.scl	8;	.type	03;	.endef
	.def	_blue;	.val	4;	.scl	8;	.type	03;	.endef
	.def	.eos;	.val	6;	.scl	102;	.tag	_SclRgb;	.size	6;	.endef
	.def	_SclRgb;	.scl	13;	.tag	_SclRgb;	.size	6;	.type	010;	.endef
	.def	_SclLineTb;	.scl	10;	.type	010;	.size	12;	.endef
	.def	_h;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_v;	.val	4;	.scl	8;	.type	05;	.endef
	.def	_dh;	.val	8;	.scl	8;	.type	05;	.endef
	.def	.eos;	.val	12;	.scl	102;	.tag	_SclLineTb;	.size	12;	.endef
	.def	_SclLineTb;	.scl	13;	.tag	_SclLineTb;	.size	12;	.type	010;	.endef
	.def	_SclLineWindowTb;	.scl	10;	.type	010;	.size	4;	.endef
	.def	_start;	.val	0;	.scl	8;	.type	015;	.endef
	.def	_end;	.val	2;	.scl	8;	.type	015;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_SclLineWindowTb;	.size	4;	.endef
	.def	_SclLineWindowTb;	.scl	13;	.tag	_SclLineWindowTb;	.size	4;	.type	010;	.endef
	.def	_SclVramConfig;	.scl	10;	.type	010;	.size	16;	.endef
	.def	_ktboffsetA;	.val	0;	.scl	8;	.type	017;	.endef
	.def	_ktboffsetB;	.val	4;	.scl	8;	.type	017;	.endef
	.def	_vramModeA;	.val	8;	.scl	8;	.type	014;	.endef
	.def	_vramModeB;	.val	9;	.scl	8;	.type	014;	.endef
	.def	_vramA0;	.val	10;	.scl	8;	.type	014;	.endef
	.def	_vramA1;	.val	11;	.scl	8;	.type	014;	.endef
	.def	_vramB0;	.val	12;	.scl	8;	.type	014;	.endef
	.def	_vramB1;	.val	13;	.scl	8;	.type	014;	.endef
	.def	_colram;	.val	14;	.scl	8;	.type	014;	.endef
	.def	.eos;	.val	16;	.scl	102;	.tag	_SclVramConfig;	.size	16;	.endef
	.def	_SclVramConfig;	.scl	13;	.tag	_SclVramConfig;	.size	16;	.type	010;	.endef
	.def	_SclRotreg;	.scl	10;	.type	010;	.size	128;	.endef
	.def	_screenst;	.val	0;	.scl	8;	.tag	_SclXyz;	.size	12;	.type	010;	.endef
	.def	_screendlt;	.val	12;	.scl	8;	.tag	_SclXy;	.size	8;	.type	010;	.endef
	.def	_delta;	.val	20;	.scl	8;	.tag	_SclXy;	.size	8;	.type	010;	.endef
	.def	_matrix_a;	.val	28;	.scl	8;	.type	05;	.endef
	.def	_matrix_b;	.val	32;	.scl	8;	.type	05;	.endef
	.def	_matrix_c;	.val	36;	.scl	8;	.type	05;	.endef
	.def	_matrix_d;	.val	40;	.scl	8;	.type	05;	.endef
	.def	_matrix_e;	.val	44;	.scl	8;	.type	05;	.endef
	.def	_matrix_f;	.val	48;	.scl	8;	.type	05;	.endef
	.def	_viewp;	.val	52;	.scl	8;	.tag	_SclXyz16;	.size	6;	.type	010;	.endef
	.def	_dummy1;	.val	58;	.scl	8;	.type	015;	.endef
	.def	_rotatecenter;	.val	60;	.scl	8;	.tag	_SclXyz16;	.size	6;	.type	010;	.endef
	.def	_dummy2;	.val	66;	.scl	8;	.type	015;	.endef
	.def	_move;	.val	68;	.scl	8;	.tag	_SclXy;	.size	8;	.type	010;	.endef
	.def	_zoom;	.val	76;	.scl	8;	.tag	_SclXy;	.size	8;	.type	010;	.endef
	.def	_k_tab;	.val	84;	.scl	8;	.type	05;	.endef
	.def	_k_delta;	.val	88;	.scl	8;	.tag	_SclXy;	.size	8;	.type	010;	.endef
	.def	_dummy3;	.val	96;	.scl	8;	.dim	8;	.size	32;	.type	065;	.endef
	.def	.eos;	.val	128;	.scl	102;	.tag	_SclRotreg;	.size	128;	.endef
	.def	_SclRotreg;	.scl	13;	.tag	_SclRotreg;	.size	128;	.type	010;	.endef
	.def	_SclRotparam;	.scl	10;	.type	010;	.size	144;	.endef
	.def	_addr;	.val	0;	.scl	8;	.type	017;	.endef
	.def	_screenst;	.val	4;	.scl	8;	.tag	_SclXyz;	.size	12;	.type	010;	.endef
	.def	_screendlt;	.val	16;	.scl	8;	.tag	_SclXy;	.size	8;	.type	010;	.endef
	.def	_delta;	.val	24;	.scl	8;	.tag	_SclXy;	.size	8;	.type	010;	.endef
	.def	_matrix_a;	.val	32;	.scl	8;	.type	05;	.endef
	.def	_matrix_b;	.val	36;	.scl	8;	.type	05;	.endef
	.def	_matrix_c;	.val	40;	.scl	8;	.type	05;	.endef
	.def	_matrix_d;	.val	44;	.scl	8;	.type	05;	.endef
	.def	_matrix_e;	.val	48;	.scl	8;	.type	05;	.endef
	.def	_matrix_f;	.val	52;	.scl	8;	.type	05;	.endef
	.def	_matrix_g;	.val	56;	.scl	8;	.type	05;	.endef
	.def	_matrix_h;	.val	60;	.scl	8;	.type	05;	.endef
	.def	_matrix_i;	.val	64;	.scl	8;	.type	05;	.endef
	.def	_drotangle;	.val	68;	.scl	8;	.type	05;	.endef
	.def	_viewp;	.val	72;	.scl	8;	.tag	_SclXyz;	.size	12;	.type	010;	.endef
	.def	_rotatecenter;	.val	84;	.scl	8;	.tag	_SclXyz;	.size	12;	.type	010;	.endef
	.def	_disprotcenter;	.val	96;	.scl	8;	.tag	_SclXyz;	.size	12;	.type	010;	.endef
	.def	_move;	.val	108;	.scl	8;	.tag	_SclXyz;	.size	12;	.type	010;	.endef
	.def	_zoom;	.val	120;	.scl	8;	.tag	_SclXy;	.size	8;	.type	010;	.endef
	.def	_k_tab;	.val	128;	.scl	8;	.type	05;	.endef
	.def	_k_delta;	.val	132;	.scl	8;	.tag	_SclXy;	.size	8;	.type	010;	.endef
	.def	_k_size;	.val	140;	.scl	8;	.type	015;	.endef
	.def	.eos;	.val	144;	.scl	102;	.tag	_SclRotparam;	.size	144;	.endef
	.def	_SclRotparam;	.scl	13;	.tag	_SclRotparam;	.size	144;	.type	010;	.endef
	.def	_SclConfig;	.scl	10;	.type	010;	.size	76;	.endef
	.def	_dispenbl;	.val	0;	.scl	8;	.type	014;	.endef
	.def	_charsize;	.val	1;	.scl	8;	.type	014;	.endef
	.def	_pnamesize;	.val	2;	.scl	8;	.type	014;	.endef
	.def	_platesize;	.val	3;	.scl	8;	.type	014;	.endef
	.def	_bmpsize;	.val	4;	.scl	8;	.type	014;	.endef
	.def	_coltype;	.val	5;	.scl	8;	.type	014;	.endef
	.def	_datatype;	.val	6;	.scl	8;	.type	014;	.endef
	.def	_mapover;	.val	7;	.scl	8;	.type	014;	.endef
	.def	_flip;	.val	8;	.scl	8;	.type	014;	.endef
	.def	_patnamecontrl;	.val	10;	.scl	8;	.type	015;	.endef
	.def	_plate_addr;	.val	12;	.scl	8;	.dim	16;	.size	64;	.type	077;	.endef
	.def	.eos;	.val	76;	.scl	102;	.tag	_SclConfig;	.size	76;	.endef
	.def	_SclConfig;	.scl	13;	.tag	_SclConfig;	.size	76;	.type	010;	.endef
	.def	_SclLineparam;	.scl	10;	.type	010;	.size	6416;	.endef
	.def	_delta_enbl;	.val	0;	.scl	8;	.type	014;	.endef
	.def	_v_enbl;	.val	1;	.scl	8;	.type	014;	.endef
	.def	_h_enbl;	.val	2;	.scl	8;	.type	014;	.endef
	.def	_cell_enbl;	.val	3;	.scl	8;	.type	014;	.endef
	.def	_interval;	.val	4;	.scl	8;	.type	014;	.endef
	.def	_line_addr;	.val	8;	.scl	8;	.type	017;	.endef
	.def	_cell_addr;	.val	12;	.scl	8;	.type	017;	.endef
	.def	_line_tbl;	.val	16;	.scl	8;	.tag	_SclLineTb;	.dim	512;	.size	6144;	.type	070;	.endef
	.def	_cell_tbl;	.val	6160;	.scl	8;	.dim	64;	.size	256;	.type	065;	.endef
	.def	.eos;	.val	6416;	.scl	102;	.tag	_SclLineparam;	.size	6416;	.endef
	.def	_SclLineparam;	.scl	13;	.tag	_SclLineparam;	.size	6416;	.type	010;	.endef
	.def	_SclLineWin;	.scl	10;	.type	010;	.size	2060;	.endef
	.def	_winum;	.val	0;	.scl	8;	.type	015;	.endef
	.def	_y;	.val	2;	.scl	8;	.dim	2;	.size	4;	.type	075;	.endef
	.def	_addr;	.val	8;	.scl	8;	.type	017;	.endef
	.def	_tbl;	.val	12;	.scl	8;	.tag	_SclXy16;	.dim	512;	.size	2048;	.type	070;	.endef
	.def	.eos;	.val	2060;	.scl	102;	.tag	_SclLineWin;	.size	2060;	.endef
	.def	_SclLineWin;	.scl	13;	.tag	_SclLineWin;	.size	2060;	.type	010;	.endef
	.def	_.1fake;	.scl	10;	.type	010;	.size	16;	.endef
	.def	_SpriteControl;	.val	0;	.scl	8;	.type	015;	.endef
	.def	_ShadowControl;	.val	2;	.scl	8;	.type	015;	.endef
	.def	_ColorRamOffset0;	.val	4;	.scl	8;	.type	015;	.endef
	.def	_ColorRamOffset1;	.val	6;	.scl	8;	.type	015;	.endef
	.def	_LineColorEnable;	.val	8;	.scl	8;	.type	015;	.endef
	.def	_SpecialPriorityMode;	.val	10;	.scl	8;	.type	015;	.endef
	.def	_ColorMixControl;	.val	12;	.scl	8;	.type	015;	.endef
	.def	_SpecialColorMixMode;	.val	14;	.scl	8;	.type	015;	.endef
	.def	.eos;	.val	16;	.scl	102;	.tag	_.1fake;	.size	16;	.endef
	.def	_SclOtherPriRegister;	.scl	13;	.tag	_.1fake;	.size	16;	.type	010;	.endef
	.def	_.2fake;	.scl	10;	.type	010;	.size	8;	.endef
	.def	_PriorityNumberSP01;	.val	0;	.scl	8;	.type	015;	.endef
	.def	_PriorityNumberSP23;	.val	2;	.scl	8;	.type	015;	.endef
	.def	_PriorityNumberSP45;	.val	4;	.scl	8;	.type	015;	.endef
	.def	_PriorityNumberSP67;	.val	6;	.scl	8;	.type	015;	.endef
	.def	.eos;	.val	8;	.scl	102;	.tag	_.2fake;	.size	8;	.endef
	.def	_SclSpPriNumRegister;	.scl	13;	.tag	_.2fake;	.size	8;	.type	010;	.endef
	.def	_.3fake;	.scl	10;	.type	010;	.size	6;	.endef
	.def	_PriorityNumberNBG01;	.val	0;	.scl	8;	.type	015;	.endef
	.def	_PriorityNumberNBG23;	.val	2;	.scl	8;	.type	015;	.endef
	.def	_PriorityNumberRBG0;	.val	4;	.scl	8;	.type	015;	.endef
	.def	.eos;	.val	6;	.scl	102;	.tag	_.3fake;	.size	6;	.endef
	.def	_SclBgPriNumRegister;	.scl	13;	.tag	_.3fake;	.size	6;	.type	010;	.endef
	.def	_.4fake;	.scl	10;	.type	010;	.size	8;	.endef
	.def	_ColMixRateSP01;	.val	0;	.scl	8;	.type	015;	.endef
	.def	_ColMixRateSP23;	.val	2;	.scl	8;	.type	015;	.endef
	.def	_ColMixRateSP45;	.val	4;	.scl	8;	.type	015;	.endef
	.def	_ColMixRateSP67;	.val	6;	.scl	8;	.type	015;	.endef
	.def	.eos;	.val	8;	.scl	102;	.tag	_.4fake;	.size	8;	.endef
	.def	_SclSpColMixRegister;	.scl	13;	.tag	_.4fake;	.size	8;	.type	010;	.endef
	.def	_.5fake;	.scl	10;	.type	010;	.size	8;	.endef
	.def	_ColMixRateNBG01;	.val	0;	.scl	8;	.type	015;	.endef
	.def	_ColMixRateNBG23;	.val	2;	.scl	8;	.type	015;	.endef
	.def	_ColMixRateRBG0;	.val	4;	.scl	8;	.type	015;	.endef
	.def	_ColMixRateLCBAK;	.val	6;	.scl	8;	.type	015;	.endef
	.def	.eos;	.val	8;	.scl	102;	.tag	_.5fake;	.size	8;	.endef
	.def	_SclBgColMixRegister;	.scl	13;	.tag	_.5fake;	.size	8;	.type	010;	.endef
	.def	_.6fake;	.scl	10;	.type	010;	.size	16;	.endef
	.def	_ColorOffsetEnable;	.val	0;	.scl	8;	.type	015;	.endef
	.def	_ColorOffsetSelect;	.val	2;	.scl	8;	.type	015;	.endef
	.def	_ColorOffsetA_RED;	.val	4;	.scl	8;	.type	015;	.endef
	.def	_ColorOffsetA_GREEN;	.val	6;	.scl	8;	.type	015;	.endef
	.def	_ColorOffsetA_BLUE;	.val	8;	.scl	8;	.type	015;	.endef
	.def	_ColorOffsetB_RED;	.val	10;	.scl	8;	.type	015;	.endef
	.def	_ColorOffsetB_GREEN;	.val	12;	.scl	8;	.type	015;	.endef
	.def	_ColorOffsetB_BLUE;	.val	14;	.scl	8;	.type	015;	.endef
	.def	.eos;	.val	16;	.scl	102;	.tag	_.6fake;	.size	16;	.endef
	.def	_SclColOffsetRegister;	.scl	13;	.tag	_.6fake;	.size	16;	.type	010;	.endef
	.def	_.7fake;	.scl	10;	.type	010;	.size	1;	.endef
	.def	_SclOtherPri;	.val	0;	.scl	18;	.type	017;	.size	1;	.endef
	.def	_SclSpPriNum;	.val	1;	.scl	18;	.type	017;	.size	1;	.endef
	.def	_SclBgPriNum;	.val	2;	.scl	18;	.type	017;	.size	1;	.endef
	.def	_SclSpColMix;	.val	3;	.scl	18;	.type	017;	.size	1;	.endef
	.def	_SclBgColMix;	.val	4;	.scl	18;	.type	017;	.size	1;	.endef
	.def	_SclColOffset;	.val	5;	.scl	18;	.type	017;	.size	1;	.endef
	.def	.eos;	.val	1;	.scl	102;	.tag	_.7fake;	.size	1;	.endef
	.def	_SclPriBuffDirtyFlags;	.scl	13;	.tag	_.7fake;	.size	1;	.type	010;	.endef
	.def	_.8fake;	.scl	10;	.type	010;	.size	12;	.endef
	.def	_pr;	.val	0;	.scl	8;	.type	017;	.endef
	.def	_dme;	.val	4;	.scl	8;	.type	017;	.endef
	.def	_msk;	.val	8;	.scl	8;	.type	017;	.endef
	.def	.eos;	.val	12;	.scl	102;	.tag	_.8fake;	.size	12;	.endef
	.def	_DmaCpuComPrm;	.scl	13;	.tag	_.8fake;	.size	12;	.type	010;	.endef
	.def	_.9fake;	.scl	10;	.type	010;	.size	40;	.endef
	.def	_sar;	.val	0;	.scl	8;	.type	017;	.endef
	.def	_dar;	.val	4;	.scl	8;	.type	017;	.endef
	.def	_tcr;	.val	8;	.scl	8;	.type	017;	.endef
	.def	_dm;	.val	12;	.scl	8;	.type	017;	.endef
	.def	_sm;	.val	16;	.scl	8;	.type	017;	.endef
	.def	_ts;	.val	20;	.scl	8;	.type	017;	.endef
	.def	_ar;	.val	24;	.scl	8;	.type	017;	.endef
	.def	_ie;	.val	28;	.scl	8;	.type	017;	.endef
	.def	_drcr;	.val	32;	.scl	8;	.type	017;	.endef
	.def	_msk;	.val	36;	.scl	8;	.type	017;	.endef
	.def	.eos;	.val	40;	.scl	102;	.tag	_.9fake;	.size	40;	.endef
	.def	_DmaCpuPrm;	.scl	13;	.tag	_.9fake;	.size	40;	.type	010;	.endef
	.def	_.10fake;	.scl	10;	.type	010;	.size	8;	.endef
	.def	_ae;	.val	0;	.scl	8;	.type	017;	.endef
	.def	_nmif;	.val	4;	.scl	8;	.type	017;	.endef
	.def	.eos;	.val	8;	.scl	102;	.tag	_.10fake;	.size	8;	.endef
	.def	_DmaCpuComStatus;	.scl	13;	.tag	_.10fake;	.size	8;	.type	010;	.endef
	.def	_DmaCpuStatus;	.scl	13;	.type	017;	.endef
	.def	_.11fake;	.scl	10;	.type	010;	.size	40;	.endef
	.def	_dxr;	.val	0;	.scl	8;	.type	017;	.endef
	.def	_dxw;	.val	4;	.scl	8;	.type	017;	.endef
	.def	_dxc;	.val	8;	.scl	8;	.type	017;	.endef
	.def	_dxad_r;	.val	12;	.scl	8;	.type	017;	.endef
	.def	_dxad_w;	.val	16;	.scl	8;	.type	017;	.endef
	.def	_dxmod;	.val	20;	.scl	8;	.type	017;	.endef
	.def	_dxrup;	.val	24;	.scl	8;	.type	017;	.endef
	.def	_dxwup;	.val	28;	.scl	8;	.type	017;	.endef
	.def	_dxft;	.val	32;	.scl	8;	.type	017;	.endef
	.def	_msk;	.val	36;	.scl	8;	.type	017;	.endef
	.def	.eos;	.val	40;	.scl	102;	.tag	_.11fake;	.size	40;	.endef
	.def	_DmaScuPrm;	.scl	13;	.tag	_.11fake;	.size	40;	.type	010;	.endef
	.def	_.12fake;	.scl	10;	.type	010;	.size	4;	.endef
	.def	_dxmv;	.val	0;	.scl	8;	.type	017;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_.12fake;	.size	4;	.endef
	.def	_DmaScuStatus;	.scl	13;	.tag	_.12fake;	.size	4;	.type	010;	.endef
	.def	_CdcStatus;	.scl	15;	.type	012;	.size	4;	.endef
	.def	_CDC_ST_BUSY;	.val	0;	.scl	16;	.type	013;	.endef
	.def	_CDC_ST_PAUSE;	.val	1;	.scl	16;	.type	013;	.endef
	.def	_CDC_ST_STANDBY;	.val	2;	.scl	16;	.type	013;	.endef
	.def	_CDC_ST_PLAY;	.val	3;	.scl	16;	.type	013;	.endef
	.def	_CDC_ST_SEEK;	.val	4;	.scl	16;	.type	013;	.endef
	.def	_CDC_ST_SCAN;	.val	5;	.scl	16;	.type	013;	.endef
	.def	_CDC_ST_OPEN;	.val	6;	.scl	16;	.type	013;	.endef
	.def	_CDC_ST_NODISC;	.val	7;	.scl	16;	.type	013;	.endef
	.def	_CDC_ST_RETRY;	.val	8;	.scl	16;	.type	013;	.endef
	.def	_CDC_ST_ERROR;	.val	9;	.scl	16;	.type	013;	.endef
	.def	_CDC_ST_FATAL;	.val	10;	.scl	16;	.type	013;	.endef
	.def	_CDC_ST_PERI;	.val	32;	.scl	16;	.type	013;	.endef
	.def	_CDC_ST_TRNS;	.val	64;	.scl	16;	.type	013;	.endef
	.def	_CDC_ST_WAIT;	.val	128;	.scl	16;	.type	013;	.endef
	.def	_CDC_ST_REJECT;	.val	255;	.scl	16;	.type	013;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_CdcStatus;	.size	4;	.endef
	.def	_CdcErrCode;	.scl	15;	.type	012;	.size	4;	.endef
	.def	_CDC_ERR_OK;	.val	0;	.scl	16;	.type	013;	.endef
	.def	_CDC_ERR_CMDBUSY;	.val	-1;	.scl	16;	.type	013;	.endef
	.def	_CDC_ERR_CMDNG;	.val	-2;	.scl	16;	.type	013;	.endef
	.def	_CDC_ERR_TMOUT;	.val	-3;	.scl	16;	.type	013;	.endef
	.def	_CDC_ERR_PUT;	.val	-4;	.scl	16;	.type	013;	.endef
	.def	_CDC_ERR_REJECT;	.val	-5;	.scl	16;	.type	013;	.endef
	.def	_CDC_ERR_WAIT;	.val	-6;	.scl	16;	.type	013;	.endef
	.def	_CDC_ERR_TRNS;	.val	-7;	.scl	16;	.type	013;	.endef
	.def	_CDC_ERR_PERI;	.val	-8;	.scl	16;	.type	013;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_CdcErrCode;	.size	4;	.endef
	.def	_CdcPosType;	.scl	15;	.type	012;	.size	4;	.endef
	.def	_CDC_PTYPE_DFL;	.val	0;	.scl	16;	.type	013;	.endef
	.def	_CDC_PTYPE_FAD;	.val	1;	.scl	16;	.type	013;	.endef
	.def	_CDC_PTYPE_TNO;	.val	2;	.scl	16;	.type	013;	.endef
	.def	_CDC_PTYPE_NOCHG;	.val	3;	.scl	16;	.type	013;	.endef
	.def	_CDC_PTYPE_END;	.val	4;	.scl	16;	.type	013;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_CdcPosType;	.size	4;	.endef
	.def	_CdcSubMode;	.scl	15;	.type	012;	.size	4;	.endef
	.def	_CDC_SM_EOR;	.val	1;	.scl	16;	.type	013;	.endef
	.def	_CDC_SM_VIDEO;	.val	2;	.scl	16;	.type	013;	.endef
	.def	_CDC_SM_AUDIO;	.val	4;	.scl	16;	.type	013;	.endef
	.def	_CDC_SM_DATA;	.val	8;	.scl	16;	.type	013;	.endef
	.def	_CDC_SM_TRIG;	.val	16;	.scl	16;	.type	013;	.endef
	.def	_CDC_SM_FORM;	.val	32;	.scl	16;	.type	013;	.endef
	.def	_CDC_SM_RT;	.val	64;	.scl	16;	.type	013;	.endef
	.def	_CDC_SM_EOF;	.val	128;	.scl	16;	.type	013;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_CdcSubMode;	.size	4;	.endef
	.def	_CdcDrdyType;	.scl	15;	.type	012;	.size	4;	.endef
	.def	_CDC_DRDY_GET;	.val	0;	.scl	16;	.type	013;	.endef
	.def	_CDC_DRDY_PUT;	.val	1;	.scl	16;	.type	013;	.endef
	.def	_CDC_DRDY_END;	.val	2;	.scl	16;	.type	013;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_CdcDrdyType;	.size	4;	.endef
	.def	_CdcScanDir;	.scl	15;	.type	012;	.size	4;	.endef
	.def	_CDC_SCAN_FWD;	.val	0;	.scl	16;	.type	013;	.endef
	.def	_CDC_SCAN_RVS;	.val	1;	.scl	16;	.type	013;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_CdcScanDir;	.size	4;	.endef
	.def	_CdcSctLen;	.scl	15;	.type	012;	.size	4;	.endef
	.def	_CDC_SLEN_2048;	.val	0;	.scl	16;	.type	013;	.endef
	.def	_CDC_SLEN_2336;	.val	1;	.scl	16;	.type	013;	.endef
	.def	_CDC_SLEN_2340;	.val	2;	.scl	16;	.type	013;	.endef
	.def	_CDC_SLEN_2352;	.val	3;	.scl	16;	.type	013;	.endef
	.def	_CDC_SLEN_NOCHG;	.val	255;	.scl	16;	.type	013;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_CdcSctLen;	.size	4;	.endef
	.def	_CdcCopyErr;	.scl	15;	.type	012;	.size	4;	.endef
	.def	_CDC_COPY_OK;	.val	0;	.scl	16;	.type	013;	.endef
	.def	_CDC_COPY_NG;	.val	1;	.scl	16;	.type	013;	.endef
	.def	_CDC_COPY_BUSY;	.val	255;	.scl	16;	.type	013;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_CdcCopyErr;	.size	4;	.endef
	.def	_.13fake;	.scl	10;	.type	010;	.size	8;	.endef
	.def	_flgrep;	.val	0;	.scl	8;	.type	014;	.endef
	.def	_ctladr;	.val	1;	.scl	8;	.type	014;	.endef
	.def	_tno;	.val	2;	.scl	8;	.type	014;	.endef
	.def	_idx;	.val	3;	.scl	8;	.type	014;	.endef
	.def	_fad;	.val	4;	.scl	8;	.type	05;	.endef
	.def	.eos;	.val	8;	.scl	102;	.tag	_.13fake;	.size	8;	.endef
	.def	_.14fake;	.scl	10;	.type	010;	.size	12;	.endef
	.def	_status;	.val	0;	.scl	8;	.type	014;	.endef
	.def	_report;	.val	4;	.scl	8;	.tag	_.13fake;	.size	8;	.type	010;	.endef
	.def	.eos;	.val	12;	.scl	102;	.tag	_.14fake;	.size	12;	.endef
	.def	_CdcStat;	.scl	13;	.tag	_.14fake;	.size	12;	.type	010;	.endef
	.def	_.15fake;	.scl	10;	.type	010;	.size	5;	.endef
	.def	_hflag;	.val	0;	.scl	8;	.type	014;	.endef
	.def	_ver;	.val	1;	.scl	8;	.type	014;	.endef
	.def	_mpver;	.val	2;	.scl	8;	.type	014;	.endef
	.def	_drv;	.val	3;	.scl	8;	.type	014;	.endef
	.def	_rev;	.val	4;	.scl	8;	.type	014;	.endef
	.def	.eos;	.val	5;	.scl	102;	.tag	_.15fake;	.size	5;	.endef
	.def	_CdcHw;	.scl	13;	.tag	_.15fake;	.size	5;	.type	010;	.endef
	.def	_.16fake;	.scl	10;	.type	010;	.size	2;	.endef
	.def	_tno;	.val	0;	.scl	8;	.type	014;	.endef
	.def	_idx;	.val	1;	.scl	8;	.type	014;	.endef
	.def	.eos;	.val	2;	.scl	102;	.tag	_.16fake;	.size	2;	.endef
	.def	_.17fake;	.scl	12;	.type	011;	.size	4;	.endef
	.def	_fad;	.val	0;	.scl	11;	.type	05;	.endef
	.def	_trkidx;	.val	0;	.scl	11;	.tag	_.16fake;	.size	2;	.type	010;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_.17fake;	.size	4;	.endef
	.def	_.18fake;	.scl	10;	.type	010;	.size	8;	.endef
	.def	_ptype;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_pbody;	.val	4;	.scl	8;	.tag	_.17fake;	.size	4;	.type	011;	.endef
	.def	.eos;	.val	8;	.scl	102;	.tag	_.18fake;	.size	8;	.endef
	.def	_CdcPos;	.scl	13;	.tag	_.18fake;	.size	8;	.type	010;	.endef
	.def	_.19fake;	.scl	10;	.type	010;	.size	20;	.endef
	.def	_start;	.val	0;	.scl	8;	.tag	_.18fake;	.size	8;	.type	010;	.endef
	.def	_end;	.val	8;	.scl	8;	.tag	_.18fake;	.size	8;	.type	010;	.endef
	.def	_pmode;	.val	16;	.scl	8;	.type	014;	.endef
	.def	.eos;	.val	20;	.scl	102;	.tag	_.19fake;	.size	20;	.endef
	.def	_CdcPly;	.scl	13;	.tag	_.19fake;	.size	20;	.type	010;	.endef
	.def	_.20fake;	.scl	10;	.type	010;	.size	6;	.endef
	.def	_fn;	.val	0;	.scl	8;	.type	014;	.endef
	.def	_cn;	.val	1;	.scl	8;	.type	014;	.endef
	.def	_smmsk;	.val	2;	.scl	8;	.type	014;	.endef
	.def	_smval;	.val	3;	.scl	8;	.type	014;	.endef
	.def	_cimsk;	.val	4;	.scl	8;	.type	014;	.endef
	.def	_cival;	.val	5;	.scl	8;	.type	014;	.endef
	.def	.eos;	.val	6;	.scl	102;	.tag	_.20fake;	.size	6;	.endef
	.def	_CdcSubh;	.scl	13;	.tag	_.20fake;	.size	6;	.type	010;	.endef
	.def	_.21fake;	.scl	10;	.type	010;	.size	8;	.endef
	.def	_fad;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_fn;	.val	4;	.scl	8;	.type	014;	.endef
	.def	_cn;	.val	5;	.scl	8;	.type	014;	.endef
	.def	_sm;	.val	6;	.scl	8;	.type	014;	.endef
	.def	_ci;	.val	7;	.scl	8;	.type	014;	.endef
	.def	.eos;	.val	8;	.scl	102;	.tag	_.21fake;	.size	8;	.endef
	.def	_CdcSct;	.scl	13;	.tag	_.21fake;	.size	8;	.type	010;	.endef
	.def	_.22fake;	.scl	10;	.type	010;	.size	12;	.endef
	.def	_fad;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_size;	.val	4;	.scl	8;	.type	05;	.endef
	.def	_unit;	.val	8;	.scl	8;	.type	014;	.endef
	.def	_gap;	.val	9;	.scl	8;	.type	014;	.endef
	.def	_fn;	.val	10;	.scl	8;	.type	014;	.endef
	.def	_atr;	.val	11;	.scl	8;	.type	014;	.endef
	.def	.eos;	.val	12;	.scl	102;	.tag	_.22fake;	.size	12;	.endef
	.def	_CdcFile;	.scl	13;	.tag	_.22fake;	.size	12;	.type	010;	.endef
	.def	_CdcMpErrCode;	.scl	15;	.type	012;	.size	4;	.endef
	.def	_CDC_ERR_MP_COMU;	.val	-20;	.scl	16;	.type	013;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_CdcMpErrCode;	.size	4;	.endef
	.def	_.23fake;	.scl	10;	.type	010;	.size	8;	.endef
	.def	_actst;	.val	0;	.scl	8;	.type	014;	.endef
	.def	_pict;	.val	1;	.scl	8;	.type	014;	.endef
	.def	_stat_a;	.val	2;	.scl	8;	.type	014;	.endef
	.def	_stat_v;	.val	4;	.scl	8;	.type	015;	.endef
	.def	_vcnt;	.val	6;	.scl	8;	.type	015;	.endef
	.def	.eos;	.val	8;	.scl	102;	.tag	_.23fake;	.size	8;	.endef
	.def	_.24fake;	.scl	10;	.type	010;	.size	10;	.endef
	.def	_status;	.val	0;	.scl	8;	.type	014;	.endef
	.def	_report;	.val	2;	.scl	8;	.tag	_.23fake;	.size	8;	.type	010;	.endef
	.def	.eos;	.val	10;	.scl	102;	.tag	_.24fake;	.size	10;	.endef
	.def	_CdcMpStat;	.scl	13;	.tag	_.24fake;	.size	10;	.type	010;	.endef
	.def	_.25fake;	.scl	10;	.type	010;	.size	4;	.endef
	.def	_hour;	.val	0;	.scl	8;	.type	014;	.endef
	.def	_min;	.val	1;	.scl	8;	.type	014;	.endef
	.def	_sec;	.val	2;	.scl	8;	.type	014;	.endef
	.def	_pic;	.val	3;	.scl	8;	.type	014;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_.25fake;	.size	4;	.endef
	.def	_CdcMpTc;	.scl	13;	.tag	_.25fake;	.size	4;	.type	010;	.endef
	.def	_.26fake;	.scl	10;	.type	010;	.size	3;	.endef
	.def	_cmod;	.val	0;	.scl	8;	.type	014;	.endef
	.def	_lay;	.val	1;	.scl	8;	.type	014;	.endef
	.def	_bn;	.val	2;	.scl	8;	.type	014;	.endef
	.def	.eos;	.val	3;	.scl	102;	.tag	_.26fake;	.size	3;	.endef
	.def	_CdcMpCon;	.scl	13;	.tag	_.26fake;	.size	3;	.type	010;	.endef
	.def	_.27fake;	.scl	10;	.type	010;	.size	3;	.endef
	.def	_smod;	.val	0;	.scl	8;	.type	014;	.endef
	.def	_id;	.val	1;	.scl	8;	.type	014;	.endef
	.def	_cn;	.val	2;	.scl	8;	.type	014;	.endef
	.def	.eos;	.val	3;	.scl	102;	.tag	_.27fake;	.size	3;	.endef
	.def	_CdcMpStm;	.scl	13;	.tag	_.27fake;	.size	3;	.type	010;	.endef
	.def	_.28fake;	.scl	10;	.type	010;	.size	6;	.endef
	.def	_intpol;	.val	0;	.scl	8;	.type	014;	.endef
	.def	_trnsp;	.val	1;	.scl	8;	.type	014;	.endef
	.def	_moz_h;	.val	2;	.scl	8;	.type	014;	.endef
	.def	_moz_v;	.val	3;	.scl	8;	.type	014;	.endef
	.def	_soft_h;	.val	4;	.scl	8;	.type	014;	.endef
	.def	_soft_v;	.val	5;	.scl	8;	.type	014;	.endef
	.def	.eos;	.val	6;	.scl	102;	.tag	_.28fake;	.size	6;	.endef
	.def	_CdcMpVeff;	.scl	13;	.tag	_.28fake;	.size	6;	.type	010;	.endef
	.def	_GfsAccessMode;	.scl	15;	.type	012;	.size	4;	.endef
	.def	_GFS_NWSTAT_NOACT;	.val	0;	.scl	16;	.type	013;	.endef
	.def	_GFS_NWSTAT_FREAD;	.val	1;	.scl	16;	.type	013;	.endef
	.def	_GFS_NWSTAT_CDREAD;	.val	2;	.scl	16;	.type	013;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_GfsAccessMode;	.size	4;	.endef
	.def	_GfsSeekMode;	.scl	15;	.type	012;	.size	4;	.endef
	.def	_GFS_SEEK_SET;	.val	0;	.scl	16;	.type	013;	.endef
	.def	_GFS_SEEK_CUR;	.val	1;	.scl	16;	.type	013;	.endef
	.def	_GFS_SEEK_END;	.val	2;	.scl	16;	.type	013;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_GfsSeekMode;	.size	4;	.endef
	.def	_GfsGetMode;	.scl	15;	.type	012;	.size	4;	.endef
	.def	_GFS_GMODE_ERASE;	.val	0;	.scl	16;	.type	013;	.endef
	.def	_GFS_GMODE_RESIDENT;	.val	1;	.scl	16;	.type	013;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_GfsGetMode;	.size	4;	.endef
	.def	_GfsTransMode;	.scl	15;	.type	012;	.size	4;	.endef
	.def	_GFS_TMODE_SCU;	.val	0;	.scl	16;	.type	013;	.endef
	.def	_GFS_TMODE_SDMA0;	.val	1;	.scl	16;	.type	013;	.endef
	.def	_GFS_TMODE_SDMA1;	.val	2;	.scl	16;	.type	013;	.endef
	.def	_GFS_TMODE_CPU;	.val	3;	.scl	16;	.type	013;	.endef
	.def	_GFS_TMODE_STM;	.val	4;	.scl	16;	.type	013;	.endef
	.def	_GFS_TMODE_END;	.val	5;	.scl	16;	.type	013;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_GfsTransMode;	.size	4;	.endef
	.def	_GfsServerStatus;	.scl	15;	.type	012;	.size	4;	.endef
	.def	_GFS_SVR_COMPLETED;	.val	0;	.scl	16;	.type	013;	.endef
	.def	_GFS_SVR_BUSY;	.val	1;	.scl	16;	.type	013;	.endef
	.def	_GFS_SVR_CDPAUSE;	.val	2;	.scl	16;	.type	013;	.endef
	.def	_GFS_SVR_ERROR;	.val	3;	.scl	16;	.type	013;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_GfsServerStatus;	.size	4;	.endef
	.def	_GfsDirType;	.scl	15;	.type	012;	.size	4;	.endef
	.def	_GFS_DIR_ID;	.val	0;	.scl	16;	.type	013;	.endef
	.def	_GFS_DIR_NAME;	.val	1;	.scl	16;	.type	013;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_GfsDirType;	.size	4;	.endef
	.def	_.29fake;	.scl	10;	.type	010;	.size	28;	.endef
	.def	_fid;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_finfo;	.val	4;	.scl	8;	.tag	_.22fake;	.size	12;	.type	010;	.endef
	.def	_sctsz;	.val	16;	.scl	8;	.type	05;	.endef
	.def	_nsct;	.val	20;	.scl	8;	.type	05;	.endef
	.def	_lstrm;	.val	24;	.scl	8;	.type	05;	.endef
	.def	.eos;	.val	28;	.scl	102;	.tag	_.29fake;	.size	28;	.endef
	.def	_GfsFinfo;	.scl	13;	.tag	_.29fake;	.size	28;	.type	010;	.endef
	.def	_.30fake;	.scl	10;	.type	010;	.size	44;	.endef
	.def	_bufno;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_sctsz;	.val	4;	.scl	8;	.type	05;	.endef
	.def	_flt;	.val	8;	.scl	8;	.type	05;	.endef
	.def	_subh;	.val	12;	.scl	8;	.tag	_.20fake;	.size	6;	.type	010;	.endef
	.def	_fmode;	.val	20;	.scl	8;	.type	05;	.endef
	.def	_puid;	.val	24;	.scl	8;	.type	05;	.endef
	.def	_filepos;	.val	28;	.scl	8;	.type	05;	.endef
	.def	_sctpos;	.val	32;	.scl	8;	.type	05;	.endef
	.def	_sfad;	.val	36;	.scl	8;	.type	05;	.endef
	.def	_efad;	.val	40;	.scl	8;	.type	05;	.endef
	.def	.eos;	.val	44;	.scl	102;	.tag	_.30fake;	.size	44;	.endef
	.def	_GfsCdRsrc;	.scl	13;	.tag	_.30fake;	.size	44;	.type	010;	.endef
	.def	_.31fake;	.scl	10;	.type	010;	.size	16;	.endef
	.def	_fid;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_filepos;	.val	4;	.scl	8;	.type	05;	.endef
	.def	_sctpos;	.val	8;	.scl	8;	.type	05;	.endef
	.def	_sctnum;	.val	12;	.scl	8;	.type	05;	.endef
	.def	.eos;	.val	16;	.scl	102;	.tag	_.31fake;	.size	16;	.endef
	.def	_GfsScsiRsrc;	.scl	13;	.tag	_.31fake;	.size	16;	.type	010;	.endef
	.def	_.32fake;	.scl	10;	.type	010;	.size	16;	.endef
	.def	_data;	.val	0;	.scl	8;	.type	021;	.endef
	.def	_filepos;	.val	4;	.scl	8;	.type	05;	.endef
	.def	_sctpos;	.val	8;	.scl	8;	.type	05;	.endef
	.def	_sctnum;	.val	12;	.scl	8;	.type	05;	.endef
	.def	.eos;	.val	16;	.scl	102;	.tag	_.32fake;	.size	16;	.endef
	.def	_GfsMemRsrc;	.scl	13;	.tag	_.32fake;	.size	16;	.type	010;	.endef
	.def	_.33fake;	.scl	12;	.type	011;	.size	44;	.endef
	.def	_cd;	.val	0;	.scl	11;	.tag	_.30fake;	.size	44;	.type	010;	.endef
	.def	_scsi;	.val	0;	.scl	11;	.tag	_.31fake;	.size	16;	.type	010;	.endef
	.def	_mem;	.val	0;	.scl	11;	.tag	_.32fake;	.size	16;	.type	010;	.endef
	.def	.eos;	.val	44;	.scl	102;	.tag	_.33fake;	.size	44;	.endef
	.def	_.34fake;	.scl	10;	.type	010;	.size	48;	.endef
	.def	_ftype;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_rsrc;	.val	4;	.scl	8;	.tag	_.33fake;	.size	44;	.type	011;	.endef
	.def	.eos;	.val	48;	.scl	102;	.tag	_.34fake;	.size	48;	.endef
	.def	_GfsDtsrc;	.scl	13;	.tag	_.34fake;	.size	48;	.type	010;	.endef
	.def	_.35fake;	.scl	10;	.type	010;	.size	96;	.endef
	.def	_finfo;	.val	0;	.scl	8;	.tag	_.29fake;	.size	28;	.type	010;	.endef
	.def	_dtsrc;	.val	28;	.scl	8;	.tag	_.34fake;	.size	48;	.type	010;	.endef
	.def	_gmode;	.val	76;	.scl	8;	.type	05;	.endef
	.def	_stat;	.val	80;	.scl	8;	.type	05;	.endef
	.def	_sct;	.val	84;	.scl	8;	.type	05;	.endef
	.def	_sctcnt;	.val	88;	.scl	8;	.type	05;	.endef
	.def	_sctmax;	.val	92;	.scl	8;	.type	05;	.endef
	.def	.eos;	.val	96;	.scl	102;	.tag	_.35fake;	.size	96;	.endef
	.def	_GfsFlow;	.scl	13;	.tag	_.35fake;	.size	96;	.type	010;	.endef
	.def	_.36fake;	.scl	10;	.type	010;	.size	20;	.endef
	.def	_data;	.val	0;	.scl	8;	.type	021;	.endef
	.def	_adlt;	.val	4;	.scl	8;	.type	05;	.endef
	.def	_len;	.val	8;	.scl	8;	.type	05;	.endef
	.def	_nsct;	.val	12;	.scl	8;	.type	05;	.endef
	.def	_use;	.val	16;	.scl	8;	.type	04;	.endef
	.def	.eos;	.val	20;	.scl	102;	.tag	_.36fake;	.size	20;	.endef
	.def	_GfsDataPack;	.scl	13;	.tag	_.36fake;	.size	20;	.type	010;	.endef
	.def	_GfdpHn;	.scl	13;	.tag	_.36fake;	.size	20;	.type	030;	.endef
	.def	_GfsTransFunc;	.scl	13;	.type	0225;	.endef
	.def	_.37fake;	.scl	10;	.type	010;	.size	60;	.endef
	.def	_buf;	.val	0;	.scl	8;	.type	021;	.endef
	.def	_size;	.val	4;	.scl	8;	.type	05;	.endef
	.def	_wp;	.val	8;	.scl	8;	.type	05;	.endef
	.def	_dps;	.val	12;	.scl	8;	.tag	_.36fake;	.size	20;	.type	030;	.endef
	.def	_dpd;	.val	16;	.scl	8;	.tag	_.36fake;	.size	20;	.type	030;	.endef
	.def	_tsctmax;	.val	20;	.scl	8;	.type	05;	.endef
	.def	_tsct;	.val	24;	.scl	8;	.type	05;	.endef
	.def	_tsctcnt;	.val	28;	.scl	8;	.type	05;	.endef
	.def	_tbytcnt;	.val	32;	.scl	8;	.type	05;	.endef
	.def	_obj;	.val	36;	.scl	8;	.type	021;	.endef
	.def	_tfunc;	.val	40;	.scl	8;	.type	0225;	.endef
	.def	_unit;	.val	44;	.scl	8;	.type	05;	.endef
	.def	_active;	.val	48;	.scl	8;	.type	04;	.endef
	.def	_stat;	.val	52;	.scl	8;	.type	05;	.endef
	.def	_mode;	.val	56;	.scl	8;	.type	05;	.endef
	.def	.eos;	.val	60;	.scl	102;	.tag	_.37fake;	.size	60;	.endef
	.def	_GfsTrans;	.scl	13;	.tag	_.37fake;	.size	60;	.type	010;	.endef
	.def	_GftrHn;	.scl	13;	.tag	_.37fake;	.size	60;	.type	030;	.endef
	.def	_.38fake;	.scl	10;	.type	010;	.size	168;	.endef
	.def	_used;	.val	0;	.scl	8;	.type	04;	.endef
	.def	_amode;	.val	4;	.scl	8;	.type	05;	.endef
	.def	_astat;	.val	8;	.scl	8;	.type	05;	.endef
	.def	_flow;	.val	12;	.scl	8;	.tag	_.35fake;	.size	96;	.type	010;	.endef
	.def	_trans;	.val	108;	.scl	8;	.tag	_.37fake;	.size	60;	.type	010;	.endef
	.def	.eos;	.val	168;	.scl	102;	.tag	_.38fake;	.size	168;	.endef
	.def	_GfsFile;	.scl	13;	.tag	_.38fake;	.size	168;	.type	010;	.endef
	.def	_GfsHn;	.scl	13;	.tag	_.38fake;	.size	168;	.type	030;	.endef
	.def	_.39fake;	.scl	10;	.type	010;	.size	12;	.endef
	.def	_dirrec;	.val	0;	.scl	8;	.tag	_.22fake;	.size	12;	.type	010;	.endef
	.def	.eos;	.val	12;	.scl	102;	.tag	_.39fake;	.size	12;	.endef
	.def	_GfsDirId;	.scl	13;	.tag	_.39fake;	.size	12;	.type	010;	.endef
	.def	_.40fake;	.scl	10;	.type	010;	.size	24;	.endef
	.def	_dirrec;	.val	0;	.scl	8;	.tag	_.22fake;	.size	12;	.type	010;	.endef
	.def	_fname;	.val	12;	.scl	8;	.dim	12;	.size	12;	.type	062;	.endef
	.def	.eos;	.val	24;	.scl	102;	.tag	_.40fake;	.size	24;	.endef
	.def	_GfsDirName;	.scl	13;	.tag	_.40fake;	.size	24;	.type	010;	.endef
	.def	_.41fake;	.scl	12;	.type	011;	.size	4;	.endef
	.def	_dir_i;	.val	0;	.scl	11;	.tag	_.39fake;	.size	12;	.type	030;	.endef
	.def	_dir_n;	.val	0;	.scl	11;	.tag	_.40fake;	.size	24;	.type	030;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_.41fake;	.size	4;	.endef
	.def	_.42fake;	.scl	10;	.type	010;	.size	12;	.endef
	.def	_type;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_ndir;	.val	4;	.scl	8;	.type	05;	.endef
	.def	_dir;	.val	8;	.scl	8;	.tag	_.41fake;	.size	4;	.type	011;	.endef
	.def	.eos;	.val	12;	.scl	102;	.tag	_.42fake;	.size	12;	.endef
	.def	_GfsDirTbl;	.scl	13;	.tag	_.42fake;	.size	12;	.type	010;	.endef
	.def	_GfsErrFunc;	.scl	13;	.type	0221;	.endef
	.def	_.43fake;	.scl	10;	.type	010;	.size	12;	.endef
	.def	_func;	.val	0;	.scl	8;	.type	0221;	.endef
	.def	_obj;	.val	4;	.scl	8;	.type	021;	.endef
	.def	_code;	.val	8;	.scl	8;	.type	05;	.endef
	.def	.eos;	.val	12;	.scl	102;	.tag	_.43fake;	.size	12;	.endef
	.def	_GfsErrStat;	.scl	13;	.tag	_.43fake;	.size	12;	.type	010;	.endef
	.def	_.44fake;	.scl	10;	.type	010;	.size	16;	.endef
	.def	_flowin;	.val	0;	.scl	8;	.type	0225;	.endef
	.def	_stopin;	.val	4;	.scl	8;	.type	0221;	.endef
	.def	_seek;	.val	8;	.scl	8;	.type	0225;	.endef
	.def	_tell;	.val	12;	.scl	8;	.type	0225;	.endef
	.def	.eos;	.val	16;	.scl	102;	.tag	_.44fake;	.size	16;	.endef
	.def	_GfsFileFunc;	.scl	13;	.tag	_.44fake;	.size	16;	.type	010;	.endef
	.def	_.45fake;	.scl	10;	.type	010;	.size	100;	.endef
	.def	_access_file;	.val	0;	.scl	8;	.tag	_.38fake;	.dim	24;	.size	96;	.type	0170;	.endef
	.def	_nfile;	.val	96;	.scl	8;	.type	05;	.endef
	.def	.eos;	.val	100;	.scl	102;	.tag	_.45fake;	.size	100;	.endef
	.def	_GfsSvr;	.scl	13;	.tag	_.45fake;	.size	100;	.type	010;	.endef
	.def	_.46fake;	.scl	10;	.type	010;	.size	16;	.endef
	.def	_dirtbl;	.val	0;	.scl	8;	.tag	_.42fake;	.size	12;	.type	010;	.endef
	.def	_nfile;	.val	12;	.scl	8;	.type	05;	.endef
	.def	.eos;	.val	16;	.scl	102;	.tag	_.46fake;	.size	16;	.endef
	.def	_GfsDirMng;	.scl	13;	.tag	_.46fake;	.size	16;	.type	010;	.endef
	.def	_.47fake;	.scl	10;	.type	010;	.size	16;	.endef
	.def	_flt;	.val	0;	.scl	8;	.type	014;	.endef
	.def	_fmode;	.val	1;	.scl	8;	.type	014;	.endef
	.def	_subh;	.val	2;	.scl	8;	.tag	_.20fake;	.size	6;	.type	010;	.endef
	.def	_fad;	.val	8;	.scl	8;	.type	05;	.endef
	.def	_snum;	.val	12;	.scl	8;	.type	05;	.endef
	.def	.eos;	.val	16;	.scl	102;	.tag	_.47fake;	.size	16;	.endef
	.def	_GfcdSelQu;	.scl	13;	.tag	_.47fake;	.size	16;	.type	010;	.endef
	.def	_.48fake;	.scl	10;	.type	010;	.size	12;	.endef
	.def	_flt;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_buf;	.val	4;	.scl	8;	.type	05;	.endef
	.def	_flnout;	.val	8;	.scl	8;	.type	05;	.endef
	.def	.eos;	.val	12;	.scl	102;	.tag	_.48fake;	.size	12;	.endef
	.def	_GfcdFconQu;	.scl	13;	.tag	_.48fake;	.size	12;	.type	010;	.endef
	.def	_.49fake;	.scl	10;	.type	010;	.size	392;	.endef
	.def	_len;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_stat;	.val	4;	.scl	8;	.type	05;	.endef
	.def	_selq;	.val	8;	.scl	8;	.tag	_.47fake;	.dim	24;	.size	384;	.type	070;	.endef
	.def	.eos;	.val	392;	.scl	102;	.tag	_.49fake;	.size	392;	.endef
	.def	_.50fake;	.scl	10;	.type	010;	.size	296;	.endef
	.def	_len;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_stat;	.val	4;	.scl	8;	.type	05;	.endef
	.def	_fconq;	.val	8;	.scl	8;	.tag	_.48fake;	.dim	24;	.size	288;	.type	070;	.endef
	.def	.eos;	.val	296;	.scl	102;	.tag	_.50fake;	.size	296;	.endef
	.def	_.51fake;	.scl	10;	.type	010;	.size	8;	.endef
	.def	_stat;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_flt;	.val	4;	.scl	8;	.type	05;	.endef
	.def	.eos;	.val	8;	.scl	102;	.tag	_.51fake;	.size	8;	.endef
	.def	_.52fake;	.scl	10;	.type	010;	.size	28;	.endef
	.def	_stat;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_bufno;	.val	4;	.scl	8;	.type	05;	.endef
	.def	_spos;	.val	8;	.scl	8;	.type	05;	.endef
	.def	_usct;	.val	12;	.scl	8;	.type	05;	.endef
	.def	_cnt;	.val	16;	.scl	8;	.type	05;	.endef
	.def	_nsct;	.val	20;	.scl	8;	.type	025;	.endef
	.def	_nbyte;	.val	24;	.scl	8;	.type	025;	.endef
	.def	.eos;	.val	28;	.scl	102;	.tag	_.52fake;	.size	28;	.endef
	.def	_.53fake;	.scl	10;	.type	010;	.size	16;	.endef
	.def	_stat;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_bufno;	.val	4;	.scl	8;	.type	05;	.endef
	.def	_sctpos;	.val	8;	.scl	8;	.type	05;	.endef
	.def	_nsct;	.val	12;	.scl	8;	.type	05;	.endef
	.def	.eos;	.val	16;	.scl	102;	.tag	_.53fake;	.size	16;	.endef
	.def	_.54fake;	.scl	10;	.type	010;	.size	16;	.endef
	.def	_stat;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_bufno;	.val	4;	.scl	8;	.type	05;	.endef
	.def	_sctpos;	.val	8;	.scl	8;	.type	05;	.endef
	.def	_nsct;	.val	12;	.scl	8;	.type	05;	.endef
	.def	.eos;	.val	16;	.scl	102;	.tag	_.54fake;	.size	16;	.endef
	.def	_.55fake;	.scl	10;	.type	010;	.size	24;	.endef
	.def	_stat;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_dst;	.val	4;	.scl	8;	.type	05;	.endef
	.def	_src;	.val	8;	.scl	8;	.type	05;	.endef
	.def	_spos;	.val	12;	.scl	8;	.type	05;	.endef
	.def	_snum;	.val	16;	.scl	8;	.type	05;	.endef
	.def	_fmode;	.val	20;	.scl	8;	.type	05;	.endef
	.def	.eos;	.val	24;	.scl	102;	.tag	_.55fake;	.size	24;	.endef
	.def	_.56fake;	.scl	10;	.type	010;	.size	12;	.endef
	.def	_stat;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_fid;	.val	4;	.scl	8;	.type	03;	.endef
	.def	_work;	.val	6;	.scl	8;	.type	03;	.endef
	.def	_ndir;	.val	8;	.scl	8;	.type	025;	.endef
	.def	.eos;	.val	12;	.scl	102;	.tag	_.56fake;	.size	12;	.endef
	.def	_.57fake;	.scl	10;	.type	010;	.size	876;	.endef
	.def	_use_buf;	.val	0;	.scl	8;	.dim	24;	.size	24;	.type	062;	.endef
	.def	_use_filt;	.val	24;	.scl	8;	.dim	24;	.size	24;	.type	062;	.endef
	.def	_use_pu;	.val	48;	.scl	8;	.type	04;	.endef
	.def	_tr_bufno;	.val	52;	.scl	8;	.type	05;	.endef
	.def	_puid;	.val	56;	.scl	8;	.type	05;	.endef
	.def	_timer;	.val	60;	.scl	8;	.type	05;	.endef
	.def	_stat;	.val	64;	.scl	8;	.tag	_.14fake;	.size	12;	.type	010;	.endef
	.def	_func;	.val	76;	.scl	8;	.type	0221;	.endef
	.def	_obj;	.val	80;	.scl	8;	.type	021;	.endef
	.def	_tsk_setflt;	.val	84;	.scl	8;	.tag	_.49fake;	.size	392;	.type	010;	.endef
	.def	_tsk_fltcon;	.val	476;	.scl	8;	.tag	_.50fake;	.size	296;	.type	010;	.endef
	.def	_tsk_setcon;	.val	772;	.scl	8;	.tag	_.51fake;	.size	8;	.type	010;	.endef
	.def	_tsk_getlen;	.val	780;	.scl	8;	.tag	_.52fake;	.size	28;	.type	010;	.endef
	.def	_tsk_reqdat;	.val	808;	.scl	8;	.tag	_.53fake;	.size	16;	.type	010;	.endef
	.def	_tsk_delsct;	.val	824;	.scl	8;	.tag	_.54fake;	.size	16;	.type	010;	.endef
	.def	_tsk_movsct;	.val	840;	.scl	8;	.tag	_.55fake;	.size	24;	.type	010;	.endef
	.def	_tsk_chgdir;	.val	864;	.scl	8;	.tag	_.56fake;	.size	12;	.type	010;	.endef
	.def	.eos;	.val	876;	.scl	102;	.tag	_.57fake;	.size	876;	.endef
	.def	_GfsCdbMng;	.scl	13;	.tag	_.57fake;	.size	876;	.type	010;	.endef
	.def	_.58fake;	.scl	10;	.type	010;	.size	3336;	.endef
	.def	_openmax;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_functbl;	.val	4;	.scl	8;	.tag	_.44fake;	.dim	3;	.size	48;	.type	070;	.endef
	.def	_svr;	.val	52;	.scl	8;	.tag	_.45fake;	.size	100;	.type	010;	.endef
	.def	_curdir;	.val	152;	.scl	8;	.tag	_.46fake;	.size	16;	.type	010;	.endef
	.def	_pickup;	.val	168;	.scl	8;	.tag	_.38fake;	.size	168;	.type	030;	.endef
	.def	_sfad;	.val	172;	.scl	8;	.type	05;	.endef
	.def	_efad;	.val	176;	.scl	8;	.type	05;	.endef
	.def	_trans;	.val	180;	.scl	8;	.tag	_.38fake;	.size	168;	.type	030;	.endef
	.def	_error;	.val	184;	.scl	8;	.tag	_.43fake;	.size	12;	.type	010;	.endef
	.def	_flags;	.val	196;	.scl	8;	.type	017;	.endef
	.def	_timer;	.val	200;	.scl	8;	.type	05;	.endef
	.def	_cdb;	.val	204;	.scl	8;	.tag	_.57fake;	.size	876;	.type	010;	.endef
	.def	_srcpk;	.val	1080;	.scl	8;	.tag	_.36fake;	.size	20;	.type	010;	.endef
	.def	_dstpk;	.val	1100;	.scl	8;	.tag	_.36fake;	.size	20;	.type	010;	.endef
	.def	_sect_buf;	.val	1120;	.scl	8;	.dim	2048;	.size	2048;	.type	074;	.endef
	.def	_file;	.val	3168;	.scl	8;	.tag	_.38fake;	.dim	1;	.size	168;	.type	070;	.endef
	.def	.eos;	.val	3336;	.scl	102;	.tag	_.58fake;	.size	3336;	.endef
	.def	_GfsMng;	.scl	13;	.tag	_.58fake;	.size	3336;	.type	010;	.endef
	.def	_.59fake;	.scl	10;	.type	010;	.size	16;	.endef
	.def	_cgadd;	.val	0;	.scl	8;	.type	034;	.endef
	.def	_cgsize;	.val	4;	.scl	8;	.type	017;	.endef
	.def	_clradd;	.val	8;	.scl	8;	.type	035;	.endef
	.def	_clrcnt;	.val	12;	.scl	8;	.type	015;	.endef
	.def	.eos;	.val	16;	.scl	102;	.tag	_.59fake;	.size	16;	.endef
	.def	_SPRCG;	.scl	13;	.tag	_.59fake;	.size	16;	.type	010;	.endef
	.def	_.60fake;	.scl	10;	.type	010;	.size	20;	.endef
	.def	_cgadd;	.val	0;	.scl	8;	.type	034;	.endef
	.def	_cgsize;	.val	4;	.scl	8;	.type	05;	.endef
	.def	_mapadd;	.val	8;	.scl	8;	.type	035;	.endef
	.def	_clradd;	.val	12;	.scl	8;	.type	035;	.endef
	.def	_clrcnt;	.val	16;	.scl	8;	.type	015;	.endef
	.def	_vsize;	.val	18;	.scl	8;	.type	014;	.endef
	.def	_hsize;	.val	19;	.scl	8;	.type	014;	.endef
	.def	.eos;	.val	20;	.scl	102;	.tag	_.60fake;	.size	20;	.endef
	.def	_FIXCG;	.scl	13;	.tag	_.60fake;	.size	20;	.type	010;	.endef
	.def	_.61fake;	.scl	10;	.type	010;	.size	8;	.endef
	.def	_mapadd;	.val	0;	.scl	8;	.type	035;	.endef
	.def	_vsize;	.val	4;	.scl	8;	.type	014;	.endef
	.def	_hsize;	.val	5;	.scl	8;	.type	014;	.endef
	.def	.eos;	.val	8;	.scl	102;	.tag	_.61fake;	.size	8;	.endef
	.def	_BOXMAP;	.scl	13;	.tag	_.61fake;	.size	8;	.type	010;	.endef
	.def	_.62fake;	.scl	10;	.type	010;	.size	8;	.endef
	.def	_cgadd;	.val	0;	.scl	8;	.type	034;	.endef
	.def	_cgsize;	.val	4;	.scl	8;	.type	05;	.endef
	.def	.eos;	.val	8;	.scl	102;	.tag	_.62fake;	.size	8;	.endef
	.def	_FONTCG;	.scl	13;	.tag	_.62fake;	.size	8;	.type	010;	.endef
	.def	_.63fake;	.scl	10;	.type	010;	.size	8;	.endef
	.def	_cgadd;	.val	0;	.scl	8;	.type	035;	.endef
	.def	_cgsize;	.val	4;	.scl	8;	.type	05;	.endef
	.def	.eos;	.val	8;	.scl	102;	.tag	_.63fake;	.size	8;	.endef
	.def	_WORDBLOCK;	.scl	13;	.tag	_.63fake;	.size	8;	.type	010;	.endef
	.def	_.64fake;	.scl	15;	.type	012;	.size	4;	.endef
	.def	_DUMMY_MODE;	.val	0;	.scl	16;	.type	013;	.endef
	.def	_LOGO_MODE;	.val	1;	.scl	16;	.type	013;	.endef
	.def	_TITLE_MODE;	.val	2;	.scl	16;	.type	013;	.endef
	.def	_SELECT_MODE;	.val	3;	.scl	16;	.type	013;	.endef
	.def	_SMSC_SELECTMODE;	.val	4;	.scl	16;	.type	013;	.endef
	.def	_SMSC_MODE;	.val	5;	.scl	16;	.type	013;	.endef
	.def	_SMSC_BACK;	.val	6;	.scl	16;	.type	013;	.endef
	.def	_SMSC_NORMSCLMODE;	.val	7;	.scl	16;	.type	013;	.endef
	.def	_SMSC_MULTISCLMODE;	.val	8;	.scl	16;	.type	013;	.endef
	.def	_SMSC_ROTSCLMODE;	.val	9;	.scl	16;	.type	013;	.endef
	.def	_LNSC_MODE;	.val	10;	.scl	16;	.type	013;	.endef
	.def	_LCLR_MODE;	.val	11;	.scl	16;	.type	013;	.endef
	.def	_SPSAMPLE_MODE;	.val	12;	.scl	16;	.type	013;	.endef
	.def	_WIND_MODE;	.val	13;	.scl	16;	.type	013;	.endef
	.def	_GAME_MODE;	.val	14;	.scl	16;	.type	013;	.endef
	.def	_DEMO214_MODE;	.val	15;	.scl	16;	.type	013;	.endef
	.def	_SMRK_MODE;	.val	16;	.scl	16;	.type	013;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_.64fake;	.size	4;	.endef
	.def	_DUMMYACT;	.scl	10;	.type	010;	.size	20;	.endef
	.def	_actid;	.val	0;	.scl	8;	.type	014;	.endef
	.def	_level;	.val	1;	.scl	8;	.type	014;	.endef
	.def	_mode;	.val	2;	.scl	8;	.type	014;	.endef
	.def	_status;	.val	3;	.scl	8;	.type	014;	.endef
	.def	_pcbuff;	.val	4;	.scl	8;	.type	0221;	.endef
	.def	_msttbl;	.val	8;	.scl	8;	.tag	_DUMMYACT;	.size	20;	.type	030;	.endef
	.def	_subtbl;	.val	12;	.scl	8;	.tag	_DUMMYACT;	.size	20;	.type	030;	.endef
	.def	_akiwk;	.val	16;	.scl	8;	.type	014;	.endef
	.def	.eos;	.val	20;	.scl	102;	.tag	_DUMMYACT;	.size	20;	.endef
	.def	_DUMMYACT;	.scl	13;	.tag	_DUMMYACT;	.size	20;	.type	010;	.endef
	.def	_ACTWK;	.scl	10;	.type	010;	.size	128;	.endef
	.def	_actid;	.val	0;	.scl	8;	.type	014;	.endef
	.def	_level;	.val	1;	.scl	8;	.type	014;	.endef
	.def	_mode;	.val	2;	.scl	8;	.type	014;	.endef
	.def	_status;	.val	3;	.scl	8;	.type	014;	.endef
	.def	_pcbuff;	.val	4;	.scl	8;	.type	0221;	.endef
	.def	_msttbl;	.val	8;	.scl	8;	.tag	_ACTWK;	.size	128;	.type	030;	.endef
	.def	_subtbl;	.val	12;	.scl	8;	.tag	_ACTWK;	.size	128;	.type	030;	.endef
	.def	_free;	.val	16;	.scl	8;	.dim	109;	.size	109;	.type	074;	.endef
	.def	.eos;	.val	128;	.scl	102;	.tag	_ACTWK;	.size	128;	.endef
	.def	_ACTWK;	.scl	13;	.tag	_ACTWK;	.size	128;	.type	010;	.endef
	.def	_.65fake;	.scl	15;	.type	012;	.size	4;	.endef
	.def	_PMD_sprx_nrm;	.val	0;	.scl	16;	.type	013;	.endef
	.def	_PMD_sprx_zoom;	.val	1;	.scl	16;	.type	013;	.endef
	.def	_PMD_sprx_rotzoom;	.val	2;	.scl	16;	.type	013;	.endef
	.def	_PMD_sprx_line;	.val	3;	.scl	16;	.type	013;	.endef
	.def	_PMD_sprx_npolygon;	.val	4;	.scl	16;	.type	013;	.endef
	.def	_PMD_sprx_zpolygon;	.val	5;	.scl	16;	.type	013;	.endef
	.def	_PMD_sprx_rzpolygon;	.val	6;	.scl	16;	.type	013;	.endef
	.def	_PMD_sprx_rzpolyline;	.val	7;	.scl	16;	.type	013;	.endef
	.def	_PMD_sprx_4pset;	.val	8;	.scl	16;	.type	013;	.endef
	.def	.eos;	.val	4;	.scl	102;	.tag	_.65fake;	.size	4;	.endef
	.def	_SPRITE;	.scl	10;	.type	010;	.size	80;	.endef
	.def	_putmode;	.val	0;	.scl	8;	.type	014;	.endef
	.def	_patno;	.val	1;	.scl	8;	.type	014;	.endef
	.def	_patchgcnt;	.val	2;	.scl	8;	.type	014;	.endef
	.def	_patchgtim;	.val	3;	.scl	8;	.type	014;	.endef
	.def	_sprhsize;	.val	4;	.scl	8;	.type	014;	.endef
	.def	_sprvsize;	.val	5;	.scl	8;	.type	014;	.endef
	.def	_pmode;	.val	6;	.scl	8;	.type	03;	.endef
	.def	_color;	.val	8;	.scl	8;	.type	03;	.endef
	.def	_sproffset;	.val	10;	.scl	8;	.type	03;	.endef
	.def	_patbase;	.val	12;	.scl	8;	.type	0123;	.endef
	.def	_xposi;	.val	16;	.scl	8;	.type	05;	.endef
	.def	_yposi;	.val	20;	.scl	8;	.type	05;	.endef
	.def	_zposi;	.val	24;	.scl	8;	.type	05;	.endef
	.def	_posi;	.val	28;	.scl	8;	.dim	4,3;	.size	24;	.type	0363;	.endef
	.def	_xspd;	.val	52;	.scl	8;	.type	03;	.endef
	.def	_yspd;	.val	54;	.scl	8;	.type	03;	.endef
	.def	_zspd;	.val	56;	.scl	8;	.type	03;	.endef
	.def	_xacc;	.val	58;	.scl	8;	.type	03;	.endef
	.def	_yacc;	.val	60;	.scl	8;	.type	03;	.endef
	.def	_zacc;	.val	62;	.scl	8;	.type	03;	.endef
	.def	_xdirec;	.val	64;	.scl	8;	.type	03;	.endef
	.def	_ydirec;	.val	66;	.scl	8;	.type	03;	.endef
	.def	_zdirec;	.val	68;	.scl	8;	.type	03;	.endef
	.def	_vzoom;	.val	70;	.scl	8;	.type	03;	.endef
	.def	_hzoom;	.val	72;	.scl	8;	.type	03;	.endef
	.def	_sprprio;	.val	74;	.scl	8;	.type	03;	.endef
	.def	_linktbl;	.val	76;	.scl	8;	.type	034;	.endef
	.def	.eos;	.val	80;	.scl	102;	.tag	_SPRITE;	.size	80;	.endef
	.def	_SPRITE;	.scl	13;	.tag	_SPRITE;	.size	80;	.type	010;	.endef
	.def	_.66fake;	.scl	10;	.type	010;	.size	96;	.endef
	.def	_xst;	.val	0;	.scl	8;	.type	05;	.endef
	.def	_yst;	.val	4;	.scl	8;	.type	05;	.endef
	.def	_zst;	.val	8;	.scl	8;	.type	05;	.endef
	.def	_dxst;	.val	12;	.scl	8;	.type	05;	.endef
	.def	_dyst;	.val	16;	.scl	8;	.type	05;	.endef
	.def	_dx;	.val	20;	.scl	8;	.type	05;	.endef
	.def	_dy;	.val	24;	.scl	8;	.type	05;	.endef
	.def	_mtr_a;	.val	28;	.scl	8;	.type	05;	.endef
	.def	_mtr_b;	.val	32;	.scl	8;	.type	05;	.endef
	.def	_mtr_c;	.val	36;	.scl	8;	.type	05;	.endef
	.def	_mtr_d;	.val	40;	.scl	8;	.type	05;	.endef
	.def	_mtr_e;	.val	44;	.scl	8;	.type	05;	.endef
	.def	_mtr_f;	.val	48;	.scl	8;	.type	05;	.endef
	.def	_px;	.val	52;	.scl	8;	.type	03;	.endef
	.def	_py;	.val	54;	.scl	8;	.type	03;	.endef
	.def	_pz;	.val	56;	.scl	8;	.type	03;	.endef
	.def	_pdummy;	.val	58;	.scl	8;	.type	03;	.endef
	.def	_cx;	.val	60;	.scl	8;	.type	03;	.endef
	.def	_cy;	.val	62;	.scl	8;	.type	03;	.endef
	.def	_cz;	.val	64;	.scl	8;	.type	03;	.endef
	.def	_cdummy;	.val	66;	.scl	8;	.type	03;	.endef
	.def	_mx;	.val	68;	.scl	8;	.type	05;	.endef
	.def	_my;	.val	72;	.scl	8;	.type	05;	.endef
	.def	_kx;	.val	76;	.scl	8;	.type	05;	.endef
	.def	_ky;	.val	80;	.scl	8;	.type	05;	.endef
	.def	_kast;	.val	84;	.scl	8;	.type	05;	.endef
	.def	_dkasy;	.val	88;	.scl	8;	.type	05;	.endef
	.def	_dkasx;	.val	92;	.scl	8;	.type	05;	.endef
	.def	.eos;	.val	96;	.scl	102;	.tag	_.66fake;	.size	96;	.endef
	.def	_ROTPARAM;	.scl	13;	.tag	_.66fake;	.size	96;	.type	010;	.endef
	.data
	.text
	.data
	.align 2
_SMV1_sprwk:
	.short	1024
	.short	128
	.short	2
	.short	512
	.short	512
	.space 2
	.long	_sp2CharTbl
	.long	_sp2CmdChain
	.long	_sp2DrawPrtyBlk
	.global	_SMV1_SpritePutModeTbl
	.align 2
_SMV1_SpritePutModeTbl:
	.long	_put_nsprite_sx
	.long	_put_zsprite_sx
	.long	_put_rzsprite_sx
	.long	_put_line_sx
	.long	_put_polygon_sx
	.long	_put_zpolygon_sx
	.long	_put_rzpolygon_sx
	.long	_put_rzpolyline_sx
	.long	_put_4psprite_sx
	.text
	.align 1
	.def	_SMV1_SetFont4sp;	.val	_SMV1_SetFont4sp;	.scl	2;	.type	041;	.endef
	.global	_SMV1_SetFont4sp
_SMV1_SetFont4sp:
	mov.l	r8,@-r15
	mov.l	r9,@-r15
	mov.l	r10,@-r15
	mov.l	r11,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15 ! push
	add	#-48,r15
	mov	r15,r14
	mov	r5,r2
	mov	r6,r0
	mov	r4,r11
	.def	.bf;	.val	.;	.scl	101;	.line	57;	.endef
	.def	_topcharno;	.val	11;	.scl	17;	.type	014;	.endef
	.def	_fontcolor;	.val	2;	.scl	17;	.type	017;	.endef
	.def	_backcolor;	.val	0;	.scl	17;	.type	017;	.endef
	.ln	2
	.def	_src;	.val	8;	.scl	4;	.type	034;	.endef
	.def	_dst;	.val	2;	.scl	4;	.type	034;	.endef
	.def	_rdata;	.val	7;	.scl	4;	.type	014;	.endef
	.def	_i;	.val	9;	.scl	4;	.type	015;	.endef
	.def	_j;	.val	6;	.scl	4;	.type	015;	.endef
	.def	_k;	.val	4;	.scl	4;	.type	015;	.endef
	.def	_celbuf;	.val	0;	.scl	1;	.dim	32;	.size	32;	.type	074;	.endef
	.def	_fontdata;	.val	32;	.scl	1;	.dim	4;	.size	16;	.type	077;	.endef
	.ln	9
	mov	#15,r12
	and	r12,r2
	.ln	10
	and	#15,r0
	.ln	11
	mov	r0,r1
	shll2	r1
	shll2	r1
	add	r0,r1
	mov.l	r1,@(32,r14)
	.ln	12
	mov	r0,r1
	shll2	r1
	shll2	r1
	add	r2,r1
	mov.l	r1,@(36,r14)
	.ln	13
	mov	r2,r1
	shll2	r1
	shll2	r1
	add	r0,r1
	mov.l	r1,@(40,r14)
	.ln	14
	mov	r2,r1
	shll2	r1
	shll2	r1
	add	r2,r1
	mov.l	r1,@(44,r14)
	.ln	17
	mov.l	L12,r8
	.ln	19
	mov	#0,r9
	mov	#7,r10
L5:
	.ln	20
	mov	r14,r2
	.ln	21
	mov	#0,r6
L9:
	.ln	22
	mov.b	@r8+,r7
	.ln	25
	extu.b	r7,r3
	mov	r3,r0
	shlr2	r0
	shlr2	r0
	and	#12,r0
	mov	r0,r1
	add	r14,r1
	add	#35,r1
	mov.b	@r1,r1
	mov.b	r1,@r2
	.ln	26
	add	#1,r2
	.ln	28
	mov	r3,r0
	shlr2	r0
	and	#12,r0
	mov	r0,r1
	add	r14,r1
	add	#35,r1
	mov.b	@r1,r1
	mov.b	r1,@r2
	.ln	29
	add	#1,r2
	.ln	31
	mov	r3,r0
	and	#12,r0
	mov	r0,r1
	add	r14,r1
	add	#35,r1
	mov.b	@r1,r1
	mov.b	r1,@r2
	.ln	32
	add	#1,r2
	.ln	34
	mov	r7,r0
	and	#3,r0
	mov	r0,r1
	shll2	r1
	add	r14,r1
	add	#35,r1
	mov.b	@r1,r1
	mov.b	r1,@r2
	.ln	21
	add	#1,r6
	extu.w	r6,r1
	cmp/hi	r10,r1
	bf.s	L9
	add	#1,r2
	.ln	37
	extu.b	r11,r4
	add	r9,r4
	.ln	38
	extu.w	r4,r4
	mov.l	r14,@-r15
	mov	#8,r7
	mov.l	r7,@-r15
	mov.l	L13,r1
	mov	#0,r6
	jsr	@r1
	mov	#0,r5
	.ln	19
	add	#1,r9
	extu.w	r9,r1
	mov	#63,r12
	cmp/hi	r12,r1
	bf.s	L5
	add	#8,r15
	.ln	42
	mov	r14,r15
	add	#48,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r11
	mov.l	@r15+,r10
	mov.l	@r15+,r9
	rts	
	mov.l	@r15+,r8
L14:
	.align 2
L12:
	.long	_font8cg
L13:
	.long	_SPR_2SetChar
	.def	.ef;	.val	.;	.scl	101;	.line	42;	.endef
	.def	_SMV1_SetFont4sp;	.val	.;	.scl	-1;	.endef
	.align 1
	.def	_put_nsprite_sx;	.val	_put_nsprite_sx;	.scl	3;	.type	041;	.endef
_put_nsprite_sx:
	mov.l	r8,@-r15
	mov.l	r9,@-r15
	mov.l	r10,@-r15
	mov.l	r11,@-r15
	mov.l	r12,@-r15
	mov.l	r13,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15 ! push
	add	#-24,r15
	mov	r15,r14
	.def	.bf;	.val	.;	.scl	101;	.line	105;	.endef
	.def	_spr;	.val	4;	.scl	17;	.tag	_SPRITE;	.size	80;	.type	030;	.endef
	.ln	2
	.def	_hvrev;	.val	5;	.scl	4;	.type	015;	.endef
	.def	_pmode;	.val	12;	.scl	4;	.type	015;	.endef
	.def	_color;	.val	22;	.scl	1;	.type	015;	.endef
	.def	_cgadr;	.val	7;	.scl	4;	.type	015;	.endef
	.def	_posi;	.val	0;	.scl	1;	.tag	_XyInt;	.dim	4;	.size	16;	.type	070;	.endef
	.def	_pattbl;	.val	8;	.scl	4;	.type	023;	.endef
	.def	_i;	.val	9;	.scl	4;	.type	03;	.endef
	.def	_xposi;	.val	18;	.scl	1;	.type	03;	.endef
	.def	_yposi;	.val	13;	.scl	4;	.type	03;	.endef
	.def	_hofst;	.val	1;	.scl	4;	.type	03;	.endef
	.def	_vofst;	.val	3;	.scl	4;	.type	03;	.endef
	.ln	7
	mov	r4,r1
	add	#1,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov.l	@(12,r4),r2
	shll2	r1
	mov	r1,r0
	mov.l	@(r0,r2),r8
	.ln	8
	mov	r4,r1
	add	#16,r1
	mov.w	@r1,r1
	mov	#18,r11
	add	r14,r11
	mov.w	r1,@r11
	.ln	9
	mov	r4,r1
	add	#20,r1
	mov.w	@r1,r13
	.ln	11
	mov	r4,r1
	add	#6,r1
	mov.w	@r1,r12
	.ln	12
	add	#8,r4
	mov.w	@r4,r4
	mov	#22,r0
	add	r14,r0
	mov.w	r4,@r0
	.ln	14
	mov.w	@r8+,r9
	exts.w	r9,r1
	cmp/pl	r1
	bf	L17
	mov.l	L21,r10
L19:
	.ln	15
	mov.w	@r8+,r7
	.ln	16
	mov.w	@r8+,r5
	.ln	18
	add	#4,r8
	.ln	19
	mov.w	@r8+,r1
	.ln	20
	mov.w	@r8+,r3
	.ln	22
	mov	#18,r11
	add	r14,r11
	mov.w	@r11,r11
	add	r11,r1
	mov.w	r1,@r14
	.ln	23
	mov	r14,r2
	add	#2,r2
	mov	r13,r1
	add	r3,r1
	mov.w	r1,@r2
	.ln	24
	extu.w	r5,r5
	mov.l	r10,@-r15
	mov.l	r14,@-r15
	extu.w	r7,r7
	mov.l	r7,@-r15
	mov	#22,r0
	add	r14,r0
	mov.w	@r0,r0
	extu.w	r0,r7
	extu.w	r12,r6
	mov.l	L22,r11
	jsr	@r11
	mov	#0,r4
	.ln	14
	add	#-1,r9
	exts.w	r9,r1
	cmp/pl	r1
	bt.s	L19
	add	#12,r15
L17:
	.ln	27
	mov	r14,r15
	add	#24,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r13
	mov.l	@r15+,r12
	mov.l	@r15+,r11
	mov.l	@r15+,r10
	mov.l	@r15+,r9
	rts	
	mov.l	@r15+,r8
L23:
	.align 2
L21:
	.long	65535
L22:
	.long	_SPR_2NormSpr
	.def	.ef;	.val	.;	.scl	101;	.line	27;	.endef
	.def	_put_nsprite_sx;	.val	.;	.scl	-1;	.endef
	.align 1
	.def	_put_zsprite_sx;	.val	_put_zsprite_sx;	.scl	3;	.type	041;	.endef
_put_zsprite_sx:
	mov.l	r8,@-r15
	mov.l	r9,@-r15
	mov.l	r10,@-r15
	mov.l	r11,@-r15
	mov.l	r12,@-r15
	mov.l	r13,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15 ! push
	add	#-40,r15
	mov	r15,r14
	mov	r4,r9
	.def	.bf;	.val	.;	.scl	101;	.line	139;	.endef
	.def	_spr;	.val	9;	.scl	17;	.tag	_SPRITE;	.size	80;	.type	030;	.endef
	.ln	2
	.def	_hvrev;	.val	38;	.scl	1;	.type	015;	.endef
	.def	_pmode;	.val	18;	.scl	1;	.type	015;	.endef
	.def	_color;	.val	22;	.scl	1;	.type	015;	.endef
	.def	_cgadr;	.val	13;	.scl	4;	.type	015;	.endef
	.def	_posi;	.val	0;	.scl	1;	.tag	_XyInt;	.dim	4;	.size	16;	.type	070;	.endef
	.def	_pattbl;	.val	8;	.scl	4;	.type	023;	.endef
	.def	_i;	.val	10;	.scl	4;	.type	03;	.endef
	.def	_xposi;	.val	26;	.scl	1;	.type	03;	.endef
	.def	_yposi;	.val	30;	.scl	1;	.type	03;	.endef
	.def	_hsize;	.val	3;	.scl	4;	.type	03;	.endef
	.def	_vsize;	.val	5;	.scl	4;	.type	03;	.endef
	.def	_hofst;	.val	2;	.scl	4;	.type	03;	.endef
	.def	_vofst;	.val	7;	.scl	4;	.type	03;	.endef
	.def	_hofst0;	.val	34;	.scl	1;	.type	03;	.endef
	.def	_hofst1;	.val	6;	.scl	4;	.type	03;	.endef
	.def	_vofst0;	.val	3;	.scl	4;	.type	03;	.endef
	.ln	8
	mov	r9,r1
	add	#1,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov.l	@(12,r9),r2
	shll2	r1
	mov	r1,r0
	mov.l	@(r0,r2),r8
	.ln	9
	mov	r9,r1
	add	#16,r1
	mov.w	@r1,r1
	mov	#26,r12
	add	r14,r12
	mov.w	r1,@r12
	.ln	10
	mov	r9,r1
	add	#20,r1
	mov.w	@r1,r1
	mov	#30,r0
	add	r14,r0
	mov.w	r1,@r0
	.ln	13
	mov	r9,r1
	add	#6,r1
	mov.w	@r1,r1
	mov	#18,r12
	add	r14,r12
	mov.w	r1,@r12
	.ln	14
	mov	r9,r1
	add	#8,r1
	mov.w	@r1,r1
	mov	#22,r0
	add	r14,r0
	mov.w	r1,@r0
	.ln	16
	mov.w	@r8+,r10
	tst	r10,r10 !t2c
	bf	LF104
	bra	L26	! 12 bit cond 
	or	r0,r0
LF104:
	mov.l	L34,r11
L28:
	.ln	17
	mov.w	@r8+,r13
	.ln	18
	mov.w	@r8+,r0
	mov	#38,r12
	add	r14,r12
	mov.w	r0,@r12
	.ln	19
	mov.w	@r8+,r3
	.ln	20
	mov.w	@r8+,r5
	.ln	21
	mov.w	@r8+,r2
	.ln	25
	mov	r9,r1
	add	#72,r1
	mov.w	@r1,r1
	muls	r1,r2
	sts	macl,r4
	cmp/pz	r4
	bt.s	L29
	mov.w	@r8+,r7
	mov.w	L35,r1
	add	r1,r4
L29:
	jsr	@r11
	mov	#34,r12
	add	r14,r12
	mov.w	r4,@r12
	.ln	26
	exts.w	r2,r2
	exts.w	r3,r1
	add	r1,r2
	mov	r9,r1
	add	#72,r1
	mov.w	@r1,r1
	exts.w	r1,r1
	mul.l	r1,r2
	sts	macl,r4
	cmp/pz	r4
	bt	L30
	mov.w	L35,r1
	add	r1,r4
L30:
	jsr	@r11
	mov	r9,r1
	mov	r4,r6
	.ln	27
	add	#70,r1
	mov.w	@r1,r1
	muls	r1,r7
	sts	macl,r4
	cmp/pz	r4
	bt	L31
	mov.w	L35,r1
	add	r1,r4
L31:
	jsr	@r11
	exts.w	r7,r2
	mov	r4,r3
	.ln	28
	exts.w	r5,r1
	add	r1,r2
	mov	r9,r1
	add	#70,r1
	mov.w	@r1,r1
	exts.w	r1,r1
	mul.l	r1,r2
	sts	macl,r4
	cmp/pz	r4
	bt.s	L32
	mov	#30,r12
	mov.w	L35,r1
	add	r1,r4
L32:
	jsr	@r11
	mov	#26,r0
	.ln	30
	add	r14,r0
	mov.w	@r0,r7
	mov	#34,r0
	add	r14,r0
	mov.w	@r0,r0
	add	r0,r7
	mov.w	r7,@r14
	.ln	31
	mov	r14,r1
	add	#2,r1
	add	r14,r12
	mov.w	@r12,r12
	add	r12,r3
	mov.w	r3,@r1
	.ln	32
	mov	r14,r1
	add	#4,r1
	mov	#26,r0
	add	r14,r0
	mov.w	@r0,r2
	add	r6,r2
	mov.w	r2,@r1
	.ln	33
	mov	r14,r1
	add	#6,r1
	mov.w	r3,@r1
	.ln	34
	mov	r14,r1
	add	#8,r1
	mov.w	r2,@r1
	.ln	35
	mov	r14,r1
	add	#10,r1
	mov	#30,r12
	add	r14,r12
	mov.w	@r12,r2
	add	r4,r2
	mov.w	r2,@r1
	.ln	36
	mov	r14,r1
	add	#12,r1
	mov.w	r7,@r1
	.ln	37
	mov	r14,r1
	add	#14,r1
	mov.w	r2,@r1
	.ln	38
	mov	#38,r0
	add	r14,r0
	mov.w	@r0,r0
	extu.w	r0,r5
	mov.l	L36,r1
	mov.l	r1,@-r15
	mov.l	r14,@-r15
	extu.w	r13,r1
	mov.l	r1,@-r15
	mov.l	L37,r1
	mov	#22,r12
	add	r14,r12
	mov.w	@r12,r12
	extu.w	r12,r7
	mov	#18,r0
	add	r14,r0
	mov.w	@r0,r0
	extu.w	r0,r6
	jsr	@r1
	mov	#0,r4
	.ln	16
	add	#-1,r10
	exts.w	r10,r1
	tst	r1,r1 ! t0
	bt.s	LF109
	add	#12,r15
	bra	L28	! 12 bit cond 
	or	r0,r0
LF109:
L26:
	.ln	41
	mov	r14,r15
	add	#40,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r13
	mov.l	@r15+,r12
	mov.l	@r15+,r11
	mov.l	@r15+,r10
	mov.l	@r15+,r9
	rts	
	mov.l	@r15+,r8
	.align 1
L35:
	.short	255
L38:
	.align 2
L34:
	.long	___ashiftrt_r4_8
L36:
	.long	65535
L37:
	.long	_SPR_2DistSpr
	.def	.ef;	.val	.;	.scl	101;	.line	41;	.endef
	.def	_put_zsprite_sx;	.val	.;	.scl	-1;	.endef
	.align 1
	.def	_put_rzsprite_sx;	.val	_put_rzsprite_sx;	.scl	3;	.type	041;	.endef
_put_rzsprite_sx:
	mov.l	r8,@-r15
	mov.l	r9,@-r15
	mov.l	r10,@-r15
	mov.l	r11,@-r15
	mov.l	r12,@-r15
	mov.l	r13,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15 ! push
	add	#-76,r15
	mov	r15,r14
	mov	r4,r8
	.def	.bf;	.val	.;	.scl	101;	.line	185;	.endef
	.def	_spr;	.val	8;	.scl	17;	.tag	_SPRITE;	.size	80;	.type	030;	.endef
	.ln	2
	.def	_hvrev;	.val	18;	.scl	1;	.type	015;	.endef
	.def	_pmode;	.val	22;	.scl	1;	.type	015;	.endef
	.def	_color;	.val	26;	.scl	1;	.type	015;	.endef
	.def	_cgadr;	.val	30;	.scl	1;	.type	015;	.endef
	.def	_pattbl;	.val	11;	.scl	4;	.type	023;	.endef
	.def	_i;	.val	62;	.scl	1;	.type	03;	.endef
	.def	_xposi;	.val	34;	.scl	1;	.type	03;	.endef
	.def	_yposi;	.val	38;	.scl	1;	.type	03;	.endef
	.def	_hsize;	.val	2;	.scl	4;	.type	03;	.endef
	.def	_vsize;	.val	7;	.scl	4;	.type	03;	.endef
	.def	_hofst;	.val	1;	.scl	4;	.type	03;	.endef
	.def	_vofst;	.val	3;	.scl	4;	.type	03;	.endef
	.def	_rx0c;	.val	8;	.scl	4;	.type	05;	.endef
	.def	_rx0s;	.val	68;	.scl	1;	.type	05;	.endef
	.def	_ry0c;	.val	72;	.scl	1;	.type	05;	.endef
	.def	_ry0s;	.val	6;	.scl	4;	.type	05;	.endef
	.def	_rx1c;	.val	64;	.scl	1;	.type	05;	.endef
	.def	_rx1s;	.val	9;	.scl	4;	.type	05;	.endef
	.def	_ry1c;	.val	5;	.scl	4;	.type	05;	.endef
	.def	_ry1s;	.val	3;	.scl	4;	.type	05;	.endef
	.def	_hcosz;	.val	40;	.scl	1;	.type	05;	.endef
	.def	_vcosz;	.val	44;	.scl	1;	.type	05;	.endef
	.def	_hsinz;	.val	48;	.scl	1;	.type	05;	.endef
	.def	_vsinz;	.val	52;	.scl	1;	.type	05;	.endef
	.def	_sinz;	.val	9;	.scl	4;	.type	05;	.endef
	.def	_cosz;	.val	64;	.scl	1;	.type	05;	.endef
	.def	_posi;	.val	0;	.scl	1;	.tag	_XyInt;	.dim	4;	.size	16;	.type	070;	.endef
	.ln	10
	mov	r8,r1
	add	#1,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov.l	@(12,r8),r2
	shll2	r1
	mov	r1,r0
	mov.l	@(r0,r2),r11
	.ln	11
	mov	r8,r1
	add	#16,r1
	mov.w	@r1,r1
	mov	#34,r10
	add	r14,r10
	mov.w	r1,@r10
	.ln	12
	mov	r8,r1
	add	#20,r1
	mov.w	@r1,r1
	mov	#38,r12
	add	r14,r12
	mov.w	r1,@r12
	.ln	13
	mov	r8,r10
	add	#68,r10
	mov	#72,r0
	add	r14,r0
	mov.l	r10,@r0
	mov	#72,r12
	add	r14,r12
	mov.l	@r12,r0
	mov.w	@r0,r4
	mov.l	L54,r10
	jsr	@r10
	exts.w	r4,r4
	mov	r0,r9
	exts.w	r9,r9
	.ln	14
	mov	#72,r12
	add	r14,r12
	mov.l	@r12,r0
	mov.w	@r0,r4
	mov.l	L55,r10
	jsr	@r10
	exts.w	r4,r4
	mov	#64,r12
	add	r14,r12
	mov.w	r0,@r12
	mov	#64,r0
	add	r14,r0
	mov.w	@r0,r10
	mov.l	r10,@r0
	.ln	15
	mov	r8,r1
	add	#72,r1
	mov.w	@r1,r1
	exts.w	r1,r1
	mul.l	r1,r9
	sts	macl,r12
	mov.l	r12,@(48,r14)
	.ln	16
	mov	#64,r0
	add	r14,r0
	mov.l	@r0,r0
	mul.l	r1,r0
	sts	macl,r10
	mov.l	r10,@(40,r14)
	.ln	17
	mov	r8,r1
	add	#70,r1
	mov.w	@r1,r1
	exts.w	r1,r1
	mul.l	r1,r9
	sts	macl,r12
	mov.l	r12,@(52,r14)
	.ln	18
	mov	#64,r0
	add	r14,r0
	mov.l	@r0,r0
	mul.l	r1,r0
	sts	macl,r10
	mov.l	r10,@(44,r14)
	.ln	20
	mov	r8,r1
	add	#6,r1
	mov.w	@r1,r1
	mov	#22,r12
	add	r14,r12
	mov.w	r1,@r12
	.ln	21
	add	#8,r8
	mov.w	@r8,r8
	mov	#26,r0
	add	r14,r0
	mov.w	r8,@r0
	.ln	23
	mov.w	@r11+,r12
	mov	#62,r10
	add	r14,r10
	mov.w	r12,@r10
	mov	#62,r0
	add	r14,r0
	mov.w	@r0,r10
	tst	r10,r10 !t2c
	bf.s	LF110
	mov	r14,r12
	bra	L41	! 12 bit cond 
	or	r0,r0
LF110:
	mov.l	L56,r13
	add	#2,r12
	mov.l	r12,@(56,r14)
L43:
	.ln	24
	mov.w	@r11+,r10
	mov	#30,r0
	add	r14,r0
	mov.w	r10,@r0
	.ln	25
	mov.w	@r11+,r0
	mov	#18,r12
	add	r14,r12
	mov.w	r0,@r12
	.ln	27
	mov.w	@r11+,r2
	.ln	28
	mov.w	@r11+,r7
	.ln	30
	mov.w	@r11+,r1
	.ln	31
	mov.w	@r11+,r3
	.ln	36
	exts.w	r1,r8
	mov.l	@(40,r14),r10
	mul.l	r10,r8
	sts	macl,r8
	.ln	37
	exts.w	r3,r0
	mov	#72,r12
	add	r14,r12
	mov.l	r0,@r12
	mov	#72,r10
	add	r14,r10
	mov.l	@r10,r10
	mov.l	@(44,r14),r12
	mul.l	r12,r10
	sts	macl,r10
	mov	#72,r0
	add	r14,r0
	mov.l	r10,@r0
	.ln	38
	exts.w	r1,r0
	mov	#68,r12
	add	r14,r12
	mov.l	r0,@r12
	mov	#68,r10
	add	r14,r10
	mov.l	@r10,r10
	mov.l	@(48,r14),r12
	mul.l	r12,r10
	sts	macl,r10
	mov	#68,r0
	add	r14,r0
	mov.l	r10,@r0
	.ln	39
	exts.w	r3,r6
	mov.l	@(52,r14),r12
	mul.l	r12,r6
	sts	macl,r6
	.ln	40
	exts.w	r1,r10
	mov	#64,r0
	add	r14,r0
	mov.l	r10,@r0
	exts.w	r2,r2
	mov	#64,r12
	add	r14,r12
	mov.l	@r12,r0
	add	r2,r0
	mov.l	r0,@r12
	mov	#64,r10
	add	r14,r10
	mov.l	@r10,r10
	mov.l	@(40,r14),r12
	mul.l	r12,r10
	sts	macl,r10
	mov	#64,r0
	add	r14,r0
	mov.l	r10,@r0
	.ln	41
	exts.w	r3,r5
	exts.w	r7,r7
	add	r7,r5
	mov.l	@(44,r14),r12
	mul.l	r12,r5
	sts	macl,r5
	.ln	42
	exts.w	r1,r9
	add	r2,r9
	mov.l	@(48,r14),r0
	mul.l	r0,r9
	sts	macl,r9
	.ln	43
	exts.w	r3,r3
	add	r7,r3
	mov.l	@(52,r14),r10
	mul.l	r10,r3
	.ln	46
	mov	r8,r4
	sub	r6,r4
	cmp/pz	r4
	bt.s	L44
	sts	macl,r3
	mov.l	L57,r1
	add	r1,r4
L44:
	jsr	@r13
	mov	#34,r12
	add	r14,r12
	mov.w	@r12,r1
	add	r4,r1
	mov.w	r1,@r14
	.ln	47
	mov	#68,r0
	add	r14,r0
	mov.l	@r0,r4
	mov	#72,r0
	add	r14,r0
	mov.l	@r0,r0
	add	r0,r4
	cmp/pz	r4
	bt.s	L45
	mov.l	@(56,r14),r12
	mov.l	L57,r1
	add	r1,r4
L45:
	jsr	@r13
	mov	#38,r10
	add	r14,r10
	mov.w	@r10,r1
	add	r4,r1
	mov.w	r1,@r12
	.ln	49
	mov	r14,r2
	mov	#64,r0
	add	r14,r0
	mov.l	@r0,r4
	sub	r6,r4
	cmp/pz	r4
	bt.s	L46
	add	#4,r2
	mov.l	L57,r1
	add	r1,r4
L46:
	jsr	@r13
	mov	#34,r10
	add	r14,r10
	mov.w	@r10,r1
	add	r4,r1
	mov.w	r1,@r2
	.ln	50
	mov	r14,r2
	mov	#72,r12
	add	r14,r12
	mov.l	@r12,r4
	add	r9,r4
	cmp/pz	r4
	bt.s	L47
	add	#6,r2
	mov.l	L57,r1
	add	r1,r4
L47:
	jsr	@r13
	mov	#38,r0
	add	r14,r0
	mov.w	@r0,r1
	add	r4,r1
	mov.w	r1,@r2
	.ln	52
	mov	r14,r2
	add	#8,r2
	bra	L53
	mov	#64,r10
L58:
	.align 2
L54:
	.long	_SMMI_Sinset8
L55:
	.long	_SMMI_Cosset8
L56:
	.long	___ashiftrt_r4_16
L57:
	.long	65535
L53:
	add	r14,r10
	mov.l	@r10,r4
	sub	r3,r4
	cmp/pz	r4
	bt	L48
	mov.l	L59,r1
	add	r1,r4
L48:
	jsr	@r13
	mov	#34,r12
	add	r14,r12
	mov.w	@r12,r1
	add	r4,r1
	mov.w	r1,@r2
	.ln	53
	mov	r14,r2
	mov	r9,r4
	add	r5,r4
	cmp/pz	r4
	bt.s	L49
	add	#10,r2
	mov.l	L59,r1
	add	r1,r4
L49:
	jsr	@r13
	mov	#38,r0
	add	r14,r0
	mov.w	@r0,r1
	add	r4,r1
	mov.w	r1,@r2
	.ln	55
	mov	r14,r2
	mov	r8,r4
	sub	r3,r4
	cmp/pz	r4
	bt.s	L50
	add	#12,r2
	mov.l	L59,r1
	add	r1,r4
L50:
	jsr	@r13
	mov	#34,r10
	add	r14,r10
	mov.w	@r10,r1
	add	r4,r1
	mov.w	r1,@r2
	.ln	56
	mov	r14,r2
	mov	#68,r12
	add	r14,r12
	mov.l	@r12,r4
	add	r5,r4
	cmp/pz	r4
	bt.s	L51
	add	#14,r2
	mov.l	L59,r1
	add	r1,r4
L51:
	jsr	@r13
	mov	#38,r0
	add	r14,r0
	mov.w	@r0,r1
	add	r4,r1
	mov.w	r1,@r2
	.ln	58
	mov	#18,r10
	add	r14,r10
	mov.w	@r10,r10
	extu.w	r10,r5
	mov.l	L59,r1
	mov.l	r1,@-r15
	mov.l	r14,@-r15
	mov	#30,r12
	add	r14,r12
	mov.w	@r12,r12
	extu.w	r12,r1
	mov.l	r1,@-r15
	mov.l	L60,r1
	mov	#26,r0
	add	r14,r0
	mov.w	@r0,r0
	extu.w	r0,r7
	mov	#22,r10
	add	r14,r10
	mov.w	@r10,r10
	extu.w	r10,r6
	jsr	@r1
	mov	#0,r4
	.ln	23
	mov	#62,r12
	add	r14,r12
	mov.w	@r12,r0
	add	#-1,r0
	mov.w	r0,@r12
	mov	#62,r10
	add	r14,r10
	mov.w	@r10,r1
	tst	r1,r1 ! t0
	bt.s	LF119
	add	#12,r15
	bra	L43	! 12 bit cond 
	or	r0,r0
LF119:
L41:
	.ln	61
	mov	r14,r15
	add	#76,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r13
	mov.l	@r15+,r12
	mov.l	@r15+,r11
	mov.l	@r15+,r10
	mov.l	@r15+,r9
	rts	
	mov.l	@r15+,r8
L61:
	.align 2
L59:
	.long	65535
L60:
	.long	_SPR_2DistSpr
	.def	.ef;	.val	.;	.scl	101;	.line	61;	.endef
	.def	_put_rzsprite_sx;	.val	.;	.scl	-1;	.endef
	.align 1
	.def	_put_line_sx;	.val	_put_line_sx;	.scl	3;	.type	041;	.endef
_put_line_sx:
	sts.l	pr,@-r15 ! push
	add	#-8,r15
	.def	.bf;	.val	.;	.scl	101;	.line	255;	.endef
	.def	_spr;	.val	4;	.scl	17;	.tag	_SPRITE;	.size	80;	.type	030;	.endef
	.ln	2
	.def	_pmode;	.val	5;	.scl	4;	.type	015;	.endef
	.def	_color;	.val	6;	.scl	4;	.type	015;	.endef
	.def	_posi;	.val	0;	.scl	1;	.tag	_XyInt;	.dim	2;	.size	8;	.type	070;	.endef
	.def	_lineposi;	.val	2;	.scl	4;	.type	023;	.endef
	.ln	7
	mov	r4,r2
	add	#28,r2
	.ln	8
	mov	r4,r1
	add	#6,r1
	mov.w	@r1,r5
	.ln	9
	mov	r4,r1
	add	#8,r1
	mov.w	@r1,r6
	.ln	10
	mov.w	@r2,r2
	mov.w	r2,@r15
	mov	r4,r2
	add	#30,r2
	.ln	11
	mov	r15,r1
	add	#2,r1
	mov.w	@r2,r2
	mov.w	r2,@r1
	.ln	12
	mov	r4,r2
	add	#34,r2
	.ln	13
	mov	r15,r1
	add	#4,r1
	mov.w	@r2,r2
	mov.w	r2,@r1
	mov	r4,r2
	add	#36,r2
	.ln	14
	mov	r15,r1
	add	#6,r1
	mov.w	@r2,r2
	mov.w	r2,@r1
	.ln	16
	extu.w	r6,r6
	extu.w	r5,r5
	mov.l	L63,r1
	mov.l	r1,@-r15
	mov.l	L64,r1
	mov	r14,r7
	add	#4,r7
	jsr	@r1
	mov	#0,r4
	.ln	18
	add	#4,r15
	add	#8,r15
	lds.l	@r15+,pr
	rts	
	nop
L65:
	.align 2
L63:
	.long	65535
L64:
	.long	_SPR_2Line
	.def	.ef;	.val	.;	.scl	101;	.line	18;	.endef
	.def	_put_line_sx;	.val	.;	.scl	-1;	.endef
	.align 1
	.def	_put_polygon_sx;	.val	_put_polygon_sx;	.scl	3;	.type	041;	.endef
_put_polygon_sx:
	mov.l	r8,@-r15
	mov.l	r9,@-r15
	mov.l	r10,@-r15
	mov.l	r11,@-r15
	mov.l	r12,@-r15
	mov.l	r13,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15 ! push
	add	#-16,r15
	.def	.bf;	.val	.;	.scl	101;	.line	280;	.endef
	.def	_spr;	.val	4;	.scl	17;	.tag	_SPRITE;	.size	80;	.type	030;	.endef
	.ln	2
	.def	_pmode;	.val	13;	.scl	4;	.type	015;	.endef
	.def	_color;	.val	6;	.scl	4;	.type	015;	.endef
	.def	_posi;	.val	0;	.scl	1;	.tag	_XyInt;	.dim	4;	.size	16;	.type	070;	.endef
	.def	_pattbl;	.val	8;	.scl	4;	.type	023;	.endef
	.def	_i;	.val	11;	.scl	4;	.type	03;	.endef
	.def	_xposi;	.val	10;	.scl	4;	.type	03;	.endef
	.def	_yposi;	.val	9;	.scl	4;	.type	03;	.endef
	.ln	9
	mov	r4,r1
	add	#1,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov.l	@(12,r4),r2
	shll2	r1
	mov	r1,r0
	mov.l	@(r0,r2),r8
	.ln	10
	mov	r4,r1
	add	#16,r1
	mov.w	@r1,r10
	.ln	11
	mov	r4,r1
	add	#20,r1
	mov.w	@r1,r9
	.ln	13
	add	#6,r4
	.ln	15
	mov.w	@r8+,r11
	tst	r11,r11 !t2c
	bt.s	L68
	mov.w	@r4,r13
	mov.l	L72,r12
	mov.l	L73,r14
L70:
	.ln	16
	mov.w	@r8+,r6
	.ln	18
	mov.w	@r8+,r1
	add	r10,r1
	mov.w	r1,@r15
	.ln	20
	mov	r15,r2
	add	#2,r2
	mov.w	@r8+,r1
	add	r9,r1
	mov.w	r1,@r2
	.ln	23
	mov	r15,r2
	add	#4,r2
	mov.w	@r8+,r1
	add	r10,r1
	mov.w	r1,@r2
	.ln	25
	mov	r15,r2
	add	#6,r2
	mov.w	@r8+,r1
	add	r9,r1
	mov.w	r1,@r2
	.ln	28
	mov	r15,r2
	add	#8,r2
	mov.w	@r8+,r1
	add	r10,r1
	mov.w	r1,@r2
	.ln	30
	mov	r15,r2
	add	#10,r2
	mov.w	@r8+,r1
	add	r9,r1
	mov.w	r1,@r2
	.ln	33
	mov	r15,r2
	add	#12,r2
	mov.w	@r8+,r1
	add	r10,r1
	mov.w	r1,@r2
	.ln	35
	mov	r15,r2
	add	#14,r2
	mov.w	@r8+,r1
	add	r9,r1
	mov.w	r1,@r2
	.ln	38
	extu.w	r6,r6
	mov.l	r12,@-r15
	mov	r14,r7
	add	#4,r7
	extu.w	r13,r5
	jsr	@r14
	mov	#0,r4
	.ln	15
	add	#-1,r11
	exts.w	r11,r1
	tst	r1,r1 ! t0
	bf.s	L70
	add	#4,r15
L68:
	.ln	41
	add	#16,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r13
	mov.l	@r15+,r12
	mov.l	@r15+,r11
	mov.l	@r15+,r10
	mov.l	@r15+,r9
	rts	
	mov.l	@r15+,r8
L74:
	.align 2
L72:
	.long	65535
L73:
	.long	_SPR_2Polygon
	.def	.ef;	.val	.;	.scl	101;	.line	41;	.endef
	.def	_put_polygon_sx;	.val	.;	.scl	-1;	.endef
	.align 1
	.def	_put_zpolygon_sx;	.val	_put_zpolygon_sx;	.scl	3;	.type	041;	.endef
_put_zpolygon_sx:
	mov.l	r8,@-r15
	mov.l	r9,@-r15
	mov.l	r10,@-r15
	mov.l	r11,@-r15
	mov.l	r12,@-r15
	mov.l	r13,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15 ! push
	add	#-24,r15
	mov	r4,r9
	.def	.bf;	.val	.;	.scl	101;	.line	328;	.endef
	.def	_spr;	.val	9;	.scl	17;	.tag	_SPRITE;	.size	80;	.type	030;	.endef
	.ln	2
	.def	_pmode;	.val	18;	.scl	1;	.type	015;	.endef
	.def	_color;	.val	6;	.scl	4;	.type	015;	.endef
	.def	_posi;	.val	0;	.scl	1;	.tag	_XyInt;	.dim	4;	.size	16;	.type	070;	.endef
	.def	_pattbl;	.val	8;	.scl	4;	.type	023;	.endef
	.def	_rx;	.val	3;	.scl	4;	.type	03;	.endef
	.def	_ry;	.val	2;	.scl	4;	.type	03;	.endef
	.def	_i;	.val	14;	.scl	4;	.type	03;	.endef
	.def	_xposi;	.val	13;	.scl	4;	.type	03;	.endef
	.def	_yposi;	.val	12;	.scl	4;	.type	03;	.endef
	.ln	9
	mov	r9,r1
	add	#1,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov.l	@(12,r9),r2
	shll2	r1
	mov	r1,r0
	mov.l	@(r0,r2),r8
	.ln	10
	mov	r9,r1
	add	#16,r1
	mov.w	@r1,r13
	.ln	11
	mov	r9,r1
	add	#20,r1
	mov.w	@r1,r12
	.ln	13
	mov	r9,r1
	add	#6,r1
	mov.w	@r1,r1
	mov	#18,r11
	add	r15,r11
	mov.w	r1,@r11
	.ln	15
	mov.w	@r8+,r14
	tst	r14,r14 !t2c
	bf.s	LF122
	mov	r9,r0
	bra	L77	! 12 bit cond 
	or	r0,r0
LF122:
	add	#72,r0
	mov.l	r0,@(20,r15)
	mov.l	L89,r10
L79:
	.ln	16
	mov.w	@r8+,r6
	.ln	18
	mov.w	@r8+,r3
	.ln	20
	mov.l	@(20,r15),r11
	mov.w	@r11,r1
	muls	r1,r3
	sts	macl,r4
	cmp/pz	r4
	bt.s	L80
	mov.w	@r8+,r2
	mov.w	L90,r1
	add	r1,r4
L80:
	jsr	@r10
	mov	r9,r1
	mov	r4,r3
	.ln	21
	add	#70,r1
	mov.w	@r1,r1
	muls	r1,r2
	sts	macl,r4
	cmp/pz	r4
	bt.s	L81
	mov	r15,r2
	mov.w	L90,r1
	add	r1,r4
L81:
	jsr	@r10
	mov	r13,r1
	.ln	22
	add	r3,r1
	mov.w	r1,@r15
	.ln	23
	add	#2,r2
	mov	r12,r1
	add	r4,r1
	mov.w	r1,@r2
	.ln	25
	mov.w	@r8+,r3
	.ln	27
	mov.l	@(20,r15),r0
	mov.w	@r0,r1
	muls	r1,r3
	sts	macl,r4
	cmp/pz	r4
	bt.s	L82
	mov.w	@r8+,r2
	mov.w	L90,r1
	add	r1,r4
L82:
	jsr	@r10
	mov	r9,r1
	mov	r4,r3
	.ln	28
	add	#70,r1
	mov.w	@r1,r1
	muls	r1,r2
	sts	macl,r4
	cmp/pz	r4
	bt	L83
	mov.w	L90,r1
	add	r1,r4
L83:
	jsr	@r10
	mov	r15,r2
	.ln	29
	add	#4,r2
	mov	r13,r1
	add	r3,r1
	mov.w	r1,@r2
	.ln	30
	mov	r15,r2
	add	#6,r2
	mov	r12,r1
	add	r4,r1
	mov.w	r1,@r2
	.ln	32
	mov.w	@r8+,r3
	.ln	34
	mov.l	@(20,r15),r11
	mov.w	@r11,r1
	muls	r1,r3
	sts	macl,r4
	cmp/pz	r4
	bt.s	L84
	mov.w	@r8+,r2
	mov.w	L90,r1
	add	r1,r4
L84:
	jsr	@r10
	mov	r9,r1
	mov	r4,r3
	.ln	35
	add	#70,r1
	mov.w	@r1,r1
	muls	r1,r2
	sts	macl,r4
	cmp/pz	r4
	bt	L85
	mov.w	L90,r1
	add	r1,r4
L85:
	jsr	@r10
	mov	r15,r2
	.ln	36
	add	#8,r2
	mov	r13,r1
	add	r3,r1
	mov.w	r1,@r2
	.ln	37
	mov	r15,r2
	add	#10,r2
	mov	r12,r1
	add	r4,r1
	mov.w	r1,@r2
	.ln	39
	mov.w	@r8+,r3
	.ln	41
	mov.l	@(20,r15),r0
	mov.w	@r0,r1
	muls	r1,r3
	sts	macl,r4
	cmp/pz	r4
	bt.s	L86
	mov.w	@r8+,r2
	mov.w	L90,r1
	add	r1,r4
L86:
	jsr	@r10
	mov	r9,r1
	mov	r4,r3
	.ln	42
	add	#70,r1
	mov.w	@r1,r1
	muls	r1,r2
	sts	macl,r4
	cmp/pz	r4
	bt.s	L87
	extu.w	r6,r6
	mov.w	L90,r1
	add	r1,r4
L87:
	jsr	@r10
	mov	r15,r2
	.ln	43
	add	#12,r2
	mov	r13,r1
	add	r3,r1
	mov.w	r1,@r2
	.ln	44
	mov	r15,r2
	add	#14,r2
	mov	r12,r1
	add	r4,r1
	mov.w	r1,@r2
	.ln	46
	mov.l	L91,r1
	mov.l	r1,@-r15
	mov.l	L92,r1
	mov	r14,r7
	add	#4,r7
	mov	#22,r11
	add	r14,r11
	mov.w	@r11,r11
	extu.w	r11,r5
	jsr	@r1
	mov	#0,r4
	.ln	15
	add	#-1,r14
	exts.w	r14,r1
	tst	r1,r1 ! t0
	bt.s	LF131
	add	#4,r15
	bra	L79	! 12 bit cond 
	or	r0,r0
LF131:
L77:
	.ln	49
	add	#24,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r13
	mov.l	@r15+,r12
	mov.l	@r15+,r11
	mov.l	@r15+,r10
	mov.l	@r15+,r9
	rts	
	mov.l	@r15+,r8
	.align 1
L90:
	.short	255
L93:
	.align 2
L89:
	.long	___ashiftrt_r4_8
L91:
	.long	65535
L92:
	.long	_SPR_2Polygon
	.def	.ef;	.val	.;	.scl	101;	.line	49;	.endef
	.def	_put_zpolygon_sx;	.val	.;	.scl	-1;	.endef
	.align 1
	.def	_put_rzpolygon_sx;	.val	_put_rzpolygon_sx;	.scl	3;	.type	041;	.endef
_put_rzpolygon_sx:
	mov.l	r8,@-r15
	mov.l	r9,@-r15
	mov.l	r10,@-r15
	mov.l	r11,@-r15
	mov.l	r12,@-r15
	mov.l	r13,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15 ! push
	add	#-28,r15
	mov	r4,r10
	.def	.bf;	.val	.;	.scl	101;	.line	384;	.endef
	.def	_spr;	.val	10;	.scl	17;	.tag	_SPRITE;	.size	80;	.type	030;	.endef
	.ln	2
	.def	_pmode;	.val	18;	.scl	1;	.type	015;	.endef
	.def	_color;	.val	6;	.scl	4;	.type	015;	.endef
	.def	_posi;	.val	0;	.scl	1;	.tag	_XyInt;	.dim	4;	.size	16;	.type	070;	.endef
	.def	_pattbl;	.val	9;	.scl	4;	.type	023;	.endef
	.def	_sinz;	.val	12;	.scl	4;	.type	03;	.endef
	.def	_cosz;	.val	11;	.scl	4;	.type	03;	.endef
	.def	_rx;	.val	3;	.scl	4;	.type	03;	.endef
	.def	_ry;	.val	7;	.scl	4;	.type	03;	.endef
	.def	_i;	.val	22;	.scl	1;	.type	03;	.endef
	.def	_xposi;	.val	14;	.scl	4;	.type	03;	.endef
	.def	_yposi;	.val	26;	.scl	1;	.type	03;	.endef
	.ln	9
	mov	r10,r1
	add	#1,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov.l	@(12,r10),r2
	shll2	r1
	mov	r1,r0
	mov.l	@(r0,r2),r9
	.ln	10
	mov	r10,r1
	add	#16,r1
	mov.w	@r1,r14
	.ln	11
	mov	r10,r1
	add	#20,r1
	mov.w	@r1,r1
	mov	#26,r13
	add	r15,r13
	mov.w	r1,@r13
	.ln	13
	mov	r10,r8
	add	#68,r8
	mov.w	@r8,r4
	mov.l	L117,r0
	jsr	@r0
	exts.w	r4,r4
	mov	r0,r12
	.ln	14
	mov.w	@r8,r4
	mov.l	L118,r13
	jsr	@r13
	exts.w	r4,r4
	mov	r0,r11
	.ln	16
	mov	r10,r1
	add	#6,r1
	mov.w	@r1,r1
	mov	#18,r0
	add	r15,r0
	mov.w	r1,@r0
	.ln	18
	mov.w	@r9+,r0
	mov	#22,r13
	add	r15,r13
	mov.w	r0,@r13
	mov	#22,r13
	add	r15,r13
	mov.w	@r13,r0
	tst	r0,r0 !t2c
	bf	LF132
	bra	L96	! 12 bit cond 
	or	r0,r0
LF132:
	mov.l	L119,r8
L98:
	.ln	19
	mov.w	@r9+,r6
	.ln	21
	mov.w	@r9+,r3
	.ln	23
	mov	r10,r1
	add	#72,r1
	mov.w	@r1,r1
	muls	r1,r3
	sts	macl,r4
	cmp/pz	r4
	bt.s	L99
	mov.w	@r9+,r7
	mov.w	L120,r1
	add	r1,r4
L99:
	jsr	@r8
	mov	r10,r1
	mov	r4,r3
	.ln	24
	add	#70,r1
	mov.w	@r1,r1
	muls	r1,r7
	sts	macl,r4
	cmp/pz	r4
	bt	L100
	mov.w	L120,r1
	add	r1,r4
L100:
	jsr	@r8
	muls	r11,r3
	mov	r4,r7
	.ln	25
	sts	macl,r2
	muls	r12,r7
	sts	macl,r1
	mov	r2,r4
	sub	r1,r4
	cmp/pz	r4
	bt.s	L101
	mov	r15,r5
	mov.w	L120,r1
	add	r1,r4
L101:
	jsr	@r8
	mov	r14,r1
	add	r4,r1
	mov.w	r1,@r15
	.ln	26
	muls	r12,r3
	sts	macl,r2
	muls	r11,r7
	sts	macl,r1
	mov	r2,r4
	add	r1,r4
	cmp/pz	r4
	bt.s	L102
	add	#2,r5
	mov.w	L120,r1
	add	r1,r4
L102:
	jsr	@r8
	mov	#26,r13
	add	r15,r13
	mov.w	@r13,r1
	add	r4,r1
	mov.w	r1,@r5
	.ln	28
	mov.w	@r9+,r3
	.ln	30
	mov	r10,r1
	add	#72,r1
	mov.w	@r1,r1
	muls	r1,r3
	sts	macl,r4
	cmp/pz	r4
	bt.s	L103
	mov.w	@r9+,r7
	mov.w	L120,r1
	add	r1,r4
L103:
	jsr	@r8
	mov	r10,r1
	mov	r4,r3
	.ln	31
	add	#70,r1
	mov.w	@r1,r1
	muls	r1,r7
	sts	macl,r4
	cmp/pz	r4
	bt.s	L104
	muls	r11,r3
	mov.w	L120,r1
	add	r1,r4
L104:
	jsr	@r8
	mov	r15,r5
	mov	r4,r7
	.ln	32
	sts	macl,r2
	muls	r12,r7
	sts	macl,r1
	mov	r2,r4
	sub	r1,r4
	cmp/pz	r4
	bt.s	L105
	add	#4,r5
	mov.w	L120,r1
	add	r1,r4
L105:
	jsr	@r8
	mov	r14,r1
	add	r4,r1
	mov.w	r1,@r5
	.ln	33
	mov	r15,r5
	muls	r12,r3
	sts	macl,r2
	muls	r11,r7
	sts	macl,r1
	mov	r2,r4
	add	r1,r4
	cmp/pz	r4
	bt.s	L106
	add	#6,r5
	mov.w	L120,r1
	add	r1,r4
L106:
	jsr	@r8
	mov	#26,r0
	add	r15,r0
	mov.w	@r0,r1
	add	r4,r1
	mov.w	r1,@r5
	.ln	35
	mov.w	@r9+,r3
	.ln	37
	mov	r10,r1
	add	#72,r1
	mov.w	@r1,r1
	muls	r1,r3
	sts	macl,r4
	cmp/pz	r4
	bt.s	L107
	mov.w	@r9+,r7
	mov.w	L120,r1
	add	r1,r4
L107:
	jsr	@r8
	mov	r10,r1
	mov	r4,r3
	.ln	38
	add	#70,r1
	mov.w	@r1,r1
	muls	r1,r7
	sts	macl,r4
	cmp/pz	r4
	bt.s	L108
	muls	r11,r3
	mov.w	L120,r1
	add	r1,r4
L108:
	jsr	@r8
	mov	r15,r5
	mov	r4,r7
	.ln	39
	add	#8,r5
	sts	macl,r2
	muls	r12,r7
	sts	macl,r1
	mov	r2,r4
	sub	r1,r4
	bra	L116
	cmp/pz	r4
	.align 1
L120:
	.short	255
L121:
	.align 2
L117:
	.long	_SMMI_Sinset8
L118:
	.long	_SMMI_Cosset8
L119:
	.long	___ashiftrt_r4_8
L116:
	bt.s	L109
	muls	r12,r3
	mov.w	L122,r1
	add	r1,r4
L109:
	jsr	@r8
	mov	r14,r1
	add	r4,r1
	mov.w	r1,@r5
	.ln	40
	mov	r15,r5
	sts	macl,r2
	muls	r11,r7
	sts	macl,r1
	mov	r2,r4
	add	r1,r4
	cmp/pz	r4
	bt.s	L110
	add	#10,r5
	mov.w	L122,r1
	add	r1,r4
L110:
	jsr	@r8
	mov	#26,r13
	add	r15,r13
	mov.w	@r13,r1
	add	r4,r1
	mov.w	r1,@r5
	.ln	42
	mov.w	@r9+,r3
	.ln	44
	mov	r10,r1
	add	#72,r1
	mov.w	@r1,r1
	muls	r1,r3
	sts	macl,r4
	cmp/pz	r4
	bt.s	L111
	mov.w	@r9+,r7
	mov.w	L122,r1
	add	r1,r4
L111:
	jsr	@r8
	mov	r10,r1
	mov	r4,r3
	.ln	45
	add	#70,r1
	mov.w	@r1,r1
	muls	r1,r7
	sts	macl,r4
	cmp/pz	r4
	bt.s	L112
	muls	r11,r3
	mov.w	L122,r1
	add	r1,r4
L112:
	jsr	@r8
	mov	r15,r5
	mov	r4,r7
	.ln	46
	sts	macl,r2
	muls	r12,r7
	sts	macl,r1
	mov	r2,r4
	sub	r1,r4
	cmp/pz	r4
	bt.s	L113
	add	#12,r5
	mov.w	L122,r1
	add	r1,r4
L113:
	jsr	@r8
	mov	r14,r1
	add	r4,r1
	mov.w	r1,@r5
	.ln	47
	mov	r15,r5
	muls	r12,r3
	sts	macl,r2
	muls	r11,r7
	sts	macl,r1
	mov	r2,r4
	add	r1,r4
	cmp/pz	r4
	bt.s	L114
	add	#14,r5
	mov.w	L122,r1
	add	r1,r4
L114:
	jsr	@r8
	mov	#26,r0
	add	r15,r0
	mov.w	@r0,r1
	add	r4,r1
	mov.w	r1,@r5
	.ln	49
	extu.w	r6,r6
	mov.l	L123,r1
	mov.l	r1,@-r15
	mov.l	L124,r1
	mov	r14,r7
	add	#4,r7
	mov	#22,r13
	add	r14,r13
	mov.w	@r13,r13
	extu.w	r13,r5
	jsr	@r1
	mov	#0,r4
	.ln	18
	add	#4,r15
	mov	#22,r0
	add	r15,r0
	mov.w	@r0,r13
	add	#-1,r13
	mov.w	r13,@r0
	mov	#22,r0
	add	r15,r0
	mov.w	@r0,r1
	tst	r1,r1 ! t0
	bt	LF149
	bra	L98	! 12 bit cond 
	or	r0,r0
LF149:
L96:
	.ln	52
	add	#28,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r13
	mov.l	@r15+,r12
	mov.l	@r15+,r11
	mov.l	@r15+,r10
	mov.l	@r15+,r9
	rts	
	mov.l	@r15+,r8
	.align 1
L122:
	.short	255
L125:
	.align 2
L123:
	.long	65535
L124:
	.long	_SPR_2Polygon
	.def	.ef;	.val	.;	.scl	101;	.line	52;	.endef
	.def	_put_rzpolygon_sx;	.val	.;	.scl	-1;	.endef
	.align 1
	.def	_put_rzpolyline_sx;	.val	_put_rzpolyline_sx;	.scl	3;	.type	041;	.endef
_put_rzpolyline_sx:
	mov.l	r8,@-r15
	mov.l	r9,@-r15
	mov.l	r10,@-r15
	mov.l	r11,@-r15
	mov.l	r12,@-r15
	mov.l	r13,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15 ! push
	add	#-28,r15
	mov	r4,r10
	.def	.bf;	.val	.;	.scl	101;	.line	442;	.endef
	.def	_spr;	.val	10;	.scl	17;	.tag	_SPRITE;	.size	80;	.type	030;	.endef
	.ln	2
	.def	_pmode;	.val	18;	.scl	1;	.type	015;	.endef
	.def	_color;	.val	6;	.scl	4;	.type	015;	.endef
	.def	_posi;	.val	0;	.scl	1;	.tag	_XyInt;	.dim	4;	.size	16;	.type	070;	.endef
	.def	_pattbl;	.val	9;	.scl	4;	.type	023;	.endef
	.def	_sinz;	.val	12;	.scl	4;	.type	03;	.endef
	.def	_cosz;	.val	11;	.scl	4;	.type	03;	.endef
	.def	_rx;	.val	3;	.scl	4;	.type	03;	.endef
	.def	_ry;	.val	7;	.scl	4;	.type	03;	.endef
	.def	_i;	.val	22;	.scl	1;	.type	03;	.endef
	.def	_xposi;	.val	14;	.scl	4;	.type	03;	.endef
	.def	_yposi;	.val	26;	.scl	1;	.type	03;	.endef
	.ln	9
	mov	r10,r1
	add	#1,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov.l	@(12,r10),r2
	shll2	r1
	mov	r1,r0
	mov.l	@(r0,r2),r9
	.ln	10
	mov	r10,r1
	add	#16,r1
	mov.w	@r1,r14
	.ln	11
	mov	r10,r1
	add	#20,r1
	mov.w	@r1,r1
	mov	#26,r13
	add	r15,r13
	mov.w	r1,@r13
	.ln	13
	mov	r10,r8
	add	#68,r8
	mov.w	@r8,r4
	mov.l	L149,r0
	jsr	@r0
	exts.w	r4,r4
	mov	r0,r12
	.ln	14
	mov.w	@r8,r4
	mov.l	L150,r13
	jsr	@r13
	exts.w	r4,r4
	mov	r0,r11
	.ln	17
	mov	r10,r1
	add	#6,r1
	mov.w	@r1,r1
	mov	#18,r0
	add	r15,r0
	mov.w	r1,@r0
	.ln	19
	mov.w	@r9+,r0
	mov	#22,r13
	add	r15,r13
	mov.w	r0,@r13
	mov	#22,r13
	add	r15,r13
	mov.w	@r13,r0
	tst	r0,r0 !t2c
	bf	LF150
	bra	L128	! 12 bit cond 
	or	r0,r0
LF150:
	mov.l	L151,r8
L130:
	.ln	20
	mov.w	@r9+,r6
	.ln	22
	mov.w	@r9+,r3
	.ln	24
	mov	r10,r1
	add	#72,r1
	mov.w	@r1,r1
	muls	r1,r3
	sts	macl,r4
	cmp/pz	r4
	bt.s	L131
	mov.w	@r9+,r7
	mov.w	L152,r1
	add	r1,r4
L131:
	jsr	@r8
	mov	r10,r1
	mov	r4,r3
	.ln	25
	add	#70,r1
	mov.w	@r1,r1
	muls	r1,r7
	sts	macl,r4
	cmp/pz	r4
	bt	L132
	mov.w	L152,r1
	add	r1,r4
L132:
	jsr	@r8
	muls	r11,r3
	mov	r4,r7
	.ln	26
	sts	macl,r2
	muls	r12,r7
	sts	macl,r1
	mov	r2,r4
	sub	r1,r4
	cmp/pz	r4
	bt.s	L133
	mov	r15,r5
	mov.w	L152,r1
	add	r1,r4
L133:
	jsr	@r8
	mov	r14,r1
	add	r4,r1
	mov.w	r1,@r15
	.ln	27
	muls	r12,r3
	sts	macl,r2
	muls	r11,r7
	sts	macl,r1
	mov	r2,r4
	add	r1,r4
	cmp/pz	r4
	bt.s	L134
	add	#2,r5
	mov.w	L152,r1
	add	r1,r4
L134:
	jsr	@r8
	mov	#26,r13
	add	r15,r13
	mov.w	@r13,r1
	add	r4,r1
	mov.w	r1,@r5
	.ln	29
	mov.w	@r9+,r3
	.ln	31
	mov	r10,r1
	add	#72,r1
	mov.w	@r1,r1
	muls	r1,r3
	sts	macl,r4
	cmp/pz	r4
	bt.s	L135
	mov.w	@r9+,r7
	mov.w	L152,r1
	add	r1,r4
L135:
	jsr	@r8
	mov	r10,r1
	mov	r4,r3
	.ln	32
	add	#70,r1
	mov.w	@r1,r1
	muls	r1,r7
	sts	macl,r4
	cmp/pz	r4
	bt.s	L136
	muls	r11,r3
	mov.w	L152,r1
	add	r1,r4
L136:
	jsr	@r8
	mov	r15,r5
	mov	r4,r7
	.ln	33
	sts	macl,r2
	muls	r12,r7
	sts	macl,r1
	mov	r2,r4
	sub	r1,r4
	cmp/pz	r4
	bt.s	L137
	add	#4,r5
	mov.w	L152,r1
	add	r1,r4
L137:
	jsr	@r8
	mov	r14,r1
	add	r4,r1
	mov.w	r1,@r5
	.ln	34
	mov	r15,r5
	muls	r12,r3
	sts	macl,r2
	muls	r11,r7
	sts	macl,r1
	mov	r2,r4
	add	r1,r4
	cmp/pz	r4
	bt.s	L138
	add	#6,r5
	mov.w	L152,r1
	add	r1,r4
L138:
	jsr	@r8
	mov	#26,r0
	add	r15,r0
	mov.w	@r0,r1
	add	r4,r1
	mov.w	r1,@r5
	.ln	36
	mov.w	@r9+,r3
	.ln	38
	mov	r10,r1
	add	#72,r1
	mov.w	@r1,r1
	muls	r1,r3
	sts	macl,r4
	cmp/pz	r4
	bt.s	L139
	mov.w	@r9+,r7
	mov.w	L152,r1
	add	r1,r4
L139:
	jsr	@r8
	mov	r10,r1
	mov	r4,r3
	.ln	39
	add	#70,r1
	mov.w	@r1,r1
	muls	r1,r7
	sts	macl,r4
	cmp/pz	r4
	bt.s	L140
	muls	r11,r3
	mov.w	L152,r1
	add	r1,r4
L140:
	jsr	@r8
	mov	r15,r5
	mov	r4,r7
	.ln	40
	add	#8,r5
	sts	macl,r2
	muls	r12,r7
	sts	macl,r1
	mov	r2,r4
	sub	r1,r4
	bra	L148
	cmp/pz	r4
	.align 1
L152:
	.short	255
L153:
	.align 2
L149:
	.long	_SMMI_Sinset8
L150:
	.long	_SMMI_Cosset8
L151:
	.long	___ashiftrt_r4_8
L148:
	bt.s	L141
	muls	r12,r3
	mov.w	L154,r1
	add	r1,r4
L141:
	jsr	@r8
	mov	r14,r1
	add	r4,r1
	mov.w	r1,@r5
	.ln	41
	mov	r15,r5
	sts	macl,r2
	muls	r11,r7
	sts	macl,r1
	mov	r2,r4
	add	r1,r4
	cmp/pz	r4
	bt.s	L142
	add	#10,r5
	mov.w	L154,r1
	add	r1,r4
L142:
	jsr	@r8
	mov	#26,r13
	add	r15,r13
	mov.w	@r13,r1
	add	r4,r1
	mov.w	r1,@r5
	.ln	43
	mov.w	@r9+,r3
	.ln	45
	mov	r10,r1
	add	#72,r1
	mov.w	@r1,r1
	muls	r1,r3
	sts	macl,r4
	cmp/pz	r4
	bt.s	L143
	mov.w	@r9+,r7
	mov.w	L154,r1
	add	r1,r4
L143:
	jsr	@r8
	mov	r10,r1
	mov	r4,r3
	.ln	46
	add	#70,r1
	mov.w	@r1,r1
	muls	r1,r7
	sts	macl,r4
	cmp/pz	r4
	bt.s	L144
	muls	r11,r3
	mov.w	L154,r1
	add	r1,r4
L144:
	jsr	@r8
	mov	r15,r5
	mov	r4,r7
	.ln	47
	sts	macl,r2
	muls	r12,r7
	sts	macl,r1
	mov	r2,r4
	sub	r1,r4
	cmp/pz	r4
	bt.s	L145
	add	#12,r5
	mov.w	L154,r1
	add	r1,r4
L145:
	jsr	@r8
	mov	r14,r1
	add	r4,r1
	mov.w	r1,@r5
	.ln	48
	mov	r15,r5
	muls	r12,r3
	sts	macl,r2
	muls	r11,r7
	sts	macl,r1
	mov	r2,r4
	add	r1,r4
	cmp/pz	r4
	bt.s	L146
	add	#14,r5
	mov.w	L154,r1
	add	r1,r4
L146:
	jsr	@r8
	mov	#26,r0
	add	r15,r0
	mov.w	@r0,r1
	add	r4,r1
	mov.w	r1,@r5
	.ln	50
	extu.w	r6,r6
	mov.l	L155,r1
	mov.l	r1,@-r15
	mov.l	L156,r1
	mov	r14,r7
	add	#4,r7
	mov	#22,r13
	add	r14,r13
	mov.w	@r13,r13
	extu.w	r13,r5
	jsr	@r1
	mov	#0,r4
	.ln	19
	add	#4,r15
	mov	#22,r0
	add	r15,r0
	mov.w	@r0,r13
	add	#-1,r13
	mov.w	r13,@r0
	mov	#22,r0
	add	r15,r0
	mov.w	@r0,r1
	tst	r1,r1 ! t0
	bt	LF167
	bra	L130	! 12 bit cond 
	or	r0,r0
LF167:
L128:
	.ln	53
	add	#28,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r13
	mov.l	@r15+,r12
	mov.l	@r15+,r11
	mov.l	@r15+,r10
	mov.l	@r15+,r9
	rts	
	mov.l	@r15+,r8
	.align 1
L154:
	.short	255
L157:
	.align 2
L155:
	.long	65535
L156:
	.long	_SPR_2PolyLine
	.def	.ef;	.val	.;	.scl	101;	.line	53;	.endef
	.def	_put_rzpolyline_sx;	.val	.;	.scl	-1;	.endef
	.align 1
	.def	_put_4psprite_sx;	.val	_put_4psprite_sx;	.scl	3;	.type	041;	.endef
_put_4psprite_sx:
	mov.l	r8,@-r15
	mov.l	r9,@-r15
	mov.l	r10,@-r15
	mov.l	r11,@-r15
	mov.l	r12,@-r15
	mov.l	r13,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15 ! push
	add	#-16,r15
	mov	r15,r14
	mov	r4,r10
	.def	.bf;	.val	.;	.scl	101;	.line	501;	.endef
	.def	_spr;	.val	10;	.scl	17;	.tag	_SPRITE;	.size	80;	.type	030;	.endef
	.ln	2
	.def	_hvrev;	.val	5;	.scl	4;	.type	015;	.endef
	.def	_pmode;	.val	13;	.scl	4;	.type	015;	.endef
	.def	_color;	.val	12;	.scl	4;	.type	015;	.endef
	.def	_cgadr;	.val	3;	.scl	4;	.type	015;	.endef
	.def	_posi;	.val	0;	.scl	1;	.tag	_XyInt;	.dim	4;	.size	16;	.type	070;	.endef
	.def	_pattbl;	.val	9;	.scl	4;	.type	023;	.endef
	.def	_i;	.val	8;	.scl	4;	.type	03;	.endef
	.ln	8
	mov	r10,r1
	add	#1,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov.l	@(12,r10),r2
	shll2	r1
	mov	r1,r0
	mov.l	@(r0,r2),r9
	.ln	11
	mov	r10,r8
	add	#68,r8
	mov.w	@r8,r4
	mov.l	L164,r0
	jsr	@r0
	exts.w	r4,r4
	.ln	12
	mov.w	@r8,r4
	mov.l	L165,r0
	jsr	@r0
	exts.w	r4,r4
	.ln	14
	mov	r10,r1
	add	#6,r1
	mov.w	@r1,r13
	.ln	15
	mov	r10,r1
	add	#8,r1
	.ln	17
	mov.w	@r9+,r8
	tst	r8,r8 !t2c
	bf.s	LF168
	mov.w	@r1,r12
	bra	L160	! 12 bit cond 
	or	r0,r0
LF168:
	mov.l	L166,r11
L162:
	.ln	18
	mov.w	@r9+,r3
	.ln	19
	mov.w	@r9+,r5
	.ln	23
	add	#8,r9
	.ln	25
	mov	r10,r1
	add	#28,r1
	mov.w	@r1,r1
	mov.w	r1,@r14
	.ln	26
	mov	r14,r2
	add	#2,r2
	mov	r10,r1
	add	#30,r1
	mov.w	@r1,r1
	mov.w	r1,@r2
	.ln	28
	mov	r14,r2
	add	#4,r2
	mov	r10,r1
	add	#34,r1
	mov.w	@r1,r1
	mov.w	r1,@r2
	.ln	29
	mov	r14,r2
	add	#6,r2
	mov	r10,r1
	add	#36,r1
	mov.w	@r1,r1
	mov.w	r1,@r2
	.ln	31
	mov	r14,r2
	add	#8,r2
	mov	r10,r1
	add	#40,r1
	mov.w	@r1,r1
	mov.w	r1,@r2
	.ln	32
	mov	r14,r2
	add	#10,r2
	mov	r10,r1
	add	#42,r1
	mov.w	@r1,r1
	mov.w	r1,@r2
	.ln	34
	mov	r14,r2
	add	#12,r2
	mov	r10,r1
	add	#46,r1
	mov.w	@r1,r1
	mov.w	r1,@r2
	.ln	35
	mov	r14,r2
	add	#14,r2
	mov	r10,r1
	add	#48,r1
	mov.w	@r1,r1
	mov.w	r1,@r2
	.ln	37
	extu.w	r5,r5
	mov.l	r11,@-r15
	mov.l	r14,@-r15
	extu.w	r3,r3
	mov.l	r3,@-r15
	extu.w	r12,r7
	extu.w	r13,r6
	mov.l	L167,r0
	jsr	@r0
	mov	#0,r4
	.ln	17
	add	#-1,r8
	exts.w	r8,r1
	tst	r1,r1 ! t0
	bt.s	LF169
	add	#12,r15
	bra	L162	! 12 bit cond 
	or	r0,r0
LF169:
L160:
	.ln	41
	mov	r14,r15
	add	#16,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r13
	mov.l	@r15+,r12
	mov.l	@r15+,r11
	mov.l	@r15+,r10
	mov.l	@r15+,r9
	rts	
	mov.l	@r15+,r8
L168:
	.align 2
L164:
	.long	_SMMI_Sinset8
L165:
	.long	_SMMI_Cosset8
L166:
	.long	65535
L167:
	.long	_SPR_2DistSpr
	.def	.ef;	.val	.;	.scl	101;	.line	41;	.endef
	.def	_put_4psprite_sx;	.val	.;	.scl	-1;	.endef
	.align 1
	.def	_SMV1_SprCmdStart;	.val	_SMV1_SprCmdStart;	.scl	2;	.type	041;	.endef
	.global	_SMV1_SprCmdStart
_SMV1_SprCmdStart:
	mov.l	r8,@-r15
	mov.l	r9,@-r15
	mov.l	r10,@-r15
	sts.l	pr,@-r15 ! push
	add	#-20,r15
	.def	.bf;	.val	.;	.scl	101;	.line	553;	.endef
	.ln	2
	.def	_reg_xy;	.val	0;	.scl	1;	.tag	_XyInt;	.size	4;	.type	010;	.endef
	.def	_pos_xy;	.val	4;	.scl	1;	.tag	_XyInt;	.dim	4;	.size	16;	.type	070;	.endef
	.ln	6
	mov.l	L170,r1
	jsr	@r1
	mov	#1,r4
	.ln	8
	mov.w	L171,r9
	mov.w	r9,@r15
	.ln	9
	mov	r15,r8
	add	#2,r8
	mov.w	L172,r10
	mov.w	r10,@r8
	.ln	10
	mov.l	L173,r1
	mov	r15,r5
	jsr	@r1
	mov	#0,r4
	.ln	13
	mov	#0,r3
	mov.w	r3,@r15
	.ln	14
	mov.w	r3,@r8
	.ln	15
	mov.l	L174,r1
	mov	r15,r5
	jsr	@r1
	mov	#0,r4
	.ln	18
	mov.w	L175,r5
	.ln	20
	mov.l	L176,r2
	.ln	21
	mov	r15,r7
	add	#4,r7
	mov	#0,r3
	mov.w	r3,@r7
	.ln	22
	mov	r15,r1
	add	#6,r1
	mov.w	r3,@r1
	.ln	23
	mov	r7,r1
	add	#4,r1
	mov.w	r9,@r1
	.ln	24
	mov	r15,r1
	add	#10,r1
	mov.w	r3,@r1
	.ln	25
	mov	r15,r1
	add	#12,r1
	mov.w	r9,@r1
	.ln	26
	mov	r15,r1
	add	#14,r1
	mov.w	r10,@r1
	.ln	27
	mov	r15,r1
	add	#16,r1
	mov.w	r3,@r1
	.ln	28
	mov	r15,r1
	add	#18,r1
	mov.w	r10,@r1
	.ln	29
	mov.l	r2,@-r15
	mov.l	L177,r1
	mov	#0,r6
	jsr	@r1
	mov	#0,r4
	.ln	34
	add	#4,r15
	add	#20,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r10
	mov.l	@r15+,r9
	rts	
	mov.l	@r15+,r8
	.align 1
L171:
	.short	319
L172:
	.short	223
L175:
	.short	192
L178:
	.align 2
L170:
	.long	_SPR_2OpenCommand
L173:
	.long	_SPR_2SysClip
L174:
	.long	_SPR_2LocalCoord
L176:
	.long	65535
L177:
	.long	_SPR_2Polygon
	.def	.ef;	.val	.;	.scl	101;	.line	34;	.endef
	.def	_SMV1_SprCmdStart;	.val	.;	.scl	-1;	.endef
	.align 1
	.def	_SMV1_SprCmdEnd;	.val	_SMV1_SprCmdEnd;	.scl	2;	.type	041;	.endef
	.global	_SMV1_SprCmdEnd
_SMV1_SprCmdEnd:
	sts.l	pr,@-r15 ! push
	.def	.bf;	.val	.;	.scl	101;	.line	593;	.endef
	.ln	2
	mov.l	L180,r1
	jsr	@r1
	nop
	.ln	3
	lds.l	@r15+,pr
	rts	
	nop
L181:
	.align 2
L180:
	.long	_SPR_2CloseCommand
	.def	.ef;	.val	.;	.scl	101;	.line	3;	.endef
	.def	_SMV1_SprCmdEnd;	.val	.;	.scl	-1;	.endef
	.align 1
	.def	_SMV1_IniSprite;	.val	_SMV1_IniSprite;	.scl	2;	.type	041;	.endef
	.global	_SMV1_IniSprite
_SMV1_IniSprite:
	mov.l	r8,@-r15
	mov.l	r9,@-r15
	mov.l	r10,@-r15
	sts.l	pr,@-r15 ! push
	.def	.bf;	.val	.;	.scl	101;	.line	602;	.endef
	.ln	2
	.def	_reg_xy;	.val	0;	.scl	1;	.tag	_XyInt;	.size	4;	.type	010;	.endef
	.def	_pos_xy;	.val	4;	.scl	1;	.tag	_XyInt;	.dim	4;	.size	16;	.type	070;	.endef
	.ln	6
	mov.l	L183,r4
	mov.l	L184,r1
	jsr	@r1
	add	#-20,r15
	.ln	7
	mov.l	L185,r1
	mov	#0,r6
	mov	#0,r5
	jsr	@r1
	mov	#0,r4
	.ln	8
	mov.l	L186,r1
	jsr	@r1
	mov	#0,r4
	.ln	9
	mov.l	L187,r1
	jsr	@r1
	mov	#1,r4
	.ln	11
	mov.w	L188,r9
	mov.w	r9,@r15
	.ln	12
	mov	r15,r8
	add	#2,r8
	mov.w	L189,r10
	mov.w	r10,@r8
	.ln	13
	mov.l	L190,r1
	mov	r15,r5
	jsr	@r1
	mov	#0,r4
	.ln	15
	mov	#0,r3
	mov.w	r3,@r15
	.ln	16
	mov.w	r3,@r8
	.ln	17
	mov.l	L191,r1
	mov	r15,r5
	jsr	@r1
	mov	#0,r4
	.ln	21
	mov.w	L192,r5
	.ln	23
	mov.l	L193,r2
	.ln	24
	mov	r15,r7
	add	#4,r7
	mov	#0,r3
	mov.w	r3,@r7
	.ln	25
	mov	r15,r1
	add	#6,r1
	mov.w	r3,@r1
	.ln	26
	mov	r7,r1
	add	#4,r1
	mov.w	r9,@r1
	.ln	27
	mov	r15,r1
	add	#10,r1
	mov.w	r3,@r1
	.ln	28
	mov	r15,r1
	add	#12,r1
	mov.w	r9,@r1
	.ln	29
	mov	r15,r1
	add	#14,r1
	mov.w	r10,@r1
	.ln	30
	mov	r15,r1
	add	#16,r1
	mov.w	r3,@r1
	.ln	31
	mov	r15,r1
	add	#18,r1
	mov.w	r10,@r1
	.ln	32
	mov.l	r2,@-r15
	mov.l	L194,r1
	mov	#0,r6
	jsr	@r1
	mov	#0,r4
	.ln	36
	add	#4,r15
	add	#20,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r10
	mov.l	@r15+,r9
	rts	
	mov.l	@r15+,r8
	.align 1
L188:
	.short	319
L189:
	.short	223
L192:
	.short	192
L195:
	.align 2
L183:
	.long	_SMV1_sprwk
L184:
	.long	_SPR_2Initial
L185:
	.long	_SPR_2SetTvMode
L186:
	.long	_SPR_2FrameEraseData
L187:
	.long	_SPR_2OpenCommand
L190:
	.long	_SPR_2SysClip
L191:
	.long	_SPR_2LocalCoord
L193:
	.long	65535
L194:
	.long	_SPR_2Polygon
	.def	.ef;	.val	.;	.scl	101;	.line	36;	.endef
	.def	_SMV1_IniSprite;	.val	.;	.scl	-1;	.endef
	.def	_sp2CharTbl;	.val	_sp2CharTbl;	.scl	3;	.tag	_SprCharTbl;	.dim	512;	.size	5120;	.type	070;	.endef
	.lcomm _sp2CharTbl,5120
	.def	_sp2CmdChain;	.val	_sp2CmdChain;	.scl	3;	.tag	_SprCmdChain;	.dim	1024;	.size	4096;	.type	070;	.endef
	.lcomm _sp2CmdChain,4096
	.def	_sp2DrawPrtyBlk;	.val	_sp2DrawPrtyBlk;	.scl	3;	.tag	_SprDrawPrtyBlk;	.dim	512;	.size	2048;	.type	070;	.endef
	.lcomm _sp2DrawPrtyBlk,2048
	.def	_SMV1_SpritePutModeTbl;	.val	_SMV1_SpritePutModeTbl;	.scl	2;	.dim	9;	.size	36;	.type	01161;	.endef
	.def	_SMV1_sprwk;	.val	_SMV1_sprwk;	.scl	3;	.tag	_Spr2WorkArea;	.size	24;	.type	010;	.endef
