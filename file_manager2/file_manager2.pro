TARGET       = file_manager2
TEMPLATE     = lib
CONFIG      += staticlib c++17
#######################################################################################
DESTDIR = $$PWD/debug

include(file_manager2.pri)

RESOURCES += file_manager2.qrc

QT += core
