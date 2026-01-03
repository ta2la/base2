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

#include "AnalyzerCode.h"
#include "Cmds_code_analyzer.h"

/// @view:beg

void AnalyzerCode::loadDot()
{
    // singleton
    //Cmds_code_analyzer::sys_.clear();

    for (int i = 0; i < Cmds_code_analyzer::dirs_.count(); ++i) {

        const AnalyzerModule& mod = Cmds_code_analyzer::dirs_.get(i);
        if (!mod.used())
            continue;

        QDir moduleDir(mod.dirPath());
        if (!moduleDir.exists())
            continue;

        const QString moduleName = moduleDir.dirName();

        const QStringList files =
            AnalyzerCode::getFiles(
                moduleDir,
                QStringList() << "*.h" << "*.cpp" << "*.qml"
                );

        for (const QString& filePath : files) {
            Cmds_code_analyzer::sys_.add(filePath, moduleName);
        }
    }
}

/// @view:end
