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
                    text: "Attr Types"
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
                model: craseAttrTypesModel

                delegate: Rectangle {
                    width: parent.width
                    height: 28
                    color: itemMouse.containsMouse ? "#E0E8F0" : "transparent"

                    Row {
                        anchors.verticalCenter: parent.verticalCenter
                        leftPadding: 8
                        spacing: 6

                        Text {
                            text: attrTypeItem.icon
                            height: 28
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 14
                        }
                        Text {
                            text: attrTypeItem.type
                            height: 28
                            font.pointSize: 10
                            font.bold: true
                            color: "#404040"
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    MouseArea {
                        id: itemMouse
                        anchors.fill: parent
                        hoverEnabled: true
                    }
                }
            }
        }
    }
}
///@view:end
