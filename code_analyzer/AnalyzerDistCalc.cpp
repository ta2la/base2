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

#include "AnalyzerDistCalc.h"
#include "AnalyzerSys.h"
#include "AnalyzerModule.h"
#include "AnalyzerNode.h"
#include "Cmds_code_analyzer.h"

#include <queue>
#include <vector>
#include <functional>

/// @view:beg

//=============================================================================
void AnalyzerDistCalc::buildGraph()
{
    graph_.clear();

    const CodeModuleCol& modules = data_->modules();
        //CodeData::inst().modules().nodes();

    const QStringList nodes = modules.nodes();
    for (const QString& name : nodes) {
        graph_[name];   // vytvoří prázdný seznam hran
    }

    // NEW: get connectors from code_data (stack-local)
    const QList<CodeConnector> connectors =
        modules.connectors();

    // zkopírujeme všechny konektory do adjacency listu
    for (const CodeConnector& c : connectors) {
        graph_[c.node1()].append({ c.node2(), 1.0 });
    }
}

//=============================================================================
void AnalyzerDistCalc::calculate()
{
    const QString center = data_->center_.node;//const QString center = sys_->center_;
    if (center.isEmpty())
        return;

    const double INF = std::numeric_limits<double>::infinity();

    QMap<QString, double> dist;

    for (auto it = graph_.cbegin(); it != graph_.cend(); ++it)
        dist[it.key()] = INF;

    if (!dist.contains(center))
        return;

    dist[center] = 0.0;

    using Item = std::pair<double, QString>;
    std::priority_queue<Item, std::vector<Item>, std::greater<Item>> pq;
    pq.push({0.0, center});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d != dist[u])
            continue;

        for (const Edge& e : graph_[u]) {
            double nd = d + e.w;
            if (nd < dist[e.to]) {
                dist[e.to] = nd;
                pq.push({nd, e.to});
            }
        }
    }

    CodeModuleCol& modules = data_->modules();

    for (auto it = dist.cbegin(); it != dist.cend(); ++it) {
        CodeNode* n =
            modules.get(CodeNodeAddress(
                QString(),       // node name is globally unique
                it.key()
                ));

        if (n) n->setDistToCenter(it.value());
    }

    for (auto it = dist.cbegin(); it != dist.cend(); ++it) {
        AnalyzerNode* n = Cmds_code_analyzer::sys_.node(it.key());
        if (n)
            n->setDistToCenter(it.value());
    }
}

//=============================================================================
void AnalyzerDistCalc::addObservers()
{
    const QString center = data_->center_.node;

    if (center.isEmpty())
        return;

    CodeModuleCol& modules = data_->modules();
    const QList<CodeConnector> connectors = modules.connectors();

    for (const CodeConnector& c : connectors) {

        if (c.node2() == center && c.node1() != center) {

            CodeNode* n =
                modules.get(CodeNodeAddress(
                    QString(),
                    c.node1()
                    ));

            if (!n)
                continue;

            if (!std::isfinite(n->distToCenter())) {
                n->setDistToCenter(-1);
            }
        }
    }

    for (const auto& mit : modules.modules_) {
        CodeModule* mod = mit.second;
        if (!mod)
            continue;

        // fast reject
        if (mod->name() != data_->center_.module)
            continue;

        CodeNodeCol& nodes = mod->nodes();

        for (const auto& nit : nodes.nodes_) {
            CodeNode* n = nit.second;
            if (!n)
                continue;

            if (!std::isfinite(n->distToCenter())) {
                n->setDistToCenter(-100);
            }
        }
    }
}

//=============================================================================

/// @view:end
