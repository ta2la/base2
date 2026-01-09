import QtQuick 2.15
import QtQuick.Controls 2.15

/// @view:include #include "InteractiveOutputModel"

/// @view:beg
Rectangle {
    color: "#F0F0F0"
    clip: true
    ListView {
        width: parent.width; height: parent.height
        model: interactiveOutput
        spacing: 2
        delegate: Item {
            height: jobText.implicitHeight + 2;
            Rectangle {
                width: root.width-40
                height: jobText.implicitHeight + 2;
                color: "#CBD7E0"
                border.color: "darkgray"

                Text {
                    id: jobText
                    x: 10
                    width: parent.width-50
                    text: model.item_string
                    font.pixelSize: 11
                    wrapMode: Text.Wrap
                    textFormat: Text.RichText

                    onLinkActivated: { interactiveIface.callCmd(link); }
                }
            }
        }
    }
}

/// @view:end
