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
This file tests safe_queue.h file.
*/

#include "gtest/gtest.h"

#include "src/base/safe_queue.h"

TEST(SafeQueueTest, POP_AND_PUSH) {
  int a = 0;
  int b = 1;
  int c = 2;
  SafeQueue<int*> queue;
  queue.push(&a);
  queue.push(&b);
  queue.push(&c);
  EXPECT_EQ(queue.size(), 3);
  int *a_ptr = queue.pop();
  int *b_ptr = queue.pop();
  int *c_ptr = queue.pop();
  EXPECT_EQ(*a_ptr, a);
  EXPECT_EQ(*b_ptr, b);
  EXPECT_EQ(*c_ptr, c);
  EXPECT_EQ(queue.empty(), true);
}