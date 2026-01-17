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
 * @class OregContainer
 * @brief Container of OregObserver.
 */
#pragma once

class OregObject;
class OregObserver;
class OregFilter;

#include <QString>

class OregContainer {
public:
//! Construction
    OregContainer(const QString& name);
    virtual ~OregContainer();
//! Methods
    void oo_filterSet(OregFilter* filter);
    const OregFilter* oo_filter() const { return filter_; }

    QString name() const { return name_; }

    virtual bool oo_solveContainment(OregObject* object, bool force) = 0;
    virtual void oo_onObserverChange(OregObserver* object) = 0;
    virtual void oo_removeObserver(OregObserver* observer) = 0;
    virtual void oo_clean() {};

    virtual QString oo_to_string() const   { return "???????????????????????????????"; }
//=============================================================================
protected:
//! Data
    bool oo_fresh_ = true;
    bool mutableContainment_ = false;

    OregFilter* filter_ = nullptr;
    QString name_;

    friend class OregPool;
    friend class OregObserver;
};
