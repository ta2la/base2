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
 * @class OregPool
 * @brief Registry for OregObject.
 */
#pragma once

#include <QMap>

#include "DebugListModel.h"

class OregObject;
class OregContainer;

class OregPool {
public:
//! Construction
    static OregPool& instance() { static OregPool i; return i; }

//! Methods
    void solveChanges();
    void refreshDebug();
//=============================================================================
protected:
//! Data
    QMap<int, OregObject*> oo_objects_;

    int maxId_   = 0;
    int lock_    = 0;

    bool solvingChangesInProgress_ = false;

    QList<OregContainer*> containers_;
    QList<OregContainer*> containersNew_;

    OregPool();
//! Friends
    friend class OregObject;
    friend class Cmds_oreg_test;
    friend class OregContainer;
    friend class OregUpdateLock;
    friend class OregContainerList;

public:
    DebugListModel debugContainers;
};
