import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: mdiWin
    clip: false

    property string title: ""
    property string icon: ""
    property string contentSource: ""
    property bool maximized: false

    property int gridSize: 50
    property int inset: 5

    property real _prevX: 0
    property real _prevY: 0
    property real _prevW: 400
    property real _prevH: 300

    function snapToGrid(val) { return Math.round(val / gridSize) * gridSize }

    function bringToFront() {
        var siblings = parent.children
        var maxZ = 0
        for (var i = 0; i < siblings.length; i++) {
            if (siblings[i].z > maxZ) maxZ = siblings[i].z
        }
        z = maxZ + 1
    }

    function toggleMaximize() {
        if (maximized) {
            x = _prevX; y = _prevY
            width = _prevW; height = _prevH
            maximized = false
        } else {
            _prevX = x; _prevY = y
            _prevW = width; _prevH = height
            x = 0; y = 0
            width = parent.width; height = parent.height
            maximized = true
        }
    }

    // Visual frame (inset by 5px from grid cell)
    Rectangle {
        id: frame
        x: inset; y: inset
        width: mdiWin.width - inset * 2
        height: mdiWin.height - inset * 2
        color: "#e8e8e8"
        border.color: appStyle.barColor
        border.width: 2
        radius: 4
        clip: true

        // Title bar
        Rectangle {
            id: titleBar
            x: 2; y: 2
            width: parent.width - 4
            height: 30
            color: appStyle.barColor
            radius: 2

            Rectangle {
                x: 4; y: 3; width: 24; height: 24; radius: 3
                color: "transparent"; border.color: appStyle.textColor; border.width: 1
                visible: mdiWin.icon !== ""
                Text { anchors.centerIn: parent; font.pointSize: 14; text: mdiWin.icon; color: appStyle.textColor }
            }
            Text {
                x: mdiWin.icon !== "" ? 32 : 8; anchors.verticalCenter: parent.verticalCenter
                text: mdiWin.title
                font.pointSize: 13; font.bold: true
                color: appStyle.textColor
            }

            Row {
                id: titleRow
                anchors.right: parent.right; anchors.rightMargin: 4
                anchors.verticalCenter: parent.verticalCenter
                spacing: 2

                // optional content-provided title bar widget; content's root
                // can expose a Component property `titleBarExtra` and it appears here
                Loader {
                    sourceComponent: contentLoader.item && contentLoader.item.titleBarExtra
                                     ? contentLoader.item.titleBarExtra : null
                    anchors.verticalCenter: parent.verticalCenter
                }

                Button {
                    width: 22; height: 20
                    text: mdiWin.maximized ? "\u2752" : "\u25a1"
                    font.pointSize: 10
                    onClicked: mdiWin.toggleMaximize()
                }

                Button {
                    width: 22; height: 20
                    text: "\u2715"
                    font.pointSize: 10
                    onClicked: mdiWin.visible = false
                }
            }

            MouseArea {
                id: dragArea
                anchors.left: parent.left
                anchors.right: titleRow.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                property real startX: 0
                property real startY: 0
                onPressed: {
                    mdiWin.bringToFront()
                    startX = mouseX; startY = mouseY
                }
                onPositionChanged: {
                    if (!mdiWin.maximized) {
                        var newX = mdiWin.x + mouseX - startX
                        var newY = mdiWin.y + mouseY - startY
                        mdiWin.x = snapToGrid(newX)
                        mdiWin.y = snapToGrid(newY)
                    }
                }
                onDoubleClicked: mdiWin.toggleMaximize()
            }
        }

        // Content
        Loader {
            id: contentLoader
            x: 2; y: titleBar.y + titleBar.height
            width: frame.width - 4
            height: frame.height - titleBar.y - titleBar.height - 2
            source: mdiWin.contentSource
        }

        // Resize handle (bottom-right)
        MouseArea {
            id: resizeHandle
            width: 28; height: 28
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            cursorShape: Qt.SizeFDiagCursor
            property real globalStartX: 0
            property real globalStartY: 0
            property real startW: 0
            property real startH: 0
            onPressed: {
                mdiWin.bringToFront()
                var global = mapToItem(mdiWin.parent, mouseX, mouseY)
                globalStartX = global.x; globalStartY = global.y
                startW = mdiWin.width; startH = mdiWin.height
            }
            onPositionChanged: {
                if (!mdiWin.maximized) {
                    var global = mapToItem(mdiWin.parent, mouseX, mouseY)
                    mdiWin.width = Math.max(200, snapToGrid(startW + global.x - globalStartX))
                    mdiWin.height = Math.max(100, snapToGrid(startH + global.y - globalStartY))
                }
            }

            Canvas {
                anchors.fill: parent
                onPaint: {
                    var ctx = getContext("2d")
                    ctx.strokeStyle = "#999"
                    ctx.lineWidth = 1
                    for (var i = 0; i < 3; i++) {
                        var offset = 3 + i * 4
                        ctx.beginPath()
                        ctx.moveTo(width, offset)
                        ctx.lineTo(offset, height)
                        ctx.stroke()
                    }
                }
            }
        }
    }

    // Click anywhere to bring to front
    MouseArea {
        anchors.fill: parent
        z: -1
        onPressed: {
            mdiWin.bringToFront()
            mouse.accepted = false
        }
    }
}
