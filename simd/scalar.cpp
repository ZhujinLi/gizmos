#include "util.h"
#include <iostream>

float findMaxDist(const FVec2 *points, size_t n) {
  FVec2 p = points[0];
  FVec2 q = points[n - 1];

  FVec2 pq = {q.x - p.x, q.y - p.y};
  float pqLenInv = 1.f / sqrtf(pq.x * pq.x + pq.y * pq.y);

  float maxDist = 0;
  for (size_t i = 0; i < n; i++) {
    FVec2 r = points[i];
    FVec2 pr = {r.x - p.x, r.y - p.y};
    float dist = fabsf(pq.x * pr.y - pq.y * pr.x) * pqLenInv;
    maxDist = std::max(maxDist, dist);
  }

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
