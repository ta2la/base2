#pragma once

#include "Cmds_file_manager_base.h"
#include "MdDirModel.h"
#include "BookmarkModel.h"

#include <QFile>
#include <QTextStream>

//=============================================================================
class Cmds_file_manager_md {
public:
    static void registerCmds() {
        Cmds_file_manager_base::registerSetDir("md_set_dir", MdDirModel::inst());
        Cmds_file_manager_base::registerSetBook("bookmark_set_file", BookmarkModel::inst());
        CMD_SYS.add("bookmark_shift", shiftBookmark_, "file_manager_md");
        CMD_SYS.add("bookmark_add", addBookmark_, "file_manager_md");
        CMD_SYS.add("bookmark_level", levelBookmark_, "file_manager_md");
    }

private:
    static QStringList readBook_() {
        QStringList lines;
        QString path = BookmarkModel::inst().dir();
        if (path.isEmpty()) return lines;
        QFile f(path);
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return lines;
        while (!f.atEnd()) {
            QString line = QString::fromUtf8(f.readLine()).trimmed();
            if (!line.isEmpty()) lines.append(line);
        }
        return lines;
    }

    static bool writeBook_(const QStringList& lines) {
        QString path = BookmarkModel::inst().dir();
        if (path.isEmpty()) return false;
        QFile f(path);
        if (!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return false;
        QTextStream out(&f);
        for (const QString& line : lines) out << line << "\n";
        return true;
    }

    static int shiftBookmark_(CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) {
        if (args.count() < 2) return -1;
        int delta = args.get(1).value().toInt();
        if (delta != 1 && delta != -1) return -1;

        BookmarkModel& m = BookmarkModel::inst();
        int index = m.selectedIndex();
        if (index < 0) return -1;

        QStringList lines = readBook_();
        int target = index + delta;
        if (target < 0 || target >= lines.size()) return -1;

        lines.swapItemsAt(index, target);
        if (!writeBook_(lines)) return -1;

        m.swapItems(index, target);
        m.setSelectedIndex(target);
        return 0;
    }

    static int levelBookmark_(CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) {
        if (args.count() < 2) return -1;
        int delta = args.get(1).value().toInt();
        if (delta != 1 && delta != -1) return -1;

        BookmarkModel& m = BookmarkModel::inst();
        int index = m.selectedIndex();
        if (index < 0) return -1;

        QStringList lines = readBook_();
        if (index >= lines.size()) return -1;

        QString line = lines[index];
        int level = 0;
        QString path = line;
        int lastSpace = line.lastIndexOf(' ');
        if (lastSpace > 0) {
            bool ok;
            int val = line.mid(lastSpace + 1).toInt(&ok);
            if (ok) { level = val; path = line.left(lastSpace); }
        }

        level += delta;
        if (level < 0) level = 0;

        lines[index] = path + " " + QString::number(level);
        if (!writeBook_(lines)) return -1;

        m.refresh();
        m.setSelectedIndex(index);
        return 0;
    }

    static int addBookmark_(CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) {
        if (args.count() < 2) return -1;
        QString filePath = args.get(1).value().trimmed();
        if (filePath.isEmpty()) return -1;

        QString path = BookmarkModel::inst().dir();
        if (path.isEmpty()) return -1;

        QFile f(path);
        if (!f.open(QIODevice::Append | QIODevice::Text)) return -1;
        QTextStream out(&f);
        out << filePath << "\n";
        f.close();

        BookmarkModel::inst().refresh();
        return 0;
    }
};
