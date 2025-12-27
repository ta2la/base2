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
 * @class CmdArg
 * @brief Command argument class.
 */
#pragma once

#include "CmdArgCol.h"

#include <QString>

///@view:beg

class  CmdArgCol_parser {
//=============================================================================
public:
//! @section Construction
    CmdArgCol_parser() = delete;
//<METHODS>
    static void createList_old(const QString& args, CmdArgCol& argsCol);

    static void splitCustomAppend(QStringList& result, QString& current, bool& isNoname);
    static void splitCustom(const QString &input, QStringList& result );
    static void createList(QStringList& divided, CmdArgCol& args);

    static QString afs_single_test(const QString testName, const QString& cmd);
protected:
    static bool isJsonQuote_(const QChar& ch);
};

///@view:end
