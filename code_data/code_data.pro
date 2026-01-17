TARGET       = code_data
TEMPLATE     = lib
CONFIG      += staticlib c++17
#######################################################################################
include($$PWD/../build_target.pri)

include(code_data.pri)

INCLUDEPATH += $$PWD/../../base2/base
INCLUDEPATH += $$PWD/../../base2/cmd_sys
INCLUDEPATH += $$PWD/../../base2/object_registry

QT += core
