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
* \file file_util.h
* \brief This file contains facilitlies to control the file.
*/
#ifndef XFOREST_BASE_FILE_UTIL_H_
#define XFOREST_BASE_FILE_UTIL_H_

#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "src/base/common.h"
#include "src/base/stringprintf.h"
#include "src/base/scoped_ptr.h"

/*!
* Basic operations for a file
*/
const size_t KB = 1024.0;
const size_t MB = 1024.0 * 1024.0;
const size_t GB = 1024.0 * 1024.0 * 1024.0;

/*!
* \brief Max size of one line TXT data
*/
static const uint32 kMaxLineSize = 500 * 1024;  // 500 KB

/*!
* \brief Max chunk size of hash block 
*/
static const uint32 kChunkSize = 1000 * 1024; // 1000 KB

/*!
* \brief Check whether the file exists.
* \param filename name of file
* \return true for success and false for error.
*/
inline bool FileExist(const char* filename) {
  if (access(filename, F_OK) != -1) {
    return true;
  }
  LOG(WARNING) << "File: " << filename << " doesn't exist.";
  return false;
}

/*!
* \brief Open file using fopen() and return the file pointer.
* \param filename name of file
* \param mode 'w' for write and 'r' for read
* \return file pointer
*/
inline FILE* OpenFileOrDie(const char* filename, const char* mode) {
  CHECK_NOTNULL(filename);
  CHECK_NOTNULL(mode);
  FILE* input_stream = fopen(filename, mode);
  if (input_stream == nullptr) {
    LOG(FATAL) << "Cannot open file: " << filename
               << " with mode: " << mode;
  }
  return input_stream;
}

/*!
* \brief Close file using fclose() by given the file pointer.
* \param FILE file pointer
*/
inline void Close(FILE *file) {
  CHECK_NOTNULL(file);
  if (fclose(file) == -1) {
    LOG(FATAL) << "Error: invoke fclose().";
  }
}

/*!
* \brief Return the size (byte) of a target file.
* \param FILE file pointer
* \return file size
*/
inline uint64 GetFileSize(FILE* file) {
  CHECK_NOTNULL(file);
  if (fseek(file, 0L, SEEK_END) != 0) {
    LOG(FATAL) << "Error: invoke fseek().";
  }
  // Note that we use uin64 here for big file
  uint64 total_size = ftell(file);
  if (total_size == -1) {
    LOG(FATAL) << "Error: invoke ftell().";
  }
  // Return to the head of file
  rewind(file);
  return total_size;
}

/*!
* \brief Get one line of data from file by given a file pointer.
* \param FILE file pointer
* \param str_line string of one line
*/
inline void GetLine(FILE* file, std::string& str_line) {
  CHECK_NOTNULL(file);
  scoped_array<char> line(new char[kMaxLineSize]);
  CHECK_NOTNULL(fgets(line.get(), kMaxLineSize, file));
  int read_len = strlen(line.get());
  if (line[read_len - 1] != '\n') {
    LOG(FATAL) << "Encountered a too-long line: \
                   Cannot find the '\n' char.   \
                   Please check the data.";
  } else {
    line[read_len - 1] = '\0';
    // Handle the format in DOS and windows
    if (read_len > 1 && line[read_len - 2] == '\r') {
      line[read_len - 2] = '\0';
    }
  }
  str_line.assign(line.get());
}

/*!
* \brief Write a block of data from a buffer to disk file.
* \param FILE file pointer
* \param buf data buffer
* \param len length of data
* \return size (byte) of data that we write to this file
*/
inline size_t WriteDataToDisk(FILE* file, const char* buf, size_t len) {
  CHECK_NOTNULL(file);
  CHECK_NOTNULL(buf);
  CHECK_GT(len, 0);
  size_t write_len = fwrite(buf, 1, len, file);
  if (write_len != len) {
    LOG(FATAL) << "Error: invoke fwrite().";
  }
  return write_len;
}

/*!
* \brief Read a block data from disk file to a buffer.
* \param FILE file pointer
* \param buf data buffer
* \param len length of data
* \return data size (byte) we read from the file. 
* If we reach the end of the file, return 0.
*/
inline size_t ReadDataFromDisk(FILE* file, char* buf, size_t len) {
  CHECK_NOTNULL(file);
  CHECK_NOTNULL(buf);
  CHECK_GE(len, 0);
  // Reach the end of the file
  if (feof(file)) {
    return 0;
  }
  size_t ret = fread(buf, 1, len, file);
  if (ret > len) {
    LOG(FATAL) << "Error: invoke fread().";
  }
  return ret;
}

/*!
* \brief Delete target file from disk.
* \param filename name of file
*/
inline void RemoveFile(const char* filename) {
  CHECK_NOTNULL(filename);
  if (remove(filename) == -1) {
    LOG(FATAL) << "Error: invoke remove().";
  }
}

