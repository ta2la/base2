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

#include <CmdSys.h>

///@view:beg

class  Cmds_cmd_sys {
//=============================================================================
public:
//! @section Construction
    Cmds_cmd_sys() = delete;
    static bool registerCmds_()
    {
        CMD_SYS.add("voidcmd", voidcmd );
        CMD_SYS.add("logcmd",  logcmd );

        CMD_SYS.add("__not_implemented__",
        [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {

            QString name = args.get(0).value();
            args.appendError(QString("command not implemented: ") + name);
            return -1;
        });

        return true;
    }

//! @section Command
    static int voidcmd( CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context );
    static int logcmd ( CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context );

};

///@view:end
