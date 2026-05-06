section .text

default rel

extern strcmp

global list_find
list_find:
		push	RBX
		mov	RBX, RDI
		push	RBP
		mov	RBP, RSI



	.Loop:	test	RBX, RBX
		jz	.No

		mov	RDI, [RBX + 8]
		mov	RSI, RBP
		call	strcmp wrt ..plt

		mov	RBX, [RBX + 0]

		test	EAX, EAX
		jnz	.Loop



		mov	 AL, 1

		pop	RBP
		pop	RBX
		ret

	.No:	xor	 AL,  AL

		pop	RBP
		pop	RBX
		ret
