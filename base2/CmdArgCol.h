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
 * @class CmdArgCol
 * @brief Collection of arguments.
 */
#pragma once

#include "CmdArg.h"

#include <QVector>

///@view:beg

//=============================================================================
class  CmdArgCol {
//=============================================================================
public:
    enum EStyle { AS_NORMAL, AS_FORMATTED, AS_JSON };
//! @section Construction
    CmdArgCol() {};
    CmdArgCol(const QString& args);
//<METHODS>
    int append(const QString& value, const QString& name = "");
    int appendError(const QString& text) { return append(text, "ERROR"); }
    int appendWarning(const QString& text) { return append(text, "WARNING"); }
    size_t count() { return args_.count(); }
    CmdArg get(size_t index);
    CmdArg get(const QString& name, const QString& implicitValue = "", size_t index = 0);
    const CmdArg* getArg(const QString& name, int index = 0) const;

    //! @note last == 0 - all
    //! last positive - last arguments;
    //! last negative - without |last| at end
    QString toString( EStyle style = AS_NORMAL, int last = 0);

    int sizeOccupied() {
        int result = 0;
        for (CmdArg& arg: args_) { result+=arg.name().length()+arg.value().length(); }
        return result;
    }

    bool isNull() const { return args_.isEmpty(); }
//=============================================================================
protected:
//! @section Data
    QVector<CmdArg> args_;
};

///@view:end
