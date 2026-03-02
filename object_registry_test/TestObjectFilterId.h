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
 * @class TestObjectFilterId
 * @brief Filters TestObject by id.
 */
#pragma once

#include "OregFilter.h"
#include "TestObject.h"

class TestObjectFilterId : public OregFilter {
public:
//! Construction
    TestObjectFilterId(int id) : id_(id) {}
//! @section Overrides
    bool pass(OregObject* object) override {
        TestObject* to = dynamic_cast<TestObject*>(object);
        if (!to) return false;
        return to->oo_id() == id_;
    }
//! Data
private:
    int id_;
//=============================================================================
};
