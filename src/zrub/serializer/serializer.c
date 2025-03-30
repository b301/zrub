#include "zrub/serializer/interface.h"


void zrub_serializer_print(struct zrub_serializer *s)
{
    if (s == NULL) 
    {
        ZRUB_LOG_WARNING("passed empty serializer to print\n");
        return;
    }

    for (uint32_t i = 0; i < s->offset; i++)
    {
        printf("%02x ", s->buf[i]);
    }
    printf("\n");
}
