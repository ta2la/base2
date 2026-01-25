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

#include "CodeNode.h"
#include "Cmds_code_analyzer.h"

/// @view:beg

//=============================================================================
AnalyzerModuleFileData::AnalyzerModuleFileData(const QString& name) :
    node_(CodeData::inst().modules().get(
        CodeNodeAddress(QString(), name)))
{

}

//=============================================================================
QString AnalyzerModuleFileData::name() const
{
    if (!node_)
        return "UNKNOWN";

    return node_->name();
}

//=============================================================================
QString AnalyzerModuleFileData::module() const
{
    if (!node_)
        return "UNKNOWN";

    return node_->address().module;
}

//=============================================================================
double AnalyzerModuleFileData::dist() const
{
    if (!node_) return 1;
    return node_->distToCenter();
}

//=============================================================================
bool AnalyzerModuleFileData::isCenter() const
{
    if (!node_) return false;
    return node_->name() == CodeData::inst().center().node;
}

//=============================================================================
QStringList AnalyzerModuleFileData::types() const
{
    if (!node_)
        return {};

    QStringList list = node_->extensions();
    list.sort();
    return list;
}

//=============================================================================

/// @view:end
