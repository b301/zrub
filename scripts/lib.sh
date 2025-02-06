#!/bin/bash

set -x
set -e

[[ ! -e build ]] && mkdir -p build
[[ ! -e build/tests ]] && mkdir -p build/tests
[[ ! -e build/bin/tests ]] && mkdir -p build/bin/tests

EXTRA_ARGS="-I${PWD} -Wall -Wextra -pedantic -O3 -s"

# compile src to obj
gcc ${EXTRA_ARGS} -c zrub/types/string.c -o build/string.o
gcc ${EXTRA_ARGS} -c zrub/types/linked_list.c -o build/linked_list.o
gcc ${EXTRA_ARGS} -c zrub/types/hashmap.c -o build/hashmap.o
gcc ${EXTRA_ARGS} -c zrub/types/vector.c -o build/vector.o
gcc ${EXTRA_ARGS} -c zrub/types/queue.c -o build/queue.o

# combine base types object files to one
ld -relocatable build/hashmap.o \
    build/linked_list.o \
    build/string.o \
    build/vector.o \
    build/queue.o \
    -o build/zrub_types.o

# compile rest
gcc ${EXTRA_ARGS} -c zrub/types/iterator.c build/zrub_types.o -o build/iterator.o
gcc ${EXTRA_ARGS} -c zrub/os/file.c build/zrub_types.o -o build/file.o
gcc ${EXTRA_ARGS} -c zrub/cipher/classic.c build/zrub_types.o -o build/cipher_classic.o

gcc ${EXTRA_ARGS} -c zrub/os/time.c -o build/time.o
gcc ${EXTRA_ARGS} -c zrub/core/logger.c -o build/logger.o

# combine all into zrub.o
ld -relocatable build/zrub_types.o \
    build/iterator.o \
    build/file.o \
    build/time.o \
    build/logger.o \
    build/cipher_classic.o \
    -o build/zrub.o
