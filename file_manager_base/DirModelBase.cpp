#include "DirModelBase.h"
#include "GitIgnoreMatch.h"
#include <QProcess>
#include <QSet>

//=============================================================================
DirModelBase::DirModelBase(QObject* parent)
    : QAbstractListModel(parent)
{
    connect(&watcher_, &QFileSystemWatcher::directoryChanged,
            this, [this]() { refresh_(); });
}

//=============================================================================
void DirModelBase::setDir(const QString& path)
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
void DirModelBase::resolveRepoName_()
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
void DirModelBase::refresh_()
{
    beginResetModel();

    QList<FileItemData> items;
    if (!dir_.isEmpty()) {
        GitIgnoreMatch gitIgnore(dir_);

        QSet<QString> gitTracked;
        QProcess git;
        git.setWorkingDirectory(dir_);
        git.start("git", {"ls-files", "."});
        if (git.waitForFinished(3000)) {
            QString output = QString::fromUtf8(git.readAllStandardOutput());
            for (const QString& line : output.split('\n', Qt::SkipEmptyParts)) {
                QString first = line.section('/', 0, 0);
                gitTracked.insert(first);
            }
        }

        QDir dir(dir_);
        for (const QFileInfo& fi : dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries | QDir::Hidden, QDir::Name)) {
            FileItemData item = createItem_(fi);
            if (gitIgnore.hasGitIgnore())
                item.setGitIgnored(gitIgnore.isIgnored(fi.fileName(), fi.isDir()));
            item.setInGit(gitTracked.contains(fi.fileName()));
            items.append(item);
        }
        sortItems_(items);
    }

    clearGroups_();
    buildGroups_(items);

    endResetModel();
}

//=============================================================================
void DirModelBase::clearGroups_()
{
    qDeleteAll(groups_);
    groups_.clear();
}

//=============================================================================
FileItemData DirModelBase::createItem_(const QFileInfo& fi)
{
    return FileItemData(fi);
}

//=============================================================================
void DirModelBase::sortItems_(QList<FileItemData>& items)
{
    std::sort(items.begin(), items.end(), [](const FileItemData& a, const FileItemData& b) {
        if (a.isDir() != b.isDir()) return a.isDir();
        return a.name().compare(b.name(), Qt::CaseInsensitive) < 0;
    });
}

//=============================================================================
void DirModelBase::buildGroups_(const QList<FileItemData>& items)
{
    for (const FileItemData& item : items) {
        auto* group = new FileGroupModel(this);
        group->append(item);
        groups_.append(group);
    }
}

//=============================================================================
int DirModelBase::rowCount(const QModelIndex&) const
{
    return groups_.size();
}

//=============================================================================
QVariant DirModelBase::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= groups_.size()) return {};
    if (role == DataRole) return QVariant::fromValue(groups_[index.row()]->get(0));
    if (role == GroupRole) return QVariant::fromValue(groups_[index.row()]);
    return {};
}

//=============================================================================
QHash<int, QByteArray> DirModelBase::roleNames() const
{
    return { { DataRole, "fileData" }, { GroupRole, "groupModel" } };
}
