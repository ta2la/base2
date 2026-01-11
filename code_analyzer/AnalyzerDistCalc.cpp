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

#include <queue>
#include <vector>
#include <functional>

/// @view:beg

//=============================================================================
void AnalyzerDistCalc::buildGraph()
{
    graph_.clear();

    // zajistíme existenci všech uzlů
    for (auto it = sys_->nodes_.cbegin(); it != sys_->nodes_.cend(); ++it) {
        graph_[it->first];   // vytvoří prázdný seznam hran
    }

    // zkopírujeme všechny konektory do adjacency listu
    for (auto it = sys_->net_.connectors_.cbegin();
         it != sys_->net_.connectors_.cend(); ++it)
    {
        const AnalyzerConnector& c = it.value();
        graph_[c.node1()].append({ c.node2(), c.length() });
    }
}

//=============================================================================
void AnalyzerDistCalc::calculate()
{
    const QString center = sys_->center_;
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

    // výsledek zapíšeme zpět do AnalyzerNode
    for (auto it = dist.cbegin(); it != dist.cend(); ++it) {
        AnalyzerNode* n = sys_->node(it.key());
        if (n)
            n->setDistToCenter(it.value());
    }
}

//=============================================================================
void AnalyzerDistCalc::addObservers()
{
    const QString& center = sys_->center_;
    const auto& connectors = sys_->net_.connectors_;

    if (center.isEmpty())
        return;

    for (auto it = connectors.cbegin(); it != connectors.cend(); ++it) {
        const AnalyzerConnector& c = it.value();

        // X -> center  ==> observer
        if (c.node2() == center && c.node1() != center) {
            if (AnalyzerNode* n = sys_->node(c.node1())) {

                const double d = n->distToCenter();

                // only override "not a number" (NaN / ±inf)
                if (!std::isfinite(d)) {
                    n->setDistToCenter(-1);
                }
            }
        }
    }
}

//=============================================================================

/// @view:end
