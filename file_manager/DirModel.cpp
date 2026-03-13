#include "DirModel.h"
#include "GitIgnoreMatch.h"

//=============================================================================
DirModel::DirModel(QObject* parent)
    : QAbstractListModel(parent)
{
    connect(&watcher_, &QFileSystemWatcher::directoryChanged,
            this, [this]() { refresh_(); });
}

//=============================================================================
void DirModel::setDir(const QString& path)
{
    if (dir_ == path) return;

    if (!dir_.isEmpty()) {
        watcher_.removePath(dir_);
    }

    dir_ = path;

    if (!dir_.isEmpty()) {
        watcher_.addPath(dir_);
    }

    resolveRepoName_();
    refresh_();
    emit dirChanged();
}

//=============================================================================
void DirModel::resolveRepoName_()
{
    repoName_.clear();
    QDir dir(dir_);
    while (!dir.isRoot()) {
        if (QFileInfo::exists(dir.absolutePath() + "/.git")) {
            repoName_ = dir.dirName();
            return;
        }
        if (!dir.cdUp()) break;
    }
}

//=============================================================================
void DirModel::refresh_()
{
    beginResetModel();

    QList<FileItemData> items;
    if (!dir_.isEmpty()) {
        GitIgnoreMatch gitIgnore(dir_);
        QDir dir(dir_);
        for (const QFileInfo& fi : dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries, QDir::Name)) {
            FileItemData item(fi);
            if (gitIgnore.hasGitIgnore())
                item.setGitIgnored(gitIgnore.isIgnored(fi.fileName(), fi.isDir()));
            items.append(item);
        }
        sortItems_(items);
    }

    clearGroups_();
    buildGroups_(items);

    endResetModel();
}

//=============================================================================
void DirModel::clearGroups_()
{
    qDeleteAll(groups_);
    groups_.clear();
}

//=============================================================================
void DirModel::buildGroups_(const QList<FileItemData>& items)
{
    auto canGroup = [](int role) {
        return role == FileItemData::ModulePro || role == FileItemData::PriFile
            || role == FileItemData::CppFile   || role == FileItemData::HFile;
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

//=============================================================================
void DirModel::sortItems_(QList<FileItemData>& items)
{
    auto groupPriority = [](const FileItemData& f) -> int {
        QString suffix = QFileInfo(f.name()).suffix();
        if (suffix == "pro") return 0;
        if (suffix == "pri") return 1;
        if (f.isDir() && f.name().startsWith("resource", Qt::CaseInsensitive)) return 2;
        if (suffix == "cpp" || suffix == "h") return 3;
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

//=============================================================================
int DirModel::rowCount(const QModelIndex&) const
{
    return groups_.size();
}

//=============================================================================
QVariant DirModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= groups_.size()) return {};
    if (role == DataRole) return QVariant::fromValue(groups_[index.row()]->get(0));
    if (role == GroupRole) return QVariant::fromValue(groups_[index.row()]);
    return {};
}

//=============================================================================
QHash<int, QByteArray> DirModel::roleNames() const
{
    return { { DataRole, "fileData" }, { GroupRole, "groupModel" } };
}
