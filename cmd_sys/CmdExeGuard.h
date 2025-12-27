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
 * @class CmdExeGuard
 * @brief Pure virual base for classes guarding execution.
 */

#pragma once

#include <QString>

///@view:beg

//=============================================================================
class  CmdExeGuard {
//=============================================================================
public:
enum Phase {
    Record = 0,
    Update = 100
};

//! @section Construction
CmdExeGuard(Phase phase = Record) : phase_(phase) {};
    virtual ~CmdExeGuard() = default;
//! @section Methods
    Phase phase() { return phase_; }
    virtual void commandExecuted() = 0;
//=============================================================================

    Phase phase_;
};

///@view:end
