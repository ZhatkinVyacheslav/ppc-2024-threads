// Copyright 2024 Gontsov Roman

#include <string>
#include <vector>

#include "core/task/include/task.hpp"

class RadixSeqG : public ppc::core::Task {
 public:
  explicit RadixSeqG(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;
  std::vector<int> input_;
};

class RadixTBBG : public ppc::core::Task {
 public:
  explicit RadixTBBG(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<int> input_;
};
std::vector<int> radixSort2(std::vector<int> vector);
std::vector<int> getRandomVector2(int sz);
