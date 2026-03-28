#pragma once

#include <QMetaType>
#include <QString>

//=============================================================================
struct CraseTypeItem {
    Q_GADGET
    Q_PROPERTY(QString icon       READ icon       CONSTANT)
    Q_PROPERTY(QString type       READ type       CONSTANT)
    Q_PROPERTY(QString attrIcons  READ attrIcons  CONSTANT)

public:
    CraseTypeItem() = default;
    CraseTypeItem(const QString& icon, const QString& type, const QString& attrIcons)
        : icon_(icon), type_(type), attrIcons_(attrIcons) {}

    QString icon()      const { return icon_; }
    QString type()      const { return type_; }
    QString attrIcons() const { return attrIcons_; }

private:
    QString icon_;
    QString type_;
    QString attrIcons_;
};

Q_DECLARE_METATYPE(CraseTypeItem)
