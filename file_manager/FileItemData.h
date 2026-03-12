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
    enum Depth { ROOT = 0, ROOT_PLUS1 = 1, ROOT_PLUS2 = 2, ROOT_PLUS3 = 3 };
    enum Role { None = 0, CodeRoot, Repo, Module, ModulePro, CppFile, HFile, PriFile, Separator };

    FileItemData() = default;
    FileItemData(const QFileInfo& fi)
        : name_(fi.fileName())
        , filePath_(fi.filePath())
        , size_(fi.size())
        , isDir_(fi.isDir())
        , role_(resolveRole_(fi))
    {}

    static FileItemData separator() { FileItemData f; f.role_ = Separator; return f; }

    QString name()     const { return name_; }
    QString filePath() const { return filePath_; }
    qint64  size()     const { return size_; }
    bool    isDir()    const { return isDir_; }
    int     role()     const { return role_; }

protected:
    static int depthFromRoot_(const QFileInfo& fi) {
        const QString& codeRoot = FmSettings::inst().codeRoot();
        if (codeRoot.isEmpty()) return -1;
        QString root = QDir::cleanPath(codeRoot);
        QString path = QDir::cleanPath(fi.absoluteFilePath());
        if (!path.startsWith(root)) return -1;
        QString rel = path.mid(root.length());
        if (rel.startsWith('/')) rel = rel.mid(1);
        if (rel.isEmpty()) return 0;
        return rel.count('/') + 1;
    }

private:
    static Role resolveRole_(const QFileInfo& fi) {
        int depth = depthFromRoot_(fi);
        if (depth == ROOT) return CodeRoot;
        if (fi.isDir() && depth == ROOT_PLUS1
            && QFileInfo::exists(fi.absoluteFilePath() + "/.git"))
            return Repo;
        if (fi.isDir() && depth == ROOT_PLUS2
            && QFileInfo::exists(fi.absoluteFilePath() + "/" + fi.fileName() + ".pro"))
            return Module;
        if (depth == ROOT_PLUS3 && fi.suffix() == "pro"
            && fi.completeBaseName() == QFileInfo(fi.absolutePath()).fileName())
            return ModulePro;
        if (!fi.isDir() && depth == ROOT_PLUS3) {
            QString suffix = fi.suffix();
            if (suffix == "cpp") return CppFile;
            if (suffix == "h")   return HFile;
            if (suffix == "pri") return PriFile;
        }
        return None;
    }

    QString name_;
    QString filePath_;
    qint64  size_    = 0;
    bool    isDir_   = false;
    Role    role_    = None;
};

Q_DECLARE_METATYPE(FileItemData)
