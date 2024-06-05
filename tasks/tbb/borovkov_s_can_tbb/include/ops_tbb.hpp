// Copyright 2024 Borovkov Sergey
#pragma once

#include <cmath>
#include <random>
#include <string>
#include <vector>

#include "core/task/include/task.hpp"

namespace BorovkovTbb {
bool isEqual(double valueOne, double valueTwo, double eps = 0.01);

bool validateMatrix(size_t sizeOne, size_t sizeTwo);

+std::vector<double> CannonMatrixMultSeq(const std::vector<double> &matrOne, const std::vector<double> &matrTwo,
                                         int size, int block);

+std::vector<double> CannonMatrixMultTbb(const std::vector<double> &matrOne, const std::vector<double> &matrTwo,
                                         int size, int block);

+std::vector<double> getRandomSquareMatrix(size_t size, double minVal, double maxVal);

class BorovkovCanMultMatrTbb : public ppc::core::Task {
 public:
+  explicit BorovkovCanMultMatrTbb(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<double> mMatrOne;
  std::vector<double> mMatrTwo;
  std::vector<double> mMatrRes;
  size_t mSize;
  size_t mBlock;
};
}  // namespace BorovkovTbb
