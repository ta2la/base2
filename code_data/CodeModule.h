//
// Copyright (C) 2025 Petr Talla. [petr.talla@gmail.com]
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
 * @class CodeData
 * @brief
 */
#pragma once

#include "OregObject.h"
#include "CodeNodeCol.h"
#include "CodeNode.h"
#include "OregContainerList.h"

/// @view:beg

class  CodeModule : public OregObject
//=============================================================================
{
public:
    //!` @section Construction
    CodeModule(const QString& path);
    void loadFiles(bool subdirs, bool strict);
    //! @section Methods
    const QString& name() const { return name_; }
    const QString& path() const { return path_; }

    bool used() const { return used_; }
    void setUsed(bool value)
    {
        if (used_ == value) return;
        used_ = value;
        oo_changed();
    }

    CodeNodeCol& nodes()       { return nodes_; }
    const CodeNodeCol& nodes() const { return nodes_; }

//=============================================================================
    //! @section Overrides
    QString oo_to_string(EStringFormat format = SF_BASIC) const override;
protected:
    QString name_;
    QString path_;

    CodeNodeCol       nodes_;
    bool used_ = true;
};

/// @view:end
