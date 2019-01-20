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
#include "src/base/class_register.h"
#include "src/solver/hyper_parameter.h"

#include <vector>

namespace xforest {

// Find maximal and minimal value for each feature
struct MaxMin {
  real_t gap = 0.0;
  real_t max_feat = kFloatMin;
  real_t min_feat = kFloatMax;
};

class Histogram;
class DTNode;

// Tmp information during training
class TInfo {
 public:
  // ctor and dctor
  TInfo(const index_t num_feat,
        const index_t num_bin,
        const uint8 num_class) {}
  ~TInfo() {}
  // left or right
  char l_or_r;
  // node layer
  uint8 level = 1;
  // start position
  index_t start_pos = 0;
  // end position
  index_t end_pos = 0;
  // mid position
  index_t mid_pos = 0;
  // best gini value
  real_t best_gini = 1.0;
  // parent node
  DTNode* parent = nullptr;
  // Brother node
  DTNode* brother = nullptr;
  // Histogram bin
  Histogram* histo = nullptr;
 private:
  DISALLOW_COPY_AND_ASSIGN(TInfo);
};

// Decision tree node
class DTNode {
 public:
  // ctor and dctor
  DTNode() {}
  ~DTNode() {}
  // If current node is a leaf node?
  bool is_leaf = false;
  // leaf node value
  real_t leaf_val = -1.0;
  // left child
  DTNode* l_child = nullptr;
  // right child
  DTNode* r_child = nullptr;
  // best feature id
  index_t best_feat_id = 0;
  // best bin value
  uint8 best_bin_val = 0;
  // Tmp info used by training
  TInfo* info = nullptr;
  // Initialize
  inline void Init(const index_t num_feat,
                   const index_t num_bin,
                   const uint8 num_class) {
    info = new TInfo(num_feat, num_bin, num_class);
  }
  // Clear TInfo
  inline void Clear() { 
    delete info;
  }
  // Clear parent info
  inline void ClearParent() {
    delete info->parent->info;
  }
  // Is a leaf node?
  inline bool IsLeaf() {
    return is_leaf;
  }
  inline void SetLeaf() {
    is_leaf = true;
  }
  // value of leaf
  inline real_t LeafVal() {
    return leaf_val;
  }
  inline void SetLeafVal(real_t val) {
    leaf_val = val;
  }
  // Left child
  inline DTNode* LeftChild() {
    return l_child;
  }
  inline void SetLeftChild(DTNode* node) {
    l_child = node;
  }
  // Right child
  inline DTNode* RightChild() {
    return r_child;
  }
  inline void SetRightChild(DTNode* node) {
    r_child = node;
  }
  // Best feature ID
  inline index_t BestFeatID() {
    return best_feat_id;
  }
  inline void SetBestFeatID(index_t id) {
    best_feat_id = id;
  }
  // Best bin value
  inline uint8 BestBinVal() {
    return best_bin_val;
  }
  inline void SetBestBinVal(uint8 val) {
    best_bin_val = val;
  }
  // Left or Right
  inline char LeftOrRight() {
    return info->l_or_r;
  }
  inline void SetLeftOrRight(char ch) {
    info->l_or_r = ch;
  }
  // Node level
  inline uint8 Level() {
    return info->level;
  }
  inline void SetLevel(uint8 level) {
    info->level = level;
  }
  // Start position
  inline index_t StartPos() {
    return info->start_pos;
  }
  inline void SetStartPos(index_t pos) {
    info->start_pos = pos;
  }
  // End position
  inline index_t EndPos() {
    return info->end_pos;
  }
  inline void SetEndPos(index_t pos) {
    info->end_pos = pos;
  }
  // Mid position (split pos)
  inline index_t MidPos() {
    return info->mid_pos;
  }
  inline void SetMidPos(index_t pos) {
    info->mid_pos = pos;
  }
  // Best gini
  inline real_t BestGini() {
    return info->best_gini;
  }
  inline void SetBestGini(real_t gini) {
    info->best_gini = gini;
  }
  // Parent node
  inline DTNode* Parent() {
    return info->parent;
  }
  inline void SetParent(DTNode* node) {
    info->parent = node;
  }
  // Brother node
  inline DTNode* Brother() {
    return info->brother;
  }
  inline void SetBrother(DTNode* node) {
    info->brother = node;
  }
  // Histogram bin
  inline Histogram* Histo() {
    return info->histo;
  }
  inline void SetHisto(Histogram* histo) {
    info->histo = histo;
  }
  // Data size
  inline index_t DataSize() {
    return info->end_pos-info->start_pos+1;
  }
};

//------------------------------------------------------------------------------
// The DTree class is an abstract class, which will be implemented
// by real decision tree, such as BTree (for binary classification), 
// MCTree (for multi-class classificaition), and RTree (for regression).
//------------------------------------------------------------------------------
class DTree {
 public:
  // ctor and dctor
  explicit DTree(uint8* X, real_t* Y,
                 const uint8 num_class,
                 const index_t num_feat, 
                 const index_t data_size,
                 const HyperParam& hyper_param) {
    CHECK_NOTNULL(X);
    CHECK_NOTNULL(Y);
    CHECK_GE(num_class, 2);
    CHECK_LE(num_class, 255);
    CHECK_GT(num_feat, 0);
    CHECK_GT(data_size, 0);
    CHECK_GT(hyper_param.max_bin, 10);
    CHECK_LE(hyper_param.max_bin, 255);
    CHECK_GT(hyper_param.max_depth, 1);
    CHECK_LE(hyper_param.max_depth, 255);
    CHECK_GE(hyper_param.min_samples_split, 2);
    CHECK_GE(hyper_param.min_samples_leaf, 1);
    CHECK_GE(hyper_param.max_leaf_nodes, 2);
    X_ = X;
    Y_ = Y;
    num_class_ = num_class;
    num_feat_ = num_feat;
    data_size_ = data_size;
    max_bin_ = hyper_param.max_bin;
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

  // Build decision tree
  void BuildTree();

  // Given data x, predict y 
  real_t Predict(const uint8* x);

  // Serilize tree to string
  void Serilize(std::string* str);

  // Deserilize tree from string
  void Deserilize(const std::string& str);

  // Print decision to human-readable txt format
  void PrintToTXT(std::string* str);

 private:
  uint8 max_bin_;               // Maximal histogram bin value
  uint8 max_depth_;             // Maximal depth to grow a tree (< 256)
  index_t min_samples_split_;   // Minimal samples to split a node
  index_t min_samples_leaf_;    // Minimal samples in a leaf node
  index_t max_leaf_;            // Maximal number of leaf nodes
  real_t min_impurity_dec_;     // Minimal impurity decrease to split a node
  real_t min_impurity_;         // Minimal impurity to split a node

  std::vector<index_t> rowIdx_;   // data sample
  std::vector<index_t> colIdx_;   // feature sample

  DTNode* root_ = nullptr;   // root node
  index_t leaf_size_ = 1;    // number of leaf nodes
  uint8 tree_depth_ = 1;     // tree depth

  uint8 num_class_ = 0;    // Number of classification
  index_t num_feat_;       // Number of feature
  index_t data_size_ = 0;  // Total data size for training data

  uint8* X_ = nullptr;    // Training data X
  real_t* Y_ = nullptr;   // Label y 

  // Get leaf value
  virtual real_t LeafVal(const DTNode* node) = 0;

  // If current node is a leaf node
  bool IsLeaf(DTNode* node);

  // Find best split position for current node
  virtual void FindPosition(DTNode* node) = 0;

  // Split current node
  void SplitData(DTNode* node);

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

//------------------------------------------------------------------------------
// Class register
//------------------------------------------------------------------------------
CLASS_REGISTER_DEFINE_REGISTRY(xforest_dtree_registry, DTree);

#define REGISTER_DTREE(format_name, rdtree_name)            \
  CLASS_REGISTER_OBJECT_CREATOR(                            \
      xforest_dtree_registry,                               \
      DTree,                                                \
      format_name,                                          \
      dtree_name)

#define CREATE_DTREE(format_name)                           \
  CLASS_REGISTER_CREATE_OBJECT(                             \
      xforest_dtree_registry,                               \
      format_name)


}  // xforest

#endif  // XFOREST_TREE_DTREE_H_