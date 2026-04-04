TARGET       = cmd_sys
TEMPLATE     = lib
CONFIG      += staticlib c++17
#######################################################################################
include($$PWD/cmd_sys.pri)

include($$PWD/../build_target.pri)

RESOURCES += Resources/cmd_sys.qrc

INCLUDEPATH += $$PWD/../../base2/base2
INCLUDEPATH += $$PWD/../../base2/utility
