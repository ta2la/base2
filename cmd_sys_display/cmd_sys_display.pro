TARGET       = cmd_sys_display
TEMPLATE     = lib
CONFIG      += staticlib c++17
#######################################################################################
include($$PWD/../build_target.pri)

include($$PWD/cmd_sys_display.pri)

INCLUDEPATH += $$PWD/../base
INCLUDEPATH += $$PWD/../cmd_sys
INCLUDEPATH += $$PWD/../utility/cpp_include_entry

RESOURCES += cmd_sys_display.qrc
