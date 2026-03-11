#pragma once

#include "CmdSys.h"

#include <QObject>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>

//=============================================================================
class PreviewModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString filePath READ filePath NOTIFY changed)
    Q_PROPERTY(QString content  READ content  NOTIFY changed)
    Q_PROPERTY(bool    isImage  READ isImage  NOTIFY changed)
    Q_PROPERTY(bool    isDir    READ isDir    NOTIFY changed)
    Q_PROPERTY(int     level    READ level    NOTIFY changed)
public:
    static PreviewModel& inst() { static PreviewModel i; return i; }

    QString filePath() const { return filePath_; }
    QString content()  const { return content_;  }
    bool    isImage()  const { return isImage_;  }
    bool    isDir()    const { return isDir_;    }
    int     level()    const { return level_;    }

    void setFile(const QString& path)
    {
        QFileInfo fi(path);
        QString absPath = QDir::cleanPath(fi.absoluteFilePath());

        CMD_SYS.execute_threadSafe("logcmd setFile absPath=[" + absPath + "] filePath_=[" + filePath_ + "] level=" + QString::number(level_));

        if (absPath == filePath_ && level_ == 1) {
            level_ = 2;
            loadContent_(fi);
            emit changed();
            return;
        }

        filePath_ = QDir::cleanPath(absPath);
        level_    = 1;
        isDir_    = fi.isDir();
        isImage_  = false;
        content_.clear();

        emit changed();
    }

signals:
    void changed();

private:
    PreviewModel() = default;

    void loadContent_(const QFileInfo& fi)
    {
        isDir_   = fi.isDir();
        isImage_ = false;
        content_.clear();

        if (isDir_) return;

        static const QStringList imgExt = { "png", "jpg", "jpeg", "bmp", "svg", "gif" };
        isImage_ = imgExt.contains(fi.suffix().toLower());

        if (!isImage_) {
            QFile f(filePath_);
            if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream ts(&f);
                content_ = ts.readAll();
            }
        }
    }

    QString filePath_;
    QString content_;
    bool    isImage_ = false;
    bool    isDir_   = false;
    int     level_   = 0;
};
