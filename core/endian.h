#ifndef INC_CORE_ENDIAN_H
#define INC_CORE_ENDIAN_H

#include "types.h"

u16 reverse_endian_u16(u16 n);
u32 reverse_endian_u32(u32 n);
u64 reverse_endian_u64(u64 n);

i16 reverse_endian_i16(i16 n);
i32 reverse_endian_i32(i32 n);
i64 reverse_endian_i64(i64 n);

#endif
