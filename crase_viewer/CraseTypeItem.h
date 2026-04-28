#pragma once

#include <QMetaType>
#include <QString>

//=============================================================================
struct CraseTypeItem {
    Q_GADGET
    Q_PROPERTY(int     itemId     READ itemId     CONSTANT)
    Q_PROPERTY(QString icon       READ icon       CONSTANT)
    Q_PROPERTY(QString type       READ type       CONSTANT)
    Q_PROPERTY(QString attrIcons  READ attrIcons  CONSTANT)

public:
    CraseTypeItem() = default;
    CraseTypeItem(int itemId, const QString& icon, const QString& type, const QString& attrIcons)
        : itemId_(itemId), icon_(icon), type_(type), attrIcons_(attrIcons) {}

    int     itemId()    const { return itemId_; }
    QString icon()      const { return icon_; }
    QString type()      const { return type_; }
    QString attrIcons() const { return attrIcons_; }

private:
    int     itemId_ = 0;
    QString icon_;
    QString type_;
    QString attrIcons_;
};

Q_DECLARE_METATYPE(CraseTypeItem)
