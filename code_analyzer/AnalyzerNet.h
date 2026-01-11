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

#include "AnalyzerConnector.h"
#include "AnalyzerCode.h"

/// @view:beg

class AnalyzerNet
//=============================================================================
{
public:
    /// @section Construction
    AnalyzerNet() = default;

/// @section Methods
    //=================================
    bool add(const QString& node1, const QString& node2)
    {
        const QString sig = AnalyzerConnector::signature(node1, node2);

        if (connectors_.contains(sig))
            return false;

        connectors_.insert(sig, AnalyzerConnector(node1, node2));
        return true;
    }

    //=================================
    void addIncludesFromFile(const QString& filePath)
    {
        QString log("logcmd ");

        const QString from = AnalyzerNode::nameFromFilePath(filePath);

        log += from + " -> ";

        const QStringList includes = AnalyzerCode::extractIncludes(filePath);

        log += includes.join(" ");

        for (const QString& inc : includes) {
            const QString to = QFileInfo(inc).completeBaseName();
            if (from == to) continue;
            if (!to.isEmpty()) add(from, to);
        }

        log += " -> " + QString::number(connectors_.size());
        CMD_SYS.execute(log);
    }

//=============================================================================
protected:
/// @section Internals
    //=================================
    QString toDot_() const
    {
        QString out;
        QTextStream s(&out);

        for (auto it = connectors_.cbegin(); it != connectors_.cend(); ++it) {
            const AnalyzerConnector& c = it.value();
            s << "  \"" << c.node1() << "\" -> \"" << c.node2() << "\";\n";
        }

        return out;
    }

/// @section Data
    QMap<QString, AnalyzerConnector> connectors_;

/// @section friends
    friend class AnalyzerSys;
    friend class AnalyzerDistCalc;
};

/// @view:end
