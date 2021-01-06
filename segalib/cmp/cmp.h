/*******************************************************************
*
*                �@�@�@ �T�^�[���p�f�[�^���k
*
*                      Copyright(c) 1994 SEGA
*
*   Comment: �c�[���E���C�u�������p�w�b�_
*   File   : CMP.H
*   Date   : 1994-01-18
*   Author : Y.T
*
*******************************************************************/

/*******************************************************************
	  �萔�}�N��
*******************************************************************/

#define MAXFILENAME		(65)		/* max file name 		*/
#define CMP_BUFSIZE 	(64000U)

/* ���k�t�@�C���w�b�_ */
/* �A���S���Y�� */
#define		CMP_ALGO_MASK	(0xF000)	/* �A���S���Y���L�q�r�b�g15..12	*/
#define		CMP_ALGO_RUNLEN	(0x0000)	/* ���������O�X 	*/
#define		CMP_ALGO_SLIDE	(0x1000)	/* �X���C�h���� 	*/
#define		CMP_ALGO_HUFF	(0x2000)	/* �n�t�}�� 		*/
/* �����P�ʁi�P�L���̕��j */
#define		CMP_UNIT_MASK 	(0x0C00)	/* �����P�ʋL�q�r�b�g11..10 	*/
#define		CMP_UNIT_BYTE 	(0x0000)	/* �P�o�C�g 	*/
#define		CMP_UNIT_WORD 	(0x0400)	/* �Q�o�C�g 	*/
#define		CMP_UNIT_DWORD 	(0x0C00)	/* �S�o�C�g 	*/
/* ���T�C�Y�� */
#define		CMP_ORGW_MASK	(0x0008)	/* ���T�C�Y���L�q�r�b�g3	 	*/
#define		CMP_ORGW_WORD 	(0x0000)	/* �Q�o�C�g 	*/
#define		CMP_ORGW_DWORD	(0x0008)	/* �S�o�C�g 	*/
/* �g�������� */
#define		CMP_EXTC_MASK	(0x0001)	/* �g��������L�q�r�b�g0	 	*/
#define		CMP_EXTC_NOUSE 	(0x0000)	/* �g�p���Ȃ� 	*/
#define		CMP_EXTC_USE	(0x0001)	/* �g�p���� 	*/

/* ���������O�X���� */
/* ���͌n���� */
#define		CMP_RST_RUN1	(1)			/* �����^�������P���			*/
#define		CMP_RST_RUN2	(2)			/* �������						*/
#define		CMP_RST_NRUN	(3)			/* �s��v�n����				*/
#define		CMP_RST_NRUNR	(4)			/* �s��v�n��^�������[�`���	*/
/* �\���\�ȁA�ő��v�n�񒷁B */
#define		CMP_MAX_EQRUN_BYTE		(0x0000007fL + 2L)
#define		CMP_MAX_EQRUN_WORD		(0x00007fffL + 2L)
#define		CMP_MAX_EQRUN_DWORD		(0x7fffffffL)
/* �\���\�ȁA�ő�s��v�n�񒷁B */
#define		CMP_MAX_NEQRUN_BYTE		(0x00000080L)
#define		CMP_MAX_NEQRUN_WORD		(0x00008000L)
#define		CMP_MAX_NEQRUN_DWORD	(0x7fffffffL)
/* �P�L���i�����j�̃o�C�g���B */
#define		CMP_BYTEBYTE		(1)
#define		CMP_BYTEWORD		(2)
#define		CMP_BYTEDWORD		(4)
/* �o�E���_���������B */
#define		CMP_BOUNBYTE		(CMP_BYTEBYTE - 1)
#define		CMP_BOUNWORD		(CMP_BYTEWORD - 1)
#define		CMP_BOUNDWORD		(CMP_BYTEDWORD - 1)

/* �X���C�h���� */
/* �P��̓ǂݍ��݃T�C�Y�i4��Ń����O�o�b�t�@�S�̕��j */
#define		CMP_SD_READ_SIZE		(4096)
/* ���͗p�����O�o�b�t�@�T�C�Y */
#define		CMP_SD_RINGBUF_SIZE		(4*CMP_SD_READ_SIZE)
/* �V�t�g�T�C�Y�i memmove ����f�[�^�ʂ̂��ƁA�V�t�g���鋗���ł͂Ȃ��j  */
#define		CMP_SD_SHIFT_SIZE		(CMP_SD_RINGBUF_SIZE - CMP_SD_READ_SIZE)
/* �ǉ��ǂݍ��݈ʒu�i�o�b�t�@�擪����I�t�Z�b�g�j */
#define		CMP_SD_READ_OFFSET		(CMP_SD_SHIFT_SIZE)
/* �V�t�g�����g���K�ʒu�i�o�b�t�@�擪����I�t�Z�b�g�j  */
#define		CMP_SD_TRIGGER_OFFSET	(CMP_SD_RINGBUF_SIZE - CMP_SD_READ_SIZE)


