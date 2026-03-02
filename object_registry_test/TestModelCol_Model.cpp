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

#include "TestModelCol_Model.h"
#include "TestObject.h"
#include "OregUpdateLock.h"
#include "OregPool.h"

//=============================================================================
TestModelCol_Model& TestModelCol_Model::inst()
{
    static TestModelCol_Model i;
    return i;
}

//=============================================================================
TestModelCol_Model::TestModelCol_Model()
{
    new TestModel(0,  10, this);
    new TestModel(11, 30, this);

    OregUpdateLock lock;
    new TestObject(5);
    new TestObject(15);
    new TestObject(25);
}

//=============================================================================
int TestModelCol_Model::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return items_.count();
}

//=============================================================================
QVariant TestModelCol_Model::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (role != DataRole)  return QVariant();
    return QVariant::fromValue(static_cast<QObject*>(items_.at(index.row())));
}

//=============================================================================
QHash<int, QByteArray> TestModelCol_Model::roleNames() const
{
    return { { DataRole, "subModel" } };
}
