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


#include <QAbstractListModel>
#include <QStringList>

/// @view:beg

class AnalyzerModuleFilesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        FileNameRole = Qt::UserRole + 1
    };

    explicit AnalyzerModuleFilesModel(QObject* parent = nullptr)
        : QAbstractListModel(parent)
    {}

    // Demo population
    explicit AnalyzerModuleFilesModel(const QStringList& demoFiles,
                                      QObject* parent = nullptr)
        : QAbstractListModel(parent),
        files_(demoFiles)
    {}

    int rowCount(const QModelIndex& parent = QModelIndex()) const override
    {
        Q_UNUSED(parent)
        return files_.count();
    }

    QVariant data(const QModelIndex& index, int role) const override
    {
        if (!index.isValid() || index.row() < 0 || index.row() >= files_.size())
            return {};

        if (role == FileNameRole)
            return files_.at(index.row());

        return {};
    }

    QHash<int, QByteArray> roleNames() const override
    {
        return {
            { FileNameRole, "fileName" }
        };
    }

private:
    QStringList files_;
};

/// @view:end
