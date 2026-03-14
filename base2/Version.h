//
// Copyright (C) 2025 Petr Talla. [petr.talla@gmail.com]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//		      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//=============================================================================
/**
 * @class Version
 * @brief Versioning system class.
 */
#pragma once

#include <QString>
#include <QRegularExpression>
#include <QStringList>

class  Version
{
public:
//! @section Enums
//! @section Construction
    Version() : major_(-1), minor_(-1), patch_(-1) {}
    Version(int major, int minor=0, int patch=0) :
        major_(major),
        minor_(minor),
        patch_(patch) {}
    Version(const QString& version) {
        QStringList list = version.split(".");
        if (list.count()!=3) { *this = Version(1); return; }

        bool ok = false;
        int major = list[0].toInt(&ok);
        if (!ok) { *this = Version(1); return; }
        int minor = list[1].toInt(&ok);
        if (!ok) { *this = Version(1); return; }
        int patch = list[2].toInt(&ok);
        if (!ok) { *this = Version(1); return; }

        *this = Version(major, minor, patch);
    }
//! @section Methods
    int major() const { return major_; }
    int minor() const { return minor_; }
    int patch() const { return patch_; }

    bool isNull() const { return major_ < 0; }

    QString toString() const {
        QString result;
        result += QString::number(major_) + ".";
        result += QString::number(minor_) + ".";
        result += QString::number(patch_);

        return result;
    }

    static bool isVersion(const QString& version) {
        static const QRegularExpression re(R"(^\d+\.\d+\.\d+$)");
        if (re.match(version).hasMatch()) return true;
        return false;
    }
//! @section Operators
    bool operator==(const Version& other) const {
        if (isNull() && other.isNull()) return true;
        if (isNull() || other.isNull()) return false;
        return major_ == other.major_ &&
               minor_ == other.minor_ &&
               patch_ == other.patch_;
    }

    bool operator>(const Version& other) const {
        if (isNull() && other.isNull()) return false;
        if (isNull()) return true;
        if (other.isNull()) return false;
        if (major_ != other.major_) return major_ > other.major_;
        if (minor_ != other.minor_) return minor_ > other.minor_;
        return patch_ > other.patch_;
    }

    // Derived comparisons
    bool operator!=(const Version& other) const { return !(*this == other); }
    bool operator<(const Version& other) const { return !(*this > other) && !(*this == other); }
    bool operator<=(const Version& other) const { return !(*this > other); }
    bool operator>=(const Version& other) const { return !(*this < other); }

    bool checkVersion(const Version& beg, const Version& end) const {
        if (beg > *this) return false;
        if (end < *this) return false;
        return true;
    }
//! @section Overrides
//=============================================================================
protected:
//! @section Data
    int major_;
    int minor_;
    int patch_;
//! @section Friends
//! @section Hidding
};
