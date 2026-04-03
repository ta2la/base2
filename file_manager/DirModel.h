#pragma once

#include "DirModelBase.h"
#include "FmSettings.h"

#include <QDir>
#include <QFileInfo>

//=============================================================================
class DirModel : public DirModelBase {
    Q_OBJECT
public:
    enum Role { None = 0, CodeRoot, Repo, Module, ModulePro, CppFile, HFile, PriFile, ResourceDir, QrcFile, QmlFile, GitIgnoreFile };
    enum Depth { ROOT = 0, ROOT_PLUS1 = 1, ROOT_PLUS2 = 2, ROOT_PLUS3 = 3, ROOT_PLUS4 = 4 };

    DirModel(QObject* parent = nullptr) : DirModelBase(parent) {}
    static DirModel& inst()        { static DirModel i; return i; }
    static DirModel& instPreview() { static DirModel i; return i; }

protected:
    FileItemData createItem_(const QFileInfo& fi) override {
        FileItemData item(fi);
        item.setRole(resolveRole_(fi));
        return item;
    }

    void sortItems_(QList<FileItemData>& items) override {
        auto groupPriority = [](const FileItemData& f) -> int {
            QString suffix = QFileInfo(f.name()).suffix();
            if (suffix == "pro") return 0;
            if (suffix == "pri") return 1;
            if (f.role() == GitIgnoreFile) return 2;
            if (f.isDir() && f.name().startsWith("resource", Qt::CaseInsensitive)) return 3;
            if (suffix == "cpp" || suffix == "h") return 4;
            return 10;
        };

        auto suffixPriority = [](const QString& suffix) -> int {
            if (suffix == "h")   return 0;
            if (suffix == "cpp") return 1;
            if (suffix == "pri") return 2;
            return 10;
        };

        std::sort(items.begin(), items.end(), [&](const FileItemData& a, const FileItemData& b) {
            int ga = groupPriority(a), gb = groupPriority(b);
            if (ga != gb) return ga < gb;
            QString ba = QFileInfo(a.name()).completeBaseName();
            QString bb = QFileInfo(b.name()).completeBaseName();
            int cmp = ba.compare(bb, Qt::CaseInsensitive);
            if (cmp != 0) return cmp < 0;
            return suffixPriority(QFileInfo(a.name()).suffix())
                 < suffixPriority(QFileInfo(b.name()).suffix());
        });
    }

    void buildGroups_(const QList<FileItemData>& items) override {
        auto canGroup = [](int role) {
            return role == ModulePro || role == PriFile
                || role == CppFile   || role == HFile;
        };

        int i = 0;
        while (i < items.size()) {
            auto* group = new FileGroupModel(this);
            group->append(items[i]);
            if (canGroup(items[i].role())) {
                QString base = QFileInfo(items[i].name()).completeBaseName();
                while (i + 1 < items.size() && canGroup(items[i+1].role())
                       && QFileInfo(items[i+1].name()).completeBaseName().compare(base, Qt::CaseInsensitive) == 0) {
                    ++i;
                    group->append(items[i]);
                }
            }
            groups_.append(group);
            ++i;
        }
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
        if (fi.isDir() && depth == ROOT_PLUS3
            && fi.fileName().startsWith("Resource", Qt::CaseInsensitive))
            return ResourceDir;
        if (!fi.isDir() && fi.fileName() == ".gitignore") return GitIgnoreFile;
        if (!fi.isDir() && depth == ROOT_PLUS3) {
            QString suffix = fi.suffix();
            if (suffix == "cpp") return CppFile;
            if (suffix == "h")   return HFile;
            if (suffix == "pri") return PriFile;
        }
        if (!fi.isDir() && depth == ROOT_PLUS4) {
            QString suffix = fi.suffix();
            if (suffix == "qrc") return QrcFile;
            if (suffix == "qml") return QmlFile;
        }
        return None;
    }

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
};
