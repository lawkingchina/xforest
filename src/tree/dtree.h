//------------------------------------------------------------------------------
// Copyright (c) 2018 by contributors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//------------------------------------------------------------------------------

/*
This file defines the DTree class.
*/

#ifndef XFOREST_TREE_DTREE_H_
#define XFOREST_TREE_DTREE_H_

#include "src/base/common.h"
#include "src/base/thread_pool.h"

#include <queue>
#include <vector>
#include <algorithm>

namespace xforest {

typedef float real_t;
typedef uint32 index_t;

//--------------------------------------------------------
// Find max and min value in every column data
//--------------------------------------------------------
struct MaxMin {
  real_t gap = 0.0;
  real_t max_feat = kFloatMin;
  real_t min_feat = kFloatMax;
};

//--------------------------------------------------------
// Histogram data structure
//--------------------------------------------------------

struct Histogram {
  ~Histogram() {
    for (size_t i = 0; i < bin_count.size(); ++i) {
      if (bin_count[i] != nullptr) {
        delete [] bin_count[i];
      }
    }
  }
  uint32 total_0 = 0;
  uint32 total_1 = 0;
  std::vector<Count*> bin_count;
};

struct DTNode;  // Decision tree node

//---------------------------------------------------
// Tmp info during training
//---------------------------------------------------
struct TInfo {
  ~TInfo() {
    delete histo;
  }
  // left or right
  char l_or_r;
  // node layer
  uint32 level = 1;
  // start postion
  uint32 start_pos = 0;
  // end position
  uint32 end_pos = 0;
  // split position
  uint32 mid_pos = 0;
  // Best gini value
  real_t best_gini = 1.0;
  // Parent node
  DTNode* parent = nullptr;
  // Brother node
  DTNode* brother = nullptr;
  // Histogram
  Histogram* histo = nullptr;
  // Init histogram
  void InitHistoGram(const uint32 col_size, const uint8 max_bin) {
    histo->bin_count.resize(col_size);
    for (uint32_t i = 0; i < col_size; ++i) {
      histo->bin_count[i] = new Count[max_bin+1];
      for (uint32_t j = 0; j <= max_bin; ++j) {
        histo->bin_count[i][j].count_0 = 0;
        histo->bin_count[i][j].count_1 = 0;
      }
    }
    histo->total_1 = 0;
    histo->total_0 = 0;
  }
};

//------------------------------------------------------------------------------
// The Deicision Tree class is an abstract class, which will be implemented
// by real decision tree, such as CTree (Classification Tree) and RTree 
// (Regression Tree).
//------------------------------------------------------------------------------
class DTree {
 public:
  // ctor and dctor
  explicit DTree(const HyperParam& hyper_param) {
    CHECK_GT(hyper_param.row_size, 0);
    CHECK_GT(hyper_param.col_size, 0);
    CHECK_GT(hyper_param.max_bin, 0);
    CHECK_LT(hyper_param.max_bin, 256);
    CHECK_GT(hyper_param.max_depth, 0);
    CHECK_LT(hyper_param.max_depth, 256);
    CHECK_GT(hyper_param.max_leaf, 0);
    CHECK_GE(hyper_param.min_gini, 0);
    CHECK_GT(hyper_param.min_split, 1);
    CHECK_GT(hyper_param.col_sample, 0);
    CHECK_LE(hyper_param.col_sample, 
             hyper_param.col_size);
    row_size_ = hyper_param.row_size;
    col_size_ = hyper_param.col_size;
    max_bin_ = hyper_param.max_bin;
    max_depth_ = hyper_param.max_depth;
    max_leaf_ = hyper_param.max_leaf;
    min_gini_ = hyper_param.min_gini;
    min_split_ = hyper_param.min_split;
    bootstrap_ = hyper_param.bootstrap;
    col_sample_ = hyper_param.col_sample;
  }
  ~DTree() { }

 private:
  // The number of data
  uint32 row_size_;
  // The number of feature
  uint32 col_size_;
  // The max index of histogram bin value
  uint8 max_bin_;
  // The max depth of tree
  uint8 max_depth_;
  // The max number of leaves in tree
  uint16 max_leaf_;
  // The minimal gini val in node to split
  real_t min_gini_;
  // The minimal examples in node to split
  uint16 min_split_;
  // If using bootstrap
  bool bootstrap_ ;
  // The number of feature to use
  uint32 col_sample_;

  DISALLOW_COPY_AND_ASSIGN(DTree);
};

// Classification Tree
class CTree : public DTree {

};

// Regression Tree
class RTree : public DTree {

}

}  // xforest

#endif  // XFOREST_TREE_DTREE_H_