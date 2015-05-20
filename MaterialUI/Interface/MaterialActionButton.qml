/*
 * QML Material - An application framework implementing Material Design.
 * Copyright (C) 2015 Michael Spencer <sonrisesoftware@gmail.com>
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
import QtGraphicalEffects 1.0
import "../Element"

Controls.Button {
    id: button

    property color backgroundColor: materialUI.accentColor
    property color textColor: materialUI.lightDark(button.backgroundColor,
                                              materialUI.light.iconColor,
                                              materialUI.dark.iconColor)

    property string textFontFamily

    property int elevation: backgroundColor == "white" ? 0 : 1

    style: ControlStyles.ButtonStyle {
        background: Item {
            RectangularGlow {

                anchors.centerIn: parent
                anchors.verticalCenterOffset: elevation == 1 ? materialUI.dp(1.5)
                                                             : materialUI.dp(1)

                width: parent.width
                height: parent.height

                glowRadius: elevation == 1 ? materialUI.dp(0.75) : materialUI.dp(0.3)
                opacity: elevation == 1 ? 0.6 : 0.3
                spread: elevation == 1 ? 0.7 : 0.85
                color: "black"
                cornerRadius: height/2
            }

            View {
                anchors.fill: parent
                radius: width/2

                backgroundColor: button.backgroundColor

                tintColor: control.pressed ||
                           (control.focus && !button.elevation) ||
                           (control.hovered && !button.elevation) ?
                           Qt.rgba(0,0,0, control.pressed ? 0.1 : 0.05) : "transparent"

                Ink {
                    id: mouseArea
                    anchors.fill: parent
                    Connections {
                        target: control.__behavior
                        onPressed: mouseArea.onPressed(mouse)
                        onCanceled: mouseArea.onCanceled()
                        onReleased: mouseArea.onReleased(mouse)
                    }

                    circular: true
                }
            }
        }
        label: Text {
            font.pixelSize: 15
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: control.text
            font.family: textFontFamily
            color: textColor
        }
    }
}
