//
// Copyright (C) 2026 Petr Talla. [petr.talla@gmail.com]
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

class OregObject;
class DebugRecordItem;

//=============================================================================
class  Cmds_oreg_test {
//=============================================================================
public:
//! @section Construction
    Cmds_oreg_test() = delete;
//<METHODS>
    static void registerCmds() {
        CMD_SYS.add("oreg_test_setup",             oreg_test_setup );
        CMD_SYS.add("oreg_test_display_pool",      oreg_test_display_pool );
        CMD_SYS.add("oreg_test_create_container",  oreg_test_create_container );
        CMD_SYS.add("oreg_test_container_display", oreg_test_container_display );
        CMD_SYS.add("oreg_test_pool_solve",        oreg_test_pool_solve );
    }
//=============================================================================
protected:
//! @section Commands
    static int oreg_test_setup             CMD_ARGS;
    static int oreg_test_display_pool      CMD_ARGS;
    static int oreg_test_create_container  CMD_ARGS;
    static int oreg_test_container_display CMD_ARGS;
    static int oreg_test_pool_solve        CMD_ARGS;

    static QString displayObject_(OregObject* object);
};
