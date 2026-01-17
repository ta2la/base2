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
 * @file Cmds_test.cpp
 * @brief Implementation of Cmds_test class.
 */

#include "Cmds_oreg_test.h"
#include "OregObjectTest.h"
#include "OregObserver.h"
#include "OregPool.h"
#include "OregContainerList.h"
#include "OregUpdateLock.h"

#include <QTextStream>

//=============================================================================
int Cmds_oreg_test::oreg_test_setup(CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context)
{
    OregUpdateLock lock;

    new OregObjectTest("Object 1");
    new OregObjectTest("Object 2");
    new OregObjectTest("Object 3");

    return 0;
}

//=============================================================================
QString Cmds_oreg_test::displayObject_(OregObject* object)
{
    QString result;
    QTextStream stream(&result);
    stream  << "<br/>" << "[" << object->oo_id_ << "] " << object->oo_to_string()
         << " " << OregObject::oo_state_to_string(object->oo_state());
    return result;
}

//=============================================================================
int Cmds_oreg_test::oreg_test_display_pool(CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context)
{
    QString di;

    for (OregObject* object : OregPool::instance().oo_objects_) {
        di += displayObject_(object);
    }

    int i = 0;
    for (OregContainer* cont: OregPool::instance().containers_) {
        di += QString() + "<br/><a href='oreg_test_container_display " + QString::number(i)
                               + "'>CONTAINER " + QString::number(i) + "</a>";
        i++;
    }

    args.append(di, "RESULT");
    return 1;
}

//=============================================================================
int Cmds_oreg_test::oreg_test_create_container(CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context)
{
    OregUpdateLock lock;
    new OregContainerList("oreg_test_create_container");
    return 0;
}

//=============================================================================
int Cmds_oreg_test::oreg_test_container_display(CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context)
{
    int index = 0;
    if (args.count() > 1) {
        CmdArg arg = args.get(1);
        index = arg.value().toInt();
    }

    if ( index >= OregPool::instance().containers_.count() ) return 0;

    QString di;
    di += QString() + "<br/>CONTAINER" + QString::number(index) + ":";

    OregContainer* cont = OregPool::instance().containers_.at(index);
    OregContainerList* listCont = dynamic_cast<OregContainerList*>(cont);

    if (listCont != nullptr) {
        for(OregObserver* obs: listCont->oo_items_) {
            di += displayObject_(obs->oo_object_);
        }
    }
    else {
        di += " ERROR";
    }

    args.append(di, "RESULT");

    return 1;
}

//=============================================================================
int Cmds_oreg_test::oreg_test_pool_solve(CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context)
{
    OregPool::instance().solveChanges();
    return 0;
}


//=============================================================================
