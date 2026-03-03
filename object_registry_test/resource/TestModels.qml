import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    visible: true
    color: "#F0F0F0"
    anchors.fill: parent

    Rectangle {
        x: 10
        y: 10
        width: parent.width - 20
        height: 30
        color: "white"
        radius: 3

        Row {
            anchors.verticalCenter: parent.verticalCenter
            x: 8
            spacing: 8

            Text {
                text: "Create cont.:"
                font.pixelSize: 12
            }

            TextField {
                width: 100
                height: 24
                font.pixelSize: 12
                placeholderText: "from to"
                onAccepted: {
                    qmlInterface.callCmd("create_model_test " + text)
                    text = ""
                }
            }
        }
    }

    Column {
        x: 10
        y: 46
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

                        delegate: Text {
                            text: "" + testItem.value
                            font.pixelSize: 12
                            y: 2
                        }
                    }
                }
            }
        }
    }
}
