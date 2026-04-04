//
// Copyright (C) 2025 Petr Talla. [petr.talla@gmail.com]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//		      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//=============================================================================
/**
 * @class Model_controls
 * @brief Model for test ui buttons and text entries.
 */
#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QString>

///@view:beg

class Model_controlsEx : public QAbstractListModel
//=============================================================================
{
    Q_OBJECT
public:
    enum RowType {
        TextType = 0,
        EditType,
        ButtonType
    };
    Q_ENUM(RowType)

    enum ModelRoles {
        RowTypeRole = Qt::UserRole + 1,
        ContentRole,
        CommandRole
    };

    explicit Model_controlsEx(QObject* parent = nullptr) :
        QAbstractListModel(parent)
    {
        setContent("start_stop");
    }

    virtual ~Model_controlsEx() = default;

    QString content() { return content_; }


    virtual void setContent(const QString& content) {
        content_ = content;

        beginResetModel();

        m_items.clear();

        if (content == "start_stop") {
            m_items.append({TextType, "Create table:", ""});
            m_items.append({EditType, "Edit content", ""});
            m_items.append({ButtonType, "STOP CMDS REC", "cmds_stop_record"});
            m_items.append({ButtonType, "on off message", "log_arg_message_onoff"});
        }
        else if (content == "disabled") {
            m_items.append({TextType, "Disabled", ""});
        }
        else if (content == "xxx") {
            m_items.append({TextType, "xxx", ""});
        }
        else {
            m_items.append({TextType, "no valid content", ""});
        }

        endResetModel();
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        Q_UNUSED(parent);
        return m_items.count();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || index.row() < 0 || index.row() >= m_items.size())
            return QVariant();

        const Item &item = m_items.at(index.row());
        if (role == RowTypeRole)
            return item.rowType;
        else if (role == ContentRole)
            return item.content;
        else if (role == CommandRole)
            return item.command;
        return QVariant();
    }

    QHash<int, QByteArray> roleNames() const override {
        QHash<int, QByteArray> roles;
        roles[RowTypeRole] = "rowType";
        roles[ContentRole] = "content";
        roles[CommandRole] = "command";
        return roles;
    }

protected:
    struct Item {
        RowType rowType;
        QString content;
        QString command;
    };

    QList<Item> m_items;
    QString     content_;
};

///@view:end
