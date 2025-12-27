TARGET       = cmd_sys
TEMPLATE     = lib
CONFIG      += staticlib c++11
#######################################################################################
include($$PWD/cmd_sys.pri)

include($$PWD/../build_target.pri)

INCLUDEPATH += $$PWD/../../base2/base
INCLUDEPATH += $$PWD/../../base2/utility
