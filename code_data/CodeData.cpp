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

#include "CodeData.h"

#include <QTextStream>

/// @view:beg

CodeData::CodeData()
{
}

QString CodeData::toDot() const
{
    QString out;
    QTextStream s(&out);

    s << "digraph Analyzer {\n";

    // === modules as clusters ===
    for (const QString& moduleName : modules_.names()) {

        const CodeModule* module = modules_.get(moduleName);
        if (!module)
            continue;

        s << "  subgraph cluster_" << moduleName << " {\n";
        s << "    label=\"" << moduleName << "\";\n";

        const CodeNodeCol& nodes = module->nodes();

        for (const QString& nodeName : nodes.names()) {
            const CodeNode* n = nodes.get(nodeName);
            if (!n)
                continue;

            QString label = n->name();
            QStringList exts = n->extensions();
            if (!exts.isEmpty()) label += "\<BR/>[" + exts.join(" ") + "]";

            const double d = n->distToCenter();

            if (std::isfinite(d)) {
                label = n->name() + " <B>[" +
                        QString::number(d, 'f', 0) +
                        "]</B>";
            }

            CodeNodeAddress addr(moduleName, n->name());
            const bool isCenterNode = isCenter(addr);

            s << "    \"" << n->name() << "\" [label=<"
              << label << ">";

            if (isCenterNode) {
                s << ", style=filled, fillcolor=\"#E6C1D8\"";
            }

            s << "];\n";
        }

        s << "  }\n";
    }

    // === edges ===
    const QList<CodeConnector> conns = modules_.connectors();
    for (const CodeConnector& c : conns) {
        s << "  \"" << c.node1() << "\" -> \""
          << c.node2() << "\";\n";
    }

    s << "}\n";
    return out;
}

void CodeData::setCenter(const CodeNodeAddress& addr)
{
    if (center_.contains(addr) && center_.count() > 1) {
        center_.removeOne(addr);
        return;
    }

    center_.append(addr);
}

/// @view:end
