#!/bin/bash


gcc -c -I${PWD} doodles/cookie_maker.c -o build/cookie.o

gcc build/cookie.o build/string.o build/hashmap.o build/linked_list.o -o cookie
