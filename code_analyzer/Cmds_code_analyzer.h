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
 * @class Cmds_test
 * @brief Basic test of CmdSys.
 */
#pragma once

#include "CmdSys.h"
//#include "AnalyzerCode.h"
//#include "AnalyzerSys.h"
#include "AnalyzerModuleFileData.h"
#include "AnalyzerDistCalc.h"
#include "CodeData.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QFileDialog>
#include <QString>
#include <AnalyzerModuleCol.h>

///@view:beg
class  Cmds_code_analyzer {
///@view:end
//=============================================================================
public:
//! @section Construction
    Cmds_code_analyzer() = delete;
//<METHODS>
    static void registerCmds_() {

//@view:beg
    CMD_SYS.add("set_module_used",
    [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {

        if (args.count() < 3)
            return args.appendError(
                "usage: set_module_used <index> <0|1>");

        bool okIndex = false;
        bool okUsed  = false;

        const int index =
            args.get(1).value().toInt(&okIndex);
        const int usedInt =
            args.get(2).value().toInt(&okUsed);

        if (!okIndex || !okUsed)
            return args.appendError("invalid arguments");

        const bool used = (usedInt != 0);

        AnalyzerModuleCol::inst().setModuleUsed(index, used);

        args.append(
            QString("module[%1] -> %2")
                .arg(index)
                .arg(used ? "used" : "unused"),
            "OK");

        return 0;
    });
    CMD_SYS.add("analyzer_set_center",
    [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {

        if (args.count() < 3)
            return args.appendError("usage: analyzer_set_center <nodeName> <moduleName>");

        const QString name = args.get(1).value();
        const QString module = args.get(2).value();

        CodeData& data = CodeData::inst();

        if (!data.modules().get(
                CodeNodeAddress(QString(), name)))
            return args.appendError("unknown node: " + name);

        data.center_ = CodeNodeAddress(module, name);

        // recompute distances using code_data
        AnalyzerDistCalc calc(data);
        calc.calculate();
        calc.addObservers();

        AnalyzerModuleCol::inst().resetAllFilesModels();

        args.append("center -> " + name, "OK");

        return 0;
    });


    }
//=============================================================================
protected:
    /// @section Data
    friend class Cmds_code_analyzer_test;
    friend int main(int, char**);
    friend class AnalyzerCode;
    friend struct AnalyzerModuleFileData;
    friend class AnalyzerModule;
    friend class Cmds_code_analyzer;
    friend class AnalyzerDistCalc;
};
///@view:end
