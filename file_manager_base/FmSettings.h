#pragma once

#include <QString>

//=============================================================================
class FmSettings {
//=============================================================================
public:
    static FmSettings& inst() { static FmSettings i; return i; }

    const QString& codeRoot() const { return codeRoot_; }
    void setCodeRoot(const QString& value) { codeRoot_ = value; }

private:
    FmSettings() = default;
    QString codeRoot_;
};
