#ifndef __ZRUB_CFG_KV_H__
#define __ZRUB_CFG_KV_H__

#include "zrub/global.h"

#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#define BUFSIZE 4096
#define MAXLINE 1024
#define MAXSECT 256

bool get_kv_cfg(char *raw_config, void *dataptr, i32 num_args, ...);


#endif // __ZRUB_CFG_KV_H__