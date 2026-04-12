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
#pragma once

#include "T2lFileRetriever.h"

#include <QAbstractListModel>

class FileRetrieverModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY contentChanged)
//=============================================================================
public:
//! @section Enums
    enum Roles { ItemRole = Qt::UserRole + 1 };
//! @section Construction
    FileRetrieverModel(QObject* parent = nullptr) : QAbstractListModel(parent) {}
    static FileRetrieverModel& instance() { static FileRetrieverModel i; return i; }
//! @section Methods
    int  count() const { return retriever_ ? retriever_->count() : 0; }
    void setRetriever(T2l::FileRetriever* r) { retriever_ = r; if (r) r->setParent(this); refresh(); }
    T2l::FileRetriever* retriever() const { return retriever_; }
    Q_INVOKABLE void refresh() { beginResetModel(); endResetModel(); emit contentChanged(); }
//! @section Signals
signals:
    void contentChanged();
public:
//! @section Overrides
    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        if (parent.isValid()) return 0;
        return retriever_ ? retriever_->count() : 0;
    }
    QVariant data(const QModelIndex& index, int role) const override {
        if (!index.isValid() || role != ItemRole) return {};
        return QVariant::fromValue(retriever_->item(index.row()));
    }
    QHash<int, QByteArray> roleNames() const override {
        return { { ItemRole, "itemData" } };
    }
//=============================================================================
protected:
//! @section Data
    T2l::FileRetriever* retriever_ = nullptr;
};
