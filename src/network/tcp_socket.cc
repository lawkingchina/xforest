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
This file is the implementation the TCPSocket class.
*/

#include "src/network/tcp_socket.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>

namespace xforest {

typedef struct sockaddr_in SAI;
typedef struct sockaddr SA;

// ctor
TCPSocket::TCPSocket() {
  // init socket
  socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_ < 0) {
    LOG(FATAL) << "Can't create new socket.";
  }
}

// dctor
TCPSocket::~TCPSocket() {
  Close();
}

bool TCPSocket::Connect(const char * ip, uint16 port) {
  SAI sa_server;
  sa_server.sin_family      = AF_INET;
  sa_server.sin_port        = htons(port);

  if (0 < inet_pton(AF_INET, ip, &sa_server.sin_addr) &&
      0 <= connect(socket_, reinterpret_cast<SA*>(&sa_server),
                   sizeof(sa_server))) {
    return true;
  }

  LOG(ERROR) << "Failed connect to " << ip << ":" << port;
  return false;
}

bool TCPSocket::Bind(const char * ip, uint16 port) {
  // Avoid TIME_WAIT for last connection
  const int on=1;
  setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

  SAI sa_server;
  sa_server.sin_family      = AF_INET;
  sa_server.sin_port        = htons(port);

  if (0 < inet_pton(AF_INET, ip, &sa_server.sin_addr) &&
      0 <= bind(socket_, reinterpret_cast<SA*>(&sa_server),
                sizeof(sa_server))) {
    return true;
  }

  LOG(ERROR) << "Failed bind on " << ip << ":" << port;
  return false;
}

bool TCPSocket::Listen(int max_connection) {
  if (0 <= listen(socket_, max_connection)) {
    return true;
  }

  LOG(ERROR) << "Failed listen on socket fd: " << socket_;
  return false;
}

bool TCPSocket::Accept(TCPSocket * socket, std::string * ip, uint16 * port) {
  int sock_client;
  SAI sa_client;
  socklen_t len = sizeof(sa_client);

  sock_client = accept(socket_, reinterpret_cast<SA*>(&sa_client), &len);
  if (sock_client < 0) {
    LOG(ERROR) << "Failed accept connection on " << ip << ":" << port;
    return false;
  }

  char tmp[INET_ADDRSTRLEN];
  const char * ip_client = inet_ntop(AF_INET,
                                     &sa_client.sin_addr,
                                     tmp,
                                     sizeof(tmp));
  CHECK_NOTNULL(ip_client);
  ip->assign(ip_client);
  *port = ntohs(sa_client.sin_port);
  socket->socket_ = sock_client;

  return true;
}

bool TCPSocket::SetBlocking(bool flag) {
  int opts;

  if ((opts = fcntl(socket_, F_GETFL)) < 0) {
    LOG(ERROR) << "Failed to get socket status.";
    return false;
  }

  if (flag) {
    opts |= O_NONBLOCK;
  } else {
    opts &= ~O_NONBLOCK;
  }

  if (fcntl(socket_, F_SETFL, opts) < 0) {
    LOG(ERROR) << "Failed to set socket status.";
    return false;
  }

  return true;
}

bool TCPSocket::ShutDown(int ways) {
  return 0 == shutdown(socket_, ways);
}

void TCPSocket::Close() {
  if (socket_ >= 0) {
    CHECK_EQ(0, close(socket_));
    socket_ = -1;
  }
}

int TCPSocket::Send(const char * data, int len_data) {
  return send(socket_, data, len_data, 0);
}

int TCPSocket::Receive(char * buffer, int size_buffer) {
  return recv(socket_, buffer, size_buffer, 0);
}

int TCPSocket::Socket() const {
  return socket_;
}

}  // namespace xforest