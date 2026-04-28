#pragma once

#include "CraseAttrTypeItem.h"
#include "SqlAccess.h"

#include <QAbstractListModel>
#include <QSqlQuery>

//=============================================================================
class CraseAttrTypesModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles { DataRole = Qt::UserRole + 1 };

    CraseAttrTypesModel(QObject* parent = nullptr) : QAbstractListModel(parent) {}

    void load() {
        if (!SqlAccess::inst().connect()) return;

        QSqlQuery q(SqlAccess::inst().db());
        if (!q.exec("SELECT id, icon, type FROM attr_types ORDER BY id")) return;

        beginResetModel();
        items_.clear();
        while (q.next())
            items_.append(CraseAttrTypeItem(q.value(0).toInt(), q.value(1).toString(), q.value(2).toString()));
        endResetModel();
    }

    int rowCount(const QModelIndex& = QModelIndex()) const override { return items_.size(); }

    QVariant data(const QModelIndex& index, int role) const override {
        if (!index.isValid() || index.row() >= items_.size()) return {};
        if (role == DataRole) return QVariant::fromValue(items_[index.row()]);
        return {};
    }

    QHash<int, QByteArray> roleNames() const override {
        return { { DataRole, "attrTypeItem" } };
    }

private:
    QList<CraseAttrTypeItem> items_;
};
