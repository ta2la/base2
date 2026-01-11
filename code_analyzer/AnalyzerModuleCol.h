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
#pragma once

#include "AnalyzerModule.h"
#include <QAbstractListModel>

#include "AnalyzerSys.h"

/// @view:beg

//=============================================================================
class AnalyzerModuleCol : public QAbstractListModel
//=============================================================================
{
    Q_OBJECT
public:
    enum Roles {
        DataRole = Qt::UserRole + 1
    };
    /// @section Construction
    explicit AnalyzerModuleCol(QObject* parent = nullptr)
        : QAbstractListModel(parent)
    {}

    int rowCount(const QModelIndex& parent = QModelIndex()) const override
    {
        Q_UNUSED(parent)
        return modules_.count();
    }

    QHash<int, QByteArray> roleNames() const override
    {
        return {
            { DataRole, "moduleData" }
        };
    }

    QVariant data(
        const QModelIndex& index,
        int role) const override
    {
        if (!index.isValid())
            return QVariant();

        if (role != DataRole)
            return QVariant();

        return QVariant::fromValue(
            modules_.at(index.row()).data()
            );
    }

    operator QStringList() const
    {
        QStringList result;
        for (const AnalyzerModule& m : modules_) {
            result.append(m.dirPath());
        }
        return result;
    }

    /// @section Methods
    void add(const QString& dir)
    {
        const QString norm = QDir::cleanPath(QDir(dir).absolutePath());

        const int row = modules_.count();
        beginInsertRows(QModelIndex(), row, row);
        modules_.append(AnalyzerModule(norm));
        endInsertRows();
    }

    int count() const
    {
        return modules_.count();
    }

    const AnalyzerModule& get(int index) const
    {
        return modules_.at(index);
    }

    AnalyzerModuleCol& operator<<(const AnalyzerModule& module)
    {
        modules_.append(module);
        return *this;
    }

    QString first() const
    {
        Q_ASSERT(!modules_.isEmpty());
        return modules_.first().dirPath();
    }

    bool isEmpty() const
    {
        return modules_.isEmpty();
    }

    void setModuleUsed(int index, bool used)
    {
        if (index < 0 || index >= modules_.count()) return;

        AnalyzerModule& m = modules_[index];
        if (m.used() == used) return;

        m.setUsed(used);

        const QModelIndex modelIndex = createIndex(index, 0);
        emit dataChanged(modelIndex, modelIndex, { DataRole });
    }

    void loadFilesModels()
    {
        const int n = modules_.count();
        for (int i = 0; i < n; ++i) {
            modules_[i].buildFilesModel();
        }
    }

    void resetAllFilesModels()
    {
        for (AnalyzerModule& m : modules_) {
            if (m.filesModel_) {
                auto* fm =
                    static_cast<AnalyzerModuleFilesModel*>(m.filesModel_);

                fm->beginResetModel();
                fm->endResetModel();
            }
        }
    }

    //=============================================================================
protected:
    /// @section Data
    QList<AnalyzerModule> modules_;
};
/// @view:beg
