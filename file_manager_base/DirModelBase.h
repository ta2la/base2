#pragma once

#include "FileItemData.h"
#include "FileGroupModel.h"

#include <QAbstractListModel>
#include <QFileSystemWatcher>
#include <QDir>

//=============================================================================
class DirModelBase : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(QString dir     READ dir     NOTIFY dirChanged)
    Q_PROPERTY(QString repoName READ repoName NOTIFY dirChanged)
public:
    enum Roles { DataRole = Qt::UserRole + 1, GroupRole };

    DirModelBase(QObject* parent = nullptr);

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
    virtual FileItemData createItem_(const QFileInfo& fi);
    virtual void sortItems_(QList<FileItemData>& items);
    virtual void buildGroups_(const QList<FileItemData>& items);

protected:
    QList<FileGroupModel*>      groups_;

private:
    void refresh_();
    void clearGroups_();

    void resolveRepoName_();

    QString                     dir_;
    QString                     repoName_;
    QFileSystemWatcher          watcher_;
};
