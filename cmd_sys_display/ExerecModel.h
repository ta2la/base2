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
 * @class Cmd
 * @brief Command Class.
 */
#pragma once

#include "CmdExeGuard.h"

#include <QAbstractListModel>

///@view:beg

//=============================================================================
class  ExerecModel: public QAbstractListModel, public CmdExeGuard {
//=============================================================================
public:
//! @section Enums
    enum EExerecModelRoles { exerecData = Qt::UserRole + 1, cmdName };
//! @section Construction
    ExerecModel();
    static ExerecModel& inst() { static ExerecModel i; return i; }
//<METHODS>
//! @section Overrides
    void commandExecuted() override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override { return countIncluded_; };
    QVariant data(const QModelIndex &modelIndex, int role) const override;
    QHash<int, QByteArray> roleNames() const override { return {{exerecData, "exerecData"}, {cmdName, "cmdName"}}; }
//=============================================================================
protected:
//! @section Data
    int countIncluded_ = 0;
};

///@view:end
