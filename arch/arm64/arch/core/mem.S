.text

#define dst    x0
#define src    x1
#define n      x2
#define dstend x3
#define srcend x4
#define l1     x5
#define l1w    w5
#define h1     x6
#define h1w    w6
#define l2     x7
#define l2w    w7
#define h2     x8
#define h2w    w8
#define l3     x9
#define l3w    w9
#define h3     x10
#define h3w    w10
#define l4     x11
#define l4w    w11
#define h4     x12
#define h4w    w12
#define l5     x13
#define l5w    w13
#define h5     x14
#define h5w    w14
#define l6     x15
#define l6w    w15
#define h6     x16
#define h6w    w16
#define tmp1   x13

.global memcpy
memcpy:
	stp fp, lr, [sp, #-0x20]!
	str dst, [sp, #0x10]
	mov fp, sp
	
	add dstend, dst, n
	add srcend, src, n
	b 8f

.balign 16
8:
	cmp n, #64
	b.lo 4f

	ldp l1, h1, [src]
	ldp l2, h2, [src, #16]
	ldp l3, h3, [src, #32]
	ldp l4, h4, [src, #48] 
	add src, src, #64
	stp l1, h1, [dst]
	stp l2, h2, [dst, #16]
	stp l3, h3, [dst, #32]
	stp l4, h4, [dst, #48] 
	add dst, dst, #64
	sub n, n, #64
	b 8b

.balign 16 
4:
	cmp n, #32
	b.lo 2f

	ldp l1, h1, [src]
	ldp l2, h2, [src, #16]
	add src, src, #32
	stp l1, h1, [dst]
	stp l2, h2, [dst, #16]
	add dst, dst, #32
	sub n, n, #32
2:
	cmp n, #16
	b.lo 1f

	ldp l1, h1, [src], #16
	stp l1, h1, [dst], #16
	sub n, n, #16
1:
	cmp n, #8
	b.lo 4f

	ldr l1, [src], #8
	str l1, [dst], #8
	sub n, n, #8
4:
	cmp n, #4
	b.lo 3f

	ldr l1w, [src], #4
	str l1w, [dst], #4
	sub n, n, #4
3:    
	cbz n, 0f
	lsr tmp1, n, #1

	ldrb l1w, [src]
	ldrb h1w, [srcend, #-1]
	ldrb l2w, [src, tmp1]

	strb l1w, [dst]
	strb h1w, [dstend, #-1]
	strb l2w, [dst, tmp1]
0:
	mov sp, fp
	ldr x0, [sp, #0x10]
	ldp fp, lr, [sp], #0x20
	ret
