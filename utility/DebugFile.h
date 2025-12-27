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
 * @class DebugListModelItem
 * @brief Model for list of DebugListModel.
 */
#pragma once

#include <QString>
#include <QFile>
#include <QTextStream>

class  DebugFile {
public:
    enum ECounter {CNT_PROPT_ASSEMBLER, CNT_GENERIC};
//! @section Construction
    DebugFile() = delete;
//! @section Methods
    static void write(const QString& name, const QString& content, ECounter counter = CNT_GENERIC)
    {
        QString cnt = "cnt_generic.txt";
        if (counter == CNT_PROPT_ASSEMBLER) cnt = "cnt_promptAssembler.txt";

        if (!maxIdInitialized_) {
            QFile file(QString("/data/TEMP/") + cnt);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                in >> maxId_;
                maxIdInitialized_ = true;
                file.close();
            }
        }

        QFile file("/data/TEMP/" + QString("%1").arg(maxId_, 6, 10, QLatin1Char('0'))+ name +  ".txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            out << content;
            file.close();
            maxId_++;
        }

        QFile fileOut(QString("/data/TEMP/") + cnt);
        if (fileOut.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << maxId_;
            file.close();
        }
    }
//=============================================================================
protected:
//! @section Data
    inline static int maxId_ ;
    inline static bool maxIdInitialized_ = false;
//! @section Friends
};
