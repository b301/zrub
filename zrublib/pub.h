#ifndef _ZRUBLIB_PUB_H
#define _ZRUBLIB_PUB_H

#include <stdio.h>

#define ZRUBLIB_MALLOC malloc
#define CHECK(statement, message) if (!(statement)) { printf("%s failed\n", message); return 1; } printf("%s passed\n", message)
#define ALLOC_OBJECT(type) (type*)ZRUBLIB_MALLOC(sizeof(type))

#endif
