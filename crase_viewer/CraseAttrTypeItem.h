#pragma once

#include <QMetaType>
#include <QString>

//=============================================================================
struct CraseAttrTypeItem {
    Q_GADGET
    Q_PROPERTY(int     itemId READ itemId CONSTANT)
    Q_PROPERTY(QString icon   READ icon   CONSTANT)
    Q_PROPERTY(QString type   READ type   CONSTANT)

public:
    CraseAttrTypeItem() = default;
    CraseAttrTypeItem(int itemId, const QString& icon, const QString& type)
        : itemId_(itemId), icon_(icon), type_(type) {}

    int     itemId() const { return itemId_; }
    QString icon()   const { return icon_; }
    QString type()   const { return type_; }

private:
    int     itemId_ = 0;
    QString icon_;
    QString type_;
};

Q_DECLARE_METATYPE(CraseAttrTypeItem)
