#pragma once

#include <QObject>
#include <QHash>
#include <QPair>
#include <QString>
#include <QVariantList>
#include <QVariantMap>

//=============================================================================
class CraseSelection : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList items READ items NOTIFY changed)
    Q_PROPERTY(int          count READ count NOTIFY changed)
public:
    static CraseSelection& inst() { static CraseSelection s; return s; }

    Q_INVOKABLE bool contains(int id) const { return items_.contains(id); }
    int count() const { return items_.size(); }

    void toggle(int id, const QString& icon, const QString& text) {
        if (items_.contains(id)) items_.remove(id);
        else                     items_.insert(id, qMakePair(icon, text));
        emit changed();
    }
    void clear() {
        if (items_.isEmpty()) return;
        items_.clear();
        emit changed();
    }

    QVariantList items() const {
        QVariantList list;
        for (auto it = items_.begin(); it != items_.end(); ++it) {
            QVariantMap m;
            m["id"]   = it.key();
            m["icon"] = it.value().first;
            m["text"] = it.value().second;
            list.append(m);
        }
        return list;
    }

signals:
    void changed();

private:
    CraseSelection(QObject* p = nullptr) : QObject(p) {}
    QHash<int, QPair<QString, QString>> items_;
};
