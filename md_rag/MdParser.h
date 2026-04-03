#pragma once

#include "MdItem.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>

//=============================================================================
class MdParser {
public:
    static QList<MdItem> fromFile(const QString& path) {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return {};
        QTextStream in(&file);
        return fromString(in.readAll());
    }

    static QList<MdItem> fromString(const QString& md) {
        QList<MdItem> items;
        QStringList lines = md.split('\n');

        for (const QString& line : lines) {
            QString trimmed = line.trimmed();
            if (trimmed.isEmpty()) continue;

            MdItem::EType type = MdItem::PARAGRAPH;
            QString content = trimmed;

            if (trimmed.startsWith("### ")) {
                type = MdItem::HEADING3;
                content = trimmed.mid(4);
            } else if (trimmed.startsWith("## ")) {
                type = MdItem::HEADING2;
                content = trimmed.mid(3);
            } else if (trimmed.startsWith("# ")) {
                type = MdItem::HEADING1;
                content = trimmed.mid(2);
            }

            items.append(MdItem(type, parseWords(content)));
        }
        return items;
    }

private:
    static QList<MdWord> parseWords(const QString& text) {
        QList<MdWord> words;
        QStringList parts = text.split(' ', Qt::SkipEmptyParts);
        bool inBold = false;

        for (const QString& part : parts) {
            QString word = part;
            MdWord::EStyle style = MdWord::NORMAL;

            if (word.startsWith("**") && word.endsWith("**") && word.length() > 4) {
                word = word.mid(2, word.length() - 4);
                style = MdWord::BOLD;
            } else if (word.startsWith("**")) {
                word = word.mid(2);
                inBold = true;
                style = MdWord::BOLD;
            } else if (word.endsWith("**")) {
                word = word.left(word.length() - 2);
                style = MdWord::BOLD;
                inBold = false;
            } else if (inBold) {
                style = MdWord::BOLD;
            }

            words.append(MdWord(word, style));
        }
        return words;
    }
};
