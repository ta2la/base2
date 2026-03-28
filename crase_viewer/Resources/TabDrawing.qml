import QtQuick 2.15
import QtQuick.Controls 2.15

///@view:beg
Rectangle {
    anchors.fill: parent
    color: "#F0F0F0"
    clip: true

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

    Repeater {
        model: craseDrawingModel

        delegate: Rectangle {
            x: drawItem.posX
            y: drawItem.posY
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
                onClicked: qmlInterface.callCmd("crase_preview " + drawItem.itemId)
            }
        }
    }
}
///@view:end
