#pragma once

#include "CmdSys.h"
#include "DirModelBase.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

//=============================================================================
class Cmds_file_manager_base {
public:
    static void registerSetDir(const QString& cmdName, DirModelBase& model) {
        inst_ = &model;
        CMD_SYS.add(cmdName, setDir_);
    }

    static void registerSetBook(const QString& cmdName, DirModelBase& model) {
        instBook_ = &model;
        CMD_SYS.add(cmdName, setBook_);
        CMD_SYS.add("bookmark_shift", shiftBookmark_, "file_manager_base");
        CMD_SYS.add("bookmark_add", addBookmark_, "file_manager_base");
        CMD_SYS.add("bookmark_level", levelBookmark_, "file_manager_base");
    }

private:
    static inline DirModelBase* inst_ = nullptr;
    static inline DirModelBase* instBook_ = nullptr;

    static int setDir_(CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) {
        if (!inst_ || args.count() < 2) return -1;
        QString path = args.get(1).value().trimmed();
        QDir dir(path);
        if (!dir.exists()) return -1;
        inst_->setDir(dir.absolutePath());
        return 0;
    }

    static int setBook_(CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) {
        if (!instBook_ || args.count() < 2) return -1;
        QString path = args.get(1).value().trimmed();
        QFileInfo fi(path);
        if (!fi.exists()) return -1;
        instBook_->setDir(fi.absoluteFilePath());
        return 0;
    }

    // --- bookmark helpers ---

    static QStringList readBook_() {
        QStringList lines;
        if (!instBook_) return lines;
        QString path = instBook_->dir();
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
        if (!instBook_) return false;
        QString path = instBook_->dir();
        if (path.isEmpty()) return false;
        QFile f(path);
        if (!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return false;
        QTextStream out(&f);
        for (const QString& line : lines) out << line << "\n";
        return true;
    }

    // --- bookmark commands ---

    static int shiftBookmark_(CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) {
        if (!instBook_ || args.count() < 2) return -1;
        int delta = args.get(1).value().toInt();
        if (delta != 1 && delta != -1) return -1;

        int index = instBook_->selectedIndex();
        if (index < 0) return -1;

        QStringList lines = readBook_();
        int target = index + delta;
        if (target < 0 || target >= lines.size()) return -1;

        lines.swapItemsAt(index, target);
        if (!writeBook_(lines)) return -1;

        instBook_->swapItems(index, target);
        instBook_->setSelectedIndex(target);
        return 0;
    }

    static int addBookmark_(CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) {
        if (!instBook_ || args.count() < 2) return -1;
        QString filePath = args.get(1).value().trimmed();
        if (filePath.isEmpty()) return -1;

        QString path = instBook_->dir();
        if (path.isEmpty()) return -1;

        QFile f(path);
        if (!f.open(QIODevice::Append | QIODevice::Text)) return -1;
        QTextStream out(&f);
        out << filePath << "\n";
        f.close();

        instBook_->refresh();
        return 0;
    }

    static int levelBookmark_(CmdArgCol& args, QByteArray*, const QSharedPointer<CmdContextIface>&) {
        if (!instBook_ || args.count() < 2) return -1;
        int delta = args.get(1).value().toInt();
        if (delta != 1 && delta != -1) return -1;

        int index = instBook_->selectedIndex();
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

        instBook_->refresh();
        instBook_->setSelectedIndex(index);
        return 0;
    }
};
