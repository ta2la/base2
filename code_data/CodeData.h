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
 * @class CodeData
 * @brief
 */
#pragma once

#include "CodeNodeAddress.h"
#include "CodeModuleCol.h"

/// @view:beg

class  CodeData
//=============================================================================
{
public:
    //! @section Construction
    CodeData();
    static CodeData& inst() { static CodeData i;  return i; }
    //! @section Neighbours
    CodeModuleCol& modules()       { return modules_; }
    const CodeModuleCol& modules() const { return modules_; }
    //! @section Methods
    QString toDot() const;

    CodeNodeAddress center() {return center_; }
//=============================================================================
protected:
    CodeModuleCol   modules_;
    CodeNodeAddress center_ = { "code_analyzer", "AnalyzerSys" };
    double distToCenter_ = 1;

    friend class AnalyzerDistCalc;
    friend class Cmds_code_analyzer;
};

/// @view:end
