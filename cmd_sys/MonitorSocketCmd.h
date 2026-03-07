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
 * @class CmdContext
 * @brief CmdContext Class.
 */
#pragma once

#include "MonitorStdinThread.h"

#include <QMap>
#include <QTextStream>

//=============================================================================
class  MonitorSocketCmd  {
//=============================================================================
public:
//! @section Construction
    MonitorSocketCmd() {
        stdinThread_ = new MonitorStdinThread();
        stdinThread_->start();
    }
    static MonitorSocketCmd& inst() { static MonitorSocketCmd i; return i; }
//<METHODS>
    void add(int id) {
        auto it = registry_.find(id);
        if (it == registry_.end()) {
            registry_.insert(id, 1);
        } else {
            ++it.value();
        }
    }

    void remove(int id) {
        auto it = registry_.find(id);
        if (it == registry_.end()) return;

        if (--it.value() <= 0) registry_.erase(it);
    }

    bool registered(int id) const {
        return registry_.contains(id);
    }

    bool isEmpty() const {
        return registry_.isEmpty();
    }

    /*void startStdin()
    {
        if (stdinThread_) return;

#ifdef Q_OS_WIN
        HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
        fprintf(stdout, "STDIN_HANDLE: %p\n", (void*)hStdin);
        fflush(stdout);
        if (hStdin == INVALID_HANDLE_VALUE || hStdin == NULL) {
            fprintf(stdout, "STDIN_NOT_AVAILABLE\n");
            fflush(stdout);
            return;
        }
#endif

        stdinThread_ = new MonitorStdinThread();
        stdinThread_->start();
    }*/

private slots:
    void onStdinReady_()
    {
        QTextStream in(stdin);
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) return;

        CMD_SYS.execute_threadSafe(line, "stdin");
    }

//=============================================================================
private:
    QMap<int, int> registry_;
    MonitorStdinThread* stdinThread_ = nullptr;
};
