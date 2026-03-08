//
// Copyright (C) 2026 Petr Talla. [petr.talla@gmail.com]
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
 * @class StdoutCmdOutput
 * @brief Outputs executed commands to stdout for external tool communication.
 */

#pragma once

#include "CmdExeGuard.h"

///@view:beg
//=============================================================================
class StdoutCmdOutput : public CmdExeGuard {
    //=============================================================================
public:
    //! @section Construction
    StdoutCmdOutput();
    static StdoutCmdOutput& inst() { static StdoutCmdOutput i; return i; }
    //! @section Methods
    // TODO: Disabled by default - when enabled, Qt Creator feeds stdout back to stdin
    //       causing command echo loop. Enable only for pipe/bridge setups (e.g. STDIO_BRIDGE).
    void setEnabled(bool value) { enabled_ = value; }
    bool isEnabled() const { return enabled_; }
    //! @section Overrides
    void commandExecuted() override;
private:
    bool enabled_ = false;
};
///@view:end
