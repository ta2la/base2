/**
 * @class PromptCompModel
 * @brief Registry-backed list model exposing PromptComp items to UI.
 */
#pragma once

#include <QAbstractListModel>
#include "OregContainerList.h"
#include "PromptCompItem.h"

class PromptComp;

class PromptCompModel : public QAbstractListModel, public OregContainerList
{
public:
    //! @section Enums
    enum Roles {
        DataRole = Qt::UserRole + 1
    };

    //! @section Construction
    explicit PromptCompModel(QObject* parent = nullptr);
    static PromptCompModel& inst();

    //! @section Get-set

    //! @section Methods
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    //! @section Overrides
    void oo_onObserverChange(OregObserver* observer) override;
    bool oo_solveContainment(OregObject* object, bool force) override;

    //=============================================================================
protected:
    //! @section Data
    //! @section Friends
    //! @section Hidding
    void updateRow_(int row,  PromptComp* promptComp);

    QList<PromptCompItem> prompts_;
};
