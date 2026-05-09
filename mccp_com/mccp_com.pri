INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/CmdBridge.h

SOURCES += \
    $$PWD/mccp_com.cpp \
    $$PWD/CmdBridge.cpp

!wasm {
    HEADERS += \
        $$PWD/WsServerLite.h \
        $$PWD/WsServerLiteGuard.h
}
