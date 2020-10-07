#!/bin/bash

CC=clang++

echo "===Plain version with no optimization==="
$CC --std=c++17 plain.cpp -o plain
./plain

echo "===Plain version with O1==="
$CC --std=c++17 -O1 plain.cpp -o plain_O1
./plain_O1

echo "===Plain version with O3==="
$CC --std=c++17 -O3 plain.cpp -o plain_O3
./plain_O3

echo "===SIMD version with no optimization==="
$CC --std=c++17 -mavx -mfma simd.cpp -o simd
./simd

echo "===SIMD version with O3==="
$CC --std=c++17 -O3 -mavx2 -mfma simd.cpp -o simd_O3
./simd_O3
