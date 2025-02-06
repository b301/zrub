#!/bin/bash

set -x
set -e

[[ ! -e build ]] && mkdir -p build
[[ ! -e build/tests ]] && mkdir -p build/tests
[[ ! -e build/bin/tests ]] && mkdir -p build/bin/tests

EXTRA_ARGS="-I${PWD} -Wall -s"

# compile testsgrams
gcc ${EXTRA_ARGS} -c testsgrams/vector/main.c -o build/tests/vector.o
gcc ${EXTRA_ARGS} -c testsgrams/string/main.c -o build/tests/string.o
gcc ${EXTRA_ARGS} -c testsgrams/hashmap/main.c -o build/tests/hashmap.o
gcc ${EXTRA_ARGS} -c testsgrams/linked_list/main.c -o build/tests/linked_list.o
gcc ${EXTRA_ARGS} -c testsgrams/queue/main.c -o build/tests/queue.o
gcc ${EXTRA_ARGS} -c testsgrams/iterator/main.c -o build/tests/iterator.o
gcc ${EXTRA_ARGS} -c testsgrams/os/file.c -o build/tests/file.o
gcc ${EXTRA_ARGS} -c testsgrams/os/time.c -o build/tests/time.o

# link testsgrams
gcc build/zrublib.o build/tests/string.o -o build/bin/tests/string_tests
gcc build/zrublib.o build/tests/linked_list.o -o build/bin/tests/linked_list_tests
gcc build/zrublib.o build/tests/vector.o -o build/bin/tests/vector_tests
gcc build/zrublib.o build/tests/hashmap.o -o build/bin/tests/hashmap_tests
gcc build/zrublib.o build/tests/queue.o -o build/bin/tests/queue_tests
gcc build/zrublib.o build/tests/iterator.o -o build/bin/tests/iterator_tests
gcc build/zrublib.o build/tests/file.o -o build/bin/tests/file_tests
gcc build/zrublib.o build/tests/time.o -o build/bin/tests/time_tests

