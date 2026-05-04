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

    function setWindowGeom(idx, x, y, w, h) {
        var win = mdiArea.windowAt(idx)
        if (!win) return
        win.x = x; win.y = y
        win.width = w; win.height = h
        win.visible = true
        win.bringToFront()
    }

    function setWindowGeomPct(idx, xp, yp, wp, hp) {
        var win = mdiArea.windowAt(idx)
        if (!win) return
        win.x = mdiArea.width  * xp / 100
        win.y = mdiArea.height * yp / 100
        win.width  = mdiArea.width  * wp / 100
        win.height = mdiArea.height * hp / 100
        win.visible = true
        win.bringToFront()
    }

    // arrange a 2x2 grid of windows with fixed pixel gap and margin.
    // bottomRatio = how much taller bottom row is vs top (1.25 = 25% taller).
    function arrange2x2(idx_tl, idx_tr, idx_bl, idx_br, gap, margin, bottomRatio) {
        var aw = mdiArea.width, ah = mdiArea.height
        var cellW = (aw - 2*margin - gap) / 2
        var totalH = ah - 2*margin - gap
        var topH = totalH / (1 + bottomRatio)
        var botH = totalH - topH
        setWindowGeom(idx_tl, margin,            margin,                cellW, topH)
        setWindowGeom(idx_tr, margin+cellW+gap,  margin,                cellW, topH)
        setWindowGeom(idx_bl, margin,            margin+topH+gap,       cellW, botH)
        setWindowGeom(idx_br, margin+cellW+gap,  margin+topH+gap,       cellW, botH)
    }

    // global right-click → unselect all
    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        z: 200
        onClicked: qmlInterface.callCmd("crase_select_clear")
    }


    Rectangle { id: mainSpace; color: appStyle.barColor
        x: 0; y: 0; width: root.width; height: 40

        Rectangle { x:4; y:4; width:32; height:32; radius:4; color: appStyle.textColor
            Text { anchors.centerIn: parent; font.pointSize: 20; text: appStyle.icon }
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: root.toggleWindow(0)
            }
        }
        Text {id: appNameText; x:42; y: 8; font.pointSize: 14; font.bold: true; text: Qt.application.name; color: appStyle.textColor }

        Row {
            anchors.left: appNameText.right
            anchors.leftMargin: 16
            anchors.verticalCenter: appNameText.verticalCenter
            spacing: 4
            Repeater {
                model: [
                    {cmd: "cmd_mode_box",     icon: "▭"},
                    {cmd: "cmd_mode_draw",    icon: "✏️"},
                    {cmd: "cmd_mode_undraw",  icon: "🧹"}
                ]
                delegate: Rectangle {
                    property bool active: typeof craseMode !== "undefined" && craseMode.cmd === modelData.cmd
                    width: btnText.implicitWidth + 16
                    height: 24
                    radius: 3
                    color: active ? "#C04040"
                          : btnMa.containsMouse ? Qt.tint(Qt.lighter(appStyle.barColor, 1.2), "#40FF8080")
                          : Qt.tint(Qt.lighter(appStyle.barColor, 1.05), "#30FF8080")
                    border.color: "#C0C0C0"
                    border.width: 1
                    Text {
                        id: btnText
                        anchors.centerIn: parent
                        font.pointSize: 10
                        font.bold: true
                        color: appStyle.textColor
                        text: modelData.icon + " " + modelData.cmd
                    }
                    MouseArea {
                        id: btnMa
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: qmlInterface.callCmd("crase_set_mode " + modelData.cmd)
                    }
                }
            }
        }
    }

    // App status bar (bottom)
    Rectangle {
        id: appStatusBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 33
        color: appStyle.barColor
        z: 10

        Text {
            id: modeText
            anchors.left: parent.left
            anchors.leftMargin: 8
            anchors.verticalCenter: parent.verticalCenter
            font.pointSize: 11
            color: appStyle.textColor
            textFormat: Text.RichText
            text: typeof craseMode !== "undefined"
                  ? "mode: <b>" + craseMode.cmd + "</b>"
                  : ""
        }

        Item {
            id: selArea
            anchors.left: modeText.right
            anchors.leftMargin: 16
            anchors.right: unselBtn.left
            anchors.rightMargin: 8
            anchors.verticalCenter: parent.verticalCenter
            height: 24
            visible: typeof craseSelection !== "undefined" && craseSelection.count > 0

            property bool compact: fullRow.implicitWidth > (width - selLabel.width - 8)

            Text {
                id: selLabel
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pointSize: 11
                color: appStyle.textColor
                textFormat: Text.RichText
                text: "selected: <b>"
                      + (typeof craseSelection !== "undefined" ? craseSelection.count : 0)
                      + "</b>"
            }

            Row {
                id: fullRow
                anchors.left: selLabel.right
                anchors.leftMargin: 8
                anchors.verticalCenter: parent.verticalCenter
                spacing: 4
                visible: !selArea.compact
                Repeater {
                    model: typeof craseSelection !== "undefined" ? craseSelection.items : null
                    delegate: Rectangle {
                        height: 22
                        width: chipRow.implicitWidth + 12
                        radius: 3
                        color: "#F0C0D0"
                        Row {
                            id: chipRow
                            anchors.centerIn: parent
                            spacing: 4
                            Text { text: modelData.icon; font.pointSize: 11 }
                            Text {
                                text: "[" + modelData.id + "] " + (modelData.text.length > 20 ? modelData.text.substring(0, 20) + "…" : modelData.text)
                                font.pointSize: 9
                                color: "#404040"
                            }
                        }
                    }
                }
            }

            Row {
                id: compactRow
                anchors.left: selLabel.right
                anchors.leftMargin: 8
                anchors.verticalCenter: parent.verticalCenter
                spacing: 1
                visible: selArea.compact
                Repeater {
                    model: typeof craseSelection !== "undefined" ? craseSelection.items : null
                    delegate: Text { text: modelData.icon; font.pointSize: 11 }
                }
            }
        }

        Rectangle {
            id: unselBtn
            visible: typeof craseSelection !== "undefined" && craseSelection.count > 0
            anchors.right: parent.right
            anchors.rightMargin: 8
            anchors.verticalCenter: parent.verticalCenter
            width: unselTxt.implicitWidth + 16
            height: 24
            radius: 3
            color: unselMa.containsMouse ? "#708090" : "#5680A0"
            Text {
                id: unselTxt
                anchors.centerIn: parent
                text: "Unselect all"
                font.pointSize: 9
                font.bold: true
                color: "#fff"
            }
            MouseArea {
                id: unselMa
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: qmlInterface.callCmd("crase_select_clear")
            }
        }
    }

    // MDI Area
    Item {
        id: mdiArea
        anchors.fill: parent
        anchors.topMargin: 44
        anchors.bottomMargin: 33

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
                icon: model.icon
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

