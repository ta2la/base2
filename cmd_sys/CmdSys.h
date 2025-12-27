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
 * @class CmdSys
 * @brief Command registry.
 */

#pragma once

#include "Cmd.h"
#include "CmdContext.h"
#include "CmdExeGuard.h"

#include <QMap>
#include <QVector>
#include <QList>
#include <QElapsedTimer>
#include <QString>
#include <QObject>

class CmdExeGuard;

///@view:beg

#define CMD_SYS  CmdSys::instance()
#define CMD_ARGS ( CmdArgCol&, QByteArray*, const QSharedPointer<CmdContextIface>& )
#define CMD_ARGS_U ( [[maybe_unused]] CmdArgCol& args, [[maybe_unused]] QByteArray* data, [[maybe_unused]] const QSharedPointer<CmdContextIface>& context )

//=============================================================================
class  CmdSys : public QObject {
//=============================================================================
    Q_OBJECT
public:
//! @section Construction
    CmdSys() : QObject(nullptr) {};
    static CmdSys& instance() { static CmdSys i; return i; }
//! @section Neighbours
    //CmdExeRecCol& exeRecord() { return CmdExeRecCol::instance(); };
//<METHODS>
    int execute(const QString& args, const QString& sourceName = "", int sourceIndex = -1);
    void execute_threadSafe(const QString& args, const QString& sourceName = "", int sourceIndex = -1);

    void add(const QString& name, CommandHandler handler, bool excludeExeRec = false, bool excludeUpdate = false);
    Cmd* get(const QString& mame);
    void reg(CmdExeGuard* guard) {
        if (guard->phase() == CmdExeGuard::Record) {
            guards_.prepend(guard);
            return;
        }
        guards_.append(guard);
    }
    void setMainThread(bool value) { mainThread_ = value; }
//=============================================================================
protected:
//! @section Data
    QMap<QString, Cmd>    cmds_;
    QVector<CmdExeGuard*> guards_;
    CmdArgCol             executingArgs_;
    int                   runningIndex_ = -1;
    QElapsedTimer         timer_;
    bool                  mainThread_ = true;
//! @section Internals
    int execute_(CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context,
                const QString& sourceName, int sourceIndex);

    friend class Cmds_cmd_sys;
    friend class Interactive_uiControl;
    friend class CmdExeRecCol;
};

///@view:end
