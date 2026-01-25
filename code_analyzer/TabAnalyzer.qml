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

///@view:beg
ScrollView {
    anchors.fill: parent
    clip: true

   /* contentItem: Flickable {
            contentWidth: contentRect.width
            contentHeight: contentRect.implicitHeight
            clip: true*/

    Rectangle {
        id: contentRoot
        color: "#F0F0F0"

        width: parent.width//ScrollView.view.width        // <-- FIX
        implicitHeight: contentColumn.implicitHeight
        //implicitHeight: Math.max(contentColumn.implicitHeight, 2000)

        //anchors.margins: 10
        //padding: 10

        Column {
            id: contentColumn
            width: parent.width

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
                            font.weight: Font.DemiBold
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

                            delegate: Row {
                                width: parent.width
                                height: 16
                                spacing: 8

                                Text {
                                    width: 50
                                    horizontalAlignment: Text.AlignRight
                                    //text: fileData.dist.toFixed(1)
                                    font.pointSize: 9
                                    font.bold: isFinite(fileData.dist) && fileData.dist >= 0
                                    color: "#606060"
                                    verticalAlignment: Text.AlignVCenter
                                    text: {
                                        if (fileData.dist === Infinity)
                                            return "- ";
                                        else
                                            return fileData.dist.toFixed(1);
                                    }
                                }

                                Rectangle {
                                    width: parent.width - 50 - 25
                                    height: parent.height
                                    color: fileData.center ? "#E6C1D8" : "transparent"

                                    Text {
                                        anchors.fill: parent
                                        leftPadding: 2
                                        textFormat: Text.RichText
                                        text: {
                                            var t = fileData.types
                                            if (!t || t.length === 0) return "• " + fileData.name
                                            return "• " + fileData.name + " <span style='font-weight:600'>[" + t.join(", ") + "]</span>"
                                            }
                                        font.pointSize: 10
                                        color: "#404040"
                                        elide: Text.ElideRight
                                        verticalAlignment: Text.AlignVCenter
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor

                                        preventStealing: true

                                        onClicked: {
                                            interactiveIface.callCmd(
                                                "analyzer_set_center " + fileData.name + " " + fileData.module
                                            )
                                        }
                                    }
                                }
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
}
//}

///@view:end
