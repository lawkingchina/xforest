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
* \file socket_communicator_master_test.cc
* \brief This file tests the SocketCommunicator class.
*/
#include "src/network/socket_communicator.h"

#include "src/base/common.h"
#include "gtest/gtest.h"

#define IS_RECEIVER false

const int kNumWorker = 2;
const int kBuffSize = 100 * 1024;  // 100 KB

TEST(SocketCommunicator, MasterSide) {
  xforest::SocketCommunicator master;
  master.Initialize(IS_RECEIVER, "127.0.0.1", 50051, kNumWorker, kBuffSize);
  char* buffer = new char[5];
  int count = 0;
  for (;;) {
  	master.Receive(buffer, 5);
  	std::cout << "Recv " << ":" << buffer << std::endl;
  	if (strcmp(buffer, "endIO") == 0) {
  	  count++;
  	}
  	if (count == kNumWorker) {
  	  break;
  	}
  }
  master.Finalize();
  std::cout << "Master closed()" << std::endl;
}