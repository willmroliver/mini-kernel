.arch armv8-a

.text

.global arch_reverse_endian_u64

arch_reverse_endian_u64:
	rev x0, x0	
	mov x2, #0x40
	sub x1, x2, x1
	lsr x0, x0, x1
	ret
