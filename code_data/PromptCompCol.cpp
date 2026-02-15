/**
 * @file PromptCompCol.cpp
 * @brief Implementation of class PromptCompCol
 */

#include "PromptCompCol.h"
#include "OregUpdateLock.h"

//=============================================================================
PromptCompCol::PromptCompCol()
{
    OregUpdateLock l;

    append("in the next task follow strictly the style of the code above");
    append("prefer conservative solution, do not rebuild code, if not specified");
    append("praca");
}

//=============================================================================
PromptCompCol& PromptCompCol::inst()
{
    static PromptCompCol i;
    return i;
}

//=============================================================================
int PromptCompCol::append(const QString& text)
{
    items_.append(new PromptComp(text));
    return 1;
}

//=============================================================================
int PromptCompCol::count() const
{
    return items_.count();
}

//=============================================================================
PromptComp* PromptCompCol::get(int index) const
{
    if (index < 0 || index >= items_.count()) assert(0);
    return items_.at(index);
}

//=============================================================================
