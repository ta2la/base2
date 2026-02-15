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

#include "OregObject.h"
#include "OregPool.h"

/// @view:beg

//=============================================================================
OregObject::OregObject(bool outOfPool) :
    oo_id_(++OregPool::instance().maxId_),
    oo_state_(OO_STATE_NEW)
{
    OregPool& pool = OregPool::instance();
    QList<OregObject*> debugList = pool.oo_objects_.values();
    if (outOfPool) return;
    assert(pool.solvingChangesInProgress_ == false);
    pool.oo_objects_.insert(oo_id_, this);
    qDebug("pool");
}

//=============================================================================
OregObject::~OregObject()
{
    OregPool::instance().oo_objects_.remove(oo_id_);
}

//=============================================================================
void OregObject::oo_changed()
{
    assert(OregPool::instance().solvingChangesInProgress_ == false);
    if (oo_state_ != OO_STATE_UPTODATE) return;
    oo_state_ = OO_STATE_CHANGED;
    oo_update_count_++;
}

//=============================================================================
QString OregObject::oo_state_to_string(EState state)
{
    switch (state) {
    case OO_STATE_NEW:      return "NEW";
    case OO_STATE_CHANGED:  return "CHANGED";
    case OO_STATE_UPTODATE: return "UPTODATE";
    default: ;
    }

    return "unknown";
}

//=============================================================================

/// @view:end
