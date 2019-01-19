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

namespace xforest {

//------------------------------------------------------------------------------
// DTree class
//------------------------------------------------------------------------------

// Build decision tree
void DTree::BuildTree() {
  root_ = new DTNode();
  root_->Init(colIdx_.size(), max_bin_+1, num_class_);
}

// Given data x, predict y 
real_t DTree::Predict(const uint8* x) {
  real_t y = 0;

  return y;
}

// Serilize tree to string
void DTree::Serilize(std::string* str) {

}

// Deserilize tree from string
void DTree::Deserilize(const std::string& str) {

}

// Print decision to human-readable txt format
void DTree::PrintToTXT(std::string* str) {

}

//------------------------------------------------------------------------------
// BTree class
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// MCTree class
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// RTree class
//------------------------------------------------------------------------------

}  // namespace xforest