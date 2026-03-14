#pragma once

#include "CmdSys.h"
#include "AppUtility.h"

#include <QQuickItem>
#include <QQuickWindow>
#include <QQuickItemGrabResult>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QScreen>
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

        CMD_SYS.add("set_window",
        []CMD_ARGS_U -> int {
            if (args.count() < 5) return args.appendError("set_window: usage: set_window <x> <y> <w> <h> [screen]");
            int x = args.get(1).value().toInt();
            int y = args.get(2).value().toInt();
            int w = args.get(3).value().toInt();
            int h = args.get(4).value().toInt();
            QQuickItem* root = rootItem_();
            if (!root) return args.appendError("set_window: no root item");
            QQuickWindow* window = root->window();
            if (!window) return args.appendError("set_window: no window");

            QString screenArg = args.get(5).value();
            if (!screenArg.isEmpty()) {
                int screenIdx = screenArg.toInt();
                QList<QScreen*> screens = QGuiApplication::screens();
                if (screenIdx >= 0 && screenIdx < screens.size()) {
                    window->setScreen(screens[screenIdx]);
                    QPoint origin = screens[screenIdx]->geometry().topLeft();
                    window->setPosition(origin.x() + x, origin.y() + y);
                } else {
                    return args.appendError("set_window: screen index out of range");
                }
            } else {
                window->setPosition(x, y);
            }

            window->resize(w, h);
            args.append(QString("%1,%2 %3x%4").arg(x).arg(y).arg(w).arg(h), "WINDOW");
            return 0;
        }, "app_common");

        CMD_SYS.add("config_set",
        []CMD_ARGS_U -> int {
            if (args.count() < 2) return args.appendError("config_set: usage: config_set <command> [args...]");
            QString line = args.get(1).value();
            for (size_t i = 2; i < args.count(); ++i) {
                const auto& a = args.get(i);
                if (a.name() == "USER") break;
                line += " " + a.value();
            }

            QString error;
            if (AppUtility::replaceOrAppendInConfig(line, error) < 0)
                return args.appendError("config_set: " + error);

            args.append(line, "CONFIG");
            return 0;
        }, "app_common");

        CMD_SYS.add("save_window_position",
        []CMD_ARGS_U -> int {
            QQuickItem* root = rootItem_();
            if (!root) return args.appendError("save_window_position: no root item");
            QQuickWindow* window = root->window();
            if (!window) return args.appendError("save_window_position: no window");

            int x = window->x();
            int y = window->y();
            int w = window->width();
            int h = window->height();

            int screenIdx = -1;
            QScreen* winScreen = window->screen();
            QList<QScreen*> screens = QGuiApplication::screens();
            for (int i = 0; i < screens.size(); ++i) {
                if (screens[i] == winScreen) { screenIdx = i; break; }
            }

            QString line;
            if (screenIdx >= 0) {
                QPoint origin = screens[screenIdx]->geometry().topLeft();
                line = QString("set_window %1 %2 %3 %4 %5")
                    .arg(x - origin.x()).arg(y - origin.y()).arg(w).arg(h).arg(screenIdx);
            } else {
                line = QString("set_window %1 %2 %3 %4").arg(x).arg(y).arg(w).arg(h);
            }

            QString error;
            if (AppUtility::replaceOrAppendInConfig(line, error) < 0)
                return args.appendError("save_window_position: " + error);

            args.append(line, "SAVED");
            return 0;
        }, "app_common");
    }

private:
    static QQuickItem*& rootItem_() { static QQuickItem* item = nullptr; return item; }
};
