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
 * @class Cmds_object_registry_test
 * @brief Commands for testing object_registry.
 */
#pragma once

#include "CmdSys.h"
#include "TestObject.h"

//=============================================================================
class Cmds_object_registry_test {
//=============================================================================
public:
//! @section Construction
    Cmds_object_registry_test() = delete;
//! @section Methods
    static void registerCmds() {
        CMD_SYS.add(
        "create_object_test",
        [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {
            if (args.count() < 2) return args.appendWarning("one numeric arg expected");
            int value = args.get(1).value().toInt();
            new TestObject(value);
            return 0;
        });
    }
//=============================================================================
};
