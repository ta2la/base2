#pragma once

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "CmdSys.h"

//=============================================================================
class SqlAccess {
public:
    static SqlAccess& inst() { static SqlAccess i; return i; }

    bool connect() {
        CMD_SYS.execute_threadSafe("logcmd DBTEST connect called");
        if (db_.isOpen()) return true;
        db_ = QSqlDatabase::addDatabase("QPSQL", "crase");
        db_.setHostName("localhost");
        db_.setPort(5432);
        db_.setDatabaseName("apta");
        db_.setUserName("user");
        db_.setPassword("user");
        bool ok = db_.open();
        if (ok) CMD_SYS.execute_threadSafe("logcmd DBTEST connected OK");
        else    CMD_SYS.execute_threadSafe(QString("logcmd DBTEST connect FAILED: %1").arg(db_.lastError().text()));
        return ok;
    }

    QSqlDatabase& db() { return db_; }
    QString lastError() const { return db_.lastError().text(); }

private:
    SqlAccess() = default;
    QSqlDatabase db_;
};