/*!
* \brief Format the file size by GB, MB, and KB.
* \param file_size size of file
* \return format string
*/
inline std::string PrintSize(uint64 file_size) {
  std::string res;
  if (file_size > GB) {
    SStringPrintf(&res, "%.2f GB",
            (double) file_size / GB);
  } else if (file_size > MB) {
    SStringPrintf(&res, "%.2f MB",
            (double) file_size / MB);
  } else {
    SStringPrintf(&res, "%.2f KB",
            (double) file_size / KB);
  }
  return res;
}

/*!
* \brief Read the whole file to a memory buffer.
* \param filename name of file
* \param buf data buffer
* \return size (byte) of current file
*/
inline uint64 ReadFileToMemory(const std::string& filename, char **buf) {
  CHECK(!filename.empty());
  FILE* file = OpenFileOrDie(filename.c_str(), "r");
  uint64 len = GetFileSize(file);
  try {
    *buf = new char[len];
  } catch (std::bad_alloc&) {
    LOG(FATAL) << "Cannot allocate enough memory for Reader.";
  }
  uint64 read_size = fread(*buf, 1, len, file);
  CHECK_EQ(read_size, len);
  Close(file);
  return len;
}

/////////////// Serialize or Deserialize for std::vector and std::string ///////////////

/*!
* \brief Write a std::vector to disk file.
* \param file_ptr file pointer
* \param vec data vector
*/
template <typename T>
void WriteVectorToFile(FILE* file_ptr, const std::vector<T>& vec) {
  CHECK_NOTNULL(file_ptr);
  // We do not want to serialize an empty vector
  CHECK(!vec.empty());
  size_t len = vec.size();
  // First, write the length of this vector
  WriteDataToDisk(file_ptr, 
    reinterpret_cast<char*>(&len), 
    sizeof(len));
  // Then, write the data of this vector
  WriteDataToDisk(file_ptr, 
    (char*)(vec.data()), 
    sizeof(T)*len);
}

/*!
* \brief Read a std::vector from disk file.
* \param file_ptr file pointer
* \param vec data vector
*/
template <typename T>
void ReadVectorFromFile(FILE* file_ptr, std::vector<T>& vec) {
  CHECK_NOTNULL(file_ptr);
  // First, read the length of vector
  size_t len = 0;
  ReadDataFromDisk(file_ptr, 
    reinterpret_cast<char*>(&len), 
    sizeof(len));
  CHECK_GT(len, 0);
  // Clear the original vector
  std::vector<T>().swap(vec);
  vec.resize(len);
  ReadDataFromDisk(file_ptr, 
    reinterpret_cast<char*>(vec.data()), 
    sizeof(T)*len);
}

/*!
* \brief Write a std::string to disk file.
* \param file_ptr file pointer
* \param str data string
*/
inline void WriteStringToFile(FILE* file_ptr, const std::string& str) {
  CHECK_NOTNULL(file_ptr);
  // We do not want to serialize an empty string
  CHECK(!str.empty());
  size_t len = str.size();
  // First, write the length of string
  WriteDataToDisk(file_ptr, 
    reinterpret_cast<char*>(&len), 
    sizeof(len));
  // Then, write the data of string
  WriteDataToDisk(file_ptr, 
    const_cast<char*>(str.data()), 
    len);
}

/*!
* \brief Read a std::string from disk file.
* \param file_ptr file pointer
* \param str data string
*/
inline void ReadStringFromFile(FILE* file_ptr, std::string& str) {
  CHECK_NOTNULL(file_ptr);
  // First, read the length of vector
  size_t len = 0;
  ReadDataFromDisk(file_ptr, 
    reinterpret_cast<char*>(&len), 
    sizeof(len));
  CHECK_GT(len, 0);
  // Clear the original string
  std::string().swap(str);
  str.resize(len);
  ReadDataFromDisk(file_ptr, 
    const_cast<char*>(str.data()), 
    len);
}

/////////////// Tool function used by Reader class of xforest ///////////////

/*!
* \brief Calculate the hash value of current txt file.
* \param filename name of file
* \param one_block, a flag:
*  If one_block == true, we just read a small chunk of data.
*  If one_block == false, we read all the data from the file.
* \return hash value
*/
inline uint64_t HashFile(const std::string& filename, bool one_block=false) {
  std::ifstream f(filename, std::ios::ate | std::ios::binary);
  if(f.bad()) { return 0; }

  long end = (long) f.tellg();
  f.seekg(0, std::ios::beg);
  CHECK_EQ(static_cast<int>(f.tellg()), 0);

  uint64_t magic = 90359;
  for (long pos = 0; pos < end; ) {
    long next_pos = std::min(pos + kChunkSize, end);
    long size = next_pos - pos;
    std::vector<char> buffer(kChunkSize);
    f.read(buffer.data(), size);

    int i = 0;
    while (i < size - 8) {
      uint64_t x = *reinterpret_cast<uint64_t*>(buffer.data() + i);
      magic = ( (magic + x) * (magic + x + 1) >> 1) + x;
      i += 8;
    }
    for (; i < size; i++) {
      char x = buffer[i];
      magic = ( (magic + x) * (magic + x + 1) >> 1) + x;
    }

    pos = next_pos;
    if (one_block) { break; }
  }

  return magic;
}

#endif  // XFOREST_BASE_FILE_UTIL_H_
