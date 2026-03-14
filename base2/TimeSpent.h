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
 * @class CmdArg
 * @brief Command argument class.
 */
#pragma once

#include <QString>

//=============================================================================
class  TimeSpent {
//=============================================================================
public:
//! @section Construction
    TimeSpent() = default;
//<METHODS>
    QString timeMsThreadStr()  const { return QString::number(ms(threadNs_))  + " ms";}
    QString timeMsElapsedStr() const { return QString::number(ms(elapsedNs_)) + " ms";}

    int timeMsThread()  { return ms(threadNs_);  }
    int timeMsElapsed() { return ms(elapsedNs_); }

    void add(const TimeSpent& time) {
        elapsedNs_ += time.elapsedNs_;
        threadNs_  += time.threadNs_;
    }

    static int ms(quint64 ns) { quint64 result = ns/1000000; if (result==0 && ns>0) return 1; return result; }

//=============================================================================
protected:
//! @section Data
    quint64 elapsedNs_ = 0;
    quint64 threadNs_  = 0;

    friend class TimeCounter;
};
