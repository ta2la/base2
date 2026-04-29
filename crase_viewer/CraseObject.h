#pragma once

#include <QMetaType>
#include <QString>

//=============================================================================
struct CraseObject {
    Q_GADGET
    Q_PROPERTY(QString icon     READ icon     CONSTANT)
    Q_PROPERTY(QString text     READ text     CONSTANT)
    Q_PROPERTY(int     itemId   READ itemId   CONSTANT)
    Q_PROPERTY(bool    selected READ selected CONSTANT)

public:
    CraseObject() = default;
    CraseObject(const QString& icon, const QString& text, int itemId = 0, bool selected = false)
        : icon_(icon), text_(text), itemId_(itemId), selected_(selected) {}

    QString icon()     const { return icon_; }
    QString text()     const { return text_; }
    int     itemId()   const { return itemId_; }
    bool    selected() const { return selected_; }

    void    setSelected(bool s) { selected_ = s; }

private:
    QString icon_;
    QString text_;
    int     itemId_   = 0;
    bool    selected_ = false;
};

Q_DECLARE_METATYPE(CraseObject)
