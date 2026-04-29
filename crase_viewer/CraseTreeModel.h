#pragma once

#include "CraseTreeItem.h"
#include "CraseSelection.h"
#include "SqlAccess.h"

#include <QAbstractListModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSet>

//=============================================================================
class CraseTreeModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles { DataRole = Qt::UserRole + 1 };

    CraseTreeModel(QObject* parent = nullptr) : QAbstractListModel(parent) {}

    void reload(const QString& sql = "") {
        if (!SqlAccess::inst().connect()) return;

        QString cleaned = sql;
        if (cleaned.startsWith('{') && cleaned.endsWith('}'))
            cleaned = cleaned.mid(1, cleaned.length() - 2);

        QString query = cleaned.isEmpty()
            ? "SELECT o.id, t.icon, o.value::text FROM objects o "
              "JOIN objects_types t ON o.type = t.type "
              "ORDER BY o.root DESC, o.id"
            : cleaned;

        QSqlQuery q(SqlAccess::inst().db());
        if (!q.exec(query)) return;

        beginResetModel();
        items_.clear();
        while (q.next()) {
            int id       = q.value(0).toInt();
            QString icon = q.value(1).toString();
            QString text = q.value(2).toString();
            if (text.startsWith('"') && text.endsWith('"'))
                text = text.mid(1, text.length() - 2);
            items_.append(CraseTreeItem(icon, "", QString("[%1] %2").arg(id).arg(text), id, 0, CraseTreeItem::Object, CraseSelection::inst().contains(id)));
        }
        endResetModel();
    }

    void previewObject(int objectId) {
        if (!SqlAccess::inst().connect()) return;

        QSqlQuery q(SqlAccess::inst().db());
        q.prepare("SELECT o.id, t.icon, o.value::text FROM objects o "
                  "JOIN objects_types t ON o.type = t.type WHERE o.id = ?");
        q.addBindValue(objectId);
        if (!q.exec() || !q.next()) return;

        int id       = q.value(0).toInt();
        QString icon = q.value(1).toString();
        QString text = q.value(2).toString();
        if (text.startsWith('"') && text.endsWith('"'))
            text = text.mid(1, text.length() - 2);

        beginResetModel();
        items_.clear();
        items_.append(CraseTreeItem(icon, "", QString("[%1] %2").arg(id).arg(text), id, 0, CraseTreeItem::Object, CraseSelection::inst().contains(id)));
        endResetModel();

        expand(id);
    }

    void expand(int objectId) {
        if (!SqlAccess::inst().connect()) return;

        int pos = -1;
        int parentLevel = 0;
        for (int i = 0; i < items_.size(); i++) {
            if (items_[i].itemId() == objectId && items_[i].itemType() == CraseTreeItem::Object) {
                pos = i;
                parentLevel = items_[i].level();
                break;
            }
        }
        if (pos < 0) return;

        // check if already expanded — if next item has higher level, collapse
        if (pos + 1 < items_.size() && items_[pos + 1].level() > parentLevel) {
            collapse_(pos, parentLevel);
            return;
        }

        int insertLevel = parentLevel + 1;
        QList<CraseTreeItem> newItems;

        // collect ancestor ids to detect cycles
        QSet<int> ancestors;
        ancestors.insert(objectId);
        for (int i = pos - 1; i >= 0; i--) {
            if (items_[i].level() < parentLevel && items_[i].itemType() == CraseTreeItem::Object) {
                ancestors.insert(items_[i].itemId());
                if (items_[i].level() == 0) break;
            }
        }

        // attrs
        QSqlQuery qa(SqlAccess::inst().db());
        qa.prepare("SELECT at.icon, oa.value::text, oa.object_id "
                   "FROM attrs oa JOIN attr_types at ON oa.type = at.type "
                   "WHERE oa.object_id = ?");
        qa.addBindValue(objectId);
        if (qa.exec()) {
            while (qa.next()) {
                QString icon = qa.value(0).toString();
                QString text = qa.value(1).toString();
                int attrId   = qa.value(2).toInt();
                if (text.startsWith('"') && text.endsWith('"'))
                    text = text.mid(1, text.length() - 2);
                newItems.append(CraseTreeItem(icon, "", text, attrId, insertLevel, CraseTreeItem::Attr));
            }
        }

        // rels forward (this object is id1)
        QSqlQuery qr1(SqlAccess::inst().db());
        qr1.prepare("SELECT o.id, t.icon, o.value::text, r.name1 "
                    "FROM object_rels r "
                    "JOIN objects o ON o.id = r.id2 "
                    "JOIN objects_types t ON o.type = t.type "
                    "WHERE r.id1 = ?");
        qr1.addBindValue(objectId);
        if (qr1.exec()) {
            while (qr1.next()) {
                int relId    = qr1.value(0).toInt();
                QString icon = qr1.value(1).toString();
                QString text = qr1.value(2).toString();
                QString name = qr1.value(3).toString();
                if (text.startsWith('"') && text.endsWith('"'))
                    text = text.mid(1, text.length() - 2);
                CraseTreeItem::ItemType type = ancestors.contains(relId) ? CraseTreeItem::Rel : CraseTreeItem::Object;
                newItems.append(CraseTreeItem(icon, QString("%1: [%2]").arg(name).arg(relId), text,
                                              relId, insertLevel, type, CraseSelection::inst().contains(relId)));
            }
        }

        // rels reverse (this object is id2)
        QSqlQuery qr2(SqlAccess::inst().db());
        qr2.prepare("SELECT o.id, t.icon, o.value::text, r.name2 "
                    "FROM object_rels r "
                    "JOIN objects o ON o.id = r.id1 "
                    "JOIN objects_types t ON o.type = t.type "
                    "WHERE r.id2 = ?");
        qr2.addBindValue(objectId);
        if (qr2.exec()) {
            while (qr2.next()) {
                int relId    = qr2.value(0).toInt();
                QString icon = qr2.value(1).toString();
                QString text = qr2.value(2).toString();
                QString name = qr2.value(3).toString();
                if (text.startsWith('"') && text.endsWith('"'))
                    text = text.mid(1, text.length() - 2);
                CraseTreeItem::ItemType type = ancestors.contains(relId) ? CraseTreeItem::Rel : CraseTreeItem::Object;
                newItems.append(CraseTreeItem(icon, QString("%1: [%2]").arg(name).arg(relId), text,
                                              relId, insertLevel, type, CraseSelection::inst().contains(relId)));
            }
        }

        if (newItems.isEmpty()) return;

        beginInsertRows(QModelIndex(), pos + 1, pos + newItems.size());
        for (int i = 0; i < newItems.size(); i++)
            items_.insert(pos + 1 + i, newItems[i]);
        endInsertRows();
    }

    void refreshSelection() {
        for (int i = 0; i < items_.size(); i++)
            items_[i].setSelected(CraseSelection::inst().contains(items_[i].itemId()));
        if (!items_.isEmpty())
            emit dataChanged(index(0), index(items_.size() - 1));
    }

    int rowCount(const QModelIndex& = QModelIndex()) const override { return items_.size(); }

    QVariant data(const QModelIndex& index, int role) const override {
        if (!index.isValid() || index.row() >= items_.size()) return {};
        if (role == DataRole) return QVariant::fromValue(items_[index.row()]);
        return {};
    }

    QHash<int, QByteArray> roleNames() const override {
        return { { DataRole, "treeItem" } };
    }

private:
    void collapse_(int pos, int parentLevel) {
        int end = pos + 1;
        while (end < items_.size() && items_[end].level() > parentLevel)
            end++;
        if (end <= pos + 1) return;
        beginRemoveRows(QModelIndex(), pos + 1, end - 1);
        for (int i = end - 1; i > pos; i--)
            items_.removeAt(i);
        endRemoveRows();
    }

    QList<CraseTreeItem> items_;
};
