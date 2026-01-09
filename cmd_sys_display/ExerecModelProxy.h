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
/**
 * @class Cmd
 * @brief Command Class.
 */
#pragma once

#include "ExerecModel.h"
#include "Cmds_exerec.h"
#include "Model_filteroutCmds.h"

#include <QSortFilterProxyModel>

///@view:beg

//=============================================================================
class  ExerecModelProxy: public QSortFilterProxyModel {
//=============================================================================
public:
//! @section Construction
    ExerecModelProxy() {
        setSourceModel(&ExerecModel::inst());
    }
    static ExerecModelProxy& inst() { static ExerecModelProxy i; return i; }
//! @section Methods
//! @section Overrides
    bool lessThan(const QModelIndex &top, const QModelIndex &bottom) const override {
        return top.row() > bottom.row();
    }
    void invalidateFilterOverride() { QSortFilterProxyModel::invalidateFilter(); }
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override {
            // Get the index for the column you want to check (e.g., column 0)
        QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
        QString value = index.data(ExerecModel::EExerecModelRoles::cmdName).toString();
        bool result = !Model_filteroutCmds::inst().contains(value);
        return result;
    }
//=============================================================================
protected:
//! @section Data
    friend class Cmds_exerec;
};

///@view:end









