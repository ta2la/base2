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

#include <CmdSys.h>

//#include <TcCmdEngine.h>
#include <QString>

class  Cmds_test0 {
//=============================================================================
public:
//! @section Construction
    Cmds_test0() = delete;
    static bool registerCmds_()
    {
        CMD_SYS.add("argcol_to_string",    argcol_to_string );
        CMD_SYS.add("argcol_from_string",  argcol_from_string );
        CMD_SYS.add("argcol_from_string2", argcol_from_string2 );
        CMD_SYS.add("argcol_from_string_alone", argcol_from_string_alone );

        CMD_SYS.add("zakleta_princezna", zakleta_princezna );

        CMD_SYS.add("zakleta_princezna",
        [](CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context) -> int {
            Q_UNUSED(data) Q_UNUSED(context)

            QString text = "test arg --value ahoj --karle";
            text += " --json {\"name\": \"Alois\", \"surname:\": \"Lolonois\"}";

            CmdArgCol argsTest(text);
            //args.append(argsTest.toJson(), "JSON");

            return 1;
        });

        return true;
    }
//! @section Command
protected:
    static int argcol_to_string    ( CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context );
    static int argcol_from_string  ( CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context );
    static int argcol_from_string2 ( CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context );

    static int argcol_from_string_alone ( CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context );

    static int zakleta_princezna ( CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context );

    static QString afs_single_test_(const QString name, const char* cmd);
};
