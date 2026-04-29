#pragma once

#include "CmdSys.h"
#include "CraseObjectsBySqlModel.h"
#include "CraseTreeModel.h"
#include "CraseDrawingModel.h"
#include "CraseSelection.h"
#include "CraseMode.h"
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

        CMD_SYS.add("crase_select_toggle",
        []CMD_ARGS_U -> int {
            int id = args.get(1).value().toInt();
            if (id <= 0) return args.appendError("crase_select_toggle: invalid id");

            QString icon, text;
            if (SqlAccess::inst().connect()) {
                QSqlQuery q(SqlAccess::inst().db());
                q.prepare("SELECT t.icon, o.value::text FROM objects o "
                          "JOIN objects_types t ON o.type = t.type WHERE o.id = ?");
                q.addBindValue(id);
                if (q.exec() && q.next()) {
                    icon = q.value(0).toString();
                    text = q.value(1).toString();
                    if (text.startsWith('"') && text.endsWith('"'))
                        text = text.mid(1, text.length() - 2);
                }
            }

            CraseSelection::inst().toggle(id, icon, text);
            bool on = CraseSelection::inst().contains(id);
            if (model_())        model_()->refreshSelection();
            if (treeModel_())    treeModel_()->refreshSelection();
            if (drawingModel_()) drawingModel_()->refreshSelection();
            CMD_SYS.execute_threadSafe(
                CraseMode::inst().cmd() + (on ? " --selected " : " --unselected ") + QString::number(id));
            args.append(on ? "ON" : "OFF", QString("[%1]").arg(id));
            return 0;
        }, "crase_viewer");

        CMD_SYS.add("crase_select_clear",
        []CMD_ARGS_U -> int {
            CraseSelection::inst().clear();
            if (model_())        model_()->refreshSelection();
            if (treeModel_())    treeModel_()->refreshSelection();
            if (drawingModel_()) drawingModel_()->refreshSelection();
            return 0;
        }, "crase_viewer");

        CMD_SYS.add("cmd_mode_tree",
        []CMD_ARGS_U -> int {
            QString sel = args.get("selected", "").value();
            if (sel.isEmpty()) return 0;
            QStringList ids = sel.split(' ', Qt::SkipEmptyParts);
            if (ids.isEmpty()) return 0;
            int id = ids.first().toInt();
            if (id <= 0) return 0;
            if (treeModel_()) treeModel_()->previewObject(id);
            return 0;
        }, "crase_viewer");

        CMD_SYS.add("crase_set_mode",
        []CMD_ARGS_U -> int {
            QString name = args.count() > 1 ? args.get(1).value() : QString();
            if (name.isEmpty()) name = "voidcmd";
            CraseMode::inst().setCmd(name);
            args.append(name, "MODE");
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
    }

private:
    static CraseObjectsBySqlModel*& model_() { static CraseObjectsBySqlModel* m = nullptr; return m; }
    static CraseTreeModel*& treeModel_() { static CraseTreeModel* m = nullptr; return m; }
    static CraseDrawingModel*& drawingModel_() { static CraseDrawingModel* m = nullptr; return m; }
};
