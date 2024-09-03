#!/bin/bash


set -x
set -e

[[ ! -e tests ]] && mkdir -p tests
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

# combine all into cslib.o
ld -relocatable build/cslib_types.o \
    build/iterator.o \
    -o build/cslib.o

# compile testsgrams
gcc ${EXTRA_ARGS} -c testsgrams/vector/main.c -o build/tests/vector.o
gcc ${EXTRA_ARGS} -c testsgrams/string/main.c -o build/tests/string.o
gcc ${EXTRA_ARGS} -c testsgrams/hashmap/main.c -o build/tests/hashmap.o
gcc ${EXTRA_ARGS} -c testsgrams/linked_list/main.c -o build/tests/linked_list.o
gcc ${EXTRA_ARGS} -c testsgrams/iterator/main.c -o build/tests/iterator.o

# link testsgrams
gcc build/cslib.o build/tests/string.o -o build/bin/tests/string_tests
gcc build/cslib.o build/tests/linked_list.o -o build/bin/tests/linked_list_tests
gcc build/cslib.o build/tests/vector.o -o build/bin/tests/vector_tests
gcc build/cslib.o build/tests/hashmap.o -o build/bin/tests/hashmap_tests
gcc build/cslib.o build/tests/iterator.o -o build/bin/tests/iterator_tests
