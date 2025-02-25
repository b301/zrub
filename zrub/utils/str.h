#ifndef __ZRUB_A_H__
#define __ZRUB_A_H__

#include "zrub/global.h"
#include <string.h>

void zrub_str_rm_prefix(char *dest, const char *prefix);

// TODO: impl the below
void zrub_str_rm_suffix(char *dest, const char *suffix);
void zrub_str_add_prefix(char *dest, const char *prefix);
void zrub_str_add_suffix(char *dest, const char *suffix);


#endif // __ZRUB_A_H__