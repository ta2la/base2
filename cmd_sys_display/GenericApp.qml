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
import QtQuick 2.7
import QtQuick.Controls 2.4

///@view:beg

Rectangle  {
    id: root;
    color: "#D9D9D9";
    anchors.fill: parent;

    function activateTab(tabName) {
        tabCmdlineEx.visible   = (tabCmdlineEx.objectName    === tabName) ? true : false;
        tabInteractive.visible = (tabInteractive.objectName    === tabName) ? true : false;
        tabAbout.visible       = (tabAbout.objectName   === tabName) ? true : false;
        tabAnalyzer.visible    = (tabAnalyzer.objectName === tabName)
    }

    Rectangle { id: mainSpace; color: "#4cc4bc"
        x: 0; y: 0; width: root.width; height: 40

        Image {x:2; y:2; width:32; height:32; source:"images/icon.svg"}
        Text {x:40; y: 8; font.pointSize: 14; font.bold: true; text: "PROMPT Assembler" }

        Button { id: buttonCmdlineEx;   x:260;       y:10; width: 110; height: 30;
                 text: "Executed"; onClicked: activateTab("tabCmdlineEx") }
        Button { id: buttonInteractive; x:260+120;   y:10; width: 110; height: 30;
                 text: "Cmdline";  onClicked: activateTab("tabInteractive") }
        Button { id: buttonAbout;       x:260+120*2; y:10; width: 110; height: 30;
                 text: "About";    onClicked: {activateTab("tabAbout") } }
        Button {
            id: buttonAnalyzer
            x: 260 + 120*3
            y: 10
            width: 110
            height: 30
            text: "Analyzer"
            onClicked: activateTab("tabAnalyzer")
        }
    }

    Rectangle { objectName: "tabCmdlineEx"; id: tabCmdlineEx
        visible: false
        color: "#F0F0F0"
        x: 10; y: 50; width: root.width-20; height: root.height-60

        Button { x:10; y: 10; text: "STOP CMDS REC";
            onClicked: { interactiveIface.callCmd("cmds_stop_record"); }
        }

        CmdsExecuted {
            x: 10;
            y: 40;
            width: root.width-20;
            height: root.height-170
        }
    }

    Interactive { objectName: "tabInteractive"; id: tabInteractive
        visible: true;
        x: 10; y: 50; width: root.width-20; height: root.height-60
    }

    Rectangle { objectName: "tabAbout"; id: tabAbout

        visible: false;
        x: 10; y: 50; width: root.width-20; height: root.height-60

        Text {
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width - 20
            wrapMode: Text.WordWrap
            textFormat: Text.RichText
            text: "<p><b>PROMPT Assembler</b></p>" +
            "<p>Copyright 2025 Pert Talla. All right reserved.</p>"
        }
    }

    Rectangle {
        objectName: "tabAnalyzer"
        id: tabAnalyzer

        visible: false
        x: 10; y: 50
        width: root.width - 20
        height: root.height - 60
        color: "#F0F0F0"

        Column {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 4

            Repeater {
                model: analyzerModules

                delegate: Text {
                    text: moduleData.module
                    font.pointSize: 12
                }
            }
        }
    }
}

///@view:end

