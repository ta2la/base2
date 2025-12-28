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

/// @view:beg

//=============================================================================
class AnalyzerModuleCol
//=============================================================================
{
public:
    /// @section Construction
    AnalyzerModuleCol() = default;

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
        modules_.append(AnalyzerModule(dir));
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

    //=============================================================================
protected:
    /// @section Data
    QList<AnalyzerModule> modules_;
};
/// @view:beg
