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

#include "CodeData.h"

/// @view:beg

// ---- module:code_data     CodeNode.cpp ----

#include "CodeNode.h"
#include "CodeModule.h"

#include <QDir>
#include <QFileInfo>

CodeNode::CodeNode(
    const QString& filePath,
    CodeModule* module
    )
    : module_(module)
{
    QFileInfo fi(filePath);

    // normalize full file path
    if (fi.isAbsolute()) {
        dir_ = QDir::cleanPath(fi.absoluteFilePath());
    }
    else {
        QDir cwd = QDir::current();
        dir_ = QDir::cleanPath(cwd.absoluteFilePath(filePath));
    }

    name_ = fi.completeBaseName();

    const QString ext = fi.suffix().toLower();
    if (!ext.isEmpty()) {
        addExtension(ext);
    }
}

void CodeNode::addExtension(const QString& ext)
{
    if (ext.isEmpty())
        return;

    const QString e = ext.toLower();

    // h + cpp are treated as whole code units
    if (e == "h" || e == "cpp") {
        extensions_.insert("whole h");
        extensions_.insert("whole cpp");
        return;
    }

    extensions_.insert(e);
}

QStringList CodeNode::extensions() const
{
    QStringList list = extensions_.values();
    list.sort();
    return list;
}

QString CodeNode::oo_to_string(EStringFormat format) const
{
    if (format == SF_BASIC) {
        return QString() +
               "CodeNode: " +
               name_;
    }

    return OregObject::oo_to_string(format);
}


/// @view:end
