#pragma once

#include "CmdSys.h"
#include "CraseDrawingModel.h"
#include "SqlAccess.h"

#include <QSqlQuery>
#include <QSqlError>

//=============================================================================
class Cmds_crase_drawing {
public:
    static void setDrawingModel(CraseDrawingModel* m) { drawingModel_() = m; }
    static CraseDrawingModel* drawingModel() { return drawingModel_(); }

    static void registerCmds() {
        CMD_SYS.add("crase_draw",
        []CMD_ARGS_U -> int {
            if (!drawingModel_()) return args.appendError("crase_draw: no model");
            int id = args.get(1).value().toInt();
            if (id <= 0) return args.appendError("crase_draw: invalid id");
            drawingModel_()->loadDrawing(id);
            return 0;
        }, "crase_drawing");

        CMD_SYS.add("crase_pan",
        []CMD_ARGS_U -> int {
            if (!drawingModel_()) return args.appendError("crase_pan: no model");
            if (args.count() < 3) return args.appendError("crase_pan: usage: crase_pan <dx> <dy>");
            int drawingId = drawingModel_()->drawingId();
            if (drawingId <= 0) return args.appendError("crase_pan: no drawing loaded");
            qreal dx = args.get(1).value().toDouble();
            qreal dy = args.get(2).value().toDouble();
            if (!SqlAccess::inst().connect()) return args.appendError("crase_pan: no DB");

            QSqlQuery qr(SqlAccess::inst().db());
            qr.prepare("SELECT COALESCE((value->'origin'->>0)::float8, 0), "
                       "       COALESCE((value->'origin'->>1)::float8, 0) "
                       "FROM objects WHERE id = ?");
            qr.addBindValue(drawingId);
            qreal ox = 0, oy = 0;
            if (qr.exec() && qr.next()) { ox = qr.value(0).toDouble(); oy = qr.value(1).toDouble(); }
            ox += dx; oy += dy;

            QSqlQuery qw(SqlAccess::inst().db());
            qw.prepare("UPDATE objects SET value = jsonb_set(value, '{origin}', ?::jsonb) WHERE id = ?");
            qw.addBindValue(QString("[%1,%2]").arg(ox).arg(oy));
            qw.addBindValue(drawingId);
            if (!qw.exec()) return args.appendError("crase_pan: " + qw.lastError().text());

            drawingModel_()->loadDrawing(drawingId);
            args.append(QString("origin -> %1,%2").arg(ox).arg(oy), "PANNED");
            return 0;
        }, "crase_drawing");

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
        }, "crase_drawing");

        CMD_SYS.add("crase_resize",
        []CMD_ARGS_U -> int {
            if (!drawingModel_()) return args.appendError("crase_resize: no model");
            if (args.count() < 5) return args.appendError("crase_resize: usage: crase_resize <itemId> <relTypeId> <w> <h>");
            int drawingId = drawingModel_()->drawingId();
            if (drawingId <= 0) return args.appendError("crase_resize: no drawing loaded");
            int itemId    = args.get(1).value().toInt();
            int relTypeId = args.get(2).value().toInt();
            int w         = args.get(3).value().toInt();
            int h         = args.get(4).value().toInt();
            if (!SqlAccess::inst().connect()) return args.appendError("crase_resize: no DB");

            QSqlQuery q(SqlAccess::inst().db());
            q.prepare("UPDATE object_rels "
                      "SET value = jsonb_set(value, '{wh}', to_jsonb(ARRAY[?, ?]::int[])) "
                      "WHERE id1 = ? AND id2 = ? AND rel_type_id = ?");
            q.addBindValue(w);
            q.addBindValue(h);
            q.addBindValue(drawingId);
            q.addBindValue(itemId);
            q.addBindValue(relTypeId);
            if (!q.exec()) return args.appendError("crase_resize: " + q.lastError().text());
            if (q.numRowsAffected() == 0) return args.appendError("crase_resize: rel not found");

            drawingModel_()->loadDrawing(drawingId);
            args.append(QString("[%1]/r%2 -> %3x%4").arg(itemId).arg(relTypeId).arg(w).arg(h), "RESIZED");
            return 0;
        }, "crase_drawing");
    }

private:
    static CraseDrawingModel*& drawingModel_() { static CraseDrawingModel* m = nullptr; return m; }
};
