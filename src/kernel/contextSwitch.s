	.file	"contextSwitch.c"
	.text
	.align	2
	.global	kerent
	.type	kerent, %function
kerent:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
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
	ldmfd	sp, {r0,r1,r4,r5,r6,r7,r8,r9,r10,fp}
	/* 10 fill in the request with its arguments*/
	str r2, [r1, #0]
	/* 11 put the sp and spsr into the TD of the active task*/
	str r3, [r0, #8]
	str	ip, [r0, #4]
	mov pc, lr
	.size	kerent, .-kerent
	.align	2
	.global	kerxit
	.type	kerxit, %function
kerxit:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	/* 1 store kernel reg */
	stmfd	sp!, {r0,r1,r4,r5,r6,r7,r8,r9,r10,fp}
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
	.size	kerxit, .-kerxit
	.ident	"GCC: (GNU) 4.0.2"
