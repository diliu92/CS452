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
	add	r3, r3, #4080
	add	r3, r3, #3
	ldr	r2, [fp, #-16]
	str	r3, [r2, #8]
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
	ldr	r1, [fp, #-24]
	ldr	r2, .L6
	mov	r3, #0
	str	r3, [r1, r2]
	ldr	r1, [fp, #-24]
	ldr	r2, .L6+4
	mov	r3, #0
	str	r3, [r1, r2]
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L7:
	.align	2
.L6:
	.word	264192
	.word	264196
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
	b	.L9
.L10:
	ldr	r3, [fp, #-24]
	ldr	r2, .L13
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
.L9:
	ldr	r3, [fp, #-20]
	cmp	r3, #15
	ble	.L10
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L14:
	.align	2
.L13:
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
	ldr	sl, .L20
.L19:
	add	sl, pc, sl
	str	r0, [fp, #-20]
	str	r1, [fp, #-24]
	mov	r0, #1
	ldr	r3, .L20+4
	add	r3, sl, r3
	mov	r1, r3
	bl	bwprintf(PLT)
	mov	r0, #1
	ldr	r3, .L20+8
	add	r3, sl, r3
	mov	r1, r3
	bl	bwprintf(PLT)
/*before*/
	/* 1 store kernel reg */
	stmfd	sp!, {r0,r1,r4-sp} 
	/* 2 switch to system state*/
	msr	cpsr_c, #0xdf 
	/* 3 get sp, spsr of active */
	ldr	sp, [r0, #8]
	ldr	r3, [r0, #4]
	/* 4 pop the regs of active task */
	ldmfd	sp, {r0-r12}
	/* 6 return to svc state */
	msr	cpsr_c, #0xd3
	/* 7 install spsr of the active task*/
	msr	spsr, r3
	/* 8 install the pc of the active task*/
	ldr r3, [r0, #12]
	movs pc, r3
/*end before*/
	bl	kerent(PLT)
/*after*/
	/* 1 acquire arguments of the request */
	mov	r2, r0
	/* 2 acquire lr */
	mov	r3, lr
	/* 3 change to system state */
	msr	cpsr_c, #0xdf 
	/* 4 overwrite lr with value from 2 */
	mov	lr, r3
	/* 5 push the registers of the active task onto its stack */
	stmfd	sp!, {r0-r12}
	/* 6 acquire the sp of the active */
	mov r3, sp
	/* 7 return to svc state */
	msr	cpsr_c, #0xd3
	/* 8 acquire the spsr of the active */
	mrs ip, spsr
	/* 9 pop the registers of the kernel from its stack*/
	ldmfd	sp, {r0,r1,r4-sp}
	/* 10 fill in the request with its arguments*/
	str r2, [r1, #0]
	/* 11 put the sp and spsr into the TD of the active task*/
	str r3, [r0, #8]
	str	ip, [r0, #4]
	mov pc, lr
/*end after*/
	mov	r0, #1
	ldr	r3, .L20+12
	add	r3, sl, r3
	mov	r1, r3
	bl	bwprintf(PLT)
	sub	sp, fp, #16
	ldmfd	sp, {sl, fp, sp, pc}
.L21:
	.align	2
.L20:
	.word	_GLOBAL_OFFSET_TABLE_-(.L19+8)
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
	ldr	r3, .L29
	sub	r2, fp, #12
	str	r0, [r2, r3]
	ldr	r3, .L29+4
	sub	r2, fp, #12
	str	r1, [r2, r3]
	mov	r0, #1
	mov	r1, #0
	bl	bwsetfifo(PLT)
	sub	r3, fp, #262144
	sub	r3, r3, #12
	sub	r3, r3, #2192
	mov	r0, r3
	bl	tasksInit(PLT)
	sub	r3, fp, #262144
	sub	r3, r3, #12
	sub	r3, r3, #2192
	mov	r0, r3
	bl	queuesInit(PLT)
	mov	r3, #0
	str	r3, [fp, #-16]
	b	.L23
.L24:
	sub	r3, fp, #262144
	sub	r3, r3, #12
	sub	r3, r3, #2192
	mov	r0, r3
	bl	Scheduler_getNextReadyTask(PLT)
	mov	r3, r0
	str	r3, [fp, #-20]
	ldr	r3, [fp, #-20]
	cmp	r3, #0
	beq	.L25
	ldr	r3, .L29+8
	sub	r1, fp, #12
	add	r3, r1, r3
	ldr	r0, [fp, #-20]
	mov	r1, r3
	bl	kerxit(PLT)
	ldr	r3, .L29+8
	sub	r1, fp, #12
	ldr	r2, [r1, r3]
	sub	r3, fp, #262144
	sub	r3, r3, #12
	sub	r3, r3, #2192
	mov	r0, r3
	mov	r1, r2
	bl	syscall_kernHandler(PLT)
.L25:
	ldr	r3, [fp, #-16]
	add	r3, r3, #1
	str	r3, [fp, #-16]
.L23:
	ldr	r3, [fp, #-16]
	cmp	r3, #3
	ble	.L24
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L30:
	.align	2
.L29:
	.word	-264344
	.word	-264348
	.word	-264340
	.size	main, .-main
	.ident	"GCC: (GNU) 4.0.2"
