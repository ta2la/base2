#pragma once

#include <QObject>
#include <QString>

//=============================================================================
// Shared user-facing strings reused across apps. Register with QML as
// `appText` context property. Add new properties here when text is needed
// in more than one app.
//=============================================================================
class AppText : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString pickDirDisclaimer READ pickDirDisclaimer CONSTANT)
    Q_PROPERTY(QString pickDirNotes      READ pickDirNotes      CONSTANT)
    Q_PROPERTY(QString pickDirNoWarranty READ pickDirNoWarranty CONSTANT)

public:
    static AppText& inst() { static AppText i; return i; }

    QString pickDirNotes() const {
        return QStringLiteral(
            "Files are only created. You must delete them using your file manager.<br>"
            "Saving is not automatic, you must save manually.<br>"
            "Reloading or a crash may result in the loss of unsaved data.<br>"
            "To pick another directory just refresh this page."
        );
    }
    QString pickDirNoWarranty() const {
        return QStringLiteral(
            "<b>No warranty is provided for the use of this software.</b>"
        );
    }
    QString pickDirDisclaimer() const {
        return pickDirNotes() + "<br><br>" + pickDirNoWarranty();
    }

private:
    AppText() = default;
};
