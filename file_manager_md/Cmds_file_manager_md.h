#pragma once

#include "Cmds_file_manager_base.h"
#include "MdDirModel.h"
#include "BookmarkModel.h"

//=============================================================================
class Cmds_file_manager_md {
public:
    static void registerCmds() {
        Cmds_file_manager_base::registerSetDir("md_set_dir", MdDirModel::inst());
        Cmds_file_manager_base::registerSetBook("bookmark_set_file", BookmarkModel::inst());
    }
};
