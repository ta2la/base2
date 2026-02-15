INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/OregObject.h \
    $$PWD/OregPool.h \
    $$PWD/OregObserver.h \
    $$PWD/OregContainer.h \
    $$PWD/OregContainerList.h \
    $$PWD/OregObjectTest.h \
    $$PWD/OregUpdateLock.h \
    $$PWD/OregFilter.h \
    $$PWD/OregFilterType.h \
    $$PWD/Cmds_oreg_test.h

SOURCES += \
    $$PWD/OregObject.cpp \
    $$PWD/OregPool.cpp \
    $$PWD/OregObserver.cpp \
    $$PWD/OregContainer.cpp \
    $$PWD/OregContainerList.cpp \
    $$PWD/OregUpdateLock.cpp \
    $$PWD/Cmds_oreg_test.cpp
