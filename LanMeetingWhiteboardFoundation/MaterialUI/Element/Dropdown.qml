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

PopupBase {
    id: dropdown

    default property alias data: view.data
    property int anchor: Item.TopRight
    property alias internalView: view

    visible: view.opacity > 0

    function open(caller, offsetX, offsetY) {
        if(typeof offsetX === "undefined")
            offsetX = 0

        if(typeof offsetY === "undefined")
            offsetY = 0

        var position = caller.mapToItem(dropdown.parent, 0, 0)

        // Check to make sure we are within the window bounds, move if we need to
        var globalPos = caller.mapToItem(null, 0, 0)
        var root = materialUI.findRoot(dropdown)

        if (__internal.left) {
            dropdown.x = position.x
        } else if (__internal.center) {
            dropdown.x = caller.width / 2 - dropdown.width / 2
        } else {
            dropdown.x = position.x + caller.width - dropdown.width
        }

        if (__internal.top) {
            dropdown.y = position.y
        } else if (__internal.center) {
            dropdown.y = caller.height / 2 - dropdown.height / 2
        } else {
            dropdown.y = position.y + caller.height - dropdown.height
        }

        if(dropdown.y + height > root.height)
            offsetY = -((globalPos.y + height + materialUI.dp(16)) - root.height)
        if(dropdown.x + width > root.width)
            offsetX = -((globalPos.x + width + materialUI.dp(16)) - root.width)

        dropdown.x += offsetX
        dropdown.y += offsetY

        if(showing)
        {
            close();
        }
        else
        {
            showing = true
            opened()
        }
    }

    function close() {
        showing = false
    }

    QtObject {
        id: __internal

        property bool left: dropdown.anchor == Item.Left || dropdown.anchor == Item.TopLeft ||
        dropdown.anchor == Item.BottomLeft
        property bool right: dropdown.anchor == Item.Right || dropdown.anchor == Item.TopRight ||
        dropdown.anchor == Item.BottomRight
        property bool top: dropdown.anchor == Item.Top || dropdown.anchor == Item.TopLeft ||
        dropdown.anchor == Item.TopRight
        property bool bottom: dropdown.anchor == Item.Bottom ||
        dropdown.anchor == Item.BottomLeft ||
        dropdown.anchor == Item.BottomRight
        property bool center: dropdown.anchor == Item.Center
    }

    View {
        id: view
        elevation: 2
        radius: materialUI.dp(2)
        anchors.left: __internal.left ? parent.left : undefined
        anchors.right: __internal.right ? parent.right : undefined
        anchors.top: __internal.top ? parent.top : undefined
        anchors.bottom: __internal.bottom ? parent.bottom : undefined
        anchors.horizontalCenter: __internal.center ? parent.horizontalCenter : undefined
        anchors.verticalCenter: __internal.center ? parent.verticalCenter : undefined
    }

    state: showing ? "open" : "closed"

    states: [
        State {
            name: "closed"
            PropertyChanges {
                target: view
                opacity: 0
                width: 0
                height: 0
            }
        },

        State {
            name: "open"
            PropertyChanges {
                target: view
                opacity: 1
                width: dropdown.width
                height: dropdown.height
            }
        }
    ]

    transitions: [
        Transition {
            from: "open"
            to: "closed"

            NumberAnimation {
                target: internalView
                property: "opacity"
                duration: 400
                easing.type: Easing.InOutQuad
            }

            SequentialAnimation {

                PauseAnimation {
                    duration: 200
                }

                NumberAnimation {
                    target: internalView
                    property: "width"
                    duration: 200
                    easing.type: Easing.InOutQuad
                }
            }

            NumberAnimation {
                target: internalView
                property: "height"
                duration: 400
                easing.type: Easing.InOutQuad
            }
        },

        Transition {
            from: "closed"
            to: "open"

            NumberAnimation {
                target: internalView
                property: "opacity"
                duration: 400
                easing.type: Easing.InOutQuad
            }

            NumberAnimation {
                target: internalView
                property: "width"
                duration: 200
                easing.type: Easing.InOutQuad
            }

            NumberAnimation {
                target: internalView
                property: "height"
                duration: 400
                easing.type: Easing.InOutQuad
            }
        }
    ]
}
