#ifndef _CSLIB_STRING_H
#define _CSLIB_STRING_H

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "cslib/pub.h"
#include "cslib/types/vector.h"

bool cslib_string_remove_prefix(char *str, const char *prefix);
bool cslib_string_remove_suffix(char *str, const char *prefix);

bool cslib_string_check_prefix(const char *str, const char *prefix);
bool cslib_string_check_suffix(const char *str, const char *suffix);

char *cslib_string_create(const char *data, size_t min_size);

cslib_vector_t *cslib_string_split(const char *str, const char *delimiter);
size_t cslib_string_count_substring(const char *str, const char *sub);
char *cslib_string_slice(const char *str, size_t begin, size_t end);
void cslib_string_slice2(char *str, const char *data, size_t begin, size_t end);
char *cslib_string_leftpad(const char *data, const char *pad, size_t count);

void cslib_string_lowercase(char *str);
void cslib_string_capitalize(char *str);

#endif // _CSLIB_STRING_H
