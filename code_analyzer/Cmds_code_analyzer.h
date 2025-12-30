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

            dirs_.add("../../../base2/base/");
            dirs_.add("../../../base2/cmd_sys");
            dirs_.add("../../../base2/cmd_sys_display");
            dirs_.add("../../../base2/code_analyzer");
            dirs_.add("../../../apky/PROMPT_ASSEMBLER");
            dirs_.add("../../../base2/utility");
        }
    }
    static void registerCmds_() {

///@view:beg
    CMD_SYS.add("dir_merge_files",
    [](CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context) -> int {
        Q_UNUSED(data) Q_UNUSED(context) ///@view:exclude

        int result = 0;

        if (dirs_.isEmpty()) return args.appendError("no dir to solve");

        bool useViews =
            (args.get("views", "__UNDEF__").value() != "__UNDEF__");

        QStringList files;

        for (int i = 0; i < dirs_.count(); i++) {
            QString dirStr = dirs_.get(i).dirPath();
            QDir dir(dirStr);
            if (!dir.exists()) result += args.appendError("directory does not exist");

            files << AnalyzerCode::getFiles(dir, QStringList() << "*.cpp" << "*.h" << "*.qml");
        }

        QDir dir(dirs_.first());

        if (files.isEmpty()) return args.appendError("dir_merge_files: no source files found");
        files.sort();

        if (dir.isRoot()) return args.appendError("dir_merge_files: root not possible");

        QString resultFileName = QDir::cleanPath(dirs_.first()) + ".h";
        QFile outFile(resultFileName);

        if (!outFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
            return args.appendError("dir_merge_files: cannot open output file");

        AnalyzerCode::composeToFile(files, outFile, useViews);

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
    CMD_SYS.add("dir_load_net_debug",
    [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {
        const QString debugStr = sys_.toStringDebug();
        args.append(debugStr, "DEBUG");
        return 0;
    });
    CMD_SYS.add("dir_export_dot",
    [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {

        if (dirs_.isEmpty())
            return args.appendError("no dir to analyze");

        // stejný průchod jako dir_load_net / dir_merge_files
        //for (const QString& dirStr : dirs_) {
        for (int i = 0; i < dirs_.count(); i++) {
            QString dirStr = dirs_.get(i).dirPath();
            QDir moduleDir(dirStr);
            if (!moduleDir.exists()) {
                args.appendError("directory does not exist: " + dirStr);
                continue;
            }

            QStringList moduleFiles =
            AnalyzerCode::getFiles(
                moduleDir,
                QStringList() << "*.h" << "*.cpp" << "*.qml"
                );

            const QString moduleName = moduleDir.dirName();

            // naplnění uzlů + hran
            for (const QString& filePath : moduleFiles) {
                sys_.add(filePath, moduleName);
            }
        }

        // výstupní soubor – stejný základ jako dir_merge_files, ale .dot
        QDir dir(dirs_.first());
        if (dir.isRoot())
            return args.appendError("dir_export_dot: root not possible");

        QString base = QDir::cleanPath(dirs_.first());
        QString dotFileName = base + ".dot";

        QFile outFile(dotFileName);
        if (!outFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
            return args.appendError("dir_export_dot: cannot open output file");

        QByteArray dot = sys_.toDot().toUtf8();
        outFile.write(dot);
        outFile.close();

        args.append(dotFileName, "RESULT_DOT");

        // otevření adresáře s výsledným .dot souborem
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
    inline static AnalyzerSys sys_;
    inline static AnalyzerModuleCol dirs_ = AnalyzerModuleCol();
        /*<< AnalyzerModule( "../../../base2/base/")
        << AnalyzerModule( "../../../base2/cmd_sys")
        << AnalyzerModule("../../../base2/cmd_sys_display")
        << AnalyzerModule("../../../base2/code_analyzer")
        << AnalyzerModule("../../../apky/PROMPT_ASSEMBLER")
        << AnalyzerModule("../../../base2/utility");*/

    friend class Cmds_code_analyzer_test;
    friend int main(int, char**);
};
///@view:end
