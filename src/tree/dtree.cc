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
This file is the implementation of DTree class.
*/

#include "src/tree/dtree.h"

#include <queue>
#include <numeric>

namespace xforest {

//------------------------------------------------------------------------------
// Class register
//------------------------------------------------------------------------------
CLASS_REGISTER_IMPLEMENT_REGISTRY(xforest_dtree_registry, DTree);
REGISTER_DTREE("btree", BTree);
REGISTER_DTREE("mctree", MCTree);
REGISTER_DTREE("rtree", RTree);

//------------------------------------------------------------------------------
// DTree class
//------------------------------------------------------------------------------

// Build decision tree
void DTree::BuildTree() {
  root_ = new DTNode();
  // Make root as left node
  root_->SetLeftOrRight('l');
  root_->SetLevel(1);
  root_->SetStartPos(0);
  root_->SetEndPos(data_size_);
  // Queue for tree growing
  std::queue<DTNode*> queue;
  queue.push(root_);
  while (!queue.empty()) {
  	DTNode* node = queue.front();
  	if (IsLeaf(node) == false) {
  	  FindPosition(node);
  	  SplitData(node);
  	  // New left child
  	  DTNode* l_node = new DTNode();
  	  l_node->SetLeftOrRight('l');
  	  l_node->SetStartPos(node->StartPos());
  	  l_node->SetEndPos(node->MidPos());
  	  l_node->SetLevel(node->Level() + 1);
  	  // New right child
  	  DTNode* r_node = new DTNode();
  	  r_node->SetLeftOrRight('r');
  	  r_node->SetStartPos(node->MidPos() + 1);
  	  r_node->SetEndPos(node->EndPos());
  	  r_node->SetLevel(node->Level() + 1);
  	  // Right node can use parent and
  	  // brother to calculate histogram bin value
  	  r_node->SetParent(node);
  	  r_node->SetBrother(node);
  	  // Push new node
  	  node->SetLeftChild(l_node);
  	  node->SetRightChild(r_node);
  	  queue.push(l_node);
  	  queue.push(r_node);
  	  if (r_node->Level() > tree_depth_) {
  	  	tree_depth_ = r_node->Level();
  	  }
  	  leaf_size_++;
  	}
  	queue.pop();
  }
}

// If current node is a leaf node?
bool DTree::IsLeaf(DTNode* node) {
  if (node->Level() == max_depth_ ||
  	  node->DataSize() < min_samples_split_) {
  	node->SetLeaf();
    node->SetLeafVal(LeafVal(node));
    // Clear tmp info
    node->Clear();
    return true;
  }
  return false;
}

// Get a leaf node by given the data x
DTNode* DTree::GetLeaf(DTNode* node, const uint8* x) {
  if (node->IsLeaf()) {
  	return node;
  }
  index_t id = node->BestFeatID();
  uint8 val = node->BestBinVal();
  if (x[id] <= val) {
  	return GetLeaf(node->LeftChild(), x);
  } else {
  	return GetLeaf(node->RightChild(), x);
  }
}

// Given data x, predict y 
real_t DTree::Predict(const uint8* x) {
  DTNode* leaf_node = GetLeaf(root_, x);
  return leaf_node->LeafVal();
}

// Serilize tree to string
void DTree::Serilize(std::string* str) {
  return;
}

// Deserilize tree from string
void DTree::Deserilize(const std::string& str) {
  return;
}

// Print decision to human-readable txt format
void DTree::PrintToTXT(std::string* str) {
  return;
}

// Split current node
void DTree::SplitData(DTNode* node) {
  index_t ptr_head = node->StartPos();
  index_t ptr_tail = node->EndPos();
  index_t best_feat_id = node->BestFeatID();
  uint8 best_bin_val = node->BestBinVal();
  uint8* ptr = X_ + best_feat_id;
  while (ptr_head < ptr_tail) {
  	uint8 bin = *(ptr + rowIdx_[ptr_head] * num_feat_);
  	if (bin <= best_bin_val) {
  	  ptr_head++;
  	} else {
  	  // swap head and tail
  	  rowIdx_[ptr_head] ^= rowIdx_[ptr_tail];
  	  rowIdx_[ptr_tail] ^= rowIdx_[ptr_head];
  	  rowIdx_[ptr_head] ^= rowIdx_[ptr_tail];
  	  ptr_tail--;
  	}
  }
  node->SetMidPos(ptr_head-1);
}

//------------------------------------------------------------------------------
// BTree class
//------------------------------------------------------------------------------
  
// Get leaf value
real_t BTree::LeafVal(const DTNode* node) {
  index_t count_0 = 0;
  index_t count_1 = 0;
  index_t start_pos = node->StartPos();
  index_t end_pos = node->EndPos();
  index_t len = node->DataSize();
  for (index_t i = start_pos; i <= end_pos; ++i) {
  	if (Y_[rowIdx_[i]] == 0) {
  	  count_0++;
  	}
  	count_1 = len - count_0;
  }
  return count_0 > count_1 ? 0.0 : 1.0;
}

// Calculate gini value
real_t BTree::Gini(const real_t left_0, 
	               const real_t left_1,
	               const real_t right_0,
	               const real_t right_1) {
  index_t all_left = left_0 + left_1;
  index_t all_right = right_0 + right_1;
  index_t all = all_right + all_left;
  real_t gini_left = 1.0 - 
    ((real_t)((left_0*left_0) + (left_1*left_1)) / 
      (real_t)(all_left*all_left));
  real_t gini_right = 1.0 - 
    ((real_t)((right_0*right_0) + (right_1*right_1)) / 
      (real_t)(all_right*all_right));
  return ((real_t)all_left / all) * gini_left +
         ((real_t)all_right / all) * gini_right;
}

// Find best split position for current node
void BTree::FindPosition(DTNode* node) {
  BHistogram* histo = (BHistogram*)node->Histo();
  histo = new BHistogram(num_feat_, max_bin_);
  // Collect histogram
  index_t total_0 = 0;
  index_t total_1 = 0;
  index_t len = node->DataSize();
  index_t start_pos = node->StartPos();
  index_t end_pos = node->EndPos();
  index_t col_size = colIdx_.size();
  // If node is left node or
  // node is right but brother is leaf
  if (node->LeftOrRight() == 'l' || 
  	  node->Brother()->IsLeaf()) {
  	for (index_t i = start_pos; i <= end_pos; ++i) {
  	  index_t row_idx = rowIdx_[i];
  	  uint8* ptr = X_ + row_idx * num_feat_;
  	  if (Y_[row_idx] == 0) {
  	  	total_0++;
  	  	for (index_t j = 0; j < col_size; ++j) {
  	  	  uint8 bin = *(ptr + colIdx_[j]);
  	  	  histo->count[j][bin].count_0++;
  	  	}
  	  } else {
  	  	for (index_t j = 0; j < col_size; ++j) {
  	  	  uint8 bin = *(ptr + colIdx_[j]);
  	  	  histo->count[j][bin].count_1++;
  	  	}
  	  }
  	}
  	total_1 = len - total_0;
  } else {  // histo = parent_histo - brother_histo
  	BHistogram* parent = (BHistogram*)node->Parent()->Histo();
  	BHistogram* brother = (BHistogram*)node->Brother()->Histo();
  	total_0 = parent->total_0 - brother->total_0;
  	total_1 = parent->total_1 - brother->total_1;
  	for (index_t i = 0; i < col_size; ++i) {
  	  for (index_t j = 0; j <= max_bin_; ++j) {
  	  	histo->count[i][j].count_0 = 
  	  	  parent->count[i][j].count_0 - brother->count[i][j].count_0;
  	  	histo->count[i][j].count_1 = 
  	  	  parent->count[i][j].count_1 - brother->count[i][j].count_1;
  	  }
  	}
  }
  histo->total_0 = total_0;
  histo->total_1 = total_1;
  // Find best split position
  for (index_t i = 0; i < col_size; ++i) {
  	Count* count = histo->count[i];
  	index_t left_0 = 0;
  	index_t left_1 = 0;
  	for (index_t j = 0; j <= max_bin_; ++j) {
  	  left_0 += count[j].count_0;
  	  left_1 += count[j].count_1;
  	  index_t right_0 = total_0 - left_0;
  	  index_t right_1 = total_1 - left_1;
  	  real_t gini = Gini(left_0, left_1, right_0, right_1);
  	  if (gini < node->BestGini()) {
  	  	node->SetBestGini(gini);
  	  	node->SetBestFeatID(colIdx_[j]);
  	  	node->SetBestBinVal(i);
  	  }
  	}
  }
  if (node->LeftOrRight() == 'r') {
  	node->ClearParent();
  }
}

//------------------------------------------------------------------------------
// MCTree class
//------------------------------------------------------------------------------

// Get leaf value
real_t MCTree::LeafVal(const DTNode* node) {
  std::vector<index_t> count(num_class_, 0);
  std::vector<index_t>::iterator result;
  index_t start_pos = node->StartPos();
  index_t end_pos = node->EndPos();
  for (index_t i = start_pos; i <= end_pos; ++i) {
  	count[Y_[rowIdx_[i]]]++;
  }
  result = std::max_element(count.begin(), count.end());
  return (real_t)std::distance(count.begin(), result);
}

// Find best split position for current node
void MCTree::FindPosition(DTNode* node) {
  MCHistogram* histo = (MCHistogram*)node->Histo();
  histo = new MCHistogram(num_feat_, max_bin_, num_class_);
  index_t len = node->DataSize();
  index_t start_pos = node->StartPos();
  index_t end_pos = node->EndPos();
  index_t* count = histo->count;
  index_t col_size = colIdx_.size();
  index_t cc = num_class_ * col_size;
  // Collect histogram
  if (node->LeftOrRight() == 'l' ||
  	  node->Brother()->IsLeaf()) {
  	for (index_t i = start_pos; i <= end_pos; ++i) {
  	  index_t row_idx = rowIdx_[i];
  	  int y = Y_[row_idx];
  	  uint8* ptr = X_ + row_idx * num_feat_;
  	  for (index_t j = 0; j < col_size; ++j) {
  	  	count[num_class_*(*(ptr+colIdx_[j])*col_size+j)+y]++;
  	  }
  	}
  } else {
  	MCHistogram* histo_parent = (MCHistogram*)node->Parent()->Histo();
  	index_t* count_parent = histo_parent->count;
  	MCHistogram* histo_brother = (MCHistogram*)node->Brother()->Histo();
  	index_t* count_brother = histo_brother->count;
  	index_t count_len = histo->count_len;
  	for (index_t i = 0; i < count_len; ++i) {
  	  count[i] = count_parent[i] - count_brother[i];
  	}
  }
  // Sum total count
  std::vector<index_t> total_count(num_class_, 0);
  for (index_t i = 0; i <= max_bin_; ++i) {
  	index_t* ptr = count + i*cc;
  	for (uint8 c = 0; c < num_class_; ++c) {
  	  total_count[c] += *ptr;
  	  ptr++;
  	}
  }
  // Find best split position
  for (index_t j = 0; j < col_size; ++j) {
  	std::vector<index_t> left_count(num_class_, 0);
  	std::vector<index_t> right_count(total_count);
  	index_t* base_ptr = count + j*num_class_;
  	for (index_t i = 0; i <= max_bin_; ++i) {
  	  index_t* ptr = base_ptr + cc*i;
  	  for (uint8 c = 0; c < num_class_; ++c) {
  	  	left_count[c] += *ptr;
  	  	right_count[c] -= *ptr;
  	  	ptr++;
  	  }
  	  index_t left_sum = 
  	    std::accumulate(left_count.begin(), left_count.end(), 0);
  	  index_t right_sum = 
  	    std::accumulate(right_count.begin(), right_count.end(), 0);
  	  real_t real_left_sum = 0.0;
  	  real_t real_right_sum = 0.0;
  	  for (uint8 c = 0; c < num_class_; ++c) {
  	  	real_t tmp = (real_t)left_count[c] / left_sum;
  	  	real_left_sum += tmp*tmp;
  	  	tmp = (real_t)right_count[c] / right_sum;
  	  	real_right_sum += tmp*tmp;
  	  }
  	  real_t left_gini = 1.0 - real_left_sum;
  	  left_gini *= (real_t)left_sum / len;
  	  real_t right_gini = 1.0 - real_right_sum;
  	  right_gini *= (real_t)right_sum / len;
  	  real_t gini = left_gini + right_gini;
  	  if (gini < node->BestGini()) {
  	  	node->SetBestGini(gini);
  	  	node->SetBestFeatID(colIdx_[j]);
  	  	node->SetBestBinVal(i);
  	  }
  	}
  }
  if (node->LeftOrRight() == 'r') {
  	node->ClearParent();
  }
}

//------------------------------------------------------------------------------
// RTree class
//------------------------------------------------------------------------------

// Get leaf value
real_t RTree::LeafVal(const DTNode* node) {
  return 0;	
}

// Find best split position for current node
void RTree::FindPosition(DTNode* node) {
  
}

}  // namespace xforest