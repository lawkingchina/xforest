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
 * \file timer.h
 * \brief This file defines the Timer class.
 */
#ifndef XFOREST_BASE_TIMER_H_
#define XFOREST_BASE_TIMER_H_

#include <chrono>

#include "src/base/common.h"

/*!
 * \breif We can use the Timer class like this:
 *
 *   Timer timer();
 *   timer.tic();
 *
 *     .... // code we want to evaluate
 *
 *   float time = timer.toc();  // (sec)
 *
 * This class can be used to evaluate multi-thread code.
 */
class Timer {
 public:
  /*!
   * \breif constructor
   */
  Timer();

  /*!
   * \breif Reset start time
   */
  void reset();

  /*!
   * \breif Code start
   */
  void tic();

  /*!
   * \breif Code end
   */
  float toc();

  /*!
   * \breif Get the time duration
   */ 
  float get();

 protected:
  /*!
   * \breif begining time point
   */ 
  std::chrono::high_resolution_clock::time_point begin;
  /*!
   * \breif time duration
   */ 
  std::chrono::milliseconds duration;

 private:
  DISALLOW_COPY_AND_ASSIGN(Timer);
};

#endif  // XFOREST_BASE_TIMER_H_