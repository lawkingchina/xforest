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
* \file msg_queue_test.cc
* \brief This file tests the MessageQueue class.
*/
#include "src/network/msg_queue.h"

#include <string>
#include "gtest/gtest.h"

using xforest::MessageQueue;
using std::string;

TEST(MessageQueueTest, AddRemove) {
  MessageQueue queue(5, 1);  // size:5, num_of_producer:1
  char buff[10];
  queue.Add("111", 3);
  queue.Add("22", 2);
  EXPECT_EQ(0, queue.Add("xxxx", 4, false));  // non-blocking add
  queue.Remove(buff, 3);
  EXPECT_EQ(string(buff, 3), string("111"));
  queue.Remove(buff, 2);
  EXPECT_EQ(string(buff, 2), string("22"));
  queue.Add("33333", 5);
  queue.Remove(buff, 5);
  EXPECT_EQ(string(buff, 5), string("33333"));
  EXPECT_EQ(0, queue.Remove(buff, 10, false));  // non-blocking remove
  EXPECT_EQ(queue.Add("666666", 6), -1);  // exceed buffer size
  queue.Add("55555", 5);
  EXPECT_EQ(queue.Remove(buff, 3), -1);  // message too long
}

TEST(MessageQueueTest, EmptyAndNoMoreAdd) {
  MessageQueue queue(5, 2);  // size:5, num_of_producer:2
  char buff[10];
  EXPECT_EQ(queue.EmptyAndNoMoreAdd(), false);
  queue.Signal(1);
  queue.Signal(1);
  EXPECT_EQ(queue.EmptyAndNoMoreAdd(), false);
  queue.Signal(2);
  EXPECT_EQ(queue.EmptyAndNoMoreAdd(), true);
  EXPECT_EQ(queue.Remove(buff, 5), 0);
}