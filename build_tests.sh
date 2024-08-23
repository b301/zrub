#!/bin/bash


[[ ! -e tests ]] && mkdir -p tests
[[ ! -e build ]] && mkdir -p build
[[ ! -e build/tests ]] && mkdir -p build/tests

EXTRA_ARGS="-I${PWD} -s"

# string test 1
gcc ${EXTRA_ARGS} -c cslib/std/string.c -o build/string.o
gcc ${EXTRA_ARGS} -c testsgrams/string/main.c -o build/tests/string.o

gcc build/string.o build/tests/string.o -o tests/string_test_1


# vector test 1
gcc ${EXTRA_ARGS} -c cslib/std/string.c -o build/string.o
gcc ${EXTRA_ARGS} -c cslib/std/vector.c -o build/vector.o
gcc ${EXTRA_ARGS} -c testsgrams/vector/main.c -o build/tests/vector.o

gcc build/vector.o build/string.o build/tests/vector.o -o tests/vector_test_1


# linekd list 1
gcc ${EXTRA_ARGS} -c cslib/std/linked_list.c -o build/linked_list.o
gcc ${EXTRA_ARGS} -c testsgrams/linked_list/main.c -o build/tests/linked_list.o

gcc build/linked_list.o build/tests/linked_list.o -o tests/linked_list_test_1


# hashmap test 1
gcc ${EXTRA_ARGS} -c cslib/std/string.c -o build/string.o
gcc ${EXTRA_ARGS} -c cslib/std/linked_list.c -o build/linked_list.o
gcc ${EXTRA_ARGS} -c cslib/std/hashmap.c -o build/hashmap.o
gcc ${EXTRA_ARGS} -c testsgrams/hashmap/main.c -o build/tests/hashmap.o

gcc build/string.o build/linked_list.o build/hashmap.o build/tests/hashmap.o -o tests/hashmap_test_1
