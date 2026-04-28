TARGET       = crase_drawing
TEMPLATE     = lib
CONFIG      += staticlib c++17
#######################################################################################
include($$PWD/../build_target.pri)

include(crase_drawing.pri)

RESOURCES += Resources/crase_drawing.qrc

INCLUDEPATH += $$PWD/../base2
INCLUDEPATH += $$PWD/../cmd_sys
INCLUDEPATH += $$PWD/../utility
INCLUDEPATH += $$PWD/../crase_viewer

QT += sql
