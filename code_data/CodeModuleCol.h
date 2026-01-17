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

#include "CodeModule.h"
#include "CodeNode.h"

#include <map>

class CodeModule;

/// @view:beg

//=============================================================================
class  CodeModuleCol
{
public:
    CodeModuleCol();

    //<METHODS>
    //bool add(const QString& name, const QString& path);

    CodeModule* get(const QString& name);
    const CodeModule* get(const QString& name) const;

    bool contains(const QString& name) const;
    int  count() const;

    QStringList names() const;

    void add(const QString& path);

//=============================================================================
protected:
    std::map<QString, CodeModule*> modules_;
};

/// @view:end
