#ifndef _CSLIB_STRING_H
#define _CSLIB_STRING_H

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

bool cslib_remove_prefix(char *string, const char *prefix);
bool cslib_check_prefix(char *string, const char* prefix);
char* cslib_allocate_string(char *data, size_t min_size);


#endif // _CSLIB_STRING_H

