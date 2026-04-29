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
            int newX      = args.get(3).value().toInt();
            int newY      = args.get(4).value().toInt();
            if (!SqlAccess::inst().connect()) return args.appendError("crase_drag: no DB");

            // read old xy + wh of the dragged rel
            QSqlQuery qr(SqlAccess::inst().db());
            qr.prepare("SELECT (value->'xy'->>0)::int, (value->'xy'->>1)::int, "
                       "       COALESCE((value->'wh'->>0)::int, 0), COALESCE((value->'wh'->>1)::int, 0) "
                       "FROM object_rels WHERE id1 = ? AND id2 = ? AND rel_type_id = ?");
            qr.addBindValue(drawingId);
            qr.addBindValue(itemId);
            qr.addBindValue(relTypeId);
            if (!qr.exec() || !qr.next()) return args.appendError("crase_drag: rel not found");
            int oldX = qr.value(0).toInt();
            int oldY = qr.value(1).toInt();
            int w    = qr.value(2).toInt();
            int h    = qr.value(3).toInt();
            int dx   = newX - oldX;
            int dy   = newY - oldY;

            // if dragged item has wh, also shift any rels whose xy was inside the box
            int contained = 0;
            if (w > 0 && h > 0 && (dx != 0 || dy != 0)) {
                QSqlQuery qc(SqlAccess::inst().db());
                qc.prepare("UPDATE object_rels "
                           "SET value = jsonb_set(value, '{xy}', to_jsonb(ARRAY["
                           "  (value->'xy'->>0)::int + ?, (value->'xy'->>1)::int + ?"
                           "]::int[])) "
                           "WHERE id1 = ? AND NOT (id2 = ? AND rel_type_id = ?) "
                           "  AND (value->'xy'->>0)::int BETWEEN ? AND ? "
                           "  AND (value->'xy'->>1)::int BETWEEN ? AND ?");
                qc.addBindValue(dx);
                qc.addBindValue(dy);
                qc.addBindValue(drawingId);
                qc.addBindValue(itemId);
                qc.addBindValue(relTypeId);
                qc.addBindValue(oldX);
                qc.addBindValue(oldX + w);
                qc.addBindValue(oldY);
                qc.addBindValue(oldY + h);
                if (!qc.exec()) return args.appendError("crase_drag (contained): " + qc.lastError().text());
                contained = qc.numRowsAffected();
            }

            QSqlQuery q(SqlAccess::inst().db());
            q.prepare("UPDATE object_rels "
                      "SET value = jsonb_set(value, '{xy}', to_jsonb(ARRAY[?, ?]::int[])) "
                      "WHERE id1 = ? AND id2 = ? AND rel_type_id = ?");
            q.addBindValue(newX);
            q.addBindValue(newY);
            q.addBindValue(drawingId);
            q.addBindValue(itemId);
            q.addBindValue(relTypeId);
            if (!q.exec()) return args.appendError("crase_drag: " + q.lastError().text());

            drawingModel_()->loadDrawing(drawingId);
            args.append(QString("[%1]/r%2 -> %3,%4 (contained=%5)").arg(itemId).arg(relTypeId).arg(newX).arg(newY).arg(contained), "MOVED");
            return 0;
        }, "crase_drawing");

        CMD_SYS.add("crase_zoom",
        []CMD_ARGS_U -> int {
            if (!drawingModel_()) return args.appendError("crase_zoom: no model");
            if (args.count() < 3) return args.appendError("crase_zoom: usage: crase_zoom <x> <y> --ratio <r>");
            int drawingId = drawingModel_()->drawingId();
            if (drawingId <= 0) return args.appendError("crase_zoom: no drawing loaded");
            qreal x = args.get(1).value().toDouble();
            qreal y = args.get(2).value().toDouble();
            qreal r = args.get("ratio", "1.0").value().toDouble();
            if (r <= 0) return args.appendError("crase_zoom: ratio must be > 0");
            if (!SqlAccess::inst().connect()) return args.appendError("crase_zoom: no DB");

            QSqlQuery qr(SqlAccess::inst().db());
            qr.prepare("SELECT COALESCE((value->'origin'->>0)::float8, 0), "
                       "       COALESCE((value->'origin'->>1)::float8, 0), "
                       "       COALESCE((value->>'scale')::float8, 1.0) "
                       "FROM objects WHERE id = ?");
            qr.addBindValue(drawingId);
            qreal ox = 0, oy = 0, sc = 1.0;
            if (qr.exec() && qr.next()) {
                ox = qr.value(0).toDouble();
                oy = qr.value(1).toDouble();
                sc = qr.value(2).toDouble();
            }
            qreal newOx = x - (x - ox) / r;
            qreal newOy = y - (y - oy) / r;
            qreal newSc = sc * r;

            QSqlQuery qw(SqlAccess::inst().db());
            qw.prepare("UPDATE objects SET value = "
                       "  jsonb_set(jsonb_set(value, '{origin}', ?::jsonb), '{scale}', ?::jsonb) "
                       "WHERE id = ?");
            qw.addBindValue(QString("[%1,%2]").arg(newOx).arg(newOy));
            qw.addBindValue(QString::number(newSc));
            qw.addBindValue(drawingId);
            if (!qw.exec()) return args.appendError("crase_zoom: " + qw.lastError().text());

            drawingModel_()->loadDrawing(drawingId);
            args.append(QString("scale %1->%2 origin->%3,%4").arg(sc).arg(newSc).arg(newOx).arg(newOy), "ZOOMED");
            return 0;
        }, "crase_drawing");

        CMD_SYS.add("cmd_mode_box",
        []CMD_ARGS_U -> int {
            if (!drawingModel_()) return args.appendError("cmd_mode_box: no model");
            int drawingId = drawingModel_()->drawingId();
            if (drawingId <= 0) return args.appendError("cmd_mode_box: no drawing loaded");
            QString unselValues = args.get("unselected", "").value();
            if (unselValues.isEmpty()) return 0;
            if (!SqlAccess::inst().connect()) return args.appendError("cmd_mode_box: no DB");

            QStringList ids = unselValues.split(' ', Qt::SkipEmptyParts);
            int toggled = 0;
            for (const QString& s : ids) {
                int itemId = s.toInt();
                if (itemId <= 0) continue;

                QSqlQuery qr(SqlAccess::inst().db());
                qr.prepare("SELECT COALESCE((value->'wh'->>0)::int, 0) "
                           "FROM object_rels WHERE id1 = ? AND id2 = ?");
                qr.addBindValue(drawingId);
                qr.addBindValue(itemId);
                if (!qr.exec() || !qr.next()) continue;
                int currentW = qr.value(0).toInt();

                QSqlQuery qw(SqlAccess::inst().db());
                if (currentW > 0)
                    qw.prepare("UPDATE object_rels SET value = value - 'wh' "
                               "WHERE id1 = ? AND id2 = ?");
                else
                    qw.prepare("UPDATE object_rels "
                               "SET value = jsonb_set(value, '{wh}', '[120,80]'::jsonb) "
                               "WHERE id1 = ? AND id2 = ?");
                qw.addBindValue(drawingId);
                qw.addBindValue(itemId);
                if (qw.exec()) toggled++;
            }

            if (toggled > 0) drawingModel_()->loadDrawing(drawingId);
            args.append(QString::number(toggled), "TOGGLED");
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
