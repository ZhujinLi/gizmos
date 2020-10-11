#!/bin/bash

CC="clang++ --std=c++17 -Wall"

echo "===Scalar version with no optimization==="
$CC scalar.cpp -o scalar
./scalar

echo "===Scalar version with O1==="
$CC -O1 scalar.cpp -o scalar_O1
./scalar_O1

echo "===Scalar version with O3==="
$CC -O3 scalar.cpp -o scalar_O3
./scalar_O3

echo "===SIMD version with no optimization==="
$CC -mavx -mfma simd.cpp -o simd
./simd

echo "===SIMD version with O3==="
$CC -O3 -mavx2 -mfma simd.cpp -o simd_O3
./simd_O3

echo "===SIMD version with O3 + 4 slots==="
$CC -O3 -mavx2 -mfma simd_4s.cpp -o simd_4s_O3
./simd_4s_O3
