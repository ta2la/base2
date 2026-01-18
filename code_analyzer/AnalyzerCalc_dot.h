//
// Copyright (C) 2026 Petr Talla. [petr.talla@gmail.com]
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
#pragma once

#include <QString>
#include <QTextStream>

#include "CodeModuleCol.h"
#include "CodeData.h"

/// @view:beg

class AnalyzerCalc_dot
{
public:

    //=================================
    static QString label3(const CodeNode* n, const QString& prefix = "    ")
    {
        if (!n)
            return QString();

        QString out;
        QTextStream s(&out);

        QString attrs;

        // QML → gray box
        if (n->extensions().contains("qml")) {
            attrs =
                " shape=box"
                " style=filled"
                " fillcolor=\"#DADADA\"";
        }

        // *Model* → blue box
        if (n->name().contains("Model")) {
            attrs += ", shape=box"
                     ", style=filled"
                     ", fillcolor=\"#6FA3D8\"";
        }

        // Cmds_* → violet box
        if (n->name().startsWith("Cmds_")) {
            attrs += ", shape=box"
                     ", style=filled"
                     ", fillcolor=\"#E6C1D8\"";
        }

        s << prefix << "\"" << n->name() << "\" ";

        // label text
        QString lbl;
        {
            QString base = n->name();

            QStringList exts = n->extensions();
            if (!exts.isEmpty()) {
                lbl = base + "\\n[" + exts.join(" ") + "]";
            } else {
                lbl = base;
            }
        }

        s << "[label=\"" << lbl << "\"" << attrs << "];\n";

        return out;
    }


    static QString toDot_()
    {
        QString out;
        QTextStream s(&out);

        const CodeModuleCol& modules =
            CodeData::inst().modules();

        // modules = clusters
        for (const QString& moduleName : modules.names()) {
            const CodeModule* module = modules.get(moduleName);
            if (!module)
                continue;

            s << "  subgraph cluster_" << module->name() << " {\n";
            s << "    label=\"" << module->name() << "\";\n";

            const CodeNodeCol& nodes = module->nodes();

            // nodes
            for (const QString& nodeName : nodes.names()) {
                const CodeNode* n = nodes.get(nodeName);
                if (!n)
                    continue;

                s << label3(n);
            }

            s << "  }\n";
        }

        return out;
    }

protected:
};

/// @view:end
