section .text

default rel

global my_strcmp
my_strcmp:
	vmovdqa64	ZMM0, [RDI]
	vmovdqa64	ZMM1, [RSI]
	vpcmpq		K0, ZMM0, ZMM1, 4
	kmovb		EAX, K0
	ret
