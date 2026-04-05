#pragma once

#include "DirModelBase.h"

//=============================================================================
class MdDirModel : public DirModelBase {
    Q_OBJECT
public:
    MdDirModel(QObject* parent = nullptr) : DirModelBase(parent) {}
    static MdDirModel& inst() { static MdDirModel i; return i; }
};
