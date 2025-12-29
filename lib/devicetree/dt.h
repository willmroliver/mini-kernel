#ifndef INC_FDT_H
#define INC_FDT_H

#include <core/types.h>

static const u32 FDT_HEADER_MAGIC = 0xd00dfeed; 

static const u32 FDT_BEGIN_NODE = 0x00000001; // beginning of a ndoe
static const u32 FDT_END_NODE   = 0x00000002; // end of a nide
static const u32 FDT_PROP       = 0x00000003; // next token is a property
static const u32 FDT_NOP        = 0x00000004; // all NOP values can be ignored
static const u32 FDT_END        = 0x00000009; // end of the structure block

struct fdt_header {
	u32 magic;              // should always be 0xd00dfeed
	u32 totalsize;          // total size of fdt in bytes
	u32 off_dt_struct;      // byte-offset of structure block
	u32 off_dt_strings;     // offset of strings block
	u32 off_mem_rsvmap;     // offset of memory reservation block
	u32 version;            // devicetree spec version
	u32 last_comp_version;  // lowest compatible version
	u32 boot_cpuid_phys;    // physical id of boot CPU 
	u32 size_dt_strings;    // length of strings block
	u32 size_dt_struct;     // length of structs block
};

// memory reservation blocks are always terminated 
// with a zeroed fdt_reserve_entry

struct fdt_reserve_entry {
	u64 address; // starting address of reserved mem block
	u64 size;    // size in bytes
};

// after an FDT_PROP token in the structure block, the below data
// immediately follows (big-endian), following which is the prop itself

struct fdt_prop_desc {
	u32  len;     // length of the proceeding prop description in bytes
	u32  nameoff; // offset in the strings block of the name of this prop
	char *name;
	u8   *prop;
};

struct fdt_node {
	struct fdt_prop_desc *props;
	struct fdt_node *subnodes;
};

/**
 * Parses a flattened device tree, returning the result as a 
 * tree of structs.
 *
 * Can optionally pass in a header struct to retrieve metadata.
 */
struct fdt_node *fdt_parse(void *data, struct fdt_header* header);

void fdt_destroy(struct fdt_node* fdt);

#endif
