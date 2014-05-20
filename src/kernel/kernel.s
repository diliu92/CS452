	.file	"kernel.c"
	.text
	.align	2
	.type	tasksInit, %function
tasksInit:
	@ args = 0, pretend = 0, frame = 12
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	sub	fp, ip, #4
	sub	sp, sp, #12
	str	r0, [fp, #-24]
	mov	r3, #0
	str	r3, [fp, #-20]
	b	.L2
.L3:
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-20]
	mov	r3, r3, asl #5
	add	r3, r2, r3
	str	r3, [fp, #-16]
	ldr	r2, [fp, #-16]
	ldr	r3, [fp, #-20]
	str	r3, [r2, #0]
	ldr	r2, [fp, #-16]
	mov	r3, #0
	str	r3, [r2, #4]
	ldr	r3, [fp, #-24]
	add	r2, r3, #2048
	ldr	r3, [fp, #-20]
	mov	r3, r3, asl #12
	add	r3, r2, r3
	add	r2, r3, #4096
	ldr	r3, [fp, #-16]
	str	r2, [r3, #8]
	ldr	r2, [fp, #-16]
	mov	r3, #0
	str	r3, [r2, #12]
	ldr	r2, [fp, #-16]
	mov	r3, #0
	str	r3, [r2, #16]
	ldr	r2, [fp, #-16]
	mvn	r3, #0
	str	r3, [r2, #20]
	ldr	r2, [fp, #-16]
	mvn	r3, #0
	str	r3, [r2, #24]
	ldr	r2, [fp, #-16]
	mov	r3, #0
	str	r3, [r2, #28]
	ldr	r3, [fp, #-20]
	add	r3, r3, #1
	str	r3, [fp, #-20]
.L2:
	ldr	r3, [fp, #-20]
	cmp	r3, #63
	ble	.L3
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
	.size	tasksInit, .-tasksInit
	.align	2
	.type	queuesInit, %function
queuesInit:
	@ args = 0, pretend = 0, frame = 12
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	sub	fp, ip, #4
	sub	sp, sp, #12
	str	r0, [fp, #-24]
	mov	r3, #0
	str	r3, [fp, #-20]
	b	.L7
.L8:
	ldr	r3, [fp, #-24]
	ldr	r2, .L11
	add	r2, r3, r2
	ldr	r3, [fp, #-20]
	mov	r3, r3, asl #3
	add	r3, r2, r3
	str	r3, [fp, #-16]
	ldr	r2, [fp, #-16]
	mov	r3, #0
	str	r3, [r2, #0]
	ldr	r2, [fp, #-16]
	mov	r3, #0
	str	r3, [r2, #4]
	ldr	r3, [fp, #-20]
	add	r3, r3, #1
	str	r3, [fp, #-20]
.L7:
	ldr	r3, [fp, #-20]
	cmp	r3, #15
	ble	.L8
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L12:
	.align	2
.L11:
	.word	264200
	.size	queuesInit, .-queuesInit
	.align	2
	.global	kerent
	.type	kerent, %function
kerent:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	sub	fp, ip, #4
	ldmfd	sp, {fp, sp, pc}
	.size	kerent, .-kerent
	.section	.rodata
	.align	2
.LC0:
	.ascii	"kerxit.c: Hello.\012\015\000"
	.align	2
.LC1:
	.ascii	"kerxit.c: Activating.\012\015\000"
	.align	2
.LC2:
	.ascii	"kerxit.c: Good-bye.\012\015\000"
	.text
	.align	2
	.global	kerxit
	.type	kerxit, %function
kerxit:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {sl, fp, ip, lr, pc}
	sub	fp, ip, #4
	sub	sp, sp, #8
	ldr	sl, .L18
.L17:
	add	sl, pc, sl
	str	r0, [fp, #-20]
	str	r1, [fp, #-24]
	mov	r0, #1
	ldr	r3, .L18+4
	add	r3, sl, r3
	mov	r1, r3
	bl	bwprintf(PLT)
	mov	r0, #1
	ldr	r3, .L18+8
	add	r3, sl, r3
	mov	r1, r3
	bl	bwprintf(PLT)
	bl	kerent(PLT)
	mov	r0, #1
	ldr	r3, .L18+12
	add	r3, sl, r3
	mov	r1, r3
	bl	bwprintf(PLT)
	sub	sp, fp, #16
	ldmfd	sp, {sl, fp, sp, pc}
.L19:
	.align	2
.L18:
	.word	_GLOBAL_OFFSET_TABLE_-(.L17+8)
	.word	.LC0(GOTOFF)
	.word	.LC1(GOTOFF)
	.word	.LC2(GOTOFF)
	.size	kerxit, .-kerxit
	.align	2
	.global	main
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 264348
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	sub	fp, ip, #4
	sub	sp, sp, #262144
	sub	sp, sp, #2192
	sub	sp, sp, #12
	ldr	r3, .L27
	sub	r2, fp, #12
	str	r0, [r2, r3]
	ldr	r3, .L27+4
	sub	r2, fp, #12
	str	r1, [r2, r3]
	mov	r0, #1
	mov	r1, #0
	bl	bwsetfifo(PLT)
	ldr	r3, .L27+8
	sub	r2, fp, #12
	add	r3, r2, r3
	mov	r0, r3
	bl	tasksInit(PLT)
	ldr	r3, .L27+8
	sub	r2, fp, #12
	add	r3, r2, r3
	mov	r0, r3
	bl	queuesInit(PLT)
	mov	r3, #0
	str	r3, [fp, #-16]
	b	.L21
.L22:
	ldr	r3, .L27+8
	sub	r2, fp, #12
	add	r3, r2, r3
	mov	r0, r3
	bl	Scheduler_getNextReadyTask(PLT)
	mov	r3, r0
	str	r3, [fp, #-20]
	ldr	r3, [fp, #-20]
	cmp	r3, #0
	beq	.L23
	ldr	r0, [fp, #-20]
	ldr	r1, [fp, #-24]
	bl	kerxit(PLT)
	ldr	r3, .L27+8
	sub	r2, fp, #12
	add	r3, r2, r3
	mov	r0, r3
	ldr	r1, [fp, #-24]
	bl	syscall_kernHandler(PLT)
.L23:
	ldr	r3, [fp, #-16]
	add	r3, r3, #1
	str	r3, [fp, #-16]
.L21:
	ldr	r3, [fp, #-16]
	cmp	r3, #3
	ble	.L22
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L28:
	.align	2
.L27:
	.word	-264344
	.word	-264348
	.word	-264340
	.size	main, .-main
	.ident	"GCC: (GNU) 4.0.2"
