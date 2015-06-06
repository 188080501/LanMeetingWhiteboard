/*
 * QML Material - An application framework implementing Material Design.
 * Copyright (C) 2014-2015 Michael Spencer <sonrisesoftware@gmail.com>
 * Copyright (C) 2015 Bogdan Cuza <bogdan.cuza@hotmail.com>
 * Copyright (C) 2015 Mikhail Ivchenko <ematirov@gmail.com>
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
import QtQuick.Layouts 1.1
import "../Element"

PopupBase {
    id: dialog

    overlayLayer: "dialogOverlayLayer"
    overlayColor: Qt.rgba(0, 0, 0, 0.3)

    opacity: showing ? 1 : 0
    visible: opacity > 0

    width: Math.max(minimumWidth,
                    content.contentWidth + materialUI.dp(32))

    height: Math.min(materialUI.dp(350),
                     headerView.height + materialUI.dp(32) +
                     content.contentHeight +
                     content.topMargin +
                     content.bottomMargin +
                     buttonContainer.height)

    property int minimumWidth: materialUI.dp(270)

    property alias title: titleLabel.text
    property alias text: textLabel.text

    property string negativeButtonText: qsTr("Negative")
    property string positiveButtonText: qsTr("Positive")

    property bool hasActions: true

    default property alias dialogContent: column.data

    signal accepted()
    signal rejected()

    anchors {
        centerIn: parent
        verticalCenterOffset: (showing) ? (-30) : ((dialog.height / -3) - 20)

        Behavior on verticalCenterOffset {
            NumberAnimation { duration: 300; easing.type: Easing.OutQuad }
        }
    }

    Behavior on opacity {
        NumberAnimation { duration: 200 }
    }

    Keys.onEscapePressed: {
        dialog.close();
        rejected();
    }

    View {
        id: dialogContainer

        anchors.fill: parent
        elevation: 5
        radius: materialUI.dp(2)

        MouseArea {
            anchors.fill: parent
            propagateComposedEvents: false

            onClicked: {
                mouse.accepted = false
            }
        }

        Item {
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                topMargin: materialUI.dp(8)
            }

            clip: true
            height: headerView.height + materialUI.dp(32)

            View {
                backgroundColor: "white"
                elevation: content.atYBeginning ? 0 : 1
                fullWidth: true

                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                }

                height: headerView.height + materialUI.dp(16)
            }
        }

        Column {
            id: headerView

            spacing: materialUI.dp(16)

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top

                leftMargin: materialUI.dp(16)
                rightMargin: materialUI.dp(16)
                topMargin: materialUI.dp(16)
            }

            MaterialLabel {
                id: titleLabel

                width: parent.width
                wrapMode: Text.Wrap
                style: "title"
                visible: text != ""
            }

            MaterialLabel {
                id: textLabel

                width: parent.width
                wrapMode: Text.Wrap
                style: "dialog"
                visible: text != ""
            }
        }

        Flickable {
            id: content

            contentWidth: column.implicitWidth
            contentHeight: column.height
            clip: true

            anchors {
                left: parent.left
                right: parent.right
                top: headerView.bottom
                topMargin: materialUI.dp(8)
                bottomMargin: materialUI.dp(-8)
                bottom: buttonContainer.top
            }

            interactive: contentHeight + materialUI.dp(8) > height
            bottomMargin: hasActions ? 0 : materialUI.dp(8)

            Rectangle {
                width: content.width
                height: content.height
                color: "#ffffff"
            }

            Column {
                id: column
                anchors {
                    left: parent.left
                    margins: materialUI.dp(16)
                }

                width: content.width - materialUI.dp(32)
                spacing: materialUI.dp(16)
            }
        }

        Scrollbar {
            flickableItem: content
        }

        Item {
            id: buttonContainer

            anchors {
                bottomMargin: materialUI.dp(8)
                bottom: parent.bottom
                right: parent.right
                left: parent.left
            }

            height: hasActions ? buttonView.height + materialUI.dp(8) : 0
            clip: true

            View {
                id: buttonView

                height: hasActions ? positiveButton.implicitHeight + materialUI.dp(8) : 0
                visible: hasActions

                backgroundColor: "white"
                elevation: content.atYEnd ? 0 : 1
                fullWidth: true
                elevationInverted: true

                anchors {
                    bottom: parent.bottom
                    right: parent.right
                    left: parent.left
                }

                MaterialButton {
                    id: negativeButton

                    text: negativeButtonText
                    textColor: materialUI.accentColor
                    context: "dialog"
                    visible: negativeButtonText !== ""

                    anchors {
                        top: parent.top
                        right: positiveButton.left
                        topMargin: materialUI.dp(8)
                        rightMargin: materialUI.dp(8)
                    }

                    onClicked: {
                        dialog.close()
                        rejected();
                    }
                }

                MaterialButton {
                    id: positiveButton

                    text: positiveButtonText
                    textColor: materialUI.accentColor
                    context: "dialog"
                    visible: positiveButtonText !== ""

                    anchors {
                        top: parent.top
                        topMargin: materialUI.dp(8)
                        rightMargin: materialUI.dp(16)
                        right: parent.right
                    }

                    onClicked: {
                        dialog.close()
                        accepted();
                    }
                }
            }
        }
    }
}
