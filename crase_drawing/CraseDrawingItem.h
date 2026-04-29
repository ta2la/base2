#pragma once

#include <QMetaType>
#include <QString>

//=============================================================================
struct CraseDrawingItem {
    Q_GADGET
    Q_PROPERTY(QString icon            READ icon            CONSTANT)
    Q_PROPERTY(QString text            READ text            CONSTANT)
    Q_PROPERTY(int     itemId          READ itemId          CONSTANT)
    Q_PROPERTY(int     relTypeId       READ relTypeId       CONSTANT)
    Q_PROPERTY(int     posX            READ posX            CONSTANT)
    Q_PROPERTY(int     posY            READ posY            CONSTANT)
    Q_PROPERTY(int     posW            READ posW            CONSTANT)
    Q_PROPERTY(int     posH            READ posH            CONSTANT)
    Q_PROPERTY(int     hiddenChildren     READ hiddenChildren     CONSTANT)
    Q_PROPERTY(qreal   hideThreshold      READ hideThreshold      CONSTANT)
    Q_PROPERTY(qreal   containerThreshold READ containerThreshold CONSTANT)

public:
    CraseDrawingItem() = default;
    CraseDrawingItem(const QString& icon, const QString& text, int itemId, int relTypeId,
                     int x, int y, int w, int h,
                     int hiddenChildren = 0, qreal hideThreshold = 0, qreal containerThreshold = 0)
        : icon_(icon), text_(text), itemId_(itemId), relTypeId_(relTypeId),
          posX_(x), posY_(y), posW_(w), posH_(h),
          hiddenChildren_(hiddenChildren), hideThreshold_(hideThreshold),
          containerThreshold_(containerThreshold) {}

    QString icon()           const { return icon_; }
    QString text()           const { return text_; }
    int     itemId()         const { return itemId_; }
    int     relTypeId()      const { return relTypeId_; }
    int     posX()           const { return posX_; }
    int     posY()           const { return posY_; }
    int     posW()           const { return posW_; }
    int     posH()           const { return posH_; }
    int     hiddenChildren()     const { return hiddenChildren_; }
    qreal   hideThreshold()      const { return hideThreshold_; }
    qreal   containerThreshold() const { return containerThreshold_; }

private:
    QString icon_;
    QString text_;
    int     itemId_         = 0;
    int     relTypeId_      = 0;
    int     posX_           = 0;
    int     posY_           = 0;
    int     posW_           = 0;
    int     posH_           = 0;
    int     hiddenChildren_     = 0;
    qreal   hideThreshold_      = 0;
    qreal   containerThreshold_ = 0;
};

Q_DECLARE_METATYPE(CraseDrawingItem)
