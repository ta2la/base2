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

#include "TestModel.h"
#include "TestObject.h"
#include "TestObjectFilter.h"
#include "TestModelCol_Model.h"

//=============================================================================
TestModel::TestModel(long from, long to, TestModelCol_Model* col_, QObject* parent)
    : QAbstractListModel(parent),
      OregContainerList("TestModel")
{
    oo_filterSet(new TestObjectFilter(from, to));

    TestModelCol_Model& col = col_ ? *col_ : TestModelCol_Model::inst();
    const int row = col.items_.count();
    col.beginInsertRows(QModelIndex(), row, row);
    col.items_.append(this);
    col.endInsertRows();
}

//=============================================================================
TestModel::~TestModel()
{
    TestModelCol_Model& col = TestModelCol_Model::inst();
    const int row = col.items_.indexOf(this);
    if (row < 0) return;
    col.beginRemoveRows(QModelIndex(), row, row);
    col.items_.removeAt(row);
    col.endRemoveRows();
}

//=============================================================================
bool TestModel::oo_solveContainment(OregObject* object, bool force)
{
    if (!OregContainerList::oo_solveContainment_prerequisities_(object, force)) return false;

    TestObject* testObj = dynamic_cast<TestObject*>(object);

    OregContainerList::oo_solveContainment_(object);

    const int row = items_.count();
    beginInsertRows(QModelIndex(), row, row);
    items_.append(TestModelItem(testObj));
    endInsertRows();

    return true;
}

//=============================================================================
void TestModel::oo_onObserverRemove(OregObserver* observer)
{
    int row = observer->oo_container_index();
    beginRemoveRows(QModelIndex(), row, row);
    items_.removeAt(row);
    endRemoveRows();
}

//=============================================================================
void TestModel::oo_onObserverChange(OregObserver* object)
{
    TestObject* testObj = dynamic_cast<TestObject*>(object->oo_object());
    if (!testObj) return;

    const int row = object->oo_container_index();
    if (row < 0 || row >= items_.count()) return;

    items_[row] = TestModelItem(testObj);

    const QModelIndex modelIndex = createIndex(row, 0);
    emit dataChanged(modelIndex, modelIndex, { DataRole });
}

//=============================================================================
int TestModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return items_.count();
}

//=============================================================================
QVariant TestModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (role != DataRole)  return QVariant();

    return QVariant::fromValue(items_.at(index.row()));
}

//=============================================================================
QHash<int, QByteArray> TestModel::roleNames() const
{
    return { { DataRole, "testItem" } };
}
