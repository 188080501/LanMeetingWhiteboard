import QtQuick 2.2
import QtQuick.Layouts 1.1
import "../Element"

Item {
    id: field

    implicitHeight: hasHelperText ? helperTextLabel.y + helperTextLabel.height + materialUI.dp(4)
                                  : underline.y + materialUI.dp(8)
    implicitWidth: spinBoxContents.implicitWidth

    activeFocusOnTab: true

    property color accentColor: materialUI.accentColor
    property color errorColor: "#F44336"

    property alias model: listView.model
    readonly property string selectedText: listView.currentItem.text
    property alias selectedIndex: listView.currentIndex
    property int maxVisibleItems: 4

    property alias placeholderText: fieldPlaceholder.text
    property alias helperText: helperTextLabel.text

    property bool floatingLabel: false
    property bool hasError: false
    property bool hasHelperText: helperText.length > 0

    readonly property rect inputRect: Qt.rect(spinBox.x, spinBox.y, spinBox.width, spinBox.height)

    signal itemSelected(int index)

    Ink {
        anchors.fill: parent
        onClicked: {
            listView.positionViewAtIndex(listView.currentIndex, ListView.Center)
            var offset = listView.currentItem.itemLabel.mapToItem(menu, 0, 0)
            menu.open(label, 0, -offset.y)
        }
    }

    Item {
        z: 1
        id: spinBox

        height: materialUI.dp(24)
        width: parent.width

        y: {
            if(!floatingLabel)
                return materialUI.dp(16)
            if(floatingLabel && !hasHelperText)
                return materialUI.dp(40)
            return materialUI.dp(28)
        }

        RowLayout {
            id: spinBoxContents

            height: parent.height
            width: parent.width + materialUI.dp(5)

            MaterialLabel {
                id: label

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter

                text: listView.currentItem.text
                style: "subheading"
                elide: Text.ElideRight
            }
        }

        Dropdown {
            id: menu
            anchor: Item.TopLeft
            width: spinBox.width
            height: Math.min(maxVisibleItems*materialUI.dp(48) + materialUI.dp(24), listView.height)

            ListView {
                id: listView

                width: menu.width
                height: Math.min(count > 0 ? contentHeight : 0, 4.5 * materialUI.dp(48));

                interactive: true

                delegate: Standard {
                    id: delegateItem

                    text: modelData

                    onClicked: {
                        itemSelected(index)
                        listView.currentIndex = index
                        menu.close()
                    }
                }
            }

            Scrollbar {
                flickableItem: listView
            }
        }
    }

    MaterialLabel {
        id: fieldPlaceholder

        text: field.placeholderText
        visible: floatingLabel

        font.pixelSize: materialUI.dp(12)

        anchors.bottom: spinBox.top
        anchors.bottomMargin: materialUI.dp(8)

        color: materialUI.light.hintColor
    }

    Rectangle {
        id: underline

        color: field.hasError ? field.errorColor : field.activeFocus ? field.accentColor : materialUI.light.hintColor

        height: field.activeFocus ? materialUI.dp(2) : materialUI.dp(1)

        anchors {
            left: parent.left
            right: parent.right
            top: spinBox.bottom
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

        anchors {
            left: parent.left
            right: parent.right
            top: underline.top
            topMargin: materialUI.dp(4)
        }

        visible: hasHelperText
        font.pixelSize: materialUI.dp(12)
        color: field.hasError ? field.errorColor : Qt.darker(materialUI.light.hintColor)

        Behavior on color {
            ColorAnimation { duration: 200 }
        }
    }
}
