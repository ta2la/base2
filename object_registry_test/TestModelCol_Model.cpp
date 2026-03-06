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
#include "CmdSys.h"

//=============================================================================
TestModelCol_Model& TestModelCol_Model::inst()
{
    static TestModelCol_Model i;
    return i;
}

//=============================================================================
TestModelCol_Model::TestModelCol_Model()
{
    CMD_SYS.execute("create_model_test -1000000 1000000");
    CMD_SYS.execute("create_model_test 0 10");
    CMD_SYS.execute("create_model_test 11 30");
    CMD_SYS.execute("create_model_test 40 100");
    CMD_SYS.execute("create_object_test 5 15 25 50");
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
    TestModel* m = items_.at(index.row());
    if (role == DataRole)  return QVariant::fromValue(static_cast<QObject*>(m));
    if (role == DescrRole) {
        QString descr = m->name();
        if (m->oo_filter()) descr += " - " + m->oo_filter()->toString();
        return descr;
    }
    return QVariant();
}

//=============================================================================
QHash<int, QByteArray> TestModelCol_Model::roleNames() const
{
    return { { DataRole, "subModel" }, { DescrRole, "descr" } };
}
