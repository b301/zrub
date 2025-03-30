#ifndef __ZRUB_USTR_H__
#define __ZRUB_USTR_H__

#include "zrub/global.h"
#include <string.h>


char *zrub_str_as_dyn(char *s);

int32_t zrub_str_len(char *s);
int32_t zrub_str_findc(char *s, char c);
int32_t zrub_str_countc(char *s, char c);
int32_t zrub_str_copy(char *src, char *dest, int32_t dest_size, int32_t n);
int32_t zrub_str_splitc(char *s, char d, char **l);

bool zrub_str_compare(char *s1, char *s2, int32_t n);
bool zrub_str_delsfx(char *d, char *sfx);

// TODO: impl the below
bool zrub_str_delpfx(char *d, const char *pfx);
void zrub_str_add_prefix(char *dest, const char *prefix);
void zrub_str_add_suffix(char *dest, const char *suffix);


#endif // __ZRUB_A_H__
