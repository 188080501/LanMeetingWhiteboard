/*
 * QML Material - An application framework implementing Material Design.
 * Copyright (C) 2014 Michael Spencer
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

Controls.Button {
    id: button

    property int elevation
    property color backgroundColor: elevation > 0 ? "white" : "transparent"
    property color textColor: materialUI.lightDark(button.backgroundColor,
                                              materialUI.light.textColor,
                                              materialUI.dark.textColor)
    property string context: "default" // or "dialog" or "snackbar"

    style: ControlStyles.ButtonStyle {
        padding {
            left: 0
            right: 0
            top: 0
            bottom: 0
        }

        background: View {
            radius: materialUI.dp(2)

            elevation: {
                var elevation = button.elevation

                if (elevation > 0 && (control.focus || mouseArea.currentCircle))
                    elevation++;

                return elevation;
            }
            backgroundColor: button.backgroundColor

            tintColor: mouseArea.currentCircle || control.focus || control.hovered
                    ? Qt.rgba(0,0,0, mouseArea.currentCircle
                           ? 0.1 : button.elevation > 0 ? 0.03 : 0.05)
                    : "transparent"

            Ink {
                id: mouseArea

                anchors.fill: parent
                focused: control.focus && button.context != "dialog" && button.context != "snackbar"
                focusWidth: parent.width - materialUI.dp(30)
                focusColor: Qt.darker(button.backgroundColor, 1.05)

                Connections {
                    target: control.__behavior
                    onPressed: mouseArea.onPressed(mouse)
                    onCanceled: mouseArea.onCanceled()
                    onReleased: mouseArea.onReleased(mouse)
                }
            }
        }
        label: Item {
            implicitHeight: Math.max(materialUI.dp(36), label.height + materialUI.dp(16))
            implicitWidth: button.context == "dialog" ? Math.max(materialUI.dp(64), label.width + materialUI.dp(16))
                         : button.context == "snackbar" ? label.width + materialUI.dp(16)
                         : Math.max(materialUI.dp(88), label.width + materialUI.dp(32))

            Label {
                id: label
                anchors.centerIn: parent
                text: control.text
                color: button.textColor
            }
        }
    }
}
