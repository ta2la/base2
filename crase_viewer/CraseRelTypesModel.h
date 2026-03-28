#pragma once

#include "CraseRelTypeItem.h"
#include "SqlAccess.h"

#include <QAbstractListModel>
#include <QSqlQuery>

//=============================================================================
class CraseRelTypesModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles { DataRole = Qt::UserRole + 1 };

    CraseRelTypesModel(QObject* parent = nullptr) : QAbstractListModel(parent) {}

    void load() {
        if (!SqlAccess::inst().connect()) return;

        QSqlQuery q(SqlAccess::inst().db());
        if (!q.exec("SELECT t1.icon, rt.name1, rt.name2, t2.icon "
                     "FROM object_rels_types rt "
                     "JOIN objects_types t1 ON rt.object_type1 = t1.type "
                     "JOIN objects_types t2 ON rt.object_type2 = t2.type "
                     "ORDER BY rt.id")) return;

        beginResetModel();
        items_.clear();
        while (q.next())
            items_.append(CraseRelTypeItem(q.value(0).toString(), q.value(1).toString(),
                                           q.value(2).toString(), q.value(3).toString()));
        endResetModel();
    }

    int rowCount(const QModelIndex& = QModelIndex()) const override { return items_.size(); }

    QVariant data(const QModelIndex& index, int role) const override {
        if (!index.isValid() || index.row() >= items_.size()) return {};
        if (role == DataRole) return QVariant::fromValue(items_[index.row()]);
        return {};
    }

    QHash<int, QByteArray> roleNames() const override {
        return { { DataRole, "relTypeItem" } };
    }

private:
    QList<CraseRelTypeItem> items_;
};
