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

#include <QAbstractListModel>
#include <QStringList>

/// @view:beg

class CodeNode;

struct AnalyzerModuleFileData
{
    Q_GADGET

    Q_PROPERTY(QString name   READ name CONSTANT)
    Q_PROPERTY(double  dist   READ dist CONSTANT)
    Q_PROPERTY(bool    center READ isCenter CONSTANT)
    Q_PROPERTY(QStringList types  READ types CONSTANT)

public:
    AnalyzerModuleFileData() = default;
    explicit AnalyzerModuleFileData(const QString& name);

    QString name() const;
    double  dist() const;
    bool    isCenter() const;
    QStringList types() const;

private:
    const CodeNode* node_ = nullptr;
};

Q_DECLARE_METATYPE(AnalyzerModuleFileData)

/// @view:end
