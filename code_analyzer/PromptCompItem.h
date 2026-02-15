/**
 * @class PromptCompItem
 * @brief Value-type UI wrapper for PromptComp.
 */
#pragma once

#include <QString>
#include <QtGlobal>
#include <QMetaType>

class PromptComp;

struct PromptCompItem
{
    Q_GADGET

    Q_PROPERTY(QString text READ text CONSTANT)

public:
    PromptCompItem(const QString& text = "");
    QString text() const;

private:
    QString text_;

    friend class PromptCompModel;
};

Q_DECLARE_METATYPE(PromptCompItem)
