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
#include "OregContainerList.h"
#include "CodeModule.h"
#include "CodeNodeModuleFilter.h"

#include <QAbstractListModel>
#include <QStringList>
#include <QDir>

/// @view:beg

class AnalyzerModuleFilesModel : public QAbstractListModel, public OregContainerList
{
    Q_OBJECT
public:
    enum Roles {
        FileDataRole = Qt::UserRole + 1
    };

    explicit AnalyzerModuleFilesModel(QObject* parent = nullptr)
        : QAbstractListModel(parent),
          OregContainerList("AnalyzerModuleFilesModel")
    {}

    explicit AnalyzerModuleFilesModel(
        const QStringList& names,
        QObject* parent = nullptr)
        : QAbstractListModel(parent),
        OregContainerList("AnalyzerModuleFilesModel")
    {
        for (const QString& n : names)
            files_.append(AnalyzerModuleFileData(n));
    }

    void resetFromNames(const QStringList& names)
    {
        beginResetModel();
        files_.clear();
        for (const QString& n : names)
            files_.append(AnalyzerModuleFileData(n));
        endResetModel();
    }

    //## new: mirrors AnalyzerModuleCol::oo_solveContainment
    bool oo_solveContainment(OregObject* object, bool force) override
    {
        if (!OregContainerList::oo_solveContainment_prerequisities_(object, force)) return false;

        CodeNode* node = dynamic_cast<CodeNode*>(object);
        if (node == nullptr) return false;

        // only accept nodes that belong to our module
        if (!module_ || !node->module()) return false;
        if (node->module()->name() != module_->name()) return false; //##

        OregContainerList::oo_solveContainment_(object);

        const int row = files_.count();
        beginInsertRows(QModelIndex(), row, row);
        files_.append(AnalyzerModuleFileData(node->name()));
        endInsertRows();

        return true;
    }

    //## new: mirrors AnalyzerModuleCol::oo_onObserverChange
    void oo_onObserverChange(OregObserver* observer) override
    {
        CodeNode* node = dynamic_cast<CodeNode*>(observer->oo_object());
        if (node == nullptr) return;

        const int row = observer->oo_container_index_;

        if (row < 0 || row >= files_.count()) return;

        // rebuild the data entry for this row
        files_[row] = AnalyzerModuleFileData(node->name()); //##

        const QModelIndex modelIndex = createIndex(row, 0);
        emit dataChanged(modelIndex, modelIndex, { FileDataRole });
    }

    //## new: set the owning module so oo_solveContainment can filter
    void setModule(CodeModule* module)
    {
        module_ = module;
        if (module_)
            oo_filterSet(new CodeNodeModuleFilter(module_->name()));
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
    CodeModule* module_ = nullptr; //## new: needed for containment filter
    QList<AnalyzerModuleFileData> files_;

    friend class AnalyzerModuleCol;
};

/// @view:end
