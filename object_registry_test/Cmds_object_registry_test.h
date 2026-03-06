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
 * @class Cmds_object_registry_test
 * @brief Commands for testing object_registry.
 */
#pragma once

#include "CmdSys.h"
#include "TestObject.h"
#include "TestModel.h"
#include "TestModelCol_Model.h"
#include "TestModelItem.h"
#include "OregUpdateLock.h"
#include "OregFilter.h"
#include "OregPool.h"
#include "OregContainerList.h"

//=============================================================================
class Cmds_object_registry_test {
//=============================================================================
public:
//! @section Construction
    Cmds_object_registry_test() = delete;
//! @section Methods
    static void registerCmds() {
        CMD_SYS.add(
        "create_object_test",
        [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {
            if (args.count() < 2) return args.appendWarning("at least one numeric arg expected");
            OregUpdateLock lock;
            for (int i = 1; i < args.count(); i++) {
                new TestObject(args.get(i).value().toInt());
            }
            return 0;
        });
        CMD_SYS.add(
        "create_model_test",
        [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {
            if (args.count() < 3) return args.appendWarning("two numeric args expected: from to");
            long from = args.get(1).value().toLong();
            long to   = args.get(2).value().toLong();
            OregUpdateLock lock;
            TestModel* m = new TestModel(from, to);
            int index = OregPool::instance().containers_.indexOf(static_cast<OregContainer*>(m));
            args.append(QString::number(index), "container_index");
            return 0;
        });
        CMD_SYS.add(
        "display_model_col_test",
        [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {
            TestModelCol_Model& col = TestModelCol_Model::inst();
            QString di;
            di += QString("<br/>TestModelCol_Model: %1 models").arg(col.rowCount());
            for (int i = 0; i < col.rowCount(); i++) {
                TestModel* m = static_cast<TestModel*>(col.data(col.index(i), TestModelCol_Model::DataRole).value<QObject*>());
                if (!m) continue;
                QString filterStr = m->oo_filter() ? m->oo_filter()->toString() : "";
                di += QString("<br/> [%1] %2 items: %3").arg(i).arg(filterStr).arg(m->rowCount());
                for (int j = 0; j < m->rowCount(); j++) {
                    QVariant v = m->data(m->index(j), TestModel::DataRole);
                    TestModelItem item = v.value<TestModelItem>();
                    di += QString(" %1").arg(item.value());
                }
            }
            args.append(di, "RESULT");
            return 1;
        });
        CMD_SYS.add(
        "delete_object_test",
        [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {
            if (args.count() < 2) return args.appendWarning("at least one id arg expected");
            OregUpdateLock lock;
            for (int i = 1; i < args.count(); i++) {
                int id = args.get(i).value().toInt();
                OregObject* obj = OregPool::instance().findObject(id);
                if (!obj) { args.appendWarning(QString("id %1 not found").arg(id)); continue; }
                obj->oo_delete();
            }
            return 0;
        });
        CMD_SYS.add(
        "change_object_test",
        [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {
            if (args.count() < 3) return args.appendWarning("two args expected: id newValue");
            int id = args.get(1).value().toInt();
            long newValue = args.get(2).value().toLong();
            OregObject* obj = OregPool::instance().findObject(id);
            if (!obj) return args.appendWarning(QString("id %1 not found").arg(id));
            TestObject* testObj = dynamic_cast<TestObject*>(obj);
            if (!testObj) return args.appendWarning(QString("id %1 is not a TestObject").arg(id));
            OregUpdateLock lock;
            testObj->setValue(newValue);
            return 0;
        });
        CMD_SYS.add(
        "oreg_test_container_display_llm",
        [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {
            if (args.count() < 2) return args.appendWarning("index arg expected");
            int index = args.get(1).value().toInt();
            if (index >= OregPool::instance().containers_.count()) return args.appendWarning("index out of range");
            OregContainer* cont = OregPool::instance().containers_.at(index);
            QString filterStr = cont->oo_filter() ? cont->oo_filter()->toString() : "";
            QString di = QString("CONTAINER %1 filter: %2").arg(index).arg(filterStr);
            OregContainerList* listCont = dynamic_cast<OregContainerList*>(cont);
            if (!listCont) return args.appendWarning("not a list container");
            for (OregObserver* obs : listCont->oo_items_) {
                TestObject* t = dynamic_cast<TestObject*>(obs->oo_object_);
                if (t) di += QString(" [id:%1 val:%2]").arg(t->oo_id()).arg(t->value());
            }
            args.append(di, "RESULT");
            return 0;
        });
        CMD_SYS.add(
        "oreg_test_display_pool_llm",
        [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {
            QString di;
            for (OregObject* obj : OregPool::instance().oo_objects_) {
                if (!di.isEmpty()) di += " ";
                di += QString::number(obj->oo_id());
            }
            args.append(di, "RESULT");
            return 0;
        });
        CMD_SYS.add(
        "oreg_test_object_display",
        [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {
            if (args.count() < 2) return args.appendWarning("id arg expected");
            int id = args.get(1).value().toInt();
            OregObject* obj = OregPool::instance().findObject(id);
            if (!obj) return args.appendWarning(QString("id %1 not found").arg(id));
            args.append(obj->oo_to_string(), "RESULT");
            return 0;
        });
        CMD_SYS.add(
        "oreg_list_containers_llm",
        [](CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) -> int {
            OregPool& pool = OregPool::instance();
            if (pool.containers_.isEmpty()) return args.appendWarning("no containers");
            QString di;
            for (int i = 0; i < pool.containers_.size(); i++) {
                if (!di.isEmpty()) di += " ";
                di += QString::number(i);
            }
            args.append(di, "RESULT");
            return 0;
        });
    }
//=============================================================================
};
