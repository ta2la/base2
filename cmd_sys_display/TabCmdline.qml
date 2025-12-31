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
import QtQuick 2.7
import QtQuick.Controls 2.12
import QtQuick.Layouts 2.4

///@view:beg

Rectangle {
    id: root
    color: "#F0F0F0"
    anchors.fill: parent

    Button {
        x: 10
        y: 10
        text: "STOP CMDS REC"
        onClicked: {
            interactiveIface.callCmd("cmds_stop_record")
        }
    }

    CmdsExecuted {
        x: 10
        y: 40
        width: parent.width - 20
        height: parent.height - 50
    }
}

///@view:end
