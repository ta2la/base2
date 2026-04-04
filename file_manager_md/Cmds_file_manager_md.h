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
    }

private:
    static int shiftBookmark_(CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) {
        if (args.count() < 2) return -1;
        int delta = args.get(1).value().toInt();
        if (delta != 1 && delta != -1) return -1;

        BookmarkModel& m = BookmarkModel::inst();
        int index = m.selectedIndex();
        if (index < 0) return -1;

        QString path = m.dir();
        if (path.isEmpty()) return -1;

        QFile f(path);
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return -1;
        QStringList lines;
        while (!f.atEnd()) {
            QString line = QString::fromUtf8(f.readLine()).trimmed();
            if (!line.isEmpty()) lines.append(line);
        }
        f.close();

        int target = index + delta;
        if (target < 0 || target >= lines.size()) return -1;

        lines.swapItemsAt(index, target);

        if (!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return -1;
        QTextStream out(&f);
        for (const QString& line : lines) out << line << "\n";
        f.close();

        m.swapItems(index, target);
        m.setSelectedIndex(target);
        return 0;
    }
};
