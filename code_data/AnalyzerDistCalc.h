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
#pragma once

#include "CodeData.h"

#include <QAbstractListModel>
#include <QStringList>

/// @view:beg

class AnalyzerDistCalc
{
public:
    AnalyzerDistCalc(CodeData& data)
        : data_(&data)
    {
        buildGraph();
    }

    void buildGraph();
    void calculate();
    void calculate1(const CodeNodeAddress& centerAddr, bool firstPass);
    void addObservers();

    struct Edge {
        QString to;
        double w;
    };

protected:
    QMap<QString, QList<Edge>> graph_;
    CodeData*                  data_;

    static bool mergeDist(double& oldVal, double newVal);
};

/// @view:end
