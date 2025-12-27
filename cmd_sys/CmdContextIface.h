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
 * @class CmdContextIface
 * @brief Interface CmdContextIface Class.
 */
#pragma once

#include "CmdContextDataIface.h"

#include <QString>
#include <QByteArray>
#include <QThread>
#include <QCoreApplication>

class ClientContext;

///@view:beg

//=============================================================================
class  CmdContextIface {
//=============================================================================
public:
//! @section Construction
    virtual ~CmdContextIface() = default;
//<METHODS>
    virtual void reply(QString cmd, QByteArray data = QByteArray()) = 0;
    virtual void reply_threadSafe(QString cmd, QByteArray data = QByteArray()) { assert(0); }

    virtual ClientContext* getAsClientContext() { return nullptr; }

    virtual void forceDisconnect() {};

    virtual bool logCmd() { return false; }

    int uniqueId() {
        if (clientData_ == nullptr) return -1;
        return (clientData_->uniqueId());
    }

    CmdContextDataIface* clientData() {
        assert(QThread::currentThread() == QCoreApplication::instance()->thread());
        return clientData_; }
    void clientDataSet(CmdContextDataIface* data) {
        assert(QThread::currentThread() == QCoreApplication::instance()->thread());

        assert(clientData_ == nullptr);
        clientData_ = data; }
//=============================================================================

    CmdContextDataIface* clientData_ = nullptr;
};

///@view:end
