/**
 * @file PromptCompItem.cpp
 * @brief Implementation of class PromptCompItem
 */
#include "PromptCompItem.h"

PromptCompItem::PromptCompItem(const QString& text) :
    text_(text)
{

}

QString PromptCompItem::text() const
{
    return text_;
}
