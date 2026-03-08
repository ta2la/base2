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
            spacing: 0

            Rectangle {
                width: parent.width
                height: 40
                color: headerMouse.containsMouse ? "#D0D0D0" : "#E0E0E0"

                Text {
                    anchors.fill: parent
                    anchors.leftMargin: 12
                    text: dirModel.dir || "(no directory set)"
                    font.pointSize: 13
                    font.bold: true
                    color: "#404040"
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideMiddle
                }

                MouseArea {
                    id: headerMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        interactiveIface.callCmd(
                            "set_dir " + dirModel.dir + "/.."
                        )
                    }
                }
            }

            Rectangle {
                width: parent.width
                height: 1
                color: "#808080"
            }

            Repeater {
                model: dirModel

                delegate: Rectangle {
                    width: parent.width
                    height: 24
                    color: mouseArea.containsMouse ? "#E0E8F0" : "#FFFFFF"

                    Row {
                        anchors.fill: parent
                        anchors.leftMargin: 8
                        spacing: 8

                        Text {
                            text: fileData.isDir ? "\u{1F4C1}" : "\u{1F4C4}"
                            width: 24
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 14
                        }

                        Text {
                            text: fileData.name
                            font.pointSize: 10
                            font.bold: fileData.isDir
                            color: "#404040"
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                            width: parent.width - 100
                        }

                        Text {
                            text: fileData.isDir ? "" : (fileData.size / 1024).toFixed(1) + " kB"
                            width: 60
                            horizontalAlignment: Text.AlignRight
                            font.pointSize: 9
                            color: "#808080"
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor

                        onClicked: {
                            if (fileData.isDir) {
                                interactiveIface.callCmd(
                                    "set_dir " + fileData.filePath
                                )
                            } else {
                                interactiveIface.callCmd(
                                    "system_open_path " + fileData.filePath
                                )
                            }
                        }
                    }
                }
            }
        }
    }
}
///@view:end
