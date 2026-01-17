TARGET       = code_analyzer
TEMPLATE     = lib
CONFIG      += staticlib c++17
#######################################################################################
include($$PWD/../build_target.pri)

include(code_analyzer.pri)

INCLUDEPATH += $$PWD/../../base2/base
INCLUDEPATH += $$PWD/../../base2/utility
INCLUDEPATH += $$PWD/../../base2/cmd_sys
INCLUDEPATH += $$PWD/../../base2/cmd_sys_display
INCLUDEPATH += $$PWD/../../base2/code_data
INCLUDEPATH += $$PWD/../../base2/object_registry

RESOURCES += code_analyzer.qrc

QT += core gui widgets
