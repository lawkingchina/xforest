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

/*!
*  Copyright (c) 2018 by Contributors
* \file hyper_parameter.h
* \brief This file defines the basic hyper-parameters used by xForest.
*/
#ifndef XFOREST_SOLVER_HYPER_PARAMETER_H_
#define XFOREST_SOLVER_HYPER_PARAMETER_H_

#include <string>

#include "src/base/common.h"

/*!
* \brief HyperParam handles all of the hyper parameters used by xforest.
*/
struct HyperParam {
  /*!
  * \brief xforest uses histogram-based decision tree algorithm, and
  * max_bin defines the maximul size of histogram bin (default=255).
  */
  uint8 max_bin = 255;
  /*!
  * \brief The number of trees in the forest (default=100).
  */
  int n_estimators = 100;
  /*!
  * \brief The function to measure the quality of a split for classifier (default=gini). 
  * Supported criterion are "gini" for the gini impurity and "entropy"
  * for the information gain. We also support "mix" which means we build
  * trees using both "gini" and "entropy" criterion in a random manner.
  */
  std::string classifier_criterion = "gini";
  /*!
  * \brief The function to measure the quality of a split for regressor (default=mse).
  * Supported criterion are "mse" for the mean squared error, which is equal
  * to variance reduction as feature selection criterion, and "mae" for mean
  * absolute error. We also support "mix" which means we build trees using both
  * "mse" and "mae" criterion in a random manner.
  */
  std::string regressor_criterion = "mse";
  /*!
  * \brief The maximul depth of the tree (default=-1).
  * If max_depth = -1, then nodes are expanded until all leaves are pure or
  * until all leaves contain less than min_samples_split samples. 
  */
  int max_depth = -1;
  /*!
  * \brief The minimum number of samples required to split an internal node (default=2).
  */
  int min_samples_split = 2;
  /*!
  * \brief The minimum number of samples required to be at a leaf node (default=1). 
  * A split point at any depth will only be considered if it leaves at least 
  * min_samples_leaf training samples in each of the left and right branches. 
  * This may have the effect of smoothing the model, especially in regression.
  */
  int min_samples_leaf = 1;
  /*!
  * \brief The number of features to consider when looking for 
  * the best split (default=1).
  */
  int max_features = 1;
  /*!
  * \brief Grow trees with max_leaf_nodes (default=-1).
  * If max_leaf_nodes = -1, then unlimited number of leaf nodes.
  */
  int max_leaf_nodes = -1;
  /*!
  * A node will be split if this split induces a decrease of the impurity
  * greater than or equal to this value (default=0). The weighted impurity 
  * decrease equation is the following:
  *
  *   N_t / N * (impurity - N_t_R / N_t * right_impurity
  *                       - N_t_L / N_t * left_impurity)
  *
  * where N is the total number of samples, N_t is the number of samples at 
  * current node, N_t_L is the number of samples in the left child, and N_t_R
  * is the number of samples in the right child.
  */
  real_t min_impurity_decrease = 0.0;
  // float, (default=1e-7)
  // Threshold for early stopping in tree growth. A node will split 
  // if its impurity is above the threshold, otherwise it is a leaf.
  real_t min_impurity_split=1e-7;
  // boolean, optional (default=True)
  // Whether bootstrap samples are used when building trees.
  bool bootstrap = true;
  // int or None, optional (default=None, -1)
  // The number of jobs to run in parallel for both fit and predict.
  // -1 means using all processors.
  int n_jobs = -1;
  // int, optional (default=1231)
  // random_state is the seed used by the random number generator.
  int random_state = 1231;
};

#endif  // XFOREST_SOLVER_HYPER_PARAMETER_H_