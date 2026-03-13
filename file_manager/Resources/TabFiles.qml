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

        Menu {
            id: contextMenu
            property string filePath: ""

            MenuItem {
                text: "Copy path"
                onTriggered: {
                    qmlInterface.callCmd(
                        "text_to_clipboard " + contextMenu.filePath
                    )
                }
            }
        }

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
                    textFormat: Text.RichText
                    text: (dirModel.dir || "(no directory set)") + (dirModel.repoName ? "   <span style='background-color:#F0E8A0;'>[" + dirModel.repoName + "]</span>" : "")
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
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    cursorShape: Qt.PointingHandCursor
                    onClicked: function(mouse) {
                        if (mouse.button === Qt.RightButton) {
                            contextMenu.filePath = dirModel.dir
                            contextMenu.popup()
                        } else {
                            qmlInterface.callCmd(
                                "set_dir " + dirModel.dir + "/.."
                            )
                        }
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

                delegate: Row {
                    width: parent.width
                    height: 24
                    leftPadding: 8
                    spacing: 4

                    Repeater {
                        model: groupModel
                        delegate: Rectangle {
                            property bool isPreviewed: fileData.filePath === previewModel.filePath
                            width: itemRow.implicitWidth + 8
                            height: 24
                            color: itemMouse.containsMouse ? "#E0E8F0" : isPreviewed && previewModel.level === 2 ? "#F0C0C0" : isPreviewed && previewModel.level === 1 ? "#F0DADA" : "transparent"

                            Row {
                                id: itemRow
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.leftMargin: 4
                                spacing: 4

                                Text {
                                    text: fileData.role === 1 ? "\u{1F3DB}"
                                        : fileData.role === 2 ? "\u{1F4DA}"
                                        : fileData.role === 3 ? "\u{1F9C3}"
                                        : fileData.role === 4 ? "\u{1F530}"
                                        : fileData.role === 5 ? "\u{1F7E6}"
                                        : fileData.role === 6 ? "\u{1F537}"
                                        : fileData.role === 7 ? "\u{25CF}"
                                        : fileData.role === 8 ? "\u{1F7E9}"
                                        : fileData.role === 9 ? "\u{1F50B}"
                                        : fileData.role === 10 ? "\u{1F539}"
                                        : fileData.isDir ? "\u{1F5C1}" : "\u{1F4C4}"
                                    height: 24
                                    verticalAlignment: Text.AlignVCenter
                                    font.pointSize: 14
                                    color: fileData.role === 7 ? "green" : "#000000"
                                }
                                Text {
                                    text: fileData.name
                                    height: 24
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
                                acceptedButtons: Qt.LeftButton | Qt.RightButton
                                cursorShape: Qt.PointingHandCursor

                                onClicked: function(mouse) {
                                    if (mouse.button === Qt.RightButton) {
                                        contextMenu.filePath = fileData.filePath
                                        contextMenu.popup()
                                    } else if (fileData.isDir && isPreviewed) {
                                        qmlInterface.callCmd(
                                            "set_dir " + fileData.filePath
                                        )
                                    } else {
                                        qmlInterface.callCmd(
                                            "file_preview " + fileData.filePath
                                        )
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
///@view:end
