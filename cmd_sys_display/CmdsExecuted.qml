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
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

/// @view:include #include "ExerecModelProxy.h"

Rectangle {
    id: root
    property var modelSource: exerecModelProxy

    ListView {
        x: 0;
        y: 10;
        width: root.width-20;
        height: root.height-170
        model: modelSource
        spacing: 2

        delegate: Rectangle {
            property var echoSet: ["log_arg_log"]

            id: rootDelegate
            width: root.width-40
            height: if (cmdText.implicitHeight >300) return 300;
                    else return cmdText.implicitHeight + 2 > 35 ? cmdText.implicitHeight + 2 : 35;

            color: {    if ( echoSet.indexOf(model.exerecData.cmd) === -1) {
                            if (model.exerecData.args.indexOf("--ERROR") !== -1) return "#e58aa9";
                            else if (model.exerecData.args.indexOf("--STDOUT") !== -1) return "#83BEEA";
                            else if (model.exerecData.args.indexOf("--EMPHASIZE") !== -1) return "#83BEEA";
                            else if (model.exerecData.args.indexOf("log_arg_message") !== -1) return "#FFFFC5";
                            else return "#CBD7E0";
                   }
                        else { return "white" }
                }
            border.color: "darkgray"

            Text {
                width: parent.width
                height: parent.height
                id: cmdText
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: model.exerecData.args
                font.pixelSize: 11
                wrapMode: Text.Wrap
                textFormat: Text.RichText
                horizontalAlignment: { if (parent.echoSet.indexOf(model.exerecData.cmd) === -1) {
                                         return Text.AlignLeft }
                                     else { return Text.AlignRight } }
                onLinkActivated: { qmlInterface.callCmd(link); }
                clip: true;
            }

            MouseArea { anchors.fill: parent; z: 1;
                onClicked:  {
                    popupCmd.jsonData = model.exerecData.args
                    popupCmd.cmdName  = model.cmdName
                    popupCmd.visible = true;
                }
            }
        }

        PopupCmd {
            visible: false
            id: popupCmd
        }
    }
}
