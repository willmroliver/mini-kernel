#include "dt.h"
#include <core/mem.h>
#include <core/endian.h>

struct fdt_node *fdt_parse(void *data, struct fdt_header *header)
{
	int size = sizeof(struct fdt_header), i;
	u32 *header_u32;

	if (header == 0)
		header = kalloc(size);
	
	*header = *(struct fdt_header *)(data);
	header_u32 = (u32*)(void*)(header);

	for (i = 0; i < size / sizeof(u32); i++)
		header_u32[i] = reverse_endian_u32(header_u32[i]);
	
	if (header->magic != FDT_HEADER_MAGIC)
		return 0;
	
	// @TODO - recursively parse structure block
	
	return 0;
}
