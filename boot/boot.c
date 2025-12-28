#include <core/types.h>
#include <core/endian.h>

extern void _boot_main()
{
	u16 n = 0x1234;

	n = reverse_endian_u16(n);

	for (;;);
}

