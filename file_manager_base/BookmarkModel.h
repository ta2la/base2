#pragma once

#include "DirModelBase.h"

//=============================================================================
class BookmarkModel : public DirModelBase {
    Q_OBJECT
public:
    BookmarkModel(QObject* parent = nullptr) : DirModelBase(parent) {}
    static BookmarkModel& inst() { static BookmarkModel i; return i; }
};
