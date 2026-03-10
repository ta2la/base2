import QtQuick 2.15
import QtQuick.Controls 2.15

///@view:beg

/// @view:include #include "ModelControls.h"

Rectangle {
    visible: true
    anchors.fill: parent

    // Tab buttons
    Rectangle {
        id: tabBar
        x: 10; y: 10
        width: parent.width-20; height: 30
        color: "transparent"

        Row {
            anchors.top: parent.top
            height: 40
            property string selb: "Code Focus"
            spacing: 10
            x: 0; y: 10; width: root.width-20;
            Button { text: "Windows";     height: parent.selb===text? 32:18;
                      onClicked: { parent.selb = text; }}
            Button { text: "Code Focus";  height: parent.selb===text? 32:18;
                      onClicked: { parent.selb = text; qmlInterface.callCmd("change_controls start_stop");}}
            Button { text: "Test Cmd";    height: parent.selb===text? 32:18;
                      onClicked: { parent.selb = text; qmlInterface.callCmd("change_controls test");}}
            Button { text: "Disable Cmd"; height: parent.selb===text? 32:18;
                      onClicked: { parent.selb = text; qmlInterface.callCmd("change_controls disabled");}}
            Button { text: "Oreg Test";   height: parent.selb===text? 32:18;
                      onClicked: { parent.selb = text; qmlInterface.callCmd("change_controls oreg_test");}}
        }
    }

    // Windows toggle area
    Rectangle {
        id: windowsArea
        x: 10; y: 44
        width: parent.width-20
        height: (parent.height - 54) * 0.66
        color: "#D8DDE8"
        clip: true
        visible: tabBar.children[0].selb === "Windows"

        Flow {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 5

            Repeater {
                model: mainTabs
                delegate: Rectangle {
                    visible: index > 0
                    width: visible ? 100 : 0; height: visible ? 30 : 0
                    radius: 2; color: "#455C71"
                    Text {
                        anchors.centerIn: parent
                        text: index === 1 ? "\u2630 " + model.text : index === 2 ? "\uD83D\uDF09 " + model.text : index === 3 ? "\u00B6 " + model.text : index === 4 ? "\uD83D\uDDC1 " + model.text : index === 5 ? "\u2699 " + model.text : index === 6 ? "\u24D8 " + model.text : model.text
                        font.pointSize: 10; font.bold: true; color: "#fff"
                    }
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: root.toggleWindow(index)
                    }
                }
            }
        }
    }

    // Command buttons area (upper 2/3)
    Rectangle {
        id: cmdArea
        x: 10; y: 44
        width: parent.width-20
        height: (parent.height - 54) * 0.66
        color: "#CAD7E8"
        clip: true
        visible: tabBar.children[0].selb !== "Windows"

        Flickable {
            anchors.fill: parent
            contentHeight: cmdFlow.implicitHeight
            flickableDirection: Flickable.VerticalFlick
            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

            Flow {
                id: cmdFlow
                width: parent.width - 15
                spacing: 10

                Repeater {
                    model: cmdline
                    delegate: Loader {
                        sourceComponent: {
                            if (rowType === 0) return textComponent;
                            else if (rowType === 1) return editComponent;
                            else if (rowType === 2) return buttonComponent;
                        }
                        onLoaded: {
                            item.content = content;
                            if (rowType === 1 || rowType === 2)
                                item.command = model.command
                        }
                    }
                }
            }
        }
    }

    // Tool Settings area (lower 1/3)
    Rectangle {
        id: toolArea
        x: 10; y: cmdArea.y + cmdArea.height + 4
        width: parent.width-20
        height: parent.height - y - 10
        color: "#D8E8D8"
        clip: true

        Flickable {
            anchors.fill: parent
            contentHeight: toolFlow.implicitHeight
            flickableDirection: Flickable.VerticalFlick
            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

            Flow {
                id: toolFlow
                width: parent.width - 15
                spacing: 10

                Repeater {
                    model: toolControls
                    delegate: Loader {
                        sourceComponent: {
                            if (rowType === 0) return toolTextComp;
                            else if (rowType === 1) return toolEditComp;
                            else if (rowType === 2) return toolButtonComp;
                        }
                        onLoaded: {
                            item.content = content;
                            if (rowType === 1 || rowType === 2)
                                item.command = model.command
                        }
                    }
                }
            }
        }
    }

    // Components for cmdline
    Component {
        id: textComponent
        Text { y:6; height: 26; property string content: ""; text: content }
    }
    Component {
        id: editComponent
        TextField {
            height: 26; property string content: ""; property string command: ""
            placeholderText: content
            onAccepted: { qmlInterface.callCmd(command.replace("$text", text)); text = "" }
        }
    }
    Component {
        id: buttonComponent
        Button {
            height: 26; property string content: ""; property string command: ""
            onClicked: { qmlInterface.callCmd(command); }
            text: content
        }
    }

    // Components for toolControls
    Component {
        id: toolTextComp
        Text { y:6; height: 26; property string content: ""; text: content; font.italic: true }
    }
    Component {
        id: toolEditComp
        TextField {
            height: 26; property string content: ""; property string command: ""
            text: content
            onAccepted: { qmlInterface.callCmd(command.replace("$text", text)); text = content }
        }
    }
    Component {
        id: toolButtonComp
        Button {
            height: 26; property string content: ""; property string command: ""
            onClicked: { qmlInterface.callCmd(command); }
            text: content
        }
    }
}
///@view:end
