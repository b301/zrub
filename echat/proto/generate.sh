#!/bin/bash


for file in $(ls *.proto)
do
    nanopb_generator ${file}
done

