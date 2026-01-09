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
 * @file Cmds_test0.cpp
 * @brief Cmds_test0 class implementation.
 */
#include "Cmds_test0.h"

#include "CmdArgCol_parser.h"

#include <QTextStream>
#include <QStack>

//=============================================================================
int Cmds_test0::argcol_to_string(CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context)
{
    CmdArgCol col;
    col.append("mycmd");
    col.append("arg1");
    col.append("arg2");
    col.append("value", "named0");
    col.append("", "--named1");
    col.append("value2", "named2");

    args.append(QString("logcmd <br/>") + col.toString()
                    + "<br/>expected: mycmd arg1 arg2 --named0 value0 --named1 --named2 value2", "RESULT");

    return 0;
}

//=============================================================================
int Cmds_test0::zakleta_princezna(CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context)
{
    return 0;
}

//=============================================================================
QString Cmds_test0::afs_single_test_(const QString name, const char* cmd) {
    CmdArgCol col(cmd);

    QString result;
    QTextStream stream(&result);

    stream << "<br/>" << name << ": expected: " << QString(cmd).replace(" ", "&#9617;")  << "<br/>"
        << col.toString(CmdArgCol::AS_FORMATTED);

    return result;
}

//=============================================================================
int Cmds_test0::argcol_from_string(CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context)
{
    int result = 0;

    result += args.append(afs_single_test_( "TEST 1", R"(mycmd arg1 arg2 --named0 value0 --named1 --named2 value2)" ), "RESULT");
    result += args.append(afs_single_test_( "TEST 2", R"(mycmd "arg1 arg1 arg1" arg2 --named0 "--NECO      neco" --named1 --named2 value2)" ), "RESULT");
    result += args.append(afs_single_test_( "TEST 3", R"(mycmd "arg1 arg2 --named0 { value0 --named1 } --named2 value2")" ), "RESULT");
    result += args.append(afs_single_test_( "TEST 4", R"(mycmd arg1 arg2 --named0 {"outer":{"key":"value"," --extra": "data"},"another":"entry"} --named2 value2")" ), "RESULT");
    result += args.append(afs_single_test_( "TEST 5", R"(mycmd arg1 arg2 --named0 {"outer":{"key":"value","extra": "data"},"another":"entry"} --named2 value2")" ), "RESULT");
    result += args.append(afs_single_test_( "TEST 5", R"(mycmd arg1 {"outer":{"  key":"value","extra": "data"}," another":"entry"} arg2 --named2 value2")" ), "RESULT");

    return 0;
}

//=============================================================================
int Cmds_test0::argcol_from_string2(CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context)
{
    int result = 0;

    result += args.append(CmdArgCol_parser::afs_single_test( "<b>TEST 1</b>", QString() + "1mycmd 1.2arg1 1.3arg2 --1.4named0 1.5value value value --1.6named1 --1.7named2 value2" ), "RESULT");
    result += args.append(CmdArgCol_parser::afs_single_test( "<b>TEST 2</b>", QString() + "1mycmd arg1 arg1 arg arg2 --named0 " +
                                                                          "\"2neco neco\"" +
                                                                          " --3named1 --named2 value"), "RESULT");
    result += args.append(CmdArgCol_parser::afs_single_test( "<b>TEST 3</b>", QString() + "1mycmd argA argB --11namedA " +
                                                                          "{ 2value0 --named1 }" +
                                                                          "--3named value"), "RESULT");
    result += args.append(CmdArgCol_parser::afs_single_test( "<b>TEST 4</b>", QString() + "1mycmd arg1 arg2 --named0 " +
                                                                          "{\"2outer\":{\"key\":\"value\",\" --extra\": \"data\"},\"another\":\"entry\"}" +
                                                                          " --3named2 value2" ), "RESULT");
    result += args.append(CmdArgCol_parser::afs_single_test( "<b>TEST 5</b>", QString() + "1mycmd arg1 arg2 --named0 " +
                                                                          "{\"2outer\":{\"key\":\"value\",\"extra\": \"data\"},\"another\":\"entry\"}" +
                                                                          " --3named value)" ), "RESULT");
    result += args.append(CmdArgCol_parser::afs_single_test( "<b>TEST 6</b>", QString() + "1mycmd arg1 " +
                                                                          "{\"2outer\":{\"  key\":\"value\",\"extra\": \"data\"},\" another\":\"entry\"}" +
                                                                          "3arg2 --named2 value2 "), "RESULT");
    result += args.append(CmdArgCol_parser::afs_single_test( "<b>TEST 6</b>", QString() + "1Text text     text --parametr" +
                                                                          "{2json {vnoreni} json konec}" +
                                                                          "--3samostatny --dalsi" +
                                                                          "\"--4schovany\"" +
                                                                          "5jeste    nekonci --dalsi " +
                                                                          "{6json}" +
                                                                          "7a jeste necko konec"), "RESULT");
    result += args.append(CmdArgCol_parser::afs_single_test( "<b>TEST 6</b>", QString() + "1Text text  " +
                                                                          "\"2moc text    s mezerou\"" +
                                                                          "3dalsi dalsi --dalsi aaa" +
                                                                          "\"--4schovany sch\""), "RESULT");
    result += args.append(CmdArgCol_parser::afs_single_test( "<b>TEST 7</b>", QString() + "oo_block_update --name enterprise - --address 1 --assigned_id 1 " +
                                                                          "--is_running true --assigned_uuid 6a74301d-46b7-4b80-9c13-1e482cf710ba " +
                                                                          "--hardware_id 87e491b05e13ce2582d0363d4e91e1c8248a836fdd555824350c0cf1059d82ed " +
                                                                          "--block_port 8088 --system_info { json }"), "RESULT");

    return result;
}

//=============================================================================
int Cmds_test0::argcol_from_string_alone(CmdArgCol& args, QByteArray* data, const QSharedPointer<CmdContextIface>& context)
{
    int result = 0;

    result += args.append(CmdArgCol_parser::afs_single_test( "<b>ALONE</b>", QString() + "oo_block_update --name enterprise - --address 1 --assigned_id 1 " +
                                                                          "--is_running true --assigned_uuid 6a74301d-46b7-4b80-9c13-1e482cf710ba " +
                                                                          "--hardware_id 87e491b05e13ce2582d0363d4e91e1c8248a836fdd555824350c0cf1059d82ed " +
                                                                          "--block_port 8088 --system_info { json }"), "RESULT");

    return result;
}

//=============================================================================
/*QStringList splitCustom(const QString &input) {
    QString pattern = R"((\{[^}]*\})|("[^"]*")|(\S+))";
    QRegularExpression re(pattern);
    QStringList result;

    QRegularExpressionMatchIterator i = re.globalMatch(input);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        QString token = match.captured().trimmed(); // Trim leading/trailing spaces
        if (token.startsWith('{') || token.startsWith('"')) {
            token = token.mid(1, token.length() - 2).trimmed();
            if (token.isEmpty()) continue; // Skip empty {}
            token.prepend('{').append('}');
        }
        result.append(token);
    }

    // Split tokens containing --something into separate lines
    QStringList finalResult;
    for (const QString &entry : result) {
        QString trimmedEntry = entry.trimmed();
        if (trimmedEntry.contains("--")) {
            QStringList parts = trimmedEntry.split(QRegularExpression("(\s+)(?=--)"), Qt::SkipEmptyParts);
            for (QString &part : parts) {
                finalResult.append(part.trimmed());
            }
        } else {
            finalResult.append(trimmedEntry);
        }
    }

    return finalResult;
}*/

//=============================================================================
