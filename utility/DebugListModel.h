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
 * @class DebugListModel
 * @brief Model for QStringList.
 */
#pragma once

#include <QAbstractListModel>
#include <QStringList>

///@view:beg

class  DebugListModel : public QAbstractListModel {
    Q_OBJECT
public:
    //! @section Enums
    enum RegistryRoles {
        item_string
    };

//! @section Construction
    DebugListModel();
//! @section Items
    void updateList(const QStringList& list);
    void prepend(const QString& out, int max = -1);
    void setGuarded(int index, const QString& value);
    QString getItem(int index);
    void updateCount(int count);
    void updateItem(const QString& value, int index);
//! @section Overrides
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &modelIndex, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
//=============================================================================
protected:
//! @section Data
    QStringList list_;
//! @section Friends
    friend class Cmds_fc_client_test;
};

///@view:end
