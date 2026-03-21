#pragma once

#include <QMetaType>
#include <QString>

//=============================================================================
struct CraseObject {
    Q_GADGET
    Q_PROPERTY(QString icon READ icon CONSTANT)
    Q_PROPERTY(QString text READ text CONSTANT)

public:
    CraseObject() = default;
    CraseObject(const QString& icon, const QString& text)
        : icon_(icon), text_(text) {}

    QString icon() const { return icon_; }
    QString text() const { return text_; }

private:
    QString icon_;
    QString text_;
};

Q_DECLARE_METATYPE(CraseObject)
