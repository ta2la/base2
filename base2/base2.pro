TARGET       = base2
TEMPLATE     = lib
CONFIG      += staticlib c++17
#######################################################################################
include($$PWD/../build_target.pri)

include(base2.pri)

DISTFILES += \
    work01.qmodel
