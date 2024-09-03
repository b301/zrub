#ifndef _CSLIB_PUB_H
#define _CSLIB_PUB_H

#define CHECK(statement, message) if (!(statement)) { printf("%s failed\n", message); return 1; } printf("%s passed\n", message)
#define ALLOC_OBJECT(type) (type*)malloc(sizeof(type))

#endif
