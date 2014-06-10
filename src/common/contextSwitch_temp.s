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
	/* switch to svc mode */
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
	# Do NOT touch r0! It holds the pointer to the request.

	# Grab pc & SPSR
	mov r1, lr
	mrs r2, spsr

	# Switch to system mode.
	msr cpsr_c, #159

	mov lr, r1

	# Save user's registers.
	stmfd sp!, {r4, r5, r6, r7, r8, sb, sl, fp, ip, lr}

	# Grab user sp into r3
	mov r3, sp

	# Back to supervisor mode.
	msr cpsr_c, #147

	# Restore kernel registers.
	# The kernel's pointer to the user's stack ptr is put into r1
	ldmfd sp!, {r1, r4, r5, r6, r7, r8, sb, sl, fp, ip}

	# save active task fields
	str r2, [r1, #4]
	str r3, [r1, #8]
	str lr, [r1, #12]

	# Jump back to whoever called k_exit
	ldmfd sp!, {pc}

	.size	kerent, .-kerent
	.align	2
	.global	kerxit
	.type	kerxit, %function
kerxit:
	@ args = 0, pretend = 0, frame = 4
	@ frame_needed = 1, uses_anonymous_args = 0
	# Don't touch r0! It holds the user's return value.

	# Save kernel's registers.
	# r1 holds kernel's pointer to user's stack ptr, so we can update it later.
	stmfd sp!, {r0, r4, r5, r6, r7, r8, sb, sl, fp, ip, lr}

	# Switch to system mode.
	msr cpsr_c, #159

	# Restore user's stack ptr
	ldr r3, [r0, #4]
	ldr sp, [r0, #8]
	ldr r2, [r0, #12]

	# Restore task's registers.
	ldmfd sp!, {r4, r5, r6, r7, r8, sb, sl, fp, ip, lr}

	# Back to supervisor mode.
	msr cpsr_c, #147

	# Restore user SPSR
	msr spsr, r3

	# Jump to user task (this will also move the svc SPSR into users CPSR).
	movs pc, r2
	.size	kerxit, .-kerxit
	.ident	"GCC: (GNU) 4.0.2"
