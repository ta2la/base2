import QtQuick 2.15
import QtQuick.Controls 2.15

///@view:beg
Rectangle {
    id: bg
    anchors.fill: parent
    color: "#F0F0F0"
    clip: true

    // transient pan offset (live during bg drag, committed via crase_pan on release)
    property real panDx: 0
    property real panDy: 0

    WheelHandler {
        target: bg
        onWheel: function(event) {
            var s = craseDrawingModel.scale
            var dx = (event.x - bg.panDx) / s + craseDrawingModel.originX
            var dy = (event.y - bg.panDy) / s + craseDrawingModel.originY
            var r = event.angleDelta.y > 0 ? 1.1 : 1.0/1.1
            qmlInterface.callCmd("crase_zoom " + dx + " " + dy + " --ratio " + r)
        }
    }

    MouseArea {
        id: bgPan
        anchors.fill: parent
        z: 0
        property real startX: 0
        property real startY: 0
        property real baseDx: 0
        property real baseDy: 0
        property bool dragging: false
        onPressed: {
            startX = mouseX; startY = mouseY
            baseDx = bg.panDx; baseDy = bg.panDy
            dragging = false
        }
        onPositionChanged: {
            if (pressed) {
                bg.panDx = baseDx + (mouseX - startX)
                bg.panDy = baseDy + (mouseY - startY)
                if (Math.abs(mouseX - startX) > 3 || Math.abs(mouseY - startY) > 3) dragging = true
                linesCanvas.requestPaint()
            }
        }
        onReleased: {
            if (dragging) {
                var s = craseDrawingModel.scale
                qmlInterface.callCmd("crase_pan " + (-bg.panDx/s) + " " + (-bg.panDy/s))
                bg.panDx = 0; bg.panDy = 0
            }
            dragging = false
        }
    }

    Canvas {
        id: gridCanvas
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

    Canvas {
        id: linesCanvas
        anchors.fill: parent
        z: 0

        Connections {
            target: craseDrawingModel
            function onLinesChanged() { linesCanvas.requestPaint() }
            function onTransformChanged() { linesCanvas.requestPaint() }
        }

        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)
            ctx.strokeStyle = "#808090"
            ctx.lineWidth = 1.5
            ctx.font = "10px sans-serif"
            ctx.fillStyle = "#606070"

            var ox = craseDrawingModel.originX
            var oy = craseDrawingModel.originY
            var s  = craseDrawingModel.scale
            var px = bg.panDx
            var py = bg.panDy

            var lines = craseDrawingModel.lines
            for (var i = 0; i < lines.length; i++) {
                var l = lines[i]
                var fx = (l.fromX - ox) * s + px, fy = (l.fromY - oy) * s + py
                var tx = (l.toX   - ox) * s + px, ty = (l.toY   - oy) * s + py

                ctx.beginPath()
                ctx.moveTo(fx, fy)
                ctx.lineTo(tx, ty)
                ctx.stroke()

                // names at 1/3 and 2/3 of the line
                var dx = tx - fx, dy = ty - fy
                ctx.fillText(l.name1, fx + dx/3, fy + dy/3 - 4)
                ctx.fillText(l.name2, tx - dx/3, ty - dy/3 - 4)
            }
        }
    }

    Repeater {
        model: craseDrawingModel

        delegate: Item {
            id: itemRoot
            x: (drawItem.posX - craseDrawingModel.originX) * craseDrawingModel.scale + bg.panDx
            y: (drawItem.posY - craseDrawingModel.originY) * craseDrawingModel.scale + bg.panDy
            width: itemBox.width
            height: itemBox.height
            z: 1

            // wire-frame rectangle (visible only when wh is set, scaled)
            Rectangle {
                id: wfBox
                visible: drawItem.posW > 0 && drawItem.posH > 0
                property real dw: 0
                property real dh: 0
                width:  (drawItem.posW + dw) * craseDrawingModel.scale
                height: (drawItem.posH + dh) * craseDrawingModel.scale
                color: "transparent"
                border.color: "#A0A8B0"
                border.width: 1

                HoverHandler { id: wfHover }

                Rectangle {
                    id: resizeHandle
                    visible: wfHover.hovered || resizeMouse.pressed
                    width: 12
                    height: 12
                    color: "#808890"
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom

                    MouseArea {
                        id: resizeMouse
                        anchors.fill: parent
                        cursorShape: Qt.SizeFDiagCursor
                        property real startX: 0
                        property real startY: 0
                        onPressed: {
                            var p = mapToItem(itemRoot, mouse.x, mouse.y)
                            startX = p.x; startY = p.y
                            wfBox.dw = 0; wfBox.dh = 0
                        }
                        onPositionChanged: if (pressed) {
                            var s = craseDrawingModel.scale
                            var p = mapToItem(itemRoot, mouse.x, mouse.y)
                            wfBox.dw = Math.max(20 - drawItem.posW, (p.x - startX) / s)
                            wfBox.dh = Math.max(20 - drawItem.posH, (p.y - startY) / s)
                        }
                        onReleased: {
                            var w = Math.round(drawItem.posW + wfBox.dw)
                            var h = Math.round(drawItem.posH + wfBox.dh)
                            wfBox.dw = 0; wfBox.dh = 0
                            qmlInterface.callCmd("crase_resize " + drawItem.itemId + " " + drawItem.relTypeId + " " + w + " " + h)
                        }
                    }
                }
            }

            Rectangle {
                id: itemBox
                width: itemRow.implicitWidth + 12
                height: 28
                radius: 4
                color: itemMouse.containsMouse ? "#D0D8E0" : "#E8ECF0"
                border.color: "#A0A8B0"
                border.width: 1

                Row {
                    id: itemRow
                    anchors.centerIn: parent
                    spacing: 4

                    Text {
                        text: drawItem.icon
                        font.pointSize: 12
                        verticalAlignment: Text.AlignVCenter
                    }
                    Text {
                        text: drawItem.text
                        font.pointSize: 10
                        color: "#404040"
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                MouseArea {
                    id: itemMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    drag.target: itemRoot
                    drag.threshold: 3
                    property bool dragged: false
                    onPositionChanged: if (drag.active) dragged = true
                    onReleased: {
                        if (dragged) {
                            var s = craseDrawingModel.scale
                            var ax = Math.round((itemRoot.x - bg.panDx) / s + craseDrawingModel.originX)
                            var ay = Math.round((itemRoot.y - bg.panDy) / s + craseDrawingModel.originY)
                            qmlInterface.callCmd("crase_drag " + drawItem.itemId + " " + drawItem.relTypeId + " " + ax + " " + ay)
                            dragged = false
                        } else {
                            qmlInterface.callCmd("crase_preview " + drawItem.itemId)
                        }
                    }
                }
            }
        }
    }
}
///@view:end
