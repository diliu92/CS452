	.file	"contextSwitch.c"
	.text
	.align	2
	.global	hwi_kerent
	.type	hwi_kerent, %function

hwi_kerent:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0	
	
	/* switch to system state */
	msr	cpsr_c, #0xdf
	
	/* store scratch regs */
	stmfd	sp!, {r0,r1,r2,r3,ip}
	
	/* switch to IRQ mode */
	msr cpsr_c, #0x92
	
	/* store lr and spsr */
	sub r1, lr, #4
	mrs r2, spsr
	
	/* switch to system state */
	msr	cpsr_c, #0xdf
	
	/* save pc and spsr in stack */
	stmfd	sp!, {r1,r2}
	
	/* switch to IRQ mode(changed) */
	msr	cpsr_c, #0xd3
	
	/* jump to kerent */
	mov r0, #0
	bl kerent
	
	/* switch to system state */
	msr	cpsr_c, #0xdf
	
	/* load pc and spsr from stack */
	ldmfd 	sp!, {r1,r2}
	
	/* switch to IRQ mode */
	msr cpsr_c, #0x92
	
	/* load lr and spsr */
	mov lr, r1
	msr spsr, r2
	
	/* switch to system state */
	msr	cpsr_c, #0xdf
	
	/* load scratch regs */
	ldmfd	sp!, {r0,r1,r2,r3,ip}
	
	/* switch to IRQ mode */
	msr cpsr_c, #0x92

	movs pc, lr

	.size	hwi_kerent, .-hwi_kerent
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
	mov r1, r3
	/* 5 push the registers of the active task onto its stack */
	stmfd	sp!, {r0,r4,r5,r6,r7,r8,r9,r10,fp}
	/* 6 acquire the sp of the active */
	mov r3, sp
	/* 7 return to svc state */
	msr	cpsr_c, #0xd3
	/* 8 acquire the spsr of the active */
	mrs ip, spsr
	/* 9 pop the registers of the kernel from its stack*/
	ldmfd	sp!, {r0,r4,r5,r6,r7,r8,r9,r10,fp,lr}
	/* 11 put the sp and spsr into the TD of the active task*/
	str	ip, [r0, #4]
	str r3, [r0, #8]
	str	r1, [r0, #12]
	/* 10 fill in the request with its arguments*/
	mov r0, r2
	mov pc, lr
	.size	kerent, .-kerent
	.align	2
	.global	kerxit
	.type	kerxit, %function


kerxit:
	@ args = 0, pretend = 0, frame = 4
	@ frame_needed = 1, uses_anonymous_args = 0
	/* 1 store kernel reg */
	stmfd	sp!, {r0,r4,r5,r6,r7,r8,r9,r10,fp,lr}
	/* 2 switch to system state*/
	msr	cpsr_c, #0xdf
	/* 3 get sp, spsr, pc of active */
	ldr	r3, [r0, #4]
	ldr	sp, [r0, #8]
	ldr	r2, [r0, #12]
	/* 4 pop the regs of active task */
	ldmfd	sp!, {r0,r4,r5,r6,r7,r8,r9,r10,fp}
	/* 6 return to svc state */
	msr	cpsr_c, #0xd3
	/* 7 install spsr of the active task*/
	msr	spsr, r3
	/* 8 install the pc of the active task*/
	movs pc, r2


	.size	kerxit, .-kerxit
	.ident	"GCC: (GNU) 4.0.2"
