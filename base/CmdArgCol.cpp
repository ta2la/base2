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
 * @file CmdArgCol.cpp
 * @brief Implementation of CmdArgCol class.
 */

#include "CmdArgCol.h"
#include "CmdArgCol_parser.h"

#include <QTextStream>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonDocument>

//=============================================================================

///@view:beg
CmdArgCol::CmdArgCol(const QString& args)
{
    QStringList divided;
    CmdArgCol_parser::splitCustom(args, divided);
    CmdArgCol_parser::createList(divided, *this);
}

//=============================================================================
int CmdArgCol::append(const QString& value, const QString& name)
{
    args_.append(CmdArg(name, value));
    return 1;
}

//=============================================================================
CmdArg CmdArgCol::get(size_t index)
{
    if (index >= count()) return CmdArg();
    return args_.at(index);
}

//=============================================================================
CmdArg CmdArgCol::get(const QString& name, const QString& implicitValue, size_t index)
{
    for ( int i = 0; i < args_.count(); i++ ) {
        CmdArg arg = get(i);
        if ( arg.name() != name) continue;
        if ( index-- == 0) return arg;
    }

    return CmdArg(name, implicitValue);
}

//=============================================================================
const CmdArg* CmdArgCol::getArg(const QString& name, int index) const
{
    for ( int i = 0; i < args_.count(); i++ ) {
        const CmdArg& argi = args_[i];
        if ( argi.name() != name) continue;
        if ( index-- == 0) return &argi;
    }

    return nullptr;
}

//=============================================================================
QString CmdArgCol::toString( EStyle style, int last )
{
    int from = 0;
    int to = count();

    if (last > 0) from = to-last;
    if (from < 0) from = 0;
    if (last < 0) to += last;
    if (to > count()) to = count();

    QString result;

    if ( style == AS_FORMATTED)
    {   for ( int i = from; i < to; i++) {
            CmdArg arg = get(i);
            if (i > 0) result += "<br/>";
            result += QString("&nbsp;&nbsp;&nbsp;&nbsp;[") + QString::number(i) + QString("] ");
            if (!arg.name().isEmpty()) result += arg.name();
            result += ":" + arg.value().replace(" ","&#9617;");
        }
    }
    else if (style == AS_JSON)
    {
        result = "[\n";
        for ( int i = from; i < to; i++) {
            CmdArg arg = get(i);
            result += QString("[\"") + arg.name()+ "\", \"" + arg.value() + "\"],\n";
        }
        result += "]";
    }
    else {
        for ( int i = from; i < to; i++) {
            CmdArg arg = get(i);
            if ( i > 0 ) result += " ";
            if (!arg.name().isEmpty()) {
                result += "--";
                result += arg.name();
                if (!arg.value().isEmpty() ) result += " ";
            }
            result += arg.value();
        }
    }

    return result;
}

///@view:end

//=============================================================================
