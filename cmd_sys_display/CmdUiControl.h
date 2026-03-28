#pragma once

#include "CmdModel_controls.h"

#include <CmdSys.h>
#include <CmdExeGuard.h>

#include <QObject>

//=============================================================================
class CmdUiControl : public QObject, public CmdExeGuard {
    Q_OBJECT
public:
    CmdUiControl(CmdModel_controls& controls, QObject* parent = nullptr)
        : QObject(parent)
        , CmdExeGuard(Update)
        , controls_(controls)
    {
        CMD_SYS.reg(this);
    }

    void     setRootObject(QObject* object) { rootObject_ = object; }
    QObject* rootObject()                   { return rootObject_; }

    void commandExecuted() override {
        controls_.setContent(controls_.content());
        if (rootObject_ == nullptr) return;
        QMetaObject::invokeMethod(rootObject_, "update", Qt::QueuedConnection);
    }

protected:
    CmdModel_controls& controls_;
    QObject* rootObject_ = nullptr;

public slots:
    void callCmd(const QString& cmd) {
        CMD_SYS.execute_threadSafe(cmd);
    }
};
