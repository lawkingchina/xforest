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

//--------------------------------------------------------
// Find max and min value in every column data
//--------------------------------------------------------
struct MaxMin {
  float gap = 0.0;
  float max_feat = std::numeric_limits<float>::min();
  float min_feat = std::numeric_limits<float>::max();
};

//--------------------------------------------------------
// Histogram data structure
//--------------------------------------------------------
struct Count {
  uint32_t count_0 = 0;
  uint32_t count_1 = 0;
};

struct Histogram {
  ~Histogram() {
    for (size_t i = 0; i < bin_count.size(); ++i) {
      if (bin_count[i] != nullptr) {
        delete [] bin_count[i];
      }
    }
  }
  uint32_t total_0 = 0;
  uint32_t total_1 = 0;
  std::vector<Count*> bin_count;
};

//------------------------------------------------------------------------------
// The Deicision Tree class is an abstract class, which will be implemented
// by real decision tree, such as BCTree (Binary Classification Tree) and 
// MCRTree (Multi-Class Tree) and RTree (Regression Tree).
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

// Binary Classification Tree
class BCTree : public DTree {

};

// Binary Classification Tree
class MCtree : public DTree {

}

// Regression Tree
class RTree : public DTree {

}

}  // xforest

#endif  // XFOREST_TREE_DTREE_H_