#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QString>

//=============================================================================
class CmdModel_controls : public QAbstractListModel
{
    Q_OBJECT
public:
    enum RowType { TextType = 0, EditType, ButtonType };
    Q_ENUM(RowType)

    enum ModelRoles {
        RowTypeRole = Qt::UserRole + 1,
        ContentRole,
        CommandRole
    };

    explicit CmdModel_controls(QObject* parent = nullptr) : QAbstractListModel(parent) {}
    virtual ~CmdModel_controls() = default;

    QString content() { return content_; }
    virtual void setContent(const QString& content) { Q_UNUSED(content) }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        Q_UNUSED(parent)
        return m_items.count();
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || index.row() < 0 || index.row() >= m_items.size())
            return QVariant();
        const Item& item = m_items.at(index.row());
        if (role == RowTypeRole) return item.rowType;
        if (role == ContentRole) return item.content;
        if (role == CommandRole) return item.command;
        return QVariant();
    }

    QHash<int, QByteArray> roleNames() const override {
        return { { RowTypeRole, "rowType" }, { ContentRole, "content" }, { CommandRole, "command" } };
    }

protected:
    struct Item { RowType rowType; QString content; QString command; };
    QList<Item> m_items;
    QString     content_;
};
