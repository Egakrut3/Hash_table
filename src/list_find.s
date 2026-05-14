section .text

default rel

extern strcmp

global list_find
list_find:
		push	RBX
		push	RBP
		push	RDX
		push	R12
		mov	R12, RSI
		sub	RSP, 8

		mov	RBX, [RDI + 10h]
		mov	RBP, [RBX + 0 + 0]
	.Loop:	cmp	RBP, -1
		je	.No

		shl	RBP, 4
		mov	RDI, [RBX + RBP + 8]
		mov	RBP, [RBX + RBP + 0]
		mov	RSI, R12
		call	strcmp wrt ..plt

		test	RAX, RAX
		jnz	.Loop

		xor	RAX, RAX
		add	RSP, 8
		pop	R12
		pop	RDX
		mov	byte [RDX], 1
		pop	RBP
		pop	RBX
		ret

	.No:	xor	RAX, RAX
		add	RSP, 8
		pop	R12
		pop	RDX
		mov	byte [RDX], AL
		pop	RBP
		pop	RBX
		ret
