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
#include "InteractiveOutputModel.h"
#include "CmdExeGuard.h"

#include <QObject>

///@view:beg

#define UI_CONTROL UiControl::inst()

//===================================================================
class Interactive_uiControl : public QObject, public CmdExeGuard {
//===================================================================
    Q_OBJECT
public:
//<CONSTRUCTION>
    Interactive_uiControl(QObject *parent = nullptr) :
        QObject(parent),
        CmdExeGuard(Update)
    {
        CMD_SYS.reg(this);
    };
    static Interactive_uiControl& inst() { /*assert(!GlobalSettings::nodebug());*/
                                        static Interactive_uiControl i; return i; }
//===================================================================
//<INTERNALS>
protected:
    void commandExecuted() override {
        CmdSys& cmdsys = CMD_SYS;

        if (!myCmd_) return;
        QString args = CMD_SYS.executingArgs_.toString();
        InteractiveOutputModel::append(args);
    }

    bool myCmd_ = false;
public slots:
    void callCmd(const QString& cmd) {
        //CmdArgCol args(cmd );
        myCmd_ = true;
        CMD_SYS.execute(cmd);
        myCmd_ = false;

        //InteractiveOutput::append(args.toString());
    }
};

///@view:end

//===================================================================

