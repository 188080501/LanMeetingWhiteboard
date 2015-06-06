/*
 * QML Material - An application framework implementing Material Design.
 * Copyright (C) 2014 Jordan Neidlinger
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 2.1 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls 1.2 as Controls
import QtQuick.Controls.Styles 1.2
import "../Element"

Controls.CheckBox {
    id: checkBox

    /*!
       The checkbox color. By default this is the app's accent color
     */
    property color color: darkBackground ? materialUI.dark.accentColor : materialUI.light.accentColor

    /*!
       Set to \c true if the checkbox is on a dark background
     */
    property bool darkBackground

    style: CheckBoxStyle {
        id: checkboxStyle

        spacing: materialUI.dp(2)

        label: Item {
            implicitWidth: text.implicitWidth + 2
            implicitHeight: text.implicitHeight

            baselineOffset: text.baselineOffset

            Label {
                id: text
                anchors.centerIn: parent
                color: control.enabled ? control.darkBackground ? materialUI.dark.textColor
                                                                : materialUI.light.textColor
                                       : control.darkBackground ? materialUI.alpha("#fff", 0.30)
                                                                : materialUI.alpha("#000", 0.26)
                text: control.text
            }
        }

        indicator: Item {
            id: parentRect

            implicitWidth: materialUI.dp(54)
            implicitHeight: materialUI.dp(54)

            Rectangle {
                id: indicatorRect

                anchors.centerIn: parent

                property color __internalColor: control.enabled ? control.color
                                                                : control.darkBackground ? materialUI.alpha("#fff", 0.30)
                                                                                         : materialUI.alpha("#000", 0.26)

                width: materialUI.dp(24)
                height: materialUI.dp(24)
                radius: materialUI.dp(2)

                border.width: materialUI.dp(2)

                border.color: control.enabled ? control.checked ? control.color
                                                                : control.darkBackground ? materialUI.alpha("#fff", 0.70)
                                                                                         : materialUI.alpha("#000", 0.54)
                                              : control.darkBackground ? materialUI.alpha("#fff", 0.30)
                                                                       : materialUI.alpha("#000", 0.26)

                color: control.checked ? __internalColor : "transparent"

                Behavior on color {
                    ColorAnimation {
                        easing.type: Easing.InOutQuad
                        duration: 200
                    }
                }

                Behavior on border.color {
                    ColorAnimation {
                        easing.type: Easing.InOutQuad
                        duration: 200
                    }
                }

                Item {
                    id: container

                    anchors.centerIn: indicatorRect

                    height: parent.height
                    width: parent.width

                    opacity: control.checked ? 1 : 0

                    property int thickness: materialUI.dp(4)

                    Behavior on opacity {
                        NumberAnimation {
                            easing.type: Easing.InOutQuad
                            duration: 200
                        }
                    }

                    Rectangle {
                        id: vert

                        anchors {
                            top: parent.top
                            right: parent.right
                            bottom: parent.bottom
                        }

                        radius: materialUI.dp(1)
                        color: control.darkBackground ? materialUI.light.textColor : materialUI.dark.textColor
                        width: container.thickness * 2

                    }
                    Rectangle {
                        anchors {
                            left: parent.left
                            right: vert.left
                            bottom: parent.bottom
                        }

                        radius: materialUI.dp(1)
                        color: control.darkBackground ? materialUI.light.textColor : materialUI.dark.textColor
                        height: container.thickness
                    }

                    transform: [
                        Scale {
                            origin { x: container.width / 2; y: container.height / 2 }
                            xScale: 0.5
                            yScale: 1
                        },
                        Rotation {
                            origin { x: container.width / 2; y: container.height / 2 }
                            angle: 45;
                        },
                        Scale {
                            id: widthScale

                            origin { x: container.width / 2; y: container.height / 2 }
                            xScale: control.checked ? 0.6 : 0.2
                            yScale: control.checked ? 0.6 : 0.2

                            Behavior on xScale {
                                NumberAnimation {
                                    easing.type: Easing.InOutQuad
                                    duration: 200
                                }
                            }

                            Behavior on yScale {
                                NumberAnimation {
                                    easing.type: Easing.InOutQuad
                                    duration: 200
                                }
                            }
                        },
                        Translate { y: -(container.height - (container.height * 0.9)) }
                    ]
                }
            }
        }
    }

    Ink {
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: materialUI.dp(54-48)/2
        }

        width: materialUI.dp(48)
        height: materialUI.dp(48)
        color: checkBox.checked ? materialUI.alpha(checkBox.color, 0.20)
                                : darkBackground ? Qt.rgba(1,1,1,0.1) : Qt.rgba(0,0,0,0.1)
        enabled: checkBox.enabled

        circular: true
        centered: true

        onClicked: checkBox.checked = !checkBox.checked
    }
}
