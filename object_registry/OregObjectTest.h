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
 * @class OregObjectTest
 * @brief Specialization of OregObject for tests.
 */
#pragma once

#include "OregObject.h"

class OregObjectTest : public OregObject {
public:
//! Construction
    OregObjectTest(const QString& value ) : value(value) {}
//! Methods
    QString value;
//! @section Overrides
    QString oo_to_string(EStringFormat format = SF_BASIC) const override {
        if (format == SF_BASIC){
            return QString() + "OregObjectTest: " + value;
        }
        return OregObject::oo_to_string(format);
    }
//=============================================================================
};

