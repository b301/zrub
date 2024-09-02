#ifndef _CSLIB_PUB_H
#define _CSLIB_PUB_H

#define CHECK(statement, func) if (!(statement)) { printf("%s failed\n", func); return 1; } printf("%s passed\n", func)
#define ALLOC_OBJECT(type) (type*)malloc(sizeof(type))

#endif
