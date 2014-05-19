	.file	"contextSwitch.c"
	.section	.rodata
	.align	2
.LC0:
	.ascii	"Page table base: \000"
	.align	2
.LC1:
	.ascii	"\012\000"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {sl, fp, ip, lr, pc}
	sub	fp, ip, #4
	sub	sp, sp, #16
	ldr	sl, .L4
.L3:
	add	sl, pc, sl
	str	r0, [fp, #-28]
	str	r1, [fp, #-32]
	ldr	r3, [fp, #-20]
	str	r3, [fp, #-24]
	mov	r0, #1
	mov	r1, #0
	bl	bwsetfifo(PLT)
	mov	r0, #1
	ldr	r3, .L4+4
	add	r3, sl, r3
	mov	r1, r3
	bl	bwputstr(PLT)
	ldr	r3, [fp, #-24]
	mov	r0, #1
	/*mov	r1, r3*/
	/*bl	bwputr(PLT)*/
	mov r0, #1
	mrc p15, 0, r1, c1, c0
	bl bwputr(PLT)
	mov r0, #1
	mrc p15, 0, r1, c2, c0
	bl bwputr(PLT)
	/**/
	mov	r0, #1
	ldr	r3, .L4+8
	add	r3, sl, r3
	mov	r1, r3
	bl	bwputstr(PLT)
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #16
	ldmfd	sp, {sl, fp, sp, pc}
.L5:
	.align	2
.L4:
	.word	_GLOBAL_OFFSET_TABLE_-(.L3+8)
	.word	.LC0(GOTOFF)
	.word	.LC1(GOTOFF)
	.size	main, .-main
	.ident	"GCC: (GNU) 4.0.2"
