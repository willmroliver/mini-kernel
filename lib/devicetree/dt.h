#ifndef INC_FDT_H
#define INC_FDT_H

#include <core/types.h>

#define FDT_HEADER_MAGIC (0xd00dfeed) 

// The below are formatted big-endian, so that 
// tokens can be evaluated without byte-reversal 

#define FDT_BEGIN_NODE (0x01000000) // beginning of a node
#define FDT_END_NODE   (0x02000000) // end of a node
#define FDT_PROP       (0x03000000) // next token is a property
#define FDT_NOP        (0x04000000) // all NOP values can be ignored
#define FDT_END        (0x09000000) // end of the structure block

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

	u32 *dtb_struc;             // ptr to the structure block
	char *dtb_strings;	        // ptr to the strings block
	u32 *dtb_mem_rsv;           // ptr to the memory reservation block
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
	void *data;
};

struct fdt_node {
	char *name;
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
