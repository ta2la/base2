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


#include <QString>
#include <QFileInfo>
#include <QDir>

/// @view:beg

class AnalyzerModule
//=============================================================================
{
public:
    /// @section Construction
    explicit AnalyzerModule(const QString& dirPath)
        : dirPath_(dirPath)
    {
    }

    /// @section Accessors
    const QString& dirPath() const { return dirPath_; }

//=============================================================================
protected:
    /// @section Data
    QString dirPath_;
};

/// @view:end
