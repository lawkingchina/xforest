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

namespace xforest {

//------------------------------------------------------------------------------
// DTree class
//------------------------------------------------------------------------------

// Build decision tree
void DTree::BuildTree() {
  root_ = new DTNode();
  root_->Init(colIdx_.size(), max_bin_+1, num_class_);
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
  	  l_node->Init(colIdx_.size(), max_bin_+1, num_class_);
  	  l_node->SetLeftOrRight('l');
  	  l_node->SetStartPos(node->StartPos());
  	  l_node->SetEndPos(node->MidPos());
  	  l_node->SetLevel(node->Level() + 1);
  	  // New right child
  	  DTNode* r_node = new DTNode();
  	  r_node->Init(colIdx_.size(), max_bin_+1, num_class_);
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
  return 0;	
}

// Find best split position for current node
void BTree::FindPosition(DTNode* node) {
  
}

//------------------------------------------------------------------------------
// MCTree class
//------------------------------------------------------------------------------

// Get leaf value
real_t MCTree::LeafVal(const DTNode* node) {
  return 0;	
}

// Find best split position for current node
void MCTree::FindPosition(DTNode* node) {
  
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