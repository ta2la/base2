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

///@view:beg

Rectangle {
    color: "#F0F0F0"
    anchors.fill: parent

    Column {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 0

        Repeater {
            model: analyzerModules

            delegate: Column {
                width: parent.width
                spacing: 0

                Text {
                    text: moduleData.dirPathModuleLess +
                        " / " +
                        "<a href='system_open_path " + moduleData.dirPath + "'>" +
                        moduleData.module +
                        "</a>"
                    textFormat: Text.RichText
                    font.pointSize: 9
                    color: "#808080"
                    height: 20
                    verticalAlignment: Text.AlignVCenter

                    onLinkActivated: interactiveIface.callCmd(link)
                }

                Row { //**
                    width: parent.width
                    height: 25
                    spacing: 8

                    CheckBox {
                        anchors.verticalCenter: parent.verticalCenter
                        checked: moduleData.used

                        onToggled: {
                            interactiveIface.callCmd(
                                "set_module_used " + index + " " + (checked ? "1" : "0")
                            )
                        }
                    }

                    Item { width: 1; height: 1 }   // spacer

                    Text {
                        text: moduleData.module
                        font.pointSize: 12
                        verticalAlignment: Text.AlignVCenter
                        height: parent.height
                    }
                }

                // ---- files list (NEW)
                Column {
                    width: parent.width
                    spacing: 2
                    leftPadding: 26    // visual nesting under module

                    Repeater {
                        model: moduleData.files

                        delegate: Text {
                            text: "• " + fileName
                            font.pointSize: 10
                            color: "#404040"
                            height: 16
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }

                // vodorovná čára za každým řádkem
                Rectangle { //**
                    width: parent.width
                    height: 1
                    color: "#808080"
                }
            }
        }
    }
}

///@view:end
