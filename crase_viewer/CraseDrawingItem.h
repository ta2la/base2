#pragma once

#include <QMetaType>
#include <QString>

//=============================================================================
struct CraseDrawingItem {
    Q_GADGET
    Q_PROPERTY(QString icon   READ icon   CONSTANT)
    Q_PROPERTY(QString text   READ text   CONSTANT)
    Q_PROPERTY(int     itemId READ itemId CONSTANT)
    Q_PROPERTY(int     posX   READ posX   CONSTANT)
    Q_PROPERTY(int     posY   READ posY   CONSTANT)

public:
    CraseDrawingItem() = default;
    CraseDrawingItem(const QString& icon, const QString& text, int itemId, int x, int y)
        : icon_(icon), text_(text), itemId_(itemId), posX_(x), posY_(y) {}

    QString icon()   const { return icon_; }
    QString text()   const { return text_; }
    int     itemId() const { return itemId_; }
    int     posX()   const { return posX_; }
    int     posY()   const { return posY_; }

private:
    QString icon_;
    QString text_;
    int     itemId_ = 0;
    int     posX_   = 0;
    int     posY_   = 0;
};

Q_DECLARE_METATYPE(CraseDrawingItem)
