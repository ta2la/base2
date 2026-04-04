#pragma once

#include "MdDirModel.h"

//=============================================================================
class BookmarkModel : public MdDirModel {
    Q_OBJECT
    Q_PROPERTY(int selectedIndex READ selectedIndex WRITE setSelectedIndex NOTIFY selectedIndexChanged)
public:
    BookmarkModel(QObject* parent = nullptr) : MdDirModel(parent) {}
    static BookmarkModel& inst() { static BookmarkModel i; return i; }

    int selectedIndex() const { return selectedIndex_; }
    void setSelectedIndex(int i) { if (selectedIndex_ == i) return; selectedIndex_ = i; emit selectedIndexChanged(); }

signals:
    void selectedIndexChanged();

private:
    int selectedIndex_ = -1;
};
