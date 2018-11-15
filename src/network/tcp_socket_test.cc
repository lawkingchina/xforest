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
This file tests the TCPSocket class.
*/

#include "src/network/tcp_socket.h"

#include <string.h>
#include <unistd.h>
#include <string>

#include "src/base/common.h"
#include "gtest/gtest.h"

using std::string;
using xforest::TCPSocket;

const size_t kDataLength = 9999999;

TEST(TCPSocket, SendRecieve) {
  int pid = fork();
  const char * msg = "0123456789";
  ASSERT_GE(pid, 0);
  if (pid > 0) {  // parent: server
    TCPSocket server;
    TCPSocket client;
    string cl_ip;
    uint16 cl_port;
    char serbuff[10];
    memset(serbuff, '\0', 10);

    ASSERT_TRUE(server.Bind("127.0.0.1", 11223));
    ASSERT_TRUE(server.Listen(3));
    ASSERT_TRUE(server.Accept(&client, &cl_ip, &cl_port));

    // small block
    for (int i = 0; i < 99999; ++i) {
      int tmp;
      int recieved_bytes = 0;
      while (recieved_bytes < 10) {
      	int max_len = 10 - recieved_bytes;
        tmp = client.Receive(&serbuff[recieved_bytes], max_len);
        ASSERT_GE(tmp, 0);
        recieved_bytes += tmp;
      }
      ASSERT_EQ(string("0123456789"), string(serbuff, 10));
      int sent_bytes = 0;
      while (sent_bytes < 10) {
      	int max_len = 10 - sent_bytes;
        tmp = client.Send(&msg[sent_bytes], max_len);
        ASSERT_GE(tmp, 0);
        sent_bytes += tmp;
      }
    }

    // Big block
    char* bigdata = new char[kDataLength];
    char* bigbuff = new char[kDataLength];
    memset(bigdata, 'x', kDataLength);
    memset(bigbuff, '\0', kDataLength);
    int recieved_bytes = 0;
    while (recieved_bytes < kDataLength) {
      int max_len = kDataLength - recieved_bytes;
      int tmp = client.Receive(&bigbuff[recieved_bytes], max_len);
      ASSERT_GE(tmp, 0);
      recieved_bytes += tmp;
    }
    for (size_t i = 0; i < kDataLength; ++i) {
      ASSERT_EQ(bigbuff[i], 'x');
    }
    int sent_bytes = 0;
    while (sent_bytes < kDataLength) {
      int max_len = kDataLength - sent_bytes;
      int tmp = client.Send(&bigdata[sent_bytes], max_len);
      ASSERT_GE(tmp, 0);
      sent_bytes += tmp;
    }
  } else {  // child: client
    sleep(3);   // wait for server
    TCPSocket client;
    ASSERT_TRUE(client.Connect("127.0.0.1", 11223));
    char clibuff[10];
    memset(clibuff, '\0', 10);

    // small block
    for (int i = 0; i < 99999; ++i) {
      int tmp;
      int sent_bytes = 0;
      while (sent_bytes < 10) {
      	int max_len = 10 - sent_bytes;
        tmp = client.Send(&msg[sent_bytes], max_len);
        ASSERT_GE(tmp, 0);
        sent_bytes += tmp;
      }
      int recieved_bytes = 0;
      while (recieved_bytes < 10) {
      	int max_len = 10 - recieved_bytes;
        tmp = client.Receive(&clibuff[recieved_bytes], max_len);
        ASSERT_GE(tmp, 0);
        recieved_bytes += tmp;
      }
      ASSERT_EQ(string("0123456789"), string(clibuff, 10));
    }

    // Big block
    char* bigdata = new char[kDataLength];
    char* bigbuff = new char[kDataLength];
    memset(bigdata, 'x', kDataLength);
    memset(bigbuff, '\0', kDataLength);
    int sent_bytes = 0;
    while (sent_bytes < kDataLength) {
      int max_len = kDataLength - sent_bytes;
      int tmp = client.Send(&bigdata[sent_bytes], max_len);
      ASSERT_GE(tmp, 0);
      sent_bytes += tmp;
    }
    int recieved_bytes = 0;
    while (recieved_bytes < kDataLength) {
      int max_len = kDataLength - recieved_bytes;
      int tmp = client.Receive(&bigbuff[recieved_bytes], max_len);
      ASSERT_GE(tmp, 0);
      recieved_bytes += tmp;
    }
    for (size_t i = 0; i < kDataLength; ++i) {
      ASSERT_EQ(bigbuff[i], 'x');
    }
  }
  wait(0);
}