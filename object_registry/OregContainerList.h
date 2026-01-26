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
 * @class OregContainerList
 * @brief OregContainer specialization.
 */
#pragma once

#include "OregContainer.h"
#include "OregObserver.h"

#include <QList>
#include <functional>

class OregObject;
class OregObserver;

/// @view:beg

class OregContainerList : public OregContainer {
public:
//! Construction
    OregContainerList(const QString& name,
       std::function<OregObserver*(OregObject*)> creator =
       [](OregObject* object) { return new OregObserver(object); });
    virtual ~OregContainerList();
//! Methods
    int oo_count() const { return oo_items_.count(); }
    OregObserver* oo_getItem(int index);
    const OregObserver* oo_getItem(int index) const;
//! Overrides
    bool oo_solveContainment(OregObject* object, bool force) override;
    void oo_onObserverChange(OregObserver* object) override {};
    void oo_removeObserver(OregObserver* observer) override;
    QString oo_to_string() const override;
    void oo_clean() override;
//=============================================================================
protected:
//! Data
    std::function<OregObserver*(OregObject*)> creator_;
    QList<OregObserver*> oo_items_;
//! @section Internals
    bool oo_solveContainment_prerequisities_(OregObject* object, bool force);
    OregObserver* oo_solveContainment_(OregObject* object);
//! @section Friends
    friend class Cmds_oreg_test;
};

/// @view:end
