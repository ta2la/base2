import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: mdiWin
    color: "#e8e8e8"
    border.color: activeFocus ? "#4cc4bc" : "#999"
    border.width: 2
    radius: 4
    clip: true

    property string title: ""
    property string contentSource: ""
    property bool maximized: false

    property real _prevX: 0
    property real _prevY: 0
    property real _prevW: 400
    property real _prevH: 300

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

    // Title bar
    Rectangle {
        id: titleBar
        width: parent.width
        height: 26
        color: "#4cc4bc"
        radius: 3

        // Square off bottom corners
        Rectangle {
            width: parent.width; height: 4
            anchors.bottom: parent.bottom
            color: parent.color
        }

        Text {
            x: 8; anchors.verticalCenter: parent.verticalCenter
            text: mdiWin.title
            font.pointSize: 10; font.bold: true
            color: "#fff"
        }

        Row {
            anchors.right: parent.right; anchors.rightMargin: 4
            anchors.verticalCenter: parent.verticalCenter
            spacing: 2

            // Maximize/restore
            Button {
                width: 22; height: 20
                text: mdiWin.maximized ? "\u2752" : "\u25a1"
                font.pointSize: 10
                onClicked: mdiWin.toggleMaximize()
            }

            // Hide
            Button {
                width: 22; height: 20
                text: "\u2715"
                font.pointSize: 10
                onClicked: mdiWin.visible = false
            }
        }

        MouseArea {
            id: dragArea
            anchors.fill: parent
            anchors.rightMargin: 50
            property real startX: 0
            property real startY: 0
            onPressed: {
                mdiWin.bringToFront()
                startX = mouseX; startY = mouseY
            }
            onPositionChanged: {
                if (!mdiWin.maximized) {
                    mdiWin.x += mouseX - startX
                    mdiWin.y += mouseY - startY
                }
            }
            onDoubleClicked: mdiWin.toggleMaximize()
        }
    }

    // Content
    Loader {
        id: contentLoader
        x: 0; y: titleBar.height
        width: mdiWin.width
        height: mdiWin.height - titleBar.height
        source: mdiWin.contentSource
    }

    // Resize handle (bottom-right)
    MouseArea {
        id: resizeHandle
        width: 14; height: 14
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
                mdiWin.width = Math.max(200, startW + global.x - globalStartX)
                mdiWin.height = Math.max(100, startH + global.y - globalStartY)
            }
        }

        // Visual grip
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
