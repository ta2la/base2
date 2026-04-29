INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/CraseObject.h \
    $$PWD/CraseObjectsBySqlModel.h \
    $$PWD/CraseTreeItem.h \
    $$PWD/CraseTreeModel.h \
    $$PWD/CraseTypeItem.h \
    $$PWD/CraseTypesModel.h \
    $$PWD/CraseAttrTypeItem.h \
    $$PWD/CraseAttrTypesModel.h \
    $$PWD/CraseRelTypeItem.h \
    $$PWD/CraseRelTypesModel.h \
    $$PWD/CraseMode.h \
    $$PWD/CraseSelection.h \
    $$PWD/SqlAccess.h \
    $$PWD/Cmds_crase_viewer.h

SOURCES += \
    $$PWD/crase_viewer.cpp \
    $$PWD/SqlAccess.cpp
