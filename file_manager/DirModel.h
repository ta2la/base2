#pragma once

#include "FileItemData.h"
#include "FileGroupModel.h"

#include <QAbstractListModel>
#include <QFileSystemWatcher>
#include <QDir>

//=============================================================================
class DirModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(QString dir     READ dir     NOTIFY dirChanged)
    Q_PROPERTY(QString repoName READ repoName NOTIFY dirChanged)
public:
    enum Roles { DataRole = Qt::UserRole + 1, GroupRole };

    DirModel(QObject* parent = nullptr);
    static DirModel& inst()        { static DirModel i; return i; }
    static DirModel& instPreview() { static DirModel i; return i; }

    void setDir(const QString& path);
    QString dir() const { return dir_; }
    QString repoName() const { return repoName_; }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    int count() const { return groups_.size(); }

signals:
    void dirChanged();

protected:
    static void sortItems_(QList<FileItemData>& items);

private:
    void refresh_();
    void clearGroups_();
    void buildGroups_(const QList<FileItemData>& items);

    void resolveRepoName_();

    QString                     dir_;
    QString                     repoName_;
    QList<FileGroupModel*>      groups_;
    QFileSystemWatcher          watcher_;
};
