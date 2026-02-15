/**
 * @class PromptComp
 * @brief Položka promptu.
 */
#pragma once

#include "OregObject.h"

#include <QString>

class  PromptComp  : public OregObject
{
public:
//! @section Construction
    PromptComp(const QString& text);
//! @section Get-set
    QString text() const { return text_; }
//! @section Methods
//! @section Overrides
//=============================================================================
protected:
//! @section Data
    QString text_;
//! @section Friends
//! @section Hidding
    friend class PromptCompModel;
};
