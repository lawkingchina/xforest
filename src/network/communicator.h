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

#include <vector>
#include <string>
#include <utility>

#include "src/network/tcp_socket.h"

namespace xforest {

//------------------------------------------------------------------------------
// Network configure for Communicator.
// A configure file could be like this:
// 
//  master:
//    115.27.204.50
//  worker:
//    115.27.204.50
//    115.27.204.51
//    115.27.204.52
//    115.27.204.53
//------------------------------------------------------------------------------	
class NetConfig {
 public:
  // ctor and dctor
  NetConfig();
  ~NetConfig();

  // Parse configure from file
  void ParseFromFile(const std::string& filename);

  // Get a copy the master address
  inline std::pair<std::string, int> GetMasterAddr() {
  	return master_addr_;
  }

  // Get a copy of the worker address list
  inline std::vector<std::pair<std::string, int> > GetWorkerAddrList() {
  	return worker_addr_list_;
  }


 private:
  std::pair<std::string, /* ip */
            int          /* port */>        master_addr_;
  std::vector<std::pair<std::string, int> > worker_addr_list_;

 DISALLOW_COPY_AND_ASSIGN(NetConfig);
};

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
  inline num_machines() { return num_machines_; }

 private:
  int rank_;          // rank of local machine
  int num_machines_;  // total number of machines 
  bool is_init_;      // if Communicator is intialized

  DISALLOW_COPY_AND_ASSIGN(Communicator);
};

}  // namespace xforest

#endif  // XFOREST_NETWORK_COMMUNICATOR_H_
