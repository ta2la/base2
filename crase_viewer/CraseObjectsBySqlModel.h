#pragma once

#include "CraseObject.h"
#include "CraseSelection.h"
#include "SqlAccess.h"

#include <QAbstractListModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

//=============================================================================
class CraseObjectsBySqlModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles { DataRole = Qt::UserRole + 1 };

    CraseObjectsBySqlModel(QObject* parent = nullptr) : QAbstractListModel(parent) {}

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
        if (!q.exec(query)) {
            CMD_SYS.execute_threadSafe(QString("logcmd DBTEST query FAILED: %1").arg(q.lastError().text()));
            return;
        }

        beginResetModel();
        items_.clear();
        while (q.next()) {
            int id       = q.value(0).toInt();
            QString icon = q.value(1).toString();
            QString text = q.value(2).toString();
            // strip JSON quotes from value
            if (text.startsWith('"') && text.endsWith('"'))
                text = text.mid(1, text.length() - 2);
            items_.append(CraseObject(icon, QString("[%1] %2").arg(id).arg(text), id, CraseSelection::inst().contains(id)));
        }
        endResetModel();
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
        return { { DataRole, "craseObject" } };
    }

private:
    QList<CraseObject> items_;
};
