#include "strings.h"

size_t str_size(char *s)
{
	size_t n = 0;

	while (s[n] != '\0')
		n++;
	
	return n;
}
