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
* \file socket_communicator_worker_test.cc
* \brief This file tests the SocketCommunicator class.
*/
#include "src/network/socket_communicator.h"

#include <unistd.h>
#include <stdlib.h>

#include "src/base/common.h"
#include "gtest/gtest.h"

#define IS_SENDER true

const char* str_1 = "11111";
const char* str_2 = "22222";
const char* str_3 = "33333";

static void SendMsg(xforest::SocketCommunicator& worker, int rank) {
  worker.Send(str_1, 5);
  worker.Send(str_2, 5);
  worker.Send(str_3, 5);
}

TEST(SocketCommunicator, WorkerSide) {
  sleep(2); // wait master node
  int pid = fork();
  if (pid > 0) {  // worker 1
    xforest::SocketCommunicator worker_1;
    worker_1.Initialize(IS_SENDER, "127.0.0.1", 50051);
    for (int i = 0; i < 999999; ++i) {
      SendMsg(worker_1, 1);
    }
    worker_1.Finalize();
  } else {  // worker 2
  	xforest::SocketCommunicator worker_2;
    worker_2.Initialize(IS_SENDER, "127.0.0.1", 50051);
    for (int i = 0; i < 999999; ++i) {
      SendMsg(worker_2, 1);
    }
    worker_2.Finalize();
  }
  wait(0);
}