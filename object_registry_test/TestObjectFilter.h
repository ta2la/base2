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
 * @class TestObjectFilter
 * @brief Filters TestObject by value range [from_, to_].
 */
#pragma once

#include "OregFilter.h"
#include "TestObject.h"

class TestObjectFilter : public OregFilter {
public:
//! Construction
    TestObjectFilter(long from, long to) : from_(from), to_(to) {}
//! @section Overrides
    QString toString() const override {
        return QString("Interval filter: %1-%2").arg(from_).arg(to_);
    }
    bool pass(OregObject* object) override {
        TestObject* to = dynamic_cast<TestObject*>(object);
        if (!to) return false;
        return to->value() >= from_ && to->value() <= to_;
    }
//! Data
private:
    long from_;
    long to_;
//=============================================================================
};
