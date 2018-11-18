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

#include <unistd.h>
#include <stdlib.h>

#include "src/base/common.h"
#include "gtest/gtest.h"

const int kNumWorker = 2;

const char* str_1 = "11111";
const char* str_2 = "22222";
const char* str_3 = "33333";
const char* str_end = "endIO";

static void SendMsg(xforest::SocketCommunicator& worker, int rank) {
  sleep(1);
  worker.Send(0, str_1, 5);
  sleep(1);
  worker.Send(0, str_2, 5);
  sleep(1);
  worker.Send(0, str_3, 5);
  sleep(1);
  worker.Send(0, str_end, 5);
}

TEST(SocketCommunicator, WorkerSide) {
  sleep(3); // wait master node
  int pid = fork();
  if (pid > 0) {  // worker 1
    xforest::SocketCommunicator worker_1;
    worker_1.Initialize(1, kNumWorker, "127.0.0.1:12334");
    SendMsg(worker_1, 1);
  } else {  // worker 2
  	xforest::SocketCommunicator worker_2;
    worker_2.Initialize(2, kNumWorker, "127.0.0.1:12334");
    SendMsg(worker_2, 2);
  }
  sleep(2);
  wait(0);
}