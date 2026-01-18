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

//=============================================================================
QStringList CodeNode::extractIncludes_(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return {};

    const bool isQml = filePath.endsWith(".qml", Qt::CaseInsensitive);

    QStringList result;
    QTextStream in(&file);

    while (!in.atEnd()) {
        const QString line = in.readLine().trimmed();

        if (isQml) {
            // QML: include může být kdekoliv (typicky v komentáři)
            if (!line.contains("#include"))
                continue;
        } else {
            // C/C++: musí začínat na #include (po trimu)
            if (!line.trimmed().startsWith("#include"))
                continue;
        }

        int firstQuote = line.indexOf('"');
        if (firstQuote < 0)
            continue;

        int secondQuote = line.indexOf('"', firstQuote + 1);
        if (secondQuote < 0)
            continue;

        const QString includeFile = line.mid(firstQuote + 1, secondQuote - firstQuote - 1);

        if (!includeFile.isEmpty()) result.append(includeFile);
    }

    return result;
}

void CodeNode::loadDependencies(const QString& filePath)
{
    const QStringList includes =
        extractIncludes_(filePath);

    for (const QString& inc : includes) {
        const QString to =
            QFileInfo(inc).completeBaseName();

        if (to.isEmpty())
            continue;

        if (to == name_)
            continue;

        dependencies_.insert(to);
    }
}

CodeNodeAddress CodeNode::address() const
{
    return CodeNodeAddress(
        module_ ? module_->name() : QString(),
        name_
        );
}

/// @view:end
