//
// Copyright (C) 2025 Petr Talla. [petr.talla@gmail.com]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//		      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//=============================================================================
/**
 * @class ArrtMacro ...
 * @brief Macros for attribute definition
 */
#pragma once

#define ATTR(TYPE, NAME)                   \
public:                                    \
    TYPE NAME() { return NAME##_; }        \
    void NAME##Set(TYPE value) {           \
        NAME##_ = value;                   \
    }                                      \
private:                                   \
    TYPE NAME##_;                          \
public:

#define ATTR_OBJ(TYPE, NAME)               \
public:                                    \
    const TYPE& NAME() { return NAME##_; } \
    void NAME##Set(const TYPE& value) {    \
        NAME##_ = value;                   \
    }                                      \
private:                                   \
    TYPE NAME##_;                          \
public:
