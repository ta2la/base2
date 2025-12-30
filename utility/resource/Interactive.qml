import QtQuick 2.7
import QtQuick.Controls 2.4

///@view:beg

/// @view:include #include "ModelControls.h"

Rectangle {
    visible: true
    Rectangle {
        x: 10
        y: 10
        visible: true
        width: parent.width-20
        height: 30
        color: "transparent"

        Row {
            anchors.top: parent.top
            height: 40

                property string selb: "Code Focus"

                spacing: 10
                x: 0; y: 10; width: root.width-20;
                Button { text: "Code Focus";  height: parent.selb===text? 32:18;
                          onClicked: { parent.selb = text; qmlInterface.callCmd("change_controls start_stop");}}
                Button { text: "Disable Cmd"; height: parent.selb===text? 32:18;
                          onClicked: { parent.selb = text; qmlInterface.callCmd("change_controls disabled");}}
                Button { text: "Test Cmd";    height: parent.selb===text? 32:18;
                          onClicked: { parent.selb = text; qmlInterface.callCmd("change_controls test");}}
            }
        }

        Rectangle {
            x: 10
            y: 40
            visible: true
            width: parent.width-20
            height: 120
            color: "#CAD7E8"

            Flow {
                width: parent.width
                height: 90
                spacing: 10

                Repeater {
                model: cmdline
                delegate: Loader {
                    id: loader
                    sourceComponent: {
                        if (rowType === 0)
                            return textComponent;
                        else if (rowType === 1)
                            return editComponent;
                        else if (rowType === 2)
                            return buttonComponent;
                    }

                    onLoaded: {
                        item.content = content;
                        if (rowType === 2) // ButtonType
                                item.command = model.command
                    }
                }
            }}

            Component {
                id: textComponent
                Text {
                    y:6
                    height: 26
                    property string content: ""
                    text: content
                }
            }

            Component {
                id: editComponent
                TextField {
                    height: 26
                    property string content: ""
                    text: content
                }
            }

            Component {
                id: buttonComponent
                Button {
                    height: 26
                    property string content: ""
                    property string command: ""
                    onClicked: { interactiveIface.callCmd(command);}
                    text: content
                }
             }
         }

    InteractiveOutput {
        color: "#EEEEFF"
        visible: true;
        x: 10
        y: 170
        height: parent.height-180;
        width: parent.width-20;
    }
}
///@view:end
