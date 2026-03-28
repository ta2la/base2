#pragma once

#include <QMetaType>
#include <QString>

//=============================================================================
struct CraseTreeItem {
    Q_GADGET
    Q_PROPERTY(QString icon     READ icon     CONSTANT)
    Q_PROPERTY(QString prefix   READ prefix   CONSTANT)
    Q_PROPERTY(QString text     READ text     CONSTANT)
    Q_PROPERTY(int     itemId   READ itemId   CONSTANT)
    Q_PROPERTY(int     level    READ level    CONSTANT)
    Q_PROPERTY(int     itemType READ itemType CONSTANT)

public:
    enum ItemType { Object = 0, Attr = 1, Rel = 2 };

    CraseTreeItem() = default;
    CraseTreeItem(const QString& icon, const QString& prefix, const QString& text, int itemId, int level, ItemType itemType)
        : icon_(icon), prefix_(prefix), text_(text), itemId_(itemId), level_(level), itemType_(itemType) {}

    QString icon()     const { return icon_; }
    QString prefix()   const { return prefix_; }
    QString text()     const { return text_; }
    int     itemId()   const { return itemId_; }
    int     level()    const { return level_; }
    int     itemType() const { return itemType_; }

private:
    QString  icon_;
    QString  prefix_;
    QString  text_;
    int      itemId_   = 0;
    int      level_    = 0;
    ItemType itemType_ = Object;
};

Q_DECLARE_METATYPE(CraseTreeItem)
