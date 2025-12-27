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
class  Model_filteroutCmds: public QAbstractListModel {
//=============================================================================
public:
//! @section Enums
    enum EExerecModelRoles { cmd = Qt::UserRole + 1 };
//! @section Construction
    Model_filteroutCmds() = default;
    static Model_filteroutCmds& instance()        { static Model_filteroutCmds i; return i; }

//! @section Methods
    QStringList cmdsExcluded() { return cmds_; }

    bool contains(const QString& cmd) {
        return cmds_.contains(cmd);
    }

    void cmdAdd(const QString& cmd) {
        if (!monitor_ && contains(cmd)) return;

        int newRowIndex = cmds_.count();
        beginInsertRows(QModelIndex(), newRowIndex, newRowIndex);
        cmds_.append(cmd);
        endInsertRows();

        if (monitor_) {
            qDebug() << "monitor add";
        }
    }

    void cmdRemove(const QString& cmd) {
        int rowIndex = cmds_.indexOf(cmd);
        if (rowIndex == -1) return;

        beginRemoveRows(QModelIndex(), rowIndex, rowIndex);
        cmds_.removeAt(rowIndex);
        endRemoveRows();
    }

//<METHODS>
//! @section Overrides
    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        if (monitor_) {
            qDebug() << "monitor count";
        }
        return cmds_.count();
    };
    QVariant data(const QModelIndex &modelIndex, int role) const override {
        if (monitor_) {
            qDebug() << "monitor DATA";
        }

        if (!modelIndex.isValid()) return QVariant("mistake");

        if (role == cmd) { return QVariant(cmds_.at(modelIndex.row())); }

        return QVariant("mistake");;
    }
    QHash<int, QByteArray> roleNames() const override { return {{cmd, "cmd"}}; }
//=============================================================================
protected:
//! @section Data
    QStringList cmds_;
    bool monitor_ = false;

    friend class Model_controls;
};
///@view:end
