#include "endian.h"

extern u64 arch_reverse_endian_u64(u64 n, u64 width);

u16 reverse_endian_u16(u16 n)
{	
	return arch_reverse_endian_u64((u64)n, 0x10);
}

u32 reverse_endian_u32(u32 n)
{
	return (u32)arch_reverse_endian_u64((u64)n, 0x20);
}

u64 reverse_endian_u64(u64 n)
{
	return arch_reverse_endian_u64(n, 0x40);
}

i16 reverse_endian_i16(i16 n)
{
	return (i16)arch_reverse_endian_u64((u16)n, 0x10);
}

i32 reverse_endian_i32(i32 n)
{
	return (i32)arch_reverse_endian_u64((u32)n, 0x20);
}

i64 reverse_endian_i64(i64 n)
{
	return (i64)arch_reverse_endian_u64((u64)n, 0x40);
}

