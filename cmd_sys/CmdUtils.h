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
#pragma once

#include "CmdSys.h"

#include <QString>
#include <QStringList>

///@view:beg

//=============================================================================
class  CmdUtils {
//=============================================================================
public:
//! @section Construction
    CmdUtils() = delete;
//! @section Methods
    static QStringList mineCommandsFromScript(const QString& input)
    {
        QString text = input;
        text.replace("\r\n", "\n");
        text.replace("\r", "\n");

        QStringList result;
        QStringList currentLines;

        auto flushBlock = [&]() {
            QString cmd;
            for (const QString& line : currentLines) {
                QString s = line.trimmed();
                if (s.isEmpty()) continue;
                if (!cmd.isEmpty()) cmd += ' ';
                cmd += s;
            }
            currentLines.clear();

            if (!cmd.isEmpty())
                result.append(cmd);
        };

        int pos = 0;
        while (pos <= text.size()) {
            int eol = text.indexOf('\n', pos);
            if (eol == -1) eol = text.size();

            QString line = text.mid(pos, eol - pos);

            if (line.trimmed().isEmpty()) {
                flushBlock();           // oddělovač commandů
            } else {
                currentLines.append(line);
            }

            pos = eol + 1;
            if (eol == text.size()) break;
        }

        flushBlock(); // poslední command

        return result;
    }
    static int executeCommands(const QStringList& commands)
    {
        int executed = 0;

        for (const QString& cmd : commands) {
            if (cmd.trimmed().isEmpty())
                continue;

            CMD_SYS.execute(cmd);
            executed++;
        }

        return executed;
    }
//=============================================================================
};

///@view:end
