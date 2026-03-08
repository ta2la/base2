#pragma once

#include <QMetaType>
#include <QString>
#include <QFileInfo>

//=============================================================================
struct FileItemData {
    Q_GADGET
    Q_PROPERTY(QString name     READ name     CONSTANT)
    Q_PROPERTY(QString filePath READ filePath CONSTANT)
    Q_PROPERTY(qint64  size     READ size     CONSTANT)
    Q_PROPERTY(bool    isDir    READ isDir    CONSTANT)

public:
    FileItemData() = default;
    FileItemData(const QFileInfo& fi)
        : name_(fi.fileName())
        , filePath_(fi.filePath())
        , size_(fi.size())
        , isDir_(fi.isDir())
    {}

    QString name()     const { return name_; }
    QString filePath() const { return filePath_; }
    qint64  size()     const { return size_; }
    bool    isDir()    const { return isDir_; }

private:
    QString name_;
    QString filePath_;
    qint64  size_    = 0;
    bool    isDir_   = false;
};

Q_DECLARE_METATYPE(FileItemData)
