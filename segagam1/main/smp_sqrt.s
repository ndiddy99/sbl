!
!=======================================================================
!	�����������v�Z���[�`��
!	���� R4.L: �����Ȃ������l
!	�o�� R0.L: �������̓���
!=======================================================================
!
	.global	_SMCA_sqrt
	.section .text

	.align 2

! �����������G���g��
! �����œ��͂��ꂽ�l�ɂ���ď�����U�蕪����
! d0.l �ɕ����Ȃ����������ăR�[������
! d0.l �ɓ���������
_SMCA_sqrt:
	mov	r4,r0		!
	cmp/eq	#0,r0		!tst.l	d0		!�O�̓X�L�b�v
	bt	done		!beq.s	done		!
	nop
	mov	#0x10000,r1
	cmp/hs	r1,r0		!cmp.l	#$10000,d0	!longword? so jump
	bt	glsqrt		!bhs.s	glsqrt
	nop
	mov	#0x625,r1
	cmp/hi	r1,r0		!cmp.w	#625,d0		!���ɏ������l���H
	bt	glsqrt		!bhi.s	gsqrt		!�Ⴄ�Ȃ�ėp���[�h�ł�
	bra	qsqrt
	nop

!
!�����Ȑ��������� 16bit->8bit
!
!�����̕������͂���̕����Ԗڂ܂ł̊�̘a�ł���Ƃ����藝�𗘗p����B
!��A4�͂Q�Ԗڂ܂ł̊�̘a... 4 = 1 + 3
!   25�͂T�Ԗڂ܂ł̊�̘a...25 = 1 + 3 + 5 + 7 + 9
!
qsqrt:
	mov	#-1,r1		!move.w	#-1,d1
qsqrt1:
	add	#2,r1		!addq.w	#2,d1		!��̘a
	sub	r1,r0		!sub.w	d1,d0		!d0 - d1
	cmp/pl	r0		!bpl.s	qsqrt1		!�O�ȏ�Ȃ�܂����[�v
	bt	qsqrt1
	shar	r1		!asr.w	#1,d1		!d1 = d1 / 2
	mov	r1,r0		!move.w	d1,d0
done:
	rts
	nop


!
!�ėp�����O���������� 32bit->16bit
!
glsqrt:
	mov	#13+1,r4	!moveq.l	#13,d4
	xor	r1,r1		!	moveq.l	#0,d1
	xor	r2,r2		!	moveq.l	#0,d2
lsqrt1:
	clrt			!t bit clear
	addc	r0,r0		!	add.l	d0,d0
	addc	r1,r1		!addx.w	d1,d1
	clrt
	addc	r0,r0		!add.l	d0,d0
	addc	r1,r1		!addx.w	d1,d1
	add	r2,r2		!add.w	d2,d2
	mov	r2,r3		!move.w	d2,d3
	add	r3,r3		!add.w	d3,d3
	cmp/hi	r3,r1		!cmp.w	d3,d1		!d3 >= d1 so jump
	bf	lsqrt2		!bls.s	lsqrt2
	nop
	add	#1,r2		!addq.w	#1,d2
	add	#1,r3		!addq.w	#1,d3
	sub	r3,r1		!sub.w	d3,d1
lsqrt2:
	dt	r4		!dbra	d4,lsqrt1
	bf	lsqrt1
	nop

	clrt
	addc	r0,r0		!add.l	d0,d0
	addc	r1,r1		!addx.w	d1,d1
	clrt
	addc	r0,r0		!add.l	d0,d0
	addc	r1,r1		!addx.w	d1,d1
	add	r2,r2		!add.w	d2,d2
	mov	r2,r3		!move.l	d2,d3
	add	r3,r3		!add.w	d3,d3
	cmp/hi	r3,r1		!cmp.l	d3,d1
	bf	lsqrt3		!bls.s	lsqrt3
	nop
	add	#1,r2		!addq.w	#1,d2
	add	#1,r3		!addq.w	#1,d3
	sub	r3,r1		!sub.l	d3,d1
	
lsqrt3:
	clrt
	addc	r0,r0		!add.l	d0,d0
	addc	r1,r1		!addx.w	d1,d1
	clrt
	addc	r0,r0		!add.l	d0,d0
	addc	r1,r1		!addx.w	d1,d1
	add	r2,r2		!add.w	d2,d2
	mov	r2,r3		!move.l	d2,d3
	add	r3,r3		!add.l	d3,d3
	cmp/hi	r3,r1		!cmp.l	d3,d1
	bf	lsqrt4		!bls.s	lsqrt4
	nop
	add	#1,r2		!addq.w	#1,d2
lsqrt4:
	mov	r2,r0		!move.w	d2,d0
	rts
	nop
	

	.end
