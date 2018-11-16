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

#include "src/network/tcp_socket.h"

namespace xforest {

//------------------------------------------------------------------------------
// An network basic communication warpper.
// Will warp low level communication method, e.g., mpi, socket, and so on.
//------------------------------------------------------------------------------	
class Communicator {
 public:
  // ctor and dctor
  Communicator() { is_init_ = false; }
  ~Communicator();

  // Initialize Communicator
  virtual void Initialize(const NetConfig& config);

  // Recv data
  // Recv method will block until recv len size of data
  virtual void Recv(int rank, char* data, int len);

  // Send data
  virtual void Send(int rank, char* data, int len);

  // Get rank of local machine
  inline int rank() { return rank_; }

  // Get total number of machines
  inline int num_machines() { return num_machines_; }

 private:
  int rank_;          // rank of local machine
  int num_machines_;  // total number of machines 
  bool is_init_;      // if Communicator is intialized
  bool is_master_;    // If current process is master

  DISALLOW_COPY_AND_ASSIGN(Communicator);
};

}  // namespace xforest

#endif  // XFOREST_NETWORK_COMMUNICATOR_H_
