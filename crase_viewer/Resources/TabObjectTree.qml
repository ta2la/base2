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
                color: "#E0E0E0"

                Text {
                    anchors.fill: parent
                    anchors.leftMargin: 8
                    text: "Object Tree"
                    font.pointSize: 13
                    font.bold: true
                    color: "#404040"
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Rectangle {
                width: parent.width
                height: 1
                color: "#808080"
            }

            Repeater {
                model: craseTreeModel

                delegate: Rectangle {
                    width: parent.width
                    height: contentText.implicitHeight + 8
                    color: treeItem.selected ? "#F0C0D0" : itemMouse.containsMouse ? "#E0E8F0" : "transparent"

                    Text {
                        id: contentText
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        leftPadding: 8 + treeItem.level * 20
                        rightPadding: 8
                        wrapMode: Text.WordWrap
                        textFormat: Text.RichText
                        font.pointSize: 10
                        text: "<span style='font-size:14pt'>" + treeItem.icon + "</span> "
                              + (treeItem.prefix !== "" ? "<span style='color:#808080'>" + treeItem.prefix + "</span> " : "")
                              + "<span style='color:" + (treeItem.itemType === 1 ? "#606060" : "#404040") + "'>"
                              + treeItem.text + "</span>"
                    }

                    MouseArea {
                        id: itemMouse
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: treeItem.itemType === 0 ? Qt.PointingHandCursor : Qt.ArrowCursor
                        onClicked: {
                            if (treeItem.itemType === 0)
                                qmlInterface.callCmd("crase_expand " + treeItem.itemId)
                        }
                    }
                }
            }
        }
    }
}
///@view:end
