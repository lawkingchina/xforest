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

// Data block for feature
struct Feat {
  // Feature value
  real_t value;
  // Label y
  real_t y;
  // Original idx before sorting
  size_t ori_idx;
};

// We need to sort data matrix before training, and the
// cmp(a, b) function will be used in sorting.
static bool cmp(const Feat& feat_a, const Feat& feat_b) {
  return feat_a.value < feat_b.value;
}

// Data matrix used in decision tree
typedef std::vector<std::vector<Feat> > DMatrix;

// Gini statistics for classfication
struct ctree_gini_stats {
  // Gini statistics for left child
  std::vector<index_t> left;
  // Gini statistic for right child
  std::vector<index_t> right;
};

// Information in decision stumb (Node)
struct DTNode {
  // Layer of the tree
  size_t level;
  // If it is leaf node
  bool is_leaf = false;
  // Value in leaf
  real_t leaf_val;
  // Best gini value (1.0 is maximal)
  real_t best_gini = 1.0;
  // Best feature
  size_t best_feat_id;
  // Best value on best feature
  real_t best_feat_val;
  // Best row index for split
  size_t best_row_idx;
  // Ptr for left child
  struct DTNode* l_node = nullptr;
  // Ptr for right child
  struct DTNode* r_node = nullptr;
  // Ptr to the data matrix
  DMatrix* data = nullptr;
  // Start position of data matrix
  size_t start_pos;
  // End position of data matrix
  size_t end_pos;
};

//------------------------------------------------------------------------------
// The Deicision Tree class is an abstract class, which will be implemented
// by real decision tree, such as CTree (Classification Tree) and RTree 
// (Regression Tree).
//------------------------------------------------------------------------------
class DTree {
 public:
  // ctor and dctor
  DTree() {}
  ~DTree() {}

 private:
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