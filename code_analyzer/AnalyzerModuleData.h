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
#include "CodeData.h"
#include "CodeModule.h"

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
    Q_PROPERTY(bool    subdirs    READ used    CONSTANT)
    Q_PROPERTY(QObject* files   READ files   CONSTANT)

public:
    AnalyzerModuleData()
        : dirPath_()
        , module_()
        , used_(false)
        //, filesModel_(nullptr)
        , subdirs_(true)
    {}

    explicit AnalyzerModuleData(
        const QString& dirPath,
        bool used = true,
        //AnalyzerModuleFilesModel* filesModel,
        bool subdirs = true)
        : dirPath_(dirPath)
        , module_(QDir(dirPath).dirName())
        , used_(used)
        //, filesModel_(filesModel)
        , subdirs_(subdirs)
    {
        filesModel_ = new AnalyzerModuleFilesModel();
        buildFilesModel_();
    }

    void setUsed(bool val) { used_ = val; }
    QString dirPath() const { return dirPath_; }
    QString module()  const { return module_; }
    bool used() const { return used_; }
    QString dirPathModuleLess() const { return QFileInfo(dirPath_).dir().absolutePath(); }
    QObject* files() const { return filesModel_; }

    void buildFilesModel_()
    {
        const QString moduleName = QDir(dirPath_).dirName();
        CodeModule* module = CodeData::inst().modules().get(moduleName);
        QStringList nodes = module->nodes().names();
        filesModel_->resetFromNames(nodes);
    }

private:
    QString dirPath_;
    QString module_;
    bool    used_;
     AnalyzerModuleFilesModel* filesModel_ = nullptr;
    bool subdirs_ = true;

     friend class AnalyzerModuleCol;
};

Q_DECLARE_METATYPE(AnalyzerModuleData)

/// @view:end
