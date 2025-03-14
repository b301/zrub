#include "zrub/auth/oauth2.h"


bool zrub_oauth2_generate_token(struct zrub_oauth2_token **token)
{
    ZRUB_NOT_IMPLEMENTED;
    ZRUB_UNUSED(token);

    /* NOTE: use this to generate the tokens
    u8 bytes[ZRUB_OAUTH2_TOKEN_BYTES];
    zrub_randombytes(bytes, ZRUB_OAUTH2_TOKEN_BYTES);
    */

    return false;
}

bool zrub_oauth2_add_token(struct zrub_oauth2_token *tokenlist)
{
    ZRUB_NOT_IMPLEMENTED;
    ZRUB_UNUSED(tokenlist);

    return false;
}

bool zrub_oauth2_del_token(struct zrub_oauth2_token **tokenlist, i32 idx)
{
    ZRUB_NOT_IMPLEMENTED;
    ZRUB_UNUSED(tokenlist);
    ZRUB_UNUSED(idx);

    return false;
}
