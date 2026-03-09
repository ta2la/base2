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
    color: "#C0C8C0";
    anchors.fill: parent;

    function toggleWindow(idx) {
        var win = mdiArea.windowAt(idx)
        if (win) {
            win.visible = !win.visible
            if (win.visible) win.bringToFront()
        }
    }


    Rectangle { id: mainSpace; color: "#455C71"
        x: 0; y: 0; width: root.width; height: 40

        Image {x:2; y:2; width:32; height:32; source:"images/icon.svg"
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: root.toggleWindow(0)
            }
        }
        Text {x:40; y: 8; font.pointSize: 14; font.bold: true; text: "PROMPT Assembler" }

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

        Canvas {
            anchors.fill: parent
            z: -1
            onPaint: {
                var ctx = getContext("2d")
                ctx.clearRect(0, 0, width, height)
                ctx.strokeStyle = "#d8d8d8"
                ctx.lineWidth = 0.5
                var g = 50
                for (var x = 0; x < width; x += g) {
                    ctx.beginPath(); ctx.moveTo(x, 0); ctx.lineTo(x, height); ctx.stroke()
                }
                for (var y = 0; y < height; y += g) {
                    ctx.beginPath(); ctx.moveTo(0, y); ctx.lineTo(width, y); ctx.stroke()
                }
            }
            onWidthChanged: requestPaint()
            onHeightChanged: requestPaint()
        }

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
                x: index * 50
                y: index * 50
                visible: index === 0  // only first window visible initially
            }
        }
    }
}

///@view:end

