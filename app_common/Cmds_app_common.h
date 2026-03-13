#pragma once

#include "CmdSys.h"

#include <QQuickItem>
#include <QQuickWindow>
#include <QQuickItemGrabResult>
#include <QCoreApplication>
#include <QMouseEvent>

//=============================================================================
class Cmds_app_common {
//=============================================================================
public:
    Cmds_app_common() = delete;

    static void setRootItem(QQuickItem* item) { rootItem_() = item; }

    static void registerCmds() {
        CMD_SYS.add("screenshot",
        []CMD_ARGS_U -> int {
            QString path = args.get(1).value().trimmed();
            if (path.isEmpty()) return args.appendError("screenshot: missing path");
            QQuickItem* root = rootItem_();
            if (!root) return args.appendError("screenshot: no root item");

            auto result = root->grabToImage();
            if (!result) return args.appendError("screenshot: grabToImage failed");

            QObject::connect(result.data(), &QQuickItemGrabResult::ready, [result, path]() {
                result->saveToFile(path);
            });

            return 0;
        }, "app_common");

        CMD_SYS.add("click",
        []CMD_ARGS_U -> int {
            if (args.count() < 3) return args.appendError("click: usage: click <x> <y>");
            int x = args.get(1).value().toInt();
            int y = args.get(2).value().toInt();
            QQuickItem* root = rootItem_();
            if (!root) return args.appendError("click: no root item");
            QQuickWindow* window = root->window();
            if (!window) return args.appendError("click: no window");

            QPointF pos(x, y);
            QMouseEvent press(QEvent::MouseButtonPress, pos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
            QMouseEvent release(QEvent::MouseButtonRelease, pos, Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
            QCoreApplication::sendEvent(window, &press);
            QCoreApplication::sendEvent(window, &release);

            args.append(QString("%1,%2").arg(x).arg(y), "CLICK");
            return 0;
        }, "app_common");
    }

private:
    static QQuickItem*& rootItem_() { static QQuickItem* item = nullptr; return item; }
};
