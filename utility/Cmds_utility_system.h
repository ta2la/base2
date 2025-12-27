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

#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include <QProcess>
#include <QClipboard>
#include <QGuiApplication>

class QWebSocket;

///@view:beg

//=============================================================================
class  Cmds_utility_system {
//=============================================================================
public:
//! @section Construction
    Cmds_utility_system() = delete;
//<METHODS>
    static void registerCmds() {
    CMD_SYS.add("system_open_path",
    [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {

        if (args.count() < 2)
            return args.appendError("open_path: missing path");

        CmdArg arg1 = args.get(1);
        const QString path = arg1.value();

        QFileInfo fi(path);
        if (!fi.exists())
            return args.appendError("open_path: path does not exist: " + path);

        QUrl url = QUrl::fromLocalFile(fi.absoluteFilePath());

        if (!QDesktopServices::openUrl(url))
            return args.appendError("open_path: failed to open: " + path);

        return 0;
    });
    CMD_SYS.add("system_dot_to_svg",
    [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {

                    int result = 0;

        if (args.count() < 2)
            return args.appendError("system_dot_to_svg: missing .dot file");

        CmdArg arg1 = args.get(1);
        const QString dotPath = arg1.value();

        QFileInfo fi(dotPath);
        if (!fi.exists())
            return args.appendError("system_dot_to_svg: file does not exist: " + dotPath);

        if (fi.suffix().toLower() != "dot")
            return args.appendError("system_dot_to_svg: not a .dot file: " + dotPath);

        const QString svgPath =
            fi.absolutePath() + "/" + fi.completeBaseName() + ".svg";

        QProcess proc;
        QStringList procArgs;
        procArgs << "-v"
                 << "-Tsvg" << fi.absoluteFilePath()
                 << "-o"    << svgPath;

        proc.start("dot", procArgs);

        if (!proc.waitForFinished())
            return args.appendError("system_dot_to_svg: dot execution failed");

        if (proc.exitStatus() != QProcess::NormalExit || proc.exitCode() != 0)
            result += args.appendError("system_dot_to_svg: dot returned error");

        result += args.append(svgPath, "RESULT_SVG");

        result += args.append("<a href='system_open_path " +
                    svgPath +
                    "'>[OPEN SVG]</a>");

        return result;
    });
    CMD_SYS.add("file_to_clipboard",
    [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {

        if (args.count() < 2)
            return args.appendError("file_to_clipboard: missing file path");

        const QString filePath = args.get(1).value();
        if (filePath.isEmpty())
            return args.appendError("file_to_clipboard: empty file path");

        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return args.appendError("file_to_clipboard: cannot open file " + filePath);

        const QByteArray content = file.readAll();
        file.close();

        QClipboard* clipboard = QGuiApplication::clipboard();
        if (!clipboard)
            return args.appendError("file_to_clipboard: clipboard not available");

        clipboard->setText(QString::fromUtf8(content));

        args.append(filePath, "COPIED_FILE");
        args.append(QString::number(content.size()), "BYTES");

        const int lines = content.count('\n') + 1;
        QString scope;
        if (lines <= 500)        scope = "<b>micro-analytic</b> - up to 500";
        else if (lines <= 1500)  scope = "<b>fine-grained</b> - up to 1500";
        else if (lines <= 3500)  scope = "<b>good</b> - upto 3500";
        else                     scope = "<b>out-of-scope</b> - over 3500";
        args.append(QString::number(lines) + " (" + scope + ")", "LINES");


        return 0;
    });

    }
//=============================================================================
};

///@view:end