/*******************************************************************
	  �����}�N��
*******************************************************************/
#define dfprintf 	DFP
#define DFP(a) 		fprintf a
#define MINCUSTM(a,outpend,outp)\
		MIN(a, (Uint8 *)outpend + 1 - *(Uint8 **)outp)

/* void mload_word(Uint16 _val, void *_from) */
#define mload_word(valtype, _val, fromtype, _from)\
{\
	char 	*from;\
	Uint16 	val;\
\
	from = (char *)_from;\
	val = *from++;\
	val = (val << 8) | (*from++ & 0x00ff);\
	_val = (valtype)val;\
	_from = (fromtype)from;\
}

/* void mload_sword(Sint16 _val, void *_from) */
#define mload_sword(valtype, _val, fromtype, _from)\
{\
	char 	*from;\
	Sint16 	val;\
\
	from = (char *)_from;\
	val = *from++;\
	val = (val << 8) | (*from++ & 0x00ff);\
	_val = (valtype)val;\
	_from = (fromtype)from;\
}
/* void mload_dword(Uint32 _val, void *_from) */
#define mload_dword(valtype, _val, fromtype, _from)\
{\
	char 	*from;\
	Uint32 	val;\
\
	from = (char *)_from;\
	val = *from++;\
	val = (val << 8) | (*from++ & 0x000000ff);\
	val = (val << 8) | (*from++ & 0x000000ff);\
	val = (val << 8) | (*from++ & 0x000000ff);\
	_val = (valtype)val;\
	_from = (fromtype)from;\
}
/*******************************************************************
	  �v���g�^�C�v�錾
*******************************************************************/

extern char buf_in[];
extern char buf_out[];
extern Uint8 sd_ringbuf[CMP_SD_RINGBUF_SIZE];
extern Uint16 bittbl[16];
#ifndef	COMPILER_CC
extern int CMP_CodeRunlen(char *name_in, int unit, int opti, char *name_out);
extern int CMP_CodeRunlenByte(char *name_in, char *name_out, long *cmpfsize);
extern int CMP_CodeRunlenWord(char *name_in, char *name_out, long *cmpfsize);
extern int CMP_CodeRunlenDword(char *name_in, char *name_out, long *cmpfsize);
extern int CMP_DecSdBlkStd(void **inp, void **outp, void *outpend,int datasum);
extern int CMP_DecSdBlkExt(void **inp, void **outp, void *outpend,int datasum);
/* cmptool.c */
extern char name_tmp[MAXFILENAME];	/* �e���|�����t�@�C���� */
extern int update(char *objname, char *tmpname);
extern int fput_word(Uint16 w, FILE *fp);
extern int fput_dword(Uint32 w, FILE *fp);
extern int fget_word(Uint16 *w, FILE *fp);
extern int fget_dword(Uint32 *w, FILE *fp);
extern void swap_word(Uint16 *w);
extern void swap_dword(Uint32 *a);
extern void print_rate(char *name_in, long orgfsize, char *name_out, long cmpfsize);
/* cmpltool.c */
extern void *memset_word(void *to, Uint16 a, size_t n);
extern void *memset_dword(void *to, Uint32 a, size_t n);
extern void *memmove_byte(void *_to, void *_from, size_t n);
/*
extern void mload_word(Uint16 *_val, void **_from);
extern void mload_dword(Uint32 *_val, void **_from);
*/
#else
extern int CMP_CodeRunlen();
extern int CMP_CodeRunlenByte();
extern int CMP_CodeRunlenWord();
extern int CMP_CodeRunlenDword();
extern int CMP_DecSdBlkStd();
extern int CMP_DecSdBlkExt();
/* cmptool.c */
extern char name_tmp[MAXFILENAME];	/* �e���|�����t�@�C���� */
extern int update();
extern int fput_word();
extern int fput_dword();
extern int fget_word();
extern int fget_dword();
extern void swap_word();
extern void swap_dword();
extern void print_rate();
/* cmpltool.c */
extern void *memset_word();
extern void *memset_dword();
extern void *memmove_byte();
/*
extern void mload_word();
extern void mload_dword();
*/
#endif
