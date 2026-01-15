#ifndef INC_LIB_DSA_RING_H
#define INC_LIB_DSA_RING_H

#include <core/types.h>

struct ring {
	u8 *buf;
	u32 size;
	u32 mask;
	u64 start;
	u64 end;
};

struct ring *ring_alloc(u32 size);

void ring_free(struct ring *ring);

int ring_push(struct ring *ring, u8 byte);

int ring_pop(struct ring *ring, u8 *byte);

int ring_full(struct ring *ring);

int ring_empty(struct ring *ring);

u32 ring_size(struct ring *ring);

u8 ring_front(struct ring *ring);

u8 ring_back(struct ring *ring);

#endif
