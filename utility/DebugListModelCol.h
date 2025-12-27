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

#include "DebugListModel.h"
#include "DebugListModelItem.h"

#include <QMap>
#include <QString>

///@view:beg

class  DebugListModelCol  : public QAbstractListModel {
    Q_OBJECT
public:
//! @section Enums
    enum RegistryRoles {
        tabName = Qt::UserRole + 1,
        debugList
    };
//! @section Construction
    DebugListModelCol() = default;
    virtual ~DebugListModelCol() {}
    static DebugListModelCol& instance() { static DebugListModelCol i; return i; }
//! @section Items
    DebugListModel& get(const QString& name) {
        auto it = models_.find(name);
        if (it != models_.end()) {
            return *it.value();
        }

        auto* newModel = new DebugListModel();
        beginResetModel();
        models_.insert(name, newModel);
        modelList_.append(newModel);
        endResetModel();

        return *newModel;
    }

    QStringList     getModelNames();
//=============================================================================
//! @section overrides
    int rowCount(const QModelIndex &parent = QModelIndex()) const override { return modelList_.count(); }
    QVariant data(const QModelIndex &index, int role) const override {
        if (!index.isValid() || index.row() < 0 || index.row() >= models_.keys().size())
            return QVariant();

        DebugListModel* model = modelList_[index.row()];

        switch (role) {
            case tabName:
                return "todo";
            case debugList:
                return QVariant::fromValue(model);
            default:
                break;
            }

        return QVariant();
    }
    QHash<int, QByteArray> roleNames() const override
    {
        return
        {
        { tabName,   "tabName" },
        { debugList, "debugList" }
        };
    }
protected:
//! @section Data
    QMap<QString, DebugListModel*> models_;
    QList<DebugListModel*>         modelList_;
//! @section Friends
};

///@view:end
