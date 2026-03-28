#pragma once

#include <QObject>
#include <QString>

//=============================================================================
class AppStyle : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString barColor  READ barColor  CONSTANT)
    Q_PROPERTY(QString textColor READ textColor CONSTANT)
    Q_PROPERTY(QString icon      READ icon      CONSTANT)

public:
    static AppStyle& inst() { static AppStyle i; return i; }

    QString barColor()  const { return barColor_; }
    QString textColor() const { return textColor_; }
    QString icon()      const { return icon_; }

    void setBarColor(const QString& c)  { barColor_ = c; }
    void setTextColor(const QString& c) { textColor_ = c; }
    void setIcon(const QString& c)      { icon_ = c; }

private:
    AppStyle() = default;
    QString barColor_  = "#455C71";
    QString textColor_ = "#C0C8C0";
    QString icon_      = "\u2388";
};
