/**
 * @class OregFilterType
 * @brief Template type filter for any type.
 */
#pragma once

#include "OregFilter.h"

template <typename T>
class OregFilterType : public OregFilter
{
public:
    bool pass(OregObject* object) override
    {
        if (!object)
            return false;

        return dynamic_cast<T*>(object) != nullptr;
    }
};
