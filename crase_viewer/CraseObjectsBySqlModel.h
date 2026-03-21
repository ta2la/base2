#pragma once

#include "CraseObject.h"
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
        CMD_SYS.execute_threadSafe("logcmd DBTEST reload called");
        if (!SqlAccess::inst().connect()) return;

        QString query = sql.isEmpty()
            ? "SELECT o.id, t.icon, o.value::text FROM objects o "
              "JOIN objects_types t ON o.type = t.type "
              "ORDER BY o.id"
            : sql;

        QSqlQuery q(SqlAccess::inst().db());
        if (!q.exec(query)) {
            CMD_SYS.execute_threadSafe(QString("logcmd DBTEST query FAILED: %1").arg(q.lastError().text()));
            return;
        }

        CMD_SYS.execute_threadSafe("logcmd DBTEST query OK");
        beginResetModel();
        items_.clear();
        while (q.next()) {
            QString id   = q.value(0).toString();
            QString icon = q.value(1).toString();
            QString text = q.value(2).toString();
            // strip JSON quotes from value
            if (text.startsWith('"') && text.endsWith('"'))
                text = text.mid(1, text.length() - 2);
            items_.append(CraseObject(icon, QString("[%1] %2").arg(id, text)));
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
        return { { DataRole, "craseObject" } };
    }

private:
    QList<CraseObject> items_;
};
