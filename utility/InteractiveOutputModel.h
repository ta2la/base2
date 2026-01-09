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

#include "DebugListModelCol.h"
#include "DebugListModel.h"


///@view:beg

//===================================================================
class InteractiveOutputModel {
//===================================================================
public:
//<CONSTRUCTION>
    InteractiveOutputModel() = delete;

    static DebugListModel& inst() { return DebugListModelCol::inst().get("interactiveOutput"); }

    static void append(const QString& item) {
        items_.prepend(item);
        inst().updateList(items_);
    }
//===================================================================
//<INTERNALS>
protected:
    inline static QStringList items_;
};


///@view:end

//===================================================================

