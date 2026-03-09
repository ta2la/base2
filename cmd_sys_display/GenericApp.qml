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
import QtQuick 2.15
import QtQuick.Controls 2.15

///@view:beg

Rectangle  {
    id: root;
    color: "#D9D9D9";
    anchors.fill: parent;

    Rectangle { id: mainSpace; color: "#4cc4bc"
        x: 0; y: 0; width: root.width; height: 40

        Image {x:2; y:2; width:32; height:32; source:"images/icon.svg"}
        Text {x:40; y: 8; font.pointSize: 14; font.bold: true; text: "PROMPT Assemblera" }

        Row {
            x: 250
            y: 10
            spacing: 5

            Repeater {
                model: mainTabs

                delegate: Button {
                    width: 90
                    height: 30
                    text: model.text

                    onClicked: {
                        var win = mdiArea.windowAt(index)
                        if (win) {
                            win.visible = !win.visible
                            if (win.visible) win.bringToFront()
                        }
                    }
                }
            }
        }
    }

    // MDI Area
    Item {
        id: mdiArea
        anchors.fill: parent
        anchors.topMargin: 44

        function windowAt(idx) {
            return mdiRepeater.itemAt(idx)
        }

        Repeater {
            id: mdiRepeater
            model: mainTabs

            delegate: MdiWindow {
                title: model.text
                contentSource: model.qml
                width: 500
                height: 400
                x: 20 + index * 30
                y: 10 + index * 30
                visible: index === 0  // only first window visible initially
            }
        }
    }
}

///@view:end

