#include "util.h"
#include <cassert>
#include <immintrin.h>
#include <iostream>

float findMaxDist(const FVec2 *points, size_t n) {
  assert(n % 8 == 0);                                    // Even groups
  assert(reinterpret_cast<uintptr_t>(points) % 32 == 0); // Memory alignment

  FVec2 p = points[0];
  FVec2 q = points[n - 1];
  FVec2 pq = {q.x - p.x, q.y - p.y};
  float pqLenInv = 1.f / sqrtf(pq.x * pq.x + pq.y * pq.y);

  FVec2 s = {pqLenInv * pq.y, -pqLenInv * pq.x};
  FVec2 t = {-pqLenInv * p.x * pq.y, pqLenInv * p.y * pq.x};

  __m256 S = _mm256_set_ps(s.y, s.x, s.y, s.x, s.y, s.x, s.y, s.x);
  __m256 T = _mm256_set_ps(t.y, t.x, t.y, t.x, t.y, t.x, t.y, t.x);

  __m256 *V0 = (__m256 *)points;
  __m256 *V1 = V0 + 1;
  __m256 *distBuf = new __m256[n / 8];
  __m256 *V2 = distBuf;
  __m256 *V0End = (__m256 *)(points + n);

  tictoc();
  for (; V0 < V0End; V0 += 2, V1 += 2, V2++) {
    __m256 T0 = _mm256_fmadd_ps(S, *V0, T);
    __m256 T1 = _mm256_fmadd_ps(S, *V1, T);
    *V2 = _mm256_hadd_ps(T0, T1);
  }
  tictoc();

  float maxDist = 0;
  float *dists = reinterpret_cast<float *>(distBuf);
  for (size_t i = 0; i < n; i++) {
    maxDist = std::max(maxDist, fabsf(dists[i]));
  }
  delete[] distBuf;
  return maxDist;
}

int main() {
  FVec2 *points = genPoints();
  float res = 0;
  res = findMaxDist(points, POINT_NUMBER);
  freePoints(points);
  return (int)res;
}
