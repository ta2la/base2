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

#include "CodeNodeCol.h"
#include "CodeNode.h"

/// @view:beg

CodeNode* CodeNodeCol::get(const QString& name)
{
    auto it = nodes_.find(name);
    if (it == nodes_.end())
        return nullptr;

    return it->second;
}

const CodeNode* CodeNodeCol::get(const QString& name) const
{
    auto it = nodes_.find(name);
    if (it == nodes_.end())
        return nullptr;

    return it->second;
}

bool CodeNodeCol::contains(const QString& name) const
{
    return nodes_.find(name) != nodes_.end();
}

int CodeNodeCol::count() const
{
    return static_cast<int>(nodes_.size());
}

QStringList CodeNodeCol::names() const
{
    QStringList result;
    for (const auto& it : nodes_) {
        result.append(it.first);
    }
    return result;
}

void CodeNodeCol::add(CodeNode* node)
{
    if (!node)
        return;

    nodes_.insert({ node->name(), node });
}


/// @view:end
