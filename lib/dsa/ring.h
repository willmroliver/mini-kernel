#ifndef INC_LIB_DSA_RING_H
#define INC_LIB_DSA_RING_H

#include <core/types.h>

struct ring8 {
	u8 *buf;
	u32 size;
	u32 mask;
	u64 start;
	u64 end;
};

struct ring8 *ring8_alloc(u32 size);

void ring8_free(struct ring8 *ring8);

int ring8_push(struct ring8 *ring8, u8 byte);

int ring8_pop(struct ring8 *ring8, u8 *byte);

int ring8_full(struct ring8 *ring8);

int ring8_empty(struct ring8 *ring8);

u32 ring8_size(struct ring8 *ring8);

u8 ring8_front(struct ring8 *ring8);

u8 ring8_back(struct ring8 *ring8);

struct ring {
	void **buf;
	u32 size;
	u32 mask;
	u64 start;
	u64 end;
};

struct ring *ring_alloc(u32 size);

void ring_free(struct ring *ring);

int ring_push(struct ring *ring, void *ptr);

int ring_pop(struct ring *ring, void *ptr);

int ring_full(struct ring *ring);

int ring_empty(struct ring *ring);

u32 ring_size(struct ring *ring);

void *ring_front(struct ring *ring);

void *ring_back(struct ring *ring);

#endif
