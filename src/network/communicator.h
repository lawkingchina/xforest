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
This file defines the Communicator class.
*/

#ifndef XFOREST_NETWORK_COMMUNICATOR_H_
#define XFOREST_NETWORK_COMMUNICATOR_H_

#include "src/base/common.h"

namespace xforest {

//------------------------------------------------------------------------------
// An network basic communication warpper.
// Will warp low level communication method, e.g., mpi, socket, and so on.
//------------------------------------------------------------------------------	
class Communicator {
 public:
  // ctor and dctor
  Communicator();
  ~Communicator();

  // Initialize Communicator
  virtual void Initialize(int rank, /* master is rank_0 */
  	                      int num_workers,
  	                      const std::string& master_addr) = 0;

  // Recv data
  virtual void Recv(int rank, char* data, int len) = 0;

  // Send data
  virtual void Send(int rank, char* data, int len) = 0;

  // Finalize Communicator
  virtual void Finalize() = 0;
};

}  // namespace xforest

#endif  // XFOREST_NETWORK_COMMUNICATOR_H_
