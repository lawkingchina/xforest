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
This file defines the basic hyper-parameters used by xForest.
*/

#ifndef XFOREST_SOLVER_HYPER_PARAMETER_H_
#define XFOREST_SOLVER_HYPER_PARAMETER_H_

#include "src/base/common.h"

//------------------------------------------------------------------------------
// We use a single data structure - HyperParam to handle all of
// the hyper parameters used by xForest.
//------------------------------------------------------------------------------
struct HyperParam {
  uint32 n_estimators;
  uint32 min_samples_split;
  uint32 min_samples_leaf;
  uint32 max_features;
  uint32 max_leaf_nodes;
  real_t min_impurity_decrease;
  real_t min_impurity_split;
  uint32 n_jobs;
  int random_state;
};

#endif  // XFOREST_SOLVER_HYPER_PARAMETER_H_