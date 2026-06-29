#ifndef INC_CORE_STR_H
#define INC_CORE_STR_H

#include <core/types.h>

size_t strlen(const char *s);

int strcmp(const char *s0, const char *s1);

int strncmp(const char *s0, const char *s1, size_t n);

#endif
