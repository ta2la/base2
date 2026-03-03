import QtQuick 2.15
import QtQuick.Controls 2.15

Column {
    spacing: 4
    anchors.fill: parent

    Repeater {
        model: testModelCol

        delegate: Row {
            spacing: 6

            Repeater {
                model: testModel

                delegate: Text {
                    text: testItem.value
                    font.pointSize: 11
                }
            }
        }
    }
}
