#ifndef INC_CORE_ENDIAN_H
#define INC_CORE_ENDIAN_H

#include "types.h"

extern u64 arch_reverse_endian_u64(u64 n, u64 width);

static inline u16 reverse_endian_u16(u16 n)
{	
	return arch_reverse_endian_u64((u64)n, 0x10);
}

static inline u32 reverse_endian_u32(u32 n)
{
	return (u32)arch_reverse_endian_u64((u64)n, 0x20);
}

static inline u64 reverse_endian_u64(u64 n)
{
	return arch_reverse_endian_u64(n, 0x40);
}

static inline i16 reverse_endian_i16(i16 n)
{
	return (i16)arch_reverse_endian_u64((u16)n, 0x10);
}

static inline i32 reverse_endian_i32(i32 n)
{
	return (i32)arch_reverse_endian_u64((u32)n, 0x20);
}

static inline i64 reverse_endian_i64(i64 n)
{
	return (i64)arch_reverse_endian_u64((u64)n, 0x40);
}

#endif
