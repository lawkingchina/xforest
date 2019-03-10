//------------------------------------------------------------------------------
// Copyright (c) 2019 by contributors. All Rights Reserved.
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

/*!
*  Copyright (c) 2018 by Contributors
* \file dtree.h
* \brief This file defines the DTree class.
*/
#ifndef XFOREST_TREE_DTREE_H_
#define XFOREST_TREE_DTREE_H_

#include "src/base/common.h"
#include "src/base/class_register.h"
#include "src/solver/hyper_parameter.h"

#include <vector>

namespace xforest {

/*!
 * \brief Find maximal and minimal value for each feature.
 * For Histogram decision tree, we need to map original feature
 * value to 8-bit bin value, hence here we need to make a record
 * of the maximal and minimal value for each feature.
 */
struct MaxMin {
  real_t gap = 0.0;
  real_t max_feat = kFloatMin;
  real_t min_feat = kFloatMax;
};

/*!
 * \brief Temp information during training. 
 * This information will not be used for inference, 
 * and we can delete this structure on-the-fly.
 */
struct TInfo {
  /*!
   * \brief Left node ('l') or right node ('r').
   */
  char l_or_r;
  /*!
   * \brief Level of layer for current node.
   * Note that the maximal level is 255.
   */
  uint8 level = 1;
  /*!
   * \brief Start position (index) of the data 
   * allocated to current node
   */
  index_t start_pos = 0;
  /*!
   * \brief End position (index) of the data
   * allocated to current node
   */
  index_t end_pos = 0;
  /*!
   * \brief Split position (index) of the data
   * allocated to current node
   */
  index_t mid_pos = 0;
  /*!
   * \brief Best gini value we calculated for current node
   */
  real_t best_gini = 1.0;
  /*!
   * \brief Parent node (DTNode) of current node, which will be used
   * for calculating histogram value of current node.
   */
  void* parent = nullptr;
  /*!
   * \brief Brother node (DTNode) of current node, which will be used
   * for calculating histogram value of current node.
   */
  void* brother = nullptr;
  /*!
   * \brief Histigram bin data structure.
   */
  void* histo = nullptr;
};

/*!
 * \brief Decision tree node class, which will maintain
 * the information used in both training and inference.
 */
class DTNode {
 public:
  /*!
   * \brief If current node is a leaf node?
   */ 
  bool is_leaf = false;
  /*!
   * \brief Pointer of the left child node.
   */
  DTNode* l_child = nullptr;
  /*!
   * \brief Pointer of the right child node.
   */
  DTNode* r_child = nullptr;
  /*!
   * \brief Best split feature id of current node.
   */
  index_t best_feat_id = 0;
  /*!
   * \brief Best split histigram bin value ([0, 255]) of current node.
   */
  uint8 best_bin_val = 0;
  /*!
   * \brief Temp information used by training process.
   */
  TInfo* info = nullptr;
  /*!
   * \brief Clear temp information on-the-fly.
   */
  inline void Clear() { 
    delete info;
  }
  /*!
   * \brief Clear parent information when we calculated 
   * current histogram value.
   */
  inline void ClearParent() {
    delete info->parent->info;
  }
  /*!
   * \brief Is current node is a leaf node?
   */
  inline bool IsLeaf() const {
    return is_leaf;
  }
  /*!
   * \brief Set is_leaf to true.
   */
  inline void SetLeaf() {
    is_leaf = true;
  }
  /*!
   * \brief Get left child node.
   */
  inline DTNode* LeftChild() const {
    return l_child;
  }
  /*!
   * \brief Set left child node.
   */
  inline void SetLeftChild(DTNode* node) {
    l_child = node;
  }
  /*!
   * \brief Get right child node.
   */
  inline DTNode* RightChild() const {
    return r_child;
  }
  /*!
   * \brief Set right child node.
   */
  inline void SetRightChild(DTNode* node) {
    r_child = node;
  }
  /*!
   * \brief Get best split feature id.
   */
  inline index_t BestFeatID() const {
    return best_feat_id;
  }
  /*!
   * \brief Set best split feature id.
   */
  inline void SetBestFeatID(index_t id) {
    best_feat_id = id;
  }
  /*!
   * \brief Get best split histogram value.
   */
  inline uint8 BestBinVal() const {
    return best_bin_val;
  }
  /*!
   * \brief Set best split histogram value.
   */
  inline void SetBestBinVal(uint8 val) {
    best_bin_val = val;
  }
  /*!
   * \brief Is current node a left or Right node?
   */
  inline char LeftOrRight() const {
    return info->l_or_r;
  }
  /*!
   * \brief Set 'l' (left) or 'r' (right) to current node.
   */
  inline void SetLeftOrRight(char ch) {
    info->l_or_r = ch;
  }
  /*!
   * \brief Get node level (maximal level is 255).
   */
  inline uint8 Level() const {
    return info->level;
  }
  /*!
   * \brief Set node level (maximal level is 255).
   */
  inline void SetLevel(uint8 level) {
    info->level = level;
  }
  /*!
   * \brief Get start position (index) of data allocated to current node.
   */
  inline index_t StartPos() const {
    return info->start_pos;
  }
  /*!
   * \brief Set start position (index) of data allocated to current node.
   */
  inline void SetStartPos(index_t pos) {
    info->start_pos = pos;
  }
  /*!
   * \brief Get end position (index) of data allocated to cuurent node.
   */
  inline index_t EndPos() const {
    return info->end_pos;
  }
  /*!
   * \brief Set end position (index) of data allocated to current node.
   */
  inline void SetEndPos(index_t pos) {
    info->end_pos = pos;
  }
  /*!
   * \brief Get best split position (index) of data allocated to current node.
   */
  inline index_t MidPos() const {
    return info->mid_pos;
  }
  /*!
   * \brief Set best split pisition (index) of data allocated to current node.
   */
  inline void SetMidPos(index_t pos) {
    info->mid_pos = pos;
  }
  /*!
   * \brief Get best gini value calculated by current node.
   */
  inline real_t BestGini() const {
    return info->best_gini;
  }
  /*!
   * \brief Set best gini value calculated by current node.
   */
  inline void SetBestGini(real_t gini) {
    info->best_gini = gini;
  }
  /*!
   * \brief Get parent node of current node.
   */
  inline DTNode* Parent() const {
    return info->parent;
  }
  /*!
   * \brief Set parent node of current node.
   */
  inline void SetParent(DTNode* node) {
    info->parent = node;
  }
  /*!
   * \brief Get brother node of current node.
   */
  inline DTNode* Brother() const {
    return info->brother;
  }
  /*!
   * \brief Set brother node of current node.
   */
  inline void SetBrother(DTNode* node) {
    info->brother = node;
  }
  /*!
   * \brief Get histogram bin of current node.
   */
  inline void* Histo() const {
    return info->histo;
  }
  /*!
   * \brief Set histogram bin of current node. 
   */
  inline void SetHisto(void* histo) {
    info->histo = histo;
  }
  /*!
   * \brief Get data size allocated for current node.
   */
  inline index_t DataSize() const {
    return info->end_pos - info->start_pos + 1;
  }
};

/*!
 * \breif The DTree class is an abstract class, which could be 
 * implemented by real decision tree, such as BCTree (for binary classification) 
 * MCTree (for multi-classification), and RTree (for regression). Note that binary
 * classification is a special case of multi-classificaiton, while we can make 
 * careful design and optimization for it.  
 */
class DTree {
 public:
  /*!
   * \brief DTree constructor 
   */
  DTree() {}

