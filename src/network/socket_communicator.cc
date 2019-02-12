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
This file is the implementation of SocketCommunicator class.
*/

#include <thread>

#include "src/network/socket_communicator.h"
#include "src/base/split_string.h"

namespace xforest {

// Initialize Communicator
void SocketCommunicator::Initialize(int rank, /* master is rank_0 */
  	                                int num_workers, 
  	                                const std::string& master_addr) {
  CHECK_GE(rank, 0);
  CHECK_GT(num_workers, 0);
  rank_ = rank;
  num_workers_ = num_workers;
  is_master_ = rank_ == 0 ? true : false;
  master_addr_ = master_addr;
  if (is_master_) {
  	InitMaster();
  } else {
  	InitWorker();
  }
}

// Initialize master node
void SocketCommunicator::InitMaster() {
  // server socket + worker socket
  sockets_.resize(num_workers_ + 1);
  std::vector<std::string> ip_and_port;
  SplitStringUsing(master_addr_, ":", &ip_and_port);
  CHECK_EQ(2, ip_and_port.size());
  sockets_[0] = new TCPSocket();
  TCPSocket* server = sockets_[0]; // server socket
  server->SetTimeout(kTimeOut * 60 * 1000); // millsec
  // Bind socket
  CHECK(server->Bind(ip_and_port[0].c_str(),
                  atoi(ip_and_port[1].c_str())));
  // Listen socket
  CHECK(server->Listen(1024));
  LOG(INFO) << "Wait worker connect ...";
  // Accept all workers
  std::string accept_ip;
  uint16 accept_port;
  for (int i = 1; i <= num_workers_; ++i) {
    sockets_[i] = new TCPSocket();
    CHECK(server->Accept(sockets_[i], &accept_ip, &accept_port));
    LOG(INFO) << "master " << ip_and_port[0] << ":" << ip_and_port[1]
              << " accepts worker " << accept_ip << ":" << accept_port;
  }
}

// Initialize worker node
void SocketCommunicator::InitWorker() {
  // Only a client socket
  sockets_.resize(1);
  std::vector<std::string> ip_and_port;
  SplitStringUsing(master_addr_, ":", &ip_and_port);
  CHECK_EQ(2, ip_and_port.size());
  sockets_[0] = new TCPSocket();
  TCPSocket* client = sockets_[0];
  client->Connect(ip_and_port[0].c_str(), 
  	         atoi(ip_and_port[1].c_str()));
  LOG(INFO) << "Connect to master node " 
            << ip_and_port[0] << ":" << ip_and_port[1];
}

// Recv data
void SocketCommunicator::Recv(int rank, char* data, int len) {
  TCPSocket* socket = sockets_[rank];
  int recieved_bytes = 0;
  while (recieved_bytes < len) {
  	int max_len = len - recieved_bytes;
    int tmp = socket->Receive(data+recieved_bytes, max_len);
    CHECK_GE(tmp, 0);
    recieved_bytes += tmp;
  }
}

// Send data
void SocketCommunicator::Send(int rank, const char* data, int len) {
  TCPSocket* socket = sockets_[rank];
  int sent_bytes = 0;
  while (sent_bytes < len) {
    int max_len = len - sent_bytes;
    int tmp = socket->Send(data+sent_bytes, max_len);
    CHECK_GE(tmp, 0);
    sent_bytes += tmp;
  }
}

}  // namespace xforest