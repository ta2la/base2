#pragma once

#include "CmdSys.h"
#include "CraseObjectsBySqlModel.h"
#include "CraseTreeModel.h"

//=============================================================================
class Cmds_crase_viewer {
public:
    static void setModel(CraseObjectsBySqlModel* m) { model_() = m; }
    static void setTreeModel(CraseTreeModel* m) { treeModel_() = m; }

    static void registerCmds() {
        CMD_SYS.add("reload_view",
        []CMD_ARGS_U -> int {
            if (!model_()) return args.appendError("reload_view: no model");
            QString sql = args.get("sql", "__UNDEFINED__").value();
            if (sql == "__UNDEFINED__") {
                model_()->reload();
                args.append("default", "SQL_USED");
            } else {
                model_()->reload(sql);
                args.append(sql, "SQL_USED");
            }
            return 0;
        }, "crase_viewer");

        CMD_SYS.add("reload_tree",
        []CMD_ARGS_U -> int {
            if (!treeModel_()) return args.appendError("reload_tree: no model");
            QString sql = args.get("sql", "__UNDEFINED__").value();
            if (sql == "__UNDEFINED__") treeModel_()->reload();
            else                        treeModel_()->reload(sql);
            return 0;
        }, "crase_viewer");

        CMD_SYS.add("crase_expand",
        []CMD_ARGS_U -> int {
            if (!treeModel_()) return args.appendError("crase_expand: no model");
            int id = args.get(1).value().toInt();
            if (id <= 0) return args.appendError("crase_expand: invalid id");
            treeModel_()->expand(id);
            return 0;
        }, "crase_viewer");

        CMD_SYS.add("crase_preview",
        []CMD_ARGS_U -> int {
            if (!treeModel_()) return args.appendError("crase_preview: no model");
            int id = args.get(1).value().toInt();
            if (id <= 0) return args.appendError("crase_preview: invalid id");
            treeModel_()->previewObject(id);
            return 0;
        }, "crase_viewer");
    }

private:
    static CraseObjectsBySqlModel*& model_() { static CraseObjectsBySqlModel* m = nullptr; return m; }
    static CraseTreeModel*& treeModel_() { static CraseTreeModel* m = nullptr; return m; }
};
