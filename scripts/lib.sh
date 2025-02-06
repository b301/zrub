#!/bin/bash

set -x
set -e

[[ ! -e build ]] && mkdir -p build
[[ ! -e build/tests ]] && mkdir -p build/tests
[[ ! -e build/bin/tests ]] && mkdir -p build/bin/tests

EXTRA_ARGS="-I${PWD} -Wall -Wextra -pedantic -O3 -s"

# TYPES
gcc ${EXTRA_ARGS} -c zrub/types/string.c -o build/string.o
gcc ${EXTRA_ARGS} -c zrub/types/linked_list.c -o build/linked_list.o
gcc ${EXTRA_ARGS} -c zrub/types/hashmap.c -o build/hashmap.o
gcc ${EXTRA_ARGS} -c zrub/types/vector.c -o build/vector.o
gcc ${EXTRA_ARGS} -c zrub/types/queue.c -o build/queue.o
gcc ${EXTRA_ARGS} -c zrub/types/iterator.c -o build/iterator.o

# COMBINE TYPES
ld -relocatable build/hashmap.o \
    build/linked_list.o \
    build/string.o \
    build/vector.o \
    build/queue.o \
    build/iterator.o \
    -o build/zrub_types.o

# OS
gcc ${EXTRA_ARGS} -c zrub/os/time.c -o build/time.o
gcc ${EXTRA_ARGS} -c zrub/os/file.c -o build/file.o

# COMBINE OS
ld -relocatable build/time.o \
    build/file.o \
    -o build/os.o

gcc ${EXTRA_ARGS} -c zrub/core/logger.c -o build/logger.o
gcc ${EXTRA_ARGS} -c zrub/cipher/classic.c -o build/cipher_classic.o

# combine all into zrub.o
ld -relocatable build/zrub_types.o \
    build/os.o \
    build/logger.o \
    build/cipher_classic.o \
    -o build/zrub.o
