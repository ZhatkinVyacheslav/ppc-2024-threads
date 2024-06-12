// Copyright 2024 Kutarin Alexander
#include "omp/kutarin_a_image_sobel_operator/include/ops_omp.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

bool KutarinASobel::pre_processing() {
  internal_order_test();
  if (taskData == nullptr) {
    return false;
  }

  try {
    width_ = taskData->inputs_count[0];
    height_ = taskData->inputs_count[1];

    sourceImage.resize(width_ * height_);
    resultImage.resize(width_ * height_);

    int* input_buffer = reinterpret_cast<int*>(taskData->inputs[0]);
    if (input_buffer == nullptr) {
      return false;
    }

    int* output_buffer = reinterpret_cast<int*>(taskData->outputs[0]);
    if (output_buffer == nullptr) {
      return false;
    }

    std::copy(input_buffer, input_buffer + width_ * height_, sourceImage.begin());

  } catch (...) {
    return false;
  }
  return true;
}

bool KutarinASobel::validation() {
  internal_order_test();
  if (!taskData) {
    std::cerr << "Ошибка: taskData является nullptr." << std::endl;
    return false;
  }

  bool valid_input_count = taskData->inputs_count.size() == 2;
  if (!valid_input_count) {
    std::cerr << "Ошибка: Неверное количество входных данных. Ожидается 2, получено " << taskData->inputs_count.size()
              << "." << std::endl;
  }

  bool valid_output_count = taskData->outputs_count.size() == 2;
  if (!valid_output_count) {
    std::cerr << "Ошибка: Неверное количество выходных данных. Ожидается 2, получено " << taskData->outputs_count.size()
              << "." << std::endl;
  }

  return valid_input_count && valid_output_count;
}

bool KutarinASobel::run() {
  internal_order_test();
  try {
    #pragma omp parallel for
    for (int j = 0; j < height_; ++j) {
      for (int i = 0; i < width_; ++i) {
        int resultX = 0;
        int resultY = 0;
        for (int y = -1; y <= 1; ++y) {
          for (int x = -1; x <= 1; ++x) {
            int pixelValue = sourceImage[clamp(j + y, 0, height_ - 1) * width_ + clamp(i + x, 0, width_ - 1)];
            resultX += pixelValue * kernel_x[(y + 1) * 3 + (x + 1)];
            resultY += pixelValue * kernel_y[(y + 1) * 3 + (x + 1)];
          }
        }
        resultImage[j * width_ + i] = clamp(static_cast<int>(sqrt(resultX * resultX + resultY * resultY)), 0, 255);
      }
    }
    return true;
  } catch (...) {
    return false;
  }
}

bool KutarinASobel::post_processing() {
  internal_order_test();
  if (!taskData) {
    return false;
  }

  try {
    taskData->outputs_count[0] = width_;
    taskData->outputs_count[1] = height_;

    int* output_buffer = reinterpret_cast<int*>(taskData->outputs[0]);
    if (output_buffer == nullptr) {
      return false;
    }

    std::copy(resultImage.begin(), resultImage.end(), output_buffer);

  } catch (...) {
    return false;
  }
  return true;
}

void KutarinASobel ::generateSaltAndPepperNoise(std::vector<int>& image, int height, int width, float noise_ratio) {
  int num_noise_pixels = static_cast<int>(height * width * noise_ratio);
  for (int i = 0; i < num_noise_pixels; ++i) {
    int x = rand() % width;
    int y = rand() % height;
    int noise_value = (rand() % 2) * 255;
    image[y * width + x] = noise_value;
  }
}

int KutarinASobel ::clamp(int value, int min, int max) {
  if (value < min) return min;
  if (value > max) return max;
  return value;
}
