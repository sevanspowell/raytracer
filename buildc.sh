#!/bin/bash

cmake -E make_directory build
cmake -E chdir build cmake -E time cmake -D CMAKE_C_COMPILER=/usr/local/bin/gcc-6 -D CMAKE_CXX_COMPILER=/usr/local/bin/g++-6 ../src
cmake -E time cmake --build build --target all --config Debug --clean-first
