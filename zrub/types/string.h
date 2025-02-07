#ifndef _ZRUB_STRING_H
#define _ZRUB_STRING_H

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "zrub/pub.h"
#include "zrub/types/vector.h"

bool zrub_string_remove_prefix(char *str, const char *prefix);
bool zrub_string_remove_suffix(char *str, const char *prefix);

bool zrub_string_check_prefix(const char *str, const char *prefix);
bool zrub_string_check_suffix(const char *str, const char *suffix);

char *zrub_string_create(const char *data, size_t min_size);

zrub_vector_t *zrub_string_split(const char *str, const char *delimiter);
size_t zrub_string_count_substring(const char *str, const char *sub);
char *zrub_string_slice(const char *str, size_t begin, size_t end);
void zrub_string_slice2(char *str, const char *data, size_t begin, size_t end);
char *zrub_string_leftpad(const char *data, const char *pad, size_t count);

void zrub_string_lowercase(char *str);
void zrub_string_uppercase(char *str);

#endif // _ZRUB_STRING_H
