	.file	"syscall_kern.c"
	.text
	.align	2
	.global	Task_create
	.type	Task_create, %function
Task_create:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	sub	fp, ip, #4
	sub	sp, sp, #8
	str	r0, [fp, #-16]
	str	r1, [fp, #-20]
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
	.size	Task_create, .-Task_create
	.align	2
	.type	Scheduler_isQueueEmpty, %function
Scheduler_isQueueEmpty:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	sub	fp, ip, #4
	sub	sp, sp, #16
	str	r0, [fp, #-20]
	str	r1, [fp, #-24]
	ldr	r3, [fp, #-20]
	ldr	r2, .L9
	add	r2, r3, r2
	ldr	r3, [fp, #-24]
	mov	r3, r3, asl #3
	add	r3, r2, r3
	str	r3, [fp, #-16]
	ldr	r3, [fp, #-16]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	bne	.L4
	ldr	r3, [fp, #-16]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne	.L4
	mov	r3, #1
	str	r3, [fp, #-28]
	b	.L7
.L4:
	mov	r3, #0
	str	r3, [fp, #-28]
.L7:
	ldr	r3, [fp, #-28]
	mov	r0, r3
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L10:
	.align	2
.L9:
	.word	264200
	.size	Scheduler_isQueueEmpty, .-Scheduler_isQueueEmpty
	.align	2
	.type	Scheduler_findNextPriorityQueue, %function
Scheduler_findNextPriorityQueue:
	@ args = 0, pretend = 0, frame = 12
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	sub	fp, ip, #4
	sub	sp, sp, #12
	str	r0, [fp, #-20]
	mov	r3, #0
	str	r3, [fp, #-16]
	b	.L12
.L13:
	ldr	r0, [fp, #-20]
	ldr	r1, [fp, #-16]
	bl	Scheduler_isQueueEmpty(PLT)
	mov	r3, r0
	cmp	r3, #0
	bne	.L14
	ldr	r3, [fp, #-16]
	str	r3, [fp, #-24]
	b	.L16
.L14:
	ldr	r3, [fp, #-16]
	add	r3, r3, #1
	str	r3, [fp, #-16]
.L12:
	ldr	r3, [fp, #-16]
	cmp	r3, #15
	ble	.L13
	mvn	r3, #0
	str	r3, [fp, #-24]
.L16:
	ldr	r3, [fp, #-24]
	mov	r0, r3
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
	.size	Scheduler_findNextPriorityQueue, .-Scheduler_findNextPriorityQueue
	.align	2
	.type	Scheduler_popQueue, %function
Scheduler_popQueue:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	sub	fp, ip, #4
	sub	sp, sp, #16
	str	r0, [fp, #-24]
	str	r1, [fp, #-28]
	ldr	r3, [fp, #-24]
	ldr	r2, .L24
	add	r2, r3, r2
	ldr	r3, [fp, #-28]
	mov	r3, r3, asl #3
	add	r3, r2, r3
	str	r3, [fp, #-20]
	ldr	r3, [fp, #-20]
	ldr	r3, [r3, #0]
	str	r3, [fp, #-16]
	ldr	r3, [fp, #-16]
	ldr	r2, [r3, #28]
	ldr	r3, [fp, #-20]
	str	r2, [r3, #0]
	ldr	r2, [fp, #-16]
	mov	r3, #0
	str	r3, [r2, #28]
	ldr	r3, [fp, #-20]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	bne	.L20
	ldr	r3, [fp, #-20]
	ldr	r2, [r3, #4]
	ldr	r3, [fp, #-16]
	cmp	r2, r3
	bne	.L20
	ldr	r2, [fp, #-20]
	mov	r3, #0
	str	r3, [r2, #4]
.L20:
	ldr	r3, [fp, #-16]
	mov	r0, r3
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L25:
	.align	2
.L24:
	.word	264200
	.size	Scheduler_popQueue, .-Scheduler_popQueue
	.align	2
	.global	Scheduler_getNextTask
	.type	Scheduler_getNextTask, %function
Scheduler_getNextTask:
	@ args = 0, pretend = 0, frame = 12
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	sub	fp, ip, #4
	sub	sp, sp, #12
	str	r0, [fp, #-20]
	ldr	r0, [fp, #-20]
	bl	Scheduler_findNextPriorityQueue(PLT)
	mov	r3, r0
	str	r3, [fp, #-16]
	ldr	r3, [fp, #-16]
	cmp	r3, #0
	blt	.L27
	ldr	r0, [fp, #-20]
	ldr	r1, [fp, #-16]
	bl	Scheduler_popQueue(PLT)
	mov	r3, r0
	str	r3, [fp, #-24]
	b	.L29
.L27:
	mov	r3, #0
	str	r3, [fp, #-24]
.L29:
	ldr	r3, [fp, #-24]
	mov	r0, r3
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
	.size	Scheduler_getNextTask, .-Scheduler_getNextTask
	.align	2
	.global	Scheduler_pushQueue
	.type	Scheduler_pushQueue, %function
Scheduler_pushQueue:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	sub	fp, ip, #4
	sub	sp, sp, #16
	str	r0, [fp, #-20]
	str	r1, [fp, #-24]
	str	r2, [fp, #-28]
	ldr	r3, [fp, #-20]
	ldr	r2, .L36
	add	r2, r3, r2
	ldr	r3, [fp, #-24]
	mov	r3, r3, asl #3
	add	r3, r2, r3
	str	r3, [fp, #-16]
	ldr	r0, [fp, #-20]
	ldr	r1, [fp, #-24]
	bl	Scheduler_isQueueEmpty(PLT)
	mov	r3, r0
	cmp	r3, #0
	beq	.L32
	ldr	r2, [fp, #-16]
	ldr	r3, [fp, #-28]
	str	r3, [r2, #0]
	ldr	r2, [fp, #-16]
	ldr	r3, [fp, #-28]
	str	r3, [r2, #4]
	b	.L35
.L32:
	ldr	r3, [fp, #-16]
	ldr	r2, [r3, #4]
	ldr	r3, [fp, #-28]
	str	r3, [r2, #28]
	ldr	r3, [fp, #-16]
	ldr	r3, [r3, #4]
	ldr	r2, [r3, #28]
	ldr	r3, [fp, #-16]
	str	r2, [r3, #4]
.L35:
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}
.L37:
	.align	2
.L36:
	.word	264200
	.size	Scheduler_pushQueue, .-Scheduler_pushQueue
	.ident	"GCC: (GNU) 4.0.2"
