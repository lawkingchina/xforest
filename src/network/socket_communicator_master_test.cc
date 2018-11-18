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
This file tests the SocketCommunicator class.
*/

#include "src/network/socket_communicator.h"

#include "src/base/common.h"
#include "gtest/gtest.h"

const int kNumWorker = 2;

TEST(SocketCommunicator, MasterSide) {
  xforest::SocketCommunicator master;
  master.Initialize(0, kNumWorker, "127.0.0.1:12334");
  char* buffer = new char[5];
  int count = 0;
  for (;;) {
  	for (int rank = 1; rank <= kNumWorker; ++rank) {
  	  master.Recv(rank, buffer, 5);
  	  std::cout << "Recv " << rank << ":" << buffer << std::endl;
  	  if (strcmp(buffer, "endIO") == 0) {
  	  	count++;
  	  }
  	}
  	if (count == kNumWorker) {
  	  	break;
  	}
  }
  std::cout << "Master closed()" << std::endl;
}