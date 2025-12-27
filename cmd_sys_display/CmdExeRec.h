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
 * @class CmdExeRec
 * @brief Record of executed command.
 */

#pragma once

#include <QString>

///@view:beg

//=============================================================================
class  CmdExeRec {
//=============================================================================
public:
//! @section Construction
    CmdExeRec( bool found, const QString& argsIn, const QString& argsInOut,
               int index, const QString& sourceName, int sourceIndex) :
        argsIn(argsIn),
        argsOut(argsInOut.mid(argsIn.length())),
        index(index),
        found(found),
        sourceName(sourceName),
        sourceIndex(sourceIndex) {};
//! @section Data
    QString  argsIn;
    QString  argsOut;
    int      index;
    bool     found = true;
    QString  sourceName;
    int      sourceIndex;
//=============================================================================
};

///@view:end
