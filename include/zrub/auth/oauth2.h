#ifndef __ZRUB_OAUTH2_H__
#define __ZRUB_OAUTH2_H__

#include "zrub/global.h"
#include "zrub/core/bytes.h"        /* for randombytes, bytes_as_hex */

#include <stdlib.h>                 /* for: malloc, free */

#ifndef ZRUB_OAUTH2_TOKEN_BYTES     /* allows for customization */
#define ZRUB_OAUTH2_TOKEN_BYTES     31

#endif

/* each byte is 2 characters in base 16 */
#define ZRUB_OAUTH2_HEXSTR_LENGTH   (ZRUB_OAUTH2_TOKEN_BYTES * 2 + 1) 
/* one hour */
#define ZRUB_OAUTH2_ACCESS_EXPIRY   ((uint64_t)3600)         
/* one hour and 30 seconds */
#define ZRUB_OAUTH2_REFRESH_EXPIRY  ((uint64_t)3630)         

struct zrub_oauth2_token {
    uint8_t     access_token[ZRUB_OAUTH2_TOKEN_BYTES];
    uint64_t    access_expiry;
    uint8_t     refresh_token[ZRUB_OAUTH2_TOKEN_BYTES];
    uint64_t    refresh_expiry;
    struct zrub_oauth2_token *next;
};

struct zrub_oauth2_tokenlist {
    struct zrub_oauth2_token *next;
};

bool zrub_oauth2_initialize(struct zrub_oauth2_token **token);
bool zrub_oauth2_append(struct zrub_oauth2_token *tokenlist);
bool zrub_oauth2_remove(struct zrub_oauth2_token **tokenlist, int32_t idx);
void zrub_oauth2_finalize(struct zrub_oauth2_token **token);

bool zrub_oauth2_gen_token(struct zrub_oauth2_token *token);
void zrub_oauth2_display(const struct zrub_oauth2_token *token);

#endif /* __ZRUB_OAUTH2_H__ */
