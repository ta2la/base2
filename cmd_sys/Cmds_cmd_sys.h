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
 * @class Cmds_cmd_sys
 * @brief Some basic commands of CmdSys.
 */
#pragma once

#include "CmdSys.h"
#include "CmdUtils.h"

#include <QFile>
#include <QTextStream>

///@view:beg

class  Cmds_cmd_sys {
//=============================================================================
public:
//! @section Construction
    Cmds_cmd_sys() = delete;
    static bool registerCmds_()
    {
        CMD_SYS.add("voidcmd",
        [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {
            return 0;
        });

        CMD_SYS.add("logcmd",
        [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {
            return 0;
        });

        CMD_SYS.add("__not_implemented__",
        [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {

            QString name = args.get(0).value();
            args.appendError(QString("command not implemented: ") + name);
            return 1;
        });

        CMD_SYS.add("cmds_execute_script",
        [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {

            QString path = args.get(1).value().trimmed();
            if (path.isEmpty()) return args.appendError("cmds_from_file: missing file path");

            QFile file(path);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                return args.appendError(QString("cmds_from_file: cannot open file: ") + path);
            }

            QTextStream stream(&file);
            #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            #define QT_TEXTSTREAM_SET_UTF8(ts) (ts).setCodec("UTF-8")
            #else
            #define QT_TEXTSTREAM_SET_UTF8(ts) (ts).setEncoding(QStringConverter::Utf8)
            #endif
            QString script = stream.readAll();

            QStringList commands =
                CmdUtils::mineCommandsFromScript(script);

            int executed =
                CmdUtils::executeCommands(commands);

            args.append(QString("executed %1 command(s)").arg(executed), "INFO");
            return 0;
        });

        CMD_SYS.add("execute_script",
        [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {

            if (args.count() < 2)
                return args.appendError("usage: execute_init_script <file>");

            const QString fileName = args.get(1).value();

            QFile file(fileName);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                return args.appendError("cannot open file: " + fileName);

            QTextStream in(&file);
            uint lineNumber = 0;

            while (!in.atEnd()) {
                ++lineNumber;

                QString line = in.readLine().trimmed();
                if (line.isEmpty())
                    continue;
                if (line.startsWith("#"))
                    continue;

                const bool ok = (CMD_SYS.execute(line) == 0);

                if (!ok) {
                    return args.appendError(
                        QString("init failed at line %1").arg(lineNumber)
                        );
                }
            }

            args.append(fileName, "INIT_OK");
            return 0;
        });

        return true;
    }

//! @section Command
    static int voidcmd( CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context );
    static int logcmd ( CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context );

};

///@view:end
