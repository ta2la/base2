#pragma once

#include "MdDirModel.h"

//=============================================================================
class BookmarkModel : public MdDirModel {
    Q_OBJECT
public:
    BookmarkModel(QObject* parent = nullptr) : MdDirModel(parent) {}
    static BookmarkModel& inst() { static BookmarkModel i; return i; }
};
