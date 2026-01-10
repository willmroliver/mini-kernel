#include "str.h"

size_t strlen(const char *s)
{
	size_t n = 0;
	while (s[n])
		n++;
	return n;
}

int strcmp(const char *s0, const char *s1)
{
	int cmp = 0;
	for (int i = 0; ; i++) {
		cmp += (s0[i] != s1[i]);
		if (s0[i] == '\0')
			break;
		if (s1[i] == '\0')
			break;
	}
	return cmp;
}

