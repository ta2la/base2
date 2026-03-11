#pragma once

#include <QCoreApplication>
#include <QDir>
#include <QString>

//=============================================================================
class AppPaths {
//=============================================================================
public:
    static AppPaths& inst() { static AppPaths i; return i; }

    QString dirExe() const { return QCoreApplication::applicationDirPath(); }
    QString dirConfig() const { return QDir::cleanPath(dirExe() + "/../config"); }

private:
    AppPaths() = default;
};
