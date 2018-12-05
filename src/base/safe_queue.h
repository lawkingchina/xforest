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
This file defines the SafeQueue class.
*/

#ifndef XFOREST_BASE_SAFE_QUEUE_H_
#define XFOREST_BASE_SAFE_QUEUE_H_

//------------------------------------------------------------------------------
// SafeQueue is a message queue that is thread-safe
//------------------------------------------------------------------------------
template <typename DType>
class SafeQueue {
 public:
  // ctor and dctor
  SafeQueue() {}
  ~SafeQueue() {}

  // Push data to queue
  inline void push(DType* node) {
    std::lock_guard<std::mutex> lock(m_mutex);
    queue_.push(node);
    m_condition.notify_one();
  }

  // Pop data
  inline DType* pop() {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_condition.wait(lock, [this]() {
      return !queue_.empty();
    });
    DTNode* node = queue_.front();
    queue_.pop();
    return node;
  }

  // If queue is empty
  inline bool empty() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return queue_.empty();
  }

  // Size of current queue
  inline size_t size() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return queue_.size();
  }

 private:
  std::queue<DType*> queue_;
  std::condition_variable m_condition;
  mutable std::mutex m_mutex;
};

#endif  // XFOREST_BASE_SAFE_QUEUE_H_