#pragma once

#include "DirModelBase.h"

#include <QFileInfo>

//=============================================================================
class MdDirModel : public DirModelBase {
    Q_OBJECT
public:
    enum Role { None = 0, MdFile = 1 };

    MdDirModel(QObject* parent = nullptr) : DirModelBase(parent) {}
    static MdDirModel& inst() { static MdDirModel i; return i; }

protected:
    FileItemData createItem_(const QFileInfo& fi) override {
        FileItemData item(fi);
        item.setRole(resolveRole_(fi));
        return item;
    }

private:
    static int resolveRole_(const QFileInfo& fi) {
        if (!fi.isDir() && fi.suffix().toLower() == "md") return MdFile;
        return None;
    }
};
