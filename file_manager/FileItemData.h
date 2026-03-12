#pragma once

#include "FmSettings.h"

#include <QMetaType>
#include <QString>
#include <QFileInfo>
#include <QDir>

//=============================================================================
struct FileItemData {
    Q_GADGET
    Q_PROPERTY(QString name     READ name     CONSTANT)
    Q_PROPERTY(QString filePath READ filePath CONSTANT)
    Q_PROPERTY(qint64  size     READ size     CONSTANT)
    Q_PROPERTY(bool    isDir    READ isDir    CONSTANT)
    Q_PROPERTY(int     role     READ role     CONSTANT)

public:
    enum Role { None = 0, CodeRoot, Module };

    FileItemData() = default;
    FileItemData(const QFileInfo& fi)
        : name_(fi.fileName())
        , filePath_(fi.filePath())
        , size_(fi.size())
        , isDir_(fi.isDir())
        , role_(resolveRole_(fi))
    {}

    QString name()     const { return name_; }
    QString filePath() const { return filePath_; }
    qint64  size()     const { return size_; }
    bool    isDir()    const { return isDir_; }
    int     role()     const { return role_; }

private:
    static Role resolveRole_(const QFileInfo& fi) {
        const QString& codeRoot = FmSettings::inst().codeRoot();
        if (!codeRoot.isEmpty() && QDir::cleanPath(fi.absoluteFilePath()) == QDir::cleanPath(codeRoot))
            return CodeRoot;
        return None;
    }

    QString name_;
    QString filePath_;
    qint64  size_    = 0;
    bool    isDir_   = false;
    Role    role_    = None;
};

Q_DECLARE_METATYPE(FileItemData)
