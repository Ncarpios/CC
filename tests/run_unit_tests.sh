#!/bin/bash

set -e

clang -Wall unit_tests.c unity.c -o unit_tests
./unit_tests

clang++ -Wall tests_against_stl.cpp -o tests_against_stl
./tests_against_stl
