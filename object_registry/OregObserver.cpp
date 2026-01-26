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

#include "OregObserver.h"
#include "OregObject.h"
#include "OregContainer.h"

/// @view:beg

//=============================================================================
OregObserver::OregObserver(OregObject* object, OregContainer* container) :
    oo_object_(object),
    oo_container_(container)
{
    oo_object_->oo_observers_.append(this);
}

//=============================================================================
OregObserver::~OregObserver()
{
    oo_object_->oo_observers_.removeOne(this);
    if (oo_container_ != nullptr) {
        oo_container_->oo_removeObserver(this);
    }
}

//=============================================================================
void OregObserver::oo_onChange()
{
    if (oo_container_ != nullptr && !oo_container_->oo_fresh_ ) {
        oo_container_->oo_onObserverChange(this);
    }
}

//=============================================================================

/// @view:end
