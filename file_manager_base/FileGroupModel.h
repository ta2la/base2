#pragma once

#include "FileItemData.h"

#include <QAbstractListModel>

//=============================================================================
class FileGroupModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles { DataRole = Qt::UserRole + 1 };

    FileGroupModel(QObject* parent = nullptr) : QAbstractListModel(parent) {}

    void append(const FileItemData& item) { items_.append(item); }
    int  count() const { return items_.size(); }
    const FileItemData& get(int index) const { return items_[index]; }

    int rowCount(const QModelIndex& = QModelIndex()) const override { return items_.size(); }

    QVariant data(const QModelIndex& index, int role) const override {
        if (!index.isValid() || index.row() >= items_.size()) return {};
        if (role == DataRole) return QVariant::fromValue(items_[index.row()]);
        return {};
    }

    QHash<int, QByteArray> roleNames() const override {
        return { { DataRole, "fileData" } };
    }

private:
    QList<FileItemData> items_;
};
