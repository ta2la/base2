#pragma once

#include "CmdArgCol.h"

//=============================================================================
class CmdExeFilter {
//=============================================================================
public:
    virtual ~CmdExeFilter() = default;
    virtual bool pass(CmdArgCol& args) = 0;
};
