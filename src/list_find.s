section .text

default rel

extern strcmp

global list_find
list_find:
		push	RBP
		mov	RBP, RSP

		push	RBX
		mov	RBX, RDI
		push	R12
		mov	R12, RSI

		sub	RSP, 8



	.Loop:	test	RBX, RBX
		jz	.No

		mov	RDI, [RBX + 8]
		mov	RSI, R12
		call	strcmp wrt ..plt

		mov	RBX, [RBX + 0]

		test	EAX, EAX
		jnz	.Loop



		mov	 AL, 1

		add	RSP, 8
		pop	R12
		pop	RBX
		pop	RBP
		ret

	.No:	xor	 AL,  AL

		add	RSP, 8
		pop	R12
		pop	RBX
		pop	RBP
		ret
