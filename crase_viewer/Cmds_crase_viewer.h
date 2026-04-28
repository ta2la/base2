#pragma once

#include "CmdSys.h"
#include "CraseObjectsBySqlModel.h"
#include "CraseTreeModel.h"
#include "CraseDrawingModel.h"
#include "SqlAccess.h"

#include <QSqlQuery>
#include <QSqlError>

//=============================================================================
class Cmds_crase_viewer {
public:
    static void setModel(CraseObjectsBySqlModel* m) { model_() = m; }
    static void setTreeModel(CraseTreeModel* m) { treeModel_() = m; }
    static void setDrawingModel(CraseDrawingModel* m) { drawingModel_() = m; }

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
            args.append(QString::number(model_()->rowCount()), "ROWS");
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

            if (drawingModel_() && SqlAccess::inst().connect()) {
                QSqlQuery q(SqlAccess::inst().db());
                q.prepare("SELECT type FROM objects WHERE id = ?");
                q.addBindValue(id);
                if (q.exec() && q.next() && q.value(0).toString() == "drawing")
                    drawingModel_()->loadDrawing(id);
            }
            return 0;
        }, "crase_viewer");

        CMD_SYS.add("crase_draw",
        []CMD_ARGS_U -> int {
            if (!drawingModel_()) return args.appendError("crase_draw: no model");
            int id = args.get(1).value().toInt();
            if (id <= 0) return args.appendError("crase_draw: invalid id");
            drawingModel_()->loadDrawing(id);
            return 0;
        }, "crase_viewer");

        CMD_SYS.add("crase_drag",
        []CMD_ARGS_U -> int {
            if (!drawingModel_()) return args.appendError("crase_drag: no model");
            if (args.count() < 5) return args.appendError("crase_drag: usage: crase_drag <itemId> <relTypeId> <x> <y>");
            int drawingId = drawingModel_()->drawingId();
            if (drawingId <= 0) return args.appendError("crase_drag: no drawing loaded");
            int itemId    = args.get(1).value().toInt();
            int relTypeId = args.get(2).value().toInt();
            int x         = args.get(3).value().toInt();
            int y         = args.get(4).value().toInt();
            if (!SqlAccess::inst().connect()) return args.appendError("crase_drag: no DB");

            QSqlQuery q(SqlAccess::inst().db());
            q.prepare("UPDATE object_rels "
                      "SET value = jsonb_set(value, '{xy}', to_jsonb(ARRAY[?, ?]::int[])) "
                      "WHERE id1 = ? AND id2 = ? AND rel_type_id = ?");
            q.addBindValue(x);
            q.addBindValue(y);
            q.addBindValue(drawingId);
            q.addBindValue(itemId);
            q.addBindValue(relTypeId);
            if (!q.exec()) return args.appendError("crase_drag: " + q.lastError().text());
            if (q.numRowsAffected() == 0) return args.appendError("crase_drag: rel not found");

            drawingModel_()->loadDrawing(drawingId);
            args.append(QString("[%1]/r%2 -> %3,%4").arg(itemId).arg(relTypeId).arg(x).arg(y), "MOVED");
            return 0;
        }, "crase_viewer");
    }

private:
    static CraseObjectsBySqlModel*& model_() { static CraseObjectsBySqlModel* m = nullptr; return m; }
    static CraseTreeModel*& treeModel_() { static CraseTreeModel* m = nullptr; return m; }
    static CraseDrawingModel*& drawingModel_() { static CraseDrawingModel* m = nullptr; return m; }
};
