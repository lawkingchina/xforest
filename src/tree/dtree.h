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

#include <queue>
#include <vector>

namespace xforest {

//------------------------------------------------------------------------------
// The DTree class is an abstract class, which will be implemented
// by real decision tree, such as BTree (for binary classification), 
// MCTree (for multi-class classificaition), and RTree (for regression).
//------------------------------------------------------------------------------
class DTree {
 public:
  // ctor and dctor
  explicit DTree(const HyperParam& hyper_param) {
    CHECK_GT(hyper_param.max_depth, 1);
    CHECK_LE(hyper_param.max_depth, 255);
    CHECK_GE(hyper_param.min_samples_split, 2);
    CHECK_GE(hyper_param.min_samples_leaf, 1);
    CHECK_GE(hyper_param.max_leaf_nodes, 2);
    max_depth_ = hyper_param.max_depth;
    min_samples_split_ = hyper_param.min_samples_split;
    min_samples_leaf_ = hyper_param.min_samples_leaf;
    max_leaf_ = hyper_param.max_leaf_nodes;
    min_impurity_dec_ = hyper_param.min_impurity_decrease;
    min_impurity_ = hyper_param.min_impurity_split;
  }
  ~DTree() { }

  // Sample for training data
  void SetRowIdx(const std::vector<index_t>& idx) {
    CHECK_EQ(idx.empty(), false);
    rowIdx_.assign(idx.begin(), idx.end());
  }

  // Sample for feature
  void SetColIdx(const std::vector<index_t>& idx) {
    CHECK_EQ(idx.empty(), false);
    colIdx_.assign(idx.begin(), idx.end());
  }

 private:
  uint8 max_depth_;  // Maximal depth to grow a tree
  index_t min_samples_split_;  // Minimal samples to split a node
  index_t min_samples_leaf_;  // Minimal samples in a leaf node
  index_t max_leaf_;  // Maximal number of leaf nodes
  real_t min_impurity_dec_;  // Minimal impurity decrease to split a node
  real_t min_impurity_;  // Minimal impurity to split a node

  std::vector<index_t> rowIdx_;  // Data sample
  std::vector<index_t> colIdx_;  // Feature sample

  DISALLOW_COPY_AND_ASSIGN(DTree);
};

// Binary Tree
class BTree : public DTree {

};

// Multi-class Tree
class MCTree : public DTree {

};

// Regression Tree
class RTree : public DTree {

};

}  // xforest

#endif  // XFOREST_TREE_DTREE_H_