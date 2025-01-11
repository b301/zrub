#!/bin/bash

set -x
set -e

[[ ! -e build ]] && mkdir -p build
[[ ! -e build/tests ]] && mkdir -p build/tests
[[ ! -e build/bin/tests ]] && mkdir -p build/bin/tests

EXTRA_ARGS="-I${PWD} -Wall -s"

# compile src to obj
gcc ${EXTRA_ARGS} -c cslib/types/string.c -o build/string.o
gcc ${EXTRA_ARGS} -c cslib/types/linked_list.c -o build/linked_list.o
gcc ${EXTRA_ARGS} -c cslib/types/hashmap.c -o build/hashmap.o
gcc ${EXTRA_ARGS} -c cslib/types/vector.c -o build/vector.o
gcc ${EXTRA_ARGS} -c cslib/types/queue.c -o build/queue.o

# combine base types object files to one
ld -relocatable build/hashmap.o \
    build/linked_list.o \
    build/string.o \
    build/vector.o \
    build/queue.o \
    -o build/cslib_types.o

# compile rest
gcc ${EXTRA_ARGS} -c cslib/types/iterator.c build/cslib_types.o -o build/iterator.o
gcc ${EXTRA_ARGS} -c cslib/os/file.c build/cslib_types.o -o build/file.o
gcc ${EXTRA_ARGS} -c cslib/os/time.c build/cslib_types.o -o build/time.o

# combine all into cslib.o
ld -relocatable build/cslib_types.o \
    build/iterator.o \
    build/file.o \
    build/time.o \
    -o build/cslib.o

