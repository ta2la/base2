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

#include "CodeModule.h"
#include "CodeNode.h"
#include "CodeNodeModuleFilter.h"

#include <QDir>
#include <QFileInfo>
#include <QDirIterator>

/// @view:beg

CodeModule::CodeModule(const QString& path)
    //filteredNodes_("CodeModuleContainer")
{
    if (path.isEmpty()) {
        path_.clear();
        return;
    }

    QFileInfo fi(path);

    // keep absolute, normalize
    if (fi.isAbsolute()) {
        path_ = QDir::cleanPath(fi.absoluteFilePath());
    }
    else {
        QDir cwd = QDir::current();
        path_ = QDir::cleanPath(cwd.absoluteFilePath(path));
    }

    name_ = QFileInfo(path_).fileName();

    /*filteredNodes_.oo_filterSet(
        new CodeNodeModuleFilter(name_)
        );*/
}

QString CodeModule::oo_to_string(EStringFormat format) const
{
    if (format == SF_BASIC){
        return QString() + "CodeModule: <b>" + name_ + "</b>";
    }
    return OregObject::oo_to_string(format);
}

void CodeModule::loadFiles(bool subdirs, bool strict)
{
    if (path_.isEmpty())
        return;

    QDir root(path_);
    if (!root.exists())
        return;

    const QDirIterator::IteratorFlags flags =
        subdirs ? QDirIterator::Subdirectories
                : QDirIterator::NoIteratorFlags;

    QDirIterator it(
        root.absolutePath(),
        QStringList() << "*.h" << "*.cpp" << "*.qml",
        QDir::Files | QDir::NoSymLinks,
        flags
        );

    while (it.hasNext()) {
        const QString filePath = it.next();

        if (strict) {
            QFile f(filePath);
            if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
                continue;

            const QString content =
                QString::fromUtf8(f.readAll());

            if (!content.contains("@view:beg"))
                continue;
        }

        QFileInfo fi(filePath);

        const QString base = fi.completeBaseName();
        const QString ext  = fi.suffix().toLower();

        CodeNode* node = nodes_.get(base);
        if (!node) {
            node = new CodeNode(filePath, this);
            nodes_.add(node);
        } else {
            node->addExtension(ext);
        }

        node->loadDependencies(filePath);
    }
}

/// @view:end
