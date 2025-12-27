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
 * @class Cmd
 * @brief Command Class.
 */
#pragma once

#include "CmdArgCol.h"
#include "CmdContextIface.h"

#include <QString>
#include <QByteArray>
#include <QSharedPointer>

#include <functional>

class QWebSocket;
class CmdArgCol;

using CommandHandler = int (*)(CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context);


///@view:beg
//=============================================================================
class  Cmd {
//=============================================================================
public:
//! @section Construction
    Cmd(const QString& name, CommandHandler handler, bool excludeExeRec = false, bool excludeUpdate = false) :
        name_(name),
        handler_(handler),
        excludeExeRec_(excludeExeRec),
        excludeUpdate_(excludeUpdate)
    {}
//<METHODS>
    int execute(CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context) {
        return handler_(args, data, context);
    }

    QString name() const    { return name_; }
    bool    excludeExeRec() { return excludeExeRec_; }
    bool    excludeUpdate() { return excludeUpdate_; }
//=============================================================================
protected:
//! @section Data
    QString        name_;
    CommandHandler handler_;
    bool           excludeExeRec_;
    bool           excludeUpdate_;
};
///@view:end
