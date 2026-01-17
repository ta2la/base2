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

#include "OregContainer.h"
#include <OregPool.h>
#include "OregFilter.h"

void OregContainer::oo_filterSet(OregFilter* filter)
{
    if (filter_ != nullptr) {
        delete filter_;
        oo_clean();
        oo_fresh_ = false;
    }
    filter_ = filter;
}

//=============================================================================
OregContainer::OregContainer(const QString& name) :
    name_(name)
{
    if ( OregPool::instance().solvingChangesInProgress_ ) {
        OregPool::instance().containersNew_.append(this);
    }
    else {
        OregPool::instance().containers_.append(this);
    }
}

//=============================================================================
OregContainer::~OregContainer()
{
    OregPool::instance().containers_.removeOne(this);
    OregPool::instance().refreshDebug();
}

//=============================================================================
