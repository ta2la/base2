/**
 * @file PromptCompModel.cpp
 * @brief Implementation of class PromptCompModel
 */

#include "PromptComp.h"
#include "PromptCompModel.h"
#include "PromptCompItem.h"
#include "OregFilterType.h"

//=============================================================================
PromptCompModel::PromptCompModel(QObject* parent)
    : QAbstractListModel(parent)
    , OregContainerList("PromptCompModel")
{
    oo_filterSet(new OregFilterType<PromptComp>());
}

//=============================================================================
PromptCompModel& PromptCompModel::inst()
{
    static PromptCompModel i;
    return i;
}

//=============================================================================
int PromptCompModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return oo_items_.count();
}

//=============================================================================
QVariant PromptCompModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != DataRole)
        return QVariant();

    if (index.row() > prompts_.count()) return QVariant();

    return QVariant::fromValue(prompts_.at(index.row()) );
}

//=============================================================================
void PromptCompModel::updateRow_(int row,  PromptComp* promptComp)
{
    if (row < 0 || row >= prompts_.count()) return;
    PromptCompItem& m = prompts_[row];
    m.text_ = promptComp->text();
    //m.updateCount_++;
}

//=============================================================================
QHash<int, QByteArray> PromptCompModel::roleNames() const
{
    return {
        { DataRole, "promptComp" }
    };
}

//=============================================================================
void PromptCompModel::oo_onObserverChange(OregObserver* object)
{
    PromptComp* codeModule = dynamic_cast<PromptComp*>(object->oo_object());
    if (codeModule == nullptr) return;

    const int row = object->oo_container_index_;
    updateRow_(row, codeModule);

    const QModelIndex modelIndex = createIndex(row, 0);
    emit dataChanged(modelIndex, modelIndex, { DataRole });
}

//=============================================================================
bool PromptCompModel::oo_solveContainment(OregObject* object, bool force)
{
    if (!OregContainerList::oo_solveContainment_prerequisities_(object, force)) return false;

    PromptComp* codeModule = dynamic_cast<PromptComp*>(object);
    if (codeModule == nullptr) return false;

    OregObserver* obs = OregContainerList::oo_solveContainment_(object);

    const int row = prompts_.count();
    beginInsertRows(QModelIndex(), row, row);
    prompts_.append(codeModule->text());
    updateRow_(row, codeModule);
    endInsertRows();

    return true;
}

//=============================================================================
