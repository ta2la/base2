import QtQuick 2.15
import QtQuick.Controls 2.15

///@view:beg
Item {
    anchors.fill: parent

    // --- directory listing ---
    ScrollView {
        anchors.fill: parent
        clip: true
        visible: previewModel.level === 2 && previewModel.isDir

        Column {
            width: parent.width
            spacing: 0

            Rectangle {
                width: parent.width
                height: 40
                color: "#E0E0E0"
                Text {
                    anchors.fill: parent
                    anchors.leftMargin: 12
                    text: previewModel.filePath
                    font.pointSize: 13
                    font.bold: true
                    color: "#404040"
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideMiddle
                }
            }

            Rectangle { width: parent.width; height: 1; color: "#808080" }

            Repeater {
                model: previewDirModel

                delegate: Rectangle {
                    width: parent.width
                    height: 24
                    color: pdMouse.containsMouse ? "#E0E8F0" : "#FFFFFF"

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
                        id: pdMouse
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            qmlInterface.callCmd("file_preview " + fileData.filePath)
                        }
                    }
                }
            }
        }
    }

    // --- image ---
    ScrollView {
        anchors.fill: parent
        clip: true
        visible: previewModel.level === 2 && previewModel.isImage

        Image {
            source: previewModel.isImage ? "file:///" + previewModel.filePath : ""
            fillMode: Image.PreserveAspectFit
        }
    }

    // --- text ---
    ScrollView {
        anchors.fill: parent
        clip: true
        visible: previewModel.level === 2 && !previewModel.isDir && !previewModel.isImage

        TextArea {
            text: previewModel.content
            readOnly: true
            font.family: "Consolas"
            font.pointSize: 10
            wrapMode: TextEdit.NoWrap
            color: "#303030"
            background: Rectangle { color: "#FAFAFA" }
        }
    }
}
///@view:end
