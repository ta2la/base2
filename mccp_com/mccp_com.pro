TARGET       = mccp_com
TEMPLATE     = lib
CONFIG      += staticlib c++17
QT          += websockets
#######################################################################################
include($$PWD/../build_target.pri)

include(mccp_com.pri)

INCLUDEPATH += $$PWD/../base2
INCLUDEPATH += $$PWD/../cmd_sys
INCLUDEPATH += $$PWD/../utility
