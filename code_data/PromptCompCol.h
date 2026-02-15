/**
 * @class Template
 * @brief Template class header for Universe.
 */
#pragma once

#include <QVector>
#include "PromptComp.h"

class  PromptCompCol
{
public:
    //! @section Construction
    PromptCompCol();
    static PromptCompCol& inst();
    //! @section Methods
    int append(const QString& text);
    int count() const;
    PromptComp* get(int index) const;
protected:
    //! @section Data
    QVector<PromptComp*> items_;
};
