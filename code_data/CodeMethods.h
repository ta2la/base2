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
/**
 * @class CodeData
 * @brief
 */
#pragma once

#include <QString>

/// @view:beg
class CodeMethods
{
public:
    static QByteArray extractView( const QString& filePath, bool useViews)
    {
        QFile f(filePath);
        if (!f.open(QIODevice::ReadOnly)) {
            CMD_SYS.execute( QString() +
                            "logcmd --ERROR AnalyzerCode::composeToFile cannot open " +
                            filePath );
            return QByteArray();
        }

        QByteArray input = f.readAll();
        f.close();

        if (!useViews) return input;

        static const QByteArray kAt          = QByteArrayLiteral("@");

        static const QByteArray kViewBeg     = kAt + "view:beg";
        static const QByteArray kViewEnd     = kAt + "view:end";
        static const QByteArray kViewExport  = kAt + "view:export";
        static const QByteArray kViewExclude = kAt + "view:exclude";
        static const QByteArray kSection     = kAt + "section";

        static const QByteArray kSepEq       = QByteArrayLiteral("//=====");
        static const QByteArray kSepDash     = QByteArrayLiteral("//-----");

        QByteArray result;
        QList<QByteArray> lines = input.split('\n');

        bool inBlock = false;

        for (const QByteArray& line : lines) {

            if (line.contains(kViewBeg)) { inBlock = true;  continue; }
            if (line.contains(kViewEnd)) { inBlock = false; continue; }

            int exportPos = line.indexOf(kViewExport);
            if (exportPos >= 0) {
                result.append(line.left(exportPos).trimmed());
                result.append('\n');
                continue;
            }

            if (!inBlock) continue;

            if (line.contains(kViewExclude)) continue;
            if (line.contains(kSection))     continue;
            if (line.startsWith(kSepEq))     continue;
            if (line.startsWith(kSepDash))   continue;

            result.append(line);
            result.append('\n');
        }

        return result;
    }
};

/// @view:end
