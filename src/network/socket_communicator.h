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
This file defines the SocketCommunicator class.
*/

#ifndef XFOREST_NETWORK_SOCKET_COMMUNICATOR_H_
#define XFOREST_NETWORK_SOCKET_COMMUNICATOR_H_

#include <vector>
#include <string>

#include "src/base/scoped_ptr.h"
#include "src/network/communicator.h"
#include "src/network/tcp_socket.h"

namespace xforest {

//------------------------------------------------------------------------------
// Socket warpper for Communicator
//------------------------------------------------------------------------------	
class SocketCommunicator : public Communicator {
 public:
  SocketCommunicator();
  ~SocketCommunicator();

  // Initialize Communicator
  virtual void Initialize(int rank, /* master is rank_0 */
  	                      int num_workers, 
  	                      const std::string& master_addr);

  // Recv data
  virtual void Recv(int rank, char* data, int len);

  // Send data
  virtual void Send(int rank, char* data, int len);

 private:
  void InitMaster();  // Initialize master node
  void InitWorker();  // Initialize worker node

  int rank_;          // rank of local machine
  int num_workers_;   // total number of workers 
  bool is_master_;    // Node is master node
  bool is_init_;      // Communicator is intialized
  std::string master_addr_; // Address of master node

  std::vector<TCPSocket*> sockets_;  // sockets

  DISALLOW_COPY_AND_ASSIGN(SocketCommunicator);
};

}  // namespace xforest

#endif  // XFOREST_NETWORK_SOCKET_COMMUNICATOR_H_