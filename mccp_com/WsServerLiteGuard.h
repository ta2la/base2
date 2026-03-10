#pragma once

#include "CmdSys.h"
#include "CmdExeGuard.h"
#include "WsServerLite.h"

//=============================================================================
class WsServerLiteGuard : public CmdExeGuard {
public:
    WsServerLiteGuard(WsServerLite* server)
        : CmdExeGuard(Update)
        , server_(server)
    {
        CMD_SYS.reg(this);
    }

    static WsServerLiteGuard& inst() {
        static WsServerLiteGuard i(&WsServerLite::init());
        return i;
    }

    void commandExecuted() override {
        QString output = CMD_SYS.executingArgs_.toString();
        QMetaObject::invokeMethod(server_, [server=server_, output]() {
            server->broadcast(output);
        }, Qt::QueuedConnection);
    }

private:
    WsServerLite* server_;
};
