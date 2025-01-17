#!/bin/bash

set -e
set -x

./build/bin/tests/string_tests
./build/bin/tests/linked_list_tests
./build/bin/tests/vector_tests
./build/bin/tests/hashmap_tests
./build/bin/tests/queue_tests
./build/bin/tests/iterator_tests
./build/bin/tests/file_tests
./build/bin/tests/time_tests

echo "[+] ALL TESTS PASSED! [+]"
