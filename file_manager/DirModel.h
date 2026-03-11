#pragma once

#include "FileItemData.h"

#include <QAbstractListModel>
#include <QFileSystemWatcher>
#include <QDir>

//=============================================================================
class DirModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(QString dir READ dir NOTIFY dirChanged)
public:
    enum Roles { DataRole = Qt::UserRole + 1 };

    DirModel(QObject* parent = nullptr);
    static DirModel& inst()        { static DirModel i; return i; }
    static DirModel& instPreview() { static DirModel i; return i; }

    void setDir(const QString& path);
    QString dir() const { return dir_; }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    const FileItemData& get(int index) const { return items_[index]; }
    int count() const { return items_.size(); }

signals:
    void dirChanged();

private:
    void refresh_();

    QString                dir_;
    QList<FileItemData>    items_;
    QFileSystemWatcher     watcher_;
};