  /*!
   * \brief DTree deconstructor 
   */
  virtual ~DTree() { }

  /*!
   * \brief Initialize decision tree.
   * \param X pointer of dataset
   * \param Y pointer of label
   * \param num_class number of classification
   * \param num_feat number of feature
   * \param data_size size of dataset
   * \param hyper_param hyper-parameter used by decision tree
   */
  void Initialize(const uint8* X, 
                  const real_t* Y,
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

  /*!
   * \brief Sample index for training data.
   * \param idx sampled index vector
   */
  inline void SetRowIdx(const std::vector<index_t>& idx) {
    CHECK_EQ(idx.empty(), false);
    rowIdx_.assign(idx.begin(), idx.end());
  }

  /*!
   * \breif Sample index for feature.
   * \param idx sampled index vector
   */
  inline void SetColIdx(const std::vector<index_t>& idx) {
    CHECK_EQ(idx.empty(), false);
    colIdx_.assign(idx.begin(), idx.end());
  }

  /*!
   * \brief Build decision tree using leaf-wise growing 
   * or using level-wise growing.
   */
  void BuildTree();

  /*!
   * \breif Given data x, predict label y 
   * \param x pointer of data example
   * \return predicted value y
   */
  real_t Predict(const uint8* x);

  /*!
   * \breif Serilize a decision tree to binary string.
   * \param str pointer of bianry string
   */
  void Serilize(std::string* str);

  /*!
   * \breif Deserilize a decision tree from bianry string.
   * \param str binary string
   */
  void Deserilize(const std::string& str);

  /*!
   * \breif Print decision tree to human-readable txt format.
   * \param str TXT string
   */
  void PrintToTXT(std::string* str);

 protected:
  /*!
   * \breif Maximal histogram bin value, range from (0, 255].
   */
  uint8 max_bin_;
  /*!
   * \breif Maximal depth to grow a tree, range from (0, 255].
   */
  uint8 max_depth_;
  /*!
   * \breif Minimal samples to split a node.
   */
  index_t min_samples_split_;
  /*!
   * \breif Minimal samples required in a node.
   */
  index_t min_samples_leaf_;
  /*!
   * \breif Maximal number of leaf nodes.
   */
  index_t max_leaf_;
  /*!
   * \breif Minimal impurity decrease required to split a node.
   */
  real_t min_impurity_dec_;
  /*!
   * \breif Minimal impurity required to split a node.
   */
  real_t min_impurity_;
  /*!
   * \breif Sampled index of dataset.
   */
  std::vector<index_t> rowIdx_;
  /*!
   * \breif Sampled index of featrue.
   */
  std::vector<index_t> colIdx_;
  /*!
   * \breif Pointer of root node.
   */
  DTNode* root_ = nullptr;
  /*!
   * \breif Number of leaf nodes.
   */
  index_t leaf_size_ = 1;
  /*!
   * \breif Depth of decision tree.
   */
  uint8 tree_depth_ = 1;
  /*!
   * \breif Number of classification.
   */
  uint8 num_class_ = 0;
  /*!
   * \breif Number of feature.
   */
  index_t num_feat_ = 0;
  /*!
   * \breif Size of dataset.
   */
  index_t data_size_ = 0;
  /*!
   * \breif Pointer of dataset.
   */
  uint8* X_ = nullptr;
  /*!
   * \breif Pointer of label.
   */
  real_t* Y_ = nullptr;

  /*!
   * \breif Get leaf value.
   * \param node tree node
   * \return leaf value
   */
  virtual real_t LeafVal(const DTNode* node) = 0;

  /*!
   * \breif Find best split position for current node.
   * \param node tree node
   */
  virtual void FindPosition(DTNode* node) = 0;

  /*!
   * \breif If current node is a leaf node.
   * \param node tree node
   * \return true for Yes and false for No
   */
  bool IsLeaf(DTNode* node);

  /*!
   * \breif Get a leaf node by given the data example.
   * \param node tree node
   * \param x data example
   * \return leaf node
   */
  DTNode* GetLeaf(DTNode* node, const uint8* x);

  /*!
   * \breif Split current node.
   * \param tree node
   */
  void SplitData(DTNode* node);

 private:
  DISALLOW_COPY_AND_ASSIGN(DTree);
};

// Classification Tree
class CDTree : public DTree {
 public:
  // ctor and dctor
  BTree() {}
  ~BTree() {}

