#include "util.h"
#include <cassert>
#include <immintrin.h>
#include <iostream>

void *g_distBuf;

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
  __m256 *V2 = reinterpret_cast<__m256 *>(g_distBuf);
  __m256 *V0End = (__m256 *)(points + n);

  tictoc();
  for (; V0 < V0End; V0 += 2, V1 += 2, V2++) {
    __m256 T0 = _mm256_fmadd_ps(S, *V0, T);
    __m256 T1 = _mm256_fmadd_ps(S, *V1, T);
    *V2 = _mm256_hadd_ps(T0, T1);
  }
  tictoc();

  tictoc();
  float maxDist = 0;
  float *dists = reinterpret_cast<float *>(g_distBuf);
  for (size_t i = 0; i < n; i++) {
    maxDist = std::max(maxDist, fabsf(dists[i]));
  }
  tictoc();

  return maxDist;
}

int main() {
  FVec2 *points = genPoints();
  g_distBuf = aligned_alloc(32, POINT_NUMBER / 8 * sizeof(__m256));

  float res = 0;
  res += findMaxDist(points, POINT_NUMBER);
  res += findMaxDist(points, POINT_NUMBER);
  res *= 0.5f;
  std::cout << "Result: " << res << std::endl;

  free(g_distBuf);
  freePoints(points);
  return 0;
}
