TARGET       = object_registry_test
TEMPLATE     = lib
CONFIG      += staticlib c++17
#######################################################################################
include($$PWD/../build_target.pri)

include(object_registry_test.pri)

RESOURCES += $$PWD/Resources/object_registry_test.qrc

INCLUDEPATH += $$PWD/../base
INCLUDEPATH += $$PWD/../cmd_sys
INCLUDEPATH += $$PWD/../utility
INCLUDEPATH += $$PWD/../object_registry
