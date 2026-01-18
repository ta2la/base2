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

#include <QString>

/// @view:beg

struct CodeNodeAddress
{
    QString module_;
    QString node_;

    CodeNodeAddress() = default;

    CodeNodeAddress(const QString& module,
                    const QString& node)
        : module_(module)
        , node_(node)
    {}

    bool isValid() const
    {
        return !module_.isEmpty() && !node_.isEmpty();
    }

    bool operator==(const CodeNodeAddress& other) const
    {
        return module_ == other.module_
               && node_   == other.node_;
    }
};

/// @view:end
