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
This file defines the basic data structures.
*/

//------------------------------------------------------------------------------
// We use 32-bits float to store the real number during computation, 
// such as the calculated gini value.
//------------------------------------------------------------------------------
typedef float real_t;

//------------------------------------------------------------------------------
// We use 32-bits unsigned integer to store the feature index.
//------------------------------------------------------------------------------
typedef uint32 index_t;

//------------------------------------------------------------------------------
// Each data block stored in data matrix
//------------------------------------------------------------------------------
struct Feat {
  // Feature value
  real_t value;
  // Label y
  real_t y;
  // Original idx before sorting
  size_t ori_idx;
};

//------------------------------------------------------------------------------
// Compare two feat used in sort
//------------------------------------------------------------------------------
static bool FeatCmp(const Feat& feat_a, const Feat& feat_b) {
  return feat_a.value < feat_b.value;
}

//------------------------------------------------------------------------------
// Data matrix used by decision tree algorithm
//------------------------------------------------------------------------------
struct DMatrix {
  size_t dim_0;  // row size
  size_t dim_1;  // col size
  std::vector<std::vector<Feat> > data;
}