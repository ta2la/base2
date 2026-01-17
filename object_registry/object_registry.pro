TARGET       = object_registry
TEMPLATE     = lib
CONFIG      += staticlib c++17
#######################################################################################
include($$PWD/../build_target.pri)

include(object_registry.pri)

INCLUDEPATH += $$PWD/../base
INCLUDEPATH += $$PWD/../cmd_sys
INCLUDEPATH += $$PWD/../utility
INCLUDEPATH += $$PWD/../universe_server_shared

DISTFILES += \
    ../../models/ptalla/object_registry.qmodel
