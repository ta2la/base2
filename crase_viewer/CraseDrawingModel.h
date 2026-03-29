#pragma once

#include "CraseDrawingItem.h"
#include "SqlAccess.h"

#include <QAbstractListModel>
#include <QSqlQuery>
#include <QVariantList>
#include <QVariantMap>
#include <QMap>

//=============================================================================
class CraseDrawingModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(QVariantList lines READ lines NOTIFY linesChanged)
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
        QMap<int, int> idToIndex;

        while (q.next()) {
            int id       = q.value(0).toInt();
            QString icon = q.value(1).toString();
            QString text = q.value(2).toString();
            int x        = q.value(3).toInt();
            int y        = q.value(4).toInt();
            if (text.startsWith('"') && text.endsWith('"'))
                text = text.mid(1, text.length() - 2);
            idToIndex[id] = items_.size();
            items_.append(CraseDrawingItem(icon, text, id, x, y));
        }
        endResetModel();

        // load inter-item relations
        lines_.clear();
        if (items_.size() < 2) { emit linesChanged(); return; }

        QStringList ids;
        for (const auto& item : items_) ids.append(QString::number(item.itemId()));
        QString idList = ids.join(',');

        QSqlQuery qr(SqlAccess::inst().db());
        if (!qr.exec(QString(
            "SELECT r.id1, r.name1, r.name2, r.id2 "
            "FROM object_rels r "
            "WHERE r.id1 IN (%1) AND r.id2 IN (%1) "
            "AND r.id1 != %2 AND r.id2 != %2")
            .arg(idList).arg(drawingId))) {
            emit linesChanged();
            return;
        }

        while (qr.next()) {
            int id1 = qr.value(0).toInt();
            QString name1 = qr.value(1).toString();
            QString name2 = qr.value(2).toString();
            int id2 = qr.value(3).toInt();

            if (!idToIndex.contains(id1) || !idToIndex.contains(id2)) continue;

            const auto& from = items_[idToIndex[id1]];
            const auto& to   = items_[idToIndex[id2]];

            QVariantMap line;
            line["fromX"] = from.posX() + 40;
            line["fromY"] = from.posY() + 14;
            line["toX"]   = to.posX() + 40;
            line["toY"]   = to.posY() + 14;
            line["name1"] = name1;
            line["name2"] = name2;
            lines_.append(line);
        }
        emit linesChanged();
    }

    QVariantList lines() const { return lines_; }

    int rowCount(const QModelIndex& = QModelIndex()) const override { return items_.size(); }

    QVariant data(const QModelIndex& index, int role) const override {
        if (!index.isValid() || index.row() >= items_.size()) return {};
        if (role == DataRole) return QVariant::fromValue(items_[index.row()]);
        return {};
    }

    QHash<int, QByteArray> roleNames() const override {
        return { { DataRole, "drawItem" } };
    }

signals:
    void linesChanged();

private:
    QList<CraseDrawingItem> items_;
    QVariantList lines_;
};
