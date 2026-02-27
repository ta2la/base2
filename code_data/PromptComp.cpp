/**
 * @file PromptComp.cpp
 * @brief Implementation of class PromptComp
 */

#include "PromptComp.h"

//=============================================================================
PromptComp::PromptComp(const QString& text)
    : text_(text)
{
}

//=============================================================================
QString PromptComp::oo_to_string(EStringFormat format) const
{
    if (format == SF_BASIC) {
        return QString("PromptComp: ") + text_;
    }
    return OregObject::oo_to_string(format);
}

//=============================================================================
