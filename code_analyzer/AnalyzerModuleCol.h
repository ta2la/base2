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

//#include "AnalyzerModule.h"
#include "OregUpdateLock.h"
#include "OregContainerList.h"
#include "CodeModule.h"
#include "CodeData.h"
#include "AnalyzerModuleFilesModel.h"
#include "AnalyzerModuleData.h"

#include <QAbstractListModel>
#include <QString>
#include <QFileInfo>
#include <QDir>

/// @view:beg

//=============================================================================
class AnalyzerModuleCol : public QAbstractListModel, public OregContainerList
//=============================================================================
{
    Q_OBJECT
public:
    enum Roles {
        DataRole = Qt::UserRole + 1
    };
    /// @section Construction
    explicit AnalyzerModuleCol(QObject* parent = nullptr)
        : QAbstractListModel(parent),
        OregContainerList("testa")
    {}

    static AnalyzerModuleCol& inst() { static AnalyzerModuleCol i; return i; }

    void updateRow_(int row,  CodeModule* codeModule)
    {
        if (row < 0 || row >= modules_.count()) return;
        AnalyzerModuleData& m = modules_[row];
        m.used_ = codeModule->used();
        m.updateCount_++;
    }

    bool oo_solveContainment(OregObject* object, bool force) override
    {
        if (!OregContainerList::oo_solveContainment_prerequisities_(object, force)) return false;

        CodeModule* codeModule = dynamic_cast<CodeModule*>(object);
        if (codeModule == nullptr) return false;

        OregObserver* obs = OregContainerList::oo_solveContainment_(object);

        const int row = modules_.count();
        beginInsertRows(QModelIndex(), row, row);
        modules_.append(AnalyzerModuleData(codeModule->path(), true));
        updateRow_(row, codeModule);
        endInsertRows();

        return true;
    }

    void oo_onObserverChange(OregObserver* object) override
    {
        CodeModule* codeModule = dynamic_cast<CodeModule*>(object->oo_object());
        if (codeModule == nullptr) return;

        const int row = object->oo_container_index_;
        updateRow_(row, codeModule);

        const QModelIndex modelIndex = createIndex(row, 0);
        emit dataChanged(modelIndex, modelIndex, { DataRole });

    }

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

        /*return QVariant::fromValue(
            modules_.at(index.row()).data()
            );*/

        return QVariant::fromValue(modules_.at(index.row()) );
    }

    operator QStringList() const
    {
        QStringList result;
        for (const AnalyzerModuleData& m : modules_) {
            result.append(m.dirPath());
        }
        return result;
    }

    /// @section Methods
    /*void add(const QString& dir, bool subdirs, bool strict)
    {
        const QString norm = QDir::cleanPath(QDir(dir).absolutePath());

        const int row = modules_.count();
        beginInsertRows(QModelIndex(), row, row);
        modules_.append(AnalyzerModuleData(norm, subdirs));
        endInsertRows();


        OregUpdateLock l;
        CodeData::inst().modules().add(norm, subdirs, strict);

    }*/

    int count() const
    {
        return modules_.count();
    }

    const AnalyzerModuleData& get(int index) const
    {
        return modules_.at(index);
    }

    /*AnalyzerModuleCol& operator<<(const AnalyzerModuleData& module)
    {
        modules_.append(module);
        return *this;
    }*/

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

        AnalyzerModuleData& m = modules_[index];
        if (m.used() == used) return;

        m.setUsed(used);

        const QModelIndex modelIndex = createIndex(index, 0);
        emit dataChanged(modelIndex, modelIndex, { DataRole });
    }

    void resetAllFilesModels()
    {
        for (AnalyzerModuleData& m : modules_) {
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
    QList<AnalyzerModuleData> modules_;
};
/// @view:beg
