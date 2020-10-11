This is an attempt to optimize a program with Intel AVX2 SIMD intrinsics.

The program is to calculate the furthest point on a polyline, which is the core of Ramer-Douglas-Peucker algorithm.

Here is the result produced with my Intel i7-8850H:

```
===Scalar version with no optimization===
Tic...
Toc. Elapsed time: 0.0105139s
Result: 0.706358
===Scalar version with O1===
Tic...
Toc. Elapsed time: 0.00588918s
Result: 0.706358
===Scalar version with O3===
Tic...
Toc. Elapsed time: 0.00157706s
Result: 0.706358
===SIMD version with no optimization===
Tic...
Toc. Elapsed time: 0.00158742s
Result: 0.706358
===SIMD version with O3===
Tic...
Toc. Elapsed time: 0.000539793s
Result: 0.706358
===SIMD version with O3 + 4 slots===
Tic...
Toc. Elapsed time: 0.000474098s
Result: 0.706358
```

As the result shows, SIMD version is over 3X faster than scalar version.

Here are some of my conclusions:

- SIMD version can easily be slower than scalar version, if SIMD code is written badly, and -O3 is enabled for both.
- The procedure needs to execute at least once before performance test, or the result could be remarkably affected by cache-misses.
- My clang++ compiler performs auto-vectorization with -O3, but fails to do actual optimization; it uses %xmm registers for scalar calculations.
- According to instruction latency provided by Intel official site, I further optimize code with 4 accumulators, but only to waste effort.

