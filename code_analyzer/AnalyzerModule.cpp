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

#include "AnalyzerModule.h"
#include "Cmds_code_analyzer.h"

/// @view:beg

AnalyzerModule::AnalyzerModule(const QString& dirPath, bool used)
        : dirPath_(dirPath),
        used_(used)
    {
        /*QStringList demoFiles {
            "main.cpp",
            "controller.h",
            "view.qml"
        };*/

        /*const QString moduleName = QDir(dirPath_).dirName();

        AnalyzerSys& sys = Cmds_code_analyzer::sys_;

        QStringList nodes = sys.nodeNamesForModule(moduleName);*/

        filesModel_ = new AnalyzerModuleFilesModel();
    }

void AnalyzerModule::buildFilesModel()
    {
    const QString moduleName = QDir(dirPath_).dirName();
    AnalyzerSys& sys = Cmds_code_analyzer::sys_;

    const QStringList nodes = sys.nodeNamesForModule(moduleName);
    filesModel_->resetFromNames(nodes);
    }

/// @view:end
