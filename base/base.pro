TARGET       = base
TEMPLATE     = lib
CONFIG      += staticlib c++17
#######################################################################################
include($$PWD/../build_target.pri)

include(base.pri)

DISTFILES += \
    work01.qmodel
