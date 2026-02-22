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
#include "CodeSettings.h"

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

        bool okUsed  = false;

        const QString module = args.get(1).value();
        const int usedInt    = args.get(2).value().toInt(&okUsed);

        if (!okUsed) return args.appendError("invalid arguments");

        const bool used = (usedInt != 0);

        CodeModule* mod = CodeData::inst().modules().get(module);

        if (!mod) return args.appendError("module not found");

        OregUpdateLock l;

        mod->setUsed(used);
        mod->oo_changed();

        args.append(
            QString("module[%1] -> %2")
                .arg(module)
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

        //CodeData::inst().center_ = CodeNodeAddress(module, name);
        //CodeData::inst().setCenter( CodeNodeAddress(module, name) );
        //data.center_.append(CodeNodeAddress(module, name));
        data.setCenter(CodeNodeAddress(module, name));

        // recompute distances using code_data
        AnalyzerDistCalc calc(data);
        calc.calculate();
        calc.addObservers();

        //AnalyzerModuleCol::inst().resetAllFilesModels();

        args.append("center -> " + name, "OK");

        return 0;
    });
    CMD_SYS.add("update_claude_md",
    [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {

        if (args.count() < 2)
            return args.appendError("usage: update_claude_md <moduleName>");

        const QString moduleName = args.get(1).value();

        AnalyzerModuleCol& analyzer = AnalyzerModuleCol::inst();

        // Najdi modul podle jména
        const AnalyzerModuleData* foundModule = nullptr;
        for (int i = 0; i < analyzer.count(); i++) {
            const AnalyzerModuleData& moduleData = analyzer.get(i);
            // moduleData.dirPath() vrací cestu, potřebujeme jméno
            // Jméno je poslední část cesty
            QString dirName = QFileInfo(moduleData.dirPath()).fileName();
            if (dirName == moduleName) {
                foundModule = &moduleData;
                break;
            }
        }

        if (!foundModule)
            return args.appendError(QString("module not found: %1").arg(moduleName));

        const QString exeModulePath = foundModule->dirPath();

        // Postav obsah CLAUDE.md
        QString content = "## Source Modules\n";

        // Cyklus po všech modulech
        for (int i = 0; i < analyzer.count(); i++) {
            const AnalyzerModuleData& moduleData = analyzer.get(i);
            const QString modulePath = moduleData.dirPath();

            // Extrahuj relativní cestu vůči exeModulePath
            QString relPath;
            if (modulePath == exeModulePath) {
                relPath = "./";
            } else {
                QDir baseDir(exeModulePath);
                relPath = baseDir.relativeFilePath(modulePath);
            }

            // Typ je executable jen pro cílový modul, ostatní jsou lib
            QString moduleType = (modulePath == exeModulePath) ? "executable" : "static lib";

            // Přidej relativní cestu a typ modulu
            content += QString("- %1 (%2)\n").arg(relPath, moduleType);
        }

        // Zapiš do CLAUDE.md
        const QString claudeMdPath = QDir::cleanPath(exeModulePath + "/CLAUDE.md");
        QFile claudeFile(claudeMdPath);
        if (!claudeFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
            return args.appendError("cannot write CLAUDE.md: " + claudeMdPath);

        claudeFile.write(content.toUtf8());
        claudeFile.close();

        args.append(claudeMdPath, "CLAUDE_MD_UPDATED");

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
