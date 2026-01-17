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
 * @class OregObserver
 * @brief OregObject reference.
 */
#pragma once

class OregObject;
class OregContainer;

class OregObserver {
public:
//! Construction
    OregObserver(OregObject* object, OregContainer* container = nullptr);
    virtual ~OregObserver();
//! Methods
    virtual void oo_onChange();
    OregObject*  oo_object() { return oo_object_; }
    const OregObject*  oo_object() const { return oo_object_; }
    int          oo_container_index() { return oo_container_index_; }
//=============================================================================
protected:
//! Data
    OregObject*    oo_object_;
    OregContainer* oo_container_ = nullptr;

    //can be replaced with object as other object than OregCOntainerList will be available.
    int            oo_container_index_ = -1;

    friend class Cmds_oreg_test;
    friend class OregContainerList;
};
