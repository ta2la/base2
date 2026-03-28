#pragma once

#include "CraseDrawingItem.h"
#include "SqlAccess.h"

#include <QAbstractListModel>
#include <QSqlQuery>

//=============================================================================
class CraseDrawingModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles { DataRole = Qt::UserRole + 1 };

    CraseDrawingModel(QObject* parent = nullptr) : QAbstractListModel(parent) {}

    void loadDrawing(int drawingId) {
        if (!SqlAccess::inst().connect()) return;

        QSqlQuery q(SqlAccess::inst().db());
        q.prepare("SELECT di.id, t.icon, di.value::text, "
                  "(r.value->'xy'->0)::int, (r.value->'xy'->1)::int "
                  "FROM object_rels r "
                  "JOIN objects di ON di.id = r.id2 "
                  "JOIN objects_types t ON di.type = t.type "
                  "WHERE r.id1 = ?");
        q.addBindValue(drawingId);
        if (!q.exec()) return;

        beginResetModel();
        items_.clear();
        while (q.next()) {
            int id       = q.value(0).toInt();
            QString icon = q.value(1).toString();
            QString text = q.value(2).toString();
            int x        = q.value(3).toInt();
            int y        = q.value(4).toInt();
            if (text.startsWith('"') && text.endsWith('"'))
                text = text.mid(1, text.length() - 2);
            items_.append(CraseDrawingItem(icon, text, id, x, y));
        }
        endResetModel();
    }

    int rowCount(const QModelIndex& = QModelIndex()) const override { return items_.size(); }

    QVariant data(const QModelIndex& index, int role) const override {
        if (!index.isValid() || index.row() >= items_.size()) return {};
        if (role == DataRole) return QVariant::fromValue(items_[index.row()]);
        return {};
    }

    QHash<int, QByteArray> roleNames() const override {
        return { { DataRole, "drawItem" } };
    }

private:
    QList<CraseDrawingItem> items_;
};
