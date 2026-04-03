TARGET       = md_rag
TEMPLATE     = lib
CONFIG      += staticlib c++17
#######################################################################################
include($$PWD/../build_target.pri)

HEADERS += $$PWD/MdItem.h
HEADERS += $$PWD/MdModel.h

INCLUDEPATH += $$PWD/../../base2/base2
INCLUDEPATH += $$PWD/../../base2/cmd_sys

QT += core
