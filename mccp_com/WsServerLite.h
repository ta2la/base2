#pragma once

#include "CmdSys.h"

#include <QObject>
#include <QThread>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QList>

//=============================================================================
class WsServerLite : public QObject {
    Q_OBJECT
public:
    WsServerLite(quint16 port = 12345)
        : QObject(nullptr)
        , port_(port)
    {
    }

    static WsServerLite& init(quint16 port = 12345) {
        static WsServerLite i(port);
        static bool started = false;
        if (!started) {
            started = true;
            static QThread thread;
            i.moveToThread(&thread);
            QObject::connect(&thread, &QThread::started, &i, &WsServerLite::start_);
            thread.start();
        }
        return i;
    }

    void broadcast(const QString& text) {
        for (QWebSocket* client : clients_) {
            client->sendTextMessage(text);
        }
    }

private slots:
    void start_() {
        server_ = new QWebSocketServer("WsServerLite", QWebSocketServer::NonSecureMode, this);
        if (server_->listen(QHostAddress::LocalHost, port_)) {
            connect(server_, &QWebSocketServer::newConnection, this, &WsServerLite::onNewConnection_);
        }
    }

    void onNewConnection_() {
        QWebSocket* socket = server_->nextPendingConnection();
        clients_ << socket;
        connect(socket, &QWebSocket::textMessageReceived, this, &WsServerLite::onMessage_);
        connect(socket, &QWebSocket::disconnected, this, &WsServerLite::onDisconnected_);
    }

    void onMessage_(const QString& message) {
        QStringList lines = message.split('\n');
        for (const QString& line : lines) {
            QString trimmed = line.trimmed();
            if (!trimmed.isEmpty()) {
                CMD_SYS.execute_threadSafe(trimmed, "ws", -1, "EXTERNAL_WS");
            }
        }
    }

    void onDisconnected_() {
        QWebSocket* socket = qobject_cast<QWebSocket*>(sender());
        if (socket) {
            clients_.removeAll(socket);
            socket->deleteLater();
        }
    }

private:
    quint16 port_;
    QWebSocketServer* server_ = nullptr;
    QList<QWebSocket*> clients_;
};
