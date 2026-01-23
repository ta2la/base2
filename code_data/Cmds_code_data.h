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
/**
 * @class Cmds_test
 * @brief Basic test of CmdSys.
 */
#pragma once

#include "CmdSys.h"
#include "CodeData.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QString>
#include <QFile>
#include <QDir>

///@view:beg
class  Cmds_code_data {
///@view:end
//=============================================================================
public:
//! @section Construction
    Cmds_code_data() = delete;
//<METHODS>
    static void createModel() {
        static bool initialized = false;
        if (!initialized) {
            initialized = true;
        }
    }
    static void registerCmds_() {

//@view:beg
    CMD_SYS.add("dir_export_dot",
    [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {

        // ✅ code_data: přístup k modulům
        CodeModuleCol& modules = CodeData::inst().modules();

        if (modules.count() == 0)
            return args.appendError("no module to analyze");

        // vezmeme první modul deterministicky
        const QString moduleName = modules.names().first();
        CodeModule* module = modules.get(moduleName);

        if (!module)
            return args.appendError("invalid module");

        // ✅ code_data → path modulu
        QDir dir(module->path());
        if (!dir.exists())
            return args.appendError("module directory does not exist");

        if (dir.isRoot())
            return args.appendError("dir_export_dot: root not possible");

        // výstupní .dot soubor vedle modulu
        QString base = QDir::cleanPath(module->path());
        QString dotFileName = base + ".dot";

        // ✅ FS
        QFile outFile(dotFileName);
        if (!outFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
            return args.appendError("dir_export_dot: cannot open output file");

        // ✅ čistá analýza (code_data)
        QByteArray dot = CodeData::inst().toDot().toUtf8();
        outFile.write(dot);
        outFile.close();

        // report
        args.append(dotFileName, "RESULT_DOT");

        // UI/system akce (zůstávají)
        dir.cdUp();
        args.append("<a href='system_open_path " +
                    dir.absolutePath() +
                    "'>[OPEN DIR]</a>");
        args.append("<a href='system_dot_to_svg " +
                    dotFileName +
                    "'>[DOT-&gtSVG]</a>");

        return 0;
    });
    }
//=============================================================================
protected:
    /// @section Data
};
///@view:end
