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

///@view:beg

class  Cmds_exerec {
//=============================================================================
public:
//! @section Construction
    Cmds_exerec() = delete;
    static bool registerCmds_()
    {
        CMD_SYS.add("exerec_add_filterout_command",     exerec_add_filterout_command );
        CMD_SYS.add("exerec_remove_filterout_command",  exerec_remove_filterout_command );
        CMD_SYS.add("cmds_stop_record",  cmds_stop_record );

        return true;
    }
//! @section Command
protected:
    static int exerec_add_filterout_command  ( CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context );
    static int exerec_remove_filterout_command  ( CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context );
    static int cmds_stop_record ( CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context );

    friend class ExerecModelProxy;
    friend class Cmds_active;
    friend class Cmds_ui_model_refresh;
    friend class UiControl;
};

///@view:end
