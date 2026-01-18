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

#include "CodeModuleCol.h"

#include <QSet>

class CodeModule;

/// @view:beg

class CodeNode : public OregObject
{
public:
    CodeNode(
        const QString& filePath,
        CodeModule* module
        );

    const QString& dir() const    { return dir_; }
    const QString& name() const   { return name_; }
    CodeModule*    module() const { return module_; }

    QStringList extensions() const;

    void addExtension(const QString& ext);

    QString oo_to_string(EStringFormat format = SF_BASIC) const override;
    void loadDependencies(const QString& filePath);

protected:
    QString     dir_;        // full file path
    QString     name_;       // base name (no suffix)
    CodeModule* module_ = nullptr;

    QSet<QString> extensions_;
    QSet<QString> dependencies_;

    static QStringList extractIncludes_(const QString& filePath);

    friend class AnalyzerNet;
};


/// @view:end
