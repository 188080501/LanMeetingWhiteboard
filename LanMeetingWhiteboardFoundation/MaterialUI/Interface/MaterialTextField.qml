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
import QtQuick 2.2
import QtQuick.Controls 1.3
import "../Element"

/*
 * Note that this is a FocusScope, not a TextInput. If you need to read or
 * write properties of the input itself, use the input property.
 */
FocusScope {
   id: field

   property color accentColor: materialUI.accentColor
   property color errorColor: "#F44336"

   property alias text: textInput.text

   property alias placeholderText: fieldPlaceholder.text
   property alias helperText: helperTextLabel.text

   readonly property int characterCount: text.length

   property bool floatingLabel: true
   property bool hasError: false
   property int characterLimit: -1

   readonly property rect inputRect: Qt.rect(textInput.x, textInput.y, textInput.width, textInput.height)

   readonly property alias input: textInput

   signal accepted()
   signal editingFinished()

   implicitHeight: __internal.showHelperText ? helperTextLabel.y + helperTextLabel.height + materialUI.dp(4)
                          : underline.y + materialUI.dp(8)
   width: materialUI.dp(200)

   QtObject {
       id: __internal

       property bool showHelperText: helperText.length > 0
       property bool showCharacterCounter: characterLimit > 0
   }

   MouseArea {
      anchors.fill: parent
      onClicked: field.forceActiveFocus(Qt.MouseFocusReason)
   }

   TextInput {
      id: textInput

      focus: true

      color: materialUI.light.textColor
      selectedTextColor: "white"
      selectionColor: Qt.darker(field.accentColor, 1)

//      selectByMouse: Device.type === Device.desktop
      selectByMouse: true

      activeFocusOnTab: true

      maximumLength: (characterLimit !== -1) ? (characterLimit) : (32767)
      width: parent.width

      clip: true

      y: {
         if( !floatingLabel )
            return materialUI.dp(16)
         if( floatingLabel && !__internal.showHelperText )
            return materialUI.dp(40)
         return materialUI.dp(28)
      }

      font {
         family: echoMode == TextInput.Password && field.text.length > 0 ? "" : "Roboto"
         pixelSize: materialUI.dp(16)
      }

      onAccepted: field.accepted()
      onEditingFinished: field.editingFinished()

      MouseArea {
          anchors.fill: parent
          cursorShape: Qt.IBeamCursor
          acceptedButtons: Qt.NoButton
      }
   }

   MaterialLabel {
      id: fieldPlaceholder

      text: field.placeholderText

      font.pixelSize: materialUI.dp(16)

      anchors.baseline: textInput.baseline
      anchors.bottomMargin: materialUI.dp(8)

      color: materialUI.light.hintColor

      states: [
         State {
            name: "floating"
            when: textInput.displayText.length > 0 && floatingLabel
            AnchorChanges {
               target: fieldPlaceholder
               anchors.baseline: undefined
               anchors.bottom: textInput.top
            }
            PropertyChanges {
               target: fieldPlaceholder
               font.pixelSize: materialUI.dp(12)
            }
         },
         State {
            name: "hidden"
            when: textInput.displayText.length > 0 && !floatingLabel
            PropertyChanges {
               target: fieldPlaceholder
               visible: false
            }
         }
      ]

      transitions: [
         Transition {
            id: floatingTransition
            enabled: false
            AnchorAnimation {
               duration: 200
            }
            NumberAnimation {
               duration: 200
               property: "font.pixelSize"
            }
         }
      ]

      Component.onCompleted: floatingTransition.enabled = true
   }

   Rectangle {
      id: underline
      color: field.hasError || (__internal.showCharacterCounter && field.characterCount > field.characterLimit)
             ? field.errorColor : field.activeFocus ? field.accentColor : materialUI.light.hintColor

      height: field.activeFocus ? materialUI.dp(2) : materialUI.dp(1)

      anchors {
         left: parent.left
         right: parent.right
         top: textInput.bottom
         topMargin: materialUI.dp(8)
      }

      Behavior on height {
         NumberAnimation { duration: 200 }
      }

      Behavior on color {
         ColorAnimation { duration: 200 }
      }
   }

   MaterialLabel {
      id: helperTextLabel
      visible: __internal.showHelperText
      font.pixelSize: materialUI.dp(12)
      color: field.hasError ? field.errorColor : Qt.darker(materialUI.light.hintColor)
      anchors {
         left: parent.left
         right: parent.right
         top: underline.top
         topMargin: materialUI.dp(4)
      }

      Behavior on color {
         ColorAnimation { duration: 200 }
      }
   }

   MaterialLabel {
      id: characterCounterLabel
      visible: __internal.showCharacterCounter
      font.pixelSize: materialUI.dp(12)
      font.weight: Font.Light
      color: field.characterCount <= field.characterLimit ? Qt.darker(materialUI.light.hintColor) : field.errorColor
      text: field.characterCount + " / " + field.characterLimit
      anchors {
         right: parent.right
         top: underline.top
         topMargin: materialUI.dp(8)
      }

      Behavior on color {
         ColorAnimation { duration: 200 }
      }
   }
}
