// Copyright 2024 Kiselev Igor
#include "tbb/kiselev_i_shell_simple/include/shell_simple.hpp"

#include <memory>

#include "tbb/parallel_for.h"

using namespace std::chrono_literals;

using namespace std;
using namespace Kiselev_tbb;

bool KiselevTaskTBB::pre_processing() {
  try {
    internal_order_test();
    size_t n = taskData->inputs_count[0];
    arr = ::std::vector<int>(n, 0);
    for (size_t i = 0; i < n; ++i) {
      int *elem = reinterpret_cast<int *>(taskData->inputs[0] + i * sizeof(int));
      arr[i] = *elem;
    }
  } catch (...) {
    return false;
  }
  return true;
}

bool KiselevTaskTBB::validation() {
  try {
    internal_order_test();
    return taskData->inputs_count[0] != 0 && taskData->inputs_count[0] == taskData->outputs_count[0];
  } catch (...) {
    return false;
  }
}

bool KiselevTaskTBB::run() {
  try {
    internal_order_test();
    int n = (int)arr.size();
    FindThreadVariables();
    //  if (ThreadNum == 0) return false;
    int *Index = new int[ThreadNum + 1];
    int *BlockSize = new int[ThreadNum];
    int *BlockIndices = new int[ThreadNum];
    for (int i = 0; i <= ThreadNum; i++) {
      Index[i] = (i * n) / ThreadNum;
    }
    for (int i = 0; i < ThreadNum; i++) {
      BlockSize[i] = Index[i + 1] - Index[i];
      BlockIndices[i] = i;
    }
    tbb::parallel_for(tbb::blocked_range<int>(0, ThreadNum), [&](const tbb::blocked_range<int> &range) {
      for (int i = range.begin(); i < range.end(); i++) {
        SeqSorter(Index[i], Index[i] + BlockSize[i], arr);
      }
    });
    for (int i = 1; i < ThreadNum; i *= 2) {
      tbb::parallel_for(tbb::blocked_range<int>(0, ThreadNum), [&](const tbb::blocked_range<int> &range) {
        for (int j = range.begin(); j < range.end(); j += 2 * i) {
          int left = BlockIndices[j];
          int right = (j + i < ThreadNum) ? BlockIndices[j + i] : -1;
          if (right != -1) {
            MergeBlocks(Index[left], BlockSize[left], Index[right], BlockSize[right], arr);
            BlockIndices[j] = left;
            BlockSize[j] = BlockSize[left] + BlockSize[right];
          } else {
            BlockIndices[j] = left;
            BlockSize[j] = BlockSize[left];
          }
        }
      });
    }
    delete[] Index;
    delete[] BlockSize;
    delete[] BlockIndices;
    return true;
  } catch (...) {
    return false;
  }
  return true;
}

bool KiselevTaskTBB::post_processing() {
  try {
    internal_order_test();
    size_t n = arr.size();
    for (size_t i = 0; i < n; i++) {
      int *res = reinterpret_cast<int *>(taskData->outputs[0] + i * sizeof(int));
      *res = arr[i];
      (void)res;
    }
    return true;
  } catch (...) {
    return false;
  }
}
// Can do better
void KiselevTaskTBB::MergeBlocks(int Index1, int BlockSize1, int Index2, int BlockSize2, std::vector<int> &array) {
  int *pTempArray = new int[BlockSize1 + BlockSize2];
  for (int i = BlockSize1 + BlockSize2 - 1; i >= 0; i--) {
    pTempArray[i] = 0;
  }
  int i1 = Index1;
  int i2 = Index2;
  int curr = 0;
  if (BlockSize1 == 0) {
    for (int i = 0; i < BlockSize2; i++) {
      pTempArray[curr++] = array[i2++];
    }
  } else if (BlockSize2 == 0) {
    for (int i = 0; i < BlockSize1; i++) {
      pTempArray[curr++] = array[i1++];
    }
  } else {
    while (i1 < Index1 + BlockSize1 && i2 < Index2 + BlockSize2) {
      if (array[i1] < array[i2])
        pTempArray[curr++] = array[i1++];
      else
        pTempArray[curr++] = array[i2++];
    }
    while (i1 < Index1 + BlockSize1) pTempArray[curr++] = array[i1++];
    while (i2 < Index2 + BlockSize2) pTempArray[curr++] = array[i2++];
  }
  for (int i = 0; i < BlockSize1 + BlockSize2; i++) array[Index1 + i] = pTempArray[i];
  delete[] pTempArray;
}

bool KiselevTaskTBB::IsSorted() {
  int n = arr.size();
  for (int i = 1; i < n; i++) {
    if (arr[i - 1] > arr[i]) return false;
  }
  return true;
}

int KiselevTaskTBB::exp(int arg, int exp) {
  int res = arg;
  for (int i = 1; i < exp; i++) {
    res *= arg;
  }
  return res;
}

void KiselevTaskTBB::FindThreadVariables() {
  ThreadNum = 16;
  int helper = 1;
  DimSize = 1;
  while (ThreadNum >= helper) {
    DimSize++;
    helper = helper << 1;
  }
}

void KiselevTaskTBB::SeqSorter(int start, int end, std::vector<int> &array) {
  int n = end - start;
  for (int i = 1; i < n; i++) {
    int j = i - 1;
    while (j >= 0 && array[j + start] > array[j + 1 + start]) {
      array[j + start] += array[j + 1 + start];
      array[j + 1 + start] = array[j + start] - array[j + 1 + start];
      array[j + start] = array[j + start] - array[j + 1 + start];
      j--;
    }
  }
}
