#pragma once

#include "CmdSys.h"
#include "DirModel.h"

#include <QCoreApplication>
#include <QDir>

//=============================================================================
class Cmds_file_manager {
//=============================================================================
public:
    Cmds_file_manager() = delete;
    static bool registerCmds()
    {
        CMD_SYS.add("set_dir",
        []CMD_ARGS_U -> int {
            QString path = args.get(1).value().trimmed();
            if (path.isEmpty()) return args.appendError("set_dir: missing path");
            QDir dir(path);
            if (!dir.exists()) return args.appendError("set_dir: directory not found: " + path);
            DirModel::inst().setDir(dir.absolutePath());
            args.append(dir.absolutePath(), "DIR");
            return 0;
        });

        return true;
    }

    static void initDefaultDir()
    {
        QString appDir = QCoreApplication::applicationDirPath();
        QDir dir(appDir);
        while (dir.cdUp()) {
            if (dir.dirName() == "cvz") {
                DirModel::inst().setDir(dir.absolutePath());
                return;
            }
        }
    }
};
