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
 * @file DebugListModel.cpp
 * @brief Implementation of class DebugListModel
 */

#include "DebugListModel.h"

#include <QtGlobal>

///@view:beg

//=============================================================================
DebugListModel::DebugListModel()
{
}

//=============================================================================
QVariant DebugListModel::data(const QModelIndex &modelIndex, int role) const
{
    if (!modelIndex.isValid()) return QVariant();

    switch (role) {
    case item_string: return QVariant(list_.at(modelIndex.row()));
    }

    return QVariant();
}

//=============================================================================
QHash<int, QByteArray> DebugListModel::roleNames() const
{
    return
    {
    { item_string, "item_string" }
    };
}

//=============================================================================
int DebugListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return list_.count();
}

//=============================================================================
void DebugListModel::prepend(const QString& out, int max)
{
    QStringList old = list_;
    old.prepend(out);
    if ( max>0 && old.count()>max) old.removeLast();

    updateList(old);
}

//=============================================================================
void DebugListModel::setGuarded(int index, const QString& value)
{
    assert(index >= 0);
    list_[index] = value;
    QModelIndex indexModel = createIndex(index, 0);
    emit dataChanged(indexModel, indexModel);
}

//=============================================================================
void DebugListModel::updateCount(int count)
{
    if ( list_.size() < count ) {
        beginInsertRows(QModelIndex(), list_.size(), count - 1);
        while (list_.size() < count) list_.append("");
        endInsertRows();
    }
    else if ( list_.size() > count ) {
         beginRemoveRows(QModelIndex(), count, list_.size() - 1);
        while (list_.size() > count) list_.removeLast();
        endRemoveRows();
    }
}

//=============================================================================
void DebugListModel::updateItem(const QString& value, int index)
{
    if (list_.at(index) == value) return;

    list_[index] = value;
    QModelIndex i = createIndex(index, 0);
    emit dataChanged(i, i);
}

//=============================================================================
void DebugListModel::updateList(const QStringList& list)
{
    int oldSize = list_.size();
    int newSize = list.size();

    if (newSize > oldSize) {
        beginInsertRows(QModelIndex(), oldSize, newSize - 1);
        for (int i = oldSize; i < newSize; ++i) {
            list_.append(list[i]);
        }
        endInsertRows();
    }

    if (newSize < oldSize) {
        beginRemoveRows(QModelIndex(), newSize, oldSize - 1);
        while (list_.size() != newSize) list_.removeLast();
        endRemoveRows();
    }

    for (int i = 0; i < std::min(newSize, oldSize); ++i) {
        if (list[i] != list_[i]) {
            list_[i] = list[i];
            QModelIndex index = createIndex(i, 0);
            emit dataChanged(index, index);
        }
    }
}

//=============================================================================
QString DebugListModel::getItem(int index)
{
    if (index >= list_.count()) return "UNAVAILABLE";
    return list_[index];
}

///@view:end


//=============================================================================


