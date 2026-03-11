#pragma once

#include "AppPaths.h"
#include "FmSettings.h"
#include "CmdSys.h"

#include <QDir>

//=============================================================================
class InitInMain {
//=============================================================================
public:
    InitInMain() = delete;

    static void init() {
        FmSettings::inst().setCodeRoot(AppPaths::inst().dirConfig());

        QString script = QDir::cleanPath(AppPaths::inst().dirConfig() + "/config.t2l");
        if (QFileInfo::exists(script))
            CMD_SYS.execute_threadSafe("execute_script " + script);
    }
};
