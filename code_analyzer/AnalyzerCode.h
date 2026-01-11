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
 * @class AnalyzerCode
 * @brief Static methods for file manipulation
 */
#pragma once

#include "CmdSys.h"
//#include "Cmds_code_analyzer.h"

#include <QStringList>
#include <QDir>
#include <QDirIterator>
#include <QTextStream>

/// @view:beg

class  AnalyzerCode
{
public:
    //! @section Construction    /// @view:exclude
    AnalyzerCode() = delete; /// @view:exclude
    //! @section Methods         /// @view:exclude
    //=========================================================================
    static QStringList getFiles(QDir& dir, QStringList filters) {
        QDirIterator it(
            dir.absolutePath(),
            filters,
            QDir::Files | QDir::NoSymLinks,
            QDirIterator::Subdirectories
            );

        QStringList files;

        while (it.hasNext()) files << it.next();

        return files;
    }

    //=========================================================================
    static QString resultFile(const QString& dirPath)
    {
        QDir dir(dirPath);

        QString dirName = dir.dirName();
        if (dirName.isEmpty()) dirName = "mismatched.h";

        return dir.absoluteFilePath(dirName + ".h");
    }

    //=========================================================================
    static QByteArray extractView(const QByteArray& input)
    {
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

    //=========================================================================
    static void composeToFile (
        const QStringList& files, QFile& outFile,
        bool useViews = false )
    {
        for (const QString& path : files) {
            QFile f(path);
            if (!f.open(QIODevice::ReadOnly)) {
                CMD_SYS.execute( QString() +
                                "logcmd --ERROR AnalyzerCode::composeToFile cannot open " +
                                path );
                continue;
            }

            QByteArray content = f.readAll();
            f.close();

            if (useViews)
                content = extractView(content);

            if (content.isEmpty())
                continue;

            QFileInfo fi(path);
            const QString fileName = fi.fileName();
            const QString module   = fi.dir().dirName();

            outFile.write("\n// ---- module:");
            outFile.write(module.toUtf8());
            outFile.write("     ");
            outFile.write(fileName.toUtf8());
            outFile.write(" ----\n\n");
            // ----------------------

            outFile.write(content);
            outFile.write("\n");
        }
    }

    //=============================================================================
    static QStringList extractIncludes(const QString& filePath)
    {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return {};

        const bool isQml = filePath.endsWith(".qml", Qt::CaseInsensitive);

        QStringList result;
        QTextStream in(&file);

        while (!in.atEnd()) {
            const QString line = in.readLine().trimmed();

            if (isQml) {
                // QML: include může být kdekoliv (typicky v komentáři)
                if (!line.contains("#include"))
                    continue;
            } else {
                // C/C++: musí začínat na #include (po trimu)
                if (!line.trimmed().startsWith("#include"))
                    continue;
            }

            int firstQuote = line.indexOf('"');
            if (firstQuote < 0)
                continue;

            int secondQuote = line.indexOf('"', firstQuote + 1);
            if (secondQuote < 0)
                continue;

            const QString includeFile = line.mid(firstQuote + 1, secondQuote - firstQuote - 1);

            if (!includeFile.isEmpty()) result.append(includeFile);
        }

        return result;
    }



    //=============================================================================
    static void loadDot();

    //=============================================================================
protected:
    //! @section Data
    int arg_;
};

/// @view:end
