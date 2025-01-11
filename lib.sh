#!/bin/bash

set -x
set -e

[[ ! -e build ]] && mkdir -p build
[[ ! -e build/tests ]] && mkdir -p build/tests
[[ ! -e build/bin/tests ]] && mkdir -p build/bin/tests

EXTRA_ARGS="-I${PWD} -Wall -s"

# compile src to obj
gcc ${EXTRA_ARGS} -c cslib/std/string.c -o build/string.o
gcc ${EXTRA_ARGS} -c cslib/std/linked_list.c -o build/linked_list.o
gcc ${EXTRA_ARGS} -c cslib/std/hashmap.c -o build/hashmap.o
gcc ${EXTRA_ARGS} -c cslib/std/vector.c -o build/vector.o

# combine base types object files to one
ld -relocatable build/hashmap.o \
    build/linked_list.o \
    build/string.o \
    build/vector.o \
    -o build/cslib_types.o

# compile iterator
gcc ${EXTRA_ARGS} -c cslib/std/iterator.c build/cslib_types.o -o build/iterator.o
gcc ${EXTRA_ARGS} -c cslib/os/file.c build/cslib_types.o -o build/file.o

# combine all into cslib.o
ld -relocatable build/cslib_types.o \
    build/iterator.o \
    build/file.o \
    -o build/cslib.o

