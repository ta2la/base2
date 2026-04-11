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
#pragma once

#include "T2lFileRetriever.h"
#include <QList>

namespace T2l
{

class FileRetrieverTest : public FileRetriever {
//=============================================================================
public:
//! @section Construction
    FileRetrieverTest() {
        items_.append(FileRetrieverItem("/home/pt/data/project1.t2d"));
        items_.append(FileRetrieverItem("/home/pt/data/floor_plan.t2d"));
        items_.append(FileRetrieverItem("/home/pt/data/section_A.t2d"));
    }
//! @section Overrides
    int               count() const override { return items_.size(); }
    FileRetrieverItem item(int index) const override { return items_.value(index); }
//=============================================================================
protected:
//! @section Data
    QList<FileRetrieverItem> items_;
};

} // namespace T2l
