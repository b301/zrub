#!/bin/bash

set -x
set -e

./build/bin/tests/string_tests
./build/bin/tests/linked_list_tests
./build/bin/tests/vector_tests
./build/bin/tests/hashmap_tests
./build/bin/tests/iterator_tests
