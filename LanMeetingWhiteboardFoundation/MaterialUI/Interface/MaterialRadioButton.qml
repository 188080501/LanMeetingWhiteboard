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
import QtQuick.Controls.Styles 1.2 as ControlStyles
import "../Element"

Controls.RadioButton {
    id: radioButton

    /*!
       The switch color. By default this is the app's accent color
     */
    property color color: darkBackground ? materialUI.dark.accentColor
                                         : materialUI.light.accentColor

    /*!
       Set to \c true if the switch is on a dark background
     */
    property bool darkBackground

    style: ControlStyles.RadioButtonStyle {
        label :Label {
            text: control.text
            color: control.enabled ? radioButton.darkBackground ? materialUI.dark.textColor
                                                                : materialUI.light.textColor
                                   : radioButton.darkBackground ? materialUI.alpha("#fff", 0.30)
                                                                : materialUI.alpha("#000", 0.26)
        }

        background: Rectangle {
            color: "transparent"
        }

        indicator: Rectangle {
            implicitWidth: materialUI.dp(48)
            implicitHeight: materialUI.dp(48)
            radius: implicitHeight / 2
            color: control.activeFocus ? materialUI.alpha(radioButton.color, 0.20) : "transparent"

            Rectangle {
                anchors.centerIn: parent
                implicitWidth: materialUI.dp(20)
                implicitHeight: materialUI.dp(20)
                radius: implicitHeight / 2
                color: "transparent"
                border.color: control.enabled
                    ? control.checked ? radioButton.color
                                      : radioButton.darkBackground ? materialUI.alpha("#fff", 0.70)
                                                                   : materialUI.alpha("#000", 0.54)
                    : radioButton.darkBackground ? materialUI.alpha("#fff", 0.30)
                                                 : materialUI.alpha("#000", 0.26)
                border.width: materialUI.dp(2)
                antialiasing: true

                Behavior on border.color {
                    ColorAnimation { duration: 200}
                }

                Rectangle {
                    anchors.centerIn: parent
                    implicitWidth: control.checked ? materialUI.dp(10) : 0
                    implicitHeight: control.checked ? materialUI.dp(10) : 0
                    color: control.enabled ? radioButton.color
                                           : radioButton.darkBackground ? materialUI.alpha("#fff", 0.30)
                                                                        : materialUI.alpha("#000", 0.26)
                    radius: implicitHeight / 2
                    antialiasing: true

                    Behavior on implicitWidth {
                        NumberAnimation { easing.type: Easing.InOutQuad; duration: 200 }
                    }

                    Behavior on implicitHeight {
                        NumberAnimation { easing.type: Easing.InOutQuad; duration: 200 }
                    }
                }
            }
        }

        spacing: 0
    }

    Ink {
        anchors {
            left: parent.left
            leftMargin: materialUI.dp(4)
            verticalCenter: parent.verticalCenter
        }

        width: materialUI.dp(40)
        height: materialUI.dp(40)
        color: radioButton.checked ? materialUI.alpha(radioButton.color, 0.20) : Qt.rgba(0,0,0,0.1)
        onClicked: radioButton.checked = !radioButton.checked

        circular: true
        centered: true
    }
}
