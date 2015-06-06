/*
 * QML Material - An application framework implementing Material Design.
 * Copyright (C) 2014 Bogdan Cuza <bogdan.cuza@hotmail.com>
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
import "../Element"

View {
    id: snackbar

    property string buttonText
    property color buttonColor: materialUI.accentColor
    property string text
    property bool opened
    property int duration: 2000
    property bool fullWidth: materialUI.isSmartPhone()

    signal clicked

    function open(text) {
        snackbar.text = text
        opened = true;
        timer.restart();
    }

    anchors {
        left: parent.left
        right: fullWidth ? parent.right : undefined
        bottom: parent.bottom
        leftMargin: fullWidth ? 0 : materialUI.dp(16)
        bottomMargin: opened ? fullWidth ? 0 : materialUI.dp(16) : -snackbar.height

        Behavior on bottomMargin {
            NumberAnimation { duration: 400; easing.type: Easing.OutQuad }
        }
    }
    radius: fullWidth ? 0 : materialUI.dp(2)
    backgroundColor: "#323232"
    height: materialUI.dp(48)
    width: fullWidth ? parent.width
                     : Math.min(Math.max(implicitWidth, materialUI.dp(288)), materialUI.dp(568))
    opacity: opened ? 1 : 0
    implicitWidth: buttonText == "" ? snackText.paintedWidth + materialUI.dp(48)
                                    : snackText.paintedWidth + materialUI.dp(72) + snackButton.width

    Timer {
        id: timer

        interval: snackbar.duration

        onTriggered: {
            if (!running) {
                snackbar.opened = false;
            }
        }
    }

    MaterialLabel {
        id: snackText
        anchors {
            right: snackbar.buttonText == "" ? parent.right : snackButton.left
            left: parent.left
            top: parent.top
            bottom: parent.bottom
            leftMargin: materialUI.dp(24)
            topMargin: materialUI.dp(16)
            rightMargin: materialUI.dp(24)
        }
        text: snackbar.text
        color: "white"
    }

    MaterialButton {
        id: snackButton
        opacity: snackbar.buttonText == "" ? 0 : 1
        textColor: snackbar.buttonColor
        text: snackbar.buttonText
        context: "snackbar"
        onClicked: snackbar.clicked()
        anchors {
            right: parent.right
            //left: snackText.right
            top: parent.top
            bottom: parent.bottom

            // Recommended button touch target is 36dp
            topMargin: materialUI.dp(6)
            bottomMargin: materialUI.dp(6)

            // Normal margin is 24dp, but button itself uses 8dp margins
            rightMargin: snackbar.buttonText == "" ? 0 : materialUI.dp(16)
        }
    }

    Behavior on opacity {
        NumberAnimation { duration: 300 }
    }
}
