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
 * @class OregObject
 * @brief Object which can be referenced by OregObserver.
 */
#pragma once

#include <QList>
#include <QString>

class OregObserver;

class OregObject {
public:
//! Enums
    enum EState { OO_STATE_NEW, OO_STATE_CHANGED, OO_STATE_UPTODATE };
    enum EStringFormat { SF_BASIC, SF_CMDS };
//! Construction
    OregObject(bool outOfPool = false);
    virtual ~OregObject();
//! Methods
    EState          oo_state() const { return oo_state_; }
    void            oo_changed();
    int             oo_id() const    { return oo_id_; }

//! Methods minor
    static QString  oo_state_to_string(EState state);
    virtual QString oo_to_string(EStringFormat format = SF_BASIC) const   { Q_UNUSED(format); return "__UNDEFINED__"; }
    int             oo_update_count() const { return oo_update_count_; }
//=============================================================================
protected:
//! Data
    int                  oo_id_;
    EState               oo_state_;
    QList<OregObserver*> oo_observers_;
    int                  oo_update_count_ = 0;
//! @section Friends
    friend class OregObserver;
    friend class Cmds_oreg_test;
    friend class OregPool;
};
