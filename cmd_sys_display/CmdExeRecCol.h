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
 * @class CmdExeRecCol
 * @brief Collection of ExeRec.
 */

#pragma once

#include "CmdExeRec.h"
#include "CmdExeGuard.h"

#include <QList>

///@view:beg

//=============================================================================
class  CmdExeRecCol : public CmdExeGuard {
//=============================================================================
public:
//! @section Construction
    CmdExeRecCol();
    static CmdExeRecCol& inst() { static CmdExeRecCol i; return i; }
//! @section Data
    void commandExecuted() override;
    void add(bool found, const QString& argsIn, const QString& argsInOut,
             int index, const QString& sourceName, int sourceId) {
        if (cache_) {
            recordsCache_.append(CmdExeRec(found, argsIn, argsInOut, index, sourceName, sourceId));
        }
        else {
            records_.append(CmdExeRec(found, argsIn, argsInOut, index, sourceName, sourceId));
        }
    }
    int count() const { return records_.count(); }
    CmdExeRec& get(int index) { return records_[index]; }
    CmdExeRec& last() { assert(count() > 0); return records_.last(); }

    void cacheSet() { cache_ = !cache_; records_.append(recordsCache_); recordsCache_.clear(); }
//=============================================================================
protected:
//! @section Data
    bool cache_ = false;

    QList<CmdExeRec> records_;
    QList<CmdExeRec> recordsCache_;
};

///@view:end
