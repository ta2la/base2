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
 * @file InfoItemBase.cpp
 * @brief Implementation of class InfoItemBase
 */

#include "OregContainerList.h"
#include "OregFilter.h"
#include "OregObject.h"
#include "OregObserver.h"
#include "OregPool.h"

//=============================================================================
OregContainerList::OregContainerList(const QString& name,
    std::function<OregObserver*(OregObject*)> creator) :
    OregContainer(name),
    creator_(creator)
{
}

//=============================================================================
OregContainerList::~OregContainerList()
{
    for (OregObserver* obs: qAsConst(oo_items_)) {
        delete obs;
    }

    OregPool::instance().containers_.removeOne(this);
}

//=============================================================================
void OregContainerList::oo_clean()
{
    for (OregObserver* obs: qAsConst(oo_items_)) {
        delete obs;
    }
    oo_items_.clear();
}

//=============================================================================
bool OregContainerList::oo_solveContainment_prerequisities_(OregObject* object, bool force)
{
    if (mutableContainment_) {
        assert(0);
        return false;
    }

    if (!force && object->oo_state()!=OregObject::OO_STATE_NEW) return false;

    if ( filter_!=nullptr && filter_->pass(object)==false ) return false;

    return true;
}

//=============================================================================
OregObserver* OregContainerList::oo_solveContainment_(OregObject* object)
{
    OregObserver* observer = creator_(object); //new OregObserver(object);
    oo_items_.append(observer);
    observer->oo_container_ = this;
    observer->oo_container_index_ = oo_items_.count()-1;
    return observer;
}

//=============================================================================
bool OregContainerList::oo_solveContainment(OregObject* object, bool force)
{
    if (!oo_solveContainment_prerequisities_(object, force)) return false;
    oo_solveContainment_(object);
    return true;
}

//=============================================================================
void OregContainerList::oo_removeObserver(OregObserver* observer)
{
    oo_items_.removeOne(observer);
    for(int i = 0; i < oo_items_.count(); i++) oo_items_[i]->oo_container_index_ = i;
}

//=============================================================================
OregObserver* OregContainerList::oo_getItem(int index)
{
    return oo_items_[index];
}

//=============================================================================
const OregObserver* OregContainerList::oo_getItem(int index) const
{
    return oo_items_.at(index);

}

//=============================================================================
QString OregContainerList::oo_to_string() const
{
    QString result;

    for (int i = 0; i < oo_count(); i++) {
        result.append("■");
        result.append(oo_getItem(i)->oo_object()->oo_to_string());
    }

    return result;
}

//=============================================================================
