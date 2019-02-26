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
 * \file communicator.h
 * \brief Communicator for distributed training of xforest
 */
#ifndef XFOREST_NETWORK_COMMUNICATOR_H_
#define XFOREST_NETWORK_COMMUNICATOR_H_

#include "src/base/common.h"

namespace xforest {

/*!
 * \brief Communicator for distributed training.
 *
 * Communicator is a set of interface for network communication, which
 * can be implemented by real network libraries, such as grpc, mpi, as well
 * as raw socket. There has two types of Communicator, one is Sender 
 * (is_sender = true), and another is Receiver. For Sender, it can send binary 
 * data to remote Receiver node. For Receiver, it can listen on a specified 
 * endpoint and receive the binary data sent from Sender node. Note that, a 
 * receiver node can recv messages from multiple senders concurrently.
 */
class Communicator {
 public:
  virtual ~Communicator() {}

  /*!
   * \brief Initialize Communicator
   * \param is_sender true for sender and false for receiver
   * \param ip ip address
   * \param port end port
   * (e.g. "168.123.2.43:50051"). For Receiver, this address identifies
   * the local listening endpoint (e.g. "0.0.0.0:50051").
   * \param num_sender number of senders, only used for receiver.
   * \param queue_size the size of message queue, only used for receiver.
   * \return true for success and false for error
   */
  virtual bool Initialize(bool is_sender,
                          const char* ip,
                          int port,
                          int num_sender = 0,
                          int queue_size = 0) = 0;
  /*!
   * \brief Send message to receiver node
   * \param src data pointer
   * \param size data size
   * \return bytes send
   *   > 0 : bytes send
   *   - 1 : error
   */
  virtual int Send(const char* src, int size) = 0;

  /*!
   * \brief Receive mesesage from sender node, we
   * actually reading data from local message queue.
   * \param dest destination data pointer
   * \param max_size maximal data size
   * \return bytes received
   *   > 0 : bytes received
   *   - 1 : error
   */
  virtual int Receive(char* dest, int max_size) = 0;

  /*!
   * \brief Finalize the Communicator class
   */
  virtual void Finalize() = 0;
};

}  // namespace xforest

#endif  // XFOREST_NETWORK_COMMUNICATOR_H_
