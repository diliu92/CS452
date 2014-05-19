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
	.ident	"GCC: (GNU) 4.0.2"
