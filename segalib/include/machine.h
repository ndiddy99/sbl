
#define sleep() 	{ asm volatile ("sleep"::); }
#define tas(addr) 	{ asm volatile ("tas.b %0" : : "m" (addr)); }
#define trap(n) 	{ asm volatile ("trapa  %0" : : "i" (n));}
#define set_cr(cr)   	{ asm volatile ("ldc	%0,sr" :  : "r" (cr)); }
#define get_cr()   	({ int x; asm volatile ("stc sr,%0" : "=r" (x)   ) ; x;})
#define get_imask() 	((get_cr() >> 4) & 0xf)
#define set_imask(x) 	{ int sr = get_cr(); sr &= ~0xf0; sr |= x<< 4;  set_cr(sr);}
#define set_vbr(vbr) 	{ asm volatile ("ldc	%0,vbr" :  : "r" (vbr)); }
#define get_vbr ()  	({ int vbr; asm volatile ("stc vbr,%0" : "=r" (vbr)   ) ; vbr;})
#define set_gbr(gbr) 	{ asm volatile ("ldc	%0,gbr" :  : "r" (gbr)); }
#define get_gbr ()  	({ int gbr; asm volatile ("stc gbr,%0" : "=r" (gbr)   ) ; gbr;})

#define save_iregs () \
{                     \
 asm volatile ("      \
    stc.l  gbr,@-r15  \
	sts.l  macl,@-r15 \
	sts.l  mach,@-r15 \
"::); }            

#define restore_iregs () \
{                        \
 asm volatile ("         \
	lds.l	@r15+,mach   \
	lds.l	@r15+,macl   \
    ldc.l  @r15+,gbr     \
"::); }            

#define gbr_read_byte(offset) \
  ({ int x; asm volatile  ("mov.b 	@(%O1,gbr),r0\n\tmov	r0,%0" : "=r" (x) : "i" (offset) : "r0") ; x;})

#define gbr_read_word(offset) \
  ({ int x; asm volatile  ("mov.w 	@(%O1,gbr),r0\n\tmov	r0,%0" : "=r" (x) : "i" (offset) : "r0") ; x;})

#define gbr_read_long(offset)  \
     ({ int x; asm volatile  ("mov.l 	@(%O1,gbr),r0\n\tmov	r0,%0" : "=r" (x) : "i" (offset) : "r0") ; x;})

#define gbr_write_byte(offset, data)  \
   { asm volatile("mov.b	%0,@(%O1,gbr)" : : "z" (data), "i" (offset)); }

#define gbr_write_byte(offset, data)  \
   { asm volatile("mov.b	%0,@(%O1,gbr)" : : "z" (data), "i" (offset)); }

#define gbr_write_word(offset, data)  \
   { asm volatile("mov.w	%0,@(%O1,gbr)" : : "z" (data), "i" (offset)); }

#define gbr_write_long(offset, data)  \
   { asm volatile("mov.l	%0,@(%O1,gbr)" : : "z" (data), "i" (offset)); }

#define gbr_and_byte(offset, mask) \
   { asm volatile("and.b	%0,@(%1,gbr)" : : "i" (mask), "z" (offset)); }

#define gbr_or_byte(offset, mask) \
   { asm volatile("or.b	%0,@(%1,gbr)" : : "i" (mask), "z" (offset)); }

#define gbr_xor_byte(offset, mask) \
   { asm volatile("xor.b	%0,@(%1,gbr)" : : "i" (mask), "z" (offset)); }

#define gbr_tst_byte(offset, mask) \
   { asm volatile("tst.b	%0,@(%1,gbr)" : : "i" (mask), "z" (offset)); }
