#include "DirModel.h"

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

    refresh_();
    emit dirChanged();
}

//=============================================================================
void DirModel::refresh_()
{
    beginResetModel();
    items_.clear();

    if (!dir_.isEmpty()) {
        QDir dir(dir_);
        for (const QFileInfo& fi : dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries, QDir::Name)) {
            items_.append(FileItemData(fi));
        }
        sortItems_(items_);
    }

    endResetModel();
}

//=============================================================================
void DirModel::sortItems_(QList<FileItemData>& items)
{
    auto groupPriority = [](const FileItemData& f) -> int {
        QString suffix = QFileInfo(f.name()).suffix();
        if (suffix == "pro") return 0;
        if (f.isDir() && f.name() == "resources") return 1;
        if (suffix == "cpp" || suffix == "h") return 2;
        if (suffix == "pri") return 3;
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
    return items_.size();
}

//=============================================================================
QVariant DirModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= items_.size()) return {};
    if (role == DataRole) return QVariant::fromValue(items_[index.row()]);
    return {};
}

//=============================================================================
QHash<int, QByteArray> DirModel::roleNames() const
{
    return { { DataRole, "fileData" } };
}
