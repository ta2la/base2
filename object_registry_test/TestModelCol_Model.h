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
 * @class TestModelCol_Model
 * @brief Singleton collection of TestModel instances.
 */
#pragma once

#include "TestModel.h"

#include <QAbstractListModel>
#include <QList>

/// @view:beg

//=============================================================================
class TestModelCol_Model : public QAbstractListModel
//=============================================================================
{
    Q_OBJECT
public:
    enum Roles { DataRole = Qt::UserRole + 1 };

    static TestModelCol_Model& inst();

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
//=============================================================================
private:
    TestModelCol_Model();
    QList<TestModel*> items_;
    friend class TestModel;
};

/// @view:end
