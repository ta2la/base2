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
#include "AnalyzerCode.h"
#include "AnalyzerSys.h"
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
    static void createModel() {
        static bool initialized = false;
        if (!initialized) {
            initialized = true;
        }
    }
    static void registerCmds_() {

//@view:beg
    CMD_SYS.add("dir_merge_files",
    [](CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context) -> int {
        Q_UNUSED(data) Q_UNUSED(context) ///@view:exclude

        int result = 0;

        if (dirs_.isEmpty()) return args.appendError("no dir to solve");

        bool useViews = (args.get("views", "__UNDEF__").value() != "__UNDEF__");
        bool byDist   = (args.get("bydist", "__UNDEF__").value() != "__UNDEF__");

        QStringList files;

        for (int i = 0; i < dirs_.count(); i++) {

            const AnalyzerModule& mod = dirs_.get(i);
            if (!mod.used())
                continue;   // ← VYPNUTÝ MODUL

            const QString dirStr = mod.dirPath();
            QDir dir(dirStr);
            if (!dir.exists()) {
                result += args.appendError("directory does not exist");
                continue;
            }

            QStringList allFiles = AnalyzerCode::getFiles(
                dir,
                QStringList() << "*.cpp" << "*.h" << "*.qml",
                true
            );

            for (const QString& path : allFiles) {
                if (byDist) {
                    const QString nodeName =
                        QFileInfo(path).completeBaseName();

                    const CodeNode* node =
                        CodeData::inst().modules().get(
                            CodeNodeAddress(QString(), nodeName));

                    if (!node)
                        continue;

                    if (!std::isfinite(node->distToCenter()))
                        continue;
                }
                files << path;
            }
        }

        QDir dir(dirs_.first());

        if (files.isEmpty()) return args.appendError("dir_merge_files: no source files found");
        files.sort();

        if (dir.isRoot()) return args.appendError("dir_merge_files: root not possible");

        QString resultFileName = QDir::cleanPath(dirs_.first()) + ".h";
        QFile outFile(resultFileName);

        if (!outFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
            return args.appendError("dir_merge_files: cannot open output file");

        if (byDist) {
            const QString prompt = Cmds_code_analyzer::sys_.targetingPrompt();
            outFile.write(prompt.toUtf8());
            outFile.write("\n\n");
        }

        AnalyzerCode::composeToFile(files, outFile, useViews);

        outFile.write("\nin the next task follow strictly the style of the code above");
        outFile.write("\nprefer conservative solution, do not rebuild code, if not specified");

        outFile.close();

        args.append(resultFileName, "RESULT_FILE");


        dir.cdUp();
        args.append("<a href='system_open_path " +
                    dir.absolutePath() +
                    "'>[OPEN DIR]</a>");

        args.append(
            QString("<a href='file_to_clipboard ") + resultFileName +
            "'>[COPY TO CLIPBOARD]</a>");

        return result;
    });
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

        Cmds_code_analyzer::dirs_.setModuleUsed(index, used);

        args.append(
            QString("module[%1] -> %2")
                .arg(index)
                .arg(used ? "used" : "unused"),
            "OK");

        return 0;
    });
    CMD_SYS.add("analyzer_set_center",
    [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {

        if (args.count() < 2)
            return args.appendError("usage: analyzer_set_center <nodeName>");

        const QString name = args.get(1).value();

        CodeData& data = CodeData::inst();

        if (!data.modules().get(
                CodeNodeAddress(QString(), name)))
            return args.appendError("unknown node: " + name);

        data.center_ = CodeNodeAddress(QString(), name);
        Cmds_code_analyzer::sys_.center_ = name;

        // recompute distances using code_data
        AnalyzerDistCalc calc(data);
        calc.calculate();
        calc.addObservers();

        Cmds_code_analyzer::dirs_.resetAllFilesModels();

        args.append("center -> " + name, "OK");

        return 0;
    });

    CMD_SYS.add("analyzer_bootstrap",
    [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {

        const bool subdirs =
            (args.get("subdirs", "__UNDEF__").value() != "__UNDEF__");

        const bool strict =
            (args.get("strict", "__UNDEF__").value() != "__UNDEF__");

        Cmds_code_analyzer::createModel();

        AnalyzerCode::loadDot(subdirs, strict);

        Cmds_code_analyzer::dirs_.loadFilesModels();

        AnalyzerDistCalc dataCalc(CodeData::inst());
        dataCalc.calculate();
        dataCalc.addObservers();

        return 0;
    });

    CMD_SYS.add("module_add",
    [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {

        if (args.count() < 2)
            return args.appendError("usage: module_add <dirPath>");

        const bool subdirs =
            (args.get("subdirs", "__UNDEF__").value() != "__UNDEF__");

        const bool strict =
            (args.get("strict", "__UNDEF__").value() != "__UNDEF__");

        const QString dir = args.get(1).value();

        Cmds_code_analyzer::dirs_.add(dir, subdirs, strict);

        args.append(dir, "MODULE_ADDED");
        return 0;
    });

    }
//=============================================================================
protected:
    /// @section Data
    inline static AnalyzerSys sys_;
    inline static AnalyzerModuleCol dirs_ = AnalyzerModuleCol();

    friend class Cmds_code_analyzer_test;
    friend int main(int, char**);
    friend class AnalyzerCode;
    friend struct AnalyzerModuleFileData;
    friend class AnalyzerModule;
    friend class Cmds_code_analyzer;
    friend class AnalyzerDistCalc;
};
///@view:end
