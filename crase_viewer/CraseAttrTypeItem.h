#pragma once

#include <QMetaType>
#include <QString>

//=============================================================================
struct CraseAttrTypeItem {
    Q_GADGET
    Q_PROPERTY(QString icon READ icon CONSTANT)
    Q_PROPERTY(QString type READ type CONSTANT)

public:
    CraseAttrTypeItem() = default;
    CraseAttrTypeItem(const QString& icon, const QString& type)
        : icon_(icon), type_(type) {}

    QString icon() const { return icon_; }
    QString type() const { return type_; }

private:
    QString icon_;
    QString type_;
};

Q_DECLARE_METATYPE(CraseAttrTypeItem)
