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
 * @file CmdArgCol.cpp
 * @brief Implementation of CmdArgCol class.
 */

#include "CmdArgCol_parser.h"

#include <QTextStream>

//=============================================================================
void CmdArgCol_parser::createList_old(const QString& args, CmdArgCol& argsCol)
{
    QList<QString> words;

    bool quotesActive = false;
    QString currentWord;
    bool quotesPossible = true;

    for (qsizetype i = 0; i < args.size(); i++)
    {
        QChar currentChar = args.at(i);

        if (quotesActive) {
            if (currentChar == '"') {
                words.append(currentWord);
                currentWord = "";
                quotesActive = false;
            }
            else {
                currentWord += currentChar;
            }
        }
        else {
            if (currentChar == ' ') {
                if (!currentWord.isEmpty()) {
                    words.append(currentWord);
                    currentWord = "";
                }
            }
            else {
                if (quotesPossible && currentChar == '"') {
                    quotesActive = true;
                }
                else {
                    currentWord += currentChar;
                    if (currentWord=="--") quotesPossible = false;
                }
            }
        }
    }

    if ( !currentWord.isEmpty() ) words.append(currentWord);

    QString name;
    QString value;

    while (!words.isEmpty()) {
        QString current = words.front();
        words.pop_front();
        current = current.trimmed();

        if (current.left(2) == "--") {
            if (!name.isEmpty()) {
                argsCol.append(value, name);
            }
            name = current.right(current.length()-2);
            value = "";
            continue;
        }

        if (name.isEmpty()) {
            argsCol.append(current);
        }
        else {
            if (!value.isEmpty()) value += " ";
            value += current;
        }
    }

    if (!name.isEmpty()) {
        argsCol.append(value, name);
    }
}

///@view:beg

//=============================================================================
void CmdArgCol_parser::splitCustomAppend(QStringList& result, QString& current, bool& isNoname)
{
    current = current.trimmed();
    if (current.isEmpty()) return;

    if (current.startsWith("--")) {
        isNoname = false;
        int spaceIndex = current.indexOf(' ');
        if (spaceIndex != -1) {
            result.append(current.left(spaceIndex));
            current = current.mid(spaceIndex+1);
        }
    }

    if (isNoname && current[0]!='{' && current.contains(' ')) {
        QStringList parts = current.split(' ');
        for (QString& part : parts) { if (!part.isEmpty()) result.append(part); }
    }
    else {
        result.append(current);
    }

    current = "";
}

bool CmdArgCol_parser::isJsonQuote_(const QChar& ch)
{
    if (ch=='"') return true;
    if (ch=='\'') return true;
    return false;
}

//=============================================================================
void CmdArgCol_parser::splitCustom(const QString &input, QStringList& result )
{
    QString     current;

    bool        inEscape   = false;
    bool        inQuotes   = false;
    int         braceDepth = 0;
    int         nameLevel  = 0;
    bool        inNoname   = true;

/*  replace only after detailed tests
    bool        inString   = false;
    QChar       inStringQuote = QChar::Null;
    QChar       previousInBraces = QChar::Null;*/

    for (int i = 0; i < input.size(); ++i) {
        QChar c = input[i];

        if (inEscape) {
            if (c == '\\') { current += '\\'; }
            else if (c == '"') { current += '"'; }
            else { current += '\\'; current += c; }

            inEscape = false;
            continue;
        }

        if (inQuotes) {
            if (c == '"') {
                splitCustomAppend(result, current, inNoname);
                inQuotes = false;
                continue;
            }
            current += c;
            continue;
        }

        if ( braceDepth > 0 ) {
            if      ( c == '}' ) braceDepth--;
            else if ( c == '{' ) braceDepth++;

            current += c;

            if ( braceDepth==0 ) { splitCustomAppend(result, current, inNoname); continue; }
            continue;
        }

/* replace only after detailed tests
        if ( braceDepth > 0 ) {
            if ( inString && previousInBraces!='\\' && current==inStringQuote) {
                inString = false;
                previousInBraces = QChar::Null;
            }
            else if ( !inString && isJsonQuote_(c)) {
                inString = true;
                inStringQuote = c;
            }

            if (!inString) {
                if      ( c == '}' ) braceDepth--;
                else if ( c == '{' ) braceDepth++;
            }

            previousInBraces = c;
            current += c;

            if ( braceDepth==0 ) {
                splitCustomAppend(result, current, inNoname);
                inString = false;
                continue;
            }
            continue;
        }*/

        if (c=='\\') { inEscape = true; continue; }

        if (nameLevel==0 && c==' ') nameLevel++;
        else if (nameLevel==1 && c=='-') nameLevel++;
        else if (nameLevel==2 && c=='-') nameLevel++;
        else { nameLevel=0; if (c==' ') nameLevel++; }

        if (nameLevel==3) {
            nameLevel = 0;
            if (current.chopped(1).trimmed().isEmpty()) {
                current.append(c);
                continue;
            }
            else {
                current.chop(1);
                splitCustomAppend(result, current, inNoname);
                current = "--";
                nameLevel = 0;
                continue;
            }
        }

        QString current0;

        if ( inQuotes || braceDepth>0 ) { current += c; continue; }
        else if ( c == '{' ) { current0 = '{'; braceDepth++; }
        else if ( c == '"' ) { current0 = '"'; inQuotes = true; }
        else                 { current += c;   continue; }

        if (!current.isEmpty()) {
            splitCustomAppend(result, current, inNoname);
            current = current0;
        }
    }

    if (!current.isEmpty()) {
        splitCustomAppend(result, current, inNoname);
    }
}

//=============================================================================
void CmdArgCol_parser::createList(QStringList& divided, CmdArgCol& args)
{
    QString name;
    QString value;

    while (!divided.isEmpty()) {
        QString current = divided.front();
        divided.pop_front();
        current = current.trimmed();

        if (current.left(2) == "--") {
            if (!name.isEmpty()) {
                args.append(value, name);
            }
            name = current.mid(2);
            value = "";
            continue;
        }

        if (current.startsWith("\"")) current = current.mid(1);

        if (name.isEmpty()) {
            args.append(current);
        }
        else {
            if (!value.isEmpty()) value += " ";
            value += current;
        }
    }

    if (!name.isEmpty()) {
        args.append(value, name);
    }
}

//=============================================================================
QString CmdArgCol_parser::afs_single_test(const QString testName, const QString& cmd) {
    QStringList divided;
    CmdArgCol_parser::splitCustom(cmd, divided);

    QStringList divided2 = divided;

    CmdArgCol args;
    CmdArgCol_parser::createList(divided, args);


    divided2.append("<br/>");
    QString dividedOut = divided2.join("<br/>");
    dividedOut.replace(" ", "&#9617;");

    QString result;
    QTextStream stream(&result);

    stream << "<br/>" << testName << "<b>: expected:</b> " << QString(cmd).replace(" ", "&#9617;")  << "<br/>"
        << dividedOut;

    result.append("<br/>");
    result.append(args.toString(CmdArgCol::AS_FORMATTED));

    return result;
}

///@view:end


//=============================================================================
