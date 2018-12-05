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

//------------------------------------------------------------------------------
// The Deicision Tree class
//------------------------------------------------------------------------------
class DTree {

};

}  // xforest

#endif  // XFOREST_TREE_DTREE_H_