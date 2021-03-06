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
* \file stringprintf.h
* \brief This file provides stringprintf utilities.
*/
#ifndef XFOREST_BASE_STRING_PRINTF_H_
#define XFOREST_BASE_STRING_PRINTF_H_

#include <string>

//------------------------------------------------------------------------------
// This code comes from the re2 project host on Google Code
// (http://code.google.com/p/re2/), in particular, the following source file
// http://code.google.com/p/re2/source/browse/util/stringprintf.cc
//------------------------------------------------------------------------------

/*!
* \brief For example:
* std::string str = StringPrintf("%d", 1);    // str = "1"
*/
std::string StringPrintf(const char* format, ...);

/*!
* \brief For example:
* SStringPrintf(&str, "%d", 2);               // str = "2"
*/
void SStringPrintf(std::string* dst, const char* format, ...);

/*!
* \brief For example:
* StringAppendF(&str, "%d", 3);               // str = "23"
*/
void StringAppendF(std::string* dst, const char* format, ...);

#endif   // XFOREST_BASE_STRING_PRINTF_H_
