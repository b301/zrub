#include "zrub/auth/oauth2.h"


bool zrub_oauth2_gen_token(struct zrub_oauth2_token *token)
{
    /* access token */
    uint8_t bytes[ZRUB_OAUTH2_TOKEN_BYTES];
    
    if (!zrub_randombytes(bytes, ZRUB_OAUTH2_TOKEN_BYTES))
    {
        return false;
    }

    memcpy(token->access_token, bytes, ZRUB_OAUTH2_TOKEN_BYTES);

    token->access_expiry = (uint64_t)time(NULL) + ZRUB_OAUTH2_ACCESS_EXPIRY;

    /* refresh token */
    if (!zrub_randombytes(bytes, ZRUB_OAUTH2_TOKEN_BYTES))
    {
        return false;
    }

    memcpy(token->refresh_token, bytes, ZRUB_OAUTH2_TOKEN_BYTES);

    token->refresh_expiry = (uint64_t)time(NULL) + ZRUB_OAUTH2_REFRESH_EXPIRY;
    token->next = NULL;

    return true;
}

bool zrub_oauth2_initialize(struct zrub_oauth2_token **token)
{
    *token = (struct zrub_oauth2_token*)ZRUB_MALLOC(
                sizeof(struct zrub_oauth2_token));

    return zrub_oauth2_gen_token(*token);
}

bool zrub_oauth2_append(struct zrub_oauth2_token *tokenlist)
{
    if (tokenlist == NULL)
    {
        ZRUB_LOG_ERROR("tokenlist is null\n");
        return false;
    }

    struct zrub_oauth2_token *tkn = tokenlist;

    /* get to the last token in the list  */
    while (tkn->next != NULL)
    {
        tkn = tkn->next;
    }

    return zrub_oauth2_initialize(&tkn->next);
}

bool zrub_oauth2_remove(struct zrub_oauth2_token **tokenlist, int32_t idx)
{
    if (*tokenlist == NULL)
    {
        ZRUB_LOG_ERROR("tokenlist is null\n");
        return false;
    }

    int32_t index = 0;
    struct zrub_oauth2_token *ptr = *tokenlist;

    /* [a] -> [b] -> ... ---> [b] -> ...*/
    if (idx == 0)
    {
        *tokenlist = (*tokenlist)->next;
        ZRUB_FREE(ptr);

        return true;
    }

    struct zrub_oauth2_token *rm = NULL;
    while (ptr != NULL)
    {
        if (idx == index + 1)
        {
            /* [a] -> [b] -> [c] ---> [a] -> [b] */
            if (ptr->next != NULL && ptr->next->next != NULL)
            {
                rm = ptr->next;
                ptr->next = ptr->next->next;

                ZRUB_FREE(rm);
                return true;
            }

            /* .. -> [a] -> [b] ---> ... -> [a] */
            if (ptr->next != NULL && ptr->next->next == NULL)
            {
                rm = ptr->next;
                ptr->next = NULL;

                ZRUB_FREE(rm);
                return true;
            }
        }

        index++;
        ptr = ptr->next;
    }

    return false;
}

void zrub_oauth2_finalize(struct zrub_oauth2_token **token)
{
    struct zrub_oauth2_token *tkn = *token;
    struct zrub_oauth2_token *rm = NULL;

    while (tkn != NULL)
    {
        rm = tkn;
        tkn = tkn->next;

        ZRUB_FREE(rm);
    }

    *token = NULL;
    token = NULL;
}

void zrub_oauth2_display(const struct zrub_oauth2_token *token)
{
    ZRUB_TODO("refactor this\n");
    ZRUB_NOT_IMPLEMENTED();

    const struct zrub_oauth2_token *tkn = token;
    const char *format = 
"[[ index: %lu ]]               \n\
access: %s (expires: %lu)       \n\
refresh: %s (expires: %lu)      \n";

    uint32_t idx = 0;

    while (tkn != NULL)
    {
        printf(format, idx, token->access_token, token->access_expiry, \
            token->refresh_token, token->refresh_expiry);

        tkn = tkn->next;
        idx++;
    }
}
