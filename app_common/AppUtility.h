#pragma once

#include "AppPaths.h"

#include <QDir>
#include <QFile>
#include <QString>
#include <QStringList>

//=============================================================================
class AppUtility {
//=============================================================================
public:
    AppUtility() = delete;

    static bool replaceOrAppendInScript(QStringList& lines, const QString& line) {
        QString cmdName = line.split(' ').first();
        for (int i = 0; i < lines.size(); ++i) {
            if (lines[i].trimmed().startsWith(cmdName)) {
                lines[i] = line + "\n";
                return true;
            }
        }
        lines.append(line + "\n");
        return false;
    }

    static int replaceOrAppendInConfig(const QString& line, QString& error) {
        QString path = QDir::cleanPath(AppPaths::inst().dirConfig() + "/config.t2l");

        QStringList lines;
        QFile file(path);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            while (!file.atEnd()) lines.append(QString::fromUtf8(file.readLine()));
            file.close();
        }

        replaceOrAppendInScript(lines, line);

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            error = "cannot write " + path;
            return -1;
        }
        for (const QString& l : lines) file.write(l.toUtf8());
        file.close();
        return 0;
    }
};
