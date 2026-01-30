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
#include "OregUpdateLock.h"
#include "AnalyzerDistCalc.h"
#include "CodeMethods.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QString>
#include <QFile>
#include <QDir>

///@view:beg
class  Cmds_code_data {
//=============================================================================
public:
//! @section Construction
    Cmds_code_data() = delete;
//<METHODS>
    //=========================================================================
    static void composeToFile (
        const QStringList& files, QFile& outFile,
        bool useViews = false )
    {
        for (const QString& path : files) {
            QByteArray content = CodeMethods::extractView(path, useViews);

            if (content.isEmpty())
                continue;

            QFileInfo fi(path);
            const QString fileName = fi.fileName();
            const QString module   = fi.dir().dirName();

            outFile.write("\n// ---- module:");
            outFile.write(module.toUtf8());
            outFile.write("     ");
            outFile.write(fileName.toUtf8());
            outFile.write(" ----\n\n");
            // ----------------------

            outFile.write(content);
            outFile.write("\n");
        }
    }

    static void registerCmds_() {

        CMD_SYS.add("dir_merge_files",
        [](CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context) -> int {

            Q_UNUSED(data)
            Q_UNUSED(context)

            int result = 0;

            // ---- CHANGE BEGIN: use CodeData / CodeModuleCol instead of AnalyzerModuleCol
            CodeData& dataRef = CodeData::inst();
            CodeModuleCol& modules = dataRef.modules();

            if (modules.count() == 0)
                return args.appendError("no dir to solve");
            // ---- CHANGE END

            const bool useViews =
                (args.get("views", "__UNDEF__").value() != "__UNDEF__");

            const bool byDist =
                (args.get("bydist", "__UNDEF__").value() != "__UNDEF__");

            QStringList files;

            // ---- CHANGE BEGIN: collect files via CodeNode graph
            const QStringList nodeNames = modules.nodes();

            for (const QString& nodeName : nodeNames) {

                const CodeNode* node =
                    modules.get(CodeNodeAddress(QString(), nodeName));

                if (!node)
                    continue;

                if (byDist && !std::isfinite(node->distToCenter()))
                    continue;

                const QString basePath = node->dir();
                const QString dirPath  = QFileInfo(basePath).absolutePath();
                const QString baseName = node->name();

                for (const QString& ext : node->extensions()) {

                    const QString filePath =
                        dirPath + "/" + baseName + "." + ext;

                    if (QFileInfo::exists(filePath))
                        files << QDir::cleanPath(filePath);
                }
            }
            // ---- CHANGE END

            if (files.isEmpty())
                return args.appendError("dir_merge_files: no source files found");

            files.removeDuplicates();
            files.sort();

            // ---- CHANGE BEGIN: determine output directory without AnalyzerModuleCol
            QString firstModulePath;

            const QStringList moduleNames = modules.names();
            if (!moduleNames.isEmpty()) {
                const CodeModule* m = modules.get(moduleNames.first());
                if (m)
                    firstModulePath = m->path();
            }

            if (firstModulePath.isEmpty())
                return args.appendError("dir_merge_files: cannot determine output directory");
            // ---- CHANGE END

            QDir dir(firstModulePath);
            if (dir.isRoot())
                return args.appendError("dir_merge_files: root not possible");

            QString resultFileName =
                QDir::cleanPath(firstModulePath) + ".h";

            QFile outFile(resultFileName);
            if (!outFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
                return args.appendError("dir_merge_files: cannot open output file");

            composeToFile(files, outFile, useViews);

            outFile.write(
                "\nin the next task follow strictly the style of the code above");
            outFile.write(
                "\nprefer conservative solution, do not rebuild code, if not specified");

            outFile.close();

            args.append(resultFileName, "RESULT_FILE");

            dir.cdUp();
            args.append(
                "<a href='system_open_path " +
                dir.absolutePath() +
                "'>[OPEN DIR]</a>");

            args.append(
                QString("<a href='file_to_clipboard ") +
                resultFileName +
                "'>[COPY TO CLIPBOARD]</a>");

            return result;
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

                //Cmds_code_analyzer::dirs_.add(dir, subdirs, strict);

                OregUpdateLock l;
                const QString norm = QDir::cleanPath(QDir(dir).absolutePath());
                CodeData::inst().modules().add(norm, subdirs, strict);

                args.append(dir, "MODULE_ADDED");
                return 0;
            });

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
