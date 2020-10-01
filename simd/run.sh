#!/bin/bash

echo "===Plain version==="
g++-9 --std=c++17 plain.cpp
./a.out

echo "===Plain version with O3==="
g++-9 --std=c++17 -O3 plain.cpp
./a.out

echo "===SIMD version with O3==="
g++-9 --std=c++17 -O3 -mavx2 -mfma simd.cpp
./a.out
