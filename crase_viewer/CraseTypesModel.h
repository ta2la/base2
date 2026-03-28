#pragma once

#include "CraseTypeItem.h"
#include "SqlAccess.h"

#include <QAbstractListModel>
#include <QSqlQuery>

//=============================================================================
class CraseTypesModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles { DataRole = Qt::UserRole + 1 };

    CraseTypesModel(QObject* parent = nullptr) : QAbstractListModel(parent) {}

    void load() {
        if (!SqlAccess::inst().connect()) return;

        // load attr icons by id
        QMap<int, QString> attrIconMap;
        QSqlQuery qa(SqlAccess::inst().db());
        if (qa.exec("SELECT id, icon FROM attr_types")) {
            while (qa.next())
                attrIconMap[qa.value(0).toInt()] = qa.value(1).toString();
        }

        QSqlQuery q(SqlAccess::inst().db());
        if (!q.exec("SELECT icon, type, attributes FROM objects_types ORDER BY id")) return;

        beginResetModel();
        items_.clear();
        while (q.next()) {
            QString icon  = q.value(0).toString();
            QString type  = q.value(1).toString();
            QString attrs = q.value(2).toString();

            QString attrIcons;
            if (!attrs.isEmpty()) {
                for (const QString& idStr : attrs.split(',', Qt::SkipEmptyParts)) {
                    int id = idStr.trimmed().toInt();
                    if (attrIconMap.contains(id))
                        attrIcons += attrIconMap[id] + " ";
                }
                attrIcons = attrIcons.trimmed();
            }

            items_.append(CraseTypeItem(icon, type, attrIcons));
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
        return { { DataRole, "typeItem" } };
    }

private:
    QList<CraseTypeItem> items_;
};
