#include <immintrin.h>
#include <cassert>
#include <cstring>
#include <iostream>
#include "util.h"

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

  __m256 MAX0 = _mm256_set1_ps(0.0f);
  __m256 MAX1 = _mm256_set1_ps(0.0f);
  __m256 MAX2 = _mm256_set1_ps(0.0f);
  __m256 MAX3 = _mm256_set1_ps(0.0f);

  __m256i ABS_MASKi = _mm256_set1_epi32(0x7fffffff);
  __m256 ABS_MASK;
  memcpy(&ABS_MASK, &ABS_MASKi, 32);

  for (; V < VEnd; V += 8) {
    __m256 M0 = _mm256_fmadd_ps(S, *V, T);
    __m256 M1 = _mm256_fmadd_ps(S, *(V + 1), T);

    __m256 M2 = _mm256_fmadd_ps(S, *(V + 2), T);
    __m256 M3 = _mm256_fmadd_ps(S, *(V + 3), T);

    __m256 M4 = _mm256_fmadd_ps(S, *(V + 4), T);
    __m256 M5 = _mm256_fmadd_ps(S, *(V + 5), T);

    __m256 M6 = _mm256_fmadd_ps(S, *(V + 6), T);
    __m256 M7 = _mm256_fmadd_ps(S, *(V + 7), T);

    __m256 REDUCED0 = _mm256_hadd_ps(M0, M1);
    __m256 REDUCED1 = _mm256_hadd_ps(M2, M3);
    __m256 REDUCED2 = _mm256_hadd_ps(M4, M5);
    __m256 REDUCED3 = _mm256_hadd_ps(M6, M7);

    __m256 ABS0 = _mm256_and_ps(REDUCED0, ABS_MASK);
    MAX0 = _mm256_max_ps(MAX0, ABS0);

    __m256 ABS1 = _mm256_and_ps(REDUCED1, ABS_MASK);
    MAX1 = _mm256_max_ps(MAX1, ABS1);

    __m256 ABS2 = _mm256_and_ps(REDUCED2, ABS_MASK);
    MAX2 = _mm256_max_ps(MAX2, ABS2);

    __m256 ABS3 = _mm256_and_ps(REDUCED3, ABS_MASK);
    MAX3 = _mm256_max_ps(MAX3, ABS3);
  }

  __m256 MAX = MAX0;
  MAX = _mm256_max_ps(MAX, MAX1);
  MAX = _mm256_max_ps(MAX, MAX2);
  MAX = _mm256_max_ps(MAX, MAX3);

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
