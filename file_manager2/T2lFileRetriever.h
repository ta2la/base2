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

#include "T2lFileRetrieverItem.h"

class FileRetrieverModel;

namespace T2l
{

class FileRetriever {
//=============================================================================
public:
//! @section Construction
    virtual ~FileRetriever() = default;
//! @section Methods
    virtual int               count() const = 0;
    virtual FileRetrieverItem item(int index) const = 0;
    virtual bool              ready() const { return count() > 0; }
    virtual void              setContent(const QString& content) { content_ = content; refreshParent(); }

    void setParent(FileRetrieverModel* parent) { parent_ = parent; }
    void refreshParent();
//=============================================================================
protected:
//! @section Data
    FileRetrieverModel* parent_ = nullptr;
    QString             content_;
};

} // namespace T2l
