TARGET       = crase_viewer
TEMPLATE     = lib
CONFIG      += staticlib c++17
#######################################################################################
include($$PWD/../build_target.pri)

include(crase_viewer.pri)

RESOURCES += Resources/crase_viewer.qrc

INCLUDEPATH += $$PWD/../base2
INCLUDEPATH += $$PWD/../cmd_sys
INCLUDEPATH += $$PWD/../utility
