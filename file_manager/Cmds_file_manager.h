#pragma once

#include "CmdSys.h"
#include "DirModel.h"
#include "PreviewModel.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>

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
        }, "file_manager");

        CMD_SYS.add("file_preview_path",
        []CMD_ARGS_U -> int {
            QString path = PreviewModel::inst().filePath();
            if (path.isEmpty()) return args.appendError("file_preview_path: no file previewed");
            args.append(path, "PATH");
            return 0;
        }, "file_manager");

        CMD_SYS.add("file_preview",
        []CMD_ARGS_U -> int {
            QString path = args.get(1).value().trimmed();
            if (path.isEmpty()) return args.appendError("file_preview: missing path");
            QFileInfo fi(path);
            if (!fi.exists()) return args.appendError("file_preview: not found: " + path);
            PreviewModel::inst().setFile(fi.absoluteFilePath());
            if (fi.isDir()) {
                DirModel::instPreview().setDir(fi.absoluteFilePath());
            }
            args.append(fi.absoluteFilePath(), "PREVIEW");
            return 0;
        }, "file_manager");

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
