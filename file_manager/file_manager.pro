TARGET       = file_manager
TEMPLATE     = lib
CONFIG      += staticlib c++17
#######################################################################################
include($$PWD/../build_target.pri)

include(file_manager.pri)

INCLUDEPATH += $$PWD/../base
INCLUDEPATH += $$PWD/../cmd_sys
INCLUDEPATH += $$PWD/../utility
