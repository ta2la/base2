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

#include "CodeData.h"
#include "Cmds_oreg_test.h"

/// @view:beg

class AnalyzerSys
//=============================================================================
{
public:
/// @section Construction
    AnalyzerSys() = default;
/// @section Neighbours
    //AnalyzerNet& net() { return net_; }

/// @section Methods
    //=================================
    void add(const QString& filePath, const QString& moduleName)
    {
        const QString name =
            AnalyzerNode::nameFromFilePath(filePath);

        auto it = nodes_.find(name);
        if (it == nodes_.end()) {
            /*nodes_.insert(
                name,
                std::make_unique<AnalyzerNode>(filePath, moduleName)
                );*/
            nodes_.emplace(
                name,
                std::make_unique<AnalyzerNode>(filePath, moduleName)
                );
        } else {
            //it.value()->insertSuffix(filePath);
            it->second->insertSuffix(filePath);
        }

        //net_.addIncludesFromFile(filePath);
    }

    //=================================
    AnalyzerNode* node(const QString& name) const
    {
        auto it = nodes_.find(name);
        return (it == nodes_.end()) ? nullptr : it->second.get();
    }

    //=================================
    QString toStringDebug() const
    {
        QStringList parts;

        for (auto it = nodes_.cbegin(); it != nodes_.cend(); ++it) {
            parts.append(it->second->toStringDebug());
        }

        return parts.join(" ");
    }

//=============================================================================
protected:
    /// @section Internals
    //=================================

    QStringList nodeNamesForModule(const QString& module) const
    {
        QStringList result;

        for (auto it = nodes_.cbegin(); it != nodes_.cend(); ++it) {
            const AnalyzerNode& n = *it->second;
            if (n.module() == module)
                result.append(n.name());
        }

        result.sort();
        return result;
    }

    bool isNodeExportableByDist(const QString& name) const
    {
        auto it = nodes_.find(name);
        if (it == nodes_.end())
            return false;

        const double d = it->second->distToCenter();
        return std::isfinite(d);
    }

    QString targetingPrompt() const
    {
        QString out;
        QTextStream s(&out);

        const AnalyzerNode* centerNode = node(center_);

        const QString centerModule =
            centerNode ? centerNode->module() : QString("<unknown>");

        //s << "// TARGETING MODULE: " << centerModule << "\n";
        s << "// TARGETING MODULE: code_data\n";

        return out;
    }


/// @section Data
    std::map<QString, std::unique_ptr<AnalyzerNode>> nodes_;
    //AnalyzerNet                                      net_;
    QString                                          center_ = "AnalyzerSys";

    friend class AnalyzerModule;
    friend class AnalyzerDistCalc;
    friend class AnalyzerModuleFileData;
    friend class Cmds_code_analyzer;
};

/// @view:end
