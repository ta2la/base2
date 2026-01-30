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

#include "OregPool.h"
#include "OregContainer.h"
#include "OregFilter.h"
#include "OregObject.h"
#include "OregObserver.h"

#include <QtGlobal>

//=============================================================================
OregPool::OregPool()
{
}

/// @view:beg

//=============================================================================
void OregPool::solveChanges()
{
    solvingChangesInProgress_ = true;

    //! @step check for containment
    for (OregContainer* container: containers_) {
        if (container->oo_fresh_) {
            for ( OregObject* object: OregPool::instance().oo_objects_) {
                container->oo_solveContainment(object, true);
            }
        }
        else {
            assert(container->mutableContainment_ == false);

            for (OregObject* obj: oo_objects_) {
                if (obj->oo_state() != OregObject::OO_STATE_NEW) continue;
                //! true for non-mutable containment
                container->oo_solveContainment(obj, false);
            }
        }
    }

    //! @update changes
    for (OregObject* obj: qAsConst(oo_objects_)) {
        if (obj->oo_state() != OregObject::OO_STATE_CHANGED) continue;

        for (OregObserver* observer : qAsConst(obj->oo_observers_)) {
            observer->oo_onChange();
        }
    }

    //! reset flags
    for (OregObject* obj: oo_objects_) {
        if (obj->oo_state() == OregObject::OO_STATE_UPTODATE) continue;
        obj->oo_state_ = OregObject::OO_STATE_UPTODATE;
    }

    for (OregContainer* container: containers_) {
        container->oo_fresh_ = false;
    }

    //! solve recurent containers
    for (OregContainer* container: containersNew_) {
        containers_.append(container);
    }
    containersNew_.clear();

    solvingChangesInProgress_ = false;

   // refreshDebug();
}

/// @view:end

//=============================================================================
void OregPool::refreshDebug()
{
    //! debug
    QStringList containers;
    for (OregContainer* cont : containers_) {
        QString item = QString() + cont->oo_filter()->saveJson() + "<b>" + cont->name() + "</b>: ";
        containers.append(item + cont->oo_to_string());
    }
    debugContainers.updateList(containers);
}

//=============================================================================
