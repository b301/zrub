#ifndef __ZRUB_CFG_KV_H__
#define __ZRUB_CFG_KV_H__

#include "zrub/global.h"

#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define BUFSIZE 4096
#define MAXLINE 1024
#define MAXSECT 256

/**
 * @note the dataptr should be of the type set in the ORA config file
 */
bool zrub_get_kv_cfg(char *raw_config, void *dataptr, i32 num_args, ...);


#endif // __ZRUB_CFG_KV_H__
