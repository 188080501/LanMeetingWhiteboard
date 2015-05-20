import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

Rectangle {
    id: mainRectangle

    property alias checkBoxText: text.text
    property bool checked: false

    width: 200
    height: 43
    color: "#ffffff"

    onCheckedChanged: {
        checkBox.checked = checked;
    }

    Rectangle {
        x: 15;
        width: parent.width - 15;
        height: 1;
        color: "#b2b2b2";
        anchors.verticalCenter: parent.bottom;
    }

    CheckBox {
        id: checkBox
        x: 120
        y: 0
        width: 70
        height: 43;
        anchors.verticalCenterOffset: 0
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        checked: mainRectangle.checked

        style: CheckBoxStyle {
            indicator: Rectangle {
                width: 70;
                height: 43;
                color: "#00000000";

                Image {
                    anchors.fill: parent;
                    fillMode: Image.PreserveAspectCrop
                    source: (checkBox.checked) ? ("qrc:/iOSStyleWidgetsResources/CheckBoxOn.png") : ("qrc:/iOSStyleWidgetsResources/CheckBoxOff.png");
                }
            }
        }

        onCheckedChanged: {
            parent.checked = checked;
        }
    }

    Text {
        id: text
        x: 20
        y: 0
        width: parent.width - checkBox.width - 30;
        height: 43;
        text: (checkBoxText) ? (checkBoxText) : (qsTr("UnDefined"));
        verticalAlignment: Text.AlignVCenter
        anchors.verticalCenter: parent.verticalCenter
        horizontalAlignment: Text.AlignLeft
        font.pixelSize: 17
    }
}
