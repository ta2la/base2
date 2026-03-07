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
 * @class StdinMonitor
 * @brief Reads commands from stdin in a separate thread, dispatches via execute_threadSafe.
 */
#pragma once

#include "CmdSys.h"

#include <QSharedPointer>
#include <QThread>
#include <QStringList>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

//=============================================================================
class StdinMonitor : public QThread {
public:
    static StdinMonitor& init() { static StdinMonitor i; i.start(); return i; }
protected:
    virtual void processLine(const QString& line) {
        processLine(line);
    }


#ifdef Q_OS_WIN
    void run() override {
        HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
        if (hStdin == INVALID_HANDLE_VALUE || hStdin == NULL) return;

        char buf[4096];
        DWORD bytesRead;

        while (!isInterruptionRequested()) {
            if (!ReadFile(hStdin, buf, sizeof(buf) - 1, &bytesRead, NULL)) break;
            if (bytesRead == 0) break;

            buf[bytesRead] = '\0';

            QString chunk = QString::fromUtf8(buf);
            QStringList lines = chunk.split('\n', Qt::SkipEmptyParts);

            for (const QString& line : lines) {
                QString trimmed = line.trimmed();
                if (!trimmed.isEmpty()) {
                    processLine(trimmed);
                }
            }
        }
    }

#else
    void run() override {
        char buf[4096];
        while (!isInterruptionRequested()) {
            if (!fgets(buf, sizeof(buf), stdin)) break;
            QString line = QString::fromUtf8(buf).trimmed();
            if (!line.isEmpty()) {
                processLine(line);
            }
        }
    }
#endif
};
