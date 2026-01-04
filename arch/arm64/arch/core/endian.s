.text

#define n     x0
#define width x1

.global arch_reverse_endian_u64

arch_reverse_endian_u64:
	rev n, n	
	mov x2, #64
	sub width, x2, width
	lsr n, n, width 
	ret
