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

#include "AnalyzerModuleFilesModel.h"

#include <QString>
#include <QFileInfo>
#include <QDir>

/// @view:beg

struct AnalyzerModuleData
//=============================================================================
{
    Q_GADGET

    Q_PROPERTY(QString dirPath READ dirPath CONSTANT)
    Q_PROPERTY(QString dirPathModuleLess READ dirPathModuleLess CONSTANT)
    Q_PROPERTY(QString module  READ module  CONSTANT)
    Q_PROPERTY(bool    used    READ used    CONSTANT)
    Q_PROPERTY(QObject* files   READ files   CONSTANT)

public:
    AnalyzerModuleData()
        : dirPath_()
        , module_()
        , used_(false)
        , filesModel_(nullptr)
    {}

    explicit AnalyzerModuleData(
        const QString& dirPath,
        bool used,
        QObject* filesModel )
        : dirPath_(dirPath)
        , module_(QDir(dirPath).dirName())
        , used_(used)
        , filesModel_(filesModel)
    {}

    QString dirPath() const { return dirPath_; }
    QString module()  const { return module_; }
    bool used() const { return used_; }
    QString dirPathModuleLess() const { return QFileInfo(dirPath_).dir().absolutePath(); }
    QObject* files() const { return filesModel_; }

private:
    QString dirPath_;
    QString module_;
    bool    used_;
    QObject* filesModel_ = nullptr;
};

Q_DECLARE_METATYPE(AnalyzerModuleData)

class AnalyzerModule
//=============================================================================
{
public:
    /// @section Construction
    explicit AnalyzerModule(const QString& dirPath, bool used = true)
        : dirPath_(dirPath),
        used_(used)
    {
        QStringList demoFiles {
            "main.cpp",
            "controller.h",
            "view.qml"
        };

        filesModel_ =
            new AnalyzerModuleFilesModel(demoFiles);
    }

    /// @section Accessors
    const QString& dirPath() const { return dirPath_; }

    bool used() const { return used_; }
    void setUsed(bool value) { used_ = value; }

    AnalyzerModuleData data() const
    {
        return AnalyzerModuleData(dirPath_, used_, filesModel_);
    }

//=============================================================================
protected:
    /// @section Data
    QString dirPath_;
    bool    used_;
    QObject* filesModel_ = nullptr;
};

/// @view:end
