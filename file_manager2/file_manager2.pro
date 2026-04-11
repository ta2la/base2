TARGET       = file_manager2
TEMPLATE     = lib
CONFIG      += staticlib c++17
#######################################################################################
include($$PWD/../build_target.pri)

include(file_manager2.pri)

RESOURCES += file_manager2.qrc

QT += core
