#pragma once

#include "CmdSys.h"
#include "DirModelBase.h"

#include <QDir>

//=============================================================================
class Cmds_file_manager_base {
public:
    static void registerSetDir(const QString& cmdName, DirModelBase& model) {
        inst_ = &model;
        CMD_SYS.add(cmdName, setDir_);
    }

private:
    static inline DirModelBase* inst_ = nullptr;

    static int setDir_(CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) {
        if (!inst_ || args.count() < 2) return -1;
        QString path = args.get(1).value().trimmed();
        QDir dir(path);
        if (!dir.exists()) return -1;
        inst_->setDir(dir.absolutePath());
        return 0;
    }
};
