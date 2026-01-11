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
#include <QStringList>
#include <QFileInfo>
#include <QSet>
#include <QTextStream>

/// @view:beg

class AnalyzerNode
{
public:
    /// @section Construction
    explicit AnalyzerNode(const QString& filePath, const QString& moduleName) :
        module_(moduleName)
    {
        dir_  = QFileInfo(filePath).absolutePath();
        name_ = nameFromFilePath(filePath);

        insertSuffix(filePath);
    }

    void insertSuffix(const QString& filePath)
    {
        QFileInfo fi(filePath);
        const QString suffix = fi.suffix().toLower();
        if (!suffix.isEmpty()) {
            extensions_.insert(suffix);
        }
    }

    /// @section Methods
    QString dir()    const { return dir_;  }
    QString name()   const { return name_; }
    QString module() const { return module_; }
    const QSet<QString>& extensions() const { return extensions_; }
    double distToCenter() const { return distToCenter_; }

    static QString nameFromFilePath(const QString& filePath)
    {
        QFileInfo fi(filePath);
        return fi.completeBaseName();
    }

    QString toStringDebug() const
    {
        if (extensions_.isEmpty()) {
            return name_;
        }

        QStringList exts = extensions_.values();
        exts.sort();

        return name_ + " [" + exts.join(" ") + "]";
    }

    QString label() const
    {
        QString base = name_;
        if (extensions_.isEmpty())
            return base;

        QStringList exts = extensions_.values();
        exts.sort();
        return base + "\\n[" + exts.join(" ") + "]";
    }

    QString LabelCmd() const
    {
        QString result;
        QTextStream s(&result);

        const QString filePath = dir_ + "/" + name_ + ".h";
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return label();
        QTextStream in(&file);

        const QString marker = QStringLiteral("CMD_SYS.add(\"");

        QStringList cmds;

        while (!in.atEnd()) {
            const QString line = in.readLine();

            int pos = line.indexOf(marker);
            if (pos < 0)
                continue;

            pos += marker.length();
            int end = line.indexOf('"', pos);
            if (end < 0)
                continue;

            cmds.append(line.mid(pos, end - pos));
        }

        s << label();
        s << "\\n-----------------------------";

        for (QString cmd : cmds) {
            s << "\\n" << cmd;
        }

        return result;
    }

    QStringList commands() const
    {
        QStringList result;

        // jen hlavičky
        if (!extensions_.contains("h"))
            return result;

        const QString path = dir_ + "/" + name_ + ".h";

        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return result;

        QTextStream in(&file);

        while (!in.atEnd()) {
            const QString line = in.readLine();

            int posCmdSys = line.indexOf("CMD_SYS");
            if (posCmdSys < 0)
                continue;

            int firstQuote = line.indexOf('"', posCmdSys);
            if (firstQuote < 0)
                continue;

            int secondQuote = line.indexOf('"', firstQuote + 1);
            if (secondQuote < 0)
                continue;

            const QString cmd =
                line.mid(firstQuote + 1, secondQuote - firstQuote - 1);

            if (!cmd.isEmpty())
                result.append(cmd);
        }

        return result;
    }

    void setDistToCenter(double v)
    {
        // normalizace: žádné záporné vzdálenosti
        //if (v < 0.0) v = 0.0;

        // NaN nebo ±inf → nekonečno
        //if (!std::isfinite(v)) v = std::numeric_limits<double>::infinity();

        distToCenter_ = v;
    }

    double distToCenter() { return distToCenter_; }

private:
    QString dir_;
    QString name_;
    QString module_;
    double  distToCenter_ = 1;

    QSet<QString> extensions_;

friend class AnalyzerNet;
};

/// @view:end
