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

#include "AnalyzerModuleFileData.h"
#include "AnalyzerCode.h"

#include <QAbstractListModel>
#include <QStringList>
#include <QDir>

/// @view:beg

class AnalyzerModuleFilesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        FileDataRole = Qt::UserRole + 1
    };

    explicit AnalyzerModuleFilesModel(QObject* parent = nullptr)
        : QAbstractListModel(parent)
    {}

    explicit AnalyzerModuleFilesModel(
        const QStringList& names,
        QObject* parent = nullptr)
        : QAbstractListModel(parent)
    {
        for (const QString& n : names)
            files_.append(AnalyzerModuleFileData(n));
    }

    void resetFromDir(const QString& dirPath)
    {
        beginResetModel();
        files_.clear();

        QDir dir(dirPath);
        if (dir.exists()) {
            const QStringList paths =
                AnalyzerCode::getFiles(
                    dir,
                    QStringList() << "*.h" << "*.cpp" << "*.qml"
                    );

            for (const QString& path : paths) {
                files_.append(
                    AnalyzerModuleFileData(
                         QFileInfo(path).completeBaseName()
                        )
                    );
            }
        }

        endResetModel();
    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override
    {
        Q_UNUSED(parent)
        return files_.count();
    }

    QVariant data(const QModelIndex& index, int role) const override
    {
        if (!index.isValid() || role != FileDataRole)
            return QVariant();

        return QVariant::fromValue(files_.at(index.row()));
    }

    QHash<int, QByteArray> roleNames() const override
    {
        return {
            { FileDataRole, "fileData" }
        };
    }

private:
    QList<AnalyzerModuleFileData> files_;
};

/// @view:end
