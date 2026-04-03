#pragma once

#include <QObject>
#include <QString>
#include <QList>
#include <QAbstractListModel>

//=============================================================================
class MdWord {
    Q_GADGET
    Q_PROPERTY(QString text MEMBER text_)
    Q_PROPERTY(int style MEMBER style_)
public:
    enum EStyle { NORMAL = 0, BOLD = 1 };

    MdWord() {}
    MdWord(const QString& text, EStyle style = NORMAL)
        : text_(text), style_(style) {}

    QString text_;
    int style_ { NORMAL };
};
Q_DECLARE_METATYPE(MdWord)

//=============================================================================
class MdWordModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles { TextRole = Qt::UserRole + 1, StyleRole };

    explicit MdWordModel(QObject* parent = nullptr) : QAbstractListModel(parent) {}

    void setWords(const QList<MdWord>& words) {
        beginResetModel();
        words_ = words;
        endResetModel();
    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        Q_UNUSED(parent);
        return words_.count();
    }

    QVariant data(const QModelIndex& index, int role) const override {
        if (!index.isValid() || index.row() >= words_.count()) return {};
        const MdWord& w = words_[index.row()];
        if (role == TextRole)  return w.text_;
        if (role == StyleRole) return w.style_;
        return {};
    }

    QHash<int, QByteArray> roleNames() const override {
        return { {TextRole, "text"}, {StyleRole, "style"} };
    }

    const QList<MdWord>& words() const { return words_; }

private:
    QList<MdWord> words_;
};

//=============================================================================
class MdItem {
    Q_GADGET
    Q_PROPERTY(int type MEMBER type_)
public:
    enum EType { HEADING1 = 0, HEADING2 = 1, HEADING3 = 2, PARAGRAPH = 3 };

    MdItem() {}
    MdItem(EType type, const QList<MdWord>& words)
        : type_(type), words_(words) {}

    int type_ { PARAGRAPH };
    QList<MdWord> words_;
};
Q_DECLARE_METATYPE(MdItem)
