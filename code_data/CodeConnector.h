//
// Copyright (C) 2026 Petr Talla. [petr.talla@gmail.com]
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

#include <QString>

/// @view:beg
class CodeConnector
{
public:
    CodeConnector(
        const CodeNodeAddress& node1,
        const CodeNodeAddress& node2)
        : node1Address_(node1)
        , node2Address_(node2)
    {}

    // existing API (AnalyzerDistCalc-compatible)
    const QString& node1() const { return node1Address_.node; }
    const QString& node2() const { return node2Address_.node; }

    // address-level access (new, additive)
    const CodeNodeAddress& node1Address() const { return node1Address_; }
    const CodeNodeAddress& node2Address() const { return node2Address_; }

    // node1-node2 (node name is globally unique)
    static QString signature(
        const CodeNodeAddress& node1,
        const CodeNodeAddress& node2)
    {
        return node1.node + "-" + node2.node;
    }

    QString signature() const
    {
        return signature(node1Address_, node2Address_);
    }

protected:
    CodeNodeAddress node1Address_;
    CodeNodeAddress node2Address_;
};

/// @view:end
