TARGET       = utility
TEMPLATE     = lib
CONFIG      += staticlib c++11
#######################################################################################
include($$PWD/../build_target.pri)

include(utility.pri)

RESOURCES += $$PWD/resource/utility.qrc

INCLUDEPATH += $$PWD/../../base2/base
INCLUDEPATH += $$PWD/../../base2/cmd_sys
OTHER_FILES += cpp_include_entry/cpp_include_entry.md

QT += core gui widgets
