#pragma once

#include <QMetaType>
#include <QString>

//=============================================================================
struct CraseRelTypeItem {
    Q_GADGET
    Q_PROPERTY(QString icon1 READ icon1 CONSTANT)
    Q_PROPERTY(QString name1 READ name1 CONSTANT)
    Q_PROPERTY(QString name2 READ name2 CONSTANT)
    Q_PROPERTY(QString icon2 READ icon2 CONSTANT)

public:
    CraseRelTypeItem() = default;
    CraseRelTypeItem(const QString& icon1, const QString& name1, const QString& name2, const QString& icon2)
        : icon1_(icon1), name1_(name1), name2_(name2), icon2_(icon2) {}

    QString icon1() const { return icon1_; }
    QString name1() const { return name1_; }
    QString name2() const { return name2_; }
    QString icon2() const { return icon2_; }

private:
    QString icon1_;
    QString name1_;
    QString name2_;
    QString icon2_;
};

Q_DECLARE_METATYPE(CraseRelTypeItem)
