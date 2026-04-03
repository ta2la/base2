TARGET       = file_manager
TEMPLATE     = lib
CONFIG      += staticlib c++17
#######################################################################################
include($$PWD/../build_target.pri)

include(file_manager.pri)

RESOURCES += Resources/file_manager.qrc

INCLUDEPATH += $$PWD/../file_manager_base
INCLUDEPATH += $$PWD/../base2
INCLUDEPATH += $$PWD/../cmd_sys
INCLUDEPATH += $$PWD/../utility
