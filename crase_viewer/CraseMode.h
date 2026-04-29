#pragma once

#include <QObject>
#include <QString>

//=============================================================================
class CraseMode : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString cmd READ cmd NOTIFY cmdChanged)
public:
    static CraseMode& inst() { static CraseMode m; return m; }

    QString cmd() const { return cmd_; }
    void setCmd(const QString& c) {
        if (cmd_ != c) {
            cmd_ = c;
            emit cmdChanged();
        }
    }

signals:
    void cmdChanged();

private:
    CraseMode(QObject* p = nullptr) : QObject(p) {}
    QString cmd_ = "voidcmd";
};
