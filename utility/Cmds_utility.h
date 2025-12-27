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

#include <QString>
#include <QByteArray>

class QWebSocket;

//=============================================================================
class  Cmds_utility {
//=============================================================================
public:
//! @section Construction
    Cmds_utility() = delete;
//<METHODS>
    static void registerCmds() {
        CMD_SYS.add("change_controls_ex",
        [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {

            if (args.count() < 2)
                return 0;

            CmdArg argText = args.get(1);

            // původní záměr – zatím neimplementováno
            assert(0);
            // Model_controlsEx::instance().setContent(argText.value());

            return 0;
        });
    }
//=============================================================================
};


