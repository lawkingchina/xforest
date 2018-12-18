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

//------------------------------------------------------------------------------
// The Deicision Tree class is an abstract class, which will be implemented
// by real decision tree, such as BCTree (Binary Classification Tree) and 
// MCRTree (Multi-Class Tree) and RTree (Regression Tree).
//------------------------------------------------------------------------------
class DTree {
 public:
  // ctor and dctor
  DTree() {}
  ~DTree() {}

 private:
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