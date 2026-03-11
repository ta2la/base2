TARGET       = app_common
TEMPLATE     = lib
CONFIG      += staticlib c++17
#######################################################################################
include($$PWD/../build_target.pri)

include(app_common.pri)

INCLUDEPATH += $$PWD/../file_manager
INCLUDEPATH += $$PWD/../cmd_sys
