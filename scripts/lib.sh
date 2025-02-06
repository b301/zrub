#!/bin/bash

set -x
set -e

[[ ! -e build ]] && mkdir -p build
[[ ! -e build/tests ]] && mkdir -p build/tests
[[ ! -e build/bin/tests ]] && mkdir -p build/bin/tests

EXTRA_ARGS="-I${PWD} -Wall -Wextra -pedantic -O3 -s"

# compile src to obj
gcc ${EXTRA_ARGS} -c zrublib/types/string.c -o build/string.o
gcc ${EXTRA_ARGS} -c zrublib/types/linked_list.c -o build/linked_list.o
gcc ${EXTRA_ARGS} -c zrublib/types/hashmap.c -o build/hashmap.o
gcc ${EXTRA_ARGS} -c zrublib/types/vector.c -o build/vector.o
gcc ${EXTRA_ARGS} -c zrublib/types/queue.c -o build/queue.o

# combine base types object files to one
ld -relocatable build/hashmap.o \
    build/linked_list.o \
    build/string.o \
    build/vector.o \
    build/queue.o \
    -o build/zrublib_types.o

# compile rest
gcc ${EXTRA_ARGS} -c zrublib/types/iterator.c build/zrublib_types.o -o build/iterator.o
gcc ${EXTRA_ARGS} -c zrublib/os/file.c build/zrublib_types.o -o build/file.o
gcc ${EXTRA_ARGS} -c zrublib/os/time.c build/zrublib_types.o -o build/time.o
gcc ${EXTRA_ARGS} -c zrublib/cipher/classic.c build/zrublib_types.o -o build/cipher_classic.o

# combine all into zrublib.o
ld -relocatable build/zrublib_types.o \
    build/iterator.o \
    build/file.o \
    build/time.o \
    build/cipher_classic.o \
    -o build/zrublib.o

