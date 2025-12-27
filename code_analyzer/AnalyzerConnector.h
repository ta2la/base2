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
#pragma once

#include "AnalyzerNode.h"

/// @view:beg

class AnalyzerConnector
//=============================================================================
{
public:
//! @section Construction
    AnalyzerConnector(const QString& node1, const QString& node2) :
        node1_(node1),
        node2_(node2)
    {
    }

    //! @section Accessors
    const QString& node1() const { return node1_; }
    const QString& node2() const { return node2_; }

    //node1-node2
    static QString signature(const QString& node1, const QString& node2)
    {
            return node1 + "-" + node2;
    }

    QString signature() const { return signature(node1_, node2_); }

    //=============================================================================
protected:
//! @section Data
    QString node1_;
    QString node2_;
};

/// @view:beg
