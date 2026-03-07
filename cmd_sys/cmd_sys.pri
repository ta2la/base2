INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
$$PWD/Cmd.h \
$$PWD/CmdContext.h \
$$PWD/CmdContextIface.h \
$$PWD/CmdExeGuard.h \
$$PWD/CmdSys.h \
$$PWD/Cmds_cmd_sys.h \
$$PWD/CmdContextDataIface.h \
$$PWD/StdinMonitor.h \
$$PWD/CmdUtils.h \
$$PWD/StdoutCmdOutput.h

SOURCES += \
$$PWD/CmdSys.cpp \
$$PWD/Cmds_cmd_sys.cpp \
$$PWD/StdoutCmdOutput.cpp
