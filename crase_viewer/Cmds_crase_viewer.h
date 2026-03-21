#pragma once

#include "CmdSys.h"
#include "CraseObjectsBySqlModel.h"

//=============================================================================
class Cmds_crase_viewer {
public:
    static void setModel(CraseObjectsBySqlModel* m) { model_() = m; }

    static void registerCmds() {
        CMD_SYS.add("reload_view",
        []CMD_ARGS_U -> int {
            if (!model_()) return args.appendError("reload_view: no model");
            model_()->reload();
            return 0;
        }, "crase_viewer");
    }

private:
    static CraseObjectsBySqlModel*& model_() { static CraseObjectsBySqlModel* m = nullptr; return m; }
};
