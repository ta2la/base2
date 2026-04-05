import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt.labs.folderlistmodel 2.15

Rectangle {
    anchors.fill: parent
    color: "#F4F4F0"

    FolderListModel {
        id: bookFiles
        folder: "file://" + configDir
        nameFilters: ["*.book"]
        showDirs: false
    }

    Column {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: header
            width: parent.width
            height: 40
            color: "#E0E0E0"

            property string currentBook: ""

            Row {
                anchors.fill: parent
                anchors.leftMargin: 8
                spacing: 4

                Repeater {
                    model: bookFiles
                    delegate: Rectangle {
                        width: tabText.implicitWidth + 16
                        height: 30
                        anchors.verticalCenter: parent.verticalCenter
                        radius: 3
                        color: header.currentBook === fileName
                             ? "#4A7A5A" : tabMouse.containsMouse ? "#C8C8C8" : "#D8D8D8"

                        Text {
                            id: tabText
                            anchors.centerIn: parent
                            text: fileBaseName
                            font.pointSize: 11
                            font.bold: true
                            color: header.currentBook === fileName ? "#F0F0D0" : "#404040"
                        }

                        MouseArea {
                            id: tabMouse
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                header.currentBook = fileName
                                bookmarkModel.selectedIndex = -1
                                qmlInterface.callCmd("bookmark_set_file " + configDir + "/" + fileName)
                            }
                        }
                    }
                }
            }
        }

        Rectangle { width: parent.width; height: 1; color: "#808080" }

        ListView {
            id: listView
            width: parent.width
            height: parent.height - 41
            clip: true
            model: bookmarkModel

            property int selectedIndex: bookmarkModel.selectedIndex

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
                        anchors.leftMargin: 12 + fileData.level * 20
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
                            text: fileData.name + "  (" + fileData.filePath.substring(0, fileData.filePath.lastIndexOf("/")) + ")"
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
                                bookmarkModel.selectedIndex = -1
                            } else {
                                bookmarkModel.selectedIndex = index
                            }
                        }
                    }
                }
            }
        }
    }
}
