#include "dt.h"
#include <core/mem.h>
#include <core/endian.h>

static inline void fdt_node_count(u32 *data, u32 *n_prop, u32 *n_sub)
{
	u32 depth = 0, prop_len;

	*n_prop = *n_sub = 0;

	for (data++; data[0] != FDT_END; data++) {
		switch (data[0]) {
		case FDT_BEGIN_NODE: 
			depth++;
			break;
		case FDT_END_NODE:
			if (!depth)
				return;
			(*n_sub) += (depth == 1);
			depth--;
			break;
		case FDT_PROP:
			// skip length of prop data, rounded up to 32-bit window,
			// + attributes
			prop_len = reverse_endian_u32(data[1]);

			data += ((prop_len + 3) / 4) + 2;
			*n_prop += !depth; 	
			break;
		default:
			break;
		}
	}
}

static u32 *fdt_node_name(u32 *data, struct fdt_node *node)
{
	int n = 0;
	u8 *data_u8 = (u8 *)(data + 1);

	if (*data != FDT_BEGIN_NODE)
		return data;
	
	while (data_u8[n])
		n++;
	n++;

	node->name = kmalloc(n);
	memcpy(node->name, data + 1, n);

	return data + ((n+7)/4);
}

static u32 *fdt_prop_parse(u32 *data,
			   struct fdt_prop_desc *prop,
			   struct fdt_header *header)
{
			char *name;
			int n = 0;

			prop->len = reverse_endian_u32(data[1]);
			prop->nameoff = reverse_endian_u32(data[2]);
			prop->data = kmalloc(prop->len);
			memcpy(prop->data, data + 3, prop->len);

			name = header->dtb_strings + prop->nameoff;
			while (name[n])
				n++;
			n++;
			prop->name = kmalloc(n);
			memcpy(prop->name, name, n);
			
			return data +((prop->len + 3) / 4) + 2;
}

static u32 *fdt_node_parse(u32 *data, 
			   struct fdt_node *node, 
			   struct fdt_header *header)
{	
	u32 n_prop = 0, n_sub = 0, depth = 0;
	struct fdt_prop_desc *prop;

	if (*data != FDT_BEGIN_NODE)
		return data;

	fdt_node_count(data, &n_prop, &n_sub);

	node->props = kmalloc(sizeof(struct fdt_prop_desc) * n_prop);
	node->subnodes = kmalloc(sizeof(struct fdt_node) * n_sub);

	data = fdt_node_name(data, node);
	n_sub = n_prop = 0;

	for (; data[0] != FDT_END; data++) {
		switch (data[0]) {
		case FDT_BEGIN_NODE: 
			data = fdt_node_parse(data, node->subnodes + n_sub, header);
			n_sub++;
			break;
		case FDT_PROP:
			data = fdt_prop_parse(data, node->props + n_prop, header);
			n_prop++;
			break;
		case FDT_END_NODE:
			return data;
		default:
			break;
		}
	}

	return data;
}

struct fdt_node *fdt_parse(void *data, struct fdt_header *header)
{
	int size = sizeof(struct fdt_header), i;
	u32 *header_u32;

	struct fdt_node *node = kmalloc(sizeof(struct fdt_node));

	if (header == 0)
		header = kmalloc(size);
	
	*header = *(struct fdt_header *)(data);
	header_u32 = (u32*)(void*)(header);

	for (i = 0; i < (size / sizeof(u32)) - 3; i++)
		header_u32[i] = reverse_endian_u32(header_u32[i]);
	
	header->dtb_struc = data + header->off_dt_struct;
	header->dtb_strings = data + header->off_dt_strings;
	header->dtb_mem_rsv = data + header->off_mem_rsvmap;
	
	if (header->magic != FDT_HEADER_MAGIC)
		return 0;
	
	while (header->dtb_struc[0] != FDT_BEGIN_NODE)
		header->dtb_struc++;
	
	fdt_node_parse(header->dtb_struc, node, header);

	return node;
}
