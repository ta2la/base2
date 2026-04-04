import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    anchors.fill: parent
    color: "#F4F4F0"

    Column {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            width: parent.width
            height: 40
            color: "#E0E0E0"

            Text {
                anchors.fill: parent
                anchors.leftMargin: 12
                text: "\u{1F516} Bookmarks"
                font.pointSize: 13
                font.bold: true
                color: "#404040"
                verticalAlignment: Text.AlignVCenter
            }
        }

        Rectangle { width: parent.width; height: 1; color: "#808080" }

        ListView {
            id: listView
            width: parent.width
            height: parent.height - 41
            clip: true
            model: bookmarkModel

            property int selectedIndex: -1

            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

            delegate: Item {
                width: listView.width
                height: 28

                Rectangle {
                    anchors.fill: parent
                    color: listView.selectedIndex === index ? "#C8D8E8"
                         : itemMouse.containsMouse ? "#E0E8F0" : "transparent"

                    Row {
                        anchors.fill: parent
                        anchors.leftMargin: 12
                        spacing: 8

                        Text {
                            text: fileData.isDir ? "\u{1F4C1}"
                                : fileData.role === 1 ? "\u{1F7E5}"
                                : "\u{1F4C4}"
                            width: 24
                            height: 28
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 14
                        }
                        Text {
                            text: fileData.name
                            height: 28
                            font.pointSize: 10
                            font.bold: fileData.isDir
                            color: "#404040"
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    MouseArea {
                        id: itemMouse
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            if (listView.selectedIndex === index) {
                                if (fileData.role === 1)
                                    qmlInterface.callCmd("md_load " + fileData.filePath)
                                listView.selectedIndex = -1
                            } else {
                                listView.selectedIndex = index
                            }
                        }
                    }
                }
            }
        }
    }
}
