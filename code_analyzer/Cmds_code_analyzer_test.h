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
//#include "AnalyzerCode.h"
//#include "Cmds_code_analyzer.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QFileDialog>
#include <QString>

///@view:beg
class  Cmds_code_analyzer_test {
//=============================================================================
public:
//! @section Construction
    Cmds_code_analyzer_test() = delete;
//<METHODS>
    static void registerCmds_() {

    CMD_SYS.add("select_dir",
    [](CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context) -> int {
        Q_UNUSED(data) Q_UNUSED(context) Q_UNUSED(args)

        return 0;
    });
    }};
///@view:end
