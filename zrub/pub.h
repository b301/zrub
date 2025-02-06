#ifndef _ZRUB_PUB_H
#define _ZRUB_PUB_H

#include <stdio.h>
#include <stdlib.h>

#define ZRUB_MALLOC malloc
#define CHECK(statement, message) if (!(statement)) { printf("%s failed\n", message); return 1; } printf("%s passed\n", message)
#define ALLOC_OBJECT(type) (type*)ZRUB_MALLOC(sizeof(type))

#endif
