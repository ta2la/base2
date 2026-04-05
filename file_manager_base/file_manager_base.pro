TARGET       = file_manager_base
TEMPLATE     = lib
CONFIG      += staticlib c++17
#######################################################################################
include($$PWD/../build_target.pri)

include(file_manager_base.pri)

RESOURCES += Resources/file_manager_base.qrc

INCLUDEPATH += $$PWD/../base2
INCLUDEPATH += $$PWD/../cmd_sys
INCLUDEPATH += $$PWD/../utility

QT += core
