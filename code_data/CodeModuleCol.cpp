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

#include "CodeModuleCol.h"

#include <QFileInfo>

/// @view:beg

CodeModuleCol::CodeModuleCol()
{
}

CodeModule* CodeModuleCol::get(const QString& name)
{
    auto it = modules_.find(name);
    if (it == modules_.end())
        return nullptr;

    return it->second;
}

const CodeModule* CodeModuleCol::get(const QString& name) const
{
    auto it = modules_.find(name);
    if (it == modules_.end())
        return nullptr;

    return it->second;
}

bool CodeModuleCol::contains(const QString& name) const
{
    return modules_.find(name) != modules_.end();
}

int CodeModuleCol::count() const
{
    return static_cast<int>(modules_.size());
}

QStringList CodeModuleCol::names() const
{
    QStringList result;
    for (const auto& it : modules_) {
        result.append(it.first);
    }
    return result;
}

void CodeModuleCol::add(const QString& path)
{
    CodeModule* module = new CodeModule(path);
    modules_.insert({module->name(), module});
    module->loadFiles();
}

QList<CodeConnector> CodeModuleCol::connectors() const
{
    QList<CodeConnector> result;

    // iterate modules
    for (const auto& mit : modules_) {
        const CodeModule* module = mit.second;
        if (!module)
            continue;

        // then CodeNodeCol nodes_
        const CodeNodeCol& nodes = module->nodes();

        for (const auto& nit : nodes.nodes_) {
            const CodeNode* node = nit.second;
            if (!node)
                continue;

            const QString& from = node->name();

            // CodeNode = first
            // QSet<QString> dependencies_ = second
            for (const QString& to : node->dependencies_) {
                result.append(CodeConnector(CodeNodeAddress(mit.second->name(), from),
                                            CodeNodeAddress(nit.second->name(), to)));
            }
        }
    }

    return result;
}

QStringList CodeModuleCol::nodes() const
{
    QStringList result;

    for (const auto& mit : modules_) {
        const CodeModule* module = mit.second;
        if (!module)
            continue;

        const CodeNodeCol& nodes = module->nodes();

        for (const auto& nit : nodes.nodes_) {
            const CodeNode* node = nit.second;
            if (!node)
                continue;

            result.append(node->name());
        }
    }

    return result;
}

QStringList CodeModuleCol::filePathsForNode(const QString& nodeName) const
{
    QStringList result;

    for (const auto& mit : modules_) {
        const CodeModule* module = mit.second;
        if (!module)
            continue;

        const CodeNode* node = module->nodes().get(nodeName);
        if (!node)
            continue;

        const QString base = node->dir();   // full file path without suffix
        const QString dir  = QFileInfo(base).absolutePath();
        const QString name = node->name();

        for (const QString& ext : node->extensions()) {
            if (ext == "h")
                result << dir + "/" + name + ".h";
            else if (ext == "cpp")
                result << dir + "/" + name + ".cpp";
            else
                result << dir + "/" + name + "." + ext;
        }
    }

    return result;
}

CodeNode* CodeModuleCol::get(const CodeNodeAddress& addr)
{
    //if (!addr.isValid())
    //    return nullptr;

    if (addr.module.isEmpty()) {
        for (auto& it : modules_) {
            CodeModule* module = it.second;
            if (!module)
                continue;

            CodeNode* node = module->nodes().get(addr.node);
            if (node)
                return node;
        }
    }

    CodeModule* module = get(addr.module);
    if (!module)
        return nullptr;

    return module->nodes().get(addr.node);
}

const CodeNode* CodeModuleCol::get(const CodeNodeAddress& addr) const
{
    //if (!addr.isValid())
    //    return nullptr;

    if (addr.module.isEmpty()) {
        for (auto& it : modules_) {
            CodeModule* module = it.second;
            if (!module)
                continue;

            CodeNode* node = module->nodes().get(addr.node);
            if (node)
                return node;
        }
    }

    const CodeModule* module = get(addr.module);
    if (!module)
        return nullptr;

    return module->nodes().get(addr.node);
}

/// @view:end
