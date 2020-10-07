#!/bin/bash

CC=clang++

echo "===Scalar version with no optimization==="
$CC --std=c++17 scalar.cpp -o scalar
./scalar

echo "===Scalar version with O1==="
$CC --std=c++17 -O1 scalar.cpp -o scalar_O1
./scalar_O1

echo "===Scalar version with O3==="
$CC --std=c++17 -O3 scalar.cpp -o scalar_O3
./scalar_O3

echo "===SIMD version with no optimization==="
$CC --std=c++17 -mavx -mfma simd.cpp -o simd
./simd

echo "===SIMD version with O3==="
$CC --std=c++17 -O3 -mavx2 -mfma simd.cpp -o simd_O3
./simd_O3

echo "===SIMD version with O3 + 4 accumulators==="
$CC --std=c++17 -O3 -mavx2 -mfma simd_4acc.cpp -o simd_4acc_O3
./simd_4acc_O3
