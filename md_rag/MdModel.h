#pragma once

#include "MdItem.h"
#include <QAbstractListModel>
#include <QList>
#include <QDebug>

class MdModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int cursorItem READ cursorItem NOTIFY cursorChanged)
    Q_PROPERTY(int cursorWord READ cursorWord NOTIFY cursorChanged)
public:
    enum Roles { TypeRole = Qt::UserRole + 1, WordModelRole };

    static MdModel& inst() { static MdModel i; return i; }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        Q_UNUSED(parent);
        return items_.count();
    }

    QVariant data(const QModelIndex& index, int role) const override {
        if (!index.isValid() || index.row() >= items_.count()) return {};
        qDebug() << "MdModel::data row=" << index.row() << "role=" << role;
        if (role == TypeRole) return items_[index.row()].type_;
        if (role == WordModelRole)
            return QVariant::fromValue(wordModels_[index.row()]);
        return {};
    }

    QHash<int, QByteArray> roleNames() const override {
        return { {TypeRole, "type"}, {WordModelRole, "wordModel"} };
    }

    int cursorItem() const { return cursorItem_; }
    int cursorWord() const { return cursorWord_; }

    Q_INVOKABLE void moveCursor(int dir) {
        // dir: -1 = left, +1 = right
        if (items_.isEmpty()) return;
        int newWord = cursorWord_ + dir;
        int newItem = cursorItem_;

        if (newWord < 0) {
            newItem--;
            if (newItem < 0) return;
            newWord = items_[newItem].words_.count() - 1;
        }
        else if (newItem >= 0 && newItem < items_.count()
                 && newWord >= items_[newItem].words_.count()) {
            newItem++;
            if (newItem >= items_.count()) return;
            newWord = 0;
        }

        cursorItem_ = newItem;
        cursorWord_ = newWord;
        emit cursorChanged();
    }

    Q_INVOKABLE void setCursor(int item, int word) {
        if (item < 0 || item >= items_.count()) return;
        if (word < 0 || word >= items_[item].words_.count()) return;
        cursorItem_ = item;
        cursorWord_ = word;
        emit cursorChanged();
    }

    void load(const QList<MdItem>& items) {
        beginResetModel();
        qDeleteAll(wordModels_);
        wordModels_.clear();
        items_ = items;
        for (const auto& item : items_) {
            auto* wm = new MdWordModel(this);
            wm->setWords(item.words_);
            wordModels_.append(wm);
        }
        cursorItem_ = items_.isEmpty() ? -1 : 0;
        cursorWord_ = 0;
        endResetModel();
        emit cursorChanged();
    }

    QString cursorWordText() const {
        if (cursorItem_ < 0 || cursorItem_ >= items_.count()) return {};
        const auto& words = items_[cursorItem_].words_;
        if (cursorWord_ < 0 || cursorWord_ >= words.count()) return {};
        return words[cursorWord_].text_;
    }

    const QList<MdItem>& items() const { return items_; }

signals:
    void cursorChanged();

private:
    MdModel(QObject* parent = nullptr) : QAbstractListModel(parent) {}
    QList<MdItem> items_;
    QList<MdWordModel*> wordModels_;
    int cursorItem_ { -1 };
    int cursorWord_ { 0 };
};
