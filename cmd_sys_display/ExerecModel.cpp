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
 * @file Cmd.cpp
 * @brief Implementation of Cmd class.
 */

#include "CmdSys.h"
#include "ExerecModel.h"
#include "ExerecData.h"
#include "ExerecModelProxy.h"
#include "CmdExeRecCol.h"

#include <QRandomGenerator>
///@view:beg

//=============================================================================
ExerecModel::ExerecModel() :
    CmdExeGuard(Update)
{
    CMD_SYS.reg(this);
}

//=============================================================================
void ExerecModel::commandExecuted()
{
    CmdSys& cmdsys = CMD_SYS;
    CmdExeRecCol& reccol = CmdExeRecCol::inst();
    int sysCount = CmdExeRecCol::inst().count();

    if ( sysCount > countIncluded_ ) {
        beginInsertRows(QModelIndex(), countIncluded_, sysCount-1);
        countIncluded_ = sysCount;
        endInsertRows();

        ExerecModelProxy::inst().sort(0, Qt::AscendingOrder);
    }
}

//=============================================================================
QVariant ExerecModel::data(const QModelIndex &modelIndex, int role) const
{
    if (!modelIndex.isValid()) return QVariant();

    CmdExeRecCol& exeRecord = CmdExeRecCol::inst();
    int row = modelIndex.row();
    CmdExeRec& exeRec = exeRecord.get(row);

    if (role == cmdName) {
        QString cmd = exeRec.argsIn.trimmed().split(" ").first();
        return QVariant::fromValue(cmd);
    }
    if (role == exerecData) {
        ExerecData data;
        data.args = "";
        if (!exeRec.sourceName.isEmpty()) {
            data.args += QString() + "source: " + exeRec.sourceName +
                         " [" + QString::number(exeRec.index) + "]<br/>";
        }
        data.cmd  = exeRec.argsIn.trimmed().split(" ").first();
        data.args += "[" + QString::number(exeRec.index) + "] ";
        QString in = exeRec.argsIn;
        int spaceIndex = in.indexOf(' ');
        if (spaceIndex != -1)in.insert(spaceIndex, "</b><br/>");
        data.args += "<b>";
        if (exeRec.argsIn.length() < 320) {
            data.args += in;
        }
        else {
            data.args += in.left(240);
            data.args += QString("<span style='font-size:6pt;'>") + in.mid(240) + "</span>";
        }
        if (!exeRec.argsOut.isEmpty()) data.args += QString("<br/>") + exeRec.argsOut;
        return QVariant::fromValue(data);
    }

   return QVariant();
}

///@view:end

//=============================================================================
