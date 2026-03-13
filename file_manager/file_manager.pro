TARGET       = file_manager
TEMPLATE     = lib
CONFIG      += staticlib c++17
#######################################################################################
include($$PWD/../build_target.pri)

include(file_manager.pri)

RESOURCES += Resources/file_manager.qrc

INCLUDEPATH += $$PWD/../base
INCLUDEPATH += $$PWD/../cmd_sys
INCLUDEPATH += $$PWD/../utility