 private:
  // Get leaf value
  real_t LeafVal(const DTNode* node);

  // Calculate gini value
  real_t Gini(const real_t left_0, const real_t left_1,
              const real_t right_0, const real_t right_1);

  // Find best split position for current node
  void FindPosition(DTNode* node);  

  DISALLOW_COPY_AND_ASSIGN(BTree);
};

// Histogram for multi-classification
class MCHistogram {
 public:
  MCHistogram(const index_t num_feat,
              const index_t num_bin,
              const uint8 num_class) {
    count_len = num_feat * num_bin * num_class;
    count = new index_t[count_len];
    for (index_t i = 0; i < count_len; ++i) {
      count[i] = 0;
    }
  }
  ~MCHistogram() {
    delete [] count;
  }
  index_t count_len = 0;
  index_t* count = nullptr;

 private:
  DISALLOW_COPY_AND_ASSIGN(MCHistogram);
};

// Multi-class Tree
class MCTree : public DTree {
 public:
  // ctor and dctor
  MCTree() {}
  ~MCTree() {}

 private:
  // Get leaf value
  real_t LeafVal(const DTNode* node);

  // Find best split position for current node
  void FindPosition(DTNode* node);  

  DISALLOW_COPY_AND_ASSIGN(MCTree);
};

// Histogram for regression
class RHistogram {
 public:
  RHistogram(const index_t num_feat,
             const index_t num_bin) {

  }
  ~RHistogram() {}
 private:
  DISALLOW_COPY_AND_ASSIGN(RHistogram);
};

// Regression Tree
class RTree : public DTree {
 public:
  // ctor and dctor
  RTree() {}
  ~RTree() {}

 private:
  // Get leaf value
  real_t LeafVal(const DTNode* node);

  // Find best split position for current node
  void FindPosition(DTNode* node);  

  DISALLOW_COPY_AND_ASSIGN(RTree);
};

//------------------------------------------------------------------------------
// Class register
//------------------------------------------------------------------------------
CLASS_REGISTER_DEFINE_REGISTRY(xforest_dtree_registry, DTree);

#define REGISTER_DTREE(format_name, dtree_name)             \
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