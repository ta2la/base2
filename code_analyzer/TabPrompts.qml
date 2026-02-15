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
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

///@view:beg
ScrollView {
    anchors.fill: parent
    clip: true

    Rectangle {
        id: contentRoot
        color: "#F0F0F0"

        width: parent.width
        implicitHeight: contentColumn.implicitHeight

        Column {
            id: contentColumn
            width: parent.width
            spacing: 4

            Repeater {
                model: promptModel

                delegate: Rectangle {
                    width: parent.width
                    height: 28
                    color: "transparent"

                    Row {
                        anchors.fill: parent
                        anchors.leftMargin: 8
                        anchors.rightMargin: 8
                        spacing: 10

                        Text {
                            text: "[" + index + "]"
                            width: 40
                            color: "#808080"
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 9
                        }

                        Text {
                            text: promptComp.text
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 11
                            color: "#404040"
                            elide: Text.ElideRight
                            width: parent.width - 60
                        }
                    }
                }
            }
        }
    }
}
///@view:end
