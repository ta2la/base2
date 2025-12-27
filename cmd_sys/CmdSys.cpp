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
#include "CmdSys.h"
#include "CmdExeGuard.h"
#include "MonitorSocketCmd.h"

#include <QCoreApplication>
#include <QThread>

///@view:beg

//=============================================================================
void CmdSys::add(const QString& name, CommandHandler handler, bool excludeExeRec, bool excludeUpdate )
{
    Cmd cmd(name, handler, excludeExeRec, excludeUpdate);
    cmds_.insert(name, cmd);
    timer_.start();
}

//=============================================================================
Cmd* CmdSys::get(const QString& name)
{
    auto it = cmds_.find(name);
    if (it == cmds_.end())  return nullptr;
    return &(*it);
}

//=============================================================================
int CmdSys::execute_(CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context, const QString& sourceName, int sourceIndex)
{
    if ( mainThread_) {
        assert(QThread::currentThread()==QCoreApplication::instance()->thread());
    }
    if (QCoreApplication::closingDown()) return 0;

    //! @section - search command
    if (args.count() == 0) {
        return -1;
    }

    runningIndex_++;

    QString name = args.get(0).value();
    Cmd* cmd = get(name);
    //QString cmdString = args.toString();
    if (cmd == nullptr) {
        cmd = get("__not_implemented__");
        if (cmd == nullptr) return -1;
    }

    //! @section - execute
    int result = cmd->execute(args, data, context);

    //! @section - postprocess
    bool recordCmd = false;
    if (!cmd->excludeExeRec()) {
        recordCmd = true;
    }
    else if (!MonitorSocketCmd::instance().isEmpty()) {
        if ( MonitorSocketCmd::instance().registered(context->uniqueId()) ) recordCmd = true;
    }

    if (recordCmd) {
        QString argsStr = args.toString();
    }

    if (!cmd->excludeUpdate()) {
        int i = 0;
        for (int i = 0; i < guards_.size(); ++i) {
            guards_[i]->commandExecuted();
        }
    }

    return result;
}

//=============================================================================
void CmdSys::execute_threadSafe(const QString& args, const QString& sourceName, int sourceIndex)
{
    QMetaObject::invokeMethod(this, [this, args, sourceName, sourceIndex]() {
        execute(args, sourceName, sourceIndex);
    }, Qt::QueuedConnection);
}

//=============================================================================
int CmdSys::execute(const QString& args, const QString& sourceName, int sourceIndex)
{
    Q_ASSERT(QThread::currentThread() == this->thread());

    if (!executingArgs_.isNull()) {
        execute_threadSafe(args, sourceName, sourceIndex);
        return 0;
    }

    executingArgs_ = CmdArgCol(args);
    auto guard = qScopeGuard([this] { executingArgs_ = CmdArgCol(); });

    return execute_(executingArgs_, nullptr, CONTEXT, sourceName, sourceIndex);
}

///@view:end
