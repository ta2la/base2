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
    Q_PROPERTY(bool    gitIgnored READ gitIgnored CONSTANT)
    Q_PROPERTY(bool    inGit      READ inGit      CONSTANT)
    Q_PROPERTY(int     role     READ role     CONSTANT)
    Q_PROPERTY(int     level    READ level    CONSTANT)

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
    bool    gitIgnored() const { return gitIgnored_; }
    bool    inGit()      const { return inGit_; }
    int     role()     const { return role_; }
    int     level()    const { return level_; }

    void setGitIgnored(bool v) { gitIgnored_ = v; }
    void setInGit(bool v)      { inGit_ = v; }
    void setRole(int r)        { role_ = r; }
    void setLevel(int l)       { level_ = l; }

protected:
    QString name_;
    QString filePath_;
    qint64  size_       = 0;
    bool    isDir_      = false;
    bool    gitIgnored_ = true;
    bool    inGit_      = false;
    int     role_       = 0;
    int     level_      = 0;
};

Q_DECLARE_METATYPE(FileItemData)
