//
// Copyright (C) 2025 Petr Talla. [petr.talla@gmail.com]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//		      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//=============================================================================
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 2.4

Rectangle {
    id: overlappingPopup
    anchors.centerIn: parent
    width: parent.width-40
    height: parent.height-60
    visible: true
    border.color: "#B0C0DC"
    border.width: 2

    property alias jsonData: jsonRect.jsonData
    property var cmdName

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Row { height: 30;
            Layout.preferredHeight: 30
            Layout.fillWidth: true
            visible: true
            Button {
                   text: "Copy json to Clipboard"
                   onClicked: qmlInterface.callCmd("copy --text \"" + overlappingPopup.jsonData + "\"")
               }

            Button {
                   text: "Close"
                   onClicked: overlappingPopup.visible = false
            }

            Button {
                   text: "Hide"
                   onClicked: qmlInterface.callCmd("exerec_add_filterout_command " + cmdName)
            }
        }

        Rectangle {
            id: jsonRect
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: true;

            property alias jsonData: jsonText.text

            Text {
                id: jsonText
                anchors.fill: parent
                text: "undefined aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                font.pixelSize: 12
                wrapMode: Text.WrapAnywhere
                color: "black"
                clip: true
                visible: true
            }
        }
    }
}

