// Copyright 2024 Vinokurov Ivan
#include <gtest/gtest.h>

#include <vector>

#include "core/perf/include/perf.hpp"
#include "stl/vinokurov_i_simpson_method/include/ops_stl.hpp"

using namespace vinokurovIvanSTL;

TEST(vinokurov_i_simpson_method_stl, test_pipeline_run) {
  const int a = 0;
  const int b = 1;
  const int c = 0;
  const int d = 1;
  const int n = 2000;

  const double result = 4.5;

  func fn = fn_linear;

  // Create data
  std::vector<int> in{a, b, c, d, n};
  std::vector<double> out(1, result);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSTL = std::make_shared<ppc::core::TaskData>();
  taskDataSTL->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSTL->inputs_count.emplace_back(in.size());
  taskDataSTL->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSTL->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskSTL = std::make_shared<SimpsonMethodSTL>(taskDataSTL, fn);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskSTL);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_LE(result - out[0], 0.01);
}

TEST(vinokurov_i_simpson_method_stl, test_task_run) {
  const int a = 0;
  const int b = 1;
  const int c = 0;
  const int d = 1;
  const int n = 2000;

  double result = 4.5;

  func fn = fn_linear;

  // Create data
  std::vector<int> in{a, b, c, d, n};
  std::vector<double> out(1, result);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSTL = std::make_shared<ppc::core::TaskData>();
  taskDataSTL->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSTL->inputs_count.emplace_back(in.size());
  taskDataSTL->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataSTL->outputs_count.emplace_back(out.size());

  // Create Task
  auto testTaskSTL = std::make_shared<SimpsonMethodSTL>(taskDataSTL, fn);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskSTL);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_LE(result - out[0], 0.01);
}
