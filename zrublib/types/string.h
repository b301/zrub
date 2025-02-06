#ifndef _ZRUBLIB_STRING_H
#define _ZRUBLIB_STRING_H

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "zrublib/pub.h"
#include "zrublib/types/vector.h"

bool zrublib_string_remove_prefix(char *str, const char *prefix);
bool zrublib_string_remove_suffix(char *str, const char *prefix);

bool zrublib_string_check_prefix(const char *str, const char *prefix);
bool zrublib_string_check_suffix(const char *str, const char *suffix);

char *zrublib_string_create(const char *data, size_t min_size);

zrublib_vector_t *zrublib_string_split(const char *str, const char *delimiter);
size_t zrublib_string_count_substring(const char *str, const char *sub);
char *zrublib_string_slice(const char *str, size_t begin, size_t end);
void zrublib_string_slice2(char *str, const char *data, size_t begin, size_t end);
char *zrublib_string_leftpad(const char *data, const char *pad, size_t count);

void zrublib_string_lowercase(char *str);
void zrublib_string_uppercase(char *str);

#endif // _ZRUBLIB_STRING_H
