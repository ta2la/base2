TARGET       = file_manager_md
TEMPLATE     = lib
CONFIG      += staticlib c++17
#######################################################################################
include($$PWD/../build_target.pri)

include(file_manager_md.pri)

RESOURCES += Resources/file_manager_md.qrc

QT += core
