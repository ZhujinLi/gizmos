#include "util.h"
#include <cassert>
#include <cstring>
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

  __m256 *V = (__m256 *)points;
  __m256 *VEnd = (__m256 *)(points + n);

  __m256 MAX = _mm256_set1_ps(0.0f);

  __m256i ABS_MASKi = _mm256_set1_epi32(0x7fffffff);
  __m256 ABS_MASK;
  memcpy(&ABS_MASK, &ABS_MASKi, 32);

  for (; V < VEnd; V += 2) {
    __m256 M0 = _mm256_fmadd_ps(S, *V, T);
    __m256 M1 = _mm256_fmadd_ps(S, *(V + 1), T);

    MAX = _mm256_max_ps(MAX, _mm256_and_ps(_mm256_hadd_ps(M0, M1), ABS_MASK));
  }

  float maxDist = *reinterpret_cast<float *>(&MAX);
  maxDist = std::max(maxDist, *(reinterpret_cast<float *>(&MAX) + 1));
  maxDist = std::max(maxDist, *(reinterpret_cast<float *>(&MAX) + 2));
  maxDist = std::max(maxDist, *(reinterpret_cast<float *>(&MAX) + 3));
  maxDist = std::max(maxDist, *(reinterpret_cast<float *>(&MAX) + 4));
  maxDist = std::max(maxDist, *(reinterpret_cast<float *>(&MAX) + 5));
  maxDist = std::max(maxDist, *(reinterpret_cast<float *>(&MAX) + 6));
  maxDist = std::max(maxDist, *(reinterpret_cast<float *>(&MAX) + 7));
  return maxDist;
}

int main() {
  FVec2 *points = genPoints();
  float res = 0;

  res += findMaxDist(points, POINT_NUMBER);

  tictoc();
  res += findMaxDist(points, POINT_NUMBER);
  tictoc();

  res *= 0.5f;
  std::cout << "Result: " << res << std::endl;

  freePoints(points);
  return 0;
}
