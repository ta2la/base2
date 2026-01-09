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
 * @file Cmds_test0.cpp
 * @brief Cmds_test0 class implementation.
 */
#include "Cmds_exerec.h"

#include "ExerecModelProxy.h"
#include "Model_filteroutCmds.h"
#include "CmdExeRecCol.h"

#include <QTextStream>

//QStringList Cmds_exerec::filteroutCommands_ = {};

///@view:beg

//=============================================================================
int Cmds_exerec::exerec_remove_filterout_command(CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context)
{
    CmdArg arg1 = args.get(1);
    if (arg1.value().isEmpty()) return 0;

    Model_filteroutCmds::inst().cmdRemove(arg1.value());

    //QString comp = filteroutCommands_codeSegment();
    //ExerecCodeSegments::inst().qmlExcludedCommandsSet(comp);

    ExerecModelProxy::inst().invalidateFilterOverride();

    return 0;
}

//=============================================================================
int Cmds_exerec::exerec_add_filterout_command(CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context)
{
    CmdArg arg1 = args.get(1);
    if (arg1.value().isEmpty()) return 0;

    Model_filteroutCmds::inst().cmdAdd(arg1.value());

    /*QString comp = filteroutCommands_codeSegment();
    ExerecCodeSegments::inst().qmlExcludedCommandsSet(comp);*/

    ExerecModelProxy::inst().invalidateFilterOverride();

    return 0;
}

//=============================================================================
int Cmds_exerec::cmds_stop_record(CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context)
{
    CmdExeRecCol::inst().cacheSet();
    return 0;
}

///@view:end

//=============================================================================
