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

  // Initialized
  void Initialize(const NetConfig config);

  // Recv data
  // Recv method will block until recv len size of data
  void Recv(int rank, char* data, int len);

  // Send data
  void Send(int rank, char* data, int len);

  // Bind local listen to port
  void TryBind(int port);

  // Set socket to rank
  void SetLinker(int rank, const TCPSocket& socket);

  // Thread for listening
  void ListenThread(int incoming_cnt);

 private:
   int socket_timeout_;                     // timeout for socket, in mimutes
   int local_listen_port_;                  // local listen ports
   std::vector<std::string> client_ips_;    // store client ips
   std::vector<int> client_ports_;          // store client ports
   std::unique_ptr<TCPSocket> listener_;    // local socket listener
   std::vector<std::unique_ptr<TCPSocket> > linkers_; // linkers
};

}  // namespace xforest

#endif  // XFOREST_NETWORK_SOCKET_COMMUNICATOR_H_