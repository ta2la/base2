import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    visible: true
    color: "#F0F0F0"
    anchors.fill: parent

    Column {
        x: 10
        y: 10
        spacing: 6
        width: parent.width - 20

        Repeater {
            model: testModel

            delegate: Rectangle {
                width: parent.width
                height: 30
                color: "#CAD7E8"
                radius: 3

                Row {
                    anchors.verticalCenter: parent.verticalCenter
                    x: 8
                    spacing: 8

                    Text {
                        text: descr + " ::"
                        font.pixelSize: 12
                        y: 2
                    }

                    Repeater {
                        model: subModel

                        delegate: Row {
                            spacing: 2
                            y: 2
                            Text {
                                text: "" + testItem.value
                                font.pixelSize: 12
                            }
                            Text {
                                text: "[" + testItem.ooId + "]"
                                font.pixelSize: 10
                                color: "#999999"
                            }
                        }
                    }
                }
            }
        }
    }
}
