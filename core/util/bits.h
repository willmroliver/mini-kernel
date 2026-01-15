#ifndef INC_UTIL_BITS_H
#define INC_UTIL_BITS_H

#include <core/types.h>

#define BIT(n)		  ((1ull) << (n))
#define BITS(to, from)    ((-1u >> ((8*sizeof(int))-((to)-(from)+1))) << (from))
#define BITS_64(to, from) ((0xffffffffffffffff >> (64-((to)-(from)+1))) << (from))
#define BITS_32(to, from) ((0xffffffff >> (32 - ((to)-(from)+1))) << (from))
#define ALIGN(n, a)	  ((n) & ~((a)-1))
#define ALIGN_FWD(n, a)   (((n) + ((a)-1)) & ~(a - 1))

static inline u64 set_bits_64(u64 target, u64 bits, int to, int from)
{
	return (target & ~BITS_64(to, from)) | (bits << from);
}

static inline u32 set_bits_32(u32 target, u32 bits, int to, int from)
{
	return (target & ~BITS_64(to, from)) | (bits << from);	
}

static inline u64 mask_set_bits_64(u64 target, u64 bits, u64 mask, int from)
{
	return (target & ~mask) | ((bits << from) & mask);
}

static inline u32 mask_set_bits_32(u32 target, u32 bits, u32 mask, int from)
{
	return (target & ~mask) | ((bits << from) & mask);
}

static inline u64 range_set_bits_64(u64 target, u64 bits, u64 from, u64 range)
{
	target &= ~(BITS_64(from+range, from));
	return target | (bits << (64 - range) >> (64 - range));
}

static inline u32 range_set_bits_32(u32 target, u32 bits, u32 from, u32 range)
{
	target &= ~(BITS_32(from+range-1, from));
	return target | (bits << from);
}

#endif
