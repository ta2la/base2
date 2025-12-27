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

#include "TimeSpent.h"

#include <QString>
#include <QElapsedTimer>

#include <time.h>

//=============================================================================
class  TimeCounter {
//=============================================================================
public:
//! @section Construction
    TimeCounter(TimeSpent* result) :
        start_(getThreadCpuTime()),
        result_(result)
    {
        timer_.start();
    }
    ~TimeCounter()
    {
        timespec end   = getThreadCpuTime();
        qint64 startNs = static_cast<qint64>(start_.tv_sec) * 1000000000LL + start_.tv_nsec;
        qint64 endNs   = static_cast<qint64>(end.tv_sec) * 1000000000LL + end.tv_nsec;

        result_->threadNs_  += endNs-startNs;
        result_->elapsedNs_ += timer_.nsecsElapsed();
    }
//<METHODS>
    static timespec getThreadCpuTime() {
        timespec ts{};
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
        return ts;
    }
//=============================================================================
protected:
//! @section Data
    QElapsedTimer timer_;
    timespec      start_;

    TimeSpent*    result_;
 };
