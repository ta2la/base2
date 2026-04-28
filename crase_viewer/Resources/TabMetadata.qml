import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

///@view:beg
Rectangle {
    anchors.fill: parent
    color: "#F0F0F0"

    Row {
        id: tabBar
        x: 5; y: 5
        spacing: 5
        property string current: "Types"

        Repeater {
            model: ["Types", "AttrTypes", "RelTypes"]
            delegate: Rectangle {
                width: 80; height: 24; radius: 2
                color: tabBar.current === modelData ? "#455C71" : "#B0B8C0"
                Text { anchors.centerIn: parent; text: modelData; font.pointSize: 9; font.bold: true; color: "#fff" }
                MouseArea { anchors.fill: parent; cursorShape: Qt.PointingHandCursor; onClicked: tabBar.current = modelData }
            }
        }
    }

    ScrollView {
        x: 0; y: 34
        width: parent.width
        height: parent.height - 34
        clip: true
        visible: tabBar.current === "Types"

        Column {
            width: parent.width
            spacing: 0
            Repeater {
                model: craseTypesModel
                delegate: Rectangle {
                    width: parent.width; height: 28
                    color: m1.containsMouse ? "#E0E8F0" : "transparent"
                    Row {
                        anchors.verticalCenter: parent.verticalCenter; leftPadding: 8; spacing: 6
                        Text { text: "[" + typeItem.itemId + "]"; height: 28; font.pointSize: 10; color: "#808080"; verticalAlignment: Text.AlignVCenter }
                        Text { text: typeItem.icon; height: 28; verticalAlignment: Text.AlignVCenter; font.pointSize: 14 }
                        Text { text: typeItem.type; height: 28; font.pointSize: 10; font.bold: true; color: "#404040"; verticalAlignment: Text.AlignVCenter }
                        Text { visible: typeItem.attrIcons !== ""; text: typeItem.attrIcons; height: 28; font.pointSize: 12; color: "#808080"; verticalAlignment: Text.AlignVCenter }
                    }
                    MouseArea { id: m1; anchors.fill: parent; hoverEnabled: true }
                }
            }
        }
    }

    ScrollView {
        x: 0; y: 34
        width: parent.width
        height: parent.height - 34
        clip: true
        visible: tabBar.current === "AttrTypes"

        Column {
            width: parent.width
            spacing: 0
            Repeater {
                model: craseAttrTypesModel
                delegate: Rectangle {
                    width: parent.width; height: 28
                    color: m2.containsMouse ? "#E0E8F0" : "transparent"
                    Row {
                        anchors.verticalCenter: parent.verticalCenter; leftPadding: 8; spacing: 6
                        Text { text: "[" + attrTypeItem.itemId + "]"; height: 28; font.pointSize: 10; color: "#808080"; verticalAlignment: Text.AlignVCenter }
                        Text { text: attrTypeItem.icon; height: 28; verticalAlignment: Text.AlignVCenter; font.pointSize: 14 }
                        Text { text: attrTypeItem.type; height: 28; font.pointSize: 10; font.bold: true; color: "#404040"; verticalAlignment: Text.AlignVCenter }
                    }
                    MouseArea { id: m2; anchors.fill: parent; hoverEnabled: true }
                }
            }
        }
    }

    ScrollView {
        x: 0; y: 34
        width: parent.width
        height: parent.height - 34
        clip: true
        visible: tabBar.current === "RelTypes"

        Column {
            width: parent.width
            spacing: 0
            Repeater {
                model: craseRelTypesModel
                delegate: Rectangle {
                    width: parent.width; height: 28
                    color: m3.containsMouse ? "#E0E8F0" : "transparent"
                    Row {
                        anchors.verticalCenter: parent.verticalCenter; leftPadding: 8; spacing: 6
                        Text { text: "[" + relTypeItem.itemId + "]"; height: 28; font.pointSize: 10; color: "#808080"; verticalAlignment: Text.AlignVCenter }
                        Text { text: relTypeItem.icon1; height: 28; verticalAlignment: Text.AlignVCenter; font.pointSize: 14 }
                        Text { text: relTypeItem.name1 + "-" + relTypeItem.name2; height: 28; font.pointSize: 10; font.bold: true; color: "#404040"; verticalAlignment: Text.AlignVCenter }
                        Text { text: relTypeItem.icon2; height: 28; verticalAlignment: Text.AlignVCenter; font.pointSize: 14 }
                    }
                    MouseArea { id: m3; anchors.fill: parent; hoverEnabled: true }
                }
            }
        }
    }
}
///@view:end
