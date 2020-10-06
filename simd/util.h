#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

static int POINT_NUMBER = 500000000;

struct FVec2 {
  float x;
  float y;
};

inline FVec2 *genPoints() {
  FVec2 *points = reinterpret_cast<FVec2 *>(
      aligned_alloc(32, POINT_NUMBER * sizeof(FVec2)));
  FVec2 *p = points;
  *(p++) = FVec2{0, 0};
  for (int i = 1; i < POINT_NUMBER - 1; i++) {
    *(p++) = FVec2{float(rand()) / RAND_MAX, float(rand()) / RAND_MAX};
  }
  *(p++) = FVec2{1, 1};
  return points;
}

inline void freePoints(FVec2 *points) { free(points); }

void tictoc() {
  static bool flag;
  static std::chrono::time_point<std::chrono::high_resolution_clock> time;

  if (!flag) // tic
  {
    std::cout << "Tic..." << std::endl;
    time = std::chrono::high_resolution_clock::now();
  } else // toc
  {
    auto elapsed = std::chrono::high_resolution_clock::now() - time;
    std::cout << "Elapsed time: " << elapsed.count() / 1e9 << "s" << std::endl;
  }

  flag = !flag;
}
