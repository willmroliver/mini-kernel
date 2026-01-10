#include "dt.h"
#include <core/mem.h>
#include <core/str.h>
#include <core/endian.h>

static inline void fdt_node_count(u32 *data, struct fdt_node *node)
{
	u32 depth = 0, prop_len;
	node->n_prop = node->n_sub = 0;

	for (data++; data[0] != FDT_END; data++) {
		switch (data[0]) {
		case FDT_BEGIN_NODE: 
			depth++;
			continue;
		case FDT_END_NODE:
			if (!depth)
				break;
			node->n_sub += (depth == 1);
			depth--;
			continue;
		case FDT_PROP:
			prop_len = reverse_endian_u32(data[1]);
			data += ((prop_len + 3) / 4) + 2;
			node->n_prop += !depth; 	
			continue;
		default:
			continue;
		}

		break;
	}

	node->props = kmalloc(sizeof(struct fdt_prop_desc) * node->n_prop);
	node->subnodes = kmalloc(sizeof(struct fdt_node) * node->n_sub);
}

static inline u32 *fdt_node_name(u32 *data, struct fdt_node *node)
{
	int n;
	char *data_char = (char *)(data + 1);

	if (*data != FDT_BEGIN_NODE)
		return data;
	
	n = strlen(data_char) + 1;

	node->name = kmalloc(n);
	memcpy(node->name, data + 1, n);

	return data + ((n+7)/4);
}

static inline u32 *fdt_prop_parse(u32 *data,
			   struct fdt_prop_desc *prop,
			   struct fdt_header *header)
{
	char *name;
	int n;

	prop->len = reverse_endian_u32(data[1]);
	prop->nameoff = reverse_endian_u32(data[2]);
	prop->data = kmalloc(prop->len);
	memcpy(prop->data, data + 3, prop->len);

	name = header->dtb_strings + prop->nameoff;
	n = strlen(name) + 1;

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
	node->n_prop = node->n_sub = 0; node->props = 0; node->subnodes = 0;

	fdt_node_count(data, node);
	data = fdt_node_name(data, node);

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

static inline u64 fdt_prop_decode_int(u32 *data, size_t n)
{
	// branchless eval of 32 and 64 bit <prop-encoded-array> 
	return reverse_endian_u32(data[n/2]) << (32 * (n/2)) 
		| reverse_endian_u32(data[0]);
}

static inline void fdt_reg_evaluate(struct fdt_node *node, u32 addr_c, u32 size_c)
{
	struct fdt_prop_desc *prop = 0;
	char *name;
	int width = addr_c + size_c;
	u32 naddr_c = addr_c, nsize_c = size_c, *data_u32;
	node->n_reg = 0; node->reg = 0;

	for (int i = 0; i < node->n_prop; i++) {
		name = node->props[i].name;
		if (strcmp(FDT_PROP_REG, name) == 0)
			prop = node->props + i;
		else if (strcmp(FDT_PROP_ADDR_C, name) == 0)
			naddr_c = reverse_endian_u32(*(u32 *)(node->props[i].data));  
		else if (strcmp(FDT_PROP_SIZE_C, name) == 0)
			nsize_c = reverse_endian_u32(*(u32 *)(node->props[i].data));
	}

	for (int i = 0; i < node->n_sub; i++)
		fdt_reg_evaluate(node->subnodes + i, naddr_c, nsize_c);

	if (!prop)
		return;

	data_u32 = (u32 *)prop->data;
	node->n_reg = prop->len / (width * 4);
	node->reg = kmalloc(sizeof(struct fdt_reg) * node->n_reg);

	for (int i = 0; i < node->n_reg; i++) {
		node->reg[i].addr = fdt_prop_decode_int(data_u32, addr_c);
		node->reg[i].size = fdt_prop_decode_int(data_u32+addr_c, size_c);
		data_u32 += width;
	}
}

struct fdt_node *fdt_parse(void *data, struct fdt_header *header)
{
	int size = sizeof(struct fdt_header);
	u32 *header_u32;

	struct fdt_node *node = kmalloc(sizeof(struct fdt_node));

	if (header == 0)
		header = kmalloc(size);
	
	*header = *(struct fdt_header *)(data);
	header_u32 = (u32*)(void*)(header);

	for (int i = 0; i < (size / sizeof(u32)) - 3; i++)
		header_u32[i] = reverse_endian_u32(header_u32[i]);
	
	header->dtb_struc = data + header->off_dt_struct;
	header->dtb_strings = data + header->off_dt_strings;
	header->dtb_mem_rsv = data + header->off_mem_rsvmap;
	
	if (header->magic != FDT_HEADER_MAGIC)
		return 0;
	
	while (header->dtb_struc[0] != FDT_BEGIN_NODE)
		header->dtb_struc++;
	
	fdt_node_parse(header->dtb_struc, node, header);
	fdt_reg_evaluate(node, 2, 1);

	return node;
}

void fdt_destroy(struct fdt_node *node)
{ 
	for (int i = 0; i < node->n_prop; i++) {
		kfree(node->props[i].name);
		kfree(node->props[i].data);
	}

	for (int i = 0; i < node->n_sub; i++)
		fdt_destroy(node->subnodes + i);
	
	kfree(node->name);
}
